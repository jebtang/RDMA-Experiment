### Definition of MPICH
- Standard for writing message passing process
- MPICH is made out of functions to support MPI standard

<br>

### MPI-3 Proper Install
- download mpich
```
wget http://www.mpich.org/static/downloads/3.2/mpich-3.2.tar.gz
tar -xzvf mpich-3.2.tar.gz
```

- create a directory for installing MPI
```
$ mkdir /tmp/you/mpich-3.2
```

- install gfortran which is needed for mpich
```
apt-get install gfortran

```

- install mpi
```
$ cd  /tmp/you/mpich-3.2

// |& tee c.txt means all the logs made from install are saved in the c.txt
$ ./configure --prefix=/home/you/mpich-install |& tee c.txt

$ make

$ make install
```

- export Path
```
/etc/profile (for all users)
~/.bash_profile (for actual user)
~/.bash_login (for actual user)
~/.profile (for actual user)

// add this to the bottom of the file
// .bash_profile is a script that is executed each time you start a new shell
$ echo 'export PATH=/usr/local/bin:$PATH' >>~/.bash_profile
export PATH=/home/you/mpich-install/bin:$PATH
```

<br>

### connecting between the hosts
- MPICH2 (or at least the Hydra process manager) needs to be installed on both machines.
- application (hellow) needs to be accessible from both machines (e.g., on a shared filesystem or by copying the files).

```
Process 3 of 5 is on node3      
Process 1 of 5 is on node3      
Process 4 of 5 is on node2      
Process 0 of 5 is on node2      
Process 2 of 5 is on node2
```
