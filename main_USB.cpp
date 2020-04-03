#include "mbed.h"

#include "USBMouse.h"
#include "USBSerial.h"
#include "USBKeyboard.h"
#include "USBMSD.h"

/* To do
#include "USBAudio.h"
#include "USBCDC.h"
#include "USBCDC_ECM.h"
#include "USBHID.h"
#include "USBMIDI.h"
#include "USBMouseKeyboard.h"
*/

#include <math.h>

#define GET_TARGET_NAME_STR(tgt_name)   #tgt_name
#define GET_TARGET_NAME(tgt_name)       GET_TARGET_NAME_STR(tgt_name)

void appli_mouse(void)
{
    printf("START mouse\n");
    printf("- your mouse cursor will move during 10s\r\n");

    uint16_t x_center = (X_MAX_ABS - X_MIN_ABS) / 2;
    uint16_t y_center = (Y_MAX_ABS - Y_MIN_ABS) / 2;
    uint16_t x_screen = 0;
    uint16_t y_screen = 0;

    uint32_t x_origin = x_center;
    uint32_t y_origin = y_center;
    uint32_t radius = 1000;
    uint32_t angle = 0;

    USBMouse mouse(true, ABS_MOUSE);
    printf("USB init OK\r\n");

    uint32_t Counter = 0;
    while (Counter < 1000) {
        x_screen = x_origin + cos((double)angle * 3.14 / 180.0) * radius;
        y_screen = y_origin + sin((double)angle * 3.14 / 180.0) * radius;

        // printf("x %u y %u\n", x_screen, y_screen);
        mouse.move(x_screen, y_screen);
        angle += 3;
        ThisThread::sleep_for(10);
        Counter += 1;
    }

    printf("\nSTOP mouse\r\n");
}

void appli_keyboard(void)
{
    printf("START keyboard\n");
    printf("- Select the TeraTerm window where print can be displayed during 10s\r\n");
    printf("- Note that selected keyboard is english: World will become Zorld...\r\n");

    USBKeyboard key;
    printf("USB init OK\r\n");

    uint32_t Counter = 0;
    while (Counter < 10) {
        key.printf("Hello World\n");
        ThisThread::sleep_for(1000);
        Counter += 1;
    }

    printf("STOP keyboard\r\n");
}

void appli_serial(void)
{
    printf("START serial\n");
    printf("- After \"USB init OK\" message, open a new TeraTerm with the created COM\r\n");

    USBSerial serial;
    printf("USB init OK\r\n");

    uint32_t Counter = 0;
    while (Counter < 10) {
        serial.printf("I am a virtual serial port %u\r\n", Counter);
        ThisThread::sleep_for(1000);
        Counter += 1;
    }

    printf("STOP serial\r\n");
}


#define DEFAULT_BLOCK_SIZE  512
void appli_msd(void)
{
    printf("START msd\n");
    printf("- System should detect a new drive during 10s\r\n");
    printf("- You should then cancel Windows request\r\n");

    HeapBlockDevice bd(128 * DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    bd.init();

    USBMSD usb(&bd);
    printf("USB init OK\r\n");

    const ticker_data_t *const ticker = get_lp_ticker_data();
    uint32_t start = ticker_read(ticker);
    while ((ticker_read(ticker) - start) < (10000 * 1000)) {
        usb.process();
    }

    printf("STOP msd\r\n");
}

#define USE_USB_NO_OTG   0
#define USE_USB_OTG_FS   1
#define USE_USB_OTG_HS   2
#define USE_USB_HS_IN_FS 3

int main(void)
{
    printf("%s TARGET booted\r\n", GET_TARGET_NAME(TARGET_NAME));

#if defined(MBED_CONF_TARGET_USB_SPEED)
#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_NO_OTG)
    printf("USB configuration %u = USE_USB_NO_OTG (defined in json)\r\n", MBED_CONF_TARGET_USB_SPEED);
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_FS)
    printf("USB configuration %u = USE_USB_OTG_FS (defined in json)\r\n", MBED_CONF_TARGET_USB_SPEED);
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
    printf("USB configuration %u = USE_USB_OTG_HS (defined in json)\r\n", MBED_CONF_TARGET_USB_SPEED);
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_HS_IN_FS)
    printf("USB configuration %u = USE_USB_HS_IN_FS (defined in json)\r\n", MBED_CONF_TARGET_USB_SPEED);
#else
    printf("USB configuration %u = !!! unknown !!! (defined in json)\r\n", MBED_CONF_TARGET_USB_SPEED);
#endif

#if defined (USB)
    printf("default USB configuration would be USE_USB_NO_OTG\r\n");
#elif defined(USB_OTG_FS)
    printf("default USB configuration would be USE_USB_OTG_FS\r\n");
#else
    printf("default USB configuration would be USE_USB_OTG_HS\r\n");
#endif

#else
#if defined (USB)
    printf("USB configuration USE_USB_NO_OTG\r\n");
#elif defined(USB_OTG_FS)
    printf("USB configuration USE_USB_OTG_FS\r\n");
#else
    printf("USB configuration USE_USB_OTG_HS\r\n");
#endif

#endif

    appli_mouse();

    appli_keyboard();

    appli_serial();

    appli_msd();

    printf("ALL DONE\r\n");
}

