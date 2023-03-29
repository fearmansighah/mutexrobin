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
#include "semphr.h" //Semaphore
#include <LiquidCrystal.h> //LCD screen

//initialization
SemaphoreHandle_t mutex; //create a mutex handler, used to reference mutex
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //initialize the LCD library with the pin numbers


void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); //set up the LCD's number of columns and rows
  mutex = xSemaphoreCreateMutex(); //create mutex and assign it to its handler
  
  //Creating task
   xTaskCreate(TaskFunction, "Printer Task 1", 100, "Task 1 running", 2, NULL);
   xTaskCreate(TaskFunction, "Printer Task 2", 100, "Task 2 running", 1, NULL);
   xTaskCreate(TaskFunction, "Printer Task 3", 100, "Task 3 running", 1, NULL);
}

//Defining the function of a task
void TaskFunction (void *output){
  char *string = (char *) output; //casting it from a void pointer to a char pointer
  while(1){
    disp(string);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

//disp function to send the string to the LCD screen
void disp(const char *printString){
  xSemaphoreTake(mutex, portMAX_DELAY); //takes mutex
  Serial.println(printString);
  lcd.setCursor(0,0);
  lcd.print(printString);
  delay(1000);
  xSemaphoreGive(mutex); //return mutex
}

void loop() {}
