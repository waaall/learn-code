#  learn C++

[toc]

## Framework
---

### 上层与底层

你理解一个事物的更底层原理对理解它会有极其深刻的影响。这不仅对计算机适用，比如你学了量子力学，就理解了化学中元素周期表是怎么回事；你学了c，就理解了python是怎么实现的；你学了机械原理，也知道了汽车很多零部件功能是怎么实现的。这种互相联系，层层深入的学习思路是极其重要的。

最底层的计算机就是电信号，当然这个点信号在物理上解释起来也极其困难。所以学习计算机语言的关键就是当它是个傻子，傻到什么都不知道：比如你写一个a=1，你就要问：它怎么知道=？它怎么知道1？他又怎么知道a？

答案很简单：它当然不知道，它什么都不知道，问题是，你怎么知道你打的那个符号就是=，你只不过按下了键盘上的一个键，然后这个键帽底下连了一根线，通了电，一路到了内存，CPU。然后你得说这是一个=，那就是咯，但是计算机什么都不知道，如果理解了这一点，学计算机只是时间问题了。

正如学习python我们要问：a = 1，它（解释器/编译器）怎么知道我写的这个是整数或小数呢？我们在c++里就会发现，它不知道，只不过我们写了函数模版，看起来就自动识别类型了。

c++的底层呢？当然就是c的底层，也就是汇编语言，`g++ -S main.cpp` 就可以把一个c++代码文件编译成汇编文件，看起来的for循环，根本没有任何智能，只是编译器为我们做了这些“把某个数从内存取出来放入某个寄存器，再加减，再压入栈”这些重新又繁琐的工作。而繁琐也是相对的，否则没有会放弃写c++代码去玩游戏（很多大型游戏底层都有c++的身影）。对于c++和汇编的关系，**《Computer System：A Programer‘s Perspective》**这本书描述的极为清晰。

### 面向对象？可没有这么‘玄’

Python 是C 写的，这其中就有一个**关键的问题**：c语言并不是面向对象，也就是没有类和对象这些定义，那怎么解释python中的面向对象部分呢？

答案也很简单：c++的面向对象是怎么来的？其实这只是计算机语言体系众多抽象概念的一个罢了——函数是什么？编译器或者解释器怎么解释？list是什么？string是什么？那么类就是什么！也就是一部分代码的集合。

对于机器来说，没有函数和类这些概念效率会更高，而对于人类，一段机器码（或者c语言中变量声明，赋值，循环，）实现了一个功能，如果把它打包做一个标签就是再好不过的了，这就是函数，这也是类。这个包。就是申请的一段连续内存（当然虚拟内存技术先忽略）

举个例子：一辆汽车，大家可能都想象得到是什么，但其实没几个人懂，你知道汽车怎么实现转向的吗？恐怕不知道，但甚至没有驾照的人都会转动方向盘，这个方向盘，就是函数，这个车就是类。这就是为什么大多数人不懂的车的工作原理（不知道类的所有原代码），但却能开车（可以进行类的实例化），也能转弯（调用类地下的函数/方法），而且换一个车基本还能开（类的继承）

### list，array or vector？

刚开始接触c语言或者python等其他，就会想，list这么着重讲有什么用？其实几乎所有数据都是一个list，甚至包括string，所谓list，就是一组顺序不可以更改的二进制数，然后按照特殊的人为规定的规则编码成不同的格式，比如字符串，图片，音频，视频，实质上对于计算机来说就是list。所以这种数据格式看起简单，但却像二进制一样意义深远。

而在c++中，verctor就是自动用new和delete来创建的动态数组，具体见下文中讲解

### 文件

读写文件的函数（不管是Python还是C++），我们发现它是无法直接内容前追加新内容的，why？

如果底层可以实现，这个write函数必然早就更新了（可以直接追加内容），我们进一步站在硬盘及文件系统的角度，想象下为什么这个看似“很人性化很简单”的操作却不可以实现：

首先硬盘就是一系列可以和光或电磁反应产生“相变”的“微观”材料，组成了“位”。而文件系统就是把这些位解释为一个个文件和文件里的内容。而这些看起来可以随意操作，随意更改的内容，在底层则必须为顺序的，严格按照一定格式的位的集合。（乱序储存也是用标记，来让其“变成”顺序读写的）。

PS：文件内容，完全是字符串编码。也就意味着：你看似写了一个Python或C++文件，里面定义了一个“数组”或“整数”，但这是编译器把这些“字符串”解释为“数组”或“整数”而已。在文件中，它们就只是字符串。

比如一个文件，在硬盘里就是一段连续的位。而位的开始某些位（确定长度或者用特殊标记字符来指定始末）就分别代指文件名，文件位置，文件类型，访问权限等一系列信息，而“紧接着”就是文件内容，所以，如何才能在文件内容开始插入一段新内容，却不改变原始内容呢？这非常难，难到现在都没有被解决。

所以现在看到的：我们在任何可以写入的app中，比如Office，记事本，Markdown编辑器等等，只要进行文件读写的，我们好像都可以随意在任意位置插入一段数据，而原始数据保持不变，但在底层，这些文件的更改不是这么简单的（哪怕你在文件开头只插入了一个字符，保存时都需要把之后的全部内容先读取到内存里，再写入才行）

PS：还有一个问题，就是文件怎么“放”，只有理解了这个问题，我们才能更好的理解为什么文件不可以不被改变的插入新数据，也能了解什么叫碎片化。

![Screen Shot 2020-07-08 at 17.37.21](learn c++.assets/Screen Shot 2020-07-08 at 17.37.21.png)

现代文件系统（操作系统的一部分）是将文件抽象的分成N个Block，新文件先分配一个Block，然后再从头开始写，写满了之后再写的话，文件系统会再分配一个块，这就说明了，为什么不能“插入”，而能追加（append）。而这同时，造成了文件碎片化，这大大拖慢了文件读写速度，怎么解决呢？

操作系统会在空闲时移动（也就是给一个文件分配相邻块，重新读写）文件来处理碎片化：

![Screen Shot 2020-07-08 at 17.48.32](learn-c++.assets\Screen Shot 2020-07-08 at 17.48.32.png)

需要注意的是：尤其是对于固态硬盘，在硬件层次还是有很多问题需要处理（机械硬盘也是，但固态在分配块来均衡块寿命等），这部分不属于文件系统的工作，而是由驱动程序负责，且有专门的硬件来负责———主控芯片

### c++ 各种声明？

python 和 c++有很多区别，其中一个重要区别就是不管是变量，数组，还是函数，python都不需要声明再定义，且不用指定类型，看起来非常人性化，事实也是如此，但没这么简单：

首先c++在很多情况也可以“智能”判断，但是首先要知道，机器是不会给你“预先判断的”，所谓预先判断都是编译器的算法而已，而在使用角度肯定是越“人性化”越好，但是这样你为什么要学习代码呢？直接在屏幕上划一划不就好了，而实现这些“人性化”功能的工作总要有人做，而作为一个想了解计算机的人，这也是必经之路。

再一点就是声明也有声明的好处，比如你有些量想定义浮点值（2.0）但写成了2，这时候python不会告诉你的，而如果在c++里你声明过就没有问题。当然了，在很多情况，python是不可或缺的，因为它实现一个类似的功能，其代码要简单很多，这也是python的“占有率”这么高的原因之一。

**总结：我想要说的还是那个结论，就是理解一个事物，必须联系的看待，即理解底层，又了解上层，就非常清晰了。**

### 运算符，函数，类，库，模块？链接？

Q：new 是一个运算符，而malloc( )是一个库函数，这两者有什么区别呢？



### 标准更新与编译器

