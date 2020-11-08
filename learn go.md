**Here's an example directory layout:**

  **GOPATH=/home/user/go**

```bash
/home/user/go/

    src/
      foo/
        bar/        (go code in package bar)
          x.go
        quux/       (go code in package main)
          y.go

    bin/
      quux          (installed command)
    pkg/
      linux_amd64/
        foo/
          bar.a     (installed package object)
```

### [go 配置vscode](https://learnku.com/articles/32146)
* 第一步：点击 vscode 提示的 “Install All” 

当把 vscode 下下来，打开一个 go 文件，编辑器会自动告诉你安装 go 插件，如果 go 的插件不齐全，编辑器会提示你 “Analysis Tools Missing”，点击 “Install All”

macOS 使用安装器安装的 go 位置在 /usr/local/go（这是 $GOROOT），但是点击 vscode 默认下载插件的地方是～/go（这是 $GOPATH）. 可以看到第一行是 “Installing 11 tools at /Users/qian/go/bin”

> 有关 $GOROOT 和 $GOPATH，➡️go 环境变量
>
> $GOROOT 表示 Go 在你的电脑上的安装位置，它的值一般都是 $HOME/go，当然，你也可以安装在别的地方。
> $GOPATH 默认采用和 $GOROOT 一样的值，但从 Go 1.1 版本开始，你必须修改为其它路径。它可以包含多个包含 Go 语言源码文件、包文件和可执行文件的路径，而这些路径下又必须分别包含三个规定的目录：src、pkg 和 bin，这三个目录分别用于存放源码文件、包文件和可执行文件。
> 因为墙的原因，vscode 默认下载插件 success 并不容易，11 个插件 2 个 success。
>
> 点进 ~/go 目录，可以看到 vscode 下自动创建了 src/github.com 文件夹，就是下载的插件源代码。
>

* 第二步：在 $GOPATH/src 下新建目录，在 github 上下载文件，执行 go install 命令

好了，接下来是剩下 9 个插件，在 src 下新建 golang.org/x 目录，在 github 上下载 tools、lint、delve、goreturns，其中 tools 和 lint 放在 src/golang.org/x 下面，delve 和 goreturns 放在 src/github.com 下面。
执行命令：

// 这 5 个有 tools 之后，go install 就可以

```bash
go install github.com/mdempsky/gocode
go install github.com/ramya-rao-a/go-outline
go install github.com/acroca/go-symbols
go install golang.org/x/tools/cmd/guru
go install golang.org/x/tools/cmd/gorename
```

// 这 3 个需要在 github 上下载相应插件源代码并放到指定位置，再执行 go install
```bash
go install golang.org/x/lint/golint
go install github.com/go-delve/delve/cmd/dlv
go install github.com/sqs/goreturns
```
// 还有一个 gocode-gomod 插件，该插件源码和 gocode 插件源码名称一致，导致 install 一直失败。
```bash
go install github.com/stamblerre/gocode
```
// 先去 ~/go/bin 目录下，删除 gocode 插件，再执行上面这个，可以看到 ~/go/bin 目录下重新生成了 gocode 可执行文件，将该文件重命名为 gocode-gomod。再执行下面这句，重新生成 gocode。
```bash
go install github.com/mdempsky/gocode
```
// 以上 9 个插件 + vscode 安装的 2 个插件，一共 11 个插件安装完成。重启 vscode，打开 go 文件，已经没有了 “Analysis Tools Missing” 提示。

完成后 ~/go 目录如下，bin 是生成的插件（可执行文件），src 是下载的插件源代码。

其实最好还要将 ~/go/bin 下的可执行文件 复制 到 /usr/local/go/bin 目录下。

