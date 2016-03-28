#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <fact/power.h>
#include <MenuService.h>
#include <fact/MenuFunction.h>
#include <Console.h>
#include <Service.h>

using namespace util;

Service svc1;

MenuService menuService(svc1);

Menu menu;
ConsoleMenu console(&menu);

#ifdef ESP8266
#define ESP_VOID byte v = 0
#else
#define ESP_VOID
#endif



void powerdown_adc(ESP_VOID)
{
#ifndef ESP8266
  Power.adc.off();
#endif
}

void powerdown_usb(ESP_VOID)
{
#ifndef ESP8266
  Power.usb.off();
#endif
}

CREATE_MENUFUNCTION(menu_powerdown_adc, powerdown_adc, "Power down ADC");
CREATE_MENUFUNCTION(menu_powerdown_usb, powerdown_usb, "Power down USB");

void setup()
{
  cout.begin(115200);
  cout << F("Starting up");
  cout.println();

#ifndef ESP8266
  Power.timer[0].off();
#endif
  //Power.usart[0].off();
  //power_timer0_disable();

  menu.add(menu_powerdown_adc);
  menu.add(menu_powerdown_usb);
  //menu.getHeadMenu();
}


void loop()
{
  console.handler();
}
