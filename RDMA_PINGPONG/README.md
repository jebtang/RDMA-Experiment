### PREREQUISTIES
- [reference link](https://github.com/linux-rdma/perftest)
- [fundamentals link](https://github.com/tarickb/the-geek-in-the-corner/blob/master/02_read-write/rdma-server.c)
- [server debug](debug_server)
- [client debug](debug_client)

<br>

### creating packets

```c
const size_t SIZE = 1024;

char *buffer = malloc(SIZE);
struct ibv_mr *mr;
struct ibv_sge sge;
struct ibv_send_wr wr, *bad_wr;
uint32_t peer_key;
uint64_t peer_addr;

mr = ibv_reg_mr(
  pd,
  buffer,
  SIZE,
  IBV_ACCESS_LOCAL_WRITE);


strcpy(buffer, "Hello!");
memset(&wr, 0, sizeof(wr));

sge.addr = (uint64_t)buffer;
sge.length = SIZE;
sge.lkey = mr->lkey;

wr.sg_list = &sge;
wr.num_sge = 1;
wr.opcode = IBV_WR_RDMA_WRITE;
wr.wr.rdma.remote_addr = peer_addr;
wr.wr.rdma.rkey = peer_key;

ibv_post_send(qp, &wr, &bad_wr);

```
