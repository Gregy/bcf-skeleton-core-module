/*

Visual Studio Code
Ctrl+Shift+B     to build
Ctrl+P task dfu  to flash MCU with dfu-util

*/

#include <application.h>
#include <usb_talk.h>
#include <bc_led.h>
#include <bc_button.h>
#include <bc_i2c.h>
#include <bc_tag_humidity.h>
#include <bc_module_co2.h>
#include <bc_tmp112.h>


bc_led_t led;


void humidity_tag_event_handler(bc_tag_humidity_t *self, bc_tag_humidity_event_t event, void *event_param)
{
    if (event != BC_TAG_HUMIDITY_EVENT_UPDATE)
    {
        return;
    }

    float humidity;
    float temperature;

    if(bc_tag_humidity_get_humidity_percentage(self, &humidity))
    {
        usb_talk_print("humidity", &humidity);
    }
    if(bc_tag_humidity_get_temperature_celsius(self, &temperature))
    {
        usb_talk_print("temperature", &temperature);
    }
}

void co2_event_handler(bc_module_co2_event_t event, void *event_param)
{
    if (event != BC_MODULE_CO2_EVENT_UPDATE)
    {
        return;
    }

    float concentration;

    if(bc_module_co2_get_concentration(&concentration))
    {
        usb_talk_print("co2", &concentration);
    }
}

void internal_temp_event_handler(bc_tmp112_t *self, bc_tmp112_event_t event, void *event_param)
{
    if (event != BC_TMP112_EVENT_UPDATE)
    {
        return;
    }

    float temperature;

    if(bc_tmp112_get_temperature_celsius(self, &temperature))
    {
        usb_talk_publish_thermometer("internal", &temperature);
    }
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 100);

        static uint16_t event_count = 0;

        bc_led_set_mode(&led, (event_count & 1) != 0 ? BC_LED_MODE_BLINK : BC_LED_MODE_OFF);

        usb_talk_publish_push_button("", &event_count);

        event_count++;
    }

    if (event == BC_BUTTON_EVENT_RELEASE)
    {
        bc_led_pulse(&led, 100);
    }
}


bc_tag_humidity_t humidity_tag;
bc_tmp112_t internal_temp;


void application_init(void)
{
    usb_talk_init();

    bc_led_init(&led, BC_GPIO_LED, false, false);
    //bc_led_set_mode(&led, BC_LED_MODE_BLINK);

    static bc_button_t button;

    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_i2c_init(BC_I2C_I2C0, BC_I2C_SPEED_400_KHZ);


    bc_tag_humidity_init(&humidity_tag, BC_TAG_HUMIDITY_REVISION_R3, BC_I2C_I2C0, BC_TAG_HUMIDITY_I2C_ADDRESS_DEFAULT);
    bc_tag_humidity_set_update_interval(&humidity_tag, 30000);
    bc_tag_humidity_set_event_handler(&humidity_tag, humidity_tag_event_handler, NULL);

    bc_module_co2_init();
    bc_module_co2_set_update_interval(30000);
    bc_module_co2_set_event_handler(co2_event_handler,NULL);
    
    //bc_tmp112_init(&internal_temp, BC_I2C_I2C0, 0x49);
    //bc_tmp112_set_update_interval(&internal_temp, 5000);
    //bc_tmp112_set_event_handler(&internal_temp, internal_temp_event_handler, NULL);

}
