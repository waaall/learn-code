# learn web

[toc]

> 现代浏览器内核利用操作系统实现的socket等几乎全部API，实现了非常复杂的功能，以至于，它就像一个操作系统，但它是建立在操作系统的API之上的另一层非常全面的高层抽象。使用HTML、CSS、JavaScript这“三种语言”(除了Javascript都不算是个语言)，在任意操作系统和平台上，都能实现几乎一个APP的全部功能，这也是Web-App的由来。

## 谈Web-App与Native-App

基于Web的app，必然造成了很多性能损失，本身Javascript就不够快，再加上很多API并不是系统原生API，而是又经过浏览器内核这一层“转译”，Web-App比原生app要慢得多。

但是有舍就有得，Web的开发效率极快，原因有三：

1. Javascript 和 HTML本身相比c++、java、c#等上手快，语法逻辑简单，还是解释型语言，配置同样简单。
2. 完全不需要担心跨平台问题：Apple你要用swift，微软用c#，Google用Java，还都会穿插着c++。做一个跨平台app，非常不容易，维护起来更费劲。而基于浏览器内核和Web-API的实现，相对容易得多。
3. 很多应用对效率、性能要求不高，但它们对开发周期很敏感。

### 跨平台app

跨平台做app，当然可以只用一门较为高效的主流语言，比如`c++`或`Java`，性能高，后期维护方便，但是有一个重要的问题，各平台都有“适合”的语言不仅或根本不是其语言特性的优势特别巨大，而是其背后的母公司API对这门语言的优化、和围绕这门语言的整个生态。

当然高效API和生态的发展也的确证明这门语言特性有一定优势，比如当年`Java`之于`c++`、比如`swift`之于`Objective-c`、比如`python`和`Javascript`等解释语言的发展。

语言太多，平台就那么几个：Apple、Google、Linux、Microsoft、Web-Server。每个平台都有自己的开发“生态”，这就决定在其生态必然会获得最高效的开发体验，前提是，你不跨平台。

软件市场有个很普遍的现象：很多功能强大的跨平台应用，多数都存在更新慢、适配慢的问题；而很多某平台专有的app，往往更新节奏很快。当然大公司的跨平台app也可以更新很快，这就取决于“钞能力”和重视程度了。所以，Web-App可能不会成为下一个发展潮流，但可以有一席之地。

具体C++、Java、Web写的跨平台应用，GUI都用什么库呢：当然，Web本身就是基于浏览器的，当然就是用的浏览器的GUI渲染，所以下面的Electronic是一个基于Web的开发套件。

#### [**Swing-Java**](https://zh.wikipedia.org/wiki/Swing_(Java))





#### [**Qt-C++**](https://zh.wikipedia.org/wiki/Qt)

关于Qt的学习，具体请参照`learn-GUI.md`。

