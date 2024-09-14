#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SoftwareSerial.h>
const int flexsensor_3 = 17;
const int flexsensor_4 = 16;
MPU6050 mpu6050(Wire);
SoftwareSerial SerialBT(3,4); // RX, TX
int X;
int Y;
const int numReadings = 10 ;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int sensorValue_3 = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  SerialBT.begin(9600);
  pinMode(flexsensor_3,INPUT);
  pinMode(flexsensor_4,INPUT);
  for(int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = 0;
  }
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  X = mpu6050.getAngleX();
  Y = mpu6050.getAngleY();
  //int sensorValue_3 = analogRead(flexsensor_3);
  int sensorValue_4 = analogRead(flexsensor_4);

  total = total - readings[readIndex];
  readings[readIndex] = analogRead(flexsensor_3);
  total = total + readings[readIndex];
  readIndex = readIndex + 1 ;
  if(readIndex >= numReadings){
    readIndex = 0;
  }
  sensorValue_3 = total / numReadings;
  // if(sensorValue_3 <650 ){
  // int angle_3 = map(sensorValue_3 , 550, 650, 0, 60);
  // Serial.print("Flex:");
  // Serial.println(angle_3);
  //}

  if(sensorValue_4 > 590){
    SerialBT.write(122);
    Serial.println(122);
    }
  if(sensorValue_4 < 590){
    SerialBT.write(123);
    Serial.println(123);
    }
  if(X > 30){
    SerialBT.write(124);
    Serial.println(124);
    }
  if(X < -30){
    SerialBT.write(125);
    Serial.println(125);
    }
  if(X < 20 && X > -20){
    SerialBT.write(126);
    Serial.println(126);
  }
  if(sensorValue_3 <650 && sensorValue_3 > 550){
  int angle_3 = map(sensorValue_3 , 550, 650, 0, 60);
  SerialBT.write(angle_3);
  Serial.println(angle_3);
  }
  if(Y < -10 && Y > -50){
  int angle_2 = map(Y , -10, -50, 60, 120);
  SerialBT.write(angle_2);
  Serial.println(angle_2);
  }
}