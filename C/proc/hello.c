#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_FILENAME "rw_file"

int len = 0;
int temp = 0;
char *answer = "Hi!\n";
char *msg = NULL;

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp)
{
	if(count > temp)
		count = temp;
	temp = temp-count;
	copy_to_user(buf, answer, count);
	if(count == 0)
		temp = len;
	return count;
}

ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp)
{
	int compare = memcmp(buf, "hello\n", 6);

	if (compare == 0)
	{
		int len = strlen(buf) + 1;
		msg = kmalloc(len, GFP_KERNEL);
		copy_from_user(msg, answer, count);
		len = count;
		temp = len;
	}

	return count;
}

static struct proc_ops proc_fops = 
{
	.proc_read = read_proc,
	.proc_write = write_proc
};

static int __init proc_init (void)
{
	proc_create(PROC_FILENAME, 0, NULL, &proc_fops);
	return 0;
}

static void __exit proc_exit(void)
{
	remove_proc_entry(PROC_FILENAME, NULL);
	kfree(msg);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Borovik Alexander");
