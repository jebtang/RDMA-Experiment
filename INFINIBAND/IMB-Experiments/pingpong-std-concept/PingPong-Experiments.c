// starting from IMB.c
int main(){

  /* LOOP OVER INDIVIDUAL BENCHMARKS */
  while( (p=BList[j].name) ){

    /* LOOP OVER PROCESS NUMBERS */
    while ( do_it ){

      /* LOOP OVER MESSAGE LENGTHS */
      while ( ((C_INFO.n_lens == 0 && size < MAXMSG ) ||
               (C_INFO.n_lens > 0  && iter < C_INFO.n_lens))
              && (Bmark->sample_failure != SAMPLE_FAILED_TIME_OUT) )
      {

        IMB_init_buffers_iter(&C_INFO, &ITERATIONS, Bmark, BMODE, iter, size); // probabl add pingpong here
        MPI_Barrier(MPI_COMM_WORLD);



        // runs the number of diffferent length of message and each number of processes
        // pointer function directing to the benchmark
        Bmark->Benchmark(&C_INFO,size,&ITERATIONS,BMODE,time);
        /*
            void (*Benchmark)(struct comm_info* c_info,int size, struct iter_schedule* ITERATIONS,MODES RUN_MODE,double* time);
            void IMB_pingpong(struct comm_info* c_info,int size, struct iter_schedule* ITERATIONS, MODES RUN_MODE, double* time )
        */


        // IMB_ouput.c
        IMB_output   (&C_INFO,Bmark,BMODE,header,size,&ITERATIONS,time); // where the print out is made
        IMB_close_transfer(&C_INFO, Bmark, size);

      }
    }
  }
}


// the function is called the number of fixed message sizes
void IMB_pingpong(struct comm_info* c_info,
                  int size, // fixed vary of message size
                  struct iter_schedule* ITERATIONS, // fixed number of times for running the benchmark
                  MODES RUN_MODE,
                  double* time ) // time for calculating the average time
{

  Type_Size s_size,r_size; //Type_Size is an INT

  // get size of the data type
  // in pingpong this is 1
  MPI_Type_size(c_info->s_data_type,&s_size);
  MPI_Type_size(c_info->r_data_type,&r_size);


  // size : varing number of message sizes [0 ~ 1024 etc]
  if ((s_size!=0) && (r_size!=0)){
      s_num=size/s_size;  // divide the message with s_size
      r_num=size/r_size;  // divide the message with r_size
  }

// check whether this variable is equal with the results that are shown in the console result
// and then lets check the SendRecv to see how the time max and time min is implemented
// and then implement the way of getting the latency times for each of the iterations 10000
// and then find the biggest and the smallest latency like the sendRecv

  if (c_info->rank == c_info->pair0){ // checking for sender

      t1 = MPI_Wtime();

      for(i=0;i<ITERATIONS->n_sample;i++){
        // MPI_Send
        // MPI_Recv
        // other error checking
      }

      t2 = MPI_Wtime();
      *time=(t2 - t1)/ITERATIONS->n_sample;

  } else if (c_info->rank == c_info->pair1){ // checking for receiver

      t1 = MPI_Wtime();
  	   for(i=0;i<ITERATIONS->n_sample;i++){
         // MPI_Recv
         // MPI_Send
         // other error checking
       }

      t2 = MPI_Wtime();
      *time=(t2 - t1)/ITERATIONS->n_sample;
  }

  // data printed each of the message size has finished the operation
}


// IMB_output.c
void IMB_output(struct comm_info* c_info, struct Bench* Bmark, MODES BMODE,
                int header, int size, struct iter_schedule* ITERATIONS,
                double *time){

                IMB_print_header (out_format, Bmark, c_info, BMODE);
                IMB_display_times(Bmark, all_times, c_info, i_gr, ITERATIONS->n_sample, size, out_format);

                }

void IMB_display_times(struct Bench* Bmark, double* tlist, struct comm_info* c_info,
        int group, int n_sample, int size, int out_format){

          IMB_calculate_times(Bmark->Ntimes, c_info, group, tlist, timing);


          if (timing[MAX].times[PURE] > 0.)
          {
              if (Bmark->RUN_MODES[0].type != ParallelTransferMsgRate)
                  throughput = (Bmark->scale_bw * SCALE * MEGA) * size / timing[MAX].times[PURE];
          }

          // pingpong printout
          sprintf(aux_string + offset, format, size, n_sample, timing[MAX].times[PURE], throughput);
        }



void IMB_calculate_times(int ntimes, struct comm_info* c_info, int group_id, double* tlist, Timing* timing, double* defect){

  // for pingpong this the PURE is 0 and ntimes is 1
  for (time_id = PURE; time_id < ntimes; time_id++){

      // n count does not reflect the number of repetition in pingpong
      for (i = 0; i < ncount; i++){

          // if there is a failed data just skip
          if (tlist[offset] < 0.) {
              continue;
          }
          times_count++;

          // assign the smallest time to MIN
          if (tlist[offset] < timing[MIN].times[time_id]) {
              timing[MIN].times[time_id] = tlist[offset];
              timing[MIN].offset[time_id] = rank;
          }

          // assign the largest time to MAX
          if ((tlist[offset] > timing[MAX].times[time_id])) {
              timing[MAX].times[time_id] = tlist[offset];
              timing[MAX].offset[time_id] = rank;
          }

          // add all the time in the average
          timing[AVG].times[time_id] += tlist[offset];
      }

      // get the average value of the time
      timing[AVG].times[time_id] /= times_count;
  }
}
