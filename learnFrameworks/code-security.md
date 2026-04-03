
# 总述

代码安全首先分为依赖安全和源码安全，也就是自己项目依赖的代码和自己项目的代码。然后代码按类型分有很多。这里不再赘述。

# 开源工具

## 通用

#### [Semgrep](https://github.com/semgrep/semgrep)

分为源码质量和供应链（依赖检查）。开源，多语言。
```sh
# For macOS
brew install semgrep

# For Ubuntu/WSL/Linux/macOS
python3 -m pip install semgrep

# 不登陆使用
semgrep scan --config "p/default" .

# 只扫某个目录
semgrep scan --config "p/default" src/
```

如果需要看 Web 界面、跑 CI，需要注册登陆使用 ci 指令，具体见上述官方仓库链接。

### 容器&文件检查

#### [Trivy](https://github.com/aquasecurity/trivy)

Targets (what Trivy can scan):
- Container Image
- Filesystem
- Git Repository (remote)
- Virtual Machine Image
- Kubernetes

Scanners (what Trivy can find there):
- OS packages and software dependencies in use (SBOM)
- Known vulnerabilities (CVEs)
- IaC issues and misconfigurations
- Sensitive information and secrets
- Software licenses

```sh
trivy fs .

# 只扫漏洞， 选项包括 vuln、misconfig、secret、license
trivy fs --scanners vuln .

# 只看高危
trivy fs --severity HIGH,CRITICAL .

# 扫容器镜像
trivy image python:3.12-slim
```

## python

### 源码检查

#### [Bandit](https://github.com/pycqa/bandit)

```sh
pip install bandit
bandit -r ./your_repos/project -f json -o bandit-report.json
```

### 依赖检查

#### [pip-audit](https://github.com/pypa/pip-audit)
```sh
pip install pip-audit
pip-audit -r requirements.txt
```
#### [safety](https://github.com/pyupio/safety)
```sh
pip install safety
safety scan --output json > safety-report.json
```

## java

### 源码检查

#### [spotbugs](https://github.com/spotbugs/spotbugs)

1. Maven 项目 里加上 spotbugs-maven-plugin
```xml
<build>
  <plugins>
    <plugin>
      <groupId>com.github.spotbugs</groupId>
      <artifactId>spotbugs-maven-plugin</artifactId>
      <version>4.9.8.3</version>
    </plugin>
  </plugins>
</build>
```

2. 运行
```sh
mvn spotbugs:spotbugs
```

### 依赖检查

#### [DependencyCheck](https://github.com/dependency-check/DependencyCheck)

1. Maven 项目加插件
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.owasp</groupId>
      <artifactId>dependency-check-maven</artifactId>
      <version>12.2.0</version>
    </plugin>
  </plugins>
</build>
```

2. 运行
```sh
mvn org.owasp:dependency-check-maven:check
mvn verify

# 多模块 Maven 项目
mvn org.owasp:dependency-check-maven:aggregate
```

## npm


### 源码检查

#### ESLint

```sh
# 安装
npm i -D eslint @eslint/js

# 初始化配置
npx eslint --init

# 执行检查
npx eslint .

# 自动修（慎用）
npx eslint . --fix
```

或者配合 package.json :
```json
{
  "scripts": {
    "lint": "eslint .",
    "lint:fix": "eslint . --fix"
  }
}
```

然后 npm 项目级检查
```sh
npm run lint
```

ESLint 本体只管通用规则。React、TypeScript 往往还要装对应插件或解析器。原则上是：
- JS 项目：eslint + @eslint/js
- TS 项目：再加 TypeScript ESLint 生态
- React 项目：再加 React 相关插件

### 依赖检查

#### npm audit
```sh
# 扫描依赖漏洞
npm audit

# 自动修复可安全升级的问题
npm audit fix
```

## C++


### 源码检查

#### [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

单文件:

```sh
clang-tidy src/foo.cpp -p build
clang-tidy src/foo.cpp -p build --fix
```

##### cmake 使用 clang-tidy

1. `.clang-tidy` 放在项目根目录
```yaml
Checks: >
  -*,
  clang-analyzer-*,
  bugprone-*,
  modernize-use-nullptr,
  modernize-use-override,
  performance-*
WarningsAsErrors: ''
HeaderFilterRegex: 'src/|include/'
FormatStyle: none
```

2. cmake
```cmake
cmake_minimum_required(VERSION 3.20)
project(myproj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 继续使用你自己的编译器，比如 GCC
# cmake -S . -B build -DCMAKE_CXX_COMPILER=g++

find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)

set(CMAKE_CXX_CLANG_TIDY
    ${CLANG_TIDY_EXE};
    -p=${CMAKE_BINARY_DIR}
)

add_executable(myproj
    src/main.cpp
    src/foo.cpp
)
target_include_directories(myproj PRIVATE include)
```

##### makefile 使用 clang-tidy

```makefile
CXX := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Iinclude
SOURCES := src/main.cpp src/foo.cpp

.PHONY: all tidy tidy-fix

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o app

tidy:
	clang-tidy $(SOURCES) -p .

tidy-fix:
	clang-tidy $(SOURCES) -p . --fix
```

### 依赖检查

C++ 因为没有官方的包管理器，所以往往都是自己管理项目依赖，使用 git submodule 等方式。即使第三方包管理器，依赖检查往往不如人意。

#### [conan](https://github.com/conan-io/conan)

conan作为包管理器。
```sh
conan audit provider auth conancenter --token=<token>
conan audit scan .
```