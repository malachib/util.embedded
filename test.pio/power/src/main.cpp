#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <fact/power.h>
#include <MenuService.h>
#include <fact/MenuFunction.h>
#include <Console.h>
#include <Service.h>

#ifndef ESP8266
#include <fact/watchdog.h>
#endif

using namespace util;

#define COUT_BPS 115200

Service svc1;

MenuService menuService(svc1);

Menu menu;
ConsoleMenu console(&menu);

#ifdef ESP8266
#define ESP_VOID byte v = 0
#else
#define ESP_VOID
#endif

#define PIN_LED 17



void powerdown_adc(ESP_VOID)
{
#ifndef ESP8266
  Power.adc.off();
#endif
}

void powerdown_usb(ESP_VOID)
{
#if not defined(ESP8266) and not defined(__AVR_ATtiny85__)
  Power.usb.off();
#endif
}

void sleep_4(ESP_VOID)
{
#ifndef ESP8266
  digitalWrite(PIN_LED, HIGH);
  delay(500);
  digitalWrite(PIN_LED, LOW);
  delay(500);
  digitalWrite(PIN_LED, HIGH);
  delay(500);
  digitalWrite(PIN_LED, LOW);
#ifdef __AVR_ATmega32U4__
  Power.usb.off();
#endif
  Power.sleepWithWatchdog(WDTO_4S);

  // This worked (although no multimeter was used):
  //Watchdog.isr.on();
  //Watchdog.enable(WDTO_4S);
  //Power.sleep(SLEEP_MODE_PWR_DOWN);

  //wdt_enable(WDTO_4S); // set WDTO itself
  //Watchdog.setupPreset(WDTO_4S); // set behavior to only cause an interrupt, not a reset
  //Watchdog.sleepPreset();
  digitalWrite(PIN_LED, HIGH);

  //while ( (PLLCSR & (1 << PLOCK)) == 0){}
  //while (PLLCSR.PLOCK != 1);   // while (!Pll_ready());
  //USBCON &= ~(1 << FRZCLK);
  //USBCON.FRZCLK = 0;   // Usb_unfreeze_clock();

#ifdef __AVR_ATmega32U4__
  Power.usb.on();
#endif
  cout.begin(COUT_BPS);
  delay(100);
#endif
}

CREATE_MENUFUNCTION(menu_powerdown_adc, powerdown_adc, "Power down ADC");
CREATE_MENUFUNCTION(menu_powerdown_usb, powerdown_usb, "Power down USB");
CREATE_MENUFUNCTION(menu_sleep4, sleep_4, "Sleep for 4 seconds");

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  cout.begin(COUT_BPS);
  cout << F("Starting up");
  cout.println();

#ifndef ESP8266
  Power.timer[1].off();
  Power.timer[1].on();
#endif
  //Power.usart[0].off();
  //power_timer0_disable();

  menu.add(menu_powerdown_adc);
  menu.add(menu_powerdown_usb);
  menu.add(menu_sleep4);
  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
