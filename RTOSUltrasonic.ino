#include <Arduino_FreeRTOS.h> 
#include <semphr.h> // mekanisme pensinyalan // variable integer
#include <task.h>
#include<LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd (7,6,5,4,3,2); //RS, E, D4, D5, D6, D7 lcd pin

SemaphoreHandle_t sem; // handler untuk semaphore

//servo
#define servo 8 // D8
Servo servo1;
int pos = 0;


// Ultrasonik1
const int trigger1 = 9; //Variable Global 
const int echo1 = 10;   
long duration; 
int distance; // vGlobal

// Ultrasonik 2
const int trigger2 = 11; 
const int echo2 = 12;
long duration2;
int distance2; // Variable Global


TaskHandle_t Ultrasonic2task;   // task handler
void Task1 (void *pvParameters);

TaskHandle_t Ultrasonic1task;
void Task2 (void *pvParameters); 


void setup() 
{
  Serial.begin(9600); // Serial monitor 9600
 
  lcd.begin(16, 2);// Enable LCD library
  
  pinMode(trigger1, OUTPUT); // i/o pin ultrasonik
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  servo1.attach(8);
  
 // Create tasks
   xTaskCreate(Task1, // Task function
              "Ultrasonic2", // Task name
              128, // Stack size 
              NULL, 
              1, // Priority, Sensor Ultrasonic 2
              &Ultrasonic2task );
              
   xTaskCreate(Task2, // Task function
              "Ultrasonic", // Task name
              128, // Stack size 
              NULL, 
              2, // Priority, Sensor Ultrasonic 1 
              &Ultrasonic1task);  

 sem = xSemaphoreCreateCounting(1,1); // menggunakan semaphore create counting, dgn default 1 memory heap
   vTaskStartScheduler();
}


void loop() {} // gausa diisi

void Task1(void *pvParameters)
{ //ultrasonik2 task
  (void) pvParameters;
  for (;;) 
    {
      
      long distance2,duration2; // variables for storing data
      pinMode(11, OUTPUT);   //trigger pin
      pinMode(12, INPUT);    //echo pin 

      digitalWrite(11, LOW);
      delayMicroseconds(2);
      digitalWrite(11, HIGH);
      delayMicroseconds(10);
      digitalWrite(11, LOW);
  
  duration2 = pulseIn(12, HIGH); 
  distance2 = duration2 *0.10/2;  // Untuk Awal Range Sensor Ultra 
  xSemaphoreTake(sem, portMAX_DELAY); //Mendapatkan semaphore
        lcd.setCursor(0,0);   //display lcd
        lcd.print("Jarak Sensor 2:");
        lcd.setCursor(6,1);
        lcd.print(distance2); //output on lcd in centimeter
        lcd.println("cm");
        
        delay(800);
        lcd.clear();

        Serial.print("Distance2:"); // display it on the virtual terminal
        Serial.print(distance2);
        Serial.print("cm");
        Serial.println();

        if(distance < 8){  // Jika lebih kecil 8cm
          servo1.write(0);
        }
        if(distance > 8) { // jika lebih besar 8cm
          servo1.write(180); // servo bergerak ke 180derajat
        }
        
        xSemaphoreGive(sem); // melepaskan semaphore
         vTaskDelay(1);
  }

}


void Task2(void *pvParameters)
{  //Ultrasonic1
  (void) pvParameters;
  for (;;) 
    {
      
      long distance,duration; // variables for storing data
      pinMode(9, OUTPUT);   //trigger pin
      pinMode(10, INPUT);    //echo pin 

      digitalWrite(9, LOW);
      delayMicroseconds(2);
      digitalWrite(9, HIGH);
      delayMicroseconds(10);
      digitalWrite(9, LOW);
  
  duration = pulseIn(10, HIGH); 
  distance = duration *0.034/2; 
  xSemaphoreTake(sem, portMAX_DELAY); //mendapatkan semaphore
        lcd.setCursor(0,0);   //display on the lcd
        lcd.print("Jarak Sensor 1:");
        lcd.setCursor(6,1);
        lcd.print(distance); //output ultra di lcd 
        lcd.println("cm");
        
        delay(800);
        lcd.clear();

        Serial.print("Distance:"); // display virtual terminal
        Serial.print(distance);
        Serial.print("cm");
        Serial.println();

        if(distance < 8){
          servo1.write(180);
        }
        if(distance > 8) {
          servo1.write(0);
        }
        
        xSemaphoreGive(sem); //melepaskan semaphore
         vTaskDelay(1);
  }

}
