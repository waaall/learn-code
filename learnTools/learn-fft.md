
# DFT基本概念
频率


# [快速傅里叶变换（FFT）算法详解](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html)

- [计算离散傅里叶变换](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_label0)
- [离散傅里叶变换中的对称性](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_label1)
- [向量化的NumPy](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_label2)


快速傅里叶变换（Fast Fourier Transform）是信号处理与数据分析领域里最重要的算法之一。我打开一本老旧的算法书，欣赏了JW Cooley 和 John Tukey 在1965年的文章中，以看似简单的计算技巧来讲解这个东西。

本文的目标是，深入Cooley-Tukey FFT 算法，解释作为其根源的“对称性”，并以一些直观的python代码将其理论转变为实际。我希望这次研究能对这个算法的背景原理有更全面的认识。

FFT（快速傅里叶变换）本身就是离散傅里叶变换（Discrete Fourier Transform）的快速算法，使算法复杂度由原本的O(N^2) 变为 O(NlogN)，离散傅里叶变换DFT，如同更为人熟悉的连续傅里叶变换，有如下的正、逆定义形式：

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205418860-2014717920.png)

xn 到 Xk 的转化就是空域到频域的转换，这个转换有助于研究信号的功率谱，和使某些问题的计算更有效率。事实上，你还可以查看一下我们即将推出的天文学和统计学的图书的第十章（[这里有一些图示和python代码](http://www.astroml.org/book_figures/chapter10/)）。作为一个例子，你可以查看下我的文章《[用python求解薛定谔方程](http://jakevdp.github.io/blog/2012/09/05/quantum-python/)》，是如何利用FFT将原本复杂的微分方程简化。

正因为FFT在那么多领域里如此有用，python提供了很多标准工具和封装来计算它。NumPy 和 SciPy 都有经过充分测试的封装好的FFT库，分别位于子模块 numpy.fft 和 scipy.fftpack 。我所知的最快的FFT是在 [FFTW](http://www.fftw.org/)包中 ，而你也可以在python的[pyFFTW](https://pypi.python.org/pypi/pyFFTW) 包中使用它。

虽然说了这么远，但还是暂时先将这些库放一边，考虑一下怎样使用原始的python从头开始计算FFT。

[Return Top](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_labelTop)

## 计算离散傅里叶变换

简单起见，我们只关心正变换，因为逆变换也只是以很相似的方式就能做到。看一下上面的DFT表达式，它只是一个直观的线性运算：向量x的矩阵乘法，

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205436075-1453920281.png)

矩阵M可以表示为

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205448343-2014301532.png)

这么想的话，我们可以简单地利用矩阵乘法计算DFT：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 import numpy as np
2 def DFT_slow(x):
3     """Compute the discrete Fourier Transform of the 1D array x"""
4     x = np.asarray(x, dtype=float)
5     N = x.shape[0]
6     n = np.arange(N)
7     k = n.reshape((N, 1))
8     M = np.exp(-2j * np.pi * k * n / N)
9     return np.dot(M, x)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

对比numpy的内置FFT函数，我们来对结果进行仔细检查

```
x = np.random.random(1024)
np.allclose(DFT_slow(x), np.fft.fft(x))
```

 输出：

```
True
```

 现在为了验证我们的算法有多慢，对比下两者的执行时间

```
%timeit DFT_slow(x)
 
%timeit np.fft.fft(x)
```

 输出：

```
10 loops, best of 3: 75.4 ms per loop
10000 loops, best of 3: 25.5 µs per loop
```

使用这种简化的实现方法，正如所料，我们慢了一千多倍。但问题不是这么简单。对于长度为N的输入矢量，FFT是O(N logN)级的，而我们的慢算法是O(N^2)级的。这就意味着，FFT用50毫秒能干完的活，对于我们的慢算法来说，要差不多20小时！ 那么FFT是怎么提速完事的呢？答案就在于他利用了对称性。

[Return Top](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_labelTop)

## 离散傅里叶变换中的对称性

算法设计者所掌握的最重要手段之一，就是利用问题的对称性。如果你能清晰地展示问题的某一部分与另一部分相关，那么你就只需计算子结果一次，从而节省了计算成本。

Cooley 和 Tukey 正是使用这种方法导出FFT。 首先我们来看下![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205638388-746575156.png)的值。根据上面的表达式，推出：

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205512841-416103811.png)

