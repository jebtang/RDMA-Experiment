#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>


// static void die(const char *reason);
// static int on_event(struct rdma_cm_event *event);
struct context {
  struct ibv_context *ctx;
  struct ibv_pd *pd;
  struct ibv_cq *cq;
  struct ibv_comp_channel *comp_channel;

  pthread_t cq_poller_thread;
};


struct connection {
  struct ibv_qp *qp;

  struct ibv_mr *recv_mr;
  struct ibv_mr *send_mr;

  char *recv_region;
  char *send_region;
};


static struct context *s_ctx = NULL;
static void * poll_cq(void *);
static void on_completion(struct ibv_wc *wc);


int main(){

  // basic functions for connection
  #if _USE_IPV6
    struct sockaddr_in6 addr;
  #else
    struct sockaddr_in addr;
  #endif
    struct rdma_cm_event *event = NULL;
    struct rdma_cm_id *listener = NULL;
    struct rdma_event_channel *ec = NULL;
    uint16_t port = 0;


    memset(&addr, 0, sizeof(addr));
  #if _USE_IPV6
    addr.sin6_family = AF_INET6;
  #else
    addr.sin_family = AF_INET;
  #endif

    ec = rdma_create_event_channel();
    rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP);
    rdma_bind_addr(listener, (struct sockaddr *)&addr);
    rdma_listen(listener, 10); /* backlog=10 is arbitrary */

    port = ntohs(rdma_get_src_port(listener));
    printf("listening on port %d.\n", port);

    while (rdma_get_cm_event(ec, &event) == 0) {
          struct rdma_cm_event event_copy;

          memcpy(&event_copy, event, sizeof(*event));
          rdma_ack_cm_event(event);

          if(event->event == RDMA_CM_EVENT_CONNECT_REQUEST){
              printf("RDMA_CM_EVENT_CONNECT_REQUEST\n");
              // event->id
              struct ibv_qp_init_attr qp_attr;
              struct rdma_conn_param cm_params;
              struct connection *conn;

              // build_context(id->verbs);
              s_ctx = (struct context *)malloc(sizeof(struct context));
              s_ctx->ctx = event->id->verbs;
              s_ctx->pd = ibv_alloc_pd(s_ctx->ctx);
              s_ctx->comp_channel = ibv_create_comp_channel(s_ctx->ctx);
              s_ctx->cq = ibv_create_cq(s_ctx->ctx, 10, NULL, s_ctx->comp_channel, 0);
              ibv_req_notify_cq(s_ctx->cq, 0);
              pthread_create(&s_ctx->cq_poller_thread, NULL, poll_cq, NULL);


              // build_qp_attr(&qp_attr);
              // rdma_create_qp(id, s_ctx->pd, &qp_attr)
              // id->context = conn = (struct connection *)malloc(sizeof(struct connection));
              // conn->qp = id->qp;
              //
              // register_memory(conn);
              // post_receives(conn);
              //
              // memset(&cm_params, 0, sizeof(cm_params));
              // TEST_NZ(rdma_accept(id, &cm_params));

          } else if (event->event == RDMA_CM_EVENT_ESTABLISHED){
            printf("RDMA_CM_EVENT_ESTABLISHED\n");
          } else if (event->event == RDMA_CM_EVENT_DISCONNECTED){
            printf("RDMA_CM_EVENT_DISCONNECTED\n");
          }


    //       // where it actually listens
    //       if (on_event(&event_copy)){
    //           break;
    //       }
    }


    rdma_destroy_id(listener);
    rdma_destroy_event_channel(ec);


  return 0;
}


void * poll_cq(void *ctx)
{
  struct ibv_cq *cq;
  struct ibv_wc wc;

  while (1) {
    ibv_get_cq_event(s_ctx->comp_channel, &cq, &ctx);
    ibv_ack_cq_events(cq, 1);
    ibv_req_notify_cq(cq, 0);

    while (ibv_poll_cq(cq, 1, &wc))
      on_completion(&wc);
  }
  return NULL;
}


void on_completion(struct ibv_wc *wc){
  if (wc->status != IBV_WC_SUCCESS)
    die("on_completion: status is not IBV_WC_SUCCESS.");

  if (wc->opcode & IBV_WC_RECV) {
    struct connection *conn = (struct connection *)(uintptr_t)wc->wr_id;

    printf("received message: %s\n", conn->recv_region);


  } else if (wc->opcode == IBV_WC_SEND) {
    printf("send completed successfully.\n");
  }
}
