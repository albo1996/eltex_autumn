#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <linux/ip.h>
#include <linux/icmp.h>

#define BUF_SIZE 50
#define DATAGRAM_LENGTH 33
#define CLIENT_PORT 7777

struct udp
{
  unsigned short source;
  unsigned short destination;
  unsigned short length;
  unsigned short sum;
} udp_hdr;


int main(int argc, char** argv)
{
    unsigned short port = 33434;
    unsigned short time_to_live = 0;
    socklen_t len;
    int fd;
    int param = 1;
    char msg[] = "test";
    char buf[BUF_SIZE];
    char datagram[DATAGRAM_LENGTH];
    struct sockaddr_in saddr;
    struct iphdr ip_header;
    struct in_addr d_addr;
    struct in_addr d_addr2;
    char source[16];

    if (argc == 1)
    {
        printf("Missing host operand\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Traceroute to %s\n", argv[1]);
    }

    inet_pton(AF_INET, argv[1], (void*)&d_addr);
    
    while (1)
    {
        time_to_live++;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = d_addr.s_addr;

        udp_hdr.source = htons(CLIENT_PORT);
        udp_hdr.destination = saddr.sin_port;
        udp_hdr.length = htons(sizeof(struct udp) + sizeof(msg));
        udp_hdr.sum = htons(0);

        ip_header.version = IPVERSION;
        ip_header.ihl = 5;
        ip_header.tos = 0;
        ip_header.tot_len = 28 + sizeof(msg);
        ip_header.id = 0;
        ip_header.frag_off = 0;
        ip_header.ttl = time_to_live;
        ip_header.protocol = IPPROTO_UDP;
        ip_header.check = 0;
        ip_header.saddr = 0;
        ip_header.daddr = saddr.sin_addr.s_addr;

        memset(datagram, 0, DATAGRAM_LENGTH);
        memset(buf, 0, BUF_SIZE);

        memcpy(datagram, (const void*)&ip_header, 20);
        memcpy(datagram + 20, (const void*)&udp_hdr, 8);
        strncat(datagram + 20 + 8, msg, sizeof(msg));

        fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &param, sizeof(param));

        len = sizeof(struct sockaddr_in);

        sendto(fd, datagram, DATAGRAM_LENGTH, 0, (struct sockaddr *) &saddr, len);

        while (1)
        {
            recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *) &saddr, &len);

            struct iphdr *ip_header_recv = (struct iphdr*)(buf);
            d_addr2.s_addr = ip_header_recv->saddr;
            inet_ntop(AF_INET,(void*)&d_addr2, source, 16);
            struct icmphdr *icmp_header_recv = (struct icmphdr*)(buf + 20);

            if (ip_header_recv->protocol == 1)
            {
                if (icmp_header_recv->type == 11)
                {
                    printf("%d  %s\n", time_to_live, source);
                    break;
                }
                if ((icmp_header_recv->type == 3) && (strncmp(source, argv[1], strlen(argv[1]))) == 0)
                {
                    printf("%d  %s !\n", time_to_live, source);
                    exit(EXIT_SUCCESS);
                }
            }
            printf("%d  *   *   *\n", time_to_live);
            break;
        }
    }       
}
    

   

