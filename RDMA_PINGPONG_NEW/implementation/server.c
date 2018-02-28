#include <fcntl.h>
#include <sys/stat.h>

#include "common.h"
#include "messages.h"

static time_t start; //adding timer

struct conn_context
{
  char *buffer;
  struct ibv_mr *buffer_mr;

  struct message *msg;
  struct ibv_mr *msg_mr;
};

static void send_message(struct rdma_cm_id *id)
{
  struct conn_context *ctx = (struct conn_context *)id->context;

  struct ibv_send_wr wr, *bad_wr = NULL;
  struct ibv_sge sge;

  memset(&wr, 0, sizeof(wr));
  //ctx->msg->buffer
  strcpy(ctx->msg->buffer, ctx->buffer);
  // frisk problem here

  wr.wr_id = (uintptr_t)id;
  wr.opcode = IBV_WR_SEND;
  wr.sg_list = &sge;
  wr.num_sge = 1;
  wr.send_flags = IBV_SEND_SIGNALED;

  sge.addr = (uintptr_t)ctx->msg;
  sge.length = sizeof(*ctx->msg);
  sge.lkey = ctx->msg_mr->lkey;
  TEST_NZ(ibv_post_send(id->qp, &wr, &bad_wr));
}

static void post_receive(struct rdma_cm_id *id)
{
  struct ibv_recv_wr wr, *bad_wr = NULL;

  memset(&wr, 0, sizeof(wr));

  wr.wr_id = (uintptr_t)id;
  wr.sg_list = NULL;
  wr.num_sge = 0;

  TEST_NZ(ibv_post_recv(id->qp, &wr, &bad_wr));
}

static void on_pre_conn(struct rdma_cm_id *id)
{
  struct conn_context *ctx = (struct conn_context *)malloc(sizeof(struct conn_context));

  id->context = ctx;
  posix_memalign((void **)&ctx->buffer, sysconf(_SC_PAGESIZE), BUFFER_SIZE);
  TEST_Z(ctx->buffer_mr = ibv_reg_mr(rc_get_pd(), ctx->buffer, BUFFER_SIZE, IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE));

  posix_memalign((void **)&ctx->msg, sysconf(_SC_PAGESIZE), sizeof(*ctx->msg));
  TEST_Z(ctx->msg_mr = ibv_reg_mr(rc_get_pd(), ctx->msg, sizeof(*ctx->msg), IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE));

  // frisk
  // some how this is letting you do the thing
  // posix_memalign((void **)&ctx->msg->buffer, sysconf(_SC_PAGESIZE), BUFFER_SIZE);
  // TEST_Z(ctx->msg->buffer_mr = ibv_reg_mr(rc_get_pd(), ctx->msg->buffer, BUFFER_SIZE, IBV_ACCESS_LOCAL_WRITE));

  post_receive(id);
}

static void on_connection(struct rdma_cm_id *id)
{
  struct conn_context *ctx = (struct conn_context *)id->context;

  ctx->msg->id = MSG_MR;
  ctx->msg->data.mr.addr = (uintptr_t)ctx->buffer_mr->addr;
  ctx->msg->data.mr.rkey = ctx->buffer_mr->rkey;

  send_message(id);
}


int start_timer = 0;
static void on_completion(struct ibv_wc *wc)
{
  struct rdma_cm_id *id = (struct rdma_cm_id *)(uintptr_t)wc->wr_id;
  struct conn_context *ctx = (struct conn_context *)id->context;

  if (wc->opcode == IBV_WC_RECV_RDMA_WITH_IMM) {

    if(!start_timer){
      start_timer = 1;
      time(&start);
    }

    uint32_t size = ntohl(wc->imm_data);
    printf("received msg: %s\n", ctx->buffer);

    post_receive(id);
    ctx->msg->id = MSG_MR;
    // if(difftime(time(0), start)>=10){
    if(start_timer++>10){
      printf("activated MSG_DONE\n");
      ctx->msg->id = MSG_DONE;
    }
    send_message(id);
  }
}

static void on_disconnect(struct rdma_cm_id *id)
{
  struct conn_context *ctx = (struct conn_context *)id->context;

  ibv_dereg_mr(ctx->buffer_mr);
  ibv_dereg_mr(ctx->msg_mr);
  free(ctx->buffer);
  free(ctx->msg);
  printf("finished transferring %s\n", ctx->buffer);
  free(ctx);
  exit(0);
}

int main(int argc, char **argv)
{
  rc_init(
    on_pre_conn,
    on_connection,
    on_completion,
    on_disconnect);

  printf("waiting for connections. interrupt (^C) to exit.\n");

  rc_server_loop(DEFAULT_PORT);

  return 0;
}
