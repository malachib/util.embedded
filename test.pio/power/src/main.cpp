#include <Arduino.h>

#include <fact/Menu.h>
#include <fact/lib.h>
#include <fact/power.h>
#include <fact/sleep.h>
#include <MenuService.h>
#include <fact/MenuFunction.h>
#include <Console.h>
#include <Service.h>

#ifndef ESP8266
#include <fact/watchdog.h>
#endif

#if SAMD_SERIES
#include <driver/atmel/sam/gclk.h>
#endif

using namespace util;


#ifdef __AVR_ATtiny85__
#define COUT_BPS 9600
#else
#define COUT_BPS 115200
#endif

Service svc1;

MenuService menuService(svc1);

Menu menu;
ConsoleMenu console(&menu);

#ifdef ESP8266
#define ESP_VOID byte v = 0
#else
#define ESP_VOID
#endif

#if !defined(__AVR_ATtiny85__) and !defined(PIN_LED)
#ifdef ARDUINO_SPARKFUN_SAMD
#define PIN_LED 13
#else
#define PIN_LED 17
#endif
#endif

#ifdef __AVR_ATtiny85__
#include <SoftwareSerial.h>

#define PIN_RX PB0
#define PIN_TX PB1

SoftwareSerial cout(PIN_RX, PIN_TX);
#endif

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

void show_int_status()
{
#if SAMD_SERIES
  cout << F("Interrupt status: ") << Watchdog::getEarlyWarningStatus();
#endif
}

extern int wdt_handler_called;

void sleep_4(ESP_VOID)
{
  static int counter = 0;

#ifdef SAMD_SERIES
  digitalWrite(PIN_LED, HIGH);
  delay(500);

  // OK, power off the USB cuz the computer will freak out otherwise
  Power.usb.off();
  auto wdto = Watchdog::WDTO::fromMS<4096>();
  Watchdog::enable(wdto);

  Power.deepSleep();

  Power.usb.on();

  Watchdog::disable();

  digitalWrite(PIN_LED, LOW);
#elif not defined(ESP8266)
#ifdef PIN_LED
  digitalWrite(PIN_LED, HIGH);
  delay(500);
  digitalWrite(PIN_LED, LOW);
  delay(500);
  digitalWrite(PIN_LED, HIGH);
  delay(500);
  digitalWrite(PIN_LED, LOW);
#endif
#ifdef __AVR_ATmega32U4__
  Power.usb.off();
#endif
  auto wdto = Watchdog::WDTO::fromMS<4000>();
  Sleep.powerDown(wdto);

  // This worked (although no multimeter was used):
  //Watchdog.isr.on();
  //Watchdog.enable(WDTO_4S);
  //Power.sleep(SLEEP_MODE_PWR_DOWN);

#ifdef PIN_LED
  digitalWrite(PIN_LED, HIGH);
#endif

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

  cout.print("Sleep request# ");
  cout.print(++counter);
  cout.println();

  cout << F("WDT interrupt called: ") << wdt_handler_called;
  cout.println();
}

CREATE_MENUFUNCTION(menu_powerdown_adc, powerdown_adc, "Power down ADC");
CREATE_MENUFUNCTION(menu_powerdown_usb, powerdown_usb, "Power down USB");
CREATE_MENUFUNCTION(menu_sleep4, sleep_4, "Sleep for 4 seconds");
CREATE_MENUFUNCTION(menu_show_int_status, show_int_status, "Show last WDT interrupt status");

void setup()
{
#ifdef PIN_LED
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
#endif

  cout.begin(COUT_BPS);
  cout << F("Starting up");
  cout.println();

#if not defined(ESP8266) and not defined(SAMD_SERIES)
  Power.timer[1].off();
  Power.timer[1].on();
#endif

#if SAMD_SERIES
  Watchdog::initialize();
  // docs say that WDT interrupt is used to wake up from sleep [[1]]
  Watchdog::enableInterrupt();
  //Watchdog::disableInterrupt();

  // FIX: experimentation just to make sure we aren't having some kind of
  // power issue while testing.  According to docs if we are expecting a
  // power draw of over 50ua from the pins during sleep, activate this.
  // I expect we won't actually need this
  SYSCTRL->VREG.bit.RUNSTDBY = 1;
#endif
  //Power.usart[0].off();
  //power_timer0_disable();

  menu.add(menu_powerdown_adc);
  menu.add(menu_powerdown_usb);
  menu.add(menu_sleep4);
  menu.add(menu_show_int_status);
  //menu.getHeadMenu();

}


void loop()
{
  console.handler();
}

/*
 * REFERENCES:
 *
 * [[1]] Atmel-42181-SAM-D21_Datasheet.pdf: 18.5.2
 */
