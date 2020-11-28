# Computer System

[toc]

---

废话不多说，先提几个正式系统性学习操作系统之前困扰我的问题：

1. 编译器是在操作系统之前被发明，那没有操作系统，现代编译器还能用吗？
2. 用户所谓的应用程序也都是用像编译操作系统一样的编译器编译而成的机械指令，为何执行会有区别（比如应用程序权限低等），换句话说：操作系统是什么实现的权限管理（在不调用系统API时）？
3. 虚拟内存也就是内存映射，能实现对操作系统的隔离保护，但是如何**让/不让**“用户/软件”访问硬件？（物理硬盘，物理内存，寄存器）
4. 

[一个大致能回答上述一些问题的答案](https://www.zhihu.com/question/306127044/answer/555327651)。

---

## 参考书籍

* [Linux内核设计与实现](https://book.douban.com/subject/6097773/)
* [现代操作系统](https://book.douban.com/subject/27096665/)
* [深入理解计算机系统](https://book.douban.com/subject/26912767/)

---



## 绪论

这里肯定是随便简单的说一说。

### 内核态和用户态

> 节选自Linux内核设计与实现：
>
> 用户界面是操作系统的外在表象，内核才是操作系统的内在核心系统其他部分必须依靠内核这部分软件提供的服务，像管理硬件设备、分配系统资源等。内核有时候被称作是管理者或者是操作系统核心。通常一个内核由负责响应中断的中断服务程序，负责管理多个进程从而分享处理器时间的调度程序，负责管理进程地址空间的内存管理程序和网络、进程间通信等系统服务程序共同组成。对于提供保护机制的现代系统来说，内核独立于普通应用程序，它一般处于系统态，拥有受保护的内存空间和访问硬件设备的所有权限。这种系统态和被保护起来的内存空间，统称为内核空间。相对的，应用程序在用户空间执行。它们只能看到允许它们使用的部分系统资源，并且只使用某些特定的系统功能，不能直接访问硬件，也不能访问内核划给别人的内存范围，还有其他一些使用限制。当内核运行的时候，系统以内核态进入内核空间执行。而执行一个普通用户程序时，系统将以用户态进入以用户空间执行。
> 在系统中运行的应用程序通过系统调用来与内核通信（见图1-1）。应用程序通常调用库函数（比如C库函数）再由库函数通过系统调用界面，让内核代其完成各种不同任务。一些库调用提供了系统调用不具备的许多功能，在那些较为复杂的函数中，调用内核的操作通常只是整个工作的一个步骤而已。举个例子，拿 printfO函数来说，它提供了数据的缓存和格式化等操作，而调用 write函数将数据写到控制台上只不过是其中的一个动作罢了。不过，也有一些库函数和系统调用就是一一对应的关系，比如， open库函数除了调用 open系统调用之外，几乎什么也不做。还有一些c库函数，像strcpy根本就不需要直接调用系统级的操作。当一个应用程序执行一条系统调用，我们说内核正在代其执行。如果进一步解释，在这种情况下，应用程序被称为通过系统调用在内核空间运行，而内核被称为运行于进程上下文中。这种交互关系——应用程序通过系统调用界面陷入内核——是应用程序完成其工作的基本行为方式。

**值得注意的是，这与我们平常所谓的用户模式和管理员模式是完全两码事！**



### MMU与虚拟内存管理

> 由于所有的 Unix 内核都同宗同源，并且提供相同的 API，现代的 Unix 内核存在许多设计上的相似之处（请看参考目录中我所推荐的关于传统 Unix 内核设计的相关书籍）。Unix 内核几乎毫无例外的都是一个不可分割的静态可执行库。也就是说，它们必须以巨大、单独的可执行块的形式在一个单独的地址空间中运行。Uni 内核通常要硬件系统提供页机制（MMU）以管理内存。这种页机制可以加强对内存空间的保护，并保证每个进程都可以运行于不同的虚地址空间上。初期的 Linux 系统也需要 MMU 支持，但有一些特殊版本不依赖于此。这无疑是一个简洁的设计，因为它可以使 Lin 系统运行在没有 MMU 的小型嵌入系统上。不过现实之中，即便很简单的嵌入系统都开始具备内存管理单元这种高级功能了。



## 进程管理

### 任务队列（task list）

Task List 是一个双向循环链表（**Link List**），储存在内存某个位置，其中每一项都是个结构（**task_struct**），称为进程描述符。该结构定义在`<Linux/sched.h>`中，该结构描述了该进程打开的文件、所在的内存地址空间、挂起的信号、进程的状态等。

```c
struct task_struct {
 volatile long state; /* -1 unrunnable, 0 runnable, >0 stopped */
 void *stack;
 atomic_t usage;
 unsigned int flags; /* per process flags, defined below */
 unsigned int ptrace;
 int lock_depth; /* BKL lock depth */ 
 /* ...... */ 
};
```
Linux使用slab分配`task_struct`，其在该进程内核栈底端创建一个`thread_info`的struct，其存放着`task_struct`的地址偏移量。


> 上文 `task_struct` 中有一个 `stack` 成员，而 `stack` 正好用于保存内核栈地址。内核栈在进程创建时绑定在 `stack` 上。可以观察 `fork` 流程：Linux 通过 `clone()` 系统调用实现 `fork()`，然后由 `fork()` 去调用 `do_fork()`。定义在<kernel/fork.c>中的 `do_fork()` 负责完成进程创建的大部分工作，它通过调用 `copy_process()` 函数，然后让进程运行起来。`copy_process()` 完成了许多工作，这里重点看内核栈相关部分。`copy_process()` 调用 `dup_task_struct` 来创建内核栈、`thread_info` 和 `task_struct`：

```c
static struct task_struct *dup_task_struct(struct task_struct *orig) { 
 struct task_struct *tsk;
 struct thread_info *ti;
 unsigned long *stackend;
 int err; prepare_to_copy(orig);
 tsk = alloc_task_struct();
 if (!tsk) return NULL;
 ti = alloc_thread_info(tsk); 
 if (!ti) { 
  free_task_struct(tsk);
  return NULL; 
 } 
 err = arch_dup_task_struct(tsk, orig);
 if (err) goto out;
 tsk->stack = ti;
 err = prop_local_init_single(&tsk->dirties);
 if (err) goto out;
 setup_thread_stack(tsk, orig);
 stackend = end_of_stack(tsk);
 *stackend = STACK_END_MAGIC;
 /* for overflow detection */
 #ifdef CONFIG_CC_STACKPROTECTOR 
 tsk->stack_canary = get_random_int();
 #endif 
 /* One for us, one for whoever does the "release_task()" 
 (usually parent) */
 atomic_set(&tsk->usage,2);
 atomic_set(&tsk->fs_excl, 0);
 #ifdef CONFIG_BLK_DEV_IO_TRACE
 tsk->btrace_seq = 0;
 #endif 
 tsk->splice_pipe = NULL;
 account_kernel_stack(ti, 1);
 return tsk;
out:
 free_thread_info(ti);
 free_task_struct(tsk);
 return NULL; 
}
```





## 系统调用





## 内存管理





## 文件系统



