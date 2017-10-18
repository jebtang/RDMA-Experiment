#include "IMB_declare.h"

struct comm_info{

  int 	NP;	// #processes participating in benchmarks

  // MPI_DATAtype is a typedef of int
  MPI_Datatype 	s_data_type;	// data type of sent data
  MPI_Datatype 	r_data_type;	// data type of received data

}

void IMB_pingpong(struct comm_info* c_info,
                  int size,
                  struct iter_schedule* ITERATIONS,
                  MODES RUN_MODE,
                  double* time)
{

  Type_Size s_size,r_size; //Type_Size is an INT

  // get size of the data type
  // in pingpong this is 1
  MPI_Type_size(c_info->s_data_type,&s_size);
  MPI_Type_size(c_info->r_data_type,&r_size);

  if ((s_size!=0) && (r_size!=0))
  {
      s_num=size/s_size;  // divide the message with s_size
      r_num=size/r_size;  // divide the message with r_size
  }


// check whether this variable is equal with the results that are shown in the console result
// and then lets check the SendRecv to see how the time max and time min is implemented
// and then implement the way of getting the latency times for each of the iterations 10000
// and then find the biggest and the smallest latency like the sendRecv

t1 = MPI_Wtime();

for(i=0;i<ITERATIONS->n_sample;i++)
{
  // MPI_Send
  // MPI_Recv
}

t2 = MPI_Wtime();
*time=(t2 - t1)/ITERATIONS->n_sample;


















}
