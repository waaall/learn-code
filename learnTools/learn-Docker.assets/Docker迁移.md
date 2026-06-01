
### 查看信息

```bash
df -hT                        
Filesystem     Type      Size  Used Avail Use% Mounted on
tmpfs          tmpfs      13G  3.9M   13G   1% /run
/dev/sda2      ext4      439G  188G  229G  46% /
tmpfs          tmpfs      63G  8.0K   63G   1% /dev/shm
/dev/sdb5      xfs       3.7T   89G  3.6T   3% /mnt/hdd

sudo du -h --max-depth=1 -x /var 2>/dev/null | sort -hr
123G /var
122G /var/lib
708M /var/cache
...

# docker system prune
# docker system prune -a --volumes(不要用！)

sudo du -h --max-depth=1 -x /var/lib 2>/dev/null | sort -hr
107G /var/lib
101G /var/lib/containerd
4.7G /var/lib/snapd
738M /var/lib/docker
326M /var/lib/apt
83M /var/lib/dpkg
```

---

### 方案 A：迁移 Docker data-root (不建议)

你的 Docker 现在明显不是只用 `/var/lib/docker`，而是把大量实际内容放在 containerd 的默认 root 下面。因此**单独改 containerd root 可能会影响 Docker 和 containerd 的协作**。更推荐先让 Docker 正式使用 HDD 作为数据目录。

目标：

```text
/mnt/hdd/docker
```

执行前先看当前 Docker root：

```bash
docker info | grep -E "Docker Root Dir|Storage Driver"
```

然后停服务：
```bash
sudo systemctl stop docker
sudo systemctl stop containerd
```

复制数据：
```bash
sudo mkdir -p /mnt/hdd/docker
sudo rsync -aHAX --numeric-ids /var/lib/docker/ /mnt/hdd/docker/
```

配置 Docker：
```bash
sudo mkdir -p /etc/docker
sudo nano /etc/docker/daemon.json
```

写入：
```json
{
  "data-root": "/mnt/hdd/docker"
}
```

如果文件里已经有别的配置，就合并进去，例如：
```json
{
  "registry-mirrors": ["https://xxx"],
  "data-root": "/mnt/hdd/docker"
}
```

启动：
```bash
sudo systemctl start containerd
sudo systemctl start docker
```

验证：
```bash
docker info | grep "Docker Root Dir"
docker ps
```

如果看到：
```text
Docker Root Dir: /mnt/hdd/docker
```

就成功了一半。

---

#### **问题**

**`/var/lib/containerd`**

**仍然可能不会变小**

因为你现在大头已经在 `/var/lib/containerd`，而 Docker 只是配置了外部 containerd socket：

```text
dockerd --containerd=/run/containerd/containerd.sock
```

所以 Docker 的 `data-root` 不一定会把现有 `/var/lib/containerd` 全部带走。你更像是 Docker 和系统 containerd 共享了运行时，数据像树根缠在一起。

因此我更建议你用 **bind mount 迁移** **`/var/lib/containerd`** **到 HDD**。这招最直接，也最适合你当前情况。

---

### 方案 B：bind mount 迁移`/var/lib/containerd`

注意不是**改 containerd root 配置**！

#### bind mount 和 ln 区别
```bash
# 符号链接（类似快捷方式）
ln -s /mnt/hdd/containerd /var/lib/containerd
# 缺点：某些程序不认，权限处理不同，Docker 可能拒绝

# Bind mount（更底层、更透明）
mount --bind /mnt/hdd/containerd /var/lib/containerd
# 优点：内核级别重定向，任何程序都无差别对待
```

#### **1. 停 Docker 和 containerd**
```bash
# sudo systemctl stop docker
# sudo systemctl stop containerd

sudo systemctl mask docker.socket
sudo systemctl stop docker.socket
sudo systemctl stop docker.service
sudo systemctl stop containerd.service
```

确认都停了：
```bash
systemctl is-active docker
systemctl is-active containerd
```

应该都不是 `active`。

---

#### **2. 同步 containerd 数据到 HDD**

```bash
sudo mkdir -p /mnt/hdd/containerd
sudo rsync -aHAX --numeric-ids --info=progress2,stats /var/lib/containerd/ /mnt/hdd/containerd/
```

建议再跑一遍，确保停机后数据完全一致：
```bash
# dry-run 不真跑，看看它准备删什么 (也可以直接运行下面的)
sudo rsync -aHAX --numeric-ids --delete --dry-run --itemize-changes /var/lib/containerd/ /mnt/hdd/containerd/
# 确认没有奇怪的大量删除后，再正式跑
sudo rsync -aHAX --numeric-ids --delete --info=progress2,stats /var/lib/containerd/ /mnt/hdd/containerd/
```

---

#### **3. 备份旧目录，创建挂载点**

```bash
sudo mv /var/lib/containerd /var/lib/containerd.bak
sudo mkdir -p /var/lib/containerd
```

---

