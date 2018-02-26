# ASU - POSIX native

```
wget -qO- http://10.107.30.33:8888 | sed -e 's/<[^>]*>//g'

# latency-experiment
./persocket

./wrk -t2 -c2 -d10s http://10.107.30.33:8888 >> output
./wrk -t4 -c4 -d10s http://10.107.30.33:8888
./wrk -t8 -c8 -d10s http://10.107.30.33:8888
./wrk -t16 -c16 -d10s http://10.107.30.33:8888
./wrk -t32 -c32 -d10s http://10.107.30.33:8888
./wrk -t64 -c64 -d10s http://10.107.30.33:8888
```

# ASU - seastar POSIX native stack

```
wget -qO- http://10.107.30.33:10000 | sed -e 's/<[^>]*>//g'

./tcp_server --smp 2
./wrk -t2 -c2 -d10s http://10.107.30.33:10000

./tcp_server --smp 4
./wrk -t4 -c4 -d10s http://10.107.30.33:10000

./tcp_server --smp 8
./wrk -t8 -c8 -d10s http://10.107.30.33:10000

./tcp_server --smp 16
./wrk -t16 -c16 -d10s http://10.107.30.33:10000

./tcp_server --smp 32
./wrk -t32 -c32 -d10s http://10.107.30.33:10000

./tcp_server --smp 64
./wrk -t64 -c64 -d10s http://10.107.30.33:10000
```


# ASU - seastar DPDK userspace stack

```
wget -qO- http://10.107.30.40:10000 | sed -e 's/<[^>]*>//g'


sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 2
./wrk -t2 -c2 -d10s http://10.107.30.40:10000


sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 4
./wrk -t4 -c4 -d10s http://10.107.30.40:10000


sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 8
./wrk -t8 -c8 -d10s http://10.107.30.40:10000

sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 16
./wrk -t16 -c16 -d10s http://10.107.30.40:10000

sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 16
./wrk -t32 -c32 -d10s http://10.107.30.40:10000


sudo ./server --dpdk-pmd --dpdk-port-index 1 --network-stack native --dhcp 0 --host-ipv4-addr 10.107.30.40 --netmask-ipv4-addr 255.255.254.0 --gw-ipv4-addr 10.107.30.1 --collectd 0 --smp 16
./wrk -t64 -c64 -d10s http://10.107.30.40:10000
```




# ASU - seastar FIU userspace stack non-polling
```
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19  ./persocket
./wrk -t2 -c2 -d10s http://172.24.30.30:8888
./wrk -t4 -c4 -d10s http://172.24.30.30:8888
./wrk -t8 -c8 -d10s http://172.24.30.30:8888
./wrk -t16 -c16 -d10s http://172.24.30.30:8888
./wrk -t32 -c32 -d10s http://172.24.30.30:8888


```


# ASU - seastar FIU userspace stack polling

```
wget -qO- http://172.24.30.30:10000 | sed -e 's/<[^>]*>//g'

LD_PRELOAD=libvma.so  ./tcp_server --smp 2
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 0-2  ./tcp_server --smp 2
./wrk -t2 -c2 -d10s http://172.24.30.30:10000

LD_PRELOAD=libvma.so ./tcp_server --smp 4
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 0-4 ./tcp_server --smp 4
./wrk -t4 -c4 -d10s http://172.24.30.30:10000

LD_PRELOAD=libvma.so ./tcp_server --smp 8
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 0-8  ./tcp_server --smp 8
./wrk -t8 -c8 -d10s http://172.24.30.30:10000

LD_PRELOAD=libvma.so ./tcp_server --smp 16
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 0-16 ./tcp_server --smp 16
./wrk -t16 -c16 -d10s http://172.24.30.30:10000

LD_PRELOAD=libvma.so ./tcp_server --smp 32
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 ./tcp_server --smp 32
./wrk -t32 -c32 -d10s http://172.24.30.30:10000

LD_PRELOAD=libvma.so ./tcp_server --smp 64
VMA_SPEC=latency LD_PRELOAD=libvma.so numactl --cpunodebind=1 taskset -c 17,21,19 ./tcp_server --smp 64
./wrk -t64 -c64 -d10s http://172.24.30.30:10000

- the easiest way is to change into ethernet but need to be sure whether this makes a difference.

c1n14
connectx_port_config
```
