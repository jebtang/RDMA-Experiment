### RDMA_DPDK
1. **[infiniband benchmark implementation](INFINIBAND/imb_benchmark.md)**
   - [test result of ethernet](INFINIBAND/results/imb_result_ethernet)
   - [test result of infiniband](INFINIBAND/results/imb_result_infiniband)
   - [test result of mellanox benchmark](INFINIBAND/results/mellanox_infini_test)

<br>

2. **Infiniband benchmark interpretation**
    - the benchmark repeats itself for the specified duration of time or number of times
```
Bandwidth benchmarks may be run for a number of iterations, or for a fixed duration.
We would like to confirm that the first row does mean that the benchmark has run 2 bytes 1,000 times.
```  
<br>

3. **Proving that IMB connects several nodes**
    - log proves that IMB has attempted to connect with the other node via ssh
    - Similar to MPI the files should also be shared
    - MPI and IMB all assumes that the running file is in the same location as the initiator
```
mpirun -np 2 --hostfile hostfile ./IMB-MPI1 [proxy:0:1@node2] launch_procs (pm/pmiserv/pmip_cb.c:640): unable to change wdir to /home/sungho/mpi-benchmarks/src (No such file or directory)

[proxy:0:1@node2] HYD_pmcd_pmip_control_cmd_cb (pm/pmiserv/pmip_cb.c:892): launch_procs returned error                          
[proxy:0:1@node2] HYDT_dmxu_poll_wait_for_event (tools/demux/demux_poll.c:76): callback returned error status                   
[proxy:0:1@node2] main (pm/pmiserv/pmip.c:206): demux engine error waiting for event            
[mpiexec@node3] control_cb (pm/pmiserv/pmiserv_cb.c:200): assert (!closed) failed               
[mpiexec@node3] HYDT_dmxu_poll_wait_for_event (tools/demux/demux_poll.c:76): callback returned error status                     
[mpiexec@node3] HYD_pmci_wait_for_completion (pm/pmiserv/pmiserv_pmci.c:198): error waiting for event                           
[mpiexec@node3] main (ui/mpich/mpiexec.c:344): process manager error waiting for completion
```

<br>

4. **Command line of IMB**
```
$ mpirun -n <P> IMB-<component> [arguments]

```
| command | description |
|--|--|
| p | number of processes |
| component | type of MPI version test |

5. **Multiple Group Test**
    - the benchmarks run in several process groups.
    - parallel transfer of  benchmarks
