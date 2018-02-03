### Libvma

```
// install  /home/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/install.pl
/home/sungho/MLNX_OFED_SRC-4.2-1.2.0.0
sudo ./commonl.pl            /home/users/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/commonl.pl
sudo ./install.pl --vma      /home/users/sungho/MLNX_OFED_SRC-4.2-1.2.0.0/install.pl

// install vma library
git clone https://github.com/Mellanox/libvma.git
cd libvma
./autogen.sh
./configure --with-ofed=/home/users/sungho/libvma/usr --prefix=/home/users/sungho/libvma/usr --libdir=/home/users/sungho/libvma/usr/lib64 --includedir=/home/users/sungho/libvma/usr/include/mellanox --docdir=/home/users/sungho/libvma/usr/share/doc/libvma --sysconfdir=/home/users/sungho/libvma/etc

make
make -j 4
make install
sudo ldconfig  /sbin/ldconfig

// run vma /home/users/sungho/libvma/etc/init.d  
sudo ./vma start

export LD_PRELOAD=/home/sungho/libvma/usr/lib64/libvma.so

// install socketperf
./autogen.sh
./configure prefix=/home/sungho/libvma/usr
make
make install

socketxtreme_pol
```

<br>


### RUNNING SOCKETPERF

- server side
```
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.31 --port 19140 --tcp

VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```

- client side

```
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp

VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```
