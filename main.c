#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

#define BTN_1_GPIO 9
#define LED_1_RED_GPIO 8

#define EVENT_MASK_LOW 0x1
#define EVENT_MASK_HIGH 0x2

//Function Prototypes
void btn_callback(uint gpio, uint32_t events);
void hid_task(void);

int main() {
    stdio_init_all(); //Initialize debug interface
    printf("DEBUG: starting up buildcomics HID device...\n");

    board_init();
    tusb_init();
    printf("DEBUG: HID Device initialized\n");

    //Setup GPIO with callback:
    gpio_pull_up(BTN_1_GPIO); //Enable pullup
    gpio_set_irq_enabled_with_callback(BTN_1_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback); //Set IRQ interrupt when switch has rising edge

    //setup LED gpio
    gpio_init(LED_1_RED_GPIO);
    gpio_set_dir(LED_1_RED_GPIO, GPIO_OUT);

    // MAIN LOOP
    while (true){
        hid_task();
        tud_task(); // tinyusb device task
    }
    return 0;
}

// Invoked when device is mounted
void tud_mount_cb(void) {
    printf("DEBUG: Mounted\n");
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    printf("DEBUG: Unmounted\n");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    printf("DEBUG: Suspended\n");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    printf("DEBUG: Resumed Mounted\n");
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
void hid_task(void) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    uint32_t const btn = 1;

    // Remote wakeup
    if (tud_suspended() && btn) {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }

    /*------------- Keyboard -------------*/
    if (tud_hid_ready()) {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_key = false;

        static bool toggle = false;
        if (toggle = !toggle) {
            uint8_t keycode[6] = {0};
            keycode[0] = HID_KEY_A;

            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);

            has_key = true;
        } else {
            // send empty key report if previously has key pressed
            if (has_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_key = false;
        }
    }
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
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
