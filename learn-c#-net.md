# learn .NET

[toc]

首先需要说明：学`c#`、`.NET`、`Windows应用`开发不完全是一回事，但起码在2020年，几乎是一回事，就像是学习swift与学习IOS开发的关系。

一方面，大多数“主流”语言都是跨平台的，但另一方面，很多语言，都有个“爸爸”。比如swift就是Apple，而c#就是Microsoft，java就是Google（不是亲的），但Google这方面（跨平台）做的很好，所以Java不仅是Android开发，而c#和swift就没有这么好的“下场”了。

语言跨平台不难（对于大公司来说），但是我（码农）开发一个软件，并不是硬刚标准库的，有一个好的生态（也就是很多好用的库）就很关键，所以对于Swift，Apple用心开发了适用于IOS和Mac系统的很多好用的库，而这些，并不适用于其他操作系统，跨平台也就凉凉了，对于Microsoft的c#也是一样。

所以这还是那个问题：**App都是怎么写出来的——Windows的App都可以怎么写？**



## [Windows桌面应用](https://docs.microsoft.com/zh-cn/windows/apps/desktop/)开发

有 4 个主要平台可用于构建适合 Windows 电脑的桌面应用。 每个平台都提供用于定义应用生命周期的应用模型，用于创建 Word、Excel 和 Photoshop 等桌面应用的完整 UI 框架和一组 UI 控件以及用于使用 Windows 功能的一组全面的托管或本机 API。

