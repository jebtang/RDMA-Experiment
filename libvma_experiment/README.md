### install Mellanox driver with libvma full package
```
# logging into switch
cannot ping to 172.23.10.15

# from mac to wenji-wrk
scp /Users/hongdavid/Desktop/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64.tgz sungho@lab02:/home/sungho/Desktop
scp /Users/hongdavid/Desktop/sysinfo-snapshot-3.2.3.tar sungho@lab02:/home/sungho/Desktop

# from wenji-wrk to vm
scp /home/sungho/Desktop/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64.tgz sungho@vlab01:/home/sungho/Desktop
scp /home/sungho/Desktop/sysinfo-snapshot-3.2.3.tar sungho@vlab01:/home/sungho/Desktop

# fiu c1n15
scp /Users/hongdavid/Desktop/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64.tgz jcabr020@bear.cs.fiu.edu:/disk/bear-b/users/jcabr020
scp /disk/bear-b/users/jcabr020/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64.tgz sungho@172.23.10.15:/home/users/sungho

scp /Users/hongdavid/Desktop/sysinfo-snapshot-3.2.3.tar jcabr020@bear.cs.fiu.edu:/disk/bear-b/users/jcabr020
scp /disk/bear-b/users/jcabr020/sysinfo-snapshot-3.2.3.tar sungho@172.23.10.15:/home/users/sungho
```

<br>


### installation of official package
```
sudo ./mlnxofedinstall --vma
sudo ./mlnxofedinstall --vma-eth
cat /etc/infiniband/info
cat /etc/modprobe.d/mlnx.conf
    // options ib_uverbs disable_raw_qp_enforcement=1
    // options mlx4_core fast_drop=1
    // options mlx4_core log_num_mgm_entry_size=-1
sudo /etc/init.d/openibd restart
ibv_devinfo

sudo apt-get install infiniband-diags
ibstat

// install sockperf
./autogen.sh
./configure prefix=/home/users/sungho/sockperf/usr
make
make install
```



<br>

### Libvma installing from GIT

```
// install  /home/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/install.pl
/home/sungho/MLNX_OFED_SRC-4.2-1.2.0.0
sudo ./commonl.pl            /home/users/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/commonl.pl
sudo ./install.pl --vma      /home/users/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/install.pl

// install vma library
git clone https://github.com/Mellanox/libvma.git
cd libvma
./autogen.sh

./configure --prefix=/home/users/sungho/libvma/usr  --libdir=/home/users/sungho/libvma/usr/lib64 --sysconfdir=/home/users/sungho/libvma/etc


./configure --with-ofed=/home/users/sungho/libvma/usr --prefix=/home/users/sungho/libvma/usr --libdir=/home/users/sungho/libvma/usr/lib64 --includedir=/home/users/sungho/libvma/usr/include/mellanox --docdir=/home/users/sungho/libvma/usr/share/doc/libvma --sysconfdir=/home/users/sungho/libvma/etc

make
make -j 4
make install
sudo ldconfig  /sbin/ldconfig

// run vma /home/users/sungho/libvma/etc/init.d  
sudo ./vma start

export LD_PRELOAD=/home/users/sungho/libvma/usr/lib64/libvma.so

// install socketperf
./autogen.sh
./configure prefix=/home/users/sungho/sockperf/usr
make
make install                              
```

<br>

### results from socketperf
  - [testing in clusters](test_clusters.md)

```
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf sr --msg-size 64 --ip 172.24.30.31 --port 19142 --tcp

VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ul --time 4 --msg-size 64 --ip 172.24.30.31 --port 19142 --tcp


# infiniband used with userspace TCP stack
LD_PRELOAD=libvma.so sockperf server -i 172.24.30.30 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf server -i 172.24.30.30 --tcp


VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 2 -i 172.24.30.31 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 4 -i 172.24.30.31 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 8 -i 172.24.30.31 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 16 -i 172.24.30.31 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 32 -i 172.24.30.31 --tcp
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 sockperf ping-pong -t 64 -i 172.24.30.31 --tcp


sockperf: ---> <MAX> observation =  180.783
sockperf: ---> percentile 99.999 =   56.069
sockperf: ---> percentile 99.990 =    7.196
sockperf: ---> percentile 99.900 =    2.624
sockperf: ---> percentile 99.000 =    2.165
sockperf: ---> percentile 90.000 =    1.609
sockperf: ---> percentile 75.000 =    1.446
sockperf: ---> percentile 50.000 =    1.394
sockperf: ---> percentile 25.000 =    1.372
sockperf: ---> <MIN> observation =    1.290


# infiniband used with native TCP stack
LD_PRELOAD=
sockperf server -i 172.24.30.31
sockperf ping-pong -t 4 -i 172.24.30.31

sockperf: ---> <MAX> observation =   54.169
sockperf: ---> percentile 99.999 =   42.183
sockperf: ---> percentile 99.990 =   29.072
sockperf: ---> percentile 99.900 =   17.618
sockperf: ---> percentile 99.000 =   14.113
sockperf: ---> percentile 90.000 =   12.961
sockperf: ---> percentile 75.000 =   11.963
sockperf: ---> percentile 50.000 =   10.830
sockperf: ---> percentile 25.000 =    9.937
sockperf: ---> <MIN> observation =    7.803      
```
