#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include "DHT.h" 
#include <LowPower.h> // Include the LowPower library

#define RTC_CLK_PIN 5 /**< Clock pin for Clock Module  */
#define RTC_DATA_PIN 6 /**< Data pin for Clock Module */
#define RTC_RST_PIN 7 /**< Reset pin for Clock Module */

#define DHT_DATA_PIN 3
#define BTN_PIN 2

DHT dht(DHT_DATA_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2); /**< LCD module object. */
DS1302 rtc(RTC_RST_PIN, RTC_DATA_PIN, RTC_CLK_PIN); /**< Real-time clock module object. */

float temp;
float hum;
float heatIndex;

void setup() {
  Serial.begin(9600);
  pinMode(BTN_PIN, INPUT); // Use the internal pull-up resistor for the button
  
  //clock_init(17, 3, 0, 1, 8, 2023);
  lcd_init();
  dht_init();
  delay(500);
}

void loop() {
  lcd.noBacklight();
  
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), wakeUp, LOW);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(BTN_PIN)); 

  lcd.backlight();
  dht_update();
  lcd_update();
  delay(5000);
}


void clock_init(int hour, int min, int sec, int day, int month, int year){
  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setTime(hour, min, sec);
  rtc.setDate(day, month, year);

  Serial.println("CLK| Successo inizializzazione");
}

void lcd_init(){
  lcd.init();
  lcd.backlight(); // Turn on LCD backlight

  Serial.println("LCD| Successo inizializzazione");
}

void lcd_update(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr(FORMAT_SHORT));
  lcd.print("  ");
  lcd.print("HI: ");
  lcd.print(heatIndex);
  lcd.print(" *C");

  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print("H: ");
  lcd.print(hum);
}

void dht_init(){
  dht.begin();
}

void dht_update(){
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  heatIndex = dht.computeHeatIndex(temp, hum, false);
}

void wakeUp()
{
    // Just a handler for the pin interrupt.
}
