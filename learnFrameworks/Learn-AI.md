
# Reference
- [zh.d2l.ai](https://zh.d2l.ai/)
- [zh-d2l-github](https://github.com/d2l-ai/d2l-zh)
- [transformer快速入门](https://github.com/jsksxs360/How-to-use-Transformers)



# 机器学习的发展历程

# 机器学习的技术框架

## 语言大模型


## 文生图大模型
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
### Stable Diffusion
![SD-develop-timeline](SD-develop-timeline.jpg)
- SD1.5: 
- SDXL: 
- SD3.5: 




## 时序预测
- [Time-Series-Library](https://github.com/thuml/Time-Series-Library)
- [深度学习-时间序列分析工具TSLiB库使用指北](https://blog.csdn.net/LuohenYJ/article/details/141651215)



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
- [cudnn-support-matrix](https://docs.nvidia.com/deeplearning/cudnn/frontend/v1.14.0/reference/support-matrix.html#support-matrix)（和cuda、nvidia driver的兼容性）
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

### tensorflow和cuda的兼容性
- [tensorflow和cuda的兼容性](https://www.tensorflow.org/install/source#gpu)


# 外围框架

## RAG


### RAG AI介绍

#### **一、技术原理**

**RAG（Retrieval-Augmented Generation）** 是一种结合检索（Retrieval）与生成（Generation）的技术，通过动态引入外部知识库来增强大语言模型（LLM）的能力。其核心思想是：在生成答案前，先从海量文档中检索相关上下文，再将检索结果与用户输入结合，指导大模型生成更准确、更可靠的回答。

---

##### **1. RAG 的工作流程**
RAG 的实现通常分为以下步骤：

1. **用户输入（Query）**  
   用户提交问题或请求，例如：“如何治疗普通感冒？”

2. **文档检索（Retrieval）**  
   - **嵌入模型（Embedding Model）**：将用户查询和外部知识库中的文档均转换为向量（Embeddings）。  
   - **向量数据库（Vector Database）**：使用向量相似度计算（如余弦相似度），从数据库中检索与查询最相关的文档片段。  
   - **常见工具**：FAISS、Pinecone、Milvus、Elasticsearch 等。

3. **上下文增强（Augmentation）**  
   将检索到的相关文档（例如医学文献、药品说明书）与用户输入拼接，形成增强后的输入提示（Prompt），例如：  
   ```  
   根据以下文档内容回答问题：  
   [文档1] 普通感冒通常由病毒引起，建议多休息、补充水分……  
   [文档2] 常用药物包括布洛芬缓解症状……  
   问题：如何治疗普通感冒？  
   ```

4. **生成答案（Generation）**  
   大模型（如 GPT-4、Claude）基于增强后的上下文生成最终回答，例如：  
   ```  
   普通感冒的治疗方法包括：  
   5. 多休息和补充水分；  
   6. 使用布洛芬缓解症状；  
   7. 避免滥用抗生素（因感冒多由病毒引起）。  
   ```

---

##### **2. 关键技术细节**
- **检索优化**：  
  - **多阶段检索**：先通过关键词匹配（如 BM25）粗筛文档，再用向量检索精排。  
  - **动态分块**：根据文档内容动态调整文本块大小（如按段落或语义分割），提升检索精度。  
  - **混合检索**：结合结构化数据（如数据库表）与非结构化文本（如 PDF 文档）。

- **生成优化**：  
  - **提示工程（Prompt Engineering）**：设计模板引导模型关注检索内容，例如：  
    ```  
    请严格根据以下文档回答，若文档未涵盖则回答“未知”：  
    [检索内容]  
    问题：{用户输入}  
    ```  
  - **上下文压缩**：对过长检索结果进行摘要或筛选，避免超出模型输入长度限制。  
  - **置信度校准**：模型在回答时标注引用来源（如文档 ID），增强可信度。

---

#### **二、RAG 的优势与挑战**

##### **1. 核心优势**
- **准确性提升**：直接基于权威文档生成答案，减少模型“幻觉”（编造信息）。  
- **知识动态更新**：无需重新训练模型，仅需更新外部知识库即可适应新领域。  
- **可解释性增强**：答案来源可追溯（如标注引用文献），满足合规需求。  
- **低成本部署**：相比微调（Fine-tuning），无需大量标注数据与算力消耗。

##### **2. 主要挑战**
- **检索精度瓶颈**：若检索结果不相关，生成答案可能偏离主题。  
- **上下文长度限制**：大模型的输入长度有限（如 GPT-4 最大 128k tokens），需优化文档分块策略。  
- **实时性要求**：某些场景（如股票分析）需实时更新知识库，对检索系统提出高要求。  
- **多模态支持**：处理图像、表格等非文本数据的检索与生成仍存在技术难点。

---

#### **三、典型应用场景**

##### **1. 智能问答系统**
- **场景**：企业知识库、医疗咨询、法律助手。  
- **案例**：  
  - **医疗问答**：结合最新医学指南，回答患者关于症状、用药的问题。  
  - **法律咨询**：检索法律条文和判例，生成合规建议。

##### **2. 内容生成与编辑**
- **场景**：新闻撰写、营销文案、代码生成。  
- **案例**：  
  - **技术文档生成**：基于产品手册和 API 文档，自动生成用户帮助文档。  
  - **个性化推荐**：根据用户历史行为生成定制化内容（如旅行攻略）。

##### **3. 数据分析与决策支持**
- **场景**：金融分析、市场报告、科研辅助。  
- **案例**：  
  - **财报分析**：检索企业年报与行业数据，生成收益预测摘要。  
  - **科研文献综述**：从论文库中提取相关研究，总结领域进展。

##### **4. 客户服务与自动化**
- **场景**：智能客服、技术支持、售后服务。  
- **案例**：  
  - **产品故障排查**：根据设备手册和维修记录，指导用户解决问题。  
  - **订单查询**：结合数据库实时信息，回复物流状态。

---
#### **四、RAG 的进阶方向**
1. **主动学习（Active Learning）**  
   根据用户反馈优化检索策略，动态调整知识库优先级。  
2. **多模态 RAG**  
   支持图像、视频、音频等多模态数据的检索与生成。  
3. **端到端优化**  
   联合训练检索器与生成模型，提升整体一致性（如 Google 的 REALM）。  
4. **边缘计算部署**  
   在本地设备（如手机）上实现轻量化 RAG，保护用户隐私。

---
#### **五、总结**
RAG 通过结合检索与生成技术，显著提升了大模型在事实准确性、知识更新速度和应用范围上的表现。随着向量数据库、多模态模型等技术的进步，RAG 将在医疗、金融、教育等领域发挥更大价值，成为构建可靠 AI 系统的核心组件之一。

## function call & [MCP](https://modelcontextprotocol.io/introduction)
MCP其实就是各家function call函数的统一接口标准，但截至25年还没兴起，最终也可能会兴起新的标准，但标准肯定是趋势，技术细节还是先从function call学起。



[MCP-pythonSDK](https://github.com/modelcontextprotocol/python-sdk)





# 部署(推理)


##  部署框架
| 框架/平台            | 核心优势                                | 跨平台支持 (CPU/GPU)        | 部署灵活性 & 异构兼容性      | 代表性应用场景                        |
| ---------------- | ----------------------------------- | ---------------------- | ------------------ | ------------------------------ |
| **ONNX Runtime** | 推理优化、**标准模型格式支持**、硬件后端抽象            | **广泛** (x86, ARM, GPU) | **高** (EP机制灵活切换后端) | 边缘设备、云原生推理、多硬件环境统一部署           |
| **Triton**       | **高性能推理**、多框架支持、**动态批处理**           | **广泛** (x86, ARM, GPU) | **高** (支持多种硬件和平台)  | 高并发云服务、复杂模型管道、混合负载 (CPU/GPU协同) |
| **LMDeploy**     | **大模型（LLM）专用**、**推理性能优化**、量化        | 支持 (GPU为主)             | 中 (专注LLM高效推理)      | 大语言模型低延迟推理、消费级GPU部署、降低显存消耗79   |
| **FastDeploy**   | **端到端部署**、**多硬件适配** (国产芯片)、工具链完整    | **广泛** (x86, ARM, GPU) | **高** (国产芯片深度适配)   | 国产化环境、全场景（云边端）部署、快速原型验证        |
| **OpenVINO**     | **Intel硬件深度优化** (CPU, iGPU)、计算机视觉优势 | x86, Intel GPU         | 中 (Intel生态最佳)      | Intel平台高性能推理、边缘AI、计算机视觉应用      |

## onnxruntime
- [microsoft-onnxruntime](https://github.com/microsoft/onnxruntime)
- [onnxruntime-doc](https://onnxruntime.ai/docs/)

### onnxruntime-gpu和cuda的版本关系
- [onnx-CUDA-ExecutionProvider](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html)
#### CUDA 12.x 

|ONNX Runtime|CUDA|cuDNN|Notes|
|---|---|---|---|
|1.20.x|12.x|9.x|Avaiable in PyPI. Compatible with PyTorch >= 2.4.0 for CUDA 12.x.|
|1.19.x|12.x|9.x|Avaiable in PyPI. Compatible with PyTorch >= 2.4.0 for CUDA 12.x.|
|1.18.1|12.x|9.x|cuDNN 9 is required. No Java package.|
|1.18.0|12.x|8.x|Java package is added.|
|1.17.x|12.x|8.x|Only C++/C# Nuget and Python packages are released. No Java package.|

#### CUDA 11.x 

|ONNX Runtime|CUDA|cuDNN|Notes|
|---|---|---|---|
|1.20.x|11.8|8.x|Not available in PyPI. See [Install ORT](https://onnxruntime.ai/docs/install) for details. Compatible with PyTorch <= 2.3.1 for CUDA 11.8.|
|1.19.x|11.8|8.x|Not available in PyPI. See [Install ORT](https://onnxruntime.ai/docs/install) for details. Compatible with PyTorch <= 2.3.1 for CUDA 11.8.|
|1.18.x|11.8|8.x|Available in PyPI.|
|1.17  <br>1.16  <br>1.15|11.8|8.2.4 (Linux)  <br>8.5.0.96 (Windows)|Tested with CUDA versions from 11.6 up to 11.8, and cuDNN from 8.2 up to 8.9|
|1.14  <br>1.13|11.6|8.2.4 (Linux)  <br>8.5.0.96 (Windows)|libcudart 11.4.43  <br>libcufft 10.5.2.100  <br>libcurand 10.2.5.120  <br>libcublasLt 11.6.5.2  <br>libcublas 11.6.5.2  <br>libcudnn 8.2.4|
|1.12  <br>1.11|11.4|8.2.4 (Linux)  <br>8.2.2.26 (Windows)|libcudart 11.4.43  <br>libcufft 10.5.2.100  <br>libcurand 10.2.5.120  <br>libcublasLt 11.6.5.2  <br>libcublas 11.6.5.2  <br>libcudnn 8.2.4|
|1.10|11.4|8.2.4 (Linux)  <br>8.2.2.26 (Windows)|libcudart 11.4.43  <br>libcufft 10.5.2.100  <br>libcurand 10.2.5.120  <br>libcublasLt 11.6.1.51  <br>libcublas 11.6.1.51  <br>libcudnn 8.2.4|
|1.9|11.4|8.2.4 (Linux)  <br>8.2.2.26 (Windows)|libcudart 11.4.43  <br>libcufft 10.5.2.100  <br>libcurand 10.2.5.120  <br>libcublasLt 11.6.1.51  <br>libcublas 11.6.1.51  <br>libcudnn 8.2.4|
|1.8|11.0.3|8.0.4 (Linux)  <br>8.0.2.39 (Windows)|libcudart 11.0.221  <br>libcufft 10.2.1.245  <br>libcurand 10.2.1.245  <br>libcublasLt 11.2.0.252  <br>libcublas 11.2.0.252  <br>libcudnn 8.0.4|
|1.7|11.0.3|8.0.4 (Linux)  <br>8.0.2.39 (Windows)|libcudart 11.0.221  <br>libcufft 10.2.1.245  <br>libcurand 10.2.1.245  <br>libcublasLt 11.2.0.252  <br>libcublas 11.2.0.252  <br>libcudnn 8.0.4|


## 国产GPU

### 华为NPU-CANN框架
- [CANN](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/82RC1/index/index.html)
- [Atlas 推理卡 NPU驱动和固件安装指南](https://support.huawei.com/enterprise/zh/doc/EDOC1100493509/426cffd9)
- [华为软件包](https://support.huawei.com/enterprise/zh/ascend-computing/cann-pid-251168373/software/258923273?idAbsPath=fixnode01|23710424|251366513|22892968|251168373)
- [ascend gitee 仓库](https://gitee.com/ascend)
	- [torch-npu](https://gitee.com/ascend/pytorch)

![](learn-AI.assets/华为NPU框架.png)

CANN（Compute Architecture for Neural Networks）是华为为昇腾（Ascend）AI处理器打造的**全栈软件平台和异构计算架构**。它的核心使命是充分发挥昇腾AI处理器的强大算力，简化AI应用开发与部署，并充当上层深度学习框架（如TensorFlow, PyTorch, MindSpore）与底层昇腾硬件之间的“桥梁”。

官方文档详细介绍Host和Device的概念以及标准/开放形态，是因为这直接关系到开发者**如何利用CANN进行应用开发、资源调配和性能优化**，是实现其“连接AI框架与硬件的关键角色”这一目标的基础。

CANN官方文档介绍Host（主机）和Device（设备）的概念，以及标准形态（EP模式）与开放形态的区别，主要是因为这关系到**开发模式、资源利用和性能优化**：

*   **Host（主机）**：通常指与昇腾AI处理器相连的**X86或ARM服务器**。它负责整体的控制流、业务逻辑，以及不适合在AI处理器上运行的计算（如某些自定义预处理或后处理）。
*   **Device（设备）**：指安装了**昇腾AI处理器的硬件板卡**（如Atlas 300I推理卡），通过PCIe接口与Host服务器连接。它专注于提供强大的神经网络（NN）计算能力。

这种划分源于异构计算的常见模式，即**专用协处理器（Device）与通用主机（Host）协同工作**。

CANN支持两种主要的Device工作形态，以适应不同的场景需求：

1.  **标准形态（EP模式）**：
    *   􀋾 **Device作为被动协处理器**：在此形态下，昇腾AI处理器工作于**EP（Endpoint）模式**。它作为PCIe总线上的一个从设备，**其上的CPU资源通常仅能通过Host调用**。
    *   􀋾 **常见开发流程**：AI应用程序（如模型推理的所有步骤）主要**运行在Host侧**。Host通过CANN提供的接口（如AscendCL）调用Device的算力。
    *   􀋾 **适用场景**：**大多数推理场景**，开发相对简单，资源管理集中于Host。

2.  **开放形态**：
    *   􀋾 **释放Device侧CPU算力**：此形态下，开发者可以**利用Device板载的Control CPU的通用计算能力**。
    *   􀋾 **开发流程变化**：需要为Device侧的CPU**编译专用的应用程序**（通常使用华为的HCC编译器），并将其放入Device的文件系统镜像中。
    *   􀋾 **主要优势**：
        *   **降低Host负载**：将一些计算任务（如图像/视频预处理）卸载到Device端执行。
        *   **减少数据传输**：数据在Device内部处理，避免了在Host和Device之间的大量数据传输，从而**降低延迟、提升整体效率**。
    *   􀋾 **适用场景**：对**延迟敏感**或希望**最大化利用Device资源**的应用。


官方文档详细介绍Host、Device及两种形态，主要是因为：

*   **明确开发环境配置**：让开发者清楚知道**代码的不同部分将在何处运行**（Host的X86/ARM CPU 还是 Device的NPU或Control CPU），从而正确设置编译环境、链接库和部署路径。
*   **理解性能瓶颈与优化方向**：数据传输 between Host and Device 往往是性能瓶颈之一。了解开放形态的存在，就知道可以通过**将更多计算任务卸载到Device端**来减少数据交换，从而提升性能。
*   **实现资源高效利用**：引导开发者根据实际需求选择合适的形态，避免Device侧CPU资源的闲置，实现**异构计算资源的精细化利用和效率最大化**。
*   **避免概念混淆**：清晰区分Host和Device，有助于理解CANN的工具链（如哪些工具用在Host上分析Device状态）。

CANN 是华为昇腾AI生态的软件核心，它通过一系列工具和接口，让开发者能高效利用昇腾芯片的强大算力。

理解**Host（控制与通用计算）、Device（专用AI计算）** 的概念及**标准（集中控制）、开放（分布式利用）** 两种形态，对于在昇腾平台上进行**高效的应用开发、性能优化和资源管理至关重要**。这直接反映了CANN的设计哲学：**充分发挥异构计算优势，提供灵活且高性能的AI计算解决方案**。#### CANN安装

```bash
# conda 在线自动安装 CANN
conda config --add channels https://repo.huaweicloud.com/ascend/repos/conda/
conda install ascend::cann-toolkit
```

#### CANN组件

| CANN组件/功能类别   | 关键组成部分/技术举例                                 | 主要功能简介                                       |
| :------------ | :------------------------------------------ | :------------------------------------------- |
| **统一编程接口**    | AscendCL (Ascend Computing Language)        | 提供设备管理、内存管理、任务调度等API，是开发者直接调用的主要接口。          |
| **基础计算库与算子**  | ACL (Ascend Computing Library), AOL算子库      | 提供高效的基础数学运算（如BLAS）和深度优化过的AI算子（如卷积、矩阵运算）。     |
| **深度学习框架支持**  | torch_npu (PyTorch适配), MindSpore            | 实现与主流深度学习框架的无缝集成，允许框架调用昇腾硬件。                 |
| **模型转换与部署工具** | ATC (Ascend Tool Chain)                     | 将其他框架（如ONNX, Caffe）训练的模型转换为昇腾处理器可执行的格式（.om）。 |
| **编译与执行引擎**   | 图编译器, Runtime运行时                            | 将计算图转为硬件可执行指令，并进行深度优化（如算子融合、内存优化）。           |
| **性能调优工具**    | AOE (Ascend Optimization Engine), Profiling | 自动或辅助进行性能调优，例如优化算子调度策略。                      |
| **高级开发与调试支持** | Ascend C语言, msdebug调试工具                     | 支持开发者进行底层算子开发和调试。                            |
| **异构计算管理**    | 调度不同计算单元（NPU, CPU等）                         | 智能分配任务到合适的计算单元，以实现最佳计算性能。                    |
#### CANN依赖

| 类别                                                                                                          | 名称                  | 版本要求                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| :---------------------------------------------------------------------------------------------------------- | :------------------ | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 安装时所需工具                                                                                                     | Python              | Python3.7._x_至3.11.4版本。<br><br>如果需安装NNAL软件包的Python库，请安装Python3.10._x_或3.11._x_版本。<br><br>如果需安装TensorFlow，请安装要求的Python版本：<br><br>- TensorFlow1.15配套的Python版本是：Python3.7._x_（3.7.5~3.7.11）。<br>- TensorFlow2.6.5配套的Python版本是：Python3.7._x_（3.7.5~3.7.11）、Python3.8._x_、Python3.9._x_。<br><br>安装失败、版本不满足或者未包含动态库libpython3._x_.so请参考[编译安装Python](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/82RC1/softwareinst/instg/instg_0061.html#ZH-CN_TOPIC_0000002366267590)操作。 |
| python3-pip                                                                                                 | 与已安装的Python版本配套使用。  |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| 运行时所需工具                                                                                                     | gcc                 | >=7.3.0，以系统源提供的版本为准。                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| g++                                                                                                         | 与已安装gcc版本配套使用。      |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| 运行时所需Python第三方库                                                                                             | numpy               | 大于等于1.19.2，小于等于1.24。<br><br>Python3.7.x时推荐安装numpy 1.21.6版本。                                                                                                                                                                                                                                                                                                                                                                                                                              |
| decorator                                                                                                   | >=4.4.0             |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| sympy                                                                                                       | >=1.5.1             |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| cffi                                                                                                        | >=1.12.3            |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| protobuf                                                                                                    | 3.20._x_            |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| attrs<br><br>cython<br><br>pyyaml<br><br>pathlib2<br><br>scipy<br><br>requests<br><br>psutil<br><br>absl-py | 无版本要求，安装的版本以pip源为准。 |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| 软件包运行时依赖                                                                                                    | glibc               | 运行NNAL加速库时，glibc版本需大于等于2.17，执行**ldd --version**可以查询glibc版本（主流Linux系统均满足glibc版本要求，若不满足要求，**建议通过重装新版本的系统解决**，不推荐直接升级glibc，直接升级glibc可能导致系统崩溃）。                                                                                                                                                                                                                                                                                                                                              |

#### [onnxruntime-cann](https://onnxruntime.ai/docs/execution-providers/community-maintained/CANN-ExecutionProvider.html)
```bash
pip install onnxruntime-cann
```

```python
import numpy as np
import onnxruntime as ort

providers = [
    (
        "CANNExecutionProvider",
        {
            "device_id": 0,
            "arena_extend_strategy": "kNextPowerOfTwo",
            "npu_mem_limit": 2 * 1024 * 1024 * 1024,
            "enable_cann_graph": True,
        },
    ),
    "CPUExecutionProvider",
]

model_path = '<path to model>'

options = ort.SessionOptions()
options.graph_optimization_level = ort.GraphOptimizationLevel.ORT_DISABLE_ALL
options.execution_mode = ort.ExecutionMode.ORT_PARALLEL

session = ort.InferenceSession(model_path, sess_options=options, providers=providers)

x = np.array([[1.0, 2.0], [3.0, 4.0], [5.0, 6.0]], dtype=np.int64)
x_ortvalue = ort.OrtValue.ortvalue_from_numpy(x, "cann", 0)

io_binding = sess.io_binding()
io_binding.bind_ortvalue_input(name="input", ortvalue=x_ortvalue)
io_binding.bind_output("output", "cann")

sess.run_with_iobinding(io_binding)

return io_binding.get_outputs()[0].numpy()
```



## docker 部署onnx-cuda

docker不依赖宿主机的cuda，但却依赖宿主机的显卡驱动。另外，docker 运行还需要安装[Nvidia Container Toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)(win需要wsl2安装后额外配置，下文介绍)；且docker run 指定gpu。


### docker cuda 支持

#### 1. [Nvidia Container Toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)
- 这是 **Linux 上的通用方案**。
- 如果你在 **裸机 Linux**（比如 Ubuntu server）上跑 Docker，要让容器访问 GPU，就必须装 nvidia-container-toolkit。
- 它负责把宿主机上的 GPU 驱动、CUDA 库 mount 到容器里。
- 没有它，Linux 上的 Docker 就看不到 GPU。
##### 1.1 安装


##### 1.2 配置



#### 2. [CUDA on WSL](https://developer.nvidia.com/cuda/wsl)

要注意的是：wsl2和里面的ubuntu发行版是不同的，wsl2是虚拟机(特殊的)，docker和ubuntu都可以运行在上面。

- 这是 **NVIDIA + Microsoft** 合作的方案，让 WSL2 (Linux 子系统) 可以调用 Windows 驱动里的 GPU。
- 本质上：**让 WSL2 里的 Linux 看见显卡**。
- 如果你不用 WSL，而是直接在 Windows Docker Desktop 里跑容器，这一层就没关系。
#### 3. [GPU support in Docker Desktop for Windows](https://docs.docker.com/desktop/features/gpu/)

- 这是 **Docker Desktop** 官方提供的 GPU 集成功能。
- 在 Windows 上的 Docker Desktop **自带对 GPU 的支持**，不需要你在 Windows 上额外装 NVIDIA Container Toolkit。
- 它会自动对接 WSL2 里的 GPU (上面第 1 步)，然后让容器里可以看到 GPU。
- 所以在 Windows Docker Desktop 上跑 GPU 容器，流程是：
    1. Windows 装好 NVIDIA 驱动 (>= 470)。
    2. Docker Desktop 打开 GPU 支持。
    3. 直接在容器里 --gpus all 就能用 GPU。





### docker pytorch cuda

显卡驱动可以向下兼容cuda版本，所以宿主机的显卡驱动要比较新，然后接下来基础镜像有四种方案：
- 拉一个python，然后容器内装cuda和pip的onnxruntime
- 拉一个[cuda](https://hub.docker.com/r/nvidia/cuda/tags)，然后容器内装python
- 拉一个[PyTorch-cuda-runtime](https://hub.docker.com/r/pytorch/pytorch/tags)，然后里面装onnxruntime
- 拉一个[python-cuda-onnx](https://hub.docker.com/r/microsoft/azureml-onnxruntimefamily)，只需要自己w装库[onnx-cuda-docker博客（已过时）](https://blog.csdn.net/weixin_42939529/article/details/122006947)

虽然看似最后一种方案最好，但是这个镜像是微软做的，只很少几个版本的组合，现在也不再支持，如果不符合自己的版本，可能自己的软件出现兼容性问题，它现在提供[onnx-github-Dockerfile.cuda](https://github.com/microsoft/onnxruntime/blob/main/dockerfiles)可以自行构建（网络问题……构建也很慢）。

坑最少的还是第三种方案，但是也需要注意两点：
1. onnxruntime的版本所需要的一定要和pytorch所带的cuda和cudnn版本一致。
2. 代码中的导入方式不太一样，具体见onnx官网doc

### [tensorflow-docker-gpu](https://hub.docker.com/r/tensorflow/tensorflow/tags)



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

### tensorflow内存overflow问题
用tf2onnx，然后用onnx推理就没有内存overflow的问题了。
