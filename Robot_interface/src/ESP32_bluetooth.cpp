#include <OledDisplay.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_PWMServoDriver.h>
#include <manual.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE,19,18);

#define USE_NAME  // Comment this to use MAC address instead of a slaveName
#define PWM_FREQUENCY 60
#define SERVOMIN  320 // Minimum pulse length count (out of 4096)
#define SERVOMAX  430 // Maximum pulse length count (out of 4096)
#define SERVO_STOP 375 // Stop pulse length count (midway between SERVOMIN and SERVOMAX)
#define SERVOMIN_180 225
#define SERVOMAX_180 575
#define BAUD_RATE 9600
#define ROWS 3
#define COLS 3
//  Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//   Check Serial Port Profile 
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif
BluetoothSerial SerialBT;

#ifdef USE_NAME
String slaveName = "HC-06";  // Change this to reflect the real name of your slave BT device
char *pin = "1234";
#else
  String MACadd = "00:24:01:35:1D:3F";                      // This only for printing
  uint8_t address[6] = {0x00, 0x24, 0x01, 0x35, 0x1D, 0x3F};  // Change this to reflect real MAC address of your slave BT device
  char *pin = "1234";
#endif

String myName = "ESP32-BT-Master";
int Incoming_value = 0; 
int angle_3 = 0;
int angle_X = 0;
const int numReadings = 10 ;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int sensorValue_3 = 0;
int nums[3][3] = {
  {1,2,3},
  {4,5,6},
  {7,8,9}
};
int rowPins[ROWS] = {18,23,19}; // Chân GPIO của các hàng
int colPins[COLS] = {2,15,4}; // Chân GPIO của các cột
int temp_1 = 0;
int temp_2 = 0;
// Khởi tạo mảng để lưu trạng thái của các nút
int matrix[ROWS][COLS];
int sum = 0;
int rd_step = 0;
//int st_step = 0;
int BluetoothMode = 0;
int nd_step = 0;
int fourth_step = 0;
int ManualMode = 0;
unsigned long myTime;


void startup_status(){
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(11, 0, 60);
  delay(1000);
}

void select_button(int num_pin, int temp_2){
  switch(temp_2){
    case 1:
      nd_step = add_function(num_pin,nd_step,1);
      break;
    case 2:
      rd_step = add_function(num_pin,rd_step,2);
      break;
    case 3:
      BluetoothMode  = not_function(num_pin,BluetoothMode,3);
      break;
    case 4:
      nd_step = sub_function(num_pin,nd_step,4);
      break;
    case 5:
      rd_step = sub_function(num_pin,rd_step,5);
      break;
    case 6:
      ManualMode = not_function(num_pin,ManualMode,6);
      break;
    case 7:
      //st_step  = add_function(num_pin,st_step,7);
      break;
    case 8:
      //st_step  = sub_function(num_pin,st_step,8);
      break;
    case 9:
      fourth_step  = not_function(num_pin,fourth_step,9);
      break;
  }
}


void setup() {
  bool connectedd;
  SerialBT.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);
  SerialBT.begin(myName, true);
  u8g2.begin();
  u8g2.setI2CAddress(0x79);
  
  for(int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = 0;
  }
  //*********************************************************************************//
  // //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  warning(&u8g2);
#ifdef USE_NAME
  using_pin(&u8g2);
  connecting(&u8g2);
#endif
// connect(address) is fast (up to 10 secs max), connect(slaveName) is slow (up to 30 secs max) as it needs
// to resolve slaveName to address first, but it allows to connect to different devices with the same name.
// Set CoreDebugLevel to Info to view devices Bluetooth address and device names
#ifdef USE_NAME
  connectedd = SerialBT.connect(slaveName);
#else
  connectedd = SerialBT.connect(address);
  Serial.print("Connecting to slave BT device with MAC ");
  Serial.println(MACadd);
