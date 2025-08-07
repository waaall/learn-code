
# Docker 基本概念

## Docker 和 app 打包、虚拟机的异同

问：
比如我要部署一个pyside6的GUI APP项目，要在各种操作系统和python版本的环境下部署，这时候又四种方式可以试下在其他环境下运行：1. python虚拟环境， 2. 虚拟机（比如我是在ubuntu 20.04开发的，就可以下载一个虚拟机） 3. pyinstaller打包， 4. docker部署，除了这四种跨环境的运行实现方式还有别的吗？全面分析一下他们的异同


答：

**1. Python 虚拟环境**

**优点**：
• **依赖隔离**：不同项目的依赖不会冲突。
• **灵活性**：开发和测试时快速切换 Python 版本和包版本。

**缺点**：
• **不解决操作系统差异**：无法跨操作系统运行。
• **用户需要 Python 环境**：终端用户需要安装 Python，可能增加复杂性。
• **不适合最终用户分发**：更适合开发和测试阶段。


**2. 虚拟机**

**优点**：
• **环境一致性**：完全复制开发环境，消除环境差异。
• **隔离性**：提供操作系统级别的隔离，安全性高。

**缺点**：
• **资源消耗大**：占用大量的内存和存储空间。
• **性能开销**：运行速度可能比原生环境慢。
• **用户体验差**：终端用户需要安装和配置虚拟机，不友好。
  

**3. PyInstaller 打包**

**优点**：
• **生成可执行文件**：无需用户安装 Python 环境。
• **代码保护**：源代码被打包，不易被查看。
• **跨平台支持**：可在不同操作系统上生成对应的可执行文件。

**缺点**：
• **打包复杂性**：有时需要处理打包问题，如动态库依赖。
• **文件体积大**：可执行文件可能很大，包含所有依赖。
• **兼容性问题**：不同平台可能出现不一致的行为，需要分别调试。
  

**4. Docker 部署**

**优点**：
• **环境一致性**：确保在任何地方运行都是相同的环境。
• **轻量级**：比虚拟机更轻，占用资源少。
• **依赖管理**：所有依赖都在容器中，主机环境干净。
  
**缺点**：
• **GUI 支持复杂**：默认情况下，Docker 不支持 GUI 应用程序，需要额外配置（如 X11 转发）。
• **学习成本**：用户需要了解 Docker 的基本使用。
• **不适合桌面应用分发**：更适合服务器和服务的部署。


## Docker 开发部署和虚拟环境的异同
问：
比如用python的pytorch做深度学习开发，我设置一个python的虚拟环境，和pytorch的docker有何区别，如果在不同的机器快速部署我当前这套开发环境的话，哪个比较合适？


答：
在使用 Python 的 PyTorch 进行深度学习开发时，**Python 虚拟环境**和使用 **PyTorch 的 Docker 容器**之间存在一些关键区别。以下是对它们的比较，以及在不同机器上快速部署当前开发环境的建议。

**Python 虚拟环境**

**优点**：
• **依赖隔离**：虚拟环境可以隔离不同项目的 Python 包依赖，防止冲突。
• **轻量级**：不需要额外的系统资源，创建和激活都很快捷。
• **灵活性**：可以根据需要安装特定版本的 PyTorch 和其他库。

**缺点**：
• **系统依赖**：仍然依赖于主机操作系统，可能需要手动安装系统级别的依赖，如 CUDA 驱动、特定的库等。
• **环境可移植性**：在不同机器上重现同样的环境可能会遇到困难，尤其是当操作系统或硬件配置不同的时候。
• **配置复杂性**：需要确保每台机器上的 Python 版本和系统库一致，手动配置可能耗时。


**PyTorch 的 Docker 容器**

**优点**：
• **环境一致性**：Docker 容器包含了操作系统、Python 解释器、PyTorch 及其所有依赖，确保在不同机器上运行的环境完全一致。
• **易于部署**：只需在目标机器上安装 Docker，然后拉取并运行容器即可，无需手动配置环境。
• **包含系统依赖**：如 CUDA 驱动、cuDNN 等都可以在容器中预先配置好，避免了在不同系统上安装这些依赖的麻烦。
• **可移植性**：容器可以在任何支持 Docker 的平台上运行，无论是本地、服务器还是云环境。

**缺点**：
• **资源占用**：Docker 容器相对于虚拟环境更占用系统资源，虽然比虚拟机轻量，但仍需要一定的存储空间和内存。
• **学习成本**：需要了解 Docker 的基本使用和命令，对不熟悉容器技术的用户可能有一定的学习曲线。
• **GPU 配置**：如果需要在容器中使用 GPU，需要配置 NVIDIA Docker，步骤相对复杂。


**哪个更适合快速在不同机器上部署当前的开发环境？**

