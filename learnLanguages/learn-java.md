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



## JAVA与c++

