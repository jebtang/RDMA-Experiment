### Read / Write Throughput

- **parameters**
  - defined in `main.c` `run-servers.sh` and `run-machine.sh`
  - `num_server_threads` - number of worker threads at server machine
  - `num_threads` - number of client threads at each client machine

<br>

- **Optimizations**
  - `postlist` - Number of SENDs issued by a server using one Doorbell
  - `use_uc` - Use UC transport for RDMA writes
  - `NUM_QPS` - Number of QPs used by each server thread
  - `UNSIG_BATCH` - One post_send() per UNSIG_BATCH reads/writes is signaled

<br>

- **running the benchmark**
  - Change `HRD_REGISTRY_IP` in both `run-servers.sh` and `run-machine.sh` to the server's IP address.
  - At the server machine: ./run-servers.sh
  - At client machine i of num_server_threads / num_threads client machines: ./run-machine.sh i
