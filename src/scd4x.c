#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/crc.h>

#include "scd4x.h"

uint8_t scd4x_compute_crc(uint16_t value)
{
        uint8_t buf[2];

        sys_put_be16(value, buf);

        return crc8(buf, 2, SCD4X_CRC8_POLY, SCD4X_CRC8_INIT, false);
}

int scd4x_write_command(const struct device *dev, uint16_t cmd)
{
        uint8_t tx_buf[2];
        sys_put_be16(cmd, tx_buf);

        return i2c_write(dev, tx_buf, sizeof(tx_buf), SCD4X_I2C_ADDRESS);;
}

int scd4x_get_data_ready_status(const struct device *dev)
{
        int ret;
        uint8_t rx_buf[3];
        uint8_t tx_buf[2];
        sys_put_be16(SCD4X_GET_DATA_READY_STATUS, tx_buf);

        ret = scd4x_write_command(dev, SCD4X_GET_DATA_READY_STATUS);
        if (ret < 0) {
                printk("Failed to request data ready status\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        k_msleep(1);
        
        ret = i2c_read(dev, rx_buf, sizeof(rx_buf), SCD4X_I2C_ADDRESS);
        if (ret < 0) {
                printk("Failed to read data ready status\n");
                printk("Error: %d\n", ret);
                return ret;
        }

        uint16_t data_ready = (rx_buf[0] << 8) | rx_buf[1];
        uint8_t crc = scd4x_compute_crc(data_ready);
        if (crc != rx_buf[2]) {
                printk("CRC check failed\n");
                return -1;
        }

        // If the least significant 11 bits of word[0] are 0 then the data is not ready, else it is ready
        if (data_ready & 0x7FF) {
                // printk("Data is ready\n");
                return 1;
        } else {
                // printk("Data is not ready\n");
                return 0;
        }

        return data_ready;
}

int scd4x_get_serial_number(const struct device *dev)
{
        int ret;
        uint8_t rx_buf[9];
        uint8_t tx_buf[2];
        sys_put_be16(SCD4X_GET_SERIAL_NUMBER, tx_buf);
        
        // ret = i2c_write(dev, tx_buf, sizeof(tx_buf), SCD4X_I2C_ADDRESS);
        ret = scd4x_write_command(dev, SCD4X_GET_SERIAL_NUMBER);
        if (ret < 0) {
                printk("Failed to request serial number\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        k_msleep(1);
        
        ret = i2c_read(dev, rx_buf, sizeof(rx_buf), SCD4X_I2C_ADDRESS);
        if (ret < 0) {
                printk("Failed to read serial number\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        
        uint16_t serial_number[3];
        serial_number[0] = (rx_buf[0] << 8) | rx_buf[1];
        serial_number[1] = (rx_buf[3] << 8) | rx_buf[4];
        serial_number[2] = (rx_buf[6] << 8) | rx_buf[7];
        // print serial number as 48bit number
        printk("Serial number: %d%d%d\n", serial_number[0], serial_number[1], serial_number[2]);
        // check crc
        uint8_t crc[3] = {scd4x_compute_crc(serial_number[0]), scd4x_compute_crc(serial_number[1]), scd4x_compute_crc(serial_number[2])};
        if (crc[0] != rx_buf[2] || crc[1] != rx_buf[5] || crc[2] != rx_buf[8]) {
                printk("CRC check failed\n");
                return -1;
        }

        return 0;
}

int scd4x_start_periodic_measurement(const struct device *dev)
{
        int ret;

        ret = scd4x_write_command(dev, SCD4X_START_PERIODIC_MEASUREMENT);
        if (ret < 0) {
                printk("Failed to start periodic measurement\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        
        return 0;

}

int scd4x_read_measurement(const struct device *dev)
{
        int ret;
        uint8_t rx_buf[9];
        uint8_t tx_buf[2];
        sys_put_be16(SCD4X_READ_MEASUREMENT, tx_buf);

        // first check if data is ready
        ret = scd4x_get_data_ready_status(dev);
        if (ret < 0) {
                printk("Failed to get data ready status\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        if (ret == 0) {
                printk("Data is not ready\n");
                return 0;
        }

        ret = scd4x_write_command(dev, SCD4X_READ_MEASUREMENT);
        if (ret < 0) {
                printk("Failed to request measurement\n");
                printk("Error: %d\n", ret);
                return ret;
        }
        k_msleep(1);
        
        ret = i2c_read(dev, rx_buf, sizeof(rx_buf), SCD4X_I2C_ADDRESS);
        if (ret < 0) {
                printk("Failed to read measurement\n");
                printk("Error: %d\n", ret);
                return ret;
        }

        // rx_buf explanation:
        // - first 2 bytes are CO2, 1 byte crc
        // - next 2 bytes are temperature, needs convertion T = -45 + 175 * (T_raw / (2^16 -1)), 1 byte crc
        // - last 2 bytes are relative humidity, needs convertion RH = 100 * (RH_raw / (2^16 -1), 1 byte crc
        int16_t co2_raw = (rx_buf[0] << 8) | rx_buf[1];
        int16_t temp_raw = (rx_buf[3] << 8) | rx_buf[4];
        int16_t rh_raw = (rx_buf[6] << 8) | rx_buf[7];

        uint8_t temp = -45 + ( 175 * temp_raw / 0xFFFF);
        uint8_t rh = 100 * rh_raw / 0xFFFF;


        printk("Measurement: CO2: %d, Temp: %d, RH: %d\n", co2_raw, temp, rh);

        uint8_t crc_computed_co2 = scd4x_compute_crc(co2_raw);
        uint8_t crc_computed_temp = scd4x_compute_crc(temp_raw);
        uint8_t crc_computed_rh = scd4x_compute_crc(rh_raw);
        // printk("crc computed: %02x %02x %02x\n", crc_computed_co2, crc_computed_temp, crc_computed_rh);
        if (crc_computed_co2 != rx_buf[2] || crc_computed_temp != rx_buf[5] || crc_computed_rh != rx_buf[8]) {
                printk("CRC check failed\n");
                return -1;
        }
        
        return 0;
}