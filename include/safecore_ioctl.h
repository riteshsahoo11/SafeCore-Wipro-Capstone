#ifndef SAFECORE_IOCTL_H
#define SAFECORE_IOCTL_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define SAFECORE_IOC_MAGIC 'S'

struct safecore_sensor_data {
    __s32 temperature_c;
    __s32 motor_rpm;
    __s32 vibration;
    __u32 sensor_valid;
};

#define SAFECORE_IOC_SET_SENSOR \
    _IOW(SAFECORE_IOC_MAGIC, 1, struct safecore_sensor_data)

#define SAFECORE_IOC_GET_SENSOR \
    _IOR(SAFECORE_IOC_MAGIC, 2, struct safecore_sensor_data)

#endif
