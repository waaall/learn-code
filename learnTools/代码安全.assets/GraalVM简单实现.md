
# 需求

- 对方客户机 继续用他们的标准 Java (HotSpot JVM)；
- 你提供的模块分两部分：
- 接口文件（Java class / jar，留在字节码形式，方便集成）
- 核心算法（你不想泄露源码，想编译成本地二进制）

这相当于在 Java 平台里嵌入一个 native library，然后用 JNI (Java Native Interface) 或 JNA 去调用。GraalVM Native Image 在这里不是用来编译整个应用，而是把 算法类编译成一个 native shared library (.so/.dll)，然后接口代码通过 JNI 调用。


# java源码保护方案简介

Java 的 .class / .jar 很容易反编译（JD-GUI、CFR、FernFlower 一键出源码），所以大家也会用类似 “Cython” 的手段来 混淆 / 编译成本地代码。

1. Java 混淆器（最常见手段）
    - [ProGuard](https://github.com/Guardsquare/proguard)（Android/Java 老牌工具）
    - R8（Google 的优化版，Android 默认用）
    - Allatori / Zelix KlassMaster / DashO（商业付费工具）

这些工具会：
    - 改类名、方法名（a() → x1()）
    - 删除调试信息、字符串加密
    - 控制反射行为


2. 编译成本地代码

Java 也有把字节码转换为机器码的方法，和 Cython 最接近的有：
    - [GraalVM Native Image](https://github.com/oracle/graal)
    - 直接把 Java 应用编译成原生二进制（Linux ELF / Windows exe）。
    - 编译后没有 .class 文件，逆向难度大幅增加。
    - 缺点：
    - 反射、动态加载支持有限，要显式配置。
    - 启动快，但运行时优化（JIT）不如传统 JVM。
    - Excelsior JET（已停止维护，但类似思路）
    - 把 Java 编译成原生可执行文件，保护源码。


	1.	最低限度：混淆（ProGuard / R8 / 商业混淆器）。
	2.	更强保护：用 GraalVM Native Image 编译成本地二进制。

# GraalVM Native Image 示例流程
- [GraalVM-install](https://www.graalvm.org/latest/getting-started/)
假设我们要保护的算法是一个简单的加减乘法类：

## 1. 目录结构

```bash
project/
│── src/
│   ├── api/
│   │   └── CalculatorAPI.java   # 对外接口
│   └── core/
│       └── CalculatorCore.java  # 核心算法（要编译成so）
│── build.sh                     # 构建脚本
```


## 2. 核心算法类（要保护的部分）

src/core/CalculatorCore.java

```java
package core;

public class CalculatorCore {
    public static int add(int a, int b) {
        return a + b;
    }

    public static int sub(int a, int b) {
        return a - b;
    }

    public static int mul(int a, int b) {
        return a * b;
    }
}
```


## 3. API 接口类（留给客户使用）

src/api/CalculatorAPI.java

```java
package api;

public class CalculatorAPI {

    // 声明 native 方法
    static {
        System.loadLibrary("calculatorcore"); // 加载 libcalculatorcore.so
    }

    public native int add(int a, int b);
    public native int sub(int a, int b);
    public native int mul(int a, int b);
}
```


## 4. 用 GraalVM 编译成共享库

GraalVM 提供 --shared 选项，可以把 Java 类编译成 .so/.dll，供 JVM 加载。

```bash
# 编译字节码
javac -d out src/core/CalculatorCore.java src/api/CalculatorAPI.java

# 用 GraalVM native-image 编译 core 算法类
native-image \
  --no-fallback \
  --shared \
  -H:Name=calculatorcore \
  -cp out core.CalculatorCore
```

产物：

calculatorcore.so   # Linux 动态库
calculatorcore.h    # JNI 头文件



## 5. 客户机运行

交付给客户的内容：
	•	api/CalculatorAPI.class （接口字节码）
	•	libcalculatorcore.so （你的核心算法，已编译）
	•	一个 demo 用法：

```java
import api.CalculatorAPI;

public class Demo {
    public static void main(String[] args) {
        CalculatorAPI calc = new CalculatorAPI();
        System.out.println("Add: " + calc.add(3, 2));
        System.out.println("Sub: " + calc.sub(3, 2));
        System.out.println("Mul: " + calc.mul(3, 2));
    }
}
```

运行：

```bash
javac -cp out Demo.java -d out
java -cp out -Djava.library.path=. Demo

# 输出：
Add: 5
Sub: 1
Mul: 6
```
 
## 6. 构建脚本示例（build.sh）

```bash
#!/bin/bash
set -e

# 清理
rm -rf out *.so *.h
mkdir -p out

# 编译 Java
javac -d out src/core/CalculatorCore.java src/api/CalculatorAPI.java

# 用 GraalVM native-image 生成共享库
native-image \
  --no-fallback \
  --shared \
  -H:Name=calculatorcore \
  -cp out core.CalculatorCore
```


执行：

```bash
chmod +x build.sh
./build.sh
```

## 总结

- 对客户交付：
    - 接口字节码（API 类 .class 或打成 api.jar）
    - 核心算法 .so (Linux) 或 .dll (Windows)
    - 客户集成：
    - 在 Java 代码里 System.loadLibrary("calculatorcore")
    - 使用 CalculatorAPI 提供的接口即可

这样客户只能看到 API class，核心算法逻辑在 .so 文件里，用反编译工具也看不到源码。


# GraalVM Native Image + Maven 项目 mvn 流程


## 1. 项目结构（Maven 标准目录）

```bash
java-native-demo/
│── pom.xml
│── src/
│   └── main/
│       ├── java/
│       │   ├── api/
│       │   │   └── CalculatorAPI.java
│       │   └── core/
│       │       └── CalculatorCore.java
│       └── resources/
│           └── META-INF/   # (可选)放一些配置
```


## 2. Java 代码

src/main/java/core/CalculatorCore.java

```java
package core;

public class CalculatorCore {
    public static int add(int a, int b) { return a + b; }
    public static int sub(int a, int b) { return a - b; }
    public static int mul(int a, int b) { return a * b; }
}

src/main/java/api/CalculatorAPI.java

package api;

public class CalculatorAPI {
    static {
        System.loadLibrary("calculatorcore"); // 对应 libcalculatorcore.so
    }

    public native int add(int a, int b);
    public native int sub(int a, int b);
    public native int mul(int a, int b);
}
```

## 3. pom.xml

这里有两个关键点：

1.	普通 Maven 编译 → 编译 Java 源码成 .class（API 和 Core 都编译）。
2.	GraalVM native-image 插件执行 → 只对 CalculatorCore 类编译成 .so 动态库。

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
                             http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.demo</groupId>
    <artifactId>java-native-demo</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>

    <name>Java Native Demo</name>

    <properties>
        <maven.compiler.source>17</maven.compiler.source>
        <maven.compiler.target>17</maven.compiler.target>
    </properties>

    <build>
        <plugins>
            <!-- 编译 Java 源码 -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.11.0</version>
                <configuration>
                    <source>17</source>
                    <target>17</target>
                </configuration>
            </plugin>

            <!-- 打包 jar -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-jar-plugin</artifactId>
                <version>3.3.0</version>
                <configuration>
                    <archive>
                        <manifest>
                            <mainClass>api.CalculatorAPI</mainClass>
                        </manifest>
                    </archive>
                </configuration>
            </plugin>

            <!-- 用 exec 插件调用 GraalVM native-image -->
            <plugin>
                <groupId>org.codehaus.mojo</groupId>
                <artifactId>exec-maven-plugin</artifactId>
                <version>3.1.0</version>
                <executions>
                    <execution>
                        <id>build-native-lib</id>
                        <phase>package</phase>
                        <goals>
                            <goal>exec</goal>
                        </goals>
                        <configuration>
                            <executable>native-image</executable>
                            <arguments>
                                <!-- 输出共享库 -->
                                <argument>--shared</argument>
                                <argument>--no-fallback</argument>
                                <argument>-H:Name=calculatorcore</argument>
                                <!-- 编译目标类 -->
                                <argument>-cp</argument>
                                <argument>${project.build.outputDirectory}</argument>
                                <argument>core.CalculatorCore</argument>
                            </arguments>
                        </configuration>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>
```


## 4. 构建

确保你用的是 GraalVM JDK（JAVA_HOME 指向 GraalVM，并安装了 native-image）：

```bash
export JAVA_HOME=/opt/graalvm
export PATH=$JAVA_HOME/bin:$PATH
gu install native-image
```

然后在项目根目录执行：

```bash
mvn clean package
```

输出结果：

    - target/java-native-demo-1.0-SNAPSHOT.jar
给客户的 API jar（包含 CalculatorAPI.class）。

    - calculatorcore.so
我们的核心算法动态库，客户机器必须放到 java.library.path。


## 5. 客户使用

示例 Demo.java：

```java
import api.CalculatorAPI;

public class Demo {
    public static void main(String[] args) {
        CalculatorAPI calc = new CalculatorAPI();
        System.out.println("Add: " + calc.add(3, 2));
        System.out.println("Sub: " + calc.sub(3, 2));
        System.out.println("Mul: " + calc.mul(3, 2));
    }
}
```


运行：

```bash
javac -cp target/java-native-demo-1.0-SNAPSHOT.jar Demo.java
java -cp .:target/java-native-demo-1.0-SNAPSHOT.jar -Djava.library.path=. Demo

# 输出
Add: 5
Sub: 1
Mul: 6
```

这样只需要 mvn package，就能同时生成：
    - API jar（可反编译，但只有接口声明，没有算法实现）
    - 核心算法 so（编译成 native ELF，难以反编译）