[Qt的优点](https://www.zhihu.com/question/20153991)：

> 作者：殷允峤
> 链接：https://www.zhihu.com/question/20153991/answer/17835646
> 来源：知乎
> [Mark Shuttleworth](https://link.zhihu.com/?target=http%3A//www.markshuttleworth.com/archives/568) 宣布Ubuntu要支持Qt程序，给出的理由是软件的易用性和方便集成的能力，是提供最终用户体验的关键价值所在。Ubuntu不是因为Gtk多么"纯粹", 多么牛B，多么性感才被选中的，Ubuntu选择的是像OpenOffice, Firefox这样体验上佳的软件，软件的技术框架只是附加选择。当初之所以选择Gnome是因为在当时条件下, Gnome桌面和其软件家族，在功能和系统体验的一致性上很好，而GTK被选择，则仅仅是Gnome使用了GTK，如此而已。
>
> 然后Mark给出选择软件框架的几个要求和Qt入选的理由：
> \* is it free software? Qt支持商业, GPL和LGPL三种授权，特别是LGPL称为决定性的胜出因素
> \* is it best-in-class?  Qt在软件质量，功能，API设计，文档等方面广受赞誉
> \* does it integrate with the system settings and preferences? 当时Qt在这方面还稍有欠缺，为此Canonical决定赞助[dconf - GNOME Wiki!](https://link.zhihu.com/?target=https%3A//wiki.gnome.org/dconf)这样的软件集成进Qt, 结果就是[dconf-qt in Launchpad](https://link.zhihu.com/?target=https%3A//launchpad.net/dconf-qt)
> \* does it integrate with other applications?  原文未说明，不过这对Qt程序没什么问题，现在Qt和GTK基本上都统一到了D-BUS作为应用交互的系统总线了
> \* is it accessible to people who cannot use a mouse, or keyboard?  Qt给出的答案是QtQuick QML
> \* does it look and feel consistent with the rest of the system? Qt Style， QML表示没有压力
>
> 那么，选择Qt是否意味着Ubuntu要亲近KDE而疏远GNOME了呢？
> Mark对前者给予斩钉截铁地回答：No。 并且用整整一个段落来斩断这种遐想，表面上给出的理由是，KDE并没有集成像dconf-qt这样的软件包，而Ubuntu也无法或不想一个个去说服KDE的程序去集成进Ubuntu。而骨子里，其实Mark就是看不上KDE，随后不久, Canonical宣布解雇全职开发Kunbutu唯一的一个雇员，然后Kunbutu被迫变为完全社区驱动的发行版就是一个很明显的注解。
>
> 那么，看不上KDE这小三是不是就是因为爱GNOME这大老婆爱得深沉呢？ 是才怪！虽然为了安抚GNOME社区，Mark用了另一个整段来说明选择Qt不等于弃GNOME，当然了，一起生活了这么久，即使一下子想休也不是那么容易休掉的，要慢慢来嘛！ GNOME、GTK推3.x, 咱先不跟了，Gnome shell再慢慢用Unity换掉，嗯，X11, Wayland都不合口味，也扔掉，上Mir.... 哇，这下有点玩大了，釜底抽薪靠什么支持上面的应用, 这是就需要Qt登场了，有了锤炼了好几年的QPA系统抽象层，换底层架构就方便多了。
>
> 有了这些铺垫，今年初当Canonical宣布她的Ubuntu Phone OS就水到渠成了，被选择的，既不是GTK和GNOME，更加不是KDE，而是最为一个跨越桌面，平板和手持设备的整体系统解决方案的Qt框架，如果你曾经开发过Symbian，MeeGo的Qt程序，当你打开Ubuntu Phone OS的SDK时，你会觉得非常熟悉，没错，Canonical原封不动地把Qt Mobility, QtQuick和Qt其它基础模块一股脑地接了过来。Qt将成为Ubuntu未来地官方API， QML则是跨桌面，平板，手机的原生UI解决方案。
>
> 这是从Ubuntu的发展和Canonical公司的战略角度来看Qt被选择的背后动机， 如果单纯的比较GTK和Qt的话，Qt在以下方面是胜出的（当然不是各个方面都是Qt好，GTK也有自己的优势）：
> \1. 灵活的许可证
>     要说这是最核心的，狗血的是，Qt当初不灵活的QPL许可证也是催生GNOME/GTK的最大原因，然而此一时彼一时，Qt一直在进步，先是商业+GPL, 后被Nokia加上LGPL， 已经成了最开放的开源项目之一，无论你是开源，闭源和商业开发者，都可以放心使用Qt。GTK是LGPL，可以被闭源程序使用，但是没有真正的商业License，基本上除了雇程序员自给自足，找不到地方花钱买服务，这对很多项目其实很关键。
>
> \2. 社区活跃度
>     Qt核心活跃开发者基本上是GTK的4+倍, 代码commits是GTK的5+倍  (2011年数据），随着Nokia弃用Qt，2012-2013的统计有所下降，不过还是比GTK高一个数量级，大家可以看Ohloh的详细统计数据
>
> 3.性能
>    其实传统的Qt Widgets和GTK相比，性能上彼此彼此。不过QtQuick出现以后，Qt开始甩开GTK好几条街，更确切的说，GTK社区没有拿出相对应的解决方案。
>
> \4. 分辨率无关开发
> QtQuick/QML对现代的移动，嵌入式触摸设备的GUI开发是必不可少的。现在的显示设备的另一个特点是，像素密度很大，而且不同设备像素密度差异也很大，即使是桌面平台，随着retina的出现，也开始了DPI竞赛。而GTK+不支持“分辨率无关”开发，这样GTK写出来的程序，在现在的智能手机上看起来就像屎堆上的一对苍蝇... 当然Qt Widgets写出来的也是一样，这就体现出了QtQuick的优势来。而据说，GTK+对此的计划是... GTK4.x再提供支持...... 这样以来，估计用不了多久，连GTK桌面程序看起来也只能是渣渣了。
>
> \5. 可移植性
> 这也是Qt的王牌之一， 除了三大桌面外，Qt还支持几乎所有主流智能手机操作系统（让我们略过Windows Phone...）, 刚刚发布的Qt 5.1已经尝试支持Android和iOS平台，Qt 5.2将正式支持Android和iOS, 加上本就支持的*Symbian(dead)*, *MeeGo(dead)*, Tizen, *WebOS*, BlackBerry/QNX, VxWorks, Ubuntu Phone OS, Sailfish OS。而GTK只能勉强支持三大桌面(Linux/Unix基本上只能X11, Framebuffer支持有限，EglFS完全不支持）。
>
> \6. 其它
>     qt-creator 已经渐渐地称为了VS之外的最佳C/C++ IDE, Qt还有开源社区非常少见的优美丰富的文档，Qt框架覆盖广泛，不仅仅限于GUI，就这点来说拿Qt和GTK比，其实有点委屈了Qt。

Qt的缺点：

> https://softwareengineering.stackexchange.com/questions/88685/why-arent-more-desktop-apps-written-with-qt
>
> I don't really intend this to be a bashing answer, but these are the reasons I do not personally use Qt. There are plenty of good things to say about it -- namely that the API works most of the time, and that it does seamlessly bridge platforms. But I do not use Qt, because:
>
> 1. In some cases, it just doesn't look like native programs look. Designing a single UI for all platforms inherently is not going to look right when moved from machine to machine, for various visual styling reasons. For example, on Mac machines, split bars are usually relatively thick, and buttons are small and rounded with icons. On Windows machines, split bars are typically narrow, and buttons are more textual, with more square designs. Just because you can write one UI for every platform does not mean that you should for most applications.
> 2. Qt is not a C++ library. It requires a separate compilation step, which makes the build process much more complicated when compared with most other libraries.
> 3. As a result of (2), C++ IDEs and tools can flag Qt expressions as errors, because they do not understand Qt's specifics. This almost forces use of QtCreator or a textual only editor like `vim`.
> 4. Qt is a large amount of source, which must be present and preinstalled on any machine you use before compiling. This can make setting up a build environment much more tedious.
> 5. It's available only under LGPL, which makes it difficult to use single-binary-deployment when one needs to release under a more restrictive or less restrictive license.
> 6. It produces extremely large compiled binaries when compared with similarly written "plain ol' native applications" (excepting of course applications written for KDE).

有不少较大型的c++软件项目使用Qt作为GUI开发库：

- [Adobe Photoshop Album](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Adobe_Photoshop_Album)[[37\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-37)
- [Adobe Photoshop Elements](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Adobe_Photoshop_Elements)[[38\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-38)
- AMD's [Radeon Software](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Radeon_Software) Crimson Edition driver tool application.[[39\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-39)
- [Autodesk Maya](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Autodesk_Maya)[[40\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-40)
- [Autodesk 3ds Max](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Autodesk_3ds_Max)[[41\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-41)
- [Bitcoin Core](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Bitcoin_Core), a bitcoin client
- [Bitcoin ABC](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Bitcoin_Cash), a bitcoin cash client
- [CryEngine V](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/CryEngine) editor[[42\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-42)
- [DaVinci Resolve](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/DaVinci_Resolve), a video editor
- [Dolphin (emulator)](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Dolphin_(emulator)), an emulator for the [Nintendo Wii](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Nintendo_Wii) and [Nintendo GameCube](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Nintendo_GameCube)systems.
- [Dorico](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Dorico) notation software
- [Dragonframe](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Dragonframe)[[43\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-43) stop motion animation software
- [EAGLE](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/EAGLE_(program)) by [CadSoft Computer](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/CadSoft_Computer) / [Autodesk](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Autodesk), an EDA application with schematic capture, PCB layout, auto-router and CAM features
- Electrum, a lightweighed bitcoin client
- [FreeMat](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/FreeMat) free open source numerical computing environment
- [Google Earth](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Google_Earth)[[44\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-44)
- [Krita](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Krita) graphics editing and digital painting software[[45\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-45)
- [LMMS](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/LMMS), a cross-platform music production software
- [Mathematica](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Mathematica), a mathematical symbolic computation program, sometimes termed a computer algebra system or program, used in many scientific, engineering, mathematical, and computing fields.[[46\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-46)
- Moonlight Stream, an open-source implementation of Nvidia Shield[[47\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-47)
- [OBS](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Open_Broadcaster_Software), a libre cross-platform screencast software
- [Orange](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Orange_(software)) data mining suite[[48\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-48)
- [QGIS](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/QGIS) geographic information system[[49\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-49)
- [QuiteRSS](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/QuiteRSS) Feed Reader
- [Scribus](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Scribus) desktop publishing software[[50\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-50)
- [Sibelius](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Sibelius_(software)) music composition and notation software[[51\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-51)
- [Source 2](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Source_2) engine tools[[52\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-52) a 3D video game engine developed by [Valve Corporation](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Valve_Corporation)
- [Stellarium](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Stellarium_(software)), a planetarium program
- [Subsurface](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Subsurface_(software)), a software for logging and planning scuba dives initially designed and developed by Linus Torvalds[[53\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-53)
- [Teamviewer](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Teamviewer), a computer software package for remote control, desktop sharing, online meetings, web conferencing and file transfer between computers[[54\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-54)
- [Telegram](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Telegram_(software)), a messaging client available for Windows, Mac and Linux[[55\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-55)
- [VirtualBox](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/VirtualBox) OS virtualization software[[56\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-56)
- [VLC media player](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/VLC_media_player)[[57\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-57)
- [WPS Office](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/WPS_Office)[[58\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-58)
- [XnView MP](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/XnView_MP)[[59\]](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Qt_%28software%29%23cite_note-59)


#### [**Electron-JS**](https://www.electronjs.org/docs)

Electron优点：

> 

Electron缺点：

> 

[使用Electron开发的App](https://www.electronjs.org/apps)：VScode、WhatsApp、Atom、Slack......

<img src="learn-web.assets\app-use-electron.png" alt="app-use-electron" style="zoom:60%;" />

## 浏览器内核与JS引擎







## [Web](https://developer.mozilla.org/zh-CN/docs/learn)-API





## [Electron](https://www.electronjs.org/)



