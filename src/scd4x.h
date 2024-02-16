#define SCD4X_I2C_ADDRESS 0x62
#define SCD4X_GET_SERIAL_NUMBER 0x3682

#define SCD4X_START_PERIODIC_MEASUREMENT 0x21b1
#define SCD4X_READ_MEASUREMENT 0xEC05
#define SCD4X_STOP_PERIODIC_MEASUREMENT 0x3F86

#define SCD4X_START_LOW_POWER_PERIODIC_MEASUREMENT 0x21AC
#define SCD4X_GET_DATA_READY_STATUS 0xE4B8

#define SCD4X_CRC8_POLY 0x31
#define SCD4X_CRC8_INIT 0xFF


// struct scd4xx_data {
//         uint16_t raw_sample;
//         int8_t co2_param[3];
//         int8_t temp_param[3];
//         int8_t rh_param[3];
// };

// struct scd4xx_config {
//         struct i2c_dt_spec bus;
// };

uint8_t scd4x_compute_crc(uint16_t value);

int scd4x_get_serial_number(const struct device *dev);

int scd4x_start_periodic_measurement(const struct device *dev);

int scd4x_read_measurement(const struct device *dev);

