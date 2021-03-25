#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/blk_types.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <uapi/linux/hdreg.h>
#include <uapi/linux/cdrom.h> 


#define SUCCESS 0

static const char* _blkdev_name = "rw_dev";
static const size_t _blkdev_buffer_size = PAGE_SIZE;
unsigned short device_write_counter = 0;

typedef struct blkdev_cmd_s
{

} blkdev_cmd_t;

typedef struct blkdev_device_s
{
    sector_t capacity;			    
    u8* data;			   
    atomic_t open_counter;

    struct blk_mq_tag_set tag_set;
    struct request_queue *queue;

    struct gendisk *disk;

} blkdev_device_t;

static int major = 0;
static blkdev_device_t* _blkdev_device = NULL;

// functions
static int blkdev_allocate_buffer(blkdev_device_t* dev)
{
    dev->capacity = _blkdev_buffer_size >> SECTOR_SHIFT;
    dev->data = kmalloc(dev->capacity << SECTOR_SHIFT, GFP_KERNEL); //
    if (dev->data == NULL) 
    {
        printk(KERN_WARNING "rw_dev: vmalloc failure.\n");
        return -ENOMEM;
    }

    return SUCCESS;
}

static void blkdev_free_buffer(blkdev_device_t* dev)
{
    if (dev->data) 
    {
        kfree(dev->data);

        dev->data = NULL;
        dev->capacity = 0;
    }
}

static void blkdev_remove_device(void)
{
    blkdev_device_t* dev = _blkdev_device;
    if (dev == NULL)
        return;

    if (dev->disk)
        del_gendisk(dev->disk);

    if (dev->queue) 
    {
        blk_cleanup_queue(dev->queue);
        dev->queue = NULL;
    }

    if (dev->tag_set.tags)
        blk_mq_free_tag_set(&dev->tag_set);

    if (dev->disk) 
    {
        put_disk(dev->disk);
        dev->disk = NULL;
    }

    blkdev_free_buffer(dev);

    kfree(dev);
    _blkdev_device = NULL;

    printk(KERN_WARNING "rw_dev: simple block device was removed\n");
}

static int do_simple_request(struct request *rq, unsigned int *nr_bytes)
{
    int ret = SUCCESS;
    struct bio_vec bvec;
    struct req_iterator iter;
    blkdev_device_t *dev = rq->q->queuedata;
    loff_t pos = blk_rq_pos(rq) << SECTOR_SHIFT;
    loff_t dev_size = (loff_t)(dev->capacity << SECTOR_SHIFT);

    printk(KERN_WARNING "rw_dev: request start from sector %lld \n", blk_rq_pos(rq));
    
    rq_for_each_segment(bvec, rq, iter)
    {
        unsigned long b_len = bvec.bv_len;

        void* b_buf = page_address(bvec.bv_page) + bvec.bv_offset;
        int compare = memcmp(b_buf, "hello", 5 );

        if ((pos + b_len) > dev_size)
            b_len = (unsigned long)(dev_size - pos);

        if (rq_data_dir(rq))//WRITE
        {   
            if(compare == 0)
            {
                memcpy(dev->data + pos, b_buf, b_len);
                device_write_counter++;
            }

            printk(KERN_WARNING "rw_dev: attempt to write\n");
        }
        else //READ
        {
            if (device_write_counter)
            {
                sprintf(b_buf, "Hi!\n");
                device_write_counter--;
            }
        }
    }

    return ret;
}

static blk_status_t _queue_rq(struct blk_mq_hw_ctx *hctx, const struct blk_mq_queue_data* bd)
{
    unsigned int nr_bytes = 0;
    blk_status_t status = BLK_STS_OK;
    struct request *rq = bd->rq;

    blk_mq_start_request(rq);

    if (do_simple_request(rq, &nr_bytes) != SUCCESS)
        status = BLK_STS_IOERR;

    printk(KERN_WARNING "rw_dev: request process %d bytes\n", nr_bytes);

    blk_mq_end_request(rq, status);

    return BLK_STS_OK;
}

static struct blk_mq_ops _mq_ops = {
    .queue_rq = _queue_rq,
};

static const struct block_device_operations _fops = {
    .owner = THIS_MODULE,
};

static int blkdev_add_device(void)
{
    int ret = SUCCESS;

    blkdev_device_t* dev = kzalloc(sizeof(blkdev_device_t), GFP_KERNEL);
    if (dev == NULL) {
        printk(KERN_WARNING "rw_dev: unable to allocate %ld bytes\n", sizeof(blkdev_device_t));
        return -ENOMEM;
    }
    _blkdev_device = dev;

    do
    {
        ret = blkdev_allocate_buffer(dev);
        if(ret)
            break;

        dev->tag_set.ops = &_mq_ops;
        dev->tag_set.nr_hw_queues = 1;
        dev->tag_set.queue_depth = 128;
        dev->tag_set.numa_node = NUMA_NO_NODE;
        dev->tag_set.cmd_size = sizeof(blkdev_cmd_t);
        dev->tag_set.flags = BLK_MQ_F_SHOULD_MERGE ;
        dev->tag_set.driver_data = dev;

        ret = blk_mq_alloc_tag_set(&dev->tag_set);

        if (ret) 
        {
            printk(KERN_WARNING "rw_dev: unable to allocate tag set\n");
            break;
        }

        struct request_queue *queue = blk_mq_init_queue(&dev->tag_set);

        if (IS_ERR(queue)) 
        {
            ret = PTR_ERR(queue);
            printk(KERN_WARNING "rw_dev: Failed to allocate queue\n");
            break;
        }

        dev->queue = queue;

        dev->queue->queuedata = dev;

       struct gendisk *disk = alloc_disk(1);
        if (disk == NULL) 
        {
            printk(KERN_WARNING "rw_dev: Failed to allocate disk\n");
            ret = -ENOMEM;
            break;
        }

        disk->major = major;
        disk->first_minor = 0;
        disk->fops = &_fops;
        disk->private_data = dev;
        disk->queue = dev->queue;
        sprintf(disk->disk_name, "rw_dev%d", 0);
        set_capacity(disk, dev->capacity);

        dev->disk = disk;
        add_disk(disk);

        printk(KERN_WARNING "rw_dev: simple block device was created\n");

    } while(false);

    if (ret)
    {
        blkdev_remove_device();
        printk(KERN_WARNING "rw_dev: Failed add block device\n");
    }

    return ret;
}

static int __init blkdev_init(void)
{
    int ret = SUCCESS;

    major = register_blkdev(major, _blkdev_name);
    if (major <= 0){
        printk(KERN_WARNING "rw_dev: unable to get major number\n");
        return -EBUSY;
    }

    ret = blkdev_add_device();
    if (ret)
        unregister_blkdev(major, _blkdev_name);
        
    return ret;
}

static void __exit blkdev_exit(void)
{
    blkdev_remove_device();

    if (major > 0)
        unregister_blkdev(major, _blkdev_name);
}

module_init(blkdev_init);
module_exit(blkdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borovik Alexander");
