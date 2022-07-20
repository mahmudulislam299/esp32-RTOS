/*
 * freeRTOS task scheduling
 * 
 * blink led in a task
 * 
 * change blink speed in another task
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

 static int speed = 1000;
 int led = 13;

 // task handles
 static TaskHandle_t task_1 = NULL;
 static TaskHandle_t task_2 = NULL;


 // Tasks

 // Task: blink led lower priority
 void startTask1(void *parameter)
 {
  while(1)
  {
    digitalWrite(led,HIGH);
    Serial.println("HIGH");
    vTaskDelay(speed / portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    Serial.println("LOW");
    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
 }

 void startTask2(void *parameter)
 {
  while(1)
  {
    if(Serial.available())
    {
      speed = Serial.parseInt();
      Serial.print("change speed to ");
      Serial.println(speed);
    }
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }
 }

 void setup()
 {
  Serial.begin(115200);
  Serial.println("start the task");
  
  pinMode(led,OUTPUT);

  xTaskCreatePinnedToCore(startTask1,
                          "task 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  xTaskCreatePinnedToCore(startTask2,
                          "task 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);
 }


 void loop()
 {
  
 }
