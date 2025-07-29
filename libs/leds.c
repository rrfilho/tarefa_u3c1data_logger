#include "leds.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define GREEN_PWM_LED 11
#define BLUE_PWM_LED 12
#define RED_PWM_LED 13
#define CLOCK_DIVIDER 2.0
#define WRAP 4096
#define OFF 0
#define ON 410

static void init_pin(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    unsigned int slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, CLOCK_DIVIDER);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(pin, OFF);
    pwm_set_enabled(slice, true);
}

void leds_init() {
    init_pin(GREEN_PWM_LED);
    init_pin(BLUE_PWM_LED);
    init_pin(RED_PWM_LED);
}

static void set_level(unsigned int led_pin, unsigned int level) {
    pwm_set_gpio_level(led_pin, level);
}

static void clear() {
    set_level(GREEN_PWM_LED, OFF);
    set_level(BLUE_PWM_LED, OFF);
    set_level(RED_PWM_LED, OFF);
}

void leds_set_yellow() {
    clear();
    set_level(GREEN_PWM_LED, ON);
    set_level(RED_PWM_LED, ON);
}

void leds_set_green() {
    clear();
    set_level(GREEN_PWM_LED, ON);
}

void leds_set_red() {
    clear();
    set_level(RED_PWM_LED, ON);
}

void leds_set_blue(){
    clear();
    set_level(BLUE_PWM_LED, ON);
}

void leds_set_purple() {
    clear();
    set_level(BLUE_PWM_LED, ON);
    set_level(RED_PWM_LED, ON);
}