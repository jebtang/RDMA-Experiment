### PROCESSOR INFO
```
$ lscpu
CPU(s):                32
On-line CPU(s) list:   0-31
Thread(s) per core:    2
Core(s) per socket:    8
```

<br>

### NICS
```
Mellanox ConnectX-3 VPI FDR 56Gbp/s Single-Port Card
RX bytes:61492508420 (61.4 GB)  TX bytes:61719799448 (61.7 GB)

Ethernet Controller 10-Gigabit X540-AT2
RX bytes:12170200991 (12.1 GB)  TX bytes:12180464404 (12.1 GB)

```

<br>

### SWTICH INFO
```
Mellanox SX6036 SwitchX-2 FDR InfiniBand Managed 36-Port Switch
```

<br>

### MEMORY
```
$ cat /proc/meminfo
MemTotal:       65859688 kB
MemFree:        62710136 kB
MemAvailable:   64589756 kB
Buffers:          303308 kB
Cached:          1363256 kB
SwapCached:            0 kB
Active:          1330700 kB
```

<br>

### KERNEL VERSION
```
$ uname -a
Linux c1n14 4.4.0-96-generic #119-Ubuntu SMP Tue Sep 12 14:59:54 UTC 2017 x86_64 x86_64 x86_64 GNU/Linu
```

<br>

### LINUX VERSION
```
lsb_release -a
Ubuntu 16.04.1 LTS
```

<br>

### 16 Commands to check hardware information on Linux

```
$ lscpu
$ lshw -short // List Hardware
$ hwinfo --short
$ lspci
$ lsscsi
$ lsusb
$ lsblk
$ df -H
$ sudo fdisk -l
$ cat /proc/version
$ cat /proc/partitions
```
