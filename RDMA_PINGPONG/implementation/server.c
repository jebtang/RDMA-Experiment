#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>

#define PORT "3490"  // the port users will be connecting to
#define TEST_NZ(x) do { if ( (x)) die("error: " #x " failed (returned non-zero)." ); } while (0)
#define TEST_Z(x)  do { if (!(x)) die("error: " #x " failed (returned zero/null)."); } while (0)

const int BUFFER_SIZE = 64;
static struct context *s_ctx = NULL;
struct rdma_cm_event *event = NULL;

const char clr[] = { 27, '[', '2', 'J', '\0' };
const char topLeft[] = { 27, '[', '1', ';', '1', 'H','\0' };
time_t start; //adding timer
double latency, prev_latency;

/* Per-port statistics struct */
struct l2fwd_port_statistics {
	uint64_t tx;
	uint64_t rx;
  uint64_t tx_bytes;
	uint64_t rx_bytes;
};

struct l2fwd_port_statistics port_statistics;

void print_log();
static int on_event(struct rdma_cm_event *event);
static void on_completion(struct ibv_wc *wc);

#include "server_init.h"
#include "server_action.h"

static int on_event(struct rdma_cm_event *event);


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



	// struct addrinfo hints, *servinfo, *p;
	// if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
	// 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	// 		return 1;
	// }
  //
	// inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);


  TEST_Z(ec = rdma_create_event_channel());
  TEST_NZ(rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP));
  TEST_NZ(rdma_bind_addr(listener, (struct sockaddr *)&addr));
  TEST_NZ(rdma_listen(listener, 10)); /* backlog=10 is arbitrary */

  port = ntohs(rdma_get_src_port(listener));

	memset(&port_statistics, 0, sizeof(port_statistics));
	time(&start);
	latency = 0;

  printf("listening on port %d.\n", port);

  while (rdma_get_cm_event(ec, &event) == 0) {
		prev_latency = latency;
    struct rdma_cm_event event_copy;
    memcpy(&event_copy, event, sizeof(*event));
    rdma_ack_cm_event(event);

    if (on_event(&event_copy)){
        break;
    }

		latency = difftime(time(0), start);
		if((latency-prev_latency)>=1){
				print_log();
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
  else
    die("on_event: unknown event.");
  return r;
}

void print_log(){
  /* Clear screen and move to top left */
  printf("%s%s", clr, topLeft);
  printf("\nRDMA Pingpong Client ====================================");
  printf("\nByte Statistics ------------------------------"
         "\nPKT-SIZE: %d"
         "\nBytes sent: %ld"
         "\nBytes received: %ld"
         "\nLatency: %f"
         ,BUFFER_SIZE
         ,port_statistics.tx_bytes
         ,port_statistics.rx_bytes
         ,latency);
  printf("\nPacket Statistics ------------------------------"
         "\nPackets received: %ld"
         ,port_statistics.rx);
  printf("\n========================================================\n");
}
