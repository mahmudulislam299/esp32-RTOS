/*
 * freeRTOS Heap Challenge
 * 
 * 
 * One task reads from Serial, constructs a message buffer, and the second
 * prints the message to the console.
 * 
 * date: 8 july 2022
 * author: Mahmudul Islam
 * email: mahmudulislam299@gmail.com
 */

 // use only core 1
 #if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
 #else
  static const BaseType_t app_cpu = 1;
 #endif


 // Settings
 static const uint8_t buf_len = 255;

 // Globals
 static char *msg_ptr = NULL;
 static volatile uint8_t msg_flag = 0;



 /********************************************/
 // tasks

 // task: read message from serial buffer
 void readSerial(void *parameter)
 {
  Serial.println("readSerial start");
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // clear whole buffer
  memset(buf, 0, buf_len);

  while(1)
  {
    //read characters from serial
    if(Serial.available()>0)
    {
      c = Serial.read();


      if(idx < buf_len - 1)
      {
        buf[idx] = c;
        idx++;
      }

      if( c=='*')
      {
        Serial.println("data finished");
        // the last character in the string is '\n', so we need to
        // replace it with'\0' to make it null terminated
        buf[idx -1] = '\0';

        // try to allocate memory and copy over message. if message byffer
        // is in use, ignore the entire message
        if(msg_flag == 0)
        {
          msg_ptr = (char *)pvPortMalloc(idx * sizeof(char));

          // if malloc return 0 (out of memery), throw an error and reset
          configASSERT(msg_ptr);


          // copy message
          memcpy(msg_ptr, buf, idx);

          //notify other task that message is ready
          Serial.println("msg ready to send");
          msg_flag = 1;
      
        }


        // Reset receive buffer and index counter
        memset(buf, 0, buf_len);
        idx = 0;
        
        
      }
    }
  }
 }


// task: print message whenver flag is set and free buffer
void printMessage(void *parameter)
{
  while(1)
  {
    // wait for flag to be set and print message
    if(msg_flag == 1)
    {
      Serial.println(msg_ptr);

      // give amount of free heap memory
      Serial.print("free heap (bytes): ");
      Serial.println(xPortGetFreeHeapSize());


      // free buffer, set pointer to null, and clear flag
      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_flag = 0;
    }
  }
}



//***************************************************
// Main

void setup()
{

  // config serial
  Serial.begin(115200);

  // wait a moment to start
  vTaskDelay(1000/ portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Heap-----");
  Serial.println("enter a string");



  // start serial receive task
  xTaskCreatePinnedToCore(readSerial,
                          "Read Serial",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start Serial print task
  xTaskCreatePinnedToCore(printMessage,
                          "Print Message",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  
  // Delete "setup and loop" task
    vTaskDelete(NULL);
}

void loop() 
{
  // Execution should never get here
}
  
