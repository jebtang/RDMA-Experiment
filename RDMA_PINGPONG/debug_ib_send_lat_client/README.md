```c
/* init default values to user's parameters */
memset(&ctx,		0, sizeof(struct pingpong_context));
memset(&user_param, 0, sizeof(struct perftest_parameters));
memset(&user_comm , 0, sizeof(struct perftest_comm));
memset(&mcg_params, 0, sizeof(struct mcast_parameters));

user_param.verb    = SEND;
user_param.tst     = LAT;
strncpy(user_param.version, VERSION, sizeof(user_param.version));
user_param.r_flag  = &report;

/* Configure the parameters values according to user arguments or defalut values. */
ret_val = parser(&user_param,argv,argc);
if (ret_val) {
  if (ret_val != VERSION_EXIT && ret_val != HELP_EXIT)
    fprintf(stderr," Parser function exited with Error\n");
  return FAILURE;
}

/* Finding the IB device selected (or defalut if no selected). */
ib_dev = ctx_find_dev(user_param.ib_devname);
if (!ib_dev) {
  fprintf(stderr," Unable to find the Infiniband/RoCE device\n");
  return FAILURE;
}

/* Getting the relevant context from the device */
ctx.context = ibv_open_device(ib_dev);
if (!ctx.context) {
  fprintf(stderr, " Couldn't get context for the device\n");
  return FAILURE;
}

/* Initialize the connection and print the local data. */
if (establish_connection(&user_comm)) {
  fprintf(stderr," Unable to init the socket connection\n");
  return FAILURE;
}

exchange_versions(&user_comm, &user_param);
check_sys_data(&user_comm, &user_param);

/* See if MTU and link type are valid and supported. */
if (check_mtu(ctx.context,&user_param, &user_comm)) {
  fprintf(stderr, " Couldn't get context for the device\n");
  return FAILURE;
}


// actual signal comes from here
ALLOCATE(my_dest , struct pingpong_dest , user_param.num_of_qps);
memset(my_dest, 0, sizeof(struct pingpong_dest)*user_param.num_of_qps);
ALLOCATE(rem_dest , struct pingpong_dest , user_param.num_of_qps);
memset(rem_dest, 0, sizeof(struct pingpong_dest)*user_param.num_of_qps);

/* create all the basic IB resources (data buffer, PD, MR, CQ and events channel) */
if (ctx_init(&ctx,&user_param)) {
  fprintf(stderr, " Couldn't create IB resources\n");
  return FAILURE;
}


/* Set up the Connection. */
if (send_set_up_connection(&ctx,&user_param,my_dest,&mcg_params,&user_comm)) {
  fprintf(stderr," Unable to set up socket connection\n");
  return FAILURE;
}

/* Print basic test information. */
ctx_print_test_info(&user_param);


for (i=0; i < user_param.num_of_qps; i++)
      ctx_print_pingpong_data(&my_dest[i],&user_comm);  // local address: LID 0x01 QPN 0x024d PSN 0x25172


user_comm.rdma_params->side = REMOTE;


for (i=0; i < user_param.num_of_qps; i++) {   
      // shaking hands and gather the other side info.
      if (ctx_hand_shake(&user_comm,&my_dest[i],&rem_dest[i])) {
      fprintf(stderr,"Failed to exchange data between server and clients\n");
      return FAILURE;
      }
      ctx_print_pingpong_data(&rem_dest[i],&user_comm); // remote address: LID 0x01 QPN 0x024d PSN 0x251726
}


if (user_param.work_rdma_cm == OFF) {
  if (ctx_check_gid_compatibility(&my_dest[0], &rem_dest[0])) {
    fprintf(stderr,"\n Found Incompatibility issue with GID types.\n");
    fprintf(stderr," Please Try to use a different IP version.\n\n");
    return FAILURE;
  }
}


if (user_param.work_rdma_cm == OFF) {

  // Prepare IB resources for rtr/rts.
  if (ctx_connect(&ctx,rem_dest,&user_param,my_dest)) {
    fprintf(stderr," Unable to Connect the HCA's through the link\n");
    return FAILURE;
  }
}


/* shaking hands and gather the other side info. */
if (ctx_hand_shake(&user_comm,&my_dest[0],&rem_dest[0])) {
  fprintf(stderr,"Failed to exchange data between server and clients\n");
  return FAILURE;
}


if (user_param.output == FULL_VERBOSITY) {
  printf(RESULT_LINE);
  printf("%s",(user_param.test_type == ITERATIONS) ? RESULT_FMT_LAT : RESULT_FMT_LAT_DUR);
  printf((user_param.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
}

ctx_set_send_wqes(&ctx,&user_param,rem_dest);


if (user_param.test_method == RUN_ALL) {

      // Post receive recv_wqes fo current message size
      if (ctx_set_recv_wqes(&ctx,&user_param)) {
        fprintf(stderr," Failed to post receive recv_wqes\n");
        return FAILURE;
      }


       // Sync between the client and server so the client won't send packets
			 // Before the server has posted his receive wqes (in UC/UD it will result in a deadlock).


			if (ctx_hand_shake(&user_comm,&my_dest[0],&rem_dest[0])) {
				fprintf(stderr,"Failed to exchange data between server and clients\n");
				return FAILURE;
			}


			if(run_iter_lat_send(&ctx, &user_param))
				return 17;


      user_param.test_type == ITERATIONS ? print_report_lat(&user_param) : print_report_lat_duration(&user_param);

      if (user_param.output == FULL_VERBOSITY) {
    		printf(RESULT_LINE);
    	}


      if (ctx_close_connection(&user_comm,my_dest,rem_dest)) {
    		fprintf(stderr,"Failed to close connection between server and client\n");
    		fprintf(stderr," Trying to close this side resources\n");
    	}

    	return send_destroy_ctx(&ctx,&user_param,&mcg_params);
}











```
