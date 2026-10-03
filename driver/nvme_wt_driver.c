#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "nvme_wt_sim"
#define BUFFER_SIZE 256

static char device_buffer[BUFFER_SIZE];
static size_t data_len;

static DEFINE_MUTEX(buffer_lock);

static ssize_t nvme_wt_read(
    struct file *file,
    char __user *user_buffer,
    size_t count,
    loff_t *position
)
{
    size_t available;

    mutex_lock(&buffer_lock);

    if (*position >= data_len) {
        mutex_unlock(&buffer_lock);
        return 0;
    }

    available = data_len - *position;

    if (count > available)
        count = available;

    if (copy_to_user(user_buffer, device_buffer + *position, count)) {
        mutex_unlock(&buffer_lock);
        return -EFAULT;
    }

    *position += count;

    mutex_unlock(&buffer_lock);

    return count;
}

static ssize_t nvme_wt_write(
    struct file *file,
    const char __user *user_buffer,
    size_t count,
    loff_t *position
)
{
    size_t bytes_to_copy;

    if (count == 0)
        return 0;

    bytes_to_copy = count;

    if (bytes_to_copy >= BUFFER_SIZE)
        bytes_to_copy = BUFFER_SIZE - 1;

    mutex_lock(&buffer_lock);

    if (copy_from_user(device_buffer, user_buffer, bytes_to_copy)) {
        mutex_unlock(&buffer_lock);
        return -EFAULT;
    }

    device_buffer[bytes_to_copy] = '\0';
    data_len = bytes_to_copy;

    mutex_unlock(&buffer_lock);

    return count;
}

static const struct file_operations nvme_wt_fops = {
    .owner = THIS_MODULE,
    .read = nvme_wt_read,
    .write = nvme_wt_write,
};

static struct miscdevice nvme_wt_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &nvme_wt_fops,
    .mode = 0666,
};

static int __init nvme_wt_init(void)
{
    int ret;

    ret = misc_register(&nvme_wt_device);

    if (ret) {
        pr_err("nvme_wt_sim: failed to register device\n");
        return ret;
    }

    pr_info("nvme_wt_sim: driver loaded\n");

    return 0;
}

static void __exit nvme_wt_exit(void)
{
    misc_deregister(&nvme_wt_device);
    pr_info("nvme_wt_sim: driver unloaded\n");
}

module_init(nvme_wt_init);
module_exit(nvme_wt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Priyanshu Aman");
MODULE_DESCRIPTION(
    "Character-device interface for the NVMe write-through simulator"
);
MODULE_VERSION("1.0");