[c++20/c++17/c++14/c++11“新”特性](https://zh.cppreference.com/w/cpp/compiler_support#cpp2a)

整个计算机行业还是在“高速”发展的，尤其是和传统行业相比，学习十年前甚至几十年前的东西，当然会对理解行业底层，了解行业发展有些至关重要的帮助，但是如果要应用到当今社会，了解“更新”也是必须的，c++也是如此，常用书籍如c++ primer只是更新到c++11，而在2020年学习这些，c++20的标准已经落地了，c++14和c++17也有不少小的改动，包括对一些指令的弃用，增加和完善了一些新的功能，而这些细节虽然对整体框架没多少影响，但随着发布版本积累，积少成多的更新会影响代码结构的改变。所谓更新，就是编译器想帮你做更多事情，比如类型的自动选择，比如数组的动态长度，当然同时让代码更精简，综上所述：学习框架不分新旧，但是应用还是要了解更新，用好更新就是再利用别人（编译器）不断更新的工具，这样才能更快更好的做出自己的工具。

编译器就是对标准的“践行”，比如apple用的“自己”开发的apple clang & llvm编译器，目前2020年更新到11.0.3版本，实现了对c++17特性的全部支持和c++20特性的部分支持。[apple develop链接](https://developer.apple.com/videos/play/wwdc2019/409/)

[编译全过程](https://www.processon.com/view/link/5c70f497e4b0feef9f352ea2#map)。

![Image00035](learn-c++.assets\Image00035.jpg)

### c++编译指令

[g++ 指令](https://www.cnblogs.com/yyehl/p/6862153.html) 
```bash
g++ -E test.cpp (-o test.i)#预处理, -o是指定输出文件名
g++ -S test.i (-o bala.s) #编译（生成的就是汇编文件）
g++ -c test.s -o balabala.o #汇编，生成的为二进制文件
g++ test.o #链接，就是把相关的#include文件链接起来，生成a.out文件

#单文件直接生成可执行文件
g++ test.cpp
#多文件直接生成可执行文件
g++ test1.cpp test2.cpp

gcc -Og -S -masm=intel mstore.c #预处理+编译，且汇编语言为intel格式标准，-Og表示不进行优化
gcc -fPIC -shared great_module.c -o great_module.so -I/usr/include/python2.7/ -lpython2.7 #编译c++用的python函数

##选项
-E 			#只运行 C 预编译器。
-S			#预处理+编译，生成汇编
-c			#汇编，生成二进制文件，但不链接
-o			#指定生成二进制文件名

-g 			#生成调试信息。GNU 调试器可利用该信息。
-I(DIRECTORY) #指定额外的头文件搜索路径DIRECTORY。
-L(DIRECTORY) #指定额外的函数库搜索路径DIRECTORY。
-l(LIBRARY) #连接时搜索指定的函数库LIBRARY。
-lm			#链接数学库，由于种种历史原因，include数学库时还需要手动链接

-m486 		#针对 486 进行代码优化。
-o 			#FILE 生成指定的输出文件。用在生成可执行文件时。
-O0 		#不进行优化处理。同-Og？
-O 			#或 -O1 优化生成代码。
-O2 		#进一步优化。
-O3 		#比 -O2 更进一步优化，包括 inline 函数。
-std=c++17 	#使用c++17标准
-fPIC       #编译库文件时，
-shared 	#生成共享目标文件。通常用在建立共享库时。
-static 	#禁止使用共享连接。
-UMACRO 	#取消对 MACRO 宏的定义。
-w 			#不生成任何警告信息。
-Wall 		#生成所有警告信息。
```

### c++的标准库头文件都在哪？

比如下面代码，常用include<...>，这些往往是标准库的头文件，他们在哪里呢？
* Linux系统中，一般在usr/local/include，当然这主要取决去C_INCLUDE_PATH这个环境变量设置的路径。
* 在MAC系统中，这些常用的编译器被XCODE“接管”了，一般在`/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include`



```c++
#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)				/* child */
		exit(7);
}
```


## C++环境配置

* vscode c++[配置](https://code.visualstudio.com/docs/cpp/config-clang-mac)、

1.  **create a ``tasks.json`` file** 

    -------in the `.vscode` folder and open it in the editor. Replace the contents of that file with the following:

```json
{
  // See https://go.microsoft.com/fwlink/?LinkId=733558
  // for the documentation about the tasks.json format
  "version": "2.0.0",
  "tasks": [
    {
      "type": "shell",
      "label": "clang++ build active file",
      "command": "/usr/bin/clang++",
      "args": [
        "-std=c++17",
        "-stdlib=libc++",
        "-g",
        "${file}",
        "-o",
        "${fileDirname}/${fileBasenameNoExtension}"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

**当我们不debug，只想一键编译执行时，tasks.json 文件应该如下**

```json
{
	"version": "2.0.0",
	"tasks": [
	  {
		"type": "process",
		"label": "build",
		"command": "/usr/bin/g++",
		"args": [
		  "-std=c++17",
		  "-stdlib=libc++",
		  "-g",
		  "-Wall",
		  "-lm",
		  "${file}",
		  "-o",
		  "${fileDirname}/${fileBasenameNoExtension}.o"
		],
		"presentation": {
			"echo": false,
			"reveal": "silent",
			"showReuseMessage": false
		},
		"options": {
		  "cwd": "${workspaceFolder}"
		},
		"problemMatcher": ["$gcc"],
		"group": {
		  "kind": "build",
		  "isDefault": true
		}
	  },
	  {
		"label": "run",
		"type": "shell",
		"dependsOn": "build",
		"command": "${fileDirname}/${fileBasenameNoExtension}.o",
		"presentation": {
		  "focus": true
		},
		"group": {
		  "kind": "test",
		  "isDefault": true
		}
	  }
	]
  }
```

这样我们就可以通过 vscode 的指令`task: run test task` 一键编译执行c++文件！

2. **creates a `launch.json` file** 

------From the main menu, choose **Run** > **Add Configuration...** and then choose **C++ (GDB/LLDB)**. opens it in the editor, and builds and runs 'helloworld'. Your `launch.json` file will look something like this:

```json
{
  // Use IntelliSense to learn about possible attributes.
  // Hover to view descriptions of existing attributes.
  // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
  "version": "0.2.0",
  "configurations": [
    {
      "name": "clang++ - Build and debug active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}/${fileBasenameNoExtension}",
      "args": [],
      "stopAtEntry": true,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "lldb",
      "preLaunchTask": "clang++ build active file"
    }
  ]
}
```

3. **creat a `c_cpp_properties.json` file**

    ----- View the C/C++ configuration UI by running the command **C/C++: Edit Configurations (UI)** from the Command Palette (⇧⌘P).

```json
{
  "configurations": [
  {
    "name": "Mac",
    "includePath": [
    "${workspaceFolder}/**",
    "/Library/Developer/CommandLineTools/usr/include/c++/v1",
    "/usr/local/include",
    "/Library/Developer/CommandLineTools/usr/include"
    ],
    "defines": [],
    "macFrameworkPath": [
    "/System/Library/Frameworks",
    "/Library/Frameworks"
    ],
    "compilerPath": "/usr/bin/clang",
    "cStandard": "c11",
    "cppStandard": "c++17",
    "intelliSenseMode": "clang-x64"
  }
  ],
  "version": 4
}

```



### windows的c++编译器

#### GNU（mingw64）

* 安装mingw-[w64](http://mingw-w64.org/)、
* [一个傻瓜式安装mingw64的博客](https://www.jianshu.com/p/d66c2f2e3537)、

```bash
# 这显示mingw-c的库搜索路径
gcc -print-search-dirs
```

![库变量](C:\Users\zxll\iCloudDrive\Desktop\write\markdown\learn code\learn c++.assets\库变量.png)

上图所示，我们创建几个环境变量，定义“用户”的库文件和头文件搜索地址：

* C_INCLUDE_PATH
* CPLUS_INCLUDE_PATH
* LIBRARY_PATH
* LD_LIBRARY_PATH

上述四个变量分别指定了C/C++头文件、静态库、动态库搜索地址。但要注意的是，这只是让编译器（mingw）找到它们，然后成功编译，但是若有动态库（因为动态库是运行是链接的），还需要“告诉”操作系统动态库的位置，即添加动态库路径。当然，你也可以把你的动态库放入默认的动态库文件夹中，64位动态库默认在`C:\Windows\System32`中。但我并不推荐，这样不利于管理，系统升级后也可能失效之类的。所以把这个路径加入环境变量Path，系统会找到。



#### Clang+LLVM

* 官网有window[编译版本](https://releases.llvm.org/download.html)、

但可能会出问题：这是个半成品，GCC的库（包括头文件和动静态库都没有），它默认去找VS版本，我们需要让它找mingw版本。

https://zhuanlan.zhihu.com/p/34520938

```bash
clang++ -O3 -target x86_64-pc-windows-gnu for.cpp -o for.exe
```



#### 安装msys2

* [msys2](https://www.msys2.org/)、

msys2和mingw的关系见下文节选知乎的片段，这里简要说明一下：

msys就像是“app store”，用来在windows系统 下载安装配置 一系列linux系统的开发套件，最核心套件就是mingw，也就是GCC编译器的windows实现。

所以如上述情况，已经安装了mingw-w64，则安装msys2后可以选择把mingw64文件夹剪切到`msys64\mingw64`文件夹中。（环境变量也要改过来）

[一个msys2详细的安装博客](https://hustlei.github.io/2018/11/msys2-for-win.html)、

#### mingw、msys、cygwin？
> [一个知乎回答](https://www.zhihu.com/question/22137175)



#### make
* [Makefile](https://seisman.github.io/how-to-write-makefile/introduction.html):

一般有一定规模的软件不会自己手动在命令行编译，这时候make工具包（也是一个软件）应运而生。也就是类似一个shell脚本，执行一系列的编译文件。

但但但是！这也太麻烦了，中大型项目根本不可能自己完成makefile，所以，[autotools](http://inti.sourceforge.net/tutorial/libinti/autotoolsproject.html)应运而生，与此“竞争”的是cmake工具包，总之他们两个是目前（2020年）主流的c系语言大型软件编译的两大解决方案。



#### cmake
* [cmake](https://www.hahack.com/codes/cmake/)：

##### 什么是 CMake

> All problems in computer science can be solved by another level of indirection. 
>
> **David Wheeler**

你或许听过好几种 Make 工具，例如 [GNU Make](https://www.hahack.com/wiki/tools-makefile.html) ，QT 的 [qmake](http://qt-project.org/doc/qt-4.8/qmake-manual.html) ，微软的 [MS nmake](http://msdn.microsoft.com/en-us/library/ms930369.aspx)，BSD Make（[pmake](http://www.freebsd.org/doc/en/books/pmake/)），[Makepp](http://makepp.sourceforge.net/)，等等。这些 Make 工具遵循着不同的规范和标准，所执行的 Makefile 格式也千差万别。这样就带来了一个严峻的问题：如果软件想跨平台，必须要保证能够在不同平台编译。而如果使用上面的 Make 工具，就得为每一种标准写一次 Makefile ，这将是一件让人抓狂的工作。

CMake 就是针对上面问题所设计的工具：它首先允许开发者编写一种平台无关的 CMakeList.txt 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件，如 Unix 的 Makefile 或 Windows 的 Visual Studio 工程。从而做到“Write once, run everywhere”。显然，CMake 是一个比上述几种 make 更高级的编译配置工具。一些使用 CMake 作为项目架构系统的知名开源项目有 [VTK](http://www.vtk.org/)、[ITK](http://www.itk.org/)、[KDE](http://kde.org/)、[OpenCV](http://www.opencv.org.cn/opencvdoc/2.3.2/html/modules/core/doc/intro.html)、[OSG](http://www.openscenegraph.org/) 等 [[1\]](https://www.hahack.com/codes/cmake/#fn1)。

在 linux 平台下使用 CMake 生成 Makefile 并编译的流程如下：

1. 编写 CMake 配置文件 CMakeLists.txt 。
2. 执行命令 `cmake PATH` 或者 `ccmake PATH` 生成 Makefile（`ccmake` 和 `cmake` 的区别在于前者提供了一个交互式的界面）。其中， `PATH` 是 CMakeLists.txt 所在的目录。
3. 使用 `make` 命令进行编译。

本文将从实例入手，一步步讲解 CMake 的常见用法，文中所有的实例代码可以在[这里](https://github.com/wzpan/cmake-demo)找到。如果你读完仍觉得意犹未尽，可以继续学习我在文章末尾提供的其他资源。

##### 单个源文件


对于简单的项目，只需要写几行代码就可以了。例如，假设现在我们的项目中只有一个源文件 [main.cc](http://main.cc/) ，该程序的用途是计算一个数的指数幂。

```c++
#include <stdio.h>
#include <stdlib.h>

/**
 * power - Calculate the power of number.
 * @param base: Base value.
 * @param exponent: Exponent value.
 *
 * @return base raised to the power exponent.
 */
double power(double base, int exponent)
{
    int result = base;
    int i;
    
    if (exponent == 0) {
        return 1;
    }
    
    for(i = 1; i < exponent; ++i){
        result = result * base;
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 3){
        printf("Usage: %s base exponent \n", argv[0]);
        return 1;
    }
    double base = atof(argv[1]);
    int exponent = atoi(argv[2]);
    double result = power(base, exponent);
    printf("%g ^ %d is %g\n", base, exponent, result);
    return 0;
}
```

* 编写 CMakeLists.txt

首先编写 CMakeLists.txt 文件，并保存在与 [main.cc](http://main.cc/) 源文件同个目录下：

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo1)

# 指定生成目标
add_executable(Demo main.cc)
```

CMakeLists.txt 的语法比较简单，由命令、注释和空格组成，其中命令是不区分大小写的。符号 `#` 后面的内容被认为是注释。命令由命令名称、小括号和参数组成，参数之间使用空格进行间隔。

对于上面的 CMakeLists.txt 文件，依次出现了几个命令：

1. `cmake_minimum_required`：指定运行此配置文件所需的 CMake 的最低版本；
2. `project`：参数值是 `Demo1`，该命令表示项目的名称是 `Demo1` 。
3. `add_executable`： 将名为 [main.cc](http://main.cc/) 的源文件编译成一个名称为 Demo 的可执行文件。

* 编译项目

之后，在当前目录执行 `cmake .` ，得到 Makefile 后再使用 `make` 命令编译得到 Demo1 可执行文件。

```bash
[ehome@xman Demo1]$ cmake .
-- The C compiler identification is GNU 4.8.2
-- The CXX compiler identification is GNU 4.8.2
-- Check for working C compiler: /usr/sbin/cc
-- Check for working C compiler: /usr/sbin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: /usr/sbin/c++
-- Check for working CXX compiler: /usr/sbin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ehome/Documents/programming/C/power/Demo1
[ehome@xman Demo1]$ make
Scanning dependencies of target Demo
[100%] Building C object CMakeFiles/Demo.dir/main.cc.o
Linking C executable Demo
[100%] Built target Demo
[ehome@xman Demo1]$ ./Demo 5 4
5 ^ 4 is 625
[ehome@xman Demo1]$ ./Demo 7 3
7 ^ 3 is 343
[ehome@xman Demo1]$ ./Demo 2 10
2 ^ 10 is 1024
```

##### 同一目录，多个源文件

上面的例子只有单个源文件。现在假如把 `power` 函数单独写进一个名为 `MathFunctions.c` 的源文件里，使得这个工程变成如下的形式：

```bash
./Demo2
    |
    +--- main.cc
    |
    +--- MathFunctions.cc
    |
    +--- MathFunctions.h
```

这个时候，CMakeLists.txt 可以改成如下的形式：

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo2)

# 指定生成目标
add_executable(Demo main.cc MathFunctions.cc)
```

唯一的改动只是在 `add_executable` 命令中增加了一个 `MathFunctions.cc` 源文件。这样写当然没什么问题，但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 `aux_source_directory` 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：

```cmake
aux_source_directory(<dir> <variable>)
```

因此，可以修改 CMakeLists.txt 如下：

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo2)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})
```

这样，CMake 会将当前目录所有源文件的文件名赋值给变量 `DIR_SRCS` ，再指示变量 `DIR_SRCS` 中的源文件需要编译成一个名称为 Demo 的可执行文件。

##### 多个目录，多个源文件



本小节对应的源代码所在目录：[Demo3](https://github.com/wzpan/cmake-demo/tree/master/Demo3)。

现在进一步将 MathFunctions.h 和 [MathFunctions.cc](http://mathfunctions.cc/) 文件移动到 math 目录下。

```bash
./Demo3
    |
    +--- main.cc
    |
    +--- math/
          |
          +--- MathFunctions.cc
          |
          +--- MathFunctions.h
```

对于这种情况，需要分别在项目根目录 Demo3 和 math 目录里各编写一个 CMakeLists.txt 文件。为了方便，我们可以先将 math 目录里的文件编译成静态库再由 main 函数调用。

根目录中的 CMakeLists.txt ：

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo3)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 添加 math 子目录
add_subdirectory(math)

# 指定生成目标 
add_executable(Demo main.cc)

# 添加链接库
target_link_libraries(Demo MathFunctions)
```

该文件添加了下面的内容: 第3行，使用命令 `add_subdirectory` 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理 。第6行，使用命令 `target_link_libraries` 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库 。

子目录中的 CMakeLists.txt：

```cmake
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library (MathFunctions ${DIR_LIB_SRCS})
```

在该文件中使用命令 `add_library` 将 src 目录中的源文件编译为静态链接库。



#### automake

* [automake](https://www.gnu.org/software/automake/manual/html_node/index.html#SEC_Contents):

```bash
#Ubuntu
sudo apt update #更新
sudo apt upgrade

sudo apt install build-essential

sudo apt install gcc g++ make cmake gfortran#上边指令应该就是安装了这几个，但没有gfortran

sudo apt install wget curl vim git #下载、编辑器

sudo apt install autoconf automake libtool #额外不在build-essential里的GNU套件
```

```bash
#mac

```







## Theory

### c++参考网页
* [cplusplus](https://www.cplusplus.com)
* [cppreference](https://en.cppreference.com/w/)
* [The C++ Standards Committee](http://www.open-std.org/JTC1/SC22/WG21/)
* [Microsoft C++、C 和汇编程序文档](https://docs.microsoft.com/zh-cn/cpp/?view=msvc-170)

### 头文件

头文件最大的作用在于，一个include 就可以把其他文件的所有函数声明写在“前面”，然后随时调用。

这样就又产生了另一个好处：在编译的时候，只要头文件没有更改（也就是函数声明没有改变），只改动了函数的定义（具体实现方式），需要它（这个文件）的文件是不需要重新编译的。

比如，makefile中，文件只依赖于“所需函数”所在的头文件，而不依赖于它的定义，也就是“库文件”，这样改变函数定义（没有改声明）的话，make只需要重编译库文件，再进行链接。

### sublime写c++小技巧

* #Inc  --- include <>
* tp     --- template <typename T>
* for    --- for loop
* main --- int main(){}
* Class--- class {public ...}
* td     --- typedefine

### c++运算符的优先级

---

| 优先级 | 操作符                                                       | 描述                                                         | 例子                                                         | 结合性   |
| :----- | :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :------- |
| 1      | **()   []  -> . :: ++ --**                                   | 调节优先级的括号操作符 / 数组下标访问操作符 / 通过指向对象的指针访问成员的操作符 / 通过对象本身访问成员的操作符 / 作用域操作符 / 后置自增操作符 / 后置自减操作符 | (a + b) / 4; array[4] = 2; ptr->age = 34; obj.age = 34; Class::age = 2; for( i = 0; i < 10; i++ ) ... for( i = 10; i > 0; i-- ) ... | 从左到右 |
| 2      | **!  ~ ++ -- -   +   *   & (type) [sizeof](http://www.cppreference.com/keywords/sizeof.html)** | 逻辑取反操作符 / 按位取反(按位取补)  前置自增操作符 / 前置自减操作符 一元取负操作符 / 一元取正操作符 / 解引用操作符 / 取地址操作符 类型转换操作符 / 返回对象占用的字节数操作符 | if( !done ) ... flags = ~flags; for( i = 0; i < 10; ++i ) ... for( i = 10; i > 0; --i ) ... int i = -1; int i = +1; data = *ptr; address = &obj; int i = (int) floatNum; int size = sizeof(floatNum); | 从右到左 |
| 3      | **->* / .***                                                 | 在指针上通过指向成员的指针访问成员的操作符 在对象上通过指向成员的指针访问成员的操作符 | ptr->*var = 24; obj.*var = 24;                               | 从左到右 |
| 4      | *** % /这是除法符号**                                        | 乘法操作符 除法操作符 取余数操作符                           | int i = 2 * 4; float f = 10 / 3; int rem = 4 % 3;            | 从左到右 |
| 5      | **+ -**                                                      | 加法操作符 减法操作符                                        | int i = 2 + 3; int i = 5 - 1;                                | 从左到右 |
| 6      | **<< >>**                                                    | 按位左移操作符 按位右移操作符                                | int flags = 33 << 1; int flags = 33 >> 1;                    | 从左到右 |
| 7      | **< <= > >=**                                                | 小于比较操作符 小于或等于比较操作符 大于比较操作符 大于或等于比较操作符 | if( i < 42 ) ... if( i <= 42 ) ... if( i > 42 ) ... if( i >= 42 ) ... | 从左到右 |
| 8      | **== !=**                                                    | 等于比较操作符 不等于比较操作符                              | if( i == 42 ) ... if( i != 42 ) ...                          | 从左到右 |
| 9      | **&**                                                        | 按位与操作符                                                 | flags = flags & 42;                                          | 从左到右 |
| 10     | **^**                                                        | 按位异或操作符                                               | flags = flags ^ 42;                                          | 从左到右 |
| 11     | **\|**                                                       | 按位或操作符                                                 | flags = flags \| 42;                                         | 从左到右 |
| 12     | **&&**                                                       | 逻辑与操作符                                                 | if( conditionA && conditionB ) ...                           | 从左到右 |
| 13     | **\|\|**                                                     | 逻辑或操作符                                                 | if( conditionA \|\| conditionB ) ...                         | 从左到右 |
| 14     | **? :**                                                      | 三元条件操作符                                               | int i = (a > b) ? a : b;                                     | 从右到左 |
| 15     | **= += -= *= /= %= &= ^= \|= <<= >>=**                       | 赋值操作符 复合赋值操作符(加法) 复合赋值操作符(减法) 复合赋值操作符(乘法) 复合赋值操作符(除法) 复合赋值操作符(取余) 复合赋值操作符(按位与) 复合赋值操作符(按位异或) 复合赋值操作符(按位或) 复合赋值操作符(按位左移) 复合赋值操作符(按位右移) | int a = b; a += 3; b -= 4; a *= 5; a /= 2; a %= 3; flags &= new_flags; flags ^= new_flags; flags \|= new_flags; flags <<= 2; flags >>= 2; | 从右到左 |
| 16     | **,**                                                        | 逗号操作符                                                   | for( i = 0, j = 0; i < 10; i++, j++ ) ...                    | 从左到右 |

### 名称空间

名称空间支持是一项C++特性，旨在让您编写大型程序以及将多个厂商现有的代码组合起来的程序时更容易，它还有助于组织程序。一个潜在的问题是，可能使用两个已封装好的产品，而它们都包含一个名为wanda( )的函数。这样，使用wanda( )函数时，编译器将不知道指的是哪个版本。名称空间让厂商能够将其产品封装在一个叫做名称空间的单元中，这样就可以用名称空间的名称来指出想使用哪个厂商的产品。因此，Microflop Industries可以将其定义放到一个名为Microflop的名称空间中。这样，其wanda( )函数的全称Microflop::wanda( )；同样，Piscine公司的wanda( )版本可以表示为Piscine::wanda( )。这样，程序就可以使用名称空间来区分不同的版本了：

```c++
Microflop::wanda("go dancing?"); //using Microflop namespace
Piscine::wanda("go dancing?"); //using Piscine namespace
using namespace std; //偷懒做法，使以后的函数都不必加上std::的前缀
```



### 运算符重载

如果熟悉C后才开始学习C++，则可能注意到了，插入运算符（<<）看上去就像按位左移运算符（<<），这是一个运算符重载的例子，通过重载，同一个运算符将有不同的含义。编译器通过上下文来确定运算符的含义。C本身也有一些运算符重载的情况。例如，&符号既表示地址运算符，又表示按位AND运算符；* 既表示乘法，又表示对指针解除引用。这里重要的不是这些运算符的具体功能，而是同一个符号可以有多种含义，而编译器可以根据上下文来确定其含义（这和确定“sound card”中的“sound”与“sound financial basic”中的“sound”的含义是一样的）。C++扩展了运算符重载的概念，允许为用户定义的类型（类）重新定义运算符的含义。

**printf 与 cout**

同为显示信息，cout要“聪明”的多：

```c++
printf("%s\n","hello world!，this is in c++");
printf("%d\n",25)
   
//下面是使用cout来输出
cout << "hello world!，this is in c++" << 25;
```

### 数组

就如同c++中其他任何“item”，数组也需要声明：

```c++
int months[12]; //声明了一个数组，个数是12，数组元素类型为int，名为months


```

只有在定义数组时才能使用初始化，此后就不能使用了，也不能将一个数组赋给另一个数组。

```c++
int cards[4] = {3, 6, 8, 10}; //that's ok
int hand[4];  //that's ok
hand[4] = {5, 6, 7, 9}  //not allowed
hand = cards;  //not allowed
```

这样声明有一个问题：为了避免报错，我声明一个数组的长度肯定要大于我需要的长度。但这样会浪费很多内存，**OOP强调的是运行阶段进行决策而不是编译阶段进行决策，也就是动态调整系统分配给数组的内存块大小。**这时候就用到动态数组：

```c++
#include <iostream>

int main()
{
    using namespace std;
    int a;
    cout << "Type the lenth of a list: ";
    cin >> a;

    //用new在堆（heap）/自由存储区（free）创建动态数组（对于上述变量和数组的创建，没有用new的，都是在‘栈’中）
    int* psome = new int[a];  // 这是常规数组做不到的

    //输入数组元素
    int i;
    for (i = 0; i<a; i++)
    {   //这里没有花括号，新标准下也是合法的
        cout << "Type the " << i+1 << "th element of the list: ";
        cin >> *(psome + i);
    };

    //输出数组元素
    cout << endl << "The list is [ ";
    for (i = 0; i<a; i++)
        cout << psome[i] << " "; //c++都是用指针处理数组，所以声明了一个指针指向这个数组时，同样可以作为数组名使用
    cout << "]\n";

    delete [] psome;
    
    return 0;
}
```



#### vector

> “模板类vector类似于string类，也是一种动态数组。您可以在运行阶段设置vector对象的长度，可在末尾附加新数据，还可在中间插入新数据。基本上，它是使用new创建动态数组的替代品。实际上，vector类确实使用new和delete来管理内存，但这种工作是自动完成的。”
>
> Excerpt From: [美] Stephen Prata. “C++ Primer Plus（第6版）中文版.” Apple Books. 
```c++
#include <vector>

int main()
{
    using namespace std;

    vector<int> vi;  //vi实际上是一个vector<int>对象，
    int n;
    cin >> n;
    vector<double> vd(n);  //由于vector是用new申请的堆内存，所以效率要比array稍低

    return 0;
}
```

思考：vector对象？这应该就是一个类的应用吧！还要include，说明也是一个库文件，这个库文件定义了vector类模版？然后vector<int>就是vector类模版的具体化，然后我们就创建了这个实例vi。



### 字符串



#### 字符串指针

```c++
#include <iostream>

int main()
{
    using namespace std;
    const char * wail = "ululate";    // wail points to string

    int a = 12;
    int * p = &a;

    cout << wail << endl;    //cout 重载了 指向char类型的地址，直接将其显示为字符串本身
    
    //下面指令结果：0x10539df50
    cout << (int *) wail << endl;   //强制类型转换为int*，由于cout不重载int指针，所以能显示地址

    cout << (long long int) wail << endl;

    //下面指令结果：0x10539df50
    printf("%p\n", wail);      

    //下面指令结果：ululate
    printf("%s\n", wail);      

    //下面指令结果：87678800
    printf("%d\n", wail);      //warning: format specifies type 'int' but the argument has type 'const char *'
    
    //下面是16进制int输出wail：a79ff40
    printf("%x\n", wail);
    
    return 0;
}

```

#### c/c++ 进制转换

```c++
#include <bitset>  
#include<iostream>
using namespace std;

int main()
{
	printf("%05o\n",35);    //按八进制格式输出，保留5位高位补零
	printf("%03d\n",35);    //按十进制格式输出，保留3位高位补零
	printf("%05x\n",35);    //按十六进制格式输出，保留5位高位补
    
    cout << "35的8进制:" << std::oct << 35<< endl;  
    cout << "35的10进制" << std::dec << 35 << endl;  
    cout << "35的16进制:" << std::hex << 35 << endl;  
    cout << "35的2进制: " << bitset<8>(35) << endl;      //<8>：表示保留8位输出
    return 0;  
}
```





### 结构

数组无法存储不是同一类型的一组数据，这时“结构”就应运而生。**结构声明定义了一种新类型**

```c++
#include <iostream>
#include <string>


int main()
{
    using namespace std;
    
    struct inflatable
    {
        char name[20];
        float volume;
        double price;
    };  //有点像类，这是定义结构---定义了一种新类型
    
    inflatable hat;  //这是创建这种类型的变量
    
    return 0;
}
```





### 函数定义与调用

这就是（函数原型）为什么python中调用函数需要先定义，就是因为解释器也需要先读取函数声明，但python简化了这一过程，所以也就不能之后定义。

这更解释了python中一特例，就是在a函数中调用b函数时可以把b函数定义放在a后，这是因为python执行def时并不执行def里边的代码，只看def了什么样的函数，返回值是什么，就像是c中这个原型的声明。

当然c++也可以把函数定义放在调用之前来代替原型的声明，这也是合法的，但是main（）函数的书写规范就是在最前边，这也让代码的可读性极强，逻辑清晰。这对构建大型程序来说很重要。

所以，也看得出来，如果在代码易读性，代码书写简洁程度，和代码执行效率之间权衡，是目前计算机语言之间的根本区别。

```c++ 
// 这是一个直角坐标系转换为极坐标系的c++代码，是函数与结构的应用
#include <iostream>
#include <string>
#include <cmath>

struct polar
{
    double distance;
    double angle;
};
struct rect
{
    double x;
    double y;
};

void show_polar(polar dapos);
polar rect_to_polar(rect xypos);


int main()
{
    using namespace std;
    int x, y;
    rect xypos;
    polar answer;

    cout << "Type your x position: ";
    cin >> x;
    cout << "Type your y position: ";
    cin >> y;
    xypos.x = x;
    xypos.y = y;

    show_polar(rect_to_polar(xypos));

    return 0;
}


void show_polar (polar dapos)
{
    using namespace std;
    const double Rad_to_deg = 57.29577951;

    cout << "distance = " << dapos.distance;
    cout << endl << "angle = " << dapos.angle * Rad_to_deg;
    cout << " degrees\n";
}


polar rect_to_polar(rect xypos)
{
    using namespace std;
    polar answer;

    answer.distance = sqrt(xypos.x * xypos.x + xypos.y * xypos.y);
    answer.angle = atan2(xypos.y , xypos.x);

    return answer;
}
```

我们若用指针传递这两个结构参数，既可以避免复制数据增加内存占用，也节省了时间，代码修改如下，注意对比区别：
```c++
// 用指针传递结构参数

#include <iostream>
#include <string>
#include <cmath>

struct polar
{
    double distance;
    double angle;
};
struct rect
{
    double x;
    double y;
};

void show_polar(const polar* dapos);
void rect_to_polar(rect* xypos, polar* dapos);


int main()
{
    using namespace std;
    rect xypos;
    polar answer;

    cout << "Type your x position: ";
    cin >> xypos.x;
    cout << "Type your y position: ";
    cin >> xypos.y;

    rect_to_polar(&xypos, &answer);

    cout << "The Adress of the polar struct 'answer' is " << &answer << endl;
    show_polar(&answer);

    return 0;
}


void show_polar (const polar* dapos)
{
    using namespace std;
    const double Rad_to_deg = 57.29577951;

    cout << "distance = " << dapos->distance;
    cout << endl << "angle = " << dapos->angle * Rad_to_deg;
    cout << " degrees\n";
}


void rect_to_polar(rect* xypos, polar* answer)
{
    using namespace std;

    answer->distance = sqrt(xypos->x * xypos->x + xypos->y * xypos->y);
    answer->angle = atan2(xypos->y , xypos->x);

}
```

### 指针

关于指针声明形式的含义有多种理解方式，为了避免歧义，我倾向于下述理解方式：

```c++
int a;
int* t = &a; //int*在一起不是必要的，但可以理解为int*就是声明指针这种类型，就像是int就是声明整数类型一样，然后t就是这个指针，也即是地址，所以t = &a。

//当然下面这种理解也可以
int *t = &a; //
```

好处有很多，尤其是下文中讲到的函数指针，我感觉理解起来会有很大帮助。

```c++
#include <iostream>
#include <string>

int main()
{
    using namespace std;
 
    //整数与指针
    int a = 6;
    int* p;
    p = &a;
 
    cout << "Adress of vaiable a is " << p << endl;
    cout << "a is equal to " << *p;

    int* pt = new int;  // if i don't use the "new" command, it'll be wrong.
    *pt = 6;

    cout << "Adress of the int data '6' is " << pt << endl;
    cout << "This is not a vaiable, but it do have a memory block. ";
	
	delete pt;
	
    return 0;
}
```

#### 函数与指针：

函数指针，指针函数，函数指针数组？这些概念的区分就靠上文中讲到的**运算符优先级**。

```c++
#include <iostream>

const double* f1(const double ar[], int n);
const double* f2(const double [], int);
const double* f2(const double*, int);

int main()
{
    using namespace std;
    double av[3] = {1112.3, 1542.6, 2227.9};

    const double* (*p1)(const double* ,int) = f1; //这是一个函数指针的声明与定义，const 在*左边，它是一个const的double*，所以指针本身是常量（地址不能变），这个常量地址是函数的返回值
    auto p2 = f2;

    cout << "Using pointers to function:\n";
    cout << "Adress Value\n";
    cout << (*p1)(av, 3) << " : " << *(*p1)(av, 3) << endl;
    cout << p2(av ,3) << " : " << *p2(av , 3) << endl;

    const double* (*pa[3])(const double*, int) = {f1, f2, f3};
    auto pb = pa;
    
    cout << "\nUsing an array of pointers to functions:\n";
    cout << "Adress Value:\n";
    for (int 1 =0; i<3; i++)
        cout << pa[i](av ,3) << " : " << *pa[i](av, 3) << endl;

    cout << "\nUsing pointers to an array of pointers to functions:\n";
    cout << "Adress Value:\n";

    auto pc = &pa;
    cout << (*pc)[0](av, 3) << " : " << *(*pc)[0](av, 3);

    return 0;
}
```

> [原文链接](https://www.zhihu.com/question/59605233/answer/166952863)
>
> * 指针数组，看后面两个字，它是一个数组，数组元素是指针；
> * 数组指针，看后面两个字，它是一个指针，指向一个数组；
> * 函数指针，看后面两个字，它是一个指针，指向一个函数；
> * 指针函数，看后面两个字，它是一个函数，返回指针类型；
> * 指向函数指针的数组，看后面两个字，它是一个数组，数组元素是指向函数的指针，即函数指针。
>
> 再来看看代码吧。比如 int* p[10] 和 int (*p)[10] ，哪个是指针数组，哪个是数组指针呢？其实很容易区分，可以按照运算优先级从低到高的顺序依次说出来就得到了其名字（PS：从高到低的顺序也可以，那样的话名字要倒过来念）。
>
> ```c++
> int* p[10] // 指针运算的优先级比下标低，所以排列结果是 指针<数组，所以这是一个指针数组
> int (*p)[10] // 加了括号后指针的优先级更高，所以 数组<指针，这是一个数组指针
> int* pfun(int a, int b) // 优先级 指针<函数，所以这是一个指针函数
> int (*pfun)(int a, int b) // 函数<指针，所以这是一个函数指针
> int (*pfun[3])(int a, int b) // 函数<指针<数组，所以这是一个数组，数组元素是指针，指针是指向函数的指针，函数的返回值是int类型，参数也是两个int类型，所以这是一个函数指针数组
> ```

##### 函数指针的作用？传递函数的使用场景？

1. **函数指针：**函数名本身就是指针，所以没啥用。。。。
2. **传递函数：**有一些情况，是需要将函数（函数2）作为参数传递给另一个函数（函数1）的。往往是需要不同的函数来实现相似又不同的功能：

```c++
#include <iostream>

using namespace std;

int add(int x, int y){
    return x + y;
}

int minus(int x, int y){
    return x - y;
}

int multiply(int x, int y){
    return x * y;
}

int bala(int a, int b, int (*some_func)(int, int)){
    return (*some_func)(a, b);
}

int main(){
    int w = 10;
    int q = 3;
    cout << bala(w, q, add) << endl;
    cout << bala(w, q, minus) << endl;
}
```







`int *pt = new int;`是利用new运算符来申请一块内存，把地址给指针，当然也要和delete 配合使用，如上述代码。值得注意的是，new申请的是堆，没有用new创建的变量和指针都存在栈中（静态除外）。（见下文章节---内存管理）

**函数名和数组名都是指针，也就是数据的起始地址---这句话其实是有问题的，只是有些时候数组名和函数名被编译器根据上下文被翻译成指针，而且数组名和函数名的用法也不一样（数组与指针见下面代码）**

```c++
#include <iostream>
#include <string>

int Baby(const int* a);

int main()
{
	using namespace std;
	int Array[7] = {5, 3, 54, 65, 77, 88, 99};
    int* pl = Array;
    // 数组与指针，数组名直接可以当指针用
    cout << endl << "This part is 数组与指针" << endl;
    cout << pl << " is Adress of the Array. " << endl;
    cout << *pl << endl << *(pl+4) << endl;
    cout << Array << endl << pl << endl << pl+1 << endl << pl << endl;
    cout << Array+1 << endl;
    

    return 0;
}
```

#### typedef

上述函数指针数组让人眼花缭乱，我们可以用typedef来创建类型的别名：

```c++
typedef const double* (*p1)(const double* ,int);
p1 = f1;
```




#### 指针与内存空间的利用与处理：

> [原文链接](https://blog.csdn.net/isnil/java/article/details/4221636)
>
> 在“函数的 工作原理 ”中，提到了5个内存区域：
>
> 全局名称空间
> 自由存储区
> 寄存器
> 代码空间
> 堆栈
>
> 局部变量和函数参数位于堆栈中 ：当然，代码位于代码空间中；而全局变量位于全局名称空间中；寄存器用于内部管理工作，如记录栈顶指针和指令指针。余下的所有内存都被作为自由存储区，通常被称为堆。
>
> 局部变量不是永久性的，函数返回时，局部变量就被删除。这很好，因为这意味着根本不用为管理这种内存空间而劳神；也不好，因为这使得函数在不将堆中的对象复制到调用函数中的目标对象的情况下，将难以创建供其他对象或函数使用的对象。全局变量解决了这种问题，其代价是整个程序中都可以访问它们。这导致创建了难以理解和维护的代码。如果管理得当，将数据存储在自由存储区可以解决这两种问题。
>
> 可以将自由存储区视为一块很大的内存，其中有数以千计的依次被编号的内存单元，可用于存储数据。与堆栈不同，你不能对这些单元进行标记，而必须先申请内存单元的地址，然后将它存储到指针中。
>
> 可以使用这样的类比：朋友给了你交通银行的800客服电话号码。你回到家中，将该电话号码与某个按钮绑定，然后扔掉记录电话号码的纸张。如果按下按钮，被拨打的电话将响铃，交通银行的职员进行接听。你不需要记下这个电话号码，，也不知道被拨打的电话在哪里，但只要按下绑定的按钮就能致电交通银行。自由存储区中的数据就像交通银行一样，你不知道它在什么地方，但知道如何找到它。你使用地址（在这个例子中，地址为电话号码）访问它。你不必知道地址，只需将其放在一个指针（按钮）中。指针让你能够访问数据，而不必知道细节。
>
> 函数返回时，堆栈被自动清空。所有局部变量都不在作用域内，他们被从堆栈中删除。程序结束前，自由存储器不会自动清空，程序员使用完自己分配的内存后，必须负责将其释放。
>
> 自由存储区的优点是，你从中分配的内存将一直可用，知道你明确的指出不再需要——将其释放为止。如果在函数中分配自由存储区中的内存，在函数返回后该内存仍可用。
>
> 这也是自由存储区的缺点，如果你忘记释放内存，被占据而没有使用的内存将随着时间的推移愈来愈多，导致系统崩溃。
>
> 采用这种内存访问方式而不是全局变量的优点是，只有能够访问指针的函数才能访问它指向的数据。这样只有将包含指针的对象或指针本身传递给函数，函数才能修改指针指向的数据，从而减少了函数能够改变数据，而又无法跟踪变更的情况发生。

#### 栈（stack）

[一个栈原理的博客](https://www.cnblogs.com/findumars/p/7545818.html)



#### 参数（数据）传递

参数传递主要是指在把参数传递给函数，按值传递就是把参数复制到一个“虚拟参数”（也就是函数调用的栈内存块中），所以这被称为**形参**，与之对应，原本的参数就被称为**实参**。----见c++primer 7.2

复制，指针，引用，是三种传递参数的方式。引用可以看作一个**const 指针(这个const指针可不是不能变，这需要好好研究见下面代码)**，而指针就尝尝用在需要指定变化的量时。当然具体数据类型不同，也有所不同：

* 传递参数：c++在传递一个变量参数时，是复制了该值。
* 传递数组：只是复制了该数组名（也就是数组指针 / 或者是数组第一个元素的地址）
* 传递结构：像传递参数一样，是会复制的，所以我们尽量使用结构指针。值得注意的是，

#### 引用

```c++
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    using namespace std;
    
    int rats;
    int & rodents = rats; //与下句等同但是，很明显，引用是一个值，而const p 是一个地址！
    int* const pr = &rats; //int* 这是一个指针，指向const pr，所以pr是常量，地址（指针本身）可以变

    int a;
    const int* p = &a; //const在*的左边，这是一个不能变的指针，所以地址不能变，p不能变，*p能变

    int b;
    const int & ref = b;


    return 0;
}
```

相比于按值传递，引用和指针类似，**（但是*p是变量值，而引用名a就是变量值）**但是要记住，普通的数值类型如int，double是没必要用引用或者指针的，因为本身数据量就很小，而且用了会有一些问题，因为我们常常会用到`a = a +1` 这种，问题见下代码：

```c++
#include <iostream>
double cube(double a);
double refcube(double& ra);

int main()
{
    using namespace std;
    double x = 3.0;
    
    cout << cube(x);
    cout << " = cube of " << x << endl;
    cout << refcube(x);
    cout << " = cube of " << x << endl;
    
    return 0;
}

double cube(double a)
{
    a *= a * a;
    return a;
}

double refcube(double& ra); //符号&和类型关键字如double在一起时就是引用，而和变量在一起时就是取地址
{
    ra *= ra * ra;
    return ra;
}

/* 
该程序就有了问题
/*
```



#### 引用和指针的区别

像下面函数中接受的参数，我们来解读下这是什么意义，以及为何要用这个`LinkList *&L`？

```c++ 
//删除单链表某值
void 删除单链表某值(LinkList * & L, ElemType x) {
    LNode *p = (LNode *)malloc(sizeof(LNode));
    if (L == NULL) {
        return;
    }
    if (L->data == x) {
        p = L;
        L = L->next;  //精髓所在
        free(p);
        删除单链表某值(L, x);
    }
    else {
        删除单链表某值(L->next, x);
    }
}
```

`LinkList *` 表示这个参数是一个地址，而`&`表示，L是这个地址的引用，而传递这个地址时，不能直接拷贝这个地址，而是去存放这个地址的地址寻址！





#### 指针和引用的危险：

1. c++创建指针时，计算机将分配用来储存地址的内存，而不会分配指针指向的数据的内存！这意味着不能用指针赋值**（像这样`*p = 235`就是错的）**。
这个指令只会让计算机寻找235这个数，如果有，那么它的地址放在指针p里，如果没有就报错，而并不是创建一个新的量放在新的内存中。且`int *p;`代表指针指向的数据类型是int，而指针本身也就是地址的类型并不一定是int。
2. 指针或应用的参数，对象等可能会被“清空”。  
   比如**引用了非同一代码段的对象时**，原对象会在其所在代码段执行完成时调用析构函数进而所在内存块被标记被“无效”，这时如果还引用就会导致问题。  
   再比如返回的对象是本函数中的局部变量时，也就是**对象是在函数中创建的，要返回它，不能用引用或者指针**，因为这时这个对象作为局部变量，在函数调用完成后就被“清空”了。

对于上述这一点，非常重要，需多说几句才能

1. 指针或对象还可能被更改





#### 值类型与引用类型

> 这部分同《learn-swift》

几乎每种语言都存在这种问题。有个模糊的概念，就是值类型一般存在栈中（但不仅限于栈），而[引用类型只能放在堆中](https://blog.csdn.net/nicepainkiller/article/details/78213694)（**这里有问题，小数组不应该是在栈上吗，为啥是引用类型？**）。其中栈是个较复杂的历史问题。



> 比如C/C++的栈实现，是得到了CPU的基本支持的：一方面，CPU就有相应的栈指针寄存器，所以这是直接绑定到CPU指令上的，执行、取值都快速的多；另一方面，Stack的最顶端一般会留存在CPU registers和 cache中，遇到频繁但是层次不多的函数调用，可以利用高速 cache（栈一般是在一级缓存中做缓存的, 而堆则是在二级缓存中，**这个表述可能有问题**），而大块的内存会破坏这种优化。
>
> 比如swift语言中, 三个字及以内的struct结构, 可以在栈中内联, 从而达到更快的处理速度。

这也是为什么值类型一般存在栈上，且为占用内存小的数据。但是像python这种值类型，即使也是在栈上，它的栈并没有直接利用上CPU指令集，而是编译器靠算法实现了这种先入后出的数据结构罢了，python一开始就申请了一大块堆内存来运行时确定大部分工作，这也是python被称为动态语言的原因之一，更是效率低的重要原因之一。

对于Swift，数字、结构等是值类型（**但不一定被分配在栈上**），数组、类是引用类型。对于多数语言，**有争议的一般都在数组（Array/List）上**，因为数组算是基本类型，但可大可小，比如C++中，默认声明的数组是在栈上，使用new等申请的数组是在堆上，这时往往是数组比较大。



 **Q：结构是值类型，其可以包含函数和数组，但是数组、函数确是引用类型？**

A：结构中声明数组和函数，所以同样地，传递结构时，其”所有“数据（包括数组、函数的引用/指针，但不包括其本身）被拷贝，所以上述问题不冲突。



### 函数模版

模版主要解决了函数参数必须要声明的问题，函数模版主要是“通用”参数类型的声明。

```c++

```

最初，编译器只能通过隐式实例化，来使用模板生成函数定义，但现在C++还允许显式实例化（explicit instantiation）: 

```c++

```



### 类


#### 构造函数与析构函数

我们希望创建对象时传入参数，且自动对它内部的参数也进行初始化。为此，C++提供了一个特殊的成员函数——类构造函数，专门用于构造新对象、将值赋给它们的数据成员。也就是说，一个类在实例化时就通过这个函数传入参数，就跟python类中的init函数一样。
看起来很奇怪，因为我们可以把它就当作类的传入参数，就会很有层次感，但是只有函数才可以传递参数，所以才有了类底下的这样一个特殊的函数，用来给类传入参数。

如果你没写构造函数，系统会默认一个无参数的构造函数。但只有一个默认的构造函数，如果你定义了构造函数有参数的，但你类实例化时候没写参数，这是报错的，除非你为构造函数提供初始值。

**构造函数可以有很多个，默认构造函数只有一个（无传入参数或传入参数都有默认值），这是因为在创建对象是，内部参数初始化的形式有很多种（可以传入不同量，可以不传入，可以复制另一个对象等等）**

同样，我们也希望**在定义对象的代码块执行完毕时**（也就是**"{}"**里边的代码执行完）做些什么（比如清空申请的堆内存），这就是析构函数的作用。**只有一个析构函数，且未声明系统会自动创建一个析构函数**。





```c++

```

##### 默认构造函数



##### 复制构造函数

复制构造函数用于将一个对象复制到另一个新创建的对象中。复制构造函数的默认创建和调用远比“想象”中多：

1. 将新对象显式地初始化为现有的对象时：

```c++
// calls StringBad(const StringBad &
StringBad ditto(motto); 
StringBad metoo = motto;
StringBad also = StringBad(motto);
StringBad * pStringBad = new StringBad(motto);
```

2. 函数按值传递对象或返回对象时：

```c++
int func(StringBad motto);
StringBad func()

```

而复制构造函数是怎样进行对象复制的呢？（因为对象有很多成员，其中静态成员是属于整个类的）：复制非静态成员。

###### 有个经常出现的问题：

当对象成员中有指针时，比如上述代码中motto对象在其所在代码块执行结束时调用了析构函数，指针也被“清空”，而当前可能ditto对象还处于活跃状态，但是该地址的数据以失效。

且在这种情况，ditto对象完成时同样调用析构函数，这可能会导致内存被释放两次，从而引发错误。

###### 解决方案：

定义一个显式复制构造函数：其应当在复制指针类型时做数据拷贝，再把副本的地址赋值给新对象的指针，而不是仅仅复制地址。

##### 类自动重载赋值运算符

我们应该想到这个问题：是在创建对象时，有一种是将一个对象复制到另一个新创建的对象中，也就是调用复制构造函数时：

```c++
StringBad headline1("Celery Stalks at Midnight");
StringBad knot;
knot = headline1;   // 这种情况使用重载的赋值运算符
StringBad & StringBad::operator=(const StringBad &); //这是默认自动生成的
    

StringBad metoo = knot; //此时用到复制构造函数，也可能分两步，先使用复制构造函数创建一个临时对象，然后通过赋值将临时对象的值复制到新对象中。这就是说，初始化总是会调用复制构造函数，而使用=运算符时也可能调用赋值运算符
```

**注意：**引用类时，就不会掉用复制构造函数了。



#### 友元函数与重载运算符<<

友元函数可以像类方法一样定义，但是不属于类，所以可以把类对象作为“显式”参数传入，这样的好处是对称性。

```c++
#ifndef FRIENDTIME_H
#define FRIENDTIME_H

#include <iostream>

class Time
{
private:
    int hours;
    int minutes;
    
public:
    Time();
    Time(int h, int m = 0);
    // ~Time();
    void AddMin(int m);
    void AddHr(int h);
    void Reset(int h = 0, int m = 0);
    void Show() const;
    Time operator+(const Time & t) const;//运算双方都是这种类对象的话，像加减，就不需要友元函数
    Time operator-(const Time & t) const;
    //下面这是设定友元函数来实现“乘法符号重载后的交换性”
    Time operator*(double mult) const;
    friend Time operator*(double m, const Time & t) //这不会有错误吗？两个operator*？
    {return t * m;}

    friend std::ostream & operator<<(std::ostream & os, const Time & t);

};

#endif
```

下面是对应的类定义：

```c++
#include "friendtime.h"

Time::Time()
{
    hours = minutes = 0;
}

Time::Time(int h, int m)
{
    hours = h;
    minutes = m;
}

void Time::AddMin(int m)
{
    minutes += m;
    hours +=minutes / 60;
    minutes %= 60;
}

void Time::AddHr(int h)
{
    hours += h;
}

void Time::Reset(int h, int m)
{
    hours = h;
    minutes = m;
}

Time Time::operator+(const Time & t) const
{
    Time sum;
    sum.minutes = minutes + t.minutes;a //此时Time t这个对象是“外面”的，为什么也可以访问它的private参数？
    sum.hours = hours + sum.minutes / 60;
    sum.minutes %= 60;
    return sum;
}

Time Time::operator*(double mult) const
{
    Time result;
    long totalminutes = hours * mult * 60 + minutes * mult;
    result.hours = totalminutes / 60;
    result.minutes = totalminutes % 60;
    return result;
}

//友元函数的定义就应该和普通函数一样，因为它并不属于类方法，只是他需要在类声明中声明friend
std::ostream & operator<<(std::ostream & os, const Time & t) //这个友元函数代替了下边的show函数，使用cout重载<<符号?全程不见cout是什么鬼？因为os就是cout，都是ostream类的对象
{
     os << t.hours << " hours, " << t.minutes << " minutes.\n"; // 这里把<<符号看做*就很好理解了，因为它已经被定义和重载过了，所以你只需要用这个类
     return os; //为什么只用在h文件中声明iostream库就可以？
}
void Time::Show() const 
{
    std::cout << hours << "hours, " << minutes << "minutes.\n";
}
```

下面是使用上述类的main函数：

```c++
#include <iostream>
#include "friendtime.h"

int main()
{
    using std::cout;
    using std::endl;

    Time aida(3, 35);
    Time tosca(2, 40);
    Time temp;

    cout << "aida and tosca: ";
    cout << aida << "; " << tosca << endl; //这种显示要比show()函数更直观通用，这就是重载运算符的好处
    temp = aida + tosca;
    cout << "aida + tosca: " << temp << endl;
    temp = aida * 1.17;
    cout << "aida * 1.17: " << temp << endl;
    cout << "10.0 * tosca: " << 10.0 * tosca << endl; 

    return 0;
}
```

对于重载运算符<<，我们对cout输出理解的更深刻了，当然还是有很多问题，只能看iostream库的代码才能完全理解。

所以说，对于被重载的运算符，更像是一个类方法（基本就是），它的结果就是它定义的返回值，很多情况下这个返回值都是这个类对象修改后的引用，或者定义了一个新的这个类对象。

而对于cout对象，它的实现还要更加复杂，是因为，它不仅仅是简单的几组数和函数的集合，还要涉及输出。这必然会涉及到系统输入输出API，比如如何向显存写入数据，而显卡又是怎么将这些数据转化为像素值，像屏幕显示led元件发出信号，使其对应光电元件通电发光，这就是系统级别的实现，仅依靠c++是不可能的。（关于c++GUI的实现即使也很复杂，需要第三方库来实现，如MFC）

对于网络部分的c++库的实现也是类似，都是靠操作系统API来交互，而不用操作系统理论上也是可行的（基本上就约等于自己写操作系统），比如你要理解所有网络协议的规则及其实现细节，然后发现有些c++实现不了，还得用汇编，然后就可以独立于操作系统和标准库实现输入输出，“上网”等看似及其基本的操作了。

指针的实现也没有这么简单，怎么才能知道每一个数据都存放在哪里呢，那这个地址不也需要地址来存放吗，数据量岂不一直递增，存储和检索的效率岂不很低？即使像c这么底层的语言，很多关键字（Keyword）和运算符都需要一定量的“包装”才得以实现。

但是有些运算符，就“一路到底”的“简单”，比如加减，在晶体管级别就有加法器，所以这是存在于CPU 指令集中的，每种语言对其的包装相对都是十分简单的。

##### 值得注意的几点：

1. 重载运算符<<时，返回的是**非const string对象的引用**，只能这么做。因为该对象在库文件中被定义，使用时也是调用原本就创建在String库文件中的对象，所以是要**修改该对象**。
2. 




### c++程序：头文件，源代码，链接

make程序，使用makefile来声明源代码之间的关系等，然后把它们链接起来。

<img src="learn-c++.assets\1151.png" alt="1151" style="zoom:35%;" />



> “头文件中常包含的内容。
>
> * 函数原型。
>
> * 使用#define或const定义的符号常量。
>
> * 结构声明。
>
> * 类声明。
>
> * 模板声明。
>
> * 内联函数。
>
> 将结构声明放在头文件中是可以的，因为它们不创建变量，而只是在源代码文件中声明结构变量时，告诉编译器如何创建该结构变量。同样，模板声明不是将被编译的代码，它们指示编译器如何生成与源代码中的函数调用相匹配的函数定义。被声明为const的数据和内联函数有特殊的链接属性（稍后将介绍），因此可以将其放在头文件中，而不会引起问题。
>
> 程序清单9.1、程序清单9.2和程序清单9.3是将程序清单7.12分成几个独立部分后得到的结果。注意，在包含头文件时，我们使用“coordin.h”，而不是<coodin.h>。如果文件名包含在尖括号中，则C++编译器将在存储标准头文件的主机系统的文件系统中查找；但如果文件名包含在双引号中，则编译器将首先查找当前的工作目录或源代码目录（或其他目录，这取决于编译器）。如果没有在那里找到头文件，则将在标准位置查找。因此在包含自己的头文件时，应使用引号而不是尖括号。”
>
> 在同一个文件中只能将同一个头文件包含一次。记住这个规则很容易，但很可能在不知情的情况下将头文件包含多次。例如，可能使用包含了另外一个头文件的头文件。有一种标准的C/C++技术可以避免多次包含同一个头文件。它是基于预处理器编译指令#ifndef（即if not defined）的。下面的代码片段意味着仅当以前没有使用预处理器编译指令#define定义名称COORDINH时，才处理#ifndef和#endif之间的语句：
>
> ```c++
> #ifndef COORDIN_H
> #define COORDIN_H
> #endif
> ```
>
> Excerpt From: [美] Stephen Prata. “C++ Primer Plus（第6版）中文版.” Apple Books. 

<img src="learn-c++.assets\Screen Shot 2020-05-15 at 19.49.27.png" alt="Screen Shot 2020-05-15 at 19.49.27" style="zoom:50%;" />





### 一个很基本的问题

函数递归、类的自引用，是怎么样的逻辑？



## c++常用库


### c++标准库




### Qt库

### Eigen 库

#### 类型转化

1. 数组转化为Eigen::Matrix
```c++
// 数组转化为Eigen::Matrix
int array[9];

cout << "colMajor matrix = \n" << Map<Matrix3i>(array) << endl;                      // map a contiguous array as a column-major matrix
cout << "rowMajor matrix = \n" << Map<Matrix<int, 3, 3, RowMajor>>(array) << endl;   // map a contiguous array as a row-major matrix

Map<MatrixXi>  eigMat1(array, 3, 3);                     // eigMat1和array指向的是同一个内存空间，是绑定在一起的
MatrixXi eigMat2 = Map<MatrixXi>(array, 3, 3);    //  eigMat1和array指向不同的内存空间，互不影响
```
2.  Eigen::Matrix转化为数组

```c++
Matrix3d eigMat;

double* eigMatptr = eigMat.data();
double* eigMatptrnew = new double[eigMat.size()];
Map<MatrixXd>(eigMatptrnew, eigMat.rows(), eigMat.cols()) = eigMat;
```



#### new 申请堆

```c++
MatrixXd *XN = new MatrixXd(3,N); //大矩阵用堆内存
for (int i = 0; i < 3; ++i){
    for (int j = 0; j < N; ++j){
        (*XN)(i,j) = x_N0(i)*cos(p[i]*t[j]) + vN0(i)/p[i]*sin(p[i]*t[j]);
    }
}
MatrixXd X = AN * (*XN);
XN->resize(0, 0); //有借就有还
delete XN;
```

#### 常用函数

```c++
#include <Eigen/Dense>
 
Matrix<double, 3, 3> A;               // Fixed rows and cols. Same as Matrix3d.
Matrix<double, 3, Dynamic> B;         // Fixed rows, dynamic cols.
Matrix<double, Dynamic, Dynamic> C;   // Full dynamic. Same as MatrixXd.
Matrix<double, 3, 3, RowMajor> E;     // Row major; default is column-major.
Matrix3f P, Q, R;                     // 3x3 float matrix.
Vector3f x, y, z;                     // 3x1 float matrix.
RowVector3f a, b, c;                  // 1x3 float matrix.
VectorXd v;                           // Dynamic column vector of doubles
double s;                            
 
// Basic usage
// Eigen          // Matlab           // comments
x.size()          // length(x)        // vector size
C.rows()          // size(C,1)        // number of rows
C.cols()          // size(C,2)        // number of columns
x(i)              // x(i+1)           // Matlab is 1-based
C(i,j)            // C(i+1,j+1)       //
 
A.resize(4, 4);   // Runtime error if assertions are on.
B.resize(4, 9);   // Runtime error if assertions are on.
A.resize(3, 3);   // Ok; size didn't change.
B.resize(3, 9);   // Ok; only dynamic cols changed.
                  
A << 1, 2, 3,     // Initialize A. The elements can also be
     4, 5, 6,     // matrices, which are stacked along cols
     7, 8, 9;     // and then the rows are stacked.
B << A, A, A;     // B is three horizontally stacked A's.
A.fill(10);       // Fill A with all 10's.
// Eigen                                    // Matlab
MatrixXd::Identity(rows,cols)               // eye(rows,cols)
C.setIdentity(rows,cols)                    // C = eye(rows,cols)
MatrixXd::Zero(rows,cols)                   // zeros(rows,cols)
C.setZero(rows,cols)                        // C = zeros(rows,cols)
MatrixXd::Ones(rows,cols)                   // ones(rows,cols)
C.setOnes(rows,cols)                        // C = ones(rows,cols)
MatrixXd::Random(rows,cols)                 // rand(rows,cols)*2-1            // MatrixXd::Random returns uniform random numbers in (-1, 1).
C.setRandom(rows,cols)                      // C = rand(rows,cols)*2-1
VectorXd::LinSpaced(size,low,high)          // linspace(low,high,size)'
v.setLinSpaced(size,low,high)               // v = linspace(low,high,size)'
VectorXi::LinSpaced(((hi-low)/step)+1,      // low:step:hi
                    low,low+step*(size-1))  //
// Matrix slicing and blocks. All expressions listed here are read/write.
// Templated size versions are faster. Note that Matlab is 1-based (a size N
// vector is x(1)...x(N)).切片操作
// Eigen                           // Matlab
x.head(n)                          // x(1:n)
x.head<n>()                        // x(1:n)
x.tail(n)                          // x(end - n + 1: end)
x.tail<n>()                        // x(end - n + 1: end)
x.segment(i, n)                    // x(i+1 : i+n)
x.segment<n>(i)                    // x(i+1 : i+n)
P.block(i, j, rows, cols)          // P(i+1 : i+rows, j+1 : j+cols)
P.block<rows, cols>(i, j)          // P(i+1 : i+rows, j+1 : j+cols)
P.row(i)                           // P(i+1, :)
P.col(j)                           // P(:, j+1)
P.leftCols<cols>()                 // P(:, 1:cols)
P.leftCols(cols)                   // P(:, 1:cols)
P.middleCols<cols>(j)              // P(:, j+1:j+cols)
P.middleCols(j, cols)              // P(:, j+1:j+cols)
P.rightCols<cols>()                // P(:, end-cols+1:end)
P.rightCols(cols)                  // P(:, end-cols+1:end)
P.topRows<rows>()                  // P(1:rows, :)
P.topRows(rows)                    // P(1:rows, :)
P.middleRows<rows>(i)              // P(i+1:i+rows, :)
P.middleRows(i, rows)              // P(i+1:i+rows, :)
P.bottomRows<rows>()               // P(end-rows+1:end, :)
P.bottomRows(rows)                 // P(end-rows+1:end, :)
P.topLeftCorner(rows, cols)        // P(1:rows, 1:cols)
P.topRightCorner(rows, cols)       // P(1:rows, end-cols+1:end)
P.bottomLeftCorner(rows, cols)     // P(end-rows+1:end, 1:cols)
P.bottomRightCorner(rows, cols)    // P(end-rows+1:end, end-cols+1:end)
P.topLeftCorner<rows,cols>()       // P(1:rows, 1:cols)
P.topRightCorner<rows,cols>()      // P(1:rows, end-cols+1:end)
P.bottomLeftCorner<rows,cols>()    // P(end-rows+1:end, 1:cols)
P.bottomRightCorner<rows,cols>()   // P(end-rows+1:end, end-cols+1:end)
 
// Of particular note is Eigen's swap function which is highly optimized.
// Eigen                           // Matlab
R.row(i) = P.col(j);               // R(i, :) = P(:, j)
R.col(j1).swap(mat1.col(j2));      // R(:, [j1 j2]) = R(:, [j2, j1])
 
// Views, transpose, etc;
// Eigen                           // Matlab
R.adjoint()                        // R'
R.transpose()                      // R.' or conj(R')       // Read-write
R.diagonal()                       // diag(R)               // Read-write
x.asDiagonal()                     // diag(x)
R.transpose().colwise().reverse()  // rot90(R)              // Read-write
R.rowwise().reverse()              // fliplr(R)
R.colwise().reverse()              // flipud(R)
R.replicate(i,j)                   // repmat(P,i,j)
 
 
// All the same as Matlab, but matlab doesn't have *= style operators.
// Matrix-vector.  Matrix-matrix.   Matrix-scalar.
y  = M*x;          R  = P*Q;        R  = P*s;
a  = b*M;          R  = P - Q;      R  = s*P;
a *= M;            R  = P + Q;      R  = P/s;
                   R *= Q;          R  = s*P;
                   R += Q;          R *= s;
                   R -= Q;          R /= s;
 
// Vectorized operations on each element independently
// Eigen                       // Matlab
R = P.cwiseProduct(Q);         // R = P .* Q
R = P.array() * s.array();     // R = P .* s
R = P.cwiseQuotient(Q);        // R = P ./ Q
R = P.array() / Q.array();     // R = P ./ Q
R = P.array() + s.array();     // R = P + s
R = P.array() - s.array();     // R = P - s
R.array() += s;                // R = R + s
R.array() -= s;                // R = R - s
R.array() < Q.array();         // R < Q
R.array() <= Q.array();        // R <= Q
R.cwiseInverse();              // 1 ./ P
R.array().inverse();           // 1 ./ P
R.array().sin()                // sin(P)
R.array().cos()                // cos(P)
R.array().pow(s)               // P .^ s
R.array().square()             // P .^ 2
R.array().cube()               // P .^ 3
R.cwiseSqrt()                  // sqrt(P)
R.array().sqrt()               // sqrt(P)
R.array().exp()                // exp(P)
R.array().log()                // log(P)
R.cwiseMax(P)                  // max(R, P)
R.array().max(P.array())       // max(R, P)
R.cwiseMin(P)                  // min(R, P)
R.array().min(P.array())       // min(R, P)
R.cwiseAbs()                   // abs(P)
R.array().abs()                // abs(P)
R.cwiseAbs2()                  // abs(P.^2)
R.array().abs2()               // abs(P.^2)
(R.array() < s).select(P,Q );  // (R < s ? P : Q)
R = (Q.array()==0).select(P,R) // R(Q==0) = P(Q==0)
R = P.unaryExpr(ptr_fun(func)) // R = arrayfun(func, P)   // with: scalar func(const scalar &x);
// Reductions.
int r, c;
// Eigen                  // Matlab
R.minCoeff()              // min(R(:))
R.maxCoeff()              // max(R(:))
s = R.minCoeff(&r, &c)    // [s, i] = min(R(:)); [r, c] = ind2sub(size(R), i);
s = R.maxCoeff(&r, &c)    // [s, i] = max(R(:)); [r, c] = ind2sub(size(R), i);
R.sum()                   // sum(R(:))
R.colwise().sum()         // sum(R)
R.rowwise().sum()         // sum(R, 2) or sum(R')'
R.prod()                  // prod(R(:))
R.colwise().prod()        // prod(R)
R.rowwise().prod()        // prod(R, 2) or prod(R')'
R.trace()                 // trace(R)
R.all()                   // all(R(:))
R.colwise().all()         // all(R)
R.rowwise().all()         // all(R, 2)
R.any()                   // any(R(:))
R.colwise().any()         // any(R)
R.rowwise().any()         // any(R, 2)
// Dot products, norms, etc.
// Eigen                  // Matlab
x.norm()                  // norm(x).    Note that norm(R) doesn't work in Eigen.
x.squaredNorm()           // dot(x, x)   Note the equivalence is not true for complex
x.dot(y)                  // dot(x, y)
x.cross(y)                // cross(x, y) Requires #include <Eigen/Geometry>
 
 Type conversion
// Eigen                  // Matlab
A.cast<double>();         // double(A)
A.cast<float>();          // single(A)
A.cast<int>();            // int32(A)
A.real();                 // real(A)
A.imag();                 // imag(A)
// if the original type equals destination type, no work is done
 
// Note that for most operations Eigen requires all operands to have the same type:
MatrixXf F = MatrixXf::Zero(3,3);
A += F;                // illegal in Eigen. In Matlab A = A+F is allowed
A += F.cast<double>(); // F converted to double and then added (generally, conversion happens on-the-fly)
 
// Eigen can map existing memory into Eigen matrices.
float array[3];
Vector3f::Map(array).fill(10);            // create a temporary Map over array and sets entries to 10
int data[4] = {1, 2, 3, 4};
Matrix2i mat2x2(data);                    // copies data into mat2x2
Matrix2i::Map(data) = 2*mat2x2;           // overwrite elements of data with 2*mat2x2
MatrixXi::Map(data, 2, 2) += mat2x2;      // adds mat2x2 to elements of data (alternative syntax if size is not know at compile time)
 
// Solve Ax = b. Result stored in x. Matlab: x = A \ b.
x = A.ldlt().solve(b));  // A sym. p.s.d.    #include <Eigen/Cholesky>
x = A.llt() .solve(b));  // A sym. p.d.      #include <Eigen/Cholesky>
x = A.lu()  .solve(b));  // Stable and fast. #include <Eigen/LU>
x = A.qr()  .solve(b));  // No pivoting.     #include <Eigen/QR>
x = A.svd() .solve(b));  // Stable, slowest. #include <Eigen/SVD>
// .ldlt() -> .matrixL() and .matrixD()
// .llt()  -> .matrixL()
// .lu()   -> .matrixL() and .matrixU()
// .qr()   -> .matrixQ() and .matrixR()
// .svd()  -> .matrixU(), .singularValues(), and .matrixV()
 
// Eigenvalue problems
// Eigen                          // Matlab
A.eigenvalues();                  // eig(A);
EigenSolver<Matrix3d> eig(A);     // [vec val] = eig(A)
eig.eigenvalues();                // diag(val)
eig.eigenvectors();               // vec
```



## Detail



### ++i 与 i++的区别

> https://www.zhihu.com/question/19811087/answer/80210083
>
> i++ 与 ++i 的主要区别有两个：
> **1、 i++ 返回原来的值，++i 返回加1后的值。**
> **2、 i++ 不能作为左值，而++i 可以。**



那么为什么『i++ 不能作为左值，而++i 可以』？
看它们各自的实现就一目了然了：
以下代码来自博客：[为什么(i++)不能做左值，而(++i)可以](https://link.zhihu.com/?target=http%3A//blog.csdn.net/zlhy_/article/details/8349300)

```cpp
// 前缀形式：
int& int::operator++() //这里返回的是一个引用形式，就是说函数返回值也可以作为一个左值使用
{//函数本身无参，意味着是在自身空间内增加1的
  *this += 1;  // 增加
  return *this;  // 取回值
}

//后缀形式:
const int int::operator++(int) //函数返回值是一个非左值型的，与前缀形式的差别所在。
{//函数带参，说明有另外的空间开辟
  int oldValue = *this;  // 取回值
  ++(*this);  // 增加
  return oldValue;  // 返回被取回的值
}
```

如上所示，i++ 最后返回的是一个临时变量，而**临时变量是右值**。



#### 何为左值、右值？

> 左值：
>
> C/C++语言中可以放在赋值符号左边的变量，即具有对应的可以由用户访问的存储单元，并且能够由用户去改变其值的量。左值表示存储在计算机内存的对象，而不是常量或计算的结果。或者说左值是代表一个内存地址值，并且通过这个内存地址，就可以对内存进行读并且写（主要是能写）操作；
>
> 这也就是为什么左值可以被赋值的原因了。相对应的还有右值：当一个符号或者常量放在操作符右边的时候，计算机就读取他们的“右值”，也就是其代表的真实值。简单来说就是，左值相当于地址值，右值相当于数据值



这个[知乎问题](https://www.zhihu.com/question/26203703)问的很好，所谓的左值，也就是有内存地址的值，那么int a = 2；int b = a；a与2相比，多了内存占用（毕竟可以访问？）。有个[博客](https://www.cnblogs.com/catch/p/3500678.html)对左值右值探讨的细致一些。