要对这些平台进行深入比较并了解适合每个平台的其他资源，请参阅[选择应用平台](https://docs.microsoft.com/zh-cn/windows/apps/desktop/choose-your-platform)。

| 平台                                                         | 说明                                                         | 文档和资源                                                   |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [通用 Windows 平台 (UWP)](https://docs.microsoft.com/zh-cn/windows/uwp/) | 适合 Windows 10 应用和游戏的领先平台。 可构建仅使用 UWP 控件和 API 的 UWP 应用，也可在桌面应用中使用 UWP 控件和 API，这些应用根据设计可使用其他平台之一。 | [入门](https://docs.microsoft.com/zh-cn/windows/uwp/get-started/) [API 参考](https://docs.microsoft.com/zh-cn/uwp/) [示例](https://github.com/Microsoft/Windows-universal-samples) |
| [C++/Win32](https://docs.microsoft.com/zh-cn/windows/win32/) | 适合需要直接访问 Windows 和硬件的本机 Windows 应用的首选平台。 | [入门](https://docs.microsoft.com/zh-cn/windows/win32/desktop-programming/) [API 参考](https://docs.microsoft.com/zh-cn/windows/win32/apiindex/windows-api-list/) [示例](https://github.com/Microsoft/Windows-classic-samples) |
| [WPF](https://docs.microsoft.com/zh-cn/dotnet/framework/wpf/) | 已建立的基于 .NET 的平台，它适合带有 XAML UI 模型的图形丰富的托管 Windows 应用。 这些应用可面向 [.NET Core 3](https://docs.microsoft.com/zh-cn/dotnet/core/whats-new/dotnet-core-3-0) 或完整的 .NET Framework。 | [入门](https://docs.microsoft.com/zh-cn/dotnet/framework/wpf/getting-started/) [API 参考 (.NET)](https://docs.microsoft.com/zh-cn/dotnet/api/index) [示例](https://github.com/Microsoft/WPF-Samples) |
| [Windows 窗体（winforms）](https://docs.microsoft.com/zh-cn/dotnet/framework/winforms/) | 基于 .NET 的平台，它专用于具有轻量级 UI 模型的托管业务线应用。 这些应用可面向 [.NET Core 3](https://docs.microsoft.com/zh-cn/dotnet/core/whats-new/dotnet-core-3-0) 或完整的 .NET Framework。 |                                                              |



> 对于UI框架来讲：
>
> 在2020年，UWP已经凉了，winforms比较老，WPF还算健壮，但是Microsoft又把重心放在了一个新的UI框架——[WinUI](https://docs.microsoft.com/zh-cn/windows/apps/winui/)。Microsoft摇摆不定的方向，真不让人放心。



Anyway，`.NET`框架看来是要统一了——.NET core 取代了 .NET Framework，也就是下面要讲的。



## 框架

`.NET`是个框架——也可以理解为类似于GCC编译器——它将`c#`语言解释为机器指令，并提供一些基本函数库、类库。有点像Apple的cocoa。

* [c#官方文档](https://docs.microsoft.com/zh-cn/dotnet/csharp/)
* [.NET官方文档](https://docs.microsoft.com/zh-cn/dotnet/core/introduction)

### .NET混乱的版本

`.NET core` 和 `.NET framework` 是不同的两种实现，经过了几次“反转”，最终在2020年，Microsoft又又把重心放在了`.NET core`上。具体见下文中的引用。

这里先概述下`.NET core` 和 `.NET framework` 的区别与联系：

* .NET framework 是Windows针对c#（也有其他语言）的框架（编译器和系统函数、类库），但现在（2020年）凉了，微软停止支持，归并到.NET core 中。
* .NET core 是一个开源项目，意在将这套开发框架推向各个平台（操作系统），但是以前很多大的框架都不支持，比如WPF（UI库），但现在`.NET5.0`支持的比较好，也有较好的性能。



### .NET的迁移

> **.NET 实现**
>
> .NET 的每个实现都具有以下组件：
>
> - 一个或多个运行时。 示例：.NET Framework CLR、.NET 5 CLR。
> - 一个类库。 示例：.NET Framework 基类库、.NET 5 基类库。
> - 可选择包含一个或多个应用程序框架。 示例：[ASP.NET](https://www.asp.net/)、[Windows 窗体](https://docs.microsoft.com/zh-cn/dotnet/desktop/winforms/windows-forms-overview)和 [Windows Presentation Foundation (WPF)](https://docs.microsoft.com/zh-cn/dotnet/desktop/wpf/) 包含在 .NET Framework 和 .NET 5 中。
> - 可包含开发工具。 某些开发工具在多个实现之间共享。
>
> Microsoft 支持以下四种 .NET 实现：
>
> - .NET 5（和 .NET Core）及更高版本
> - .NET Framework
> - Mono
> - UWP
>
> .NET 5 现在是主要的实现，是正在进行的开发的重点。 .NET 5 是基于单个代码基底进行构建的，该代码基底支持多个平台和许多工作负载，例如 Windows 桌面应用和跨平台控制台应用、云服务和网站。
>
> [**为什么要从 .NET Framework 迁移**](https://docs.microsoft.com/zh-cn/dotnet/desktop/winforms/overview/?view=netdesktop-5.0#why-migrate-from-net-framework)
>
> .NET 5.0 的 Windows 窗体提供优于 .NET Framework 的新功能和增强功能。 有关详细信息，请参阅 [.NET 5 的 Windows 窗体中的新增功能](https://docs.microsoft.com/zh-cn/dotnet/desktop/winforms/whats-new/?view=netdesktop-5.0)。 要了解如何迁移应用，请参阅[如何将 Windows 窗体桌面应用迁移到 .NET 5](https://docs.microsoft.com/zh-cn/dotnet/desktop/winforms/migration/?view=netdesktop-5.0)。





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



## Detail

 

### 基本类型

就像我在`learn-swift`中所讲到——值类型与引用类型的问题，同样在c#中适用。

| **类别** |                    | **说明**                                          |
| :------- | :----------------- | :------------------------------------------------ |
| 值类型   | 简单类型           | 有符号的整型：`sbyte`、`short`、`int`、`long`     |
|          |                    | 无符号的整型：`byte`、`ushort`、`uint`、`ulong`   |
|          |                    | Unicode 字符：`char`                              |
|          |                    | IEEE 浮点：`float`、`double`                      |
|          |                    | 高精度小数：`decimal`                             |
|          |                    | 布尔：`bool`                                      |
|          | 枚举类型           | 格式为 `enum E {...}` 的用户定义类型              |
|          | 结构类型           | 格式为 `struct S {...}` 的用户定义类型            |
|          | 可以为 null 的类型 | 值为 `null` 的其他所有值类型的扩展                |
| 引用类型 | 类类型             | 其他所有类型的最终基类：`object`                  |
|          |                    | Unicode 字符串：`string`                          |
|          |                    | 格式为 `class C {...}` 的用户定义类型             |
|          | 接口类型           | 格式为 `interface I {...}` 的用户定义类型         |
|          | 数组类型           | 一维和多维，例如 `int[]` 和 `int[,]`              |
|          | 委托类型           | 格式为的用户定义的类型，例如`delegate int D(...)` |

