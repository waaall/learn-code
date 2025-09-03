# learn Java

[toc]



## Java 环境配置

安装JDK：JDK、JRE、JVM的区别

> JDK：Java Development Kit
>  JRE顾名思义是java运行时环境，包含了java虚拟机，java基础类库。是使用java语言编写的程序运行所需要的软件环境，是提供给想运行java程序的用户使用的。
>  JDK顾名思义是java开发工具包，是程序员使用java语言编写java程序所需的开发工具包，是提供给程序员使用的。JDK包含了JRE，同时还包含了编译java源码的编译器javac，还包含了很多java程序调试和分析的工具：jconsole，jvisualvm等工具软件，还包含了java程序编写所需的文档和demo例子程序。
>  如果你需要运行java程序，只需安装JRE就可以了。如果你需要编写java程序，需要安装JDK。



现在安装JDK，会集成JRE运行时环境和类库，所以很多博客上的配置JAVA环境的方法已不再适用。

> 注：2020年，已经不需要再单独配置环境变量了，JRE环境也不要于JDK分离。
>
> JAVA_HOME、CLASSPATH、Path: 
>
> **JAVA_HOME就是为了方便后两个设置的别名（即安装所在文件夹，也就是bin、lib的上一级文件夹）**，而**CLASSPATH和Path分别是Java基础类库和可执行文件的位置。**而现阶段也没有必要设置类库的环境变量了（可能是编译器已经做了内部路径寻找的工作）。
>
> 而且，再安装时java编译器会将几个重要的可执行文件如Java、javac等连接到另一个文件夹中，然后将此路径加入Path，但是由于这些可执行文件不全，所以最好还是设置一个JAVA_HOME，然后将%JAVA_HOME%\bin加入Path（其实就没必要用它安装时设置的那个环境变量了）。

所以：
1. **删除**下载 JDK 时自动在 Path 中添加的变量
2. 设置一个`JAVA_HOME = %JAVA_HOME21%` (这一步是为了方便管理多个版本)
3. 设置一个`JAVA_HOME21 = 安装目录`
4. 将`%JAVA_HOME%\bin\`和`%JAVA_HOME%\jre\bin\` 加入Path


## JAVA与c++
- [我的java调用c++示例代码](https://github.com/waaall/cpp-template/tree/dynamic-lib/java)
- [Java使用jna调用c++动态库Linux](https://zhuanlan.zhihu.com/p/466863639)

```java
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import java.io.File;

/**
* JNA接口定义，用于调用C++动态库
*/

public interface AddLibJNA extends Library {
/**
* 获取动态库实例
* 使用相对路径加载动态库
*/

AddLibJNA INSTANCE = (AddLibJNA) Native.load(getLibraryPath(), AddLibJNA.class);

/**
* 获取动态库的相对路径
* @return 动态库路径
*/

static String getLibraryPath() {

// 获取当前工作目录
String currentDir = System.getProperty("user.dir");
String libPath;

if (Platform.isMac()) {
// macOS使用.dylib扩展名
libPath = currentDir + File.separator + "build" + File.separator + "libadd.dylib";
} else if (Platform.isLinux()) {

// Linux使用.so扩展名
libPath = currentDir + File.separator + "build" + File.separator + "libadd.so";
} else if (Platform.isWindows()) {

// Windows使用.dll扩展名
libPath = currentDir + File.separator + "build" + File.separator + "libadd.dll";
} else {
throw new UnsupportedOperationException("不支持的操作系统平台");
}

// 验证文件是否存在
File libFile = new File(libPath);
if (!libFile.exists()) {
throw new RuntimeException("动态库文件不存在: " + libPath);
}
return libPath;
}

/**
* 调用C++库中的add_int函数
* @param a 第一个整数
* @param b 第二个整数
* @return 两个整数的和
*/
int add_int(int a, int b);
}
```
