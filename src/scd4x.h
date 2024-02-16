#define SCD4X_I2C_ADDRESS 0x62

// Basic commands
#define SCD4X_START_PERIODIC_MEASUREMENT 0x21b1 // send command, exec. time: -, during meas.: no
#define SCD4X_READ_MEASUREMENT 0xec05 // read, exec. time: 1ms, during meas.: yes
#define SCD4X_STOP_PERIODIC_MEASUREMENT 0x3f86 // send command, exec. time: 500ms, during meas.: yes

// On-chip output signal compensation
#define SCD4X_SET_TEMPERATURE_OFFSET 0x241d // write, exec. time: 1ms, during meas.: no
#define SCD4X_GET_TEMPERATURE_OFFSET 0x2318 // read, exec. time: 1ms, during meas.: no
#define SCD4X_SET_SENSOR_ALTITUDE 0x2427 // write, exec. time: 1ms, during meas.: no
#define SCD4X_GET_SENSOR_ALTITUDE 0x2322 // read, exec. time: 1ms, during meas.: no
#define SCD4X_SET_AMBIENT_PRESSURE 0xe000 // write, exec. time: 1ms, during meas.: yes
#define SCD4X_GET_AMBIENT_PRESSURE 0xe000 // read, exec. time: 1ms, during meas.: yes

// Field calibration
#define SCD4X_PERFORM_FORCED_RECALIBRATION 0x362f // send command and fetch result, exec. time: 400ms, during meas.: no
#define SCD4X_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2416 // write, exec. time: 1ms, during meas.: no
#define SCD4X_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2313 // read, exec. time: 1ms, during meas.: no

// Low power periodic measurement mode
#define SCD4X_START_LOW_POWER_PERIODIC_MEASUREMENT 0x21ac // send command, exec. time: -, during meas.: no
#define SCD4X_GET_DATA_READY_STATUS 0xe4b8 // read, exec. time: 1ms, during meas.: yes

// Advanced features
#define SCD4X_PERSIST_SETTINGS 0x3615 // send command, exec. time: 800ms, during meas.: no
#define SCD4X_GET_SERIAL_NUMBER 0x3682 // read, exec. time: 1ms, during meas.: no
#define SCD4X_PERFORM_SELF_TEST 0x3639 // read, exec. time: 10000ms, during meas.: no
#define SCD4X_PERFORM_FACTORY_RESET 0x3632 // send command, exec. time: 1200ms, during meas.: no
#define SCD4X_REINIT 0x3646 // send command, exec. time: 20ms, during meas.: no

// Signle shot measurement mode (SCD41 only)
#define SCD41_MEASURE_SINGLE_SHOT 0x219d // send command, exec. time: 5000ms, during meas.: no
#define SCD41_MEASURE_SINGLE_SHOT_RHT_ONLY 0x2196 // send command, exec. time: 50ms, during meas.: no
#define SCD41_POWER_DOWN 0x36e0 // send command, exec. time: 1ms, during meas.: no
#define SCD41_WAKE_UP 0x36f6 // send command, exec. time: 20ms, during meas.: no
#define SCD41_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD 0x2445 // write, exec. time: 1ms, during meas.: no
#define SCD41_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD 0x2340 // read, exec. time: 1ms, during meas.: no
#define SCD41_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD 0x244e // write, exec. time: 1ms, during meas.: no
#define SCD41_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD 0x234b // read, exec. time: 1ms, during meas.: no



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

