#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/rwlock.h>
#include <linux/ioctl.h>

#define MAJOR_NUM 0
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)
#define DEVICE_NAME "RW_module"

static ssize_t my_read(struct file *, char *, size_t, loff_t *);
static ssize_t my_write(struct file *, const char *, size_t, loff_t *);

static int Major;
static char msg_buff[80];
static char *msg_ptr = NULL;
static char answer[] = "Hi!";
static rwlock_t lock;

static ssize_t my_read(struct file *filp, char __user *user_buff, size_t length, loff_t * off) {
	size_t rc;
	read_lock(&lock);
	rc = simple_read_from_buffer(user_buff, length, off, msg_buff, 80);
	read_unlock(&lock);
	return rc;
}

static ssize_t my_write(struct file *filp, const char __user *user_buff, size_t len, loff_t * off)
{
	int ret;
	memset(msg_buff, 0, 80);
	if(len > 80)
		return -EINVAL;
	write_lock(&lock);
	ret = simple_write_to_buffer(msg_buff, sizeof(msg_buff), off, user_buff, len);
	write_unlock(&lock);
	if (!ret)
		return -EFAULT;

	return ret;
}

static int my_open(struct inode *inode, struct file *file)
{

  msg_ptr = msg_buff;
  try_module_get(THIS_MODULE);
  return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
  module_put(THIS_MODULE);
  return 0;
}

static ssize_t device_read(struct file *file, char __user * buffer, size_t length, loff_t * offset)
{
  int bytes_read = 0;
 
  if (*msg_ptr == 0)
    return 0;

  while (length && *msg_ptr) 
  {
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }

  msg_ptr = NULL;

  return bytes_read;
}

long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  char *temp;
  int compare;
 
  switch (ioctl_num) 
  {

    case IOCTL_SET_MSG:

      temp = (char *)ioctl_param;
      compare = memcmp(temp, "hello", 5);

      if (compare == 0)
        msg_ptr = answer;
      
      break;
  
    case IOCTL_GET_MSG:
      
      device_read(file, (char *)ioctl_param, 4, 0);
      
      break;
  }
 
  return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = my_read,
  	.write = my_write,
	.open = my_open,
	.release = my_release,
  	.unlocked_ioctl = my_ioctl,
};

int __init init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if( Major < 0 )
    {
		  pr_info(KERN_INFO "Error of register_chrdev\n");
		  return Major;
	}

	pr_info(KERN_INFO "RW_module major = %d\n", Major);
	return 0;
}

void __exit cleanup_module(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borovik Alexander");
