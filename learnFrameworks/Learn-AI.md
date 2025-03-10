
# Reference
- []()
- []()
- []()
- []()

# 基础知识
## 机器学习的发展历程

## 机器学习的技术框架





# 配置
## huggingface
- [huggingface](https://huggingface.co)
- [国内镜像](https://hf-mirror.com)
```bash
# 这是huggingface官方指定的下载上传工具
pip install -U huggingface_hub

# 这句放到zshrc
export HF_ENDPOINT=https://hf-mirror.com
```

## openvino和intel-opencl-icd
- [下载openvino](https://www.intel.cn/content/www/cn/zh/developer/tools/openvino-toolkit/download.html?PACKAGE=OPENVINO_BASE&VERSION=v_2025_0_0&OP_SYSTEM=MACOS&DISTRIBUTION=PIP)
- [intel-opencl-icd](https://github.com/intel/compute-runtime)
The following **OpenCL runtimes** expose your Intel integrated **GPU** as an OpenCL device to applications in your PC:

- [Intel compute runtime](https://github.com/intel/compute-runtime) (aka Neo) is the new open-source OpenCL implementation for Intel GPUs. It supports integrated GPUs from Broadwell and onward.
- [Beignet](https://01.org/beignet) is the old open-source OpenCL implementation for Intel integrated GPUs. It supports integrated GPUs since Ivy Bridge. You should use this if you have an older Intel processor.
- [Intel OpenCL](https://software.intel.com/en-us/articles/legacy-opencl-drivers#latest_linux_driver) was a proprietary alternative to Beignet for older processors since Ivy Bridge. In my experience Beignet is preferable if you have an older processor.

The [Intel OpenCL runtime](https://software.intel.com/en-us/articles/opencl-drivers#cpu-section) exposes your **CPU** as an OpenCL device to applications in your PC.

An **OpenCL ICD loader** is vendor-independent and allows you to select which OpenCL device to use at runtime. There are several available, but I suggest [ocl-icd](https://www.archlinux.org/packages/?name=ocl-icd), since it's open source and typically up-to-date.

For developing your own OpenCL programs you also need the **OpenCL headers**, which you can [download from here](https://github.com/KhronosGroup/OpenCL-Headers) or install from the [Arch repositories](https://www.archlinux.org/packages/?name=opencl-headers).

The so called **SDKs** are simply vendor bundles that generally contain: a) an **OpenCL runtime** to expose the vendor hardware as an OpenCL device; b) an **OpenCL ICD loader**, so that various processors can be exposed as OpenCL devices; c) the **OpenCL headers**; d) code samples on how to best use OpenCL on the vendor hardware; and, e) possibly other vendor related stuff.

Some examples of SDKs include:

- AMD APP SDK (discontinued).
- [Intel® SDK for OpenCL™ Applications](https://software.intel.com/en-us/opencl-sdk).

You don't actually need an SDK to develop OpenCL applications, as long as you have an **OpenCL runtime**, an **OpenCL ICD loader** and the **OpenCL headers**. If developed in C/C++, your applications should include the headers and link against an ICD loader (`libOpenCL.so`).

## cuda和cudnn
CUDA（Compute Unified Device Architecture）是 NVIDIA 的通用 GPU 编程模型和 API 框架。
- **核心功能**: 
    - 提供 C/C++/Fortran 等语言的 GPU 编程接口
    - 管理 GPU 内存、线程调度和硬件加速计算
    - 实现 CPU-GPU 异构计算

cuDNN（CUDA Deep Neural Network Library）：专门为深度神经网络优化的 GPU 加速库。
- **核心功能**：
    - 高效实现卷积、池化、归一化等神经网络层
    - 支持 FP16/FP32 精度自动混合计算
    - 提供 Winograd 等加速算法

- [What is the CUDA Software Platform?](https://modal.com/gpu-glossary/host-software/cuda-software-platform)
![/light-cuda-toolkit](learn-AI.assets/light-cuda-toolkit.svg)
- [# 显卡，显卡驱动,nvcc, cuda driver,cudatoolkit,cudnn到底是什么？](https://www.cnblogs.com/marsggbo/p/11838823.html)
pytorch 安装cuda版会自带安装cuda&cudnn的动态库，而如果只是使用whisper这种机遇pytorch的，就不用安装，但是如果编译whisper.cppCUDA版这种，就需要安装官方支持。
### 版本
```bash
# 该指令知识查看当前硬件&驱动可以安装的最高cuda版本，并不代表安装了cuda
nvidia-smi

# 该指令一般是安装了官方的cuda组件（即包含了nvcc编译器）之后可以查看
nvcc -V
```

### pytorch cuda&cudnn动态库
- [pytorch官方安装](https://pytorch.org/get-started/locally/)

### nvidia官方cuda工具包
- [cuda-downloads](https://developer.nvidia.com/cuda-downloads)
- [cudnn-archive](https://developer.nvidia.com/rdp/cudnn-archive)
上述链接是官方下载的，cudnn是个压缩包，对应的文件放到cuda的同名的安装目录中即可。
Linux系统不太一样，也是按照官网链接中的指引（ubuntu可以用 `sudo apt install nvidia-cuda-toolkit` 来安装，但是版本太老，会有问题）
```bash
# 以ubuntu24.04为例，一下指令参考上述nvidia官网链接

# 安装cuda-runfile方式
wget https://developer.download.nvidia.com/compute/cuda/12.6.3/local_installers/cuda_12.6.3_560.35.05_linux.run
sudo sh cuda_12.6.3_560.35.05_linux.run

# 安装cudnn-Deb方式
wget https://developer.download.nvidia.com/compute/cudnn/9.8.0/local_installers/cudnn-local-repo-ubuntu2404-9.8.0_1.0-1_amd64.deb
sudo dpkg -i cudnn-local-repo-ubuntu2404-9.8.0_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2404-9.8.0/cudnn-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cudnn-cuda-12
```
要强调一下：硬件型号-驱动版本−cuda版本−cudnn版本，是有依次的依赖关系的。哪个低了都要依着低的那个版本要求来。比如同样30系显卡要560驱动就可以安装cuda12.6，但是550驱动就最高安装cuda12.4。

### 无法调用？
pytorch显示没有GPU占用---nvidia-smi和windows10的任务管理器都有统计问题，CUDA是默认不在统计范围。在windows10的任务管理器-性能-GPU中可以把默认的copy换成Cuda就可以看到CUDA是可以被调用的。


# 对话大模型


# 文生图大模型
以下是目前主流的文生图（Text-to-Image）大模型及其核心区别的详细分析：

---

### **1. Stable Diffusion 系列**
- **开发者**：Stability AI（开源社区）
- **模型架构**：基于扩散模型（Diffusion Model），Latent Diffusion（在潜空间压缩处理，降低计算量）
- **版本特点**：
  - **SD 1.5**：基础版本，模型大小约 4GB，支持 512×512 分辨率，开源免费，社区插件生态丰富。
  - **SDXL 1.0**：参数量 6.6B，分辨率提升至 1024×1024，双模型结构（Base + Refiner），光影和细节更真实。
  - **SD3 (2024)**：最新版本，引入 Transformer 架构，支持多模态提示（文本+图像混合输入），生成逻辑更接近人类思维。
- **优势**：
  - 开源免费，可本地部署，支持高度自定义（LoRA、ControlNet 等插件）。
  - 社区生态强大，兼容多种工具（WebUI、ComfyUI 等）。
- **劣势**：
  - 默认模型对复杂提示理解较弱，需依赖插件优化。
  - SDXL 和 SD3 对硬件要求较高（显存 ≥8GB）。

---

### **2. DALL·E 系列**
- **开发者**：OpenAI
- **模型架构**：基于扩散模型（DALL·E 3 结合 CLIP 改进）
- **版本特点**：
  - **DALL·E 2**：支持 1024×1024 分辨率，生成风格偏写实，但细节精度有限。
  - **DALL·E 3**：深度融合 ChatGPT，支持长文本和复杂语义（如“一只戴着贝雷帽的柯基犬在画布前创作抽象画”），图像与文本对齐度显著提升。
- **优势**：
  - 文本理解能力最强，适合复杂场景生成。
  - 直接集成于 ChatGPT，交互体验流畅。
- **劣势**：
  - 仅通过 API 或 ChatGPT Plus 付费使用，无法本地部署。
  - 生成风格偏保守，艺术化调整空间小。

---

### **3. Midjourney**
- **开发者**：Midjourney Inc.
- **模型架构**：未公开（推测基于扩散模型+自研优化）
- **特点**：
  - 艺术风格化强烈，擅长光影、构图和抽象表达。
  - 提供丰富的风格参数（如 `--v 5`、`--niji` 动漫模式）。
  - 仅通过 Discord 使用，需订阅付费（$10-$120/月）。
- **优势**：
  - 艺术性最强，适合插画、概念设计。
  - 简单提示即可生成高质量图像，新手友好。
- **劣势**：
  - 闭源，无法自定义模型或本地部署。
  - 对精确控制（如人体结构）较弱，需反复调试。

---

### **4. Adobe Firefly**
- **开发者**：Adobe
- **模型架构**：基于扩散模型，训练数据为 Adobe Stock 版权内容。
- **特点**：
  - 生成内容默认商用无版权风险。
  - 与 Photoshop 等工具深度集成，支持“生成填充”等实用功能。
  - 免费使用（需注册 Adobe 账号），部分功能需订阅。
- **优势**：
  - 适合商业设计，安全性高。
  - 与专业设计工具无缝衔接。
- **劣势**：
  - 艺术风格较单一，创意自由度低。

---

### **5. Google Imagen**
- **开发者**：Google DeepMind
- **模型架构**：基于扩散模型+T5-XXL 文本编码器
- **特点**：
  - 文本理解精准，支持生成文本嵌入图像（如带文字的广告牌）。
  - 仅通过 AI Test Kitchen 有限开放，未全面公测。
- **优势**：
  - 多语言支持优秀，逻辑推理能力强。
- **劣势**：
  - 访问门槛高，无法实际应用。

---

### **6. 中国本土模型**
#### **ERNIE-ViLG（百度文心一格）**
- **架构**：扩散模型+知识增强多模态技术
- **特点**：
  - 中文提示词优化，理解成语、古诗词（如“孤舟蓑笠翁”）。
  - 免费使用，生成速度较快。
- **劣势**：
  - 写实风格精度不足，细节易出错。

#### **通义万相（阿里）**
- **架构**：组合式多模态模型（Composer）
- **特点**：
  - 支持“图像+文本”混合输入，可控性高。
  - 专注电商场景（商品图生成）。

---

### **核心区别对比表**
| 模型               | 文本理解 | 生成质量 | 艺术风格 | 可控性 | 开源/商用 | 硬件要求 |
|--------------------|----------|----------|----------|--------|-----------|----------|
| **Stable Diffusion** | 中       | 高（需调参） | 自由     | 极高   | 开源免费  | 中-高    |
| **DALL·E 3**        | 极高     | 极高      | 写实     | 中     | 闭源付费  | 云端     |
| **Midjourney**      | 高       | 极高      | 艺术化   | 低     | 闭源付费  | 云端     |
| **Adobe Firefly**   | 中       | 中        | 保守     | 中     | 闭源免费  | 云端     |
| **ERNIE-ViLG**      | 中（中文优化） | 中    | 混合     | 低     | 闭源免费  | 云端     |

---

### **选型建议**
1. **追求控制与自定义**：选 Stable Diffusion + ControlNet。
2. **商业安全设计**：Adobe Firefly 或通义万相。
3. **艺术创作**：Midjourney 或 SD 社区艺术模型（如 DreamShaper）。
4. **复杂提示词**：DALL·E 3 或 Google Imagen（若开放）。
5. **中文场景**：ERNIE-ViLG 或 通义万相。

---

### **未来趋势**
- **多模态融合**：文本+图像+3D 联合生成（如 OpenAI 的 Sora 已展示视频生成潜力）。
- **开源与闭源竞争**：Stable Diffusion 3 试图在质量上追赶 DALL·E 3，而闭源模型强化安全与易用性。
- **垂直领域优化**：电商、医疗、教育等场景定制化模型将爆发。
## Stable Diffusion
![SD-develop-timeline](SD-develop-timeline.jpg)
- SD1.5: 
- SDXL: 
- SD3.5: 


# 问题

## 框架

- 机器学习/深度学习整个架构是什么样的？

- 什么是正则防止过拟合？Dropout？为什么现在重点不在这了，更关注模型的设计？

- CNN 是架构吗？卷积层和全连接层是属于CNN的特有模式吗？属于机器学习架构中的哪部分？和其他替代的区别是什么？

- 激活函数是什么？属于机器学习架构中的哪部分？ReLU为什么效果会好一些？还有别的吗？

- 归一化 属于机器学习架构中的哪部分？

- softmax 是什么？  属于机器学习架构中的哪部分？

- 数据增强？模型融合？还在做吗？属于机器学习架构中的哪部分？

- SGD（随即梯度下降）和反向传播算法的关系是什么？替代关系还是互补？属于机器学习架构中的哪部分？ 什么叫学习率？

- 

- 模型切开多GPU训练如何实现？如何通信？ 

## 细节
### 运算的维度？

**对于图像的深度学习处理，一般会将二维的像素占平成一个向量，但是很明显一个二维图像的像素间的排列关系对图像的理解是起决定性作用的，这是一个关键信息，如果把其展平相当于失去了该相对位置关系信息，但如果所有的图像都是统一的规格像素数和长宽都是一样的话，那么就可以默认这种相对位置信息都是一样的，所以它就没有信息量了。**

**但是这样做出来的模型如何处理？不是该分辨率的图像靠接下来的学习解决。**

**一个一维的向量要转换成一个数，也就是需要和另一个向量做内积，而如果输出的这个向量每一个数都是这个一维向量跟一个不同的向量做内积，那么这两个向量的转换关系就可以写成矩阵的乘法。**

**这时候，我们就可以做一些推广，如果输入的一个样本是一个矩阵，比如是一张图片，也就是说这个样本中的独立元素是一个二维的特征，那他如果要对应另一个二维特征，就需要一个四为张量来做运算。**

**因为输出矩阵中的每一个数，都需要和输入的所有数做线性运算，而这一个数运算的“系数”都是不一样的。**

**如果是一样的，那就是一组一维样本组成的矩阵，只需要乘系数矩阵（而不是四维张量）就得到了一组输出。**


**再降维类比一下：如果一个向量（一维），不需要做****“****独立****”****线性运算（系数是不一样的）得到另一个向量，它甚至只需要乘一个常数就可以，而非一个矩阵。这两者的区别理解了，就能理解矩阵****“****全连接****”****变换需要四维张量了。**

### 向后传播求梯度？

#### 分清何时谁是变量
h(x, y)，表示一个山坡的梯度。x，y是独立的。作为自变量，而这个函数假设是 h(x, y) = ax + by。那么这个山坡定死了，a和b就定死了，不同的位置(x, y)，就有不同的高度h。那么(x, y)就是输入。

当训练根据loss function调整参数时，输入就变成了定死了的“参数”，而参数就变成了变量（一定要注意和使用模型时做好区分）。

#### 独立与嵌套？
还是以函数h(x, y)为例，假设h(x,y) = (ax + b)^y，那么类比到训练中loss function，x就类似倒数第二层参数，y就类似最后一层参数（忽略激活函数）。所以嵌套和独立是没有关系的？
#### 独立参数独立，但是对应的偏导数会不独立
在h(x,y)中x和y是独立的，但是，x偏导和y偏导这两个“变量”是不独立的。

#### 偏导数与梯度向量？
定义来看，梯度向量就是独立参数的偏导数组成的向量。但是实际上我们求偏导数时的偏差不是无穷小，而是一个相对较小值，也就是说：
还是以函数h(x, y)为例，如果求了x偏导数，然后走到了x'(新的参数)，那么这h(x‘, y)的y偏导和h(x, y)是不一样的，如果参数特别多，层数还多就会造成问题。
当然，我们是可以同时求解的，但是如果不能并行，即最终还是串行分别求解的话，是不是就可以求h(x‘, y)的y偏导而不是h(x, y)的，以提高准确性？

#### 向后传播与并行？
还是以函数h(x, y)为例，我们是可以
向后传播目前看来就是先求偏导再求y偏导的（先求最后一层偏导，再求上一层）。原因也很简单，因为这个函数太复杂，不是有一个确定解析解，而都是求的数值解。类比h(x,y)特别复杂，那我们就不知道它的导函数解析式，而是根据导数定义和当前变量值求出当前的导数值来。这时候就很难并行了？
如果不能并行，是不是我们先求最后一层参数的偏导数，更新了之后再根据最后更新的loss function求倒数第二层的参数值？当然如果能并行求每层参数导数的话，还是并行整体更优。

#### 问题
每层之间的参数和各层之间的参数有何本质不同？每层之间类似h(x, y) = ax + by？各层之间类似h(x,y) = (ax + b)^y？还是说本质没有区别？换句话说：为何向后传播算法需要逐层求解梯度，而不需要逐个求解梯度，如果不需要逐个求解（即每层的参数可以并行），各层之间的参数为何要逐层求解而不是可以并行？

#### 解答
不能并行是因为偏导不独立！之前的举例还是会不恰当，因为线性运算本身就是一种特殊的函数，而这种函数的偏导数也是相互独立的，类似h(x, y) = ax + by函数的x偏导和y偏导还是没有关系。但是两个（多个）嵌套的线性运算就不再是线性运算了？何况中间还夹了非线性的激活函数，所以这个最终的复杂函数求偏导后就互相耦合了，则这些偏导数不是独立变量了。
但是即使对于比如x的偏导数有y、z等变量，但是对于每步来说，这些数存放在内存中是不变的，可以拿来用。而对于嵌套矩阵运算来说，偏导数的形式也是固定的，这样还是可以并行运算的？具体怎样实现还需要研究。

#### chatgpt解释

### 模型格式

- [大模型格式](https://zhuanlan.zhihu.com/p/686570419)