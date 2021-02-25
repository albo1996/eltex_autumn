#include <sys/types.h>                                                          
#include <sys/socket.h>                                                         
#include <arpa/inet.h>                                                          
#include <netinet/in.h>
#include <stdio.h>                                                              
#include <stdlib.h>    
#include <string.h> 
#include <unistd.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#define SERV_PORT 3200
#define CLIENT_PORT 7777
#define BUFSIZE 32
#define IP_LEN 20
#define UDP_LEN 8
#define ETH_LEN 14

struct udp_hdr
{
  unsigned short s_port;
  unsigned short d_port;
  unsigned short length;
  unsigned short sum;
};

void get_mac(unsigned char* s_mac, char* iface)
{
  struct ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  unsigned char* mac;

  ioctl(fd, SIOCGIFHWADDR, &ifr);
	
  mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
  for(int i=0; i<6; ++i)
  {
    s_mac[i] = mac[i];
  }
  close(fd);
}

unsigned short ccsum(struct iphdr* ihdr)
{
  unsigned int csum = 0;
  unsigned short* ptr = (unsigned short*)ihdr;

  for(int i=0; i<10; ++i)
  {
    csum += *ptr;
    ++ptr;
  }
  csum = csum + (csum >> 16);
  return (~csum)&0xffff;
}

int main()
{
  unsigned short size = ETH_LEN+IP_LEN+UDP_LEN+BUFSIZE;
  unsigned char* buf = calloc(size, 1);
  int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  struct udp_hdr* uhdr = calloc(UDP_LEN, 1);
  struct udp_hdr* recv_uhdr = calloc(UDP_LEN, 1);
  struct iphdr* ihdr = calloc(IP_LEN, 1);
  struct ethhdr* ehdr = calloc(ETH_LEN, 1);
  struct sockaddr_ll serv;
  struct sockaddr_in servaddr;

  unsigned char d_mac[6] = {0x08, 0x00, 0x27, 0x72, 0x4a, 0xac};
  unsigned char s_mac[6];
  get_mac(s_mac, "wlxbc0f9a2b4fc6");

  serv.sll_family = AF_PACKET;
  serv.sll_halen = 6;
  serv.sll_ifindex = if_nametoindex("wlxbc0f9a2b4fc6");

  memcpy(ehdr->h_dest, d_mac, 6);
  memcpy(ehdr->h_source, s_mac, 6);
  ehdr->h_proto = htons(ETHERTYPE_IP);

  ihdr->version = IPVERSION;
  ihdr->ihl = 5;
  ihdr->tos = 0;
  ihdr->tot_len = htons(size-ETH_LEN);
  ihdr->id = htons(1337);
  ihdr->frag_off = 0;
  ihdr->ttl = IPDEFTTL;
  ihdr->protocol = IPPROTO_UDP;
  ihdr->check = 0;
  ihdr->saddr = inet_addr("192.168.1.7");
  ihdr->daddr = inet_addr("192.168.1.6");

  ihdr->check = ccsum(ihdr);

  uhdr->s_port = htons(CLIENT_PORT);
  uhdr->d_port = htons(SERV_PORT);
  uhdr->length = htons(UDP_LEN+BUFSIZE);
  uhdr->sum = 0;

  memcpy(buf, (const void*)ehdr, ETH_LEN);
  memcpy(buf+ETH_LEN, (const void*)ihdr, IP_LEN);
  memcpy(buf+ETH_LEN+IP_LEN, (const void*)uhdr, UDP_LEN);
  
  printf("Введите сообщение:\n");
  fgets(buf+UDP_LEN+IP_LEN+ETH_LEN, BUFSIZE, stdin);

  sendto(fd, buf, size, 0, (const struct sockaddr*)&serv, sizeof(serv));

  char* ptr = calloc(size, 1);
  socklen_t len = sizeof(struct sockaddr_in);
  while(1)
  {
    recvfrom(fd, ptr, size, 0, (struct sockaddr*) &servaddr, &len);
    recv_uhdr = (struct udp_hdr*)(ptr+IP_LEN+ETH_LEN);
    if(ntohs(recv_uhdr->d_port) == CLIENT_PORT)
    {
      printf("Response: %s", ptr+UDP_LEN+IP_LEN+ETH_LEN);
      break;
    }
  }
}
