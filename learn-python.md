# learn python

[toc]

---


## 一个很基本的问题

函数递归、类的自引用，是怎么样的逻辑？
这个问题见learn c++

[一个讲python不错的网站](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/p04_match_and_search_text.html)

### brew安装python

```bash

Python has been installed as
 /usr/local/bin/python3
Unversioned symlinks `python`, `python-config`, `pip` etc. pointing to
`python3`, `python3-config`, `pip3` etc., respectively, have been installed into
 /usr/local/opt/python/libexec/bin
 
You can install Python packages with
 pip3 install <package>
They will install into the site-package directory
 /usr/local/lib/python3.7/site-packages
See: https://docs.brew.sh/Homebrew-and-Python
```

python文件有中文的话最好以这个开头，避免编码错误
```python
# -*- coding: utf-8 -*-
```

### windows 安装与升级python

Windows下不要安装在`C:\Program Files`下，也尽量不要安装在`Users\AppData`文件夹下，原因如下：

1. 在`C:\Program Files`时，Windows默认不给软件再在这个文件夹内写入数据的“机会”，这本是好意，但这是我们用pip安装python包时，就会发现它提示没有权限，而给写入了`Users\AppData\Roaming\Python\python39` 文件夹，库的可执行文件放在此文件夹下的`Scripts`， 库源码在此文件夹下的`site-packages`，但是该文件夹并没有加入环境变量，又会造成很多麻烦，比如`ipython`没法用之类的。**这还不是唯一的问题：** 当你卸载python时，它并没有给你删除`Users\AppData\Roaming\Python`这个文件夹，而这些库文件非常大，且没有利用价值。
2. 直接安装在用户文件夹下避免了上述问题，但是这不好管理，所以建议在系统盘下可以单独建一个开发环境文件夹，python同样安装在此。

## Framework

---

接触一门新的语言我会疑惑很多问题，但有几个是非常根本的问题：

1. **这个语言是怎么让机器识别运行的?**

也就是python ‘编译’过程。