如果您的目标是**快速、一致地在不同机器上部署完全相同的开发环境**，那么 **Docker 容器**更为合适，原因如下：
• **一致性**：Docker 镜像可以确保在不同的机器上运行相同的环境，避免了“在我机器上可以运行”的问题。
• **简化部署**：只需安装 Docker，然后运行预先构建好的容器，无需在每台机器上手动配置环境。
• **减少系统差异影响**：Docker 容器包含了运行所需的所有组件，主机系统的差异（如操作系统版本、系统库等）对容器内的应用影响很小。

然而，如果您的开发环境相对简单，且目标机器的系统环境与开发机器相似（如操作系统版本、Python 版本、CUDA 驱动等），并且您或您的团队对 Docker 不太熟悉，那么使用 **Python 虚拟环境**也可以满足需求。

**总结建议**：

• **使用 Docker**：
• 需要在多台具有不同操作系统或配置的机器上部署环境。
• 希望确保环境的一致性，避免手动配置带来的问题。
• 项目依赖复杂，包括特定版本的系统库、驱动等。

• **使用 Python 虚拟环境**：
• 目标机器与开发机器环境相似。
• 项目依赖主要是 Python 包，系统级依赖较少或容易安装。
• 团队对 Docker 不熟悉，且不希望增加学习成本。


**额外提示**：
• **GPU 支持**：如果您的项目需要使用 GPU，加速深度学习训练，那么使用 Docker 时需要配置 NVIDIA Docker（现在是 NVIDIA Container Toolkit），以确保容器可以访问主机的 GPU。这可能需要在每台目标机器上进行一些初始设置。
• **环境管理工具**：无论选择哪种方式，都建议使用环境管理工具（如 requirements.txt、conda env 或 Dockerfile）来记录和管理依赖，以便在需要时重现环境。
• **镜像大小**：注意 Docker 镜像可能比较大，尤其是包含了深度学习框架和 CUDA 的镜像。在网络传输和存储时需要考虑这一点。

# docker 安装

## mac安装docker
就用brew安装就可以，但是，有cask和命令行两个。
### 1. **`brew install docker`**

- **安装内容**:  
    仅安装 Docker 的 **命令行工具（CLI）**，不包含 Docker 引擎（守护进程）或图形化界面（Docker Desktop）。
    
- **用途**:  
    适用于以下场景：
    - 你只需要 `docker`、`docker-compose` 等命令（例如连接远程 Docker 主机）。
    - 你已在 macOS 上通过其他方式（如手动安装）配置了 Docker 引擎。
        
- **局限性**:
    - **无法直接使用**：因为 macOS 不是 Linux 系统，缺少原生 Docker 引擎依赖的 Linux 内核特性（如 cgroups、命名空间等）。
    - 如果你直接运行 `docker ps`，会报错 `Cannot connect to the Docker daemon`（因为缺少本地守护进程）。
- **依赖关系**:  
    安装的二进制文件来自 Homebrew 的 **Formula**（纯命令行工具）。

### 2. **`brew install --cask docker`**
- **安装内容**:  
    安装 **Docker Desktop for Mac**（完整的图形化应用），包含：
    - Docker 引擎（守护进程）
    - 命令行工具（CLI）
    - 图形化管理界面
    - 内置的 Linux 虚拟机（用于运行容器）
        
- **用途**:  
    这是 macOS 上运行 Docker 的 **推荐方式**，因为：
    - Docker Desktop 会自动处理 macOS 的依赖（如虚拟化、文件共享、网络配置等）。
    - 开箱即用，无需手动配置守护进程。
- **安装路径**:  
    应用会被安装到 `/Applications/Docker.app`，同时命令行工具通过软链接添加到 `PATH`。
- **依赖关系**:  
    安装的包来自 Homebrew 的 **Cask**（专为 macOS 图形应用设计）。
    
---

## windows安装docker
- 装wsl，在wsl内就跟ubuntu一样的方法。

## ubuntu安装docker：

1. apt安装
```bash
sudo apt install docker.io docker-compose
```
这样安装可能版本老一些，可能存在兼容性问题，如果没有，这样最方便，如果有，就按照下面方法。

