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
  MPI_Type_size(c_info->s_data_type,&s_size);
  MPI_Type_size(c_info->r_data_type,&r_size);

  









}
