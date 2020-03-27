# 第十讲课后作业

`proc.c`：首先在 `init_main` 函数中调用 `user_main()` 进入用户态进程。在 `user_main` 中，调用 `kernel_execve` 函数执行内核进程，在其中执行 `SYS_exec syscall` 以执行由 `user_main kernel_thread` 调用的用户态程序。

`syscall.c`：在 `sys_exec` 中调用 `proc.c` 中的 `do_execve` 函数。`do_execve` 函数回收当前进程的内存空间，调用 `load_icode` 设置新的内存空间，将用户态程序加载进内存。

`load_icode` 函数：首先为用户态进程创建新的内存空间，然后建立页表，分配内存，最后为了用户态程序创建陷阱帧，切换到用户态。