#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>

#define TEST_NZ(x) do { if ( (x)) die("error: " #x " failed (returned non-zero)." ); } while (0)
#define TEST_Z(x)  do { if (!(x)) die("error: " #x " failed (returned zero/null)."); } while (0)


const int TIMEOUT_IN_MS = 500; /* ms */
const int BUFFER_SIZE = 1024;
static struct context *s_ctx = NULL;
struct rdma_cm_event *event = NULL;

/* Per-port statistics struct */
struct l2fwd_port_statistics {
	uint64_t tx;
	uint64_t rx;
	uint64_t rx_bytes;
};

static int on_event(struct rdma_cm_event *event);
static void on_completion(struct ibv_wc *wc);

#include "server_init.h"
#include "server_action.h"

static int on_event(struct rdma_cm_event *event);

struct l2fwd_port_statistics port_statistics;

int main(int argc, char **argv){

#if _USE_IPV6
  struct sockaddr_in6 addr;
#else
  struct sockaddr_in addr;
#endif
  struct rdma_cm_id *listener = NULL;
  struct rdma_event_channel *ec = NULL;
  uint16_t port = 0;

  memset(&addr, 0, sizeof(addr));
#if _USE_IPV6
  addr.sin6_family = AF_INET6;
#else
  addr.sin_family = AF_INET;
#endif

  TEST_Z(ec = rdma_create_event_channel());
  TEST_NZ(rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP));
  TEST_NZ(rdma_bind_addr(listener, (struct sockaddr *)&addr));
  TEST_NZ(rdma_listen(listener, 10)); /* backlog=10 is arbitrary */

  // port = ntohs(rdma_get_src_port(listener));

  memset(&port_statistics, 0, sizeof(port_statistics));

  printf("listening on port %d.\n", port);
  printf("initialize port_statistics rx: %ld  tx: %ld\n", port_statistics.rx, port_statistics.tx);

  while (rdma_get_cm_event(ec, &event) == 0) {
    struct rdma_cm_event event_copy;

    memcpy(&event_copy, event, sizeof(*event));
    rdma_ack_cm_event(event);

    if (on_event(&event_copy)){
        break;
    }
  }

  rdma_destroy_id(listener);
  rdma_destroy_event_channel(ec);
  return 0;
}


int on_event(struct rdma_cm_event *event){
  int r = 0;

  if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST)
    r = on_connect_request(event->id);
  else if (event->event == RDMA_CM_EVENT_ESTABLISHED)
    r = on_connection(event->id->context);
  else if (event->event == RDMA_CM_EVENT_DISCONNECTED)
    r = on_disconnect(event->id);
  else if (event->event == RDMA_CM_EVENT_ADDR_RESOLVED)
    r = on_addr_resolved(event->id);
  else if (event->event == RDMA_CM_EVENT_ROUTE_RESOLVED)
    r = on_route_resolved(event->id);
  else
    die("on_event: unknown event.");
  return r;
}
