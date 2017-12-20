### main

```c
int main(int argc, char *argv[]){

  // init default values to user's parameters
	memset(&ctx,		0, sizeof(struct pingpong_context));
	memset(&user_param, 0, sizeof(struct perftest_parameters));
	memset(&user_comm , 0, sizeof(struct perftest_comm));
	memset(&mcg_params, 0, sizeof(struct mcast_parameters));

  user_param.verb    = SEND;
  user_param.tst     = LAT;
  strncpy(user_param.version, VERSION, sizeof(user_param.version));
  user_param.r_flag  = &report;

  // Configure the parameters values according to user arguments or defalut values
  ret_val = parser(&user_param,argv,argc);

  // Finding the IB device selected (or defalut if no selected).
  ib_dev = ctx_find_dev(user_param.ib_devname);

  // Getting the relevant context from the device
  ctx.context = ibv_open_device(ib_dev);

  // Verify user parameters that require the device context
  verify_params_with_device_context(ctx.context, &user_param);

  // See if MTU and link type are valid and supported.
  check_link(ctx.context,&user_param);

  // copy the relevant user parameters to the comm struct + creating rdma_cm resources.
  create_comm_struct(&user_comm,&user_param);

  // in case of server
  if (user_param.output == FULL_VERBOSITY && user_param.machine == SERVER) {
		printf("\n************************************\n");
		printf("* Waiting for client to connect... *\n");
		printf("************************************\n");
	}

  // in case of client
  // Initialize the connection and print the local data.
	if (establish_connection(&user_comm)) {
		  fprintf(stderr," Unable to init the socket connection\n");
		  return FAILURE;
	}

  exchange_versions(&user_comm, &user_param);
  check_sys_data(&user_comm, &user_param);

  // See if MTU and link type are valid and supported.
  check_mtu(ctx.context,&user_param, &user_comm)


  ALLOCATE(my_dest , struct pingpong_dest , user_param.num_of_qps);
  memset(my_dest, 0, sizeof(struct pingpong_dest) * user_param.num_of_qps);  
  ALLOCATE(rem_dest , struct pingpong_dest , user_param.num_of_qps);
  memset(rem_dest, 0, sizeof(struct pingpong_dest) * user_param.num_of_qps);

  // Allocating arrays needed for the test.
  alloc_ctx(&ctx,&user_param);
  ctx_init(&ctx,&user_param);


  // Set up the Connection.
  send_set_up_connection(&ctx,&user_param,my_dest,&mcg_params,&user_comm);


  // Print basic test information.
  ctx_print_test_info(&user_param);


  for (i=0; i < user_param.num_of_qps; i++)
		ctx_print_pingpong_data(&my_dest[i],&user_comm);



  for (i=0; i < user_param.num_of_qps; i++) {

		// shaking hands and gather the other side info.
		if (ctx_hand_shake(&user_comm,&my_dest[i],&rem_dest[i])) {
			fprintf(stderr,"Failed to exchange data between server and clients\n");
			return FAILURE;
		}

		ctx_print_pingpong_data(&rem_dest[i],&user_comm);
	}  





}




```
