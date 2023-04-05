#include <Arduino_FreeRTOS.h>

// Define task priorities
#define HIGH_PRIORITY 3
#define MEDIUM_PRIORITY 2
#define LOW_PRIORITY 1
// Create a variable to pass to the tasks
int high = HIGH_PRIORITY;
int medium = MEDIUM_PRIORITY;
int low = LOW_PRIORITY;

void runningTask(void *pvParameters)
{
    int *taskValue = (int *)pvParameters;

    while (1)
    {
        Serial.println(*taskValue);
        if (*taskValue == 1)
        {
            vTaskDelay(pdMS_TO_TICKS(250)); // periodic about 1000ms
        }
        else if (*taskValue == 2)
        {
            vTaskDelay(pdMS_TO_TICKS(500)); // periodic about 500ms
        }
        else if (*taskValue == 3)
        {
            vTaskDelay(pdMS_TO_TICKS(1000)); // periodic about 250ms
        }
    }

    void setup()
    {
        // Start serial communication
        Serial.begin(9600);

        //  We use the (void *) cast to convert the int pointer to a void pointer,
        // which is what the pvParameters argument expects.
        xTaskCreate(runningTask, "HighTask", 128, (void *)&high, HIGH_PRIORITY, NULL);
        xTaskCreate(runningTask, "MediumTask", 128, (void *)&medium, MEDIUM_PRIORITY, NULL);
        xTaskCreate(runningTask, "LowTask", 128, (void *)&low, LOW_PRIORITY, NULL);

        vTaskStartScheduler();
    }

    void loop()
    {
        // Empty loop
    }
