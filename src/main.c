#include <zephyr/kernel.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/printk.h>

#include "scd4x.h"

const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));


int main(void)
{
        k_msleep(1000);
        printk("Starting application\n");

        if (i2c_dev == NULL) {
                printk("I2C device not found\n");
                return -1;
        }
        if (!device_is_ready(i2c_dev)) {
                printk("I2C device not ready\n");
                return -1;
        }

        printk("I2C device %s - %p is ready\n", i2c_dev->name, i2c_dev);

        int ret = scd4x_get_serial_number(i2c_dev);
        if (ret < 0) {
                printk("sdc40_get_serial_number failed\n");
        }

        ret = scd4x_start_periodic_measurement(i2c_dev);
        // k_msleep(5000);  // wait 5s until first measurement is ready

        while (1) {
                ret = scd4x_read_measurement(i2c_dev);
                if (ret < 0) {
                        printk("sdc40_read_measurement failed\n");
                }
                k_msleep(5000);
        }
        return 0;
}
