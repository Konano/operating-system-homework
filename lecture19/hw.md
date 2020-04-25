## 问题 1：请执行 FIFO 磁盘调度策略

```
./disksimv3.py   -a 0 => 195
./disksimv3.py   -a 6 => 375
./disksimv3.py   -a 30 => 375
./disksimv3.py   -a 7,30,8 => 795
./disksimv3.py   -a 10,11,12,13,24,1 => 1305
```

请回答每个磁盘请求序列的IO访问时间

## 问题 2：请执行 SSTF 磁盘调度策略

```
./disksimv3.py   -a 10,11,12,13,24,1 -p SSTF => 1305
```

请回答每个磁盘请求序列的IO访问时间

## 问题 3：请执行 SCAN, C-SCAN 磁盘调度策略

```
./disksimv3.py   -a 10,11,12,13,24,1 -p SCAN => 1305
./disksimv3.py   -a 10,11,12,13,24,1 -p CSCAN => 1305
```

请回答每个磁盘请求序列的IO访问时间



SCAN 和 C-SCAN 均已经实现在代码中，可以通过参数 `-p SCAN/CSCAN` 调用。