#### **4. 用 bind mount 挂回原路径**

临时挂载测试：
```bash
sudo mount --bind /mnt/hdd/containerd /var/lib/containerd
```

检查：
```bash
mount | grep containerd
sudo du -h --max-depth=1 -x /var/lib/containerd | sort -hr
```

应该能看到 `/var/lib/containerd` 还是 101G，但实际空间来自 `/mnt/hdd`。

---

#### **5. 启动服务**

```bash
# sudo systemctl start containerd
# sudo systemctl start docker

sudo systemctl unmask docker.socket
sudo systemctl start containerd.service
sudo systemctl start docker.socket
sudo systemctl start docker.service
```

验证：
```bash
docker ps
docker images
docker info | grep -E "Docker Root Dir|Storage Driver"
systemctl status containerd --no-pager
systemctl status docker --no-pager
```

如果容器都起来了，说明迁移成功。

---
#### **6. 重启生效**

`/mnt/hdd/containerd /var/lib/containerd none bind 0 0` 写入 **`/etc/fstab`，保证重启后仍然生效** 。

但！！！这样很难保证启动顺序，当docker 先于挂载启动，就会导致 docker 找不到服务，从而导致docker全部服务失效。 推荐 [后续启动顺序](#后续启动顺序) 的方法。


---

#### **7. 确认没问题后再删除旧数据**

建议先保留 `.bak` 一两天。确认容器正常、重启也正常后：

```bash
sudo rm -rf /var/lib/containerd.bak
```

---

## 后续启动顺序

确认 `/mnt/hdd/containerd /var/lib/containerd` 不在 `/etc/fstab` 中。

### 1. 增加启动限制服务

```bash
sudo vim /etc/systemd/system/var-lib-containerd.mount
```

然后写入

```INI
[Unit]
Description=Bind mount containerd data from HDD
Requires=mnt-hdd.mount
After=mnt-hdd.mount
Before=containerd.service docker.service

[Mount]
What=/mnt/hdd/containerd
Where=/var/lib/containerd
Type=none
Options=bind

[Install]
WantedBy=local-fs.target
```

### 2. 限制 containerd service 启动顺序

```bash
sudo systemctl daemon-reload
sudo systemctl enable var-lib-containerd.mount
```

写入：

```bash
sudo mkdir -p /etc/systemd/system/containerd.service.d

sudo tee /etc/systemd/system/containerd.service.d/override.conf >/dev/null <<'EOF'
[Unit]
Requires=var-lib-containerd.mount
After=var-lib-containerd.mount
EOF
```


### 3. 检查加载设置是否成功

```bash
sudo systemctl daemon-reload
sudo systemctl restart var-lib-containerd.mount
systemctl status var-lib-containerd.mount --no-pager
findmnt /var/lib/containerd
```

### 4. 重启测试

```bash
sudo reboot
findmnt /var/lib/containerd
df -hT /var/lib/containerd
docker images
docker ps -a
```


## 注意事项

### 做之前保存当前容器清单

防止出问题后恢复环境：

```bash
docker ps -a --format 'table {{.Names}}\t{{.Image}}\t{{.Status}}\t{{.Ports}}'
docker images
docker volume ls
docker network ls
```

如果你用 docker compose，也建议找一下 compose 项目：

```bash
docker ps --format '{{.Names}} {{.Label "com.docker.compose.project.working_dir"}}' | sort -u
```

这个命令能帮你定位哪些容器是从哪个 compose 目录启动的。

---

### **关于性能**

可以搬，问题不大，但要知道代价：

| **类型**     | **放 HDD 是否合适** |
| ---------- | -------------- |
| 镜像 blobs   | 合适             |
| 不常改的容器层    | 合适             |
| 普通服务容器     | 通常可以           |
| 数据库 volume | 不建议            |
| 高频构建缓存     | 会变慢            |
| AI 模型、大数据集 | 很适合            |

你的 HDD 是 xfs，作为 containerd/docker 存储没问题。只是机械盘随机读写慢，容器启动、构建镜像、npm install、apt install 这类操作会比 SSD 慢一点。


如果你有数据库容器，后续可以单独把数据库 volume 放 SSD，例如：

```bash
-v /srv/ssd/postgres:/var/lib/postgresql/data
```

让镜像仓库住 HDD，热数据住 SSD，空间和速度两边都不亏。

---

### docker prune 建议

不要用！
```bash
docker system prune -a --volumes
```

它会删掉：
```text
未被容器引用的所有镜像
未使用的 volume
build cache
停止的容器
未使用网络
```

以后建议用：
```bash
docker system prune
```

或者只清构建缓存：
```bash
docker builder prune
```

只清一周前的构建缓存：
```bash
docker builder prune --filter "until=168h"
```

---

### **总结**

```text
SSD /
├── 系统
├── 常用源码
├── 高频数据库 volume
└── 少量关键服务数据

HDD /mnt/hdd
├── containerd
├── docker
├── models
├── datasets
├── backups
└── archives
