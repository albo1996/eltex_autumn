#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/ip.h>

#define MESSAGE_SIZE 128
#define BUF_SIZE 284
#define DATAGRAM_LENGTH 244
#define IP_LEN 20
#define SERVER_PORT 3200
#define CLIENT_PORT 7777

struct udp
{
  unsigned short source;
  unsigned short destination;
  unsigned short length;
  unsigned short sum;
} udp_hdr;

int main()
{
  int fd;
  int param = 1;
  socklen_t len;
  char msg[MESSAGE_SIZE];
  char buf[BUF_SIZE];
  char datagram[DATAGRAM_LENGTH];
  struct sockaddr_in saddr;
  char res[128];
  struct iphdr ip_header;

  memset(datagram, 0, DATAGRAM_LENGTH);
  memset(buf, 0, BUF_SIZE);
  memset(res, 0, 128);

  printf("Введите строку...\n");
  fgets(msg, MESSAGE_SIZE, stdin);
  msg[strlen(msg) - 1] = 0;

  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(SERVER_PORT);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);

  udp_hdr.source = htons(CLIENT_PORT);
  udp_hdr.destination = saddr.sin_port;
  udp_hdr.length = htons(sizeof(struct udp) + sizeof(msg));
  udp_hdr.sum = htons(0);

  ip_header.version = IPVERSION;
  ip_header.ihl = 5;
  ip_header.tos = 0;
  ip_header.tot_len = 28 + strlen(msg) + 1;
  ip_header.id = 0;
  ip_header.frag_off = 0;
  ip_header.ttl = IPDEFTTL;
  ip_header.protocol = IPPROTO_UDP;
  ip_header.check = 0;
  ip_header.saddr = 0;
  ip_header.daddr = saddr.sin_addr.s_addr;

  memcpy(datagram, (const void*)&ip_header, 20);
  memcpy(datagram + IP_LEN, (const void*)&udp_hdr, 8);
  strncat(datagram + IP_LEN + 8, msg, strlen(msg));

  fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &param, sizeof(param));

  len = sizeof(struct sockaddr_in);

  sendto(fd, datagram, sizeof(datagram), 0, (struct sockaddr *) &saddr, len);

  while (1)
  {
    recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *) &saddr, &len);
    struct udp *udp_header_recv = (struct udp*)(buf + 20);

    if ((ntohs(udp_header_recv->destination)) == CLIENT_PORT)
    {
      printf("Response: %s\n", (buf + 28));
      break;
    }
  }
}