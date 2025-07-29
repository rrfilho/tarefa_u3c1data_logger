// MPU6050

#include "imu.h"
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1
#define BOUNDRATE 400000
#define I2C_ADDRESS 0x68

#define COMMAND_RESET_1 0x6B
#define COMMAND_RESET_2 0x80
#define COMMAND_RESET_3 0x00
#define COMMAND_ACCELERATION 0x3B
#define COMMAND_ANGLE 0x43
#define COMMAND_TEMPERATURE 0x41
#define ACCELERATION_FACTOR 16384.0f

static void init_ports() {
    i2c_init(I2C_PORT, BOUNDRATE);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));
}

static void reset(){
    unsigned char reset_command[] = {COMMAND_RESET_1, COMMAND_RESET_2};
    i2c_write_blocking(I2C_PORT, I2C_ADDRESS, reset_command, 2, false);
    sleep_ms(100);
    unsigned char reset_command2[] = {COMMAND_RESET_1, COMMAND_RESET_3};
    i2c_write_blocking(I2C_PORT, I2C_ADDRESS, reset_command2, 2, false);
    sleep_ms(10);
}

void imu_init() {
    init_ports();
    reset();
}

static short* read(unsigned char* command) {
    i2c_write_blocking(I2C_PORT, I2C_ADDRESS, command, 1, true);
    unsigned char raw_data[6];
    i2c_read_blocking(I2C_PORT, I2C_ADDRESS, raw_data, 6, false); 
    static short data[3];
    for (int i = 0; i < 3; i++)
        data[i] = (raw_data[i * 2] << 8) | raw_data[(i * 2) + 1];
    return data;
}

static short* read_accelerations() {
    unsigned char acceleration_command[] = {COMMAND_ACCELERATION};
    short* _accelerations = read(acceleration_command);
    static short accelerations[3];
    memcpy(accelerations, _accelerations, sizeof(accelerations));
    return accelerations;
}

static short* read_angles() {
    unsigned char angle_command[] = {COMMAND_ANGLE};
    short* _angles = read(angle_command);
    static short angles[3];
    memcpy(angles, _angles, sizeof(angles));
    return angles;
}

static short* read_temperatures() {
    unsigned char angle_command[] = {COMMAND_TEMPERATURE};
    short* _temperatures = read(angle_command);
    static short temperatures[3];
    memcpy(temperatures, _temperatures, sizeof(temperatures));
    return temperatures;
}

float* imu_accelerations() {
    short* _accelerations = read_accelerations();
    static float accelerations[3];
    accelerations[0] = _accelerations[0];
    accelerations[1] = _accelerations[1];
    accelerations[2] = _accelerations[2];
    return accelerations;
}

float* imu_angles() {
    short* _angles = read_angles();
    static float angles[3];
    angles[0] = _angles[0];
    angles[1] = _angles[1];
    angles[2] = _angles[2];
    return angles;
}

float imu_temperature() {
    short* temperatures = read_temperatures();
    return (temperatures[0] / 340.0f) + 36.53f;
}

float imu_to_roll(float* accelerations) {
    float y = accelerations[1] / ACCELERATION_FACTOR; 
    float z = accelerations[2] / ACCELERATION_FACTOR;
    return atan2(y,z) * 180.0f / M_PI;
}

float imu_to_pitch(float* accelerations) {
    float x = accelerations[0] / ACCELERATION_FACTOR; 
    float y = accelerations[1] / ACCELERATION_FACTOR; 
    float z = accelerations[2] / ACCELERATION_FACTOR;
    return atan2(-x, sqrt(y*y + z*z)) * 180.0f / M_PI;
}