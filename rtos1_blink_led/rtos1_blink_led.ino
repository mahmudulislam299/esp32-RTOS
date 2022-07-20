/*
 * Demo for blink led
 * 
 * toggls the LED on and off in its own task/thread
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


 // Pins
 #define LED 13

 // task: blink and LED

 void toggleLED(void *parameter)
 {
  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    digitalWrite(LED,LOW);
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }
 }

 void setup()
 {
  // configure pin
  pinMode(LED, OUTPUT);

  //Task to run forever
  xTaskCreatePinnedToCore(  // use xTaskCreate() in vanilla FreeRTOS
              toggleLED,    // function to be called
              "Toggle LED", // name of the task
              1024,         // stack size (byte in esp32, word in vanilla freeRTOS
              NULL,         // parameter to pass to function
              1,            // task priority(0 to max)
              NULL,         // Task to handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)    )
  
 }


 void loop()
 {
  // do nothing
 }

 
