# python库

[toc]

#### [导入自己的模块](https://www.google.com/search?client=safari&rls=en&q=ModuleNotFoundError:+No+module+named&ie=UTF-8&oe=UTF-8)

[官方文档](https://docs.python.org/3/reference/import.html)、


## 标准库

[每周一个python标准库](https://pymotw.com/3/index.html)

### OS模块

[python官方OS模块介绍](https://docs.python.org/3.8/library/os.html#module-os)、

|       方法       |             说明             |
| :--------------: | :--------------------------: |
|     os.mkdir     |           创建目录           |
|   os.makedirs    |  创建多级目录，且存在不报错  |
|     os.rmdir     |           删除目录           |
|    os.rename     |            重命名            |
|    os.remove     |           删除文件           |
|    os.getcwd     |       获取当前工作路径       |
|     os.walk      |           遍历目录           |
|   os.path.join   |       连接目录与文件名       |
|  os.path.split   |       分割文件名与目录       |
| os.path.abspath  |         获取绝对路径         |
| os.path.dirname  |           获取路径           |
| os.path.basename |     获取文件名或文件夹名     |
| os.path.splitext |      分离文件名与扩展名      |
|  os.path.isfile  | 判断给出的路径是否是一个文件 |
|  os.path.isdir   | 判断给出的路径是否是一个目录 |

### [`shutil`](https://docs.python.org/zh-cn/3/library/shutil.html#module-shutil)库

```python
shutil.copyfile( src, dst)     #从源src复制到dst中去。当然前提是目标地址是具备可写权限。抛出的异常信息为IOException. 如果当前的dst已存在的话就会被覆盖掉
shutil.move( src, dst)         #移动文件或重命名
shutil.copymode( src, dst)     #只是会复制其权限其他的东西是不会被复制的
shutil.copystat( src, dst)     #复制权限、最后访问时间、最后修改时间
shutil.copy( src, dst)         #复制一个文件到一个文件或一个目录
shutil.copy2( src, dst)        #在copy上的基础上再复制文件最后访问时间与修改时间也复制过来了，类似于cp –p的东西
shutil.copy2( src, dst)        #如果两个位置的文件系统是一样的话相当于是rename操作，只是改名；如果是不在相同的文件系统的话就是做move操作
shutil.copytree( olddir, newdir, True/Flase)#把olddir拷贝一份newdir，如果第3个参数是True，则复制目录时将保持文件夹下的符号连接，如果第3个参数是False，则将在复制的目录下生成物理副本来替代符号连接
shutil.rmtree( src )           #递归删除一个目录以及目录内的所有内容
```

### [`re`](https://docs.python.org/3/library/re.html#module-re)库（正则表达式）


Search:

```python
>>> import re
>>> m = re.search('(?<=abc)def', 'abcdef')
>>> m.group(0)
'def'
```

Split:(不同于内置的split，这个函数可以用多个字符串分割)

```python
>>> re.split(r'\W+', 'Words, words, words.')
['Words', 'words', 'words', '']
>>> re.split(r'(\W+)', 'Words, words, words.')
['Words', ', ', 'words', ', ', 'words', '.', '']
>>> re.split(r'\W+', 'Words, words, words.', 1)
['Words', 'words, words.']
>>> re.split('[a-f]+', '0a3B9', flags=re.IGNORECASE)
['0', '3', '9']
>>> re.split(r'\b', 'Words, words, words.')
['', 'Words', ', ', 'words', ', ', 'words', '.']
```

Sub:(可以用复杂字符串替换另一个复杂字符串)

```python
Text = open(InData, "r", encoding='utf-8', errors='ignore').read()
str1 = re.sub(r"\n.*atom types", r"\n\t3 atom types", Test)
```

* 注意一点：我们想匹配跨越多行的字符的集合`./`是不可以的，因为它不包括换行符，见下面代码：

Findall：适用于选出英文/数字元素

```python 
text1 = "1,  1,3，4 5"
text2 = "Graphene，  ’BP‘,   Pop--graphene,Graphyne_beta"
list1 = re.findall(r'[0-9]+', text1)
list2 = re.findall(r'[a-zA-Z0-9]+', text2) 
list3 = re.findall(r'[a-zA-Z0-9_-]+', text2) 
print(list1)
>>>['1', '1', '3', '4', '5']
print(list2)
>>>['Graphene', 'BP', 'Pop', 'graphene', 'Graphyne', 'beta']
print(list3)
>>>['Graphene', 'BP', 'Pop--graphene', 'Graphyne_beta']
```

还有Match，Compile，Findall，Group等好多函数很好用，具体看官方文档。



### [`multiprocessing`](https://docs.python.org/zh-cn/3/library/multiprocessing.html#module-multiprocessing) 库

函数的参数是数组，然后循环执行：

```python
from multiprocessing import Pool

def work(x):
    return x+1
pool = Pool(processes=4) # 4个线程
x = [1,2,3,4,5,6]
results = pool.map(work, x)
print results
```

函数有一个参数是普通“固定”参数，另一个参数是数组：

```python
from multiprocessing import Pool
from functools import partial

def work(x, y):
    return x + y
pool = Pool(processes=4)
partial_work = partial(work, y=1) # 提取x作为partial函数的输入变量
results = pool.map(partial_work, x)
```

函数有两个数组，同时进行循环：

```python
from multiprocessing import Pool
from functools import partial

def work(x, y):
    return x + y

pool = Pool(processes=4)
x = [1,2,3,4,5,6]
y = [1,1,1,1,1,1]
x_y = zip(x, y)
results = pool.map(work, x_y)


##或者使用pathos包下的multiprocessing，这个包是使用dill的multiprocessing的一个fork，允许多参数输入：
from pathos.multiprocessing import ProcessingPoll as Pool

pool = Pool(4)
results = pool.map(work, x, y)
```

### [venv](https://docs.python.org/zh-cn/3/tutorial/venv.html)库

```bash
python3 -m venv /Users/zxll/my_python
source /Users/zxll/my_python/bin/activate
```



| 平台    | Shell           | 用于激活虚拟环境的命令              |
| :------ | :-------------- | :---------------------------------- |
| POSIX   | bash/zsh        | $ source <venv>/bin/activate        |
|         | fish            | $ . <venv>/bin/activate.fish        |
|         | csh/tcsh        | $ source <venv>/bin/activate.csh    |
|         | PowerShell Core | $ <venv>/bin/Activate.ps1           |
| Windows | cmd.exe         | C:\> <venv>\Scripts\activate.bat    |
|         | PowerShell      | PS C:\> <venv>\Scripts\Activate.ps1 |

### [argparse 库](https://docs.python.org/zh-cn/3/library/argparse.html)

PS：以前叫optparse

#### [Argparse 教程](https://docs.python.org/zh-cn/3/howto/argparse.html#argparse-tutorial)



## 第三方库

### line_profiler（性能分析）

这个库结合cProfile，timeit，等标准库，是**python性能分析**主要模块。

这个`line_profiler`的特点就是可以逐行分析运行时间，这样可以很快捷直观的分析出性能瓶颈的指令。

1. 定义

```python
@profile   #只需要把函数定义前加上这一行
def main():
....
```

2. 运行

```bash
kernprof -l -v PlotHS_debug.py #运行的时候用这个指令
```

3. 查看

```bash
Line #      Hits         Time  Per Hit   % Time  Line Contents
==============================================================
    46                                           
    47                                           
    48         6         18.0      3.0      0.0  
    49         6         10.0      1.7      0.0  
    50         6          6.0      1.0      0.0  
    51         6          5.0      0.8      0.0  
    52                                           
    53         6        681.0    113.5      0.0  
    54         6         38.0      6.3      0.0  
    55         6       5058.0    843.0      0.0  
    56                                           
    57         6       3825.0    637.5      0.0  
    58                                           
    59        78        226.0      2.9      0.0  
    60        72        277.0      3.8      0.0  
    61        72        144.0      2.0      0.0  
    62        72    2367302.0  32879.2      9.3  
    63                                           
    64        72    1503330.0  20879.6      5.9  
    65        72     102355.0   1421.6      0.4  
    66                                           
    67        72      56645.0    786.7      0.2  
    68        72   11885994.0 165083.2     46.9  
    69                                           
    70        72      10623.0    147.5      0.0  
    71        72     100870.0   1401.0      0.4  
    72        72      25061.0    348.1      0.1  
    73                                           
    74         6       8213.0   1368.8      0.0  
    75         6         66.0     11.0      0.0  
    76                                           
    77         6      22937.0   3822.8      0.1  
    78         6     865094.0 144182.3      3.4  
    79                                           
    80         6    4085220.0 680870.0     16.1  
    81         6         12.0      2.0      0.0  
    82         6    4301986.0 716997.7     17.0  
    Total time: 17.3605 s
```

4. 结论

python的主要性能瓶颈都在IO上，所以尽量避免读写，尤其是写入文件才是最重要的。



### About Internet

这部分有些是标准库，有些是第三方库，都是关于网络前后端的。

#### socket（标准库）



#### urllib（标准库）



#### request



#### BeautifulSoup



### [flask](https://flask.palletsprojects.com/en/1.1.x/quickstart/)库

### 使用

[中文文档](http://docs.jinkan.org/docs/flask/quickstart.html#a-minimal-application)、

```python
from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

if __name__ == '__main__':
    app.run()
```

```bash
export FLASK_APP = hello.py
export FLASK_ENV = development
flask run
```

### 开发

[源码解析](https://cizixs.com/2017/01/13/flask-insight-context/)、





### virtualenv（已被自带的venv代替）

虚拟python环境


### pandas

- [ ] **[pandas中文](https://www.pypandas.cn/docs/user_guide/)**

- [ ] **[pandas-eng](https://pandas.pydata.org/docs/getting_started/index.html#getting-started)**

#### read data

```python
import numpy as np

import pandas as pd #定义pd

Name=pd.read_csv('code/data.txt',sep='\t',header=None,index_col=False,names=['id','no','type','none','x','y','z']) #读取code文件夹里名为data.txt的文件，分隔符用｜，不把第一行作为列名，不把第一列作为索引（# index_col是在读取时设置哪一列为索引的，默认为None，和false差不多），列名自定义为‘id’....

Name = pd.read_csv('doc/a2_gra.txt', sep = "\s+", index_col = 0, header = None)

Name=pd.read_csv('filename.csv',index_col=None) 

Name=pd.read_table('filename.txt',index_col=None) #read_table默认sep为\t

Name.set_index('id',inplace=True) #把 id这一列设为索引，在此数据上更改（inplace=True非常常用）

Name.columns['id','type'] # 设置
```

关于对索引的操作：https://blog.csdn.net/claroja/article/details/72930594

```python
Name.index=Name.index+1 #和下面语句同样为把index设为从1开始

Name.index = np.arange(1,len(Name)+1) 

DataFrame.index.names = ['newName']#设置新的index列名
```

#### display settings

```python
pd.get_option('display.max_row') #show the setting

pd.set_option('display.max_row',99) #set the setting

pd.set_option('display.max_row',None)

pd.reset_option('display.max_row') #reset to dafalt

pd.set_option('display.max_colwidth',100) #

pd.set_option('display.precision',3) #
```


#### select rows and columns

```python
#loc是列名和index名（注意index不一定要是从头开始的），iloc都是第几行之类的

>>> df = pd.DataFrame([[1, 2], [4, 5], [7, 8]],
...      index=['cobra', 'viper', 'sidewinder'],
...      columns=['max_speed', 'shield'])
>>> df
            max_speed  shield
cobra               1       2
viper               4       5
sidewinder          7       8

>>> df.loc['viper']
max_speed    4
shield       5
Name: viper, dtype: int64


>>> df.loc[['viper', 'sidewinder']]
            max_speed  shield
viper               4       5
sidewinder          7       8


>>> df.loc[df['shield'] > 6, ['max_speed']]
            max_speed
sidewinder          7


>>> df.loc[:, 'max_speed'] = 30
>>> df
            max_speed  shield
cobra              30      10
viper              30      50
sidewinder         30      50

##下面是iloc 
s = int(len(df) * 0.05)
e = int(len(df) * 0.95)
df.iloc[s:e] #选择中间90%的行
df.iloc[:, [0, 2, 5]] #选择第1，3，6列
```


#### remove the rows or the columns by multiple conditions

```python
Name.drop(['no','none'],axis=1,inplace=True) #delete the columns

Name.drop(['1','2'],inplace=True) # delete the rows "inplace = True"是用在

#删除前5%的行数
df.drop(df.index[:int(len(df) * 0.05)], inplace = True)
#同样删除前5%的行数
s = int(len(df) * 0.05)
e = len(df)-1
df = df.iloc[s:e]
```

```python
delete = Name[ (Name['x'] >= 30) & (Name['type'] == 2) ].index
Name.drop( delete , inplace=True)
```

```python
Name.dropna(axis=1, how='any') # delete all the rows that include NaN

Name.dropna(axis=1, how='all') # 删除全是NaN的列
```


#### change colume type

```python
data[["two"]] = data[["two"]].astype(int)
```

#### get info

```python
len(Name) #行数
Name.shape[0] #行数

Name.shape[1] #列数
```

#### 画图

```python 
Name.plot(subplots=True) #调用matplotlib画图，subplots=True是把几组数据分别做成子图


#分别做几个子图
fig, ax = plt.subplots(2, 2, figsize=(14,7))
df.plot(x="index", y="y1", ax=ax[0, 0])
df.plot.scatter(x="index", y="y2", ax=ax[0, 1])
df.plot.scatter(x="index", y="a3", ax=ax[1, 0])
df.plot(x="index", y="a1", ax=ax[1, 1])
plt.show()

#下面代码的图附在下边
fig, ax = plt.subplots(1, 2, figsize=(14,5))
atom1.plot(ax=ax[0]).legend(loc="upper right")
atom2.plot(ax=ax[1]).legend(loc="upper right")
plt.show()
```

![Figure_1](learn-python-lib.assets\Figure_1.png)

#### sort

```python
Name.sort_index() #sort by index

Name.sort_values(by=['id','z'],ascending=False,inplace=True) #sort by colume value 


```


#### replace 

```python
Name['z'].replace([0,0.22],[5,4.33],inplace=True) #replace '0' and '0.22' with '5' and '4.33' in the 'z' column

Name.rename({'a': 'X', 'b': 'Y'}, axis=1, inplace=True) #also works for rename the columns
```

#### Append

```python
Name.append([{'x' : 5.0 }], ignore_index = True)
```



#### save data

```python
Name.to_csv('code/mdata.txt','sep='/t','index_col=False')

```

### matpoltlib

- [ ] **[matplotlib中文]()**

- [ ] **[matplotlib-eng](https://matplotlib.org/users/index.html)**

matplotlib 支持 latex

<img src="learn-python-lib.assets\7931281-45a5018dcaad5bfb.png" alt="img" style="zoom:50%;" />

#### 图结构

<img src="learn-python-lib.assets\Screen Shot 2020-07-09 at 10.06.17.png" alt="Screen Shot 2020-07-09 at 10.06.17" style="zoom: 33%;" />

#### 创建Axes：

```python
fig = plt.figure() # an empty figure with no Axes
fig, ax = plt.subplots() # a figure with a single Axes
fig, axs = plt.subplots(2, 2) # a figure with a 2x2 grid of Axes

#创建一个小图
left, bottom, width, height = 0.2, 0.6, 0.25, 0.25 #都是比例，整图可以看作1*1的
ax2 = fig.add_axes([left, bottom, width, height])
ax2.plot(y, x, 'b')
ax2.set_xlabel('x')
ax2.set_ylabel('y')
ax2.set_title('title inside 1')
```

#### 绘制不同的“图”：

```python
names = ['group_a', 'group_b', 'group_c'] values = [1, 10, 100]
plt.figure(figsize=(9, 3))
plt.subplot(131)
plt.bar(names, values) plt.subplot(132)
plt.scatter(names, values) plt.subplot(133)
plt.plot(names, values) plt.suptitle('Categorical Plotting') plt.show()
```

上述代码结果如图：
![Figure_1](learn-python-lib.assets\Figure_1-7238023.png)

#### 颜色

```python 
import matplotlib.pyplot as plt
import numpy as np

t = np.linspace(0.0, 2.0, 201)
s = np.sin(2 * np.pi * t)

# 1) RGB tuple:
fig, ax = plt.subplots(facecolor=(.18, .31, .31))
# 2) hex string:
ax.set_facecolor('#eafff5')
# 3) gray level string:
ax.set_title('Voltage vs. time chart', color='0.7')
# 4) single letter color string
ax.set_xlabel('time (s)', color='c')
# 5) a named color:
ax.set_ylabel('voltage (mV)', color='peachpuff')
# 6) a named xkcd color:
ax.plot(t, s, 'xkcd:crimson')
# 7) Cn notation:
ax.plot(t, .7*s, color='C4', linestyle='--')
# 8) tab notation:
ax.tick_params(labelcolor='tab:orange')

plt.show()
```

![Voltage vs. time chart](learn-python-lib.assets\sphx_glr_color_demo_001.png)

#### Marker

<img src="learn-python-lib.assets\Screen Shot 2020-07-22 at 10.02.02.png" alt="Screen Shot 2020-07-22 at 10.02.02" style="zoom:40%;" />

| marker  | description    |
| ------- | -------------- |
| "."     | point          |
| ","     | pixel          |
| "o"     | circle         |
| "v"     | triangle_down  |
| "^"     | triangle_up    |
| "<"     | triangle_left  |
| ">"     | triangle_right |
| "1"     | tri_down       |
| "2"     | tri_up         |
| "3"     | tri_left       |
| "4"     | tri_right      |
| "8"     | octagon        |
| "s"     | square         |
| "p"     | pentagon       |
| "P"     | plus (filled)  |
| " * " å | star           |
| "h"     | hexagon1       |
| "H"     | hexagon2       |
| "+"     | plus           |
| "x"     | x              |
| "X"     | x (filled)     |
| "D"     | diamond        |
| "d"     | thin_diamond   |
| "\|"    | vline          |
| "_"     | hline          |






### Pillow

- [ ] **[pillow中文]()**

- [ ] **[pillow-eng](https://pillow.readthedocs.io/en/latest/index.html)**

### ipython

- [ ] **[ipython-eng](http://ipython.org/ipython-doc/stable/index.html)**

```python
ipython --pylab #直接导入matplotlib，实时交互作图
```


### Modin

这个库是专门用来对pandas进行多核并行优化的：[Modin Github](https://github.com/modin-project/modin)、[横向对比并行就算的效率](https://www.datarevenue.com/en-blog/pandas-vs-dask-vs-vaex-vs-modin-vs-rapids-vs-ray)。




### scipy

- [ ] **[scipy中文](https://reverland.org/2012-08-24-scipy.html)**

- [ ] **[scipy-eng](https://docs.scipy.org/doc/scipy/reference/tutorial/index.html)**

#### FFT(快速傅立叶变换)

```python
import numpy as np
import matplotlib.pyplot as plt 

sampling_rate = 8000 # 采样频率8000Hz
fft_size = 512  # 采样点512，就是说以8000Hz的速度采512个点，我们获得的数据只有这512个点的对应时刻和此时的信号值。
t = np.linspace(0, 1, sampling_rate)  # 截取一段时间，截取是任意的，这里取了0~1秒的一段时间。

x = np.sin(2*np.pi*156.25*t) + 2*np.sin(2*np.pi*234.375*t) # 输入信号序列，人工生成了一段信号序列，范围在0~1秒
xs = x[:fft_size]  # 由上所述，我们只采样了512个点，所以我们只获得了前512个点的数据
xf = np.fft.rfft(xs)/fft_size # 调用np.fft的函数rfft(用于实值信号fft)，产生长度为fft_size/2+1的一个复数向量，分别表示从0Hz~4000Hz的部分，这里之所以是4000Hz是因为Nyquist定理，采样频率8000Hz，则能恢复带宽为4000Hz的信号。最后/fft_size是为了正确显示波形能量

freqs = np.linspace(0, sampling_rate//2, fft_size//2 + 1) # 由上可知，我们得到了数据，现在产生0~4000Hz的频率向量，方便作图
xfp = 20*np.log10(np.clip(np.abs(xf), 1e-20, 1e1000)) # 防止幅值为0，先利用clip剪裁幅度，再化成分贝

pl.figure(figsize=(8, 4)) # 生成画布
pl.subplot(211) # 生成子图，211的意思是将画布分成两行一列，自己居上面。
pl.plot(t[:fft_size], xs) # 对真实波形绘图
pl.xlabel(u"time(s)")
pl.title(u"The Wave and Spectrum of 156.25Hz and 234.375Hz")
pl.subplot(212) # 同理
pl.plot(freqs, xfp) # 对频率和幅值作图，xlabel是频率Hz,ylabel是dB
pl.xlabel(u"Hz")
pl.subplots_adjust(hspace=0.4) # 调节绘图参数
pl.show()
```

上述代码运行结果如下图：

<img src="learn-python-lib.assets\20191021162649119.jpg" alt="img" style="zoom:50%;" />



### Pytorch

#### 图像处理


[opencv学习](http://codec.wang/#/opencv/)、

##### 图像的卷积

为什么图像做卷积运算物理意义和卷积本身有分歧，因为用卷积核矩阵对其运算本身是对一个二元函数做拉普拉斯算子的结果，也就是其表征了二元函数的导数/二阶导数！这当然也就表征了像素值的变化强度，而这自然就是图像边缘了。

![图像卷积](learn-python-lib.assets\图像卷积.gif)



![卷积本身就是二维函数导数的离散表达](learn-python-lib.assets\卷积本身就是二维函数导数的离散表达.jpg)

<img src="learn-python-lib.assets\equation.svg" alt="equation" style="zoom:150%;" />

但这只是卷积的一种一方面应用，我们可以调整卷积核，这样就可以是其有不同的意义，实现不同的功能，比如可以反其道行之：让图像变模糊，也就是去边缘化（大致就是卷积核数值分布反过来，就可以弱化“梯度”大的位置的像素值，从而达到模糊的效果）





### Manim

这是3blue1brown做的一个python第三方库，可以用它来制作关于数学等基础科学的视频。

* [github](https://github.com/3b1b/manim)
* [知乎教程](https://zhuanlan.zhihu.com/p/104701396)
* [知乎回答-win安装](https://www.zhihu.com/question/57357012)
* [英文教程](https://talkingphysics.wordpress.com/2019/01/08/getting-started-animating-with-manim-and-python-3-7/)

