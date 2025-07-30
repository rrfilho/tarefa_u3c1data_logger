#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "libs/buttons.h"
#include "libs/leds.h"
#include "libs/buzzer.h"
#include "libs/display.h"
#include "libs/imu.h"
#include "libs/secure_digital_card.h"

#define FILENAME "data_logger3.csv"

static volatile bool _call_capture = false;
static volatile bool _call_mount = false;

static bool _capture = false;
static bool _mount = false;
static int _samples = 0;

void call_capture() {
    _call_capture = true;
}

void call_mount() {
    _call_mount = true;
}

void init() {
    stdio_init_all();
    buttons_init(call_capture, call_mount);
    buzzer_init();
    leds_init();
    display_init();
    imu_init();
    secure_digital_card_init();
    display_set_status("Initilizing");
    display_set_samples(_samples);
    leds_set_yellow();
    sleep_ms(5000);
}

void start_stop_capture_data() {
    _call_capture = false;
    if (!_mount) {
        display_set_message("Err capture");
        return;
    }
    if (_capture) {
        buzzer_double_beep();
        leds_set_green();
        _capture = false;
        display_set_status("ready");
    } else {
        buzzer_beep();
        leds_set_red();
        _capture = true;
        display_set_status("Writing");
    }
}

void mount_unmount() {
    _call_mount = false;
    if (_capture) {
        display_set_message("Err unmount");
        return;
    }
    if (_mount) {
        display_set_status("Unmounting");
        int status = secure_digital_card_unmount();
        if (status == 0) {
            display_set_status("Unmounted");
            display_set_message("");
            leds_set_yellow();
            _mount = false;
        } else {
            display_set_status("Mounted");
            display_set_message("Err unmount");
            leds_set_purple();
        }
    } else {
        display_set_status("Mounting");
        int status = secure_digital_card_mount();
        if (status == 0) {
            display_set_status("Mounted");
            display_set_message("");
            leds_set_green();
            _mount = true;
            secure_digital_card_print_file(FILENAME);
        } else {
            display_set_status("Unmounted");
            display_set_message("Err mount");
            leds_set_purple();
        }
    }
}

void main() {
    init();
    display_set_status("Waiting");
    while (true) {
        sleep_ms(500);
        if (_call_capture) start_stop_capture_data();
        if (_call_mount) mount_unmount();
        if (!_capture) continue;
        float* accelerations = imu_accelerations();
        float* angles = imu_angles();
        char data[128];
        sprintf(data, "%d, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f\n", _samples++, accelerations[0], accelerations[1], accelerations[2], angles[0], angles[1], angles[2]);
        display_set_samples(_samples);
        int status = secure_digital_card_save_data(FILENAME, data);
        if (status != 0) display_set_message("Err write");
    }
}
