#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/un.h>
#include <linux/net.h>
#include <linux/slab.h>
#include <linux/netdevice.h> 
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/icmp.h>
#include <linux/skbuff.h>


static struct net_device *dev;
static int my_open( struct net_device *dev ) 
{
 	printk( KERN_INFO "Hit: my_open(%s)\n", dev->name );
 	netif_start_queue( dev );
 	return 0;
}

static int my_close( struct net_device *dev ) 
{
 	printk( KERN_INFO "Hit: my_close(%s)\n", dev->name );
 	netif_stop_queue( dev );
 	return 0;
}

static int stub_start_xmit( struct sk_buff *skb, struct net_device *dev )
 {
	struct icmphdr *_icmp_hdr;
	struct sockaddr_in *addr;
	struct socket *sockt;
	struct msghdr *sock_msg;
	struct kvec vec;

	char *buf;
	int size;
	int sequence = 0;

	buf=(char*) kmalloc(((sizeof(char))*200),GFP_KERNEL);
	addr = (struct sockaddr_in*) kmalloc(sizeof(struct sockaddr_in), GFP_KERNEL);
	sock_msg = (struct msghdr*) kmalloc(sizeof(struct msghdr), GFP_KERNEL);
	_icmp_hdr = (struct icmphdr*) kmalloc(sizeof(struct icmphdr), GFP_KERNEL);


	sock_create_kern(&init_net, AF_INET, SOCK_DGRAM, IPPROTO_ICMP, &sockt);

	memset(addr,0,sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr =htonl(INADDR_ANY);

	memset(_icmp_hdr, 0, sizeof(struct icmphdr));
	_icmp_hdr->type = ICMP_ECHO;
	_icmp_hdr->code = 0;
	_icmp_hdr->un.echo.id = 1234;
	_icmp_hdr->un.echo.sequence = sequence++;

	memcpy(buf, &icmp_hdr, sizeof _icmp_hdr);
    memcpy(buf + sizeof _icmp_hdr, "hello", 5);
	size = sizeof _icmp_hdr + 5;
	vec.iov_base = buf;
	vec.iov_len = size;
	sock_msg->msg_name=addr->sin_addr.s_addr;
	sock_msg->msg_control=NULL;
	sock_msg->msg_controllen=0;

	kernel_sendmsg(sockt, sock_msg, &vec, 1, size);
	sock_release(sockt);

	kfree(buf);
	kfree(addr);
	kfree(sock_msg);
	kfree(_icmp_hdr);
	
	dev_kfree_skb( skb );

	return 0;
}

static struct net_device_ops ndo = {
 .ndo_open = my_open,
 .ndo_stop = my_close,
 .ndo_start_xmit = stub_start_xmit,
};

static void my_setup( struct net_device *dev ) 
{
 	int j;
 	for( j = 0; j < ETH_ALEN; ++j )
 	dev->dev_addr[ j ] = (char)j;
 	ether_setup( dev );
 	dev->netdev_ops = &ndo;
}

static int __init my_init( void )
{
	printk( KERN_INFO "Loading stub network module:...." );
	dev = alloc_netdev_mqs( 1, "fict%d",NET_NAME_ENUM , my_setup, 1, 1 );
	if( register_netdev( dev ) ) 
	{
		printk( KERN_INFO " Failed to register\n" );
		free_netdev( dev );
		return -1;
 	}

	printk( KERN_INFO "Succeeded in loading %s!\n", dev_name( &dev->dev ) );

 	return 0;
}

static void __exit my_exit( void ) 
{
 	printk( KERN_INFO "Unloading stub network module\n" );
 	unregister_netdev( dev ); 
 	free_netdev( dev );
}

module_init( my_init );
module_exit( my_exit );
MODULE_AUTHOR( "Borovik Alexander" );
MODULE_LICENSE( "GPL" );
