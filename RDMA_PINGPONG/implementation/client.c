#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>

#define TEST_NZ(x) do { if ( (x)) die("error: " #x " failed (returned non-zero)." ); } while (0)
#define TEST_Z(x)  do { if (!(x)) die("error: " #x " failed (returned zero/null)."); } while (0)


static void die(const char *reason);



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

  TEST_Z(ec = rdma_create_event_channel());
  TEST_NZ(rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP));
  TEST_NZ(rdma_bind_addr(listener, (struct sockaddr *)&addr));
  TEST_NZ(rdma_listen(listener, 10)); /* backlog=10 is arbitrary */

  port = ntohs(rdma_get_src_port(listener));
  printf("listening on port %d.\n", port);

  // while (rdma_get_cm_event(ec, &event) == 0) {
  //       struct rdma_cm_event event_copy;
  //
  //       memcpy(&event_copy, event, sizeof(*event));
  //       rdma_ack_cm_event(event);
  //       // where it actually listens
  //       if (on_event(&event_copy)){
  //           break;
  //       }
  // }

  rdma_destroy_id(listener);
  rdma_destroy_event_channel(ec);
  return 0;
}