2. [官网方式安装](https://docs.docker.com/engine/install/ubuntu/)
```bash
# 1 卸载老版本
for pkg in docker.io docker-doc docker-compose docker-compose-v2 podman-docker containerd runc; do sudo apt-get remove $pkg; done

# 2 加入docker官方源
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update

# 3 安装
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# 用户加入docker组获取权限
sudo groupadd docker
sudo usermod -aG docker $USER

# 试运行
# docker run hello-world
```

#### [docker set proxy](https://docs.docker.com/engine/daemon/proxy/#daemon-configuration)
You may configure proxy behavior for the daemon in the `daemon.json` file, or using CLI flags for the `--http-proxy` or `--https-proxy` flags for the `dockerd` command. Configuration using `daemon.json` is recommended.

```json
{
  "proxies": {
    "http-proxy": "http://proxy.example.com:3128",
    "https-proxy": "https://proxy.example.com:3129",
    "no-proxy": "*.test.example.com,.example.org,127.0.0.0/8"
  }
}
```

After changing the configuration file, restart the daemon for the proxy configuration to take effect:

```bash
 sudo systemctl restart docker
```

The Docker daemon checks the following environment variables in its start-up environment to configure HTTP or HTTPS proxy behavior:

- `HTTP_PROXY`
- `http_proxy`
- `HTTPS_PROXY`
- `https_proxy`

3. 如果没有小飞机，就用用镜像安装docker
```bash
# 卸载docker
sudo apt-get remove docker docker-engine docker.io containerd runc
# 安装依赖
sudo apt update
sudo apt install apt-transport-https ca-certificates curl software-properties-common gnupg lsb-release
# 阿里云key
curl -fsSL http://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | sudo apt-key add -
# 阿里云ppa
sudo add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu $(lsb_release -cs) stable"
# 安装docker
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
# 用户权限
sudo usermod -aG docker $USER
reboot
```

2. docker pull设置镜像
国内大部分docker镜像都失效了，但是可以登陆[阿里云账号](https://cr.console.aliyun.com)[设置镜像](https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors)。
好像阿里也失效了，[github-docker-镜像网站](https://github.com/DaoCloud/public-image-mirror)
```bash
sudo vim /etc/docker/daemon.json
# 然后添加如下内容
{
    "registry-mirrors": ["https://docker.m.daocloud.io"] 
}
# 然后重启该服务
sudo systemctl daemon-reload
sudo systemctl restart docker
```


## 问题

### DNS问题

1. **`/etc/resolv.conf` 的作用**
    - 这个文件是 Linux/macOS 系统中 **DNS 解析的配置文件**，指定了系统使用的 DNS 服务器地址。
    - 默认情况下，系统会使用网络接口（如路由器、ISP）提供的 DNS，但有时这些 DNS 可能不稳定或无法解析某些域名（如 Docker Hub）。
        
2. **`nameserver 8.8.8.8` 和 `nameserver 8.8.4.4` 的作用**
    - `8.8.8.8` 和 `8.8.4.4` 是 **Google 的公共 DNS 服务器**，它们通常比 ISP 提供的 DNS 更稳定、响应更快。
    - 修改后，系统会优先使用 Google DNS 解析域名，从而可能解决 Docker Hub 连接超时的问题。
        
3. **为什么会影响 Docker？**
    - Docker 在拉取镜像时，需要解析 `registry-1.docker.io` 的 IP 地址。
    - 如果默认 DNS 无法正确解析或响应慢，就会导致 `i/o timeout` 错误。
    - 使用更可靠的 DNS（如 `8.8.8.8`）可以避免这个问题。
        
这种修改是否影响全局网络？✅ **是的，会影响整个系统的 DNS 解析**

- 修改 `/etc/resolv.conf` 后，**所有网络请求**（不仅仅是 Docker）都会使用新的 DNS 服务器。
- 但通常不会有负面影响，因为 Google DNS (`8.8.8.8`) 是广泛使用的公共 DNS，稳定性和速度都较好。

⚠️ **注意**：

- 在 macOS 上，`/etc/resolv.conf` 可能是由 `systemd-resolved` 或 `NetworkManager` 动态管理的，手动修改可能会被覆盖。
- 更推荐的方式是通过 **系统设置** 或 **Docker 配置** 修改 DNS，而不是直接编辑 `/etc/resolv.conf`。

---

#### 更好的解决方案（推荐）

#### 1. **直接修改 Docker 的 DNS 配置（不影响系统全局）**

在 Docker 的配置文件 `daemon.json`（通常位于 `/etc/docker/daemon.json` 或 `~/.docker/daemon.json`）中添加自定义 DNS：
```json
{
  "dns": ["8.8.8.8", "8.8.4.4"]
}
```


然后重启 Docker：
```bash
sudo systemctl restart docker  # Linux
# 或者在 macOS 上通过 Docker Desktop 重启
```

#### 2. **在 macOS 上修改全局 DNS（推荐）**

1. 打开 **系统偏好设置 > 网络**。
2. 选择当前网络（如 Wi-Fi），点击 **高级 > DNS**。
3. 添加 `8.8.8.8` 和 `8.8.4.4`，并拖到列表顶部。
4. 点击 **OK** 并应用更改。

#### 为什么会出现默认 DNS 解析失败？
可能的原因：
1. **ISP 的 DNS 服务器不稳定**（如某些国内运营商 DNS 污染或拦截）。
2. **网络代理/VPN 干扰**（如果使用代理，可能需要配置 Docker 使用代理）。
3. **系统 DNS 缓存问题**（可尝试刷新缓存：`sudo dscacheutil -flushcache`（macOS））。