# learn [git](https://git-scm.com/book/zh/v2)
[toc]

- - - -
## 版本控制系统

## git 框架


## git命令的区别
1.[git clone/git pull/git fetch/git merge/get init 的区别](https://www.cnblogs.com/lalalagq/p/9968949.html)

## 常用指令

[git 讲解](https://www.jianshu.com/p/e57a4a2cf077)

```bash
# 指令
git config --global http.proxy socks5://127.0.0.1:1086 #设置代理
git config --global --unset http.proxy	#取消代理

#windows选择sublime为默认编辑器
git config --global core.editor "C:/Program Files/Sublime Text 2/sublime_text.exe' -n -w"

#配置账号
git config --global user.name "waaall"
git config --global user.email "wallphysics@gmail.com"
git config --system --list  #看git系统信息
git config --global  --list #看当前用户信息
git config --local  --list  #看当前仓库信息

# Zsh Alias	Command
g	    git
ga	    git add
gaa	git add --all
gapa	git add --patch
gb	    git branch
gba	git branch -a
gbl	git blame -b -w
gbnm	git branch --no-merged
gbr	git branch --remote
gbs	git bisect
gbsb	git bisect bad
gbsg	git bisect good
gbsr	git bisect reset
gbss	git bisect start
gc	    git commit -v
gc!	git commit -v --amend
gca	git commit -v -a
gcam	git commit -a -m #-a表示all，等同于把所有文件先add再commit
gca!	git commit -v -a --amend
gcan!	git commit -v -a -s --no-edit --amend
gcb	git checkout -b
gcf	git config --list
gcl	git clone --recursive
gclean	git clean -df
gcm	git checkout master
gcd	git checkout develop
gcmsg	git commit -m
gco	git checkout
gcount	git shortlog -sn
gcp	git cherry-pick
gcpa	git cherry-pick --abort
gcpc	git cherry-pick --continue
gcs	git commit -S
gd	git diff
gdca	git diff --cached
gdt	git diff-tree --no-commit-id --name-only -r
gdw	git diff --word-diff
gf	git fetch
gfa	git fetch --all --prune
gfo	git fetch origin
gg	git gui citool
gga	git gui citool --amend
ghh	git help
ggpull	ggl
ggpur	ggu
ggpush	ggp
ggsup	git branch --set-upstream-to = origin/$(current_branch)
gpsup	git push --set-upstream origin $(current_branch)
gignore	git update-index --assume-unchanged
git-svn-dcommit-push	git svn dcommit && git push github master:svntrunk
gk	\gitk --all --branches
```