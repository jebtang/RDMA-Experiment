### INSTALLING IMB				

- link make to gmake  
```
$ sudo ln -s /usr/bin/make /usr/bin/gmake
```

- install IMB benchmark
```
$ git clone https://github.com/intel/mpi-benchmarks.git			
$ cd src			
$ vi make_ict //change form into -> cc= mpicc			
$ make			
$ ./IMB-MPT1			
```

- example of hosts_example file
```
172.24.30.31
172.24.30.30
```

- run IMB with two nodes 	
```			
// create hostfile with two INFINIBAND IP addresses			
$ mpirun -np 2 --hostfile hosts_example ./IMB-MPI1			
```
