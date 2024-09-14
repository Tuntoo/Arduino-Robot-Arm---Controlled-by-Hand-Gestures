#ifndef MANUAL_H
#define MANUAL_H

#include <Arduino.h>

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
#define SIZE 3

// Prototype các hàm
// int multiplyMatrices(int firstMatrix[], int secondMatrix[]);
int not_gate(int num);
int not_function(int num_pin, int num, int button);
int add_function(int num_pin, int num, int button);
int sub_function(int num_pin, int num, int button);
int read_button(int matrix[SIZE][SIZE],int rowPins[SIZE],int colPins[SIZE],int nums[SIZE][SIZE]);

#endif // MANUAL_H
