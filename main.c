#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BTN_1_GPIO 9

int main() {
    stdio_init_all(); //Initialize debug interface

    //Setup GPIO:
    gpio_init(BTN_1_GPIO); //Initialize the GPIO pin
    gpio_set_dir(BTN_1_GPIO, GPIO_IN); //Set button GPIO as input
    gpio_pull_up(BTN_1_GPIO); //Enable Pullup on button GPIO

    // MAIN LOOP
    while (true) {
        if (!gpio_get(BTN_1_GPIO)) { //If button gpio is LOW
            printf("DEBUG: Button pressed!\n");
            sleep_ms(100);
        }
    }
    return 0;
}
