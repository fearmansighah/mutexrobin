#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Define a mutex handle
SemaphoreHandle_t mutex;

// Define a shared resource
int sharedResource = 0;

void sharedTask(void *pvParameters)
{
    while (1)
    {
        // Acquire the mutex
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
        {
            // Modify the shared resource
            sharedResource++;
            Serial.print("Shared resource is now: ");
            Serial.println(sharedResource);

            // Release the mutex
            xSemaphoreGive(mutex);
        }

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup()
{
    // Start serial communication
    Serial.begin(9600);

    // Create the mutex
    mutex = xSemaphoreCreateMutex();

    // Create a task that modifies the shared resource
    xTaskCreate(sharedTask, "SharedTask1", 128, NULL, 1, NULL);
    xTaskCreate(sharedTask, "SharedTask2", 128, NULL, 1, NULL);
}

void loop()
{
    // Empty loop
}
