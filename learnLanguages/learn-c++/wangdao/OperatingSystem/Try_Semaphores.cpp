// 信号量(semaphores)
struct semaphore
{
    int value;          //剩余资源数
    struct prosess *L;  //等待队列
};

//进程进入缓冲区前的wait操作，这是“原子操作”，也称原语，执行此函数时，操作系统会暂时屏蔽所有中断
void wait(semaphore & S){
    S.value--;
    if (S.value < 0){
        block(S.L) //若资源“忙碌”，主动阻塞自己（加入等待队列）
    }
}

//进程使用完资源后，通过signal原语释放
void signal(semaphore & S){
    S.value++;
    if (S.value <= 0){
        wakeup(S.L); //若释放资源后。等待队列还有进程，则唤醒它
    }
}

//信号量机制实现进程同步（保证进程间的先后顺序），需要设置“同步”信号量S，初始为0
semaphore * S;

P1(){
    代码1;
    V(S);
    代码3;
}

P2(){
    P(S);
    代码4;
    代码5;
}


//如果不需要信号量的计数能力，那么可以使用信号量的一个简化版本---互斥量（mutex）


