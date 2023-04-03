/*
  ENGR 467 Project - RTOS Scheduling and Mutex
  Mouhamed Jaber (79588794)
  Darren Widjaja (43027291)
  Edwin Firmansyah (90320219)
  This code demonstrates the use of FreeRTOS to schedule tasks as well as
  using Mutex for task resource sharing. In this code, the resource being
  shared is the LCD screen.
*/
//Libraries import
#include <Arduino_FreeRTOS.h> //FreeRTOS
#include <Wire.h> //Serial Coms Library
#include "semphr.h" //Semaphore
#include <LiquidCrystal_I2C.h> //LCD screen

//initialization
SemaphoreHandle_t mutex; //create a mutex handler, used to reference mutex
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int button1 = 2;
const int button2 = 3;
int balance = 100;
bool bt1 = false;
bool bt2 = false;
int n = 0; 

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); //set up the LCD's number of columns and rows
  mutex = xSemaphoreCreateMutex(); //create mutex and assign it to its handler
  xTaskCreate(printSerial, "Printer Task 3", 100, "Main frame view", 3, NULL);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(button1)){
    bt1 = true;
  }
  
  if(digitalRead(button1)){
    bt2 = true;
  }
  delay(250);
  if(bt1 && bt2){
    n+=2;
    xTaskCreate(deposit, "Printer Task 1", 100, "Deposit $20", 2, NULL);
    xTaskCreate(withdraw, "Printer Task 2", 100, "Withdraw $20", 1, NULL);
    n-=2;
    bt1 = false;
    bt2 = false;
  }else if(bt1 && !bt2){
    n+=1;
    xTaskCreate(deposit, "Printer Task 1", 100, "Deposit $20", 2, NULL);
    n-=1;
    bt1 = false;
  }else if(bt2 && !bt1){
    n+=1;
    xTaskCreate(withdraw, "Printer Task 2", 100, "Withdraw $20", 1, NULL);
    n-=1;
    bt2 = false;
  }
 }

//printing parameters to serial monitor
void printSerial (){
  while(1){
    Serial.println("************************************************");
    Serial.println("Number of access: " + n);
    Serial.println("Current Balance: " + balance);
    Serial.println("************************************************");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void deposit (void *output){
  char *string = (char *) output; //casting it from a void pointer to a char pointer
  xSemaphoreTake(mutex, portMAX_DELAY); //takes mutex
  balance += 20;
  lcd.setCursor(0,0);
  lcd.print(output);
  delay(3000);
  xSemaphoreGive(mutex);
}
void withdraw (void *output){
  char *string = (char *) output; //casting it from a void pointer to a char pointer
  xSemaphoreTake(mutex, portMAX_DELAY); //takes mutex
  balance -= 20;
  lcd.setCursor(0,0);
  lcd.print(output);
  delay(3000);
  xSemaphoreGive(mutex);
}
