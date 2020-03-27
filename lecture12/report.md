# 第 12 讲书面作业

> 2017011474 郑林楷 & 2016010103 佟禹畅

在多处理机调度模拟器框架的基础上，实现BFS调度算法。

首先对框架进行了部分改动，`Job` 类増加了 `prior_atio`（优先级）和 `deadline`（虚拟截止时间）共两个参数。

新建了一个 `BFS` 类，其中 `flags` 为标志位，`lists` 为每个优先级所对应的任务列表（这里为了方便实现直接采用了 Python 内置的 List）

```python
class BFS:
    def __init__(self):
        self.flags = 0
        self.lists = []
        for _ in range(103):
            self.lists.append([])

    def append(self, job_name, prior_atio):
        self.flags |= 2 ** prior_atio
        self.lists[prior_atio].append(job_name)

    def remove(self, job_name, prior_atio):
        self.lists[prior_atio].remove(job_name)
        if len(self.lists[prior_atio]) == 0:
            self.flags -= 2 ** prior_atio

    def find_lsb(self):
        if self.flags == 0:
            return -1
        return int(math.log(self.flags & -self.flags) / math.log(2))
```

对原框架中的 `get_job` 进行改动，改成多个 CPU 共享一排调度队列。首先查看 `flags` 是否有置位的比特，如果有多个则取最低位。选定了相应的 bit 位之后，便需要遍历其相应的子队列。假如是一个 Realtime 进程的子队列，则选取其中的第一个进程；如果是其他的队列，那么就采用 EEVDF(Earliest Eligible Virtual Deadline First) 算法来选取合适的进程，即比较队列中的每一个进程的 Virtual Deadline 值，并找到最小的那个，倘若在过程中发现某个进程的 Virtual Deadline 小于当前系统时间，则直接选中该进程并跳出遍历。
