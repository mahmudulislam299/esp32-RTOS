/*
 * freeRTOS Stack Overflow
 * 
 * 
 * Generate a stack overflow on purpose
 * 
 * date: 7 july 2022
 * author: Mahmudul Islam
 * email: mahmudulislam299@gmail.com
 */

// use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Task: perform some mundane task
void testTask(void *parameter)
{
  while(1)
  {
    int a = 1;
    int b[50]; // int take 4 byte and so this array will take 400 byte


    // do something with array so its not optimized out by the compiler
    for(int i = 0; i<50; i++)
    {
      b[i] = a + 1;
    }
    Serial.println(b[0]);


    //print out remaining stack memory(words)
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));


    // print out number of free heap memory bytes before malloc
    Serial.print("heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());
    int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));


    // One way to prevent heap overflow is to check the malloc output
    if (ptr == NULL) {
      Serial.println("Not enough heap.");
      vPortFree(NULL);
    } else {
      
      // Do something with the memory so it's not optimized out by the compiler
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }

    // Print out number of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): "); 
    Serial.println(xPortGetFreeHeapSize());

    // Free up our allocated memory
    //vPortFree(ptr);

    // Wait for a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
  }
}



void setup()
{
  Serial.begin(115200);

  vTaskDelay(1000/ portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("-----FreeRTOS memory Demo----------");


  // start the task
  xTaskCreatePinnedToCore(testTask,
                          "test task",
                          1500,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}


void loop()
{
  
}
