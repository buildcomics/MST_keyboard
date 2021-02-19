#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BTN_1_GPIO 9
#define LED_1_RED_GPIO 8

#define EVENT_MASK_LOW 0x1
#define EVENT_MASK_HIGH 0x2

void btn_callback(uint gpio, uint32_t events);

int main() {
    stdio_init_all(); //Initialize debug interface
    printf("DEBUG: starting up\n");

    //Setup GPIO with callback:
    gpio_pull_up(BTN_1_GPIO); //Enable pullup
    gpio_set_irq_enabled_with_callback(BTN_1_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback); //Set IRQ interrupt when switch has rising edge

    //setup LED gpio
    gpio_init(LED_1_RED_GPIO);
    gpio_set_dir(LED_1_RED_GPIO, GPIO_OUT);

    // MAIN LOOP
    while (true);
    return 0;
}

void btn_callback(uint gpio, uint32_t events) {
    printf("DEBUG: btn_callback triggered\n");
    printf("DEBUG: gpio: %d\n", gpio);
    printf("DEBUG: events: %d\n", events);

    if (events == GPIO_IRQ_EDGE_FALL) {
        gpio_put(LED_1_RED_GPIO, 1); //Turn on the LED
    }
    else {
        gpio_put(LED_1_RED_GPIO, 0); //Turn off the LED
    }
}
