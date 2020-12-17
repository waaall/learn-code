# learn .NET

[toc]

## 框架

`.NET`是个框架——也可以理解为类似于GCC编译器——它将`c#`语言解释为机器指令，并提供一些基本函数库、类库。

* [c#官方文档](https://docs.microsoft.com/zh-cn/dotnet/csharp/)
* [.NET官方文档](https://docs.microsoft.com/zh-cn/dotnet/core/introduction)

它（.NET）由微软开发，所以一般随Windows系统安装，对于Win10，一般安装在`C:\Windows\Microsoft.NET\`。我们打开`powershell`，进入下面这个文件夹：

```shell
➜  v4.0.30319 pwd
Path
----
C:\Windows\Microsoft.NET\Framework64\v4.0.30319

➜  v4.0.30319 where.exe csc
C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe

➜  v4.0.30319 csc
Microsoft (R) Visual C# Compiler version 4.8.4084.0
for C# 5
Copyright (C) Microsoft Corporation. All rights reserved.

This compiler is provided as part of the Microsoft (R) .NET Framework, but only supports language versions up to C# 5, which is no longer the latest version. For compilers that support newer versions of the C# programming language, see http://go.microsoft.com/fwlink/?LinkID=533240

warning CS2008: 未指定源文件
error CS1562: 必须为没有源的输出指定 /out 选项

➜  v4.0.30319
```

这个可执行文件`csc.exe`，就类似于`gcc`，将c#编译为机器指令，csc指令具体用法如下：（节选自微软官网文档）

### C# [编译器的示例命令行](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/compiler-options/command-line-building-with-csc-exe#sample-command-lines-for-the-c-compiler):

- 编译生成 File.exe 的 File.cs：

  ```shell
  csc File.cs
  ```

- 编译生成 File.dll 的 File.cs：

  ```shell
  csc -target:library File.cs
  ```

- 编译 File.cs 并创建 My.exe：

  ```shell
  csc -out:My.exe File.cs
  ```

- 编译当前目录中的所有 C# 文件，对其进行优化并定义 DEBUG 符号。 输出为 File2.exe：

  ```shell
  csc -define:DEBUG -optimize -out:File2.exe *.cs
  ```

- 编译当前目录中的所有 C# 文件，生成 File2.dll 的调试版本。 不显示徽标和警告：

  ```shell
  csc -target:library -out:File2.dll -warn:0 -nologo -debug *.cs
  ```

- 将当前目录中的所有 C# 文件编译为 Something.xyz (DLL)：

  ```shell
  csc -target:library -out:Something.xyz *.cs
  ```

