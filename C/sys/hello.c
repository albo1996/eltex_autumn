#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h> 
#include <linux/fs.h>
#include <linux/string.h>

#define SYS_ENTRY_FILENAME "rw_file"


static struct kobject *example_kobject;
char foo[6];
char* answer = "Hi!\n";

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
    if (strlen(foo))
    {
        memset(foo, 0, 6);

        return sprintf(buf, "%s", answer);
    }
    
    return 0;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int compare = memcmp(buf, "hello\n", 6);

    if ((compare == 0))
    {
        sscanf(buf, "%s", foo);
    }

    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(foo, 0660, foo_show, foo_store);

static int __init sys_init (void) 
{
    int error = 0;
    pr_debug("Module initialized successfully \n");
    example_kobject = kobject_create_and_add(SYS_ENTRY_FILENAME, kernel_kobj);
    if(!example_kobject)
        return -ENOMEM;
    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error)
    {
        pr_debug("failed to create the SYS_ENTRY_FILENAME file in /sys/kernel/ \n");
    }
    return error;
}

static void __exit sys_exit (void)
{
    pr_debug ("Module un initialized successfully \n");
    kobject_put(example_kobject);
}

module_init(sys_init); 
module_exit(sys_exit); 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Borovik Alexander"); 