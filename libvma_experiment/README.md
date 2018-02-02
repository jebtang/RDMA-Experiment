### Libvma

```
```





<br>


### RUNNING SOCKETPERF

- server side
```
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp

VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf sr --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```

- client side

```
numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp

VMA_SPEC=latency LD_PRELOAD=$VMA_LOAD numactl --cpunodebind=1 taskset -c 19,13 ./sockperf pp --time 4 --msg-size 14 --ip 172.24.30.30 --port 19140 --tcp
```
