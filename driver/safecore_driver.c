#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#include "../include/safecore_ioctl.h"

static struct safecore_sensor_data sensor_data = {
    .temperature_c = 0,
    .motor_rpm = 0,
    .vibration = 0,
    .sensor_valid = 0
};

static DEFINE_MUTEX(safecore_lock);

static int safecore_open(struct inode *inode, struct file *file)
{
    pr_info("SafeCore: device opened\n");
    return 0;
}

static int safecore_release(struct inode *inode, struct file *file)
{
    pr_info("SafeCore: device closed\n");
    return 0;
}

static long safecore_ioctl(struct file *file,
                            unsigned int cmd,
                            unsigned long arg)
{
    struct safecore_sensor_data new_data;

    switch (cmd) {

    case SAFECORE_IOC_SET_SENSOR:

        if (copy_from_user(&new_data,
                           (struct safecore_sensor_data __user *)arg,
                           sizeof(new_data))) {
            return -EFAULT;
        }

        if (new_data.sensor_valid > 1) {
            return -EINVAL;
        }

        mutex_lock(&safecore_lock);
        sensor_data = new_data;
        mutex_unlock(&safecore_lock);

        pr_info(
            "SafeCore: sensor updated T=%d C RPM=%d VIB=%d VALID=%u\n",
            new_data.temperature_c,
            new_data.motor_rpm,
            new_data.vibration,
            new_data.sensor_valid
        );

        return 0;

    case SAFECORE_IOC_GET_SENSOR:

        mutex_lock(&safecore_lock);
        new_data = sensor_data;
        mutex_unlock(&safecore_lock);

        if (copy_to_user(
                (struct safecore_sensor_data __user *)arg,
                &new_data,
                sizeof(new_data))) {
            return -EFAULT;
        }

        return 0;

    default:
        return -ENOTTY;
    }
}

static const struct file_operations safecore_fops = {
    .owner = THIS_MODULE,
    .open = safecore_open,
    .release = safecore_release,
    .unlocked_ioctl = safecore_ioctl
};

static struct miscdevice safecore_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "safecore",
    .fops = &safecore_fops,
    .mode = 0666
};

static int __init safecore_init(void)
{
    int ret;

    ret = misc_register(&safecore_device);

    if (ret) {
        pr_err("SafeCore: failed to register device\n");
        return ret;
    }

    pr_info("SafeCore: driver loaded successfully\n");
    pr_info("SafeCore: device available at /dev/safecore\n");

    return 0;
}

static void __exit safecore_exit(void)
{
    misc_deregister(&safecore_device);

    pr_info("SafeCore: driver unloaded\n");
}

module_init(safecore_init);
module_exit(safecore_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SafeCore Project");
MODULE_DESCRIPTION("Virtual Linux device driver for the SafeCore industrial safety controller");
MODULE_VERSION("1.0");