* [python的所有源代码](https://github.com/python/cpython)

上文链接中的代码就是python的所有源代码，包括几大部分，一个是c语言编写的解释器，还有c语言编写的部分内置模块，另一大部分就是python语言编写的内置库，是学习c语言和python的最好的代码。

这其中就有一个**关键的问题**：c语言并不是面向对象，也就是没有类和对象这些定义，那怎么解释python中的面向对象部分呢？

答案也很简单：c++的面向对象是怎么来的？其实这只是计算机语言体系众多抽象概念的一个罢了——函数是什么？编译器或者解释器怎么解释？list是什么？string是什么？那么类就是什么！也就是一部分代码的集合。

对于机器来说，没有函数和类这些概念效率会更高，而对于人类，一段机器码（或者c语言中变量声明，赋值，循环，）实现了一个功能，如果把它打包做一个标签就是再好不过的了，这就是函数，这也是类。这个包。就是申请的一段连续内存（当然虚拟内存技术先忽略）

举个例子：一辆汽车，大家可能都想象得到是什么，但其实没几个人懂，你知道汽车怎么实现转向的吗？恐怕不知道，但甚至没有驾照的人都会转动方向盘，这个方向盘，就是函数，这个车就是类。这就是为什么大多数人不懂的车的工作原理（不知道类的所有原代码），但却能开车（可以进行类的实例化），也能转弯（调用类底下的函数/方法），而且换一个车基本还能开（类的继承）

推广一下，其实计算机和机械工程如出一辙，都是利用了物理规律，实现了一些小的功能，然后一层层抽象，最终从晦涩难懂的自然规律，整体实现了极其“人性化”功能。

When I writting English with Typora, It can auto pair the brackets, but this function doesn't work in Chinese. So, I have to try to type in English. 

* [一个关于python运行原理的文章](https://zhuanlan.zhihu.com/p/35746181)
* [learn python 一个不错的教程网站](https://www.programiz.com/python-programming)
* [python为什么不用编译器？](https://www.zhihu.com/question/33960035)

学过c语言都应该知到c是怎么被一步步的转化成机器码交给计算机运行的，编译，链接，汇编，
但是python是怎么到机器码的呢？和c并不相同。这主要是动态语言和静态语言的问题。

2. **这个语言和其他主流语言的区别在哪里?**

主要是指的机器码（cpu指令集），汇编，c，c++，python，这条发展线

因为这们几个的变迁，几乎代表了代码结构的发展，从晶体管排布成加法器，累加器等等，再到cpu指令集，再到汇编语言，c语言，c++。
最初只有逻辑和算数操作，整数和浮点数据格式，到循环出现，list出现，再到class概念的产生，最后python实现了动态编译的灵活性的突破（当然不是python第一个实现的，但它正如c++是面向对象的标志性语言一样，python也可以作为是遗留下来的‘成熟’完善的动态编译语言的代表）。

还有一个问题也就是内存管理，指针，堆栈等更接近底层的功能存在与否。

3. **这个（所有语言）语言怎么实现复杂的软件框架，而不只是写一个脚本**

这恐怕是python经常被质疑的地方，c++和java是如此的‘强大’，python又由于动态语言的特性性能可能较差，又同时有着简洁优雅的代码结构和强大的扩展库，导致python‘大材小用’只用来写脚本。
我觉得每一个通用编程语言都有着强大的能力，主要还是实现功能的人。

4. **Python写文件时，我们发现它是无法直接内容前追加新内容的，why？** 

如果底层可以实现，这个write函数必然早就更新了（可以直接追加内容），我们进一步站在硬盘及文件系统的角度，想象下为什么这个看似“很人性化很简单”的操作却不可以实现：

首先硬盘就是一系列可以和光或电磁反应产生“相变”的“微观”材料，组成了“位”。而文件系统就是把这些位解释为一个个文件和文件里的内容。而这些看起来可以随意操作，随意更改的内容，在底层则必须为顺序的，严格按照一定格式的位的集合。（乱序储存也是用标记，来让其“变成”顺序读写的）。

PS：文件内容，完全是字符串编码。也就意味着：你看似写了一个Python或C++文件，里面定义了一个“数组”或“整数”，但这是编译器把这些“字符串”解释为“数组”或“整数”而已。在文件中，它们就只是字符串。

比如一个文件，在硬盘里就是一段连续的位。而位的开始某些位（确定长度或者用特殊标记字符来指定始末）就分别代指文件名，文件位置，文件类型，访问权限等一系列信息，而“紧接着”就是文件内容，所以，如何才能在文件内容开始插入一段新内容，却不改变原始内容呢？这非常难，难到现在都没有被解决。

所以现在看到的：我们在任何可以写入的app中，比如Office，记事本，Markdown编辑器等等，只要进行文件读写的，我们好像都可以随意在任意位置插入一段数据，而原始数据保持不变，但在底层，这些文件的更改不是这么简单的（哪怕你在文件开头只插入了一个字符，保存时都需要把之后的全部内容先读取到内存里，再写入才行）

| fdi  | ff   |      |
| ---- | ---- | ---- |
|      |      |      |
|      |      |      |

 

## python基础概念


### DFT [傅里叶变换](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html)、


### 处理异常

例子：最后的 except 子句可以省略异常名，以用作通配符。但请谨慎使用，因为以这种方式很容易掩盖真正的编程错误！它还可用于打印错误消息，然后重新引发异常（同样允许调用者处理异常）:

```python
import sys

try:
    f = open('myfile.txt')
    s = f.readline()
    i = int(s.strip())
except OSError as err:
    print("OS error: {0}".format(err))
except ValueError:
    print("Could not convert data to an integer.")
except:
    print("Unexpected error:", sys.exc_info()[0])
    raise
```

首先，异常是一个对象，一般由一个基类

#### 异常类型

即使语句或表达式在语法上是正确的，但在尝试执行时，它仍可能会引发错误。 在执行时检测到的错误被称为 *异常*，异常不一定会导致严重后果：你将很快学会如何在 Python 程序中处理它们。 但是，大多数异常并不会被程序处理，此时会显示如下所示的错误信息:

\>>>

```bash
>>> 10 * (1/0)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ZeroDivisionError: division by zero
>>> 4 + spam*3
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'spam' is not defined
>>> '2' + 2
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: Can't convert 'int' object to str implicitly
```

错误信息的最后一行告诉我们程序遇到了什么类型的错误。异常有不同的类型，而其类型名称将会作为错误信息的一部分中打印出来：上述示例中的异常类型依次是：[`ZeroDivisionError`](https://docs.python.org/zh-cn/3/library/exceptions.html#ZeroDivisionError)， [`NameError`](https://docs.python.org/zh-cn/3/library/exceptions.html#NameError) 和 [`TypeError`](https://docs.python.org/zh-cn/3/library/exceptions.html#TypeError)。作为异常类型打印的字符串是发生的内置异常的名称。对于所有内置异常都是如此，但对于用户定义的异常则不一定如此（虽然这是一个有用的规范）。标准的异常类型是内置的标识符（而不是保留关键字）。

这一行的剩下的部分根据异常类型及其原因提供详细信息。

错误信息的前一部分以堆栈回溯的形式显示发生异常时的上下文。通常它包含列出源代码行的堆栈回溯；但是它不会显示从标准输入中读取的行。



[内置异常](https://docs.python.org/zh-cn/3/library/exceptions.html#bltin-exceptions) 列出了内置异常和它们的含义。

- [内置异常](https://docs.python.org/zh-cn/3/library/exceptions.html#exception-hierarchy)
  - [基类](https://docs.python.org/zh-cn/3/library/exceptions.html#base-classes)
  - [具体异常](https://docs.python.org/zh-cn/3/library/exceptions.html#concrete-exceptions)
    - [OS 异常](https://docs.python.org/zh-cn/3/library/exceptions.html#os-exceptions)
  - [警告](https://docs.python.org/zh-cn/3/library/exceptions.html#warnings)
  - [异常层次结构](https://docs.python.org/zh-cn/3/library/exceptions.html#exception-hierarchy)

#### 抛出异常

往上抛异常也是十分有必要的，这会让调用此方法（类）的开发者看到异常，并想办法处理等。

[`raise`](https://docs.python.org/zh-cn/3/reference/simple_stmts.html#raise) 语句允许程序员强制发生指定的异常。例如:

```bash
>>> raise NameError('HiThere')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: HiThere
```

[`raise`](https://docs.python.org/zh-cn/3/reference/simple_stmts.html#raise) 唯一的参数就是要抛出的异常。这个参数必须是一个异常实例或者是一个异常类（派生自 [`Exception`](https://docs.python.org/zh-cn/3/library/exceptions.html#Exception) 的类）。如果传递的是一个异常类，它将通过调用没有参数的构造函数来隐式实例化:

```python
raise ValueError  # shorthand for 'raise ValueError()'
```

如果你需要确定是否引发了异常但不打算处理它，则可以使用更简单的 [`raise`](https://docs.python.org/zh-cn/3/reference/simple_stmts.html#raise) 语句形式重新引发异常

```bash
>>> try:
...     raise NameError('HiThere')
... except NameError:
...     print('An exception flew by!')
...     raise
...
An exception flew by!
Traceback (most recent call last):
  File "<stdin>", line 2, in <module>
NameError: HiThere
```



#### 用户自定义异常

程序可以通过创建新的异常类来命名它们自己的异常（有关Python 类的更多信息，请参阅 [类](https://docs.python.org/zh-cn/3/tutorial/classes.html#tut-classes)）。异常通常应该直接或间接地从 [`Exception`](https://docs.python.org/zh-cn/3/library/exceptions.html#Exception) 类派生。

可以定义异常类，它可以执行任何其他类可以执行的任何操作，但通常保持简单，通常只提供许多属性，这些属性允许处理程序为异常提取有关错误的信息。在创建可能引发多个不同错误的模块时，通常的做法是为该模块定义的异常创建基类，并为不同错误条件创建特定异常类的子类:

```python
class Error(Exception):
    """Base class for exceptions in this module."""
    pass

class InputError(Error):
    """Exception raised for errors in the input.

    Attributes:
        expression -- input expression in which the error occurred
        message -- explanation of the error
    """

    def __init__(self, expression, message):
        self.expression = expression
        self.message = message

class TransitionError(Error):
    """Raised when an operation attempts a state transition that's not
    allowed.

    Attributes:
        previous -- state at beginning of transition
        next -- attempted new state
        message -- explanation of why the specific transition is not allowed
    """

    def __init__(self, previous, next, message):
        self.previous = previous
        self.next = next
        self.message = message
```

大多数异常都定义为名称以“Error”结尾，类似于标准异常的命名。

许多标准模块定义了它们自己的异常，以报告它们定义的函数中可能出现的错误。有关类的更多信息，请参见类 [类](https://docs.python.org/zh-cn/3/tutorial/classes.html#tut-classes)。







### if __name__ == '__main__':

[这是什么意思呢？](https://blog.konghy.cn/2017/04/24/python-entry-program/)、[Stackoverflow回答](https://stackoverflow.com/questions/4042905/what-is-main-py)、



### super函数

> [下面来自一个不错的总结文章](https://python3-cookbook.readthedocs.io/zh_CN/latest/c08/p07_calling_method_on_parent_class.html):
> 为了调用父类(超类)的一个方法，可以使用 `super()` 函数，比如：
>
> ```python
> class A:
>  def spam(self):
>      print('A.spam')
> 
> class B(A):
>  def spam(self):
>      print('B.spam')
>      super().spam()  # Call parent spam()
> ```
>
> `super()` 函数的一个常见用法是在 `__init__()` 方法中确保父类被正确的初始化了：
>
> ```python
> class A:
>  def __init__(self):
>      self.x = 0
> 
> class B(A):
>  def __init__(self):
>      super().__init__()
>      self.y = 1
> ```
>
> `super()` 的另外一个常见用法出现在覆盖Python特殊方法的代码中，比如：
>
> ```python
> class Proxy:
>  def __init__(self, obj):
>      self._obj = obj
> 
>  # Delegate attribute lookup to internal obj
>  def __getattr__(self, name):
>      return getattr(self._obj, name)
> 
>  # Delegate attribute assignment
>  def __setattr__(self, name, value):
>      if name.startswith('_'):
>          super().__setattr__(name, value) # Call original __setattr__
>      else:
>          setattr(self._obj, name, value)
> ```
>
> 在上面代码中，`__setattr__()` 的实现包含一个名字检查。 如果某个属性名以下划线(_)开头，就通过 `super()` 调用原始的 `__setattr__()` ， 否则的话就委派给内部的代理对象 `self._obj` 去处理。 这看上去有点意思，因为就算没有显式的指明某个类的父类， `super()` 仍然可以有效的工作。
>
> 实际上，大家对于在Python中如何正确使用 `super()` 函数普遍知之甚少。 你有时候会看到像下面这样直接调用父类的一个方法：
>
> ```python
> class Base:
>     def __init__(self):
>         print('Base.__init__')
> 
> class A(Base):
>     def __init__(self):
>         Base.__init__(self)
>         print('A.__init__')
> ```
>
> 尽管对于大部分代码而言这么做没什么问题，但是在更复杂的涉及到多继承的代码中就有可能导致很奇怪的问题发生。 比如，考虑如下的情况：
>
> ```python
> class Base:
>     def __init__(self):
>         print('Base.__init__')
> 
> class A(Base):
>     def __init__(self):
>         Base.__init__(self)
>         print('A.__init__')
> 
> class B(Base):
>     def __init__(self):
>         Base.__init__(self)
>         print('B.__init__')
> 
> class C(A,B):
>     def __init__(self):
>         A.__init__(self)
>         B.__init__(self)
>         print('C.__init__')
> ```
>
> 如果你运行这段代码就会发现 `Base.__init__()` 被调用两次，如下所示：
>
> ```python
> >>> c = C()
> Base.__init__
> A.__init__
> Base.__init__
> B.__init__
> C.__init__
> >>>
> ```
>
> 可能两次调用 `Base.__init__()` 没什么坏处，但有时候却不是。 另一方面，假设你在代码中换成使用 `super()` ，结果就很完美了：
>
> ```python
> class Base:
>     def __init__(self):
>         print('Base.__init__')
> 
> class A(Base):
>     def __init__(self):
>         super().__init__()
>         print('A.__init__')
> 
> class B(Base):
>     def __init__(self):
>         super().__init__()
>         print('B.__init__')
> 
> class C(A,B):
>     def __init__(self):
>         super().__init__()  # Only one call to super() here
>         print('C.__init__')
> ```
>
> 运行这个新版本后，你会发现每个 `__init__()` 方法只会被调用一次了：
>
> ```python
> >>> c = C()
> Base.__init__
> B.__init__
> A.__init__
> C.__init__
> >>>
> ```
>
> 为了弄清它的原理，我们需要花点时间解释下Python是如何实现继承的。 对于你定义的每一个类，Python会计算出一个所谓的方法解析顺序(MRO)列表。 这个MRO列表就是一个简单的所有基类的线性顺序表。例如：
>
> ```python
> >>> C.__mro__
> (<class '__main__.C'>, <class '__main__.A'>, <class '__main__.B'>,
> <class '__main__.Base'>, <class 'object'>)
> >>>
> ```
>
> 为了实现继承，Python会在MRO列表上从左到右开始查找基类，直到找到第一个匹配这个属性的类为止。
>
> 而这个MRO列表的构造是通过一个C3线性化算法来实现的。 我们不去深究这个算法的数学原理，它实际上就是合并所有父类的MRO列表并遵循如下三条准则：
>
> - 子类会先于父类被检查
> - 多个父类会根据它们在列表中的顺序被检查
> - 如果对下一个类存在两个合法的选择，选择第一个父类
>
> 老实说，你所要知道的就是MRO列表中的类顺序会让你定义的任意类层级关系变得有意义。
>
> 当你使用 `super()` 函数时，Python会在MRO列表上继续搜索下一个类。 只要每个重定义的方法统一使用 `super()` 并只调用它一次， 那么控制流最终会遍历完整个MRO列表，每个方法也只会被调用一次。 这也是为什么在第二个例子中你不会调用两次 `Base.__init__()` 的原因。
>
> `super()` 有个令人吃惊的地方是它并不一定去查找某个类在MRO中下一个直接父类， 你甚至可以在一个没有直接父类的类中使用它。例如，考虑如下这个类：
>
> ```python
> class A:
>     def spam(self):
>         print('A.spam')
>         super().spam()
> ```
>
> 如果你试着直接使用这个类就会出错：
>
> ```python
> >>> a = A()
> >>> a.spam()
> A.spam
> Traceback (most recent call last):
>     File "<stdin>", line 1, in <module>
>     File "<stdin>", line 4, in spam
> AttributeError: 'super' object has no attribute 'spam'
> >>>
> ```
>
> 但是，如果你使用多继承的话看看会发生什么：
>
> ```python
> >>> class B:
> ...     def spam(self):
> ...         print('B.spam')
> ...
> >>> class C(A,B):
> ...     pass
> ...
> >>> c = C()
> >>> c.spam()
> A.spam
> B.spam
> >>>
> ```
>
> 你可以看到在类A中使用 `super().spam()` 实际上调用的是跟类A毫无关系的类B中的 `spam()` 方法。 这个用类C的MRO列表就可以完全解释清楚了：
>
> ```python
> >>> C.__mro__
> (<class '__main__.C'>, <class '__main__.A'>, <class '__main__.B'>,
> <class 'object'>)
> >>>
> ```
>
> 在定义混入类的时候这样使用 `super()` 是很普遍的。可以参考8.13和8.18小节。
>
> 然而，由于 `super()` 可能会调用不是你想要的方法，你应该遵循一些通用原则。 首先，确保在继承体系中所有相同名字的方法拥有可兼容的参数签名(比如相同的参数个数和参数名称)。 这样可以确保 `super()` 调用一个非直接父类方法时不会出错。 其次，最好确保最顶层的类提供了这个方法的实现，这样的话在MRO上面的查找链肯定可以找到某个确定的方法。
>
> 在Python社区中对于 `super()` 的使用有时候会引来一些争议。 尽管如此，如果一切顺利的话，你应该在你最新代码中使用它。 Raymond Hettinger为此写了一篇非常好的文章 [“Python’s super() Considered Super!”](http://rhettinger.wordpress.com/2011/05/26/super-considered-super) ， 通过大量的例子向我们解释了为什么 `super()` 是极好的。





#### 为什么有时候同样的python指令执行后却不一样？

<img src="learn-python.assets/Screen Shot 2020-07-27 at 10.09.04.png" alt="Screen Shot 2020-07-27 at 10.09.04" style="zoom:33%;" />

[一个很棒的知乎解答](https://www.zhihu.com/question/53536750)：

> 1. python中的数据类型 ---可变与不可变
> 2. python中is比较与==比较的区别
> 3. python中对小整数的缓存机制
> 4. python程序的结构块---代码块
> 5. python的内存管理---新建对象时的操作

1. **python中的数据类型 ---可变与不可变**

python中任何类型都是对象，这是为什么？想想c++的运算符重载和string类就懂了：要想一个基本类型拥有像python数据类型这么多特性是不可能的，也就是必须要定义类，所以你定义的每个量便是一个对象了。

但是对于这些对象，有的是可变的，具有更多灵活性，其实也就是把几个类型做在了一个基类下，当然，python解释器不是用c++写的，而是c写的，有人会说，c里根本就没有类的定义！但是c++的类也是被gcc/clang编译器解释成某一共享内存的代码段（虽然没这么简单，但我想说的是c++的类也是由c编译器打包好的，就像是c和汇编的关系）。所以python的解释器是将其解释为类的解释，所以思想是很相似的。

2. **python中is比较与==比较的区别**

既然是对象，那两个量在对比时就有对象id、type、value。而三者全部相等is才成立，而只需要value相等==就成立。

那就有了为什么a = b = 10的时候，a 和 b共享一个内存空间，也就是id也相等。所以会随之变化。

3. **python中对小整数的缓存机制**

> The current implementation keeps an array of integer objects for all integers between -5 and 256, when you create an int in that range you actually just get back a reference to the existing object. So it should be possible to change the value of 1. I suspect the behavior of Python in this case is undefined. 

也就是python 自动将-5到256的整数进行了缓存，当你讲这些整数赋给变量时，并不会重新创建对象，而是引用已经创建好的对象。



#### [python 动态变量](https://www.cnblogs.com/technologylife/p/9211324.html)









### List, Dict, Array, Series, DataFrame 相互转换

>[List, Dict, Array, Series, DataFrame 相互转换](https://yam.gift/2017/02/15/2017-02-15-list-dict-series-dataframe-ndarray-transform/)
>
>## 概要
>
>- dict to series
>- dict to dataframe
>- list to series
>- list to dataframe
>- list to array
>- array to dataframe
>- series to array
>- dataframe to array
>- dataframe to dict
>
>
>
>## 1. 输入 Dict
>
>- dict to series
>- dict to dataframe
>
>```
>#coding:utf-8
>
>import numpy as np
>import pandas as pd
>from pandas import Series, DataFrame
>
># dict
>data = { 'state': ['Ohino', 'Ohino', 'Ohino', 'Nevada', 'Nevada'],
>         'year': [2000, 2001, 2002, 2001, 2002],
>         'pop': [1.5, 1.7, 3.6, 2.4, 2.9]}  # type(data) 为 dict
>
># dict to series，若不指定 index，data 的 key 充当 Series 的 index
>ser = Series(data)
>
># dict to dataframe，若不指定 columns，data 的 key 充当 DataFrame 的 columns
>df = DataFrame(data)
>data
>{'pop': [1.5, 1.7, 3.6, 2.4, 2.9],
> 'state': ['Ohino', 'Ohino', 'Ohino', 'Nevada', 'Nevada'],
> 'year': [2000, 2001, 2002, 2001, 2002]}
># dict to series，若不指定 index，data 的 key 充当 Series 的 index
>ser = Series(data)
>ser
>pop                  [1.5, 1.7, 3.6, 2.4, 2.9]
>state    [Ohino, Ohino, Ohino, Nevada, Nevada]
>year            [2000, 2001, 2002, 2001, 2002]
>dtype: object
>ser.shape
>(3,)
># dict to dataframe，若不指定 columns，data 的 key 充当 DataFrame 的 columns
>df = DataFrame(data)
>df
>```
>
>|      | POP  | STATE  | YEAR |
>| :--- | :--- | :----- | :--- |
>| 0    | 1.5  | Ohino  | 2000 |
>| 1    | 1.7  | Ohino  | 2001 |
>| 2    | 3.6  | Ohino  | 2002 |
>| 3    | 2.4  | Nevada | 2001 |
>| 4    | 2.9  | Nevada | 2002 |
>
>## 2. 输入 List
>
>- list to series
>- list to dataframe
>- list to array
>
>```
>#coding:utf-8
>
>import numpy as np
>import pandas as pd
>from pandas import Series, DataFrame
>
># list
>data = [[2000, 'Ohino', 1.5],
>        [2001, 'Ohino', 1.7],
>        [2002, 'Ohino', 3.6],
>        [2001, 'Nevada', 2.4],
>        [2002, 'Nevada', 2.9]]  # type(data) 为 list
>
># list to series
>ser = Series(data, index = ['one', 'two', 'three', 'four', 'five'])
>
># list to dataframe
>df = DataFrame(data, index = ['one', 'two', 'three', 'four', 'five'], columns = ['year', 'state', 'pop'])
>
># list to array
>ndarray = np.array(data)
>type(data)
>list
>data
>[[2000, 'Ohino', 1.5],
> [2001, 'Ohino', 1.7],
> [2002, 'Ohino', 3.6],
> [2001, 'Nevada', 2.4],
> [2002, 'Nevada, 2.9']]
># list to series
>ser = Series(data, index = ['one', 'two', 'three', 'four', 'five'])
>ser
>one       [2000, Ohino, 1.5]
>two       [2001, Ohino, 1.7]
>three     [2002, Ohino, 3.6]
>four     [2001, Nevada, 2.4]
>five     [2002, Nevada, 2.9]
>dtype: object
>type(ser)
>pandas.core.series.Series
>ser.shape
>(5,)
># list to dataframe
>df = DataFrame(data, index = ['one', 'two', 'three', 'four', 'five'], columns = ['year', 'state', 'pop'])
>df
>```
>
>|           | YEAR | STATE  | POP  |
>| :-------- | :--- | :----- | :--- |
>| **one**   | 2000 | Ohino  | 1.5  |
>| **two**   | 2001 | Ohino  | 1.7  |
>| **three** | 2002 | Ohino  | 3.6  |
>| **four**  | 2001 | Nevada | 2.4  |
>| **five**  | 2002 | Nevada | 2.9  |
>
>```
>type(df)
>pandas.core.frame.DataFrame
>df.shape
>(5, 3)
># list to ndarray
>ndarray = np.array(data)
>ndarray
>array([['2000', 'Ohino', '1.5'],
>       ['2001', 'Ohino', '1.7'],
>       ['2002', 'Ohino', '3.6'],
>       ['2001', 'Nevada', '2.4'],
>       ['2002', 'Nevada', '2.9']],
>      dtype='|S21')
>ndarray.shape
>(5, 3)
>```
>
>## 3 输入 Array
>
>- array to dataframe
>
>```
>#coding:utf-8
>
>import numpy as np
>import pandas as pd
>from pandas import Series, DataFrame
>
># list
>data = [[2000, 'Ohino', 1.5],
>        [2001, 'Ohino', 1.7],
>        [2002, 'Ohino', 3.6],
>        [2001, 'Nevada', 2.4],
>        [2002, 'Nevada', 2.9]]  # type(data) 为 list
>
># list to array
>ndarray = np.array(data)
>
># array to dataframe
>pd = DataFrame(ndarray, index = ['one', 'two', 'three', 'four', 'five'], columns = ['year', 'state', 'pop'])
>ndarray
># 注意：array 中所有数据类型均是一样的
>array([['2000', 'Ohino', '1.5'],
>       ['2001', 'Ohino', '1.7'],
>       ['2002', 'Ohino', '3.6'],
>       ['2001', 'Nevada', '2.4'],
>       ['2002', 'Nevada', '2.9']],
>      dtype='|S21')
>pd
>```
>
>|           | YEAR | STATE  | POP  |
>| :-------- | :--- | :----- | :--- |
>| **one**   | 2000 | Ohino  | 1.5  |
>| **two**   | 2001 | Ohino  | 1.7  |
>| **three** | 2002 | Ohino  | 3.6  |
>| **four**  | 2001 | Nevada | 2.4  |
>| **five**  | 2002 | Nevada | 2.9  |
>
>## 4. 输入 DataFrame
>
>- series to array
>- dataframe to array
>- dataframe to dict
>
>```
>#coding:utf-8
>
>import numpy as np
>import pandas as pd
>from pandas import Series, DataFrame
>
># list
>data = [[2000, 'Ohino', 1.5],
>        [2001, 'Ohino', 1.7],
>        [2002, 'Ohino', 3.6],
>        [2001, 'Nevada', 2.4],
>        [2002, 'Nevada', 2.9]]  # type(data) 为 list
>
># list to series
>ser = Series(data, index = ['one', 'two', 'three', 'four', 'five'])
>
># list to dataframe
>df = DataFrame(data, index = ['one', 'two', 'three', 'four', 'five'], columns = ['year', 'state', 'pop'])
>
>
>
># series to array
>foo = ser.as_matrix()
>foo0 = Series.as_matrix(ser)
>
># dataframe to array, foo2 foo3 foo4 结果相同
>foo1 = DataFrame.as_matrix(df)
>foo2 = df.as_matrix()
>foo3 = df.values
>foo4 = np.array(df)
>
>foo5 = df.as_matrix(["pop"])
>
># dataframe to dict
># outtype的参数为‘dict’、‘list’、‘series’和‘records’。
># dict返回的是dict of dict；list返回的是列表的字典；series返回的是序列的字典；records返回的是字典的列表
>df1 = df.to_dict(orient='dict')
>df2 = df.to_dict(orient='list')
>df3 = df.to_dict(orient='series')
>df4 = df.to_dict(orient='records')
># series to ndarray
>foo = ser.as_matrix()
>foo0 = Series.as_matrix(ser)
>foo
>array([[2000, 'Ohino', 1.5], [2001, 'Ohino', 1.7], [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4], [2002, 'Nevada', 2.9]], dtype=object)
>foo0
>array([[2000, 'Ohino', 1.5], [2001, 'Ohino', 1.7], [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4], [2002, 'Nevada', 2.9]], dtype=object)
>type(foo)
>numpy.ndarray
>foo.shape, foo0.shape
>((5,), (5,))
># dataframe to ndarray foo2 foo3 foo4 结果相同
>foo1 = DataFrame.as_matrix(df)
>foo2 = df.as_matrix()
>foo3 = df.values
>foo4 = np.array(df)
>
>foo5 = df.as_matrix(["pop"])
>foo1
>array([[2000, 'Ohino', 1.5],
>       [2001, 'Ohino', 1.7],
>       [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4],
>       [2002, 'Nevada', 2.9]], dtype=object)
>foo2
>array([[2000, 'Ohino', 1.5],
>       [2001, 'Ohino', 1.7],
>       [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4],
>       [2002, 'Nevada', 2.9]], dtype=object)
>foo3
>array([[2000, 'Ohino', 1.5],
>       [2001, 'Ohino', 1.7],
>       [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4],
>       [2002, 'Nevada', 2.9]], dtype=object)
>foo4
>array([[2000, 'Ohino', 1.5],
>       [2001, 'Ohino', 1.7],
>       [2002, 'Ohino', 3.6],
>       [2001, 'Nevada', 2.4],
>       [2002, 'Nevada', 2.9]], dtype=object)
>foo5
>array([[ 1.5],
>       [ 1.7],
>       [ 3.6],
>       [ 2.4],
>       [ 2.9]])
>foo1.shape, foo2.shape, foo3.shape, foo4.shape, foo5.shape
>((5, 3), (5, 3), (5, 3), (5, 3), (5, 1))
>df
>```
>
>|           | YEAR | STATE  | POP  |
>| :-------- | :--- | :----- | :--- |
>| **one**   | 2000 | Ohino  | 1.5  |
>| **two**   | 2001 | Ohino  | 1.7  |
>| **three** | 2002 | Ohino  | 3.6  |
>| **four**  | 2001 | Nevada | 2.4  |
>| **five**  | 2002 | Nevada | 2.9  |
>
>```
># dataframe to dict, dict返回的是dict of dict；list返回的是列表的字典；series返回的是序列的字典；records返回的是字典的列表
>df1 = df.to_dict(orient='dict')
>df1
>{'pop': {'five': 2.9,
>  'four': 2.4,
>  'one': 1.5,
>  'three': 3.6,
>  'two': 1.7},
> 'state': {'five': 'Nevada',
>  'four': 'Nevada',
>  'one': 'Ohino',
>  'three': 'Ohino',
>  'two': 'Ohino'},
> 'year': {'five': 2002, 'four': 2001, 'one': 2000, 'three': 2002, 'two': 2001}}
>type(df1)
>dict
># dataframe to dict, dict返回的是dict of dict；list返回的是列表的字典；series返回的是序列的字典；records返回的是字典的列表
>df2 = df.to_dict(orient='list')
>df2
>{'pop': [1.5, 1.7, 3.6, 2.4, 2.9],
> 'state': ['Ohino', 'Ohino', 'Ohino', 'Nevada', 'Nevada'],
> 'year': [2000, 2001, 2002, 2001, 2002]}
>type(df2)
>dict
># dataframe to dict, dict返回的是dict of dict；list返回的是列表的字典；series返回的是序列的字典；records返回的是字典的列表
>df3 = df.to_dict(orient='series')
>df3
>{'pop': one      1.5
> two      1.7
> three    3.6
> four     2.4
> five     2.9
> Name: pop, dtype: float64, 'state': one       Ohino
> two       Ohino
> three     Ohino
> four     Nevada
> five     Nevada
> Name: state, dtype: object, 'year': one      2000
> two      2001
> three    2002
> four     2001
> five     2002
> Name: year, dtype: int64}
>type(df3)
>dict
># dataframe to dict, dict返回的是dict of dict；list返回的是列表的字典；series返回的是序列的字典；records返回的是字典的列表
>df4 = df.to_dict(orient='records')
>df4
>[{'pop': 1.5, 'state': 'Ohino', 'year': 2000},
> {'pop': 1.7, 'state': 'Ohino', 'year': 2001},
> {'pop': 3.6, 'state': 'Ohino', 'year': 2002},
> {'pop': 2.4, 'state': 'Nevada', 'year': 2001},
> {'pop': 2.9, 'state': 'Nevada', 'year': 2002}]
>type(df4)
>list
>```
>
>## 4. 总结：List, Dict, Array, Series, DataFrame 的区别
>
>- List 和 Dict 是 Python 的基本数据结构
>- Series 和 DataFrame 是 Pandas 的基本数据结构
>- Array 是 Numpy 的数据结构
>
>### 4.1 Pandas 中的数据结构
>
>#### 4.1.1 Series
>
>- Series 是一个一维的类似的数组对象，包含一个数组的数据（任何 NumPy 的数据类型）和一个与数组关联的数据标签，被叫做**索引**。最简单的Series是由一个数组的数据构成。
>- Series 是一个定长的，有序的字典，因为它把索引和值映射起来了。
>- Series 与 Numpy 中的一维 array 类似。二者与 Python 基本的数据结构 List 也很相近。其区别是：List 中的元素可以是不同的数据类型，而 Array 和 Series 中则只允许存储相同的数据类型，这样可以更有效的使用内存，提高运算效率。
>- Time-Series：以时间为索引的 Series。
>
>#### 4.1.2 DataFrame
>
>- DataFrame：**二维**的表格型数据结构。很多功能与 R 中的 data.frame 类似。可以将 DataFrame 理解为 Series 的容器。
>- 有很多方法来构建一个 DataFrame，但最常用的一个是用一个相等长度列表的字典或 NumPy 数组。
>- 像 Series 一样， DataFrame 的 values 属性返回一个包含在 DataFrame 中的数据的二维 ndarray。
>- Panel ：三维的数组，可以理解为 DataFrame 的容器。



### 细节



#### 在windows’\’问题

```python
main(r”D:\doc\code\text”) #r表示\不转义
```

#### 两个list按其一排序，另一个改变顺序

一、需求

现有两个 list：A, B，其中A中保存日期，B中保存该日期下的产量，要求按照产量排序，并使日期列表随之排序。


二、解法

1. 将A，B两个 list 变成 list of tuple，使用 zip() 函数
```
zipped = zip(A,B)
# print(list(zipped))
# python2中可以直接 print(zipped), python3 中改为 print(list(zipped))
假设A = [1,2,3], B = [6,5,4]

此时 zipped = [(1,6), (2,5), (3,4)]
```


2. 排序
```
sort_zipped = sorted(zipped,key=lambda x:(x[1],x[0]))
# 先按 x[1] 进行排序，若 x[1] 相同，再按照 x[0] 排序
此时 sort_zipped = [(3,4), [2,5], [1,6]] 

上述代码完成按照产量排序。
```


3. 拆分
```
result = zip(*sort_zipped)
# 将 sort_zipped 拆分成两个元组，为 [(3,2,1),(4,5,6)]

x_axis, y_axis = [list(x) for x in result]
# 将拆分后的两个元组分别变成 list 列表，为 [3,2,1],[4,5,6]
以上，问题解决。

 

————————————————————————————————————————————————————————

补充个知识点。

zip 是一个生成器 的函数，它的结果是一个迭代器，该结果只能调用一次，举例如下图：



上图说明 b 调用一次之后，再调用的 c 则为空。如果想要复用这个结果，需要保存到 list 里，即：

temp = list(zipped)
# 接本文开头的例子，如果直接 print(list(zipped))，则之后 zipped为空，因为已经用完一次迭代器了
关于迭代器和生成器，分享个链接，简单易懂：
```
http://www.runoob.com/python3/python3-iterator-generator.html



##### [列表推导式](https://docs.python.org/zh-cn/3/tutorial/datastructures.html)



##### [在参数前加*](https://www.agiliq.com/blog/2012/06/understanding-args-and-kwargs/)

```python
positions = [(i, j) for i in range(1, 3) for j in range(1, 3)]

for pos in positions:
    print(*pos)
```





* 可变对象和不可变对象：
[] 可变
() 不可变

```python
def function(*numbers):  # *代表可变参数
    ***
    ***
    return a  # 把想输出的参数return出来
```

* 一定注意初始化时，a = b = c =1，会给他们分配相同的内存空间

##### 问题

为什么sublime执行、命令行执行、命令写成shell脚本执行却都不一样？见下图：

<img src="learn-python.assets/Screen Shot 2020-07-27 at 09.56.11.png" alt="Screen Shot 2020-07-27 at 09.56.11" style="zoom:33%;" />

如上图：我直接cd到文件夹python 运行不会报错，但是我把这两行指令写成了一个.sh脚本执行，结果报错了。why？？？

### python内置函数



[python 内置函数基本用法](https://www.runoob.com/python/python-built-in-functions.html)

#### open文件：

![python—open](learn-python.assets/python—open.png)



|    模式    |  r   |  r+  |  w   |  w+  |  a   |  a+  |
| :--------: | :--: | :--: | :--: | :--: | :--: | :--: |
|     读     |  +   |  +   |      |  +   |      |  +   |
|     写     |      |  +   |  +   |  +   |  +   |  +   |
|    创建    |      |      |  +   |  +   |  +   |  +   |
|    覆盖    |      |      |  +   |  +   |      |      |
| 指针在开始 |  +   |  +   |  +   |  +   |      |      |
| 指针在结尾 |      |      |      |      |  +   |  +   |

[python文本管理之字符串匹配搜索](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/p04_match_and_search_text.html)

#### List.insert()

这个函数用来在list中插入一个元素

### python class



class的用法的确有些奇怪，class本身的object并没有意义，而class的参数是`__init__`函数除了self那一部分定义的，比如我们后边的happy_bday的lyrics

```python
class Song(object):
    """docstring for Thing"""

    def __init__(self, lyrics): 
	# self本身就指的类的实例对象，self的名字只是习惯而后边的lyrics是传入的参数
        # super(Thing, self).__init__()
        self.lyrics = lyrics  # 这个很关键，如果没有'self.'，那么这个lyrics只是一个局部变量而已，没法应用到类中
        print("hello")           # 创建类的实例之后会执行__init__函数

    print(object)                # 定义类之后就运行

    def sing_me_a_song(self):
        for line in self.lyrics: # for循环中，量可以不用定义
            print(line)

print("Where to start?") 
#上边这个print是想测试下，class是否和def一样，不调用就不执行，但定义类的时候就执行了，不用等到创建类的实例

a = 498598459

happy_bday = Song(["happy birthday to you",
                   "I don't want to get sued",
                   "So I'll stop right there"])

happy_bday.sing_me_a_song()

print(happy_bday) # 调用了这个类，print出来是：<__main__.Song object at 0x10c973150>

# print(__init__(self, lyrics))# 此时会报错：name '__init__' is not defined

print(globals()) # globals是一个内置函数，作用是返回全局变量的字典
```

### Python包，模块，类，函数什么关系？库？

[python package里`__init__.py`作用](https://www.jianshu.com/p/73f7fbf75183)

A：简单的说就是层级关系：**一个package就是一个带有`__init__.py`文件的文件夹**，里边可能会有好几个模块；**一个模块就是一个.py文件**，里边可能会定义了好几个类和函数；一个类里边可能会定义了好几个函数。而**python中库这个概念不明确**，大体是指package，有时模块也成为库，就是可以导入的实现某些功能的代码集合。

[setup.py作用](https://blog.konghy.cn/2018/04/29/setup-dot-py/)





