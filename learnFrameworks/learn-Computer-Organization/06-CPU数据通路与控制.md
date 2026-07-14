# 中央处理器



## 指令执行过程

![Screen Shot 2021-04-17 at 11.14.01 AM](assets/Screen Shot 2021-04-17 at 11.14.01 AM.png)



### 中断

![Screen Shot 2021-04-17 at 11.13.06 AM](assets/Screen Shot 2021-04-17 at 11.13.06 AM.png)





## 数据通路的结构



![Screen Shot 2021-04-17 at 11.14.35 AM](assets/Screen Shot 2021-04-17 at 11.14.35 AM.png)



### 以内部单总线为例（大题考点）



需要注意，`MDRin` 指的是 MDR 从 CPU 内部总线接收数据的控制信号，并不是内存向 MDR 传送数据的控制信号，所以下图第 3 部分有一句是错误的。

![Screen Shot 2021-04-17 at 4.08.27 PM](assets/Screen Shot 2021-04-17 at 4.08.27 PM.png)



### 专用数据通路方式



![Screen Shot 2021-04-26 at 4.44.22 PM](assets/Screen Shot 2021-04-26 at 4.44.22 PM.png)





## 控制器设计



### 硬布线控制器

微操作是数据通路在控制信号作用下完成的基本操作。微指令只存在于微程序控制器中，用于编码一组微命令；硬布线控制器则直接由组合逻辑和时序逻辑产生控制信号，因此不能把微指令称为所有机器执行过程中的最小指令单位。

![Screen Shot 2021-04-26 at 4.52.36 PM](assets/Screen Shot 2021-04-26 at 4.52.36 PM.png)





分析每个阶段的微操作序列：

![Screen Shot 2021-04-26 at 4.53.54 PM](assets/Screen Shot 2021-04-26 at 4.53.54 PM.png)





### 微程序控制器