对于所有的整数n，exp[2π i n]=1。

最后一行展示了DFT很好的对称性：

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205527061-665402071.png)

简单地拓展一下：

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205536228-1967248611.png)

同理对于所有整数 i 。正如下面即将看到的，这个对称性能被利用于更快地计算DFT。

DFT 到 FFT：

利用对称性 Cooley 和 Tukey 证明了，DFT的计算可以分为两部分。从DFT的定义得：

![img](https://images2018.cnblogs.com/blog/1100338/201804/1100338-20180425205548630-882164773.png)

我们将单个DFT分成了看起来相似两个更小的DFT。一个奇，一个偶。目前为止，我们还没有节省计算开销，每一部分都包含(N/2)∗N的计算量，总的来说，就是N^2 。

技巧就是对每一部分利用对称性。因为 k 的范围是 0≤k<N ， 而 n 的范围是 0≤n<M≡N/2 ， 从上面的对称性特点来看，我们只需对每个子问题作一半的计算量。O(N^2) 变成了 O(M^2) 。

但我们不是到这步就停下来，只要我们的小傅里叶变换是偶倍数，就可以再作分治，直到分解出来的子问题小到无法通过分治提高效率，接近极限时，这个递归是 O(n logn) 级的。

这个递归算法能在python里快速实现，当子问题被分解到合适大小时，再用回原本那种“慢方法”。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 def FFT(x):
 2     """A recursive implementation of the 1D Cooley-Tukey FFT"""
 3     x = np.asarray(x, dtype=float)
 4     N = x.shape[0]
 5  
 6     if N % 2 > 0:
 7         raise ValueError("size of x must be a power of 2")
 8     elif N <= 32:  # this cutoff should be optimized
 9         return DFT_slow(x)
10     else:
11         X_even = FFT(x[::2])
12         X_odd = FFT(x[1::2])
13         factor = np.exp(-2j * np.pi * np.arange(N) / N)
14         return np.concatenate([X_even + factor[:N / 2] * X_odd,
15                                X_even + factor[N / 2:] * X_odd])
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

现在我们做个快速的检查，看结果是否正确：

```
x = np.random.random(1024)
np.allclose(FFT(x), np.fft.fft(x))
```

 输出：

```
True
```

 然后与“慢方法”的运行时间对比下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
%timeit DFT_slow(x)
 
%timeit FFT(x)
 
%timeit np.fft.fft(x)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

输出：

```
10 loops, best of 3: 77.6 ms per loop
100 loops, best of 3: 4.07 ms per loop
10000 loops, best of 3: 24.7 µs per loop
```

现在的算法比之前的快了一个数量级。而且，我们的递归算法渐近于 O(n logn) 。我们实现了FFT 。

需要注意的是，我们还没做到numpy的内置FFT算法，这是意料之中的。numpy 的 fft 背后的FFTPACK算法 是以 Fortran 实现的，经过了多年的调优。此外，我们的NumPy的解决方案，同时涉及的Python堆栈递归和许多临时数组的分配，这显著地增加了计算时间。

还想加快速度的话，一个好的方法是使用Python/ NumPy的工作时，尽可能将重复计算向量化。我们是可以做到的，在计算过程中消除递归，使我们的python FFT更有效率。

[Return Top](https://www.cnblogs.com/ECJTUACM-873284962/p/6919424.html#_labelTop)

## 向量化的NumPy

注意上面的递归FFT实现，在最底层的递归，我们做了N/32次的矩阵向量乘积。我们的算法会得益于将这些矩阵向量乘积化为一次性计算的矩阵-矩阵乘积。在每一层的递归，重复的计算也可以被向量化。因为NumPy很擅长这类操作，我们可以利用这一点来实现向量化的FFT

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 def FFT_vectorized(x):
 2     """A vectorized, non-recursive version of the Cooley-Tukey FFT"""
 3     x = np.asarray(x, dtype=float)
 4     N = x.shape[0]
 5  
 6     if np.log2(N) % 1 > 0:
 7         raise ValueError("size of x must be a power of 2")
 8  
 9     # N_min here is equivalent to the stopping condition above,
10     # and should be a power of 2
11     N_min = min(N, 32)
12  
13     # Perform an O[N^2] DFT on all length-N_min sub-problems at once
14     n = np.arange(N_min)
15     k = n[:, None]
16     M = np.exp(-2j * np.pi * n * k / N_min)
17     X = np.dot(M, x.reshape((N_min, -1)))
18  
19     # build-up each level of the recursive calculation all at once
20     while X.shape[0] < N:
21         X_even = X[:, :X.shape[1] / 2]
22         X_odd = X[:, X.shape[1] / 2:]
23         factor = np.exp(-1j * np.pi * np.arange(X.shape[0])
24                         / X.shape[0])[:, None]
25         X = np.vstack([X_even + factor * X_odd,
26                        X_even - factor * X_odd])
27  
28     return X.ravel()
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

```
x = np.random.random(1024)
np.allclose(FFT_vectorized(x), np.fft.fft(x))
```

 输出：

```
True
```

因为我们的算法效率更大幅地提升了，所以来做个更大的测试（不包括DFT_slow）

 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
x = np.random.random(1024 * 16)
%timeit FFT(x)
 
%timeit FFT_vectorized(x)
 
%timeit np.fft.fft(x)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 输出：

```
10 loops, best of 3: 72.8 ms per loop
100 loops, best of 3: 4.11 ms per loop
1000 loops, best of 3: 505 µs per loop
```

我们的实现又提升了一个级别。这里我们是以 FFTPACK中大约10以内的因数基准，用了仅仅几十行 Python + NumPy代码。虽然没有相应的计算来证明， Python版本是远优于 FFTPACK源，这个你可以从[这里](http://www.netlib.org/fftpack/fft.c)浏览到。

那么 FFTPACK是怎么获得这个最后一点的加速的呢？也许它只是一个详细的记录簿， FFTPACK花了大量时间来保证任何的子计算能够被复用。我们这里的numpy版本涉及到额外的内存的分配和复制，对于如Fortran的一些低级语言就能够很容易的控制和最小化内存的使用。并且Cooley-Tukey算法还能够使其分成超过两部分（正如我们这里用到的Cooley-Tukey FFT基2算法），而且，其它更为先进的FFT算法或许也可以能够得到应用，包括基于卷积的从根本上不同的方法（例如Bluestein的算法和Rader的算法）。结合以上的思路延伸和方法，就可使阵列大小即使不满足2的幂，FFT也能快速执行。


## 速度自相关函数

自相关函数是为了检测某个随机量变化的周期性特征的。分子的运动是随机的，但是快慢有时间特征。大分子慢些，小分子快些。怎么检测呢？举个栗子，假设某个分子大约每5ps自转一周。你可以想象这个分子实际自转有时快有时慢，有时3ps，有时6ps。不过，5ps左右的居多。也就是说，如果你看这个分子的转动速度数据，那么对于速度时间序列上的点，任意一对相隔5ps的数据间必然有更相似的变化规律，或者说，比其它时间间隔下的数据对有更高的相关性。相关性用数学语言表达，叫所有此间隔下两数据对于平均值的偏差的乘积的统计平均值与标准偏差的商. 请盯着此wiki网页上第二个公式，尝试弄清每个字母的含义。
http://en.wikipedia.org/wiki/Autocorrelation

如果数据对之间呈高度相关，其值就越接近于1。如果相关性很低，就接近于0。这就是为什么分子模拟的速度自相关函数会衰减到0， 因为分子在做随机运动，因此，超过其扩散运动的特征相关时间的数据对之间的相似性就大大降低了，或者说无关了。因此，从速度的自相关函数图上可以看出来其大致扩散运动的特征时间，即接近于0值的时间间隔。

不同的特征相关时间反映的是决定某种运动事件背后的相互作用的大小。比如，分子在某表面扩散的特征时间，必然是由分子与表面，分子与分子之间作用有关的。因此，实验和模拟都要用这个特征时间来表征此体系的这个性质。

还有几个值得注意的问题：如果你的速度数据是以时间为横坐标的，得到的自相关函数就叫速度的时间自相关函数。通常不说时间两个字。此外，单个分子的速度其自身随时间的相关性，这叫自相关。当然，也可以是不同分子之间，即两列速度数据之间的互相关。


## GPT解释

信号 x = [1, 2, 3, 4]，点数 N=4。我们手工推一遍 DFT 过程。

1. DFT 定义
X[k] = \sum_{n=0}^{N-1} x[n] \cdot e^{-j 2\pi kn / N}, \quad k=0,1,2,3


2. 逐个计算

(1) k=0

X[0] = \sum_{n=0}^3 x[n] \cdot e^{-j 2\pi \cdot 0 \cdot n / 4} = 1+2+3+4 = 10
    •    复数形式：10 + j0
    •    幅度：|X[0]| = 10
    •    相位：\theta = 0

(2) k=1
X[1] = \sum_{n=0}^3 x[n] \cdot e^{-j 2\pi (1)n / 4}

展开：
    •    n=0: 1 \cdot 1 = 1
    •    n=1: 2 \cdot e^{-j\pi/2} = 2 \cdot (0 - j1) = -2j
    •    n=2: 3 \cdot e^{-j\pi} = 3 \cdot (-1) = -3
    •    n=3: 4 \cdot e^{-j3\pi/2} = 4 \cdot (0 + j1) = +4j

相加：
X[1] = (1 - 3) + (-2j + 4j) = -2 + 2j
    •    幅度：|X[1]| = \sqrt{(-2)^2 + (2)^2} = \sqrt{8} \approx 2.828
    •    相位：\theta = \text{atan2}(2, -2) = 135^\circ = 3\pi/4
  

(3) k=2
X[2] = \sum_{n=0}^3 x[n] \cdot e^{-j 2\pi (2)n / 4}

展开：
    •    n=0: 1 \cdot 1 = 1
    •    n=1: 2 \cdot e^{-j\pi} = -2
    •    n=2: 3 \cdot e^{-j2\pi} = 3 \cdot 1 = 3
    •    n=3: 4 \cdot e^{-j3\pi} = 4 \cdot (-1) = -4

相加：
X[2] = 1 - 2 + 3 - 4 = -2
    •    幅度：|X[2]| = 2
    •    相位：\theta = \pi（因为是 -2 + 0j）

(4) k=3
X[3] = \sum_{n=0}^3 x[n] \cdot e^{-j 2\pi (3)n / 4}
展开：
    •    n=0: 1 \cdot 1 = 1
    •    n=1: 2 \cdot e^{-j3\pi/2} = 2 \cdot (0 + j1) = 2j
    •    n=2: 3 \cdot e^{-j3\pi} = 3 \cdot (-1) = -3
    •    n=3: 4 \cdot e^{-j9\pi/2} = 4 \cdot (0 - j1) = -4j

相加：
X[3] = (1 - 3) + (2j - 4j) = -2 - 2j
    •    幅度：|X[3]| = \sqrt{(-2)^2 + (-2)^2} = \sqrt{8} \approx 2.828
    •    相位：\theta = \text{atan2}(-2, -2) = -135^\circ = -3\pi/4


3. 结果总结
X = [10, \; -2+2j, \; -2, \; -2-2j]

k    X[k]    幅度    相位
0    10+0j    10    0
1    -2+2j    2.828    +135°
2    -2+0j    2    180°
3    -2-2j    2.828    -135°

```python
import numpy as np

# 信号
x = [1, 2, 3, 4]
N = len(x)


# 按定义公式计算 DFT
X = []

for k in range(N):
    s = 0
    for n in range(N):
        s += x[n] * np.exp(-2j * np.pi * k * n / N)  # 逐项累加
    X.append(s)

X = np.array(X)

# 计算幅度和相位

magnitude = np.abs(X)            # 幅度 = sqrt(Re^2 + Im^2)

phase_rad = np.angle(X)          # 相位 = atan2(Im, Re)，单位弧度

phase_deg = np.degrees(phase_rad) # 转为角度


# 打印结果
for k in range(N):
    print(f"k={k}: X[k]={X[k]}, |X[k]|={magnitude[k]:.3f}, "
          f"phase={phase_rad[k]:.3f} rad ({phase_deg[k]:.1f} deg)")
```

# 窗函数

## [窗函数wikipedia](https://zh.wikipedia.org/zh-sg/窗函数)
**窗函数**（英语：window function）在[信号处理](https://zh.wikipedia.org/wiki/%E4%BF%A1%E5%8F%B7%E5%A4%84%E7%90%86 "信号处理")中是指一种除在给定[区间](https://zh.wikipedia.org/wiki/%E5%8C%BA%E9%97%B4 "区间")之外取值均为0的实[函数](https://zh.wikipedia.org/wiki/%E5%87%BD%E6%95%B0 "函数")。譬如：在给定区间内为[常数](https://zh.wikipedia.org/wiki/%E5%B8%B8%E6%95%B0 "常数")而在区间外为0的窗函数被形象地称为**矩形窗**。
任何函数与窗函数之积仍为窗函数，所以相乘的结果就像透过窗口“看”其他函数一样。窗函数在[频谱](https://zh.wikipedia.org/wiki/%E9%A0%BB%E8%AD%9C "频谱")分析、[滤波器设计](https://zh.wikipedia.org/wiki/%E6%BB%A4%E6%B3%A2%E5%99%A8%E8%AE%BE%E8%AE%A1 "滤波器设计")、波束形成、以及音频数据压缩（如在[Ogg Vorbis](https://zh.wikipedia.org/wiki/Ogg_Vorbis "Ogg Vorbis")音频格式中）等方面有广泛的应用。

### 频谱分析


cos⁡(ωt)![{\displaystyle \cos(\omega t)\,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/179776ee8e70715203f4d2850f64a5667456333e) 的[傅立叶变换](https://zh.wikipedia.org/wiki/%E5%82%85%E7%AB%8B%E5%8F%B6%E5%8F%98%E6%8D%A2 "傅立叶变换")除了在频率 ±ω![{\displaystyle \pm \omega \,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/ee8db88622122d4ab38116712e43d7950a7cec32) 之外处处为 0。但是许多其它的函数或者波形数据并没有这样方便的闭式变换，或者是我们只对某一时间范围内的频谱数据感兴趣，在这种情况下，就需要对有限时间范围的波形进行傅立叶变换或者其它类似的变换。通常通过波形与一个窗函数的乘积来表示。但是，包括矩形窗在内的所有窗函数都会对待测频谱产生影响。

#### 离散时间信号
当输入波形是采样信号而非连续信号时，傅立叶分析通常对信号应用窗函数并用离散傅立叶变换。但是[离散傅里叶变换](https://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2 "离散傅里叶变换")得到的频谱只是[离散时间傅里叶变换](https://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E6%97%B6%E9%97%B4%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2 "离散时间傅里叶变换")频谱的一个粗糙采样。上图是正弦信号应用矩形窗后傅立叶频谱的一部分。位于横轴0点位置的是正弦信号真实频谱，其余频谱均为谱泄漏。频率单位为“DFT bins”（DFT 量化单位）即这些整数值是DFT采样得到的频率。所以该图显示了这样一种情况，正弦信号的实际频率正好与DFT的采样频率一致，并且频谱的最大值通过采样得到。采样错过最大值时的测量误差被称为“扇形损失”（名称源于顶点的形状）。但是这种状况下最有趣的是那些与实际频谱相一致的即值为零的那些点。这种情况下，DFT创造了没有泄露的假象。尽管不如本例这样，泄露是DFT中人为引入的也是普遍误解。但是既然任何窗函数都有泄露，那些表面上的不存在泄露才是人为造成的。

#### 总泄漏
概念的分辨率和动态范围往往是有些主观的，这取决于用户的实际意图。但他们也往往是高度相关，与总泄漏，这是量化的。它通常表示为一个等效带宽，B认为它作为再分配DTFT成长方形的高度等于频谱宽度 B的最大和泄漏的，更大的带宽。它有时被称为噪声等效带宽或等效噪声带宽，因为它是成正比的平均功率将每个登记的DFT并当输入信号包含随机噪声组件（或者只是随机噪声）。图的功率谱，平均随着时间的推移，通常显示一个单位的噪声底，造成这种效果。噪声的高度层是成正比的，所以乙两个不同的窗口功能可以产生不同的噪音楼层。

#### 处理增益与损耗
[![三种不同窗函数造成的处理损耗](https://upload.wikimedia.org/wikipedia/commons/thumb/0/0a/Processing_losses_for_3_window_functions.gif/330px-Processing_losses_for_3_window_functions.gif)](https://zh.wikipedia.org/wiki/File:Processing_losses_for_3_window_functions.gif "三种不同窗函数造成的处理损耗")
在[信号处理](https://zh.wikipedia.org/wiki/%E4%BF%A1%E5%8F%B7%E5%A4%84%E7%90%86 "信号处理")中，透过某些操作，可以利用信号和噪声之间性质的差异，来提高信号某些方面的品质。当某个正弦信号随机地叠加的破坏性噪声，频谱分析能区别的信号与噪声的分量，往往能使检测信号的某些特性更容易，如测量振幅和频率。比如说，若将大多数的正弦信号能量集中于一个频率附近，并将噪声均匀分布，能有效的改善[信噪比](https://zh.wikipedia.org/wiki/%E4%BF%A1%E5%99%AA%E6%AF%94 "信噪比")（signal to noise ratio, SNR）。**处理增益**通常用来描述SNR改善的程度。频谱分析的处理增益取决于窗函数的**噪声带宽（B）**和潜在的**扇形损失**。这些效果可能部分抵消，因为窗函数有最少的扇形自然有最大的渗漏。

右图描述了三种不同的窗函数对相同的数据的影响。此数据包含两个相等强度的正弦信号与外加的噪声。两正弦波的频率被定为其中一个无扇形而另一个有最大扇形。两个正弦信号在Hann Window下都造成比Blackman–Harris window更少的信噪比损耗。 一般情况下，这防止我们在低动态范围的应用采用高动态范围的窗。

#### 不应使用窗函数的应用
在一些应用中，最好不使用窗函数。例如：
- 在冲击模型测试中，当分析[暂态](https://zh.wikipedia.org/wiki/%E6%9A%AB%E6%85%8B "暂态")信号，如重锤打击产生的激发信号（**脉冲激发技术**）时，大部分能量位于记录的开头。若使用非矩形的窗，会无谓的减弱大部分能量和传播[频率响应](https://zh.wikipedia.org/wiki/%E9%A0%BB%E7%8E%87%E9%9F%BF%E6%87%89 "频率响应")。
- 上述的一种推广，当测量[自窗口信号](https://zh.wikipedia.org/w/index.php?title=%E8%87%AA%E7%AA%97%E5%8F%A3%E4%BF%A1%E8%99%9F&action=edit&redlink=1 "自窗口信号（页面不存在）")，如[脉冲](https://zh.wikipedia.org/wiki/%E8%84%89%E5%86%B2 "脉冲")，[冲击响应](https://zh.wikipedia.org/w/index.php?title=%E8%A1%9D%E6%93%8A%E9%9F%BF%E6%87%89&action=edit&redlink=1 "冲击响应（页面不存在）")，正弦[丛发](https://zh.wikipedia.org/w/index.php?title=%E5%8F%A2%E7%99%BC&action=edit&redlink=1 "丛发（页面不存在）")，连续变频丛发，噪声丛发时，这样的信号使用模态分析。在这种情况下应用窗函数只会恶化信噪比。
- 测量一个周期T的[虚拟随机噪声](https://zh.wikipedia.org/w/index.php?title=%E8%99%9B%E6%93%AC%E9%9A%A8%E6%A9%9F%E5%99%AA%E8%81%B2&action=edit&redlink=1 "虚拟随机噪声（页面不存在）")（PRN）激发信号，​​并使用相同的记录周期T时。PRN信号是周期性的，因此，该信号的所有频谱分量将会与FFT区间中心重合而无泄漏。
- 当测量被锁定于采样频率的重复信号，例如在测量的振动频谱分析的轴对准，轴承，发动机，变速器等的故障诊断。由于信号是重复的，所有的频谱能量被限制到基本的重复频率的整数倍。
- 在[正交分频多工](https://zh.wikipedia.org/wiki/%E6%AD%A3%E4%BA%A4%E5%88%86%E9%A0%BB%E5%A4%9A%E5%B7%A5 "正交分频多工")(OFDM)接收器中，输入信号不经窗函数，被直接乘以FFT。的频率子载波（也称为符号）被设计为恰好对齐FFT频率区间。一个Cyclic prefix通常添加到所传输的信号，使得肇因于[多径](https://zh.wikipedia.org/wiki/%E5%A4%9A%E5%BE%84 "多径")的[频率](https://zh.wikipedia.org/wiki/%E9%A0%BB%E7%8E%87_\(%E7%89%A9%E7%90%86%E5%AD%B8\) "频率 (物理学)")[选择性衰落](https://zh.wikipedia.org/wiki/%E9%80%89%E6%8B%A9%E6%80%A7%E8%A1%B0%E8%90%BD "选择性衰落")可用[圆周折积](https://zh.wikipedia.org/wiki/%E5%9C%93%E5%91%A8%E6%91%BA%E7%A9%8D "圆周折积")模拟，从而避免了在OFDM中相当于[频谱泄漏](https://zh.wikipedia.org/w/index.php?title=%E9%A0%BB%E8%AD%9C%E6%B4%A9%E6%BC%8F&action=edit&redlink=1 "频谱泄漏（页面不存在）")的[符号间干扰](https://zh.wikipedia.org/w/index.php?title=%E7%AC%A6%E8%99%9F%E9%96%93%E5%B9%B2%E6%93%BE&action=edit&redlink=1 "符号间干扰（页面不存在）")。

### 典型的窗函数

#### 矩形窗(Rectangular)
每个DFT过程都被窗所修饰限制，因为一个有限时间序列等效上是由无限长时间序列乘上一长度与资料区块长度相同(NΔt)的矩形窗。在矩形窗的例子中，其DFT是由讯号本身的DFT及一矩形的DFT做回旋所产生，然而矩形窗属于时间变量的零次幂窗。矩形窗使用最多， 习惯上不加窗就是使信号通过了矩形窗。这种窗的优点是主瓣比较集中，缺点是旁瓣较高，并有负旁瓣，导致变换中带进了高频干扰和泄漏，甚至出现负谱现象。

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/1/1b/Window_function_%28rectangular%29.svg/250px-Window_function_%28rectangular%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_\(rectangular\).svg)

矩形窗; B=1.00

w(n)=1![{\displaystyle w(n)=1\,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/8ce9f68706e0f2398b03011252fdfc0d2d2a1108)

#### 高斯窗

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/4/49/Window_function_%28gauss%29.svg/250px-Window_function_%28gauss%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_\(gauss\).svg)

高斯窗, σ=0.4; B=1.45
w(n)=e−12(n−(N−1)/2σ(N−1)/2)2![{\displaystyle w(n)=e^{-{\frac {1}{2}}\left({\frac {n-(N-1)/2}{\sigma (N-1)/2}}\right)^{2}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/3ad715a565e0f59905baff5b2f69a74e01f51a97)

σ≤0.5![{\displaystyle \sigma \leq \;0.5\,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/3e43f6fb554bba3e615f0c3cdbb425c8f0149c19)

高斯函数经由傅立叶转换得到的结果，仍然是高斯函数。由于高斯函数会向无限大与负无限大处无限延伸，因此一般都会设定一个截断高斯函数的阈值，或是将高斯函数搭配其他两端数值为零的函数。

另外，由于取高斯函数的对数会产生抛物曲线，此性质可用于频率估测的近二次内插中。

#### 受限高斯窗

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a1/Window_function_and_frequency_response_-_Confined_Gaussian_%28sigma_t_%3D_0.1%29.svg/250px-Window_function_and_frequency_response_-_Confined_Gaussian_%28sigma_t_%3D_0.1%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_and_frequency_response_-_Confined_Gaussian_\(sigma_t_%3D_0.1\).svg)

Confined Gaussian window, _σ__t_ = 0.1; _B_ = 1.9982.

受限高斯窗在给定时间宽度 N_σ__t_ 下，将产生最小可能的方均根频率宽度 _σ__ω_，并最佳化方均根时频带宽的乘积。

#### 近似受限高斯窗

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/7/78/Window_function_and_frequency_response_-_Approximate_confined_Gaussian_%28sigma_t_%3D_0.1%29.svg/250px-Window_function_and_frequency_response_-_Approximate_confined_Gaussian_%28sigma_t_%3D_0.1%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_and_frequency_response_-_Approximate_confined_Gaussian_\(sigma_t_%3D_0.1\).svg)

Approximate confined Gaussian window, _σ__t_ = 0.1; _B_ = 1.9979.

近似受限高斯窗，在给定时间宽度 N_σ__t_ 下，可由下方的式子进一步近似：

w(n)=G(n)−G(−12)[G(n+N)+G(n−N)]G(−12+N)+G(−12−N)![{\displaystyle w(n)=G(n)-{\frac {G(-{\tfrac {1}{2}})[G(n+N)+G(n-N)]}{G(-{\tfrac {1}{2}}+N)+G(-{\tfrac {1}{2}}-N)}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/6c40355672e7157d120b21eb0bb483e2a764f957)

其中 Gaussian 定义为:
G(x)=e−(x−N−122Nσt)2![{\displaystyle G(x)=e^{-\left({\cfrac {x-{\frac {N-1}{2}}}{2N\sigma _{t}}}\right)^{2}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/9b051d6d98473254ba9c32f115d24bc09bab6b0f)

#### Hamming窗与Hann窗

w(n)=a0−(1−a0)⏟a1⋅cos⁡(2πnN−1),0≤n≤N−1,![{\displaystyle w(n)=a_{0}-\underbrace {(1-a_{0})} _{a_{1}}\cdot \cos \left({\tfrac {2\pi n}{N-1}}\right),\quad 0\leq n\leq N-1,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/cff1a2374720284d548f6a59693cd58a297241ec)
当  a0=0.53836![{\displaystyle a_{0}=0.53836}](https://wikimedia.org/api/rest_v1/media/math/render/svg/e92d6e5b6e38296969a6595a76d41acd93a740f3) ，称作 **Hamming窗**；当  a0=0.5![{\displaystyle a_{0}=0.5}](https://wikimedia.org/api/rest_v1/media/math/render/svg/c11c334097d4b793c1474c43f669849d0fedd2c7)  则叫作 **Hann窗**。

##### Hann窗
[![](https://upload.wikimedia.org/wikipedia/commons/thumb/2/28/Window_function_%28hann%29.svg/250px-Window_function_%28hann%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_\(hann\).svg)

Hann窗; B=1.50
w(n)=0.5(1−cos⁡(2πnN−1))![{\displaystyle w(n)=0.5\;\left(1-\cos \left({\frac {2\pi n}{N-1}}\right)\right)}](https://wikimedia.org/api/rest_v1/media/math/render/svg/222fdf0db36de2b69fd6a627da9e38eeb081500d)

[Hann窗](https://zh.wikipedia.org/w/index.php?title=Hann%E7%AA%97&action=edit&redlink=1 "Hann窗（页面不存在）")有时也称为 "Hanning" 窗（“汉宁窗”），以与 Hamming 窗的名称类似。但是这是不对的，因为这两个窗是分别根据 [Julius von Hann](https://zh.wikipedia.org/w/index.php?title=Julius_von_Hann&action=edit&redlink=1 "Julius von Hann（页面不存在）") 和 [Richard Hamming](https://zh.wikipedia.org/wiki/Richard_Hamming "Richard Hamming") 的名字命名的。
Hann窗又称升余弦窗。Hann窗可以看作是3个矩形时间窗的频谱之和，或者说是 3个 sinc(t) 型函数之和，而括号中的两项相对于第一个谱窗向左、右各移动了π/T，从而使旁瓣互相抵消，消去高频干扰和漏能。
从减小泄漏观点出发，Hann窗优于矩形窗。但Hann窗主瓣加宽，相当于分析带宽加宽，频率分辨力下降。

##### Hamming窗

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8c/Window_function_%28hamming%29.svg/250px-Window_function_%28hamming%29.svg.png)](https://zh.wikipedia.org/wiki/File:Window_function_\(hamming\).svg)
Hamming窗; B=1.37
w(n)=0.53836−0.46164cos⁡(2πnN−1)![{\displaystyle w(n)=0.53836-0.46164\;\cos \left({\frac {2\pi n}{N-1}}\right)}](https://wikimedia.org/api/rest_v1/media/math/render/svg/0ad314fef487c2221f7842147ff92ec81c4f32b3)
如果我们将  a0![{\displaystyle a_{0}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/693ad9f934775838bd72406b41ada4a59785d7ba)  设为接近 0.53836 的数值，或是更精确来说是 25/46，便会得到Hamming窗，而设定这个数值的用意，是在频率为 5π/(_N_ − 1) 处产生零交会处(zero-crossing)，使原先Hann窗的第一个旁瓣(sidelobe)可以被大幅消除，产生只有Hann窗 1/5 高度的旁瓣。

## 