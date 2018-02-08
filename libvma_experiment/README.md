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
curl ipinfo.io/ip // c1n15  131.94.130.131
scp /Users/hongdavid/Desktop/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64.tgz sungho@131.94.130.131:/home/users/sungho
ssh-copy-id -i id_rsa.pub sungho@131.94.130.131

mlnxofedinstall
/home/users/sungho/MLNX_OFED_LINUX-4.2-1.2.0.0-ubuntu16.04-x86_64/mlnxofedinstall
/home/users/sungho/sysinfo-snapshot-3.2.3/sysinfo-snapshot.py

```

<br>


### installation orders
```
./mlnxofedinstall --vma
./mlnxofedinstall --vma-eth
cat /etc/infiniband/info
cat /etc/modprobe.d/mlnx.conf file.
    // options ib_uverbs disable_raw_qp_enforcement=1
    // options mlx4_core fast_drop=1
    // options mlx4_core log_num_mgm_entry_size=-1
/etc/init.d/openibd restart
ibv_devinfo
# install sockperf




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


### RUNNING SOCKETPERF

- server side
```
// normal tcp
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.31 --port 19140 --tcp

// running with rdma
export VMA_LOAD=/home/users/sungho/libvma/usr/lib64/libvma.so
VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```

- client side

```
// normal tcp
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp

// running with rdma
export VMA_LOAD=/home/users/sungho/libvma/usr/lib64/libvma.so
VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```
