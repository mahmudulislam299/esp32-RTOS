/*
 * freeRTOS task scheduling
 * 
 * toggle LED and print "hello mahmdul" in separate task
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

   const char msg[] = "hello mahmudul islam robince, iot engineer, adorshopranisheba";

  // task handles
  static TaskHandle_t task_1 = NULL;
  static TaskHandle_t task_2 = NULL;


  // Tasks

  // Task: print ot serial terminal with lower priority
  void startTask1(void *parameter)
  {
    // count number of character in string
    int msg_len = strlen(msg);

    // print string to terminal

    while(1)
    {
      Serial.println();
      for (int i = 0; i< msg_len; i++)
      {
        Serial.print(msg[i]);
      }
      Serial.println();
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
  }


  // Task: print to serial terminal with higher priority
  void startTask2(void *parameter)
  {
    while(1)
    {
      Serial.print('*');
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }


  // MAIN

  void setup()
  {
    Serial.begin(300);

    //wait a moment
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.println();
    Serial.println("task priority");

    // print self priority
    Serial.print("setup and loop task running on core ");
    Serial.print(xPortGetCoreID());
    Serial.print(" with priority ");
    Serial.println(uxTaskPriorityGet(NULL));
    

    //Task to run forever
    xTaskCreatePinnedToCore(startTask1,
                            "Task 1",
                            1024,
                            NULL,
                            1,
                            &task_1,
                            app_cpu);

  // task to run once with higher priority
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);
  }


  void loop()
  {
    // suspend the higher priority task for some intervals
    for(int i = 0; i< 3; i++)
    {
      vTaskSuspend(task_2);
      vTaskDelay(2000/ portTICK_PERIOD_MS);
      vTaskResume(task_2);
      vTaskDelay(2000/ portTICK_PERIOD_MS);
    }


    // Delete the lower priority task
    if(task_1 != NULL)
    {
      vTaskDelete(task_1);
      task_1 = NULL;
    }
  }
