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
> 在2020年，UWP已经凉了，winforms比较老(但是比较简单，还是有应用价值)，WPF还算健壮，但是Microsoft又把重心放在了一个新的UI框架——[WinUI](https://docs.microsoft.com/zh-cn/windows/apps/winui/)。Microsoft摇摆不定的方向，真不让人放心。



Anyway，`.NET`框架看来是要统一了——.NET core 取代了 .NET Framework，也就是下面要讲的。



## 框架

`.NET`是个框架——也可以理解为类似于GCC编译器——它将`c#`语言解释为机器指令，并提供一些基本函数库、类库。有点像Apple的cocoa。

* [c#官方文档](https://docs.microsoft.com/zh-cn/dotnet/csharp/)
* [.NET官方文档](https://docs.microsoft.com/zh-cn/dotnet/core/introduction)
* [.NET开源项目](https://github.com/jasonhua95/awesome-dotnet-core)

### .NET混乱的版本

`.NET core` 和 `.NET framework` 是不同的两种实现，经过了几次“反转”，最终在2020年，Microsoft又又把重心放在了`.NET core`上。具体见下文中的引用。

这里先概述下`.NET core` 和 `.NET framework` 的区别与联系：

* .NET framework 是Windows针对c#（也有其他语言）的框架（编译器和系统函数、类库），但现在（2020年）凉了，微软停止支持，归并到.NET core 中。
* .NET core 是一个开源项目，意在将这套开发框架推向各个平台（操作系统），但是以前很多大的框架都不支持，比如WPF（UI库），但现在`.NET5.0`支持的比较好，也有较好的性能。

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/4/49/DotNet3.0.svg/300px-DotNet3.0.svg.png)

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

### [命令行]():



#### [c#编译](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/compiler-options/command-line-building-with-csc-exe#sample-command-lines-for-the-c-compiler)

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
  
  
#### [.NET core CLI](https://docs.microsoft.com/zh-cn/dotnet/core/tools/)

`.NET core CLI`就是命令行来管理.NET项目。`dotnet`指令创建，和单独几个c#文件的区别，就是项目和脚本的区别，两者没有本质区别，就像是cmake工具管理c++项目一样，这可以看成一个c#项目管理器，其调用csc（c#编译器）编译运行这些c#文件。

* 常用dotnet指令：

##### 创建

```shell

```



##### 编译

```shell

```



##### 发布

```shell

```



### [NuGet包管理器](https://docs.microsoft.com/zh-cn/nuget/consume-packages/install-use-packages-powershell)

正如pip和python的关系、npm和nodejs的关系、homebrew和Mac的关系，.NET也有自己的包管理器——Nuget。

NuGet与上述包管理器有一个很大的不同之处，就是它在不同“环境”下的指令是不一样的，这点的确让人困惑。

* [Visual Studio](https://docs.microsoft.com/zh-cn/nuget/consume-packages/install-use-packages-visual-studio)
* [dotnet](https://docs.microsoft.com/zh-cn/nuget/consume-packages/install-use-packages-dotnet-cli)
* [Nuget](https://docs.microsoft.com/zh-cn/nuget/consume-packages/install-use-packages-nuget-cli)
* [powershell](https://docs.microsoft.com/zh-cn/nuget/consume-packages/install-use-packages-powershell)

在dotnet、powershell和使用Nuget.exe本身，指令是“完全不一样”的，当然，VS中就是用GUI操作，肯定不一样可以理解。anyway，这就是.NET的包管理器。

有一点需要注意：`dotnet add` 和 `VS安装` 的package是在项目中的，而使用powershell和nuget是全局的（这个安装位置也需要配置），而除了nuget都是添加了引用的（也就是配置好了可以使用），具体说明见下面官方文档：

> 控制台中可用的全部操作也可以通过 [NuGet CLI](https://docs.microsoft.com/zh-cn/nuget/reference/nuget-exe-cli-reference) 完成。 但是，控制台命令在 Visual Studio 和已保存的项目/解决方案的上下文中运行，并且通常比其等效的 CLI 命令完成更多操作。 例如，通过控制台安装包会添加对项目的引用，而 CLI 命令则不会执行此操作。 因此，在 Visual Studio 中工作的开发人员通常更愿意使用控制台而不是 CLI。





#### NuGet安装位置

> [来自Microsoft官方文档](https://docs.microsoft.com/zh-cn/nuget/consume-packages/managing-the-global-packages-and-cache-folders)：
>
> 每当安装、更新或还原包时，NuGet 将管理项目结构多个文件夹之外的包和包信息：
>
> | “属性”                  | 说明和位置（每个用户）                                       |
> | :---------------------- | :----------------------------------------------------------- |
> | global‑packages         | global-packages 文件夹是 NuGet 安装任何下载包的位置。 每个包完全展开到匹配包标识符和版本号的子文件夹。 使用 [PackageReference](https://docs.microsoft.com/zh-cn/nuget/consume-packages/package-references-in-project-files) 格式的项目始终直接从该文件夹中使用包。 使用 [packages.config](https://docs.microsoft.com/zh-cn/nuget/reference/packages-config) 时，包将安装到 global-packages 文件夹，然后复制到项目的 `packages` 文件夹。 Windows：`%userprofile%\.nuget\packages`Mac/Linux：`~/.nuget/packages`使用 NUGET_PACKAGES 环境变量、`globalPackagesFolder` 或 `repositoryPath` [配置设置](https://docs.microsoft.com/zh-cn/nuget/reference/nuget-config-file#config-section)（分别在使用 PackageReference 和 `packages.config` 时）或 `RestorePackagesPath` MSBuild 属性（仅限 MSBuild）进行替代。 环境变量优先于配置设置。 |
> | http‑cache              | Visual Studio 包管理器 (NuGet 3.x+) 和 `dotnet` 工具存储此缓存中下载包的副本（另存为 `.dat` 文件），这些副本被组织到每个包源的子文件夹中。 未展开包，且缓存中有 30 分钟的到期时间。 Windows：`%localappdata%\NuGet\v3-cache`Mac/Linux：`~/.local/share/NuGet/v3-cache`使用 NUGET_HTTP_CACHE_PATH 环境变量替代。 |
> | temp                    | NuGet 在各操作期间在其中存储临时文件的文件夹。 Windows：`%temp%\NuGetScratch`Mac/Linux：`/tmp/NuGetScratch` |
> | plugins-cache **4.8 +** | NuGet 存储来自操作声明请求的结果的文件夹。 Windows：`%localappdata%\NuGet\plugins-cache`Mac/Linux：`~/.local/share/NuGet/plugins-cache`使用 NUGET_PLUGINS_CACHE_PATH 环境变量替代。 |



### GUI——Winform

虽然有些过时，但是Winform是Windows历史非常悠久的一个GUI框架（这恰恰说明了Microsoft前些年在干什么.......），这一节零散的讲讲具体一些常用控件的用法。



#### [DataGridView](https://docs.microsoft.com/zh-cn/dotnet/api/system.windows.forms.datagridview?view=net-5.0)

上来一个就是比较复杂的。提到DataGridView就不得不提[DataTable](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.datatable?view=net-5.0)。



### 数据库与Win app

数据库和app界面的绑定是多数软件都可以用得到的。而大多数平台都提供了通用或专用的数据库接口（类库）方便的使用，而对于Win app 访问 access数据库，则是Odbc。



##### access 与 winform——Odbc

一些小型数据和低访问量的情况，Windows应用使用access数据库也是不错的选择。先来看代码：

```c#
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
using System.Data;
using System.Data.Odbc; //调用了这个Odbc库
using System.Windows.Forms; //winform可视化库

namespace AWindowsFormsApp
{
    //这是一个主界面类，继承于System.Windows.Forms.Form
    public partial class MainForm : Form 
    {
        public MainForm() //构造函数
        {
            InitializeComponent();
        }
        
        // 初始化access database
        static string strCon = @"Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=YourPath\YourDataBaseName.accdb;";
        OdbcConnection con = new OdbcConnection(strCon); //连接对象
        
        //数据库绑定dataGridView控件
        private DataTable DataBinding(string tablename)
        {
            con.Open();
            string strSql = $"select * from {tablename}"; //sql语句作为字符串传入
            OdbcDataAdapter dadapter = new OdbcDataAdapter();
            dadapter.SelectCommand = new OdbcCommand(strSql, con); //
            DataTable table = new DataTable();
            dadapter.Fill(table);
            con.Close();
            this.dataGridView1.DataSource = table; //dataGridView是一个可视化控件，当然也是个类，所以这里的dataGridView1是一个对象
        }
        
        //执行上述函数，当然这一般会出现在ButtonClick函数中
        DataBinding("人员信息表");
    }
}
```



下面节选自[Odbc的微软官方文档](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc?view=dotnet-plat-ext-5.0)：

> ### System.Data.Odbc 命名空间
>
> [System.Data.Odbc](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc?view=dotnet-plat-ext-5.0) 命名空间为 ODBC .NET Framework 数据提供程序。
>
> #### 类
>
> | [OdbcCommand](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbccommand?view=dotnet-plat-ext-5.0) | 表示要对数据源执行的 SQL 语句或存储过程。 此类不能被继承。   |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | [OdbcCommandBuilder](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbccommandbuilder?view=dotnet-plat-ext-5.0) | 自动生成用于协调对 [DataSet](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.dataset?view=dotnet-plat-ext-5.0) 的更改与关联的数据源的单表命令。 此类不能被继承。 |
> | [OdbcConnection](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcconnection?view=dotnet-plat-ext-5.0) | 表示到数据源的连接是打开的。                                 |
> | [OdbcConnectionStringBuilder](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcconnectionstringbuilder?view=dotnet-plat-ext-5.0) | 为创建和管理由 [OdbcConnection](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcconnection?view=dotnet-plat-ext-5.0) 类使用的连接字符串的内容提供了一种简单方法。 |
> | [OdbcDataAdapter](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcdataadapter?view=dotnet-plat-ext-5.0) | 表示用于填充 [DataSet](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.dataset?view=dotnet-plat-ext-5.0) 和更新数据源的一组数据命令和一个数据源连接。 此类不能被继承。 |
> | [OdbcDataReader](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcdatareader?view=dotnet-plat-ext-5.0) | 提供从数据源读取数据行的只进流的方法。 此类不能被继承。      |
> | [OdbcError](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcerror?view=dotnet-plat-ext-5.0) | 收集与数据源返回的警告或错误有关的信息。                     |
> | [OdbcErrorCollection](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcerrorcollection?view=dotnet-plat-ext-5.0) | 收集 [OdbcDataAdapter](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcdataadapter?view=dotnet-plat-ext-5.0) 生成的所有错误。 此类不能被继承。 |
> | [OdbcException](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcexception?view=dotnet-plat-ext-5.0) | ODBC 数据源返回警告或错误时生成的异常。 此类不能被继承。     |
> | [OdbcFactory](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcfactory?view=dotnet-plat-ext-5.0) | 表示一组方法，这些方法用于创建 ODBC 提供程序实现数据源类的实例。 |
> | [OdbcInfoMessageEventArgs](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcinfomessageeventargs?view=dotnet-plat-ext-5.0) | 为 [InfoMessage](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcconnection.infomessage?view=dotnet-plat-ext-5.0) 事件提供数据。 |
> | [OdbcMetaDataCollectionNames](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcmetadatacollectionnames?view=dotnet-plat-ext-5.0) | 提供一个常数列表，该列表与 GetSchema 方法一起使用以检索元数据集合。 |
> | [OdbcMetaDataColumnNames](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcmetadatacolumnnames?view=dotnet-plat-ext-5.0) | 提供静态值，这些值用于 [OdbcMetaDataCollectionNames](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcmetadatacollectionnames?view=dotnet-plat-ext-5.0) 中包含的 [DataTable](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.datatable?view=dotnet-plat-ext-5.0) 对象的列名称。 [DataTable](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.datatable?view=dotnet-plat-ext-5.0) 由 GetSchema 方法创建。 |
> | [OdbcParameter](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcparameter?view=dotnet-plat-ext-5.0) | 表示 [OdbcCommand](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbccommand?view=dotnet-plat-ext-5.0) 的参数，还可以表示它到 [DataColumn](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.datacolumn?view=dotnet-plat-ext-5.0) 的映射。 此类不能被继承。 |
> | [OdbcParameterCollection](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcparametercollection?view=dotnet-plat-ext-5.0) | 表示与 [OdbcCommand](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbccommand?view=dotnet-plat-ext-5.0) 相关的参数的集合以及各个参数到 [DataSet](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.dataset?view=dotnet-plat-ext-5.0) 中列的映射。 此类不能被继承。 |
> | [OdbcPermission](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcpermission?view=dotnet-plat-ext-5.0) | 启用用于 ODBC 的 .NET Framework 数据提供程序，以帮助确保用户具有足够的安全级别来访问 ODBC 数据源。 此类不能被继承。 |
> | [OdbcPermissionAttribute](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcpermissionattribute?view=dotnet-plat-ext-5.0) | 将某安全操作与自定义安全特性相关联。                         |
> | [OdbcRowUpdatedEventArgs](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcrowupdatedeventargs?view=dotnet-plat-ext-5.0) | 为 [RowUpdated](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcdataadapter.rowupdated?view=dotnet-plat-ext-5.0) 事件提供数据。 |
> | [OdbcRowUpdatingEventArgs](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcrowupdatingeventargs?view=dotnet-plat-ext-5.0) | 为 [RowUpdating](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbcdataadapter.rowupdating?view=dotnet-plat-ext-5.0) 事件提供数据。 |
> | [OdbcTransaction](https://docs.microsoft.com/zh-cn/dotnet/api/system.data.odbc.odbctransaction?view=dotnet-plat-ext-5.0) | 表示要在数据源进行的 SQL 事务。 此类不能被继承。             |



上述是利用这个Odbc库来实现Sql语言访问数据库，其实针对.NET framework应用到access数据库，有“更简单”的可视化操作，但需要注意，目前下面**这个可视化操作的功能不支持.NET5.0，仅支持.NET framework**。

> ### [为 .accdb 文件创建数据集](https://docs.microsoft.com/zh-cn/visualstudio/data-tools/connect-to-data-in-an-access-database-windows-forms?view=vs-2019#create-a-dataset-for-an-accdb-file)
>
> 使用以下过程连接到使用 Microsoft 365、Access 2013、Access 2010 或 Access 2007 创建的数据库。
>
> 1. 在 Visual Studio 中打开 Windows 窗体或 WPF 应用程序项目。
>
> 2. 若要打开 " **数据源** " 窗口，请在 " **视图** " 菜单上选择 " **其他 Windows** > **数据源** "。
>
>    ![查看其他 Windows 数据源](https://docs.microsoft.com/zh-cn/visualstudio/data-tools/media/viewdatasources.png?view=vs-2019)
>
> 3. 在 **“数据源”** 窗口中，单击 **“添加新数据源”** 。
>
>    " **数据源配置向导** " 将打开。
>
> 4. 选择 " **选择数据源类型** " 页上的 " **数据库** "，然后选择 " **下一步** "。
>
> 5. 选择 " **选择数据库模型** " 页上的 " **数据集** "，然后选择 " **下一步** "。
>
> 6. 在“选择数据连接”页面上选择“新建连接”，配置一个新的数据连接。
>
>    随即会打开“添加连接”对话框。
>
> 7. 如果 **数据源** 未设置为 " **Microsoft Access 数据库文件** "，请选择 " **更改** " 按钮。
>
>    此时将打开 " **更改数据源** " 对话框。 在数据源列表中，选择 " **Microsoft Access 数据库文件** "。 在 " **数据访问接口** " 下拉 .NET Framework 中，选择 **OLE DB 的 "数据提供程序** "，然后选择 **"确定"** 。
>
> 8. 选择 " **数据库文件名** " 旁边的 " **浏览** "，然后导航到 *.accdb* 文件，然后选择 " **打开** "。
>
> 9. 输入用户名和密码 (如有必要) ，然后选择 **"确定"** 。
>
> 10. 选择 " **选择您的数据连接** " 页上的 " **下一步** "。
>
>     你可能会看到一个对话框，告知你当前项目中没有数据文件。 选择 **"是" 或 "** **否** "。
>
> 11. 在 "将 **连接字符串保存到应用程序配置文件** " 页上选择 " **下一步** "。
>
> 12. 在“选择数据库对象”页面上展开“表”节点。
>
> 13. 选择要包含在数据集中的表或视图，然后选择 " **完成** "。
>
>     数据集将添加到项目中，并且“数据源”窗口中将显示表和视图。








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

