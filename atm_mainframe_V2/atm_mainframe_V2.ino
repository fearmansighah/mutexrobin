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
int n_requests = 0;
int n_blocked = 0;
bool transaction_type = true; // true = deposit, false = withdraw;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);                            // set up the LCD's number of columns and rows
  transaction_mutex = xSemaphoreCreateMutex(); // create mutex and assign it to its handler
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("$");
  lcd.print(balance);

  xTaskCreate(printSerial, "serial monitor resource", 100, NULL, 2, NULL);
  xTaskCreate(pollButton1, "button resource", 100, NULL, 1, NULL);
  xTaskCreate(pollButton2, "button resource", 100, NULL, 1, NULL);
  vTaskStartScheduler();
}

// printing parameters to serial monitor
void printSerial()
{
  while (1)
  {
    Serial.print("NR: ");
    Serial.print(n_requests);
    Serial.print(" | NB: ");
    Serial.print(n_blocked);
    Serial.print(" | B: ");
    Serial.print(" | B: ");
    Serial.println(balance);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void pollButton1()
{
  while (1)
  {
    bt1 = !digitalRead(button1);
    if (bt1)
    {
      delay(100);
      // Serial.println(!digitalRead(button1));
      transaction_type = true;
      transaction(transaction_type);
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
      transaction_type = false;
      transaction(transaction_type);
    }
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}

void transaction(bool type)
{
  n_requests += 1; // increment number of requests being made to mainframe

  if (xSemaphoreTake(transaction_mutex, portMAX_DELAY) == pdTRUE)
  {

    // now that we can access the resource:
    if (type)
    { // deposit operation by User A and update User A view
      balance += 20;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("+$20");
      delay(2000);
      lcd.clear();
      lcd.print("$");
      lcd.print(balance);
    }
    else
    { // withdraw operation by User B and update User A view
      balance -= 20;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("$");
      lcd.print(balance);
    }

    if (n_blocked > 0)
    { // decrement number of blocked requests made to mutex now that it has been fulfilled
      n_blocked -= 1;
    }

    n_requests -= 1; // decrement number of requests to mainframe now that it has been fulfilled
    xSemaphoreGive(transaction_mutex);
  }
  else
  {
    n_blocked += 1; // increment number of blocked requests made to mutex
  }
}

void loop()
{
}
