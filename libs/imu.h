#pragma once

void imu_init();
float* imu_accelerations();
float* imu_angles();
float imu_temperature();
float imu_to_roll(float* accelerations);
float imu_to_pitch(float* accelerations);