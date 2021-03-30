#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h> 
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/tcp.h>

#define SYS_ENTRY_FILENAME "block_port_module" 

static struct kobject *example_kobject;
static int foo;

static struct nf_hook_ops nfhook;
struct sk_buff *sock_buff;                              
struct udphdr *udp_header;
struct net NET;

unsigned int hook_func (void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{ 
    if(!skb)
        return NF_ACCEPT;

    struct iphdr *ip_hdr = (struct iphdr *)skb_network_header(skb);

    if(!ip_hdr)
        return NF_ACCEPT; 

    if(ip_hdr->protocol == IPPROTO_UDP)
    {
        struct udphdr *udph;

        udph = udp_hdr(skb);

        if(!udph)
            return NF_ACCEPT;

        unsigned int dst_port = (unsigned int)ntohs(udph->dest);

        if(dst_port == foo)
                return NF_DROP;
    }

    if(ip_hdr->protocol == IPPROTO_TCP)
    {
        struct tcphdr *tcph;

        tcph = tcp_hdr(skb);

        if(!tcph)
          return NF_ACCEPT; 
        unsigned int dst_port = (unsigned int)ntohs(tcph->dest);

        if(dst_port == foo)
          return NF_DROP;
    }

    return NF_ACCEPT;
}


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
    return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &foo);
    printk("block_port_module: recieved digit from user: %d", foo);
    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(foo, 0660, foo_show, foo_store);

int init_module () 
{
    example_kobject = kobject_create_and_add(SYS_ENTRY_FILENAME, kernel_kobj);

    if(!example_kobject)
        return -ENOMEM;
        
    sysfs_create_file(example_kobject, &foo_attribute.attr);

    nfhook.hook = hook_func;
    nfhook.hooknum = NF_INET_PRE_ROUTING; 
    nfhook.pf = PF_INET; 
    nfhook.priority = NF_IP_PRI_FIRST; 
    nf_register_net_hook(&NET, &nfhook);
    printk(KERN_INFO "Loaded Firewall to Kernal Module\n");
    return 0;
}

void cleanup_module()
{
    printk(KERN_INFO "Unloading Firewall to Kernal Module\n");
    nf_unregister_net_hook(&NET, &nfhook);
    kobject_put(example_kobject);
}



MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Borovik Alexander");