#endif

  if (connectedd) {
      success_connect(&u8g2);
      startup_status();
      BluetoothMode = 1;
  } else {
      fail_connect(&u8g2);
      ManualMode = 1;
  }

  //------------------------ button matrix -----------------
    // Cấu hình các chân hàng là OUTPUT
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW); // Đặt trạng thái ban đầu là HIGH
  }

  // Cấu hình các chân cột là INPUT_PULLDOWN
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLDOWN);
  }

// -------------------- Choose mode ------------------------------//
 }

void loop() {
  int button_num = read_button(matrix,rowPins,colPins,nums);
  select_button(button_num,temp_2);
  temp_2 = button_num;
while(BluetoothMode == 1){
  int button_num = read_button(matrix,rowPins,colPins,nums);
  select_button(button_num,temp_2);
  temp_2 = button_num;
    Incoming_value = SerialBT.read();      //Read the incoming data and store it into variable Incoming_value
    Serial.println(Incoming_value);
    switch (Incoming_value){
      case 122:
        pwm.setPWM(7, 0, SERVOMAX);
        break;
      case 123:
        pwm.setPWM(7, 0, SERVOMIN);
        break;
      case 124:
        pwm.setPWM(4, 0, SERVOMIN);
        break;
      case 125:
        pwm.setPWM(4, 0, SERVOMAX);
        break;
      case  126:
        pwm.setPWM(4,0,SERVO_STOP);
        break;
      }
    if(Incoming_value <60 && Incoming_value > 0){
      total = total - readings[readIndex];
      readings[readIndex] = Incoming_value;
      total = total + readings[readIndex];
      readIndex = readIndex + 1 ;
        if(readIndex >= numReadings){
          readIndex = 0;
      }
      angle_3 = total / numReadings;
      angle_3 =  map(Incoming_value,0, 60, SERVOMAX_180 ,340);
      pwm.setPWM(8, 0, angle_3);
    }
    if(Incoming_value <120 && Incoming_value > 60){
      angle_X =  map(Incoming_value,60 , 120, SERVOMIN_180,400);
      pwm.setPWM(11, 0, angle_X);
    }
  if(ManualMode == 1){
      BluetoothMode = 0;
      unconnect_bluetooth_mode(&u8g2);
      startup_status();
  }
}

  //------------------------ button matrix -----------------
while(ManualMode == 1){
  int button_num = read_button(matrix,rowPins,colPins,nums);
  select_button(button_num,temp_2);
  temp_2 = button_num;
    angle_X =  map(nd_step,60,0, SERVOMIN_180,400);
    pwm.setPWM(11, 0, angle_X);
    angle_3 =  map(rd_step,60,0, SERVOMAX_180 ,340);
    pwm.setPWM(8, 0, angle_3);
    // switch (st_step)                                
    // {
    // case 0:
    //   pwm.setPWM(4,0,SERVO_STOP);
    //   break;
    // case 1:
    //   pwm.setPWM(4, 0, SERVOMAX);
    //   break;
    // case -1:
    //   pwm.setPWM(4, 0, SERVOMIN);
    //   break;
    // }   
    if(fourth_step == 1){
      pwm.setPWM(7, 0, SERVOMAX);
    }else{
      pwm.setPWM(7, 0, SERVOMIN);
    }
    switch (button_num)
    {
    case 1:
      left_2(&u8g2);
      break;
    case 2:
      upper_3(&u8g2);
      break;
    case 4:
      right_2(&u8g2);
      break;
    case 5:
      under_3(&u8g2);
      break;
    case 7:    
      pwm.setPWM(4, 0, SERVOMAX);
      break;
    case 8:
      pwm.setPWM(4, 0, SERVOMIN);
      break;
    case 9:
      hand_4(&u8g2);
      break;
    default:
      pwm.setPWM(4,0,SERVO_STOP);
      break;
    }
  if(BluetoothMode == 1){
    ManualMode = 0;
    unconnect_cable_mode(&u8g2);
    startup_status();
    delay(100);
  }
      temp_1 = BluetoothMode;
  }
}

