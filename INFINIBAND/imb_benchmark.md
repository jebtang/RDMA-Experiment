
### INSTALLING MPI				
  - [reference youtube video](https://www.youtube.com/watch?v=nmymBKrXlnE&t=9s)

```		
$ sudo wget http://www.mpich.org/static/downloads/3.0.4/mpich-3.0.4.tar.gz			
$ tar xvf mpich			
$ ./configure --prefix=/mirror/mpich2 --disable-f77 --disable-fc			
$ make			
$ make install			

// add the exports in the ./bashrc file			
$ export PATH=/mirror/mpich2/bin:$PATH			
$ export LD_LIBRARY_PATH="/mirror/mpich2/lib:$LD_LIBRARY_PATH"			
$ sudo vi /etc/environment -> add the PATH value above  			
$ mpicc hellow.c -o hello			
$ mpirun -np 4 ./hello			
$ sudo ln -s /usr/bin/make /usr/bin/gmake			
```

<br>

### INSTALLING IMB				
- hosts_example file
```
172.24.30.31
172.24.30.30
```

- run IMB benchmark
```
$ git clone https://github.com/intel/mpi-benchmarks.git			
$ cd src			
$ vi make_ict //change form into -> cc= mpicc			
$ make			
$ ./IMB-MPT1			

// run IMB with two nodes 				
// create hostfile with two INFINIBAND IP addresses			
$ mpirun -np 2 --hostfile hosts_example ./IMB-MPI1			
https://docs.google.com/document/d/1W3AM0wGDInEL5QvOsxNSV5KfVDnakNOa9Y9vXKzqPWw/edit			

// properly use IMB 				
// result evaluation 			
// 4194304 / ( 1.048576 * 35776.65 )			Bytes / Usec / Repetition

```


- Problem with CONNECTING IMB with two nodes 				
  - [reference presentation](http://slideplayer.com/slide/2812715/)
  - [ibm wiki page](https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/IBM%20Platform%20HPC%20Wiki/page/Use%20Intel%20MPI%20benchmark)

```
[SOLVED] The reason for Host Key verification fails			
https://www.slothparadise.com/running-mpi-common-mpi-troubleshooting-problems/			

found out that SSH should work within infinibands 		
the error of failure of logging to SSH produce the same error "Host Key Verification Fails"		

this conversation we can assume that it is possible to use ssh over infiniband		
https://www.linuxquestions.org/questions/linux-server-73/ssh-using-infiband-only-4175425312/		

[SOLVED but UNCLEAR] Permssion Denied error 			
The MPI is attempting to connect SSH for testing 		
If I remove the slave-server's IP address - the error stops 		
mpirun -f hosts_file -n 4 ./cpi		

"The host file contains names of all of the computers on which your MPI job will execute.
For ease of execution, you should be sure that all of these computers have
SSH access, and you should also setup an authorized keys file
to avoid a password prompt for SSH."		

this is the data for establish SSH connection without password (using keys)		
http://www.rebol.com/docs/ssh-auto-login.html		

currently, not clear, but after wenji connected ssh with both of the servers, now it works		
```
