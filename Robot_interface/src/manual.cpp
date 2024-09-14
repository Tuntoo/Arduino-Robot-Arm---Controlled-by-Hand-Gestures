#include <Arduino.h>
#define SIZE 3
int timer = 0;
int temp = 0;


// ------------------------------------------------------------------
//     ESP32                         BOARD
//     3v3                           5
//     GND                           6
//     22                            7 (SCL)
//     21                            8 (SDA)
//     15                            11
//     2                             9
//     4                             10
//     18                            12
//     19                            13
//     23                            14
// -----------------------------------------------------------------

// Khai báo các chân GPIO cho các hàng và cột của ma trận nút

int not_gate(int num){
  if(num == 1){
    num = 0;
  }else{
    num = 1;
  }
  return num;
}

int not_function(int num_pin, int num,int button){
  if(num_pin != 0){
    timer++;
    if(timer > 10000){
    num = not_gate(num);
    }
  }else {
      temp = 0;
      num = not_gate(num);
      timer = 0;
  }
  return num;
}


int add_function( int num_pin, int num,int button){
  if(num_pin != 0){
    temp = num_pin;
    timer++;
    if(timer > 20){
    delay(100);
    num++;
    }
  }else {
    if(temp == button){
      temp = 0; 
      timer = 0;
      num++;
    }
  }
  return num;
}

int sub_function(int num_pin, int num, int button){
  if(num_pin != 0){
    temp = num_pin;
    timer++;
    if(timer > 20){
    delay(100);
    num--;
    }
  }else {
    if(temp == button){
      temp = 0;
      timer = 0;
      num--;
    }
  }
  return num;
}

int read_button(int matrix[SIZE][SIZE],int rowPins[SIZE],int colPins[SIZE],int nums[SIZE][SIZE]){
    // Đọc trạng thái của các nút
  for (int h = 0; h < SIZE; h++) {
    // Đặt hàng hiện tại xuống LOW
    digitalWrite(rowPins[h], HIGH);

    for (int k = 0; k < SIZE; k++) {
      // Đọc trạng thái của cột hiện tại
      matrix[h][k] = digitalRead(colPins[k]);
    }

    // Đặt hàng hiện tại trở lại HIGH
    digitalWrite(rowPins[h], LOW);
  }

  int resultMatrix[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            resultMatrix[i][j] = matrix[i][j] * nums[i][j];
        }
    }
    int num_pin = 0;
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        num_pin += resultMatrix[i][j];
      }
    }
    return num_pin;
}


