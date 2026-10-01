#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "../include/safecore_ioctl.h"

int main(void)
{
    int fd;
    struct safecore_sensor_data set_data;
    struct safecore_sensor_data get_data;

    fd = open("/dev/safecore", O_RDWR);

    if (fd < 0) {
        perror("Failed to open /dev/safecore");
        return 1;
    }

    set_data.temperature_c = 60;
    set_data.motor_rpm = 1200;
    set_data.vibration = 2;
    set_data.sensor_valid = 1;

    if (ioctl(fd, SAFECORE_IOC_SET_SENSOR, &set_data) < 0) {
        perror("SET_SENSOR ioctl failed");
        close(fd);
        return 1;
    }

    if (ioctl(fd, SAFECORE_IOC_GET_SENSOR, &get_data) < 0) {
        perror("GET_SENSOR ioctl failed");
        close(fd);
        return 1;
    }

    printf("Temperature : %d C\n", get_data.temperature_c);
    printf("Motor RPM   : %d\n", get_data.motor_rpm);
    printf("Vibration   : %d\n", get_data.vibration);
    printf("Valid       : %u\n", get_data.sensor_valid);

    if (get_data.temperature_c != 60 ||
        get_data.motor_rpm != 1200 ||
        get_data.vibration != 2 ||
        get_data.sensor_valid != 1) {

        printf("Driver test FAILED\n");
        close(fd);
        return 1;
    }

    printf("\nDriver test PASSED\n");

    close(fd);
    return 0;
}
