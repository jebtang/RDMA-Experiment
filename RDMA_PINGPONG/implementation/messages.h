#ifndef RDMA_MESSAGES_H
#define RDMA_MESSAGES_H

const char *DEFAULT_PORT = "12345";


#define BUFFER_SIZE  10
#define LATENCY 1
#define PINGS 100000
#define LIMIT PINGS*BUFFER_SIZE

double total_throughput = 0;
uint64_t start_time, end_time;

uint64_t getTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

enum message_id
{
  MSG_INVALID = 0,
  MSG_MR,
  MSG_READY,
  MSG_DONE
};

struct message
{
  int id;
  char buffer[10];
  union
  {
    struct
    {
      uint64_t addr;
      uint32_t rkey;
    } mr;
  } data;
};

#endif
