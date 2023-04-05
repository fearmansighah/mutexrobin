/*
  ENGR 467 Project - RTOS Scheduling and Mutex
  Mouhamed Jaber (79588794)
  Darren Widjaja (43027291)
  Edwin Firmansyah (90320219)
  This code demonstrates the use of FreeRTOS to schedule tasks as well as
  using Mutex for task resource sharing. In this code, the resource being
  shared is the LCD screen.
*/
// Libraries import
#include <Arduino_FreeRTOS.h>  //FreeRTOS
#include <Wire.h>              //Serial Coms Library
#include "semphr.h"            //Semaphore
#include <LiquidCrystal_I2C.h> //LCD screen

// initialization
SemaphoreHandle_t transaction_mutex; // create a mutex handler, used to reference mutex
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define button1 2
#define button2 3
int balance = 100;
bool bt1 = true;
bool bt2 = true;
int n = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);                // set up the LCD's number of columns and rows
  mutex = xSemaphoreCreateMutex(); // create mutex and assign it to its handler
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("$");
  lcd.print(balance);

  xTaskCreate(printSerial, "serial monitor resource", 100, "Main frame view", 2, NULL);
  xTaskCreate(pollButton1, "button resource", 100, NULL, 1, NULL);
  xTaskCreate(pollButton2, "button resource", 100, NULL, 1, NULL);
  vTaskStartScheduler();
}

// printing parameters to serial monitor
void printSerial()
{
  while (1)
  {
    Serial.print("N: ");
    Serial.print(n);
    Serial.print(" | B: ");
    Serial.println(balance);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void pollButton1()
{
  transaction_type =
      while (1)
  {
    bt1 = !digitalRead(button1);
    if (bt1)
    {
      delay(100);
      // Serial.println(!digitalRead(button1));
      deposit();
    }
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}

void pollButton2()
{
  while (1)
  {
    bt2 = !digitalRead(button2);
    if (bt2)
    {
      delay(100);
      // Serial.println(!digitalRead(button2));
      withdraw();
    }
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}

void deposit()
{
  // Serial.println(balance);
  n += 1;
  xSemaphoreTake(mutex, portMAX_DELAY); // takes mutex
  balance += 20;
  // Serial.println(balance);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("+$20");
  delay(2000);
  lcd.clear();
  lcd.print("$");
  lcd.print(balance);
  n -= 1;
  xSemaphoreGive(mutex);
}

void withdraw()
{
  n += 1;
  // Serial.println(balance);
  xSemaphoreTake(mutex, portMAX_DELAY); // takes mutex
  balance -= 20;
  // Serial.println(balance);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("$");
  lcd.print(balance);
  n -= 1;
  xSemaphoreGive(mutex);
}

void loop()
{
}
