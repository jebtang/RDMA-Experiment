### MPI-3 Proper Install
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
