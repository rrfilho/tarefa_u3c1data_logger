import numpy as np
import matplotlib.pyplot as plt

raw_data = np.loadtxt("data_logger.csv", delimiter=",", skiprows=1)

timeline = raw_data[:, 0] * 0.5

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8), sharex=True)

# Aceleração
acel_x = raw_data[:, 1]
acel_y = raw_data[:, 2]
acel_z = raw_data[:, 3]
ax1.plot(timeline, acel_x, label='Aceleração X')
ax1.plot(timeline, acel_y, label='Aceleração Y')
ax1.plot(timeline, acel_z, label='Aceleração Z')
ax1.set_title('Aceleração x Tempo')
ax1.set_xlabel('Tempo (s)')
ax1.set_ylabel('Dados brutos')
ax1.legend()
ax1.grid(True)

# Giroscópio
gyro_x = raw_data[:, 4]
gyro_y = raw_data[:, 5]
gyro_z = raw_data[:, 6]
ax2.plot(timeline, gyro_x, label='Giroscópio X')
ax2.plot(timeline, gyro_y, label='Giroscópio Y')
ax2.plot(timeline, gyro_z, label='Giroscópio Z')
ax2.set_title('Giroscópio x Tempo')
ax2.set_xlabel('Tempo (s)')
ax2.set_ylabel('Dados brutos')
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.show()