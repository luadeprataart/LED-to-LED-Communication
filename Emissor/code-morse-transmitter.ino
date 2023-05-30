#include <cctype>

int tPin = D3;  //Transmitter pin

//All these values are arbitrary
int s = 70;   //Short time
int l = 100;  //Long time
int b = 40;   //Time between letter signals
int e = 70;   //Time between letters
int n = 0;    //No time

//Personal values of HIGH and LOW signals
#define _HIGH 1
#define _LOW 0

/*
Initialize array with all morse code letters in order.
The letters have a max size of 4 signals, since some of them use less than that there is an 'n' signal that's equal to 0.
All letters are made with 's' (short) signals or 'l' (long) signals, they correspond to the time the signal will be set to HIGH.
*/
int letterTable[][4] = {
  { s, l, n, n },  //A
  { l, s, s, s },  //B
  { l, s, l, s },  //C
  { l, s, s, n },  //D
  { s, n, n, n },  //E
  { s, s, l, s },  //F
  { l, l, s, n },  //G
  { s, s, s, s },  //H
  { s, s, n, n },  //I
  { s, l, l, l },  //J
  { l, s, l, n },  //K
  { s, l, s, s },  //L
  { l, l, n, n },  //M
  { l, s, n, n },  //N
  { l, l, l, n },  //O
  { s, l, l, s },  //P
  { l, l, s, l },  //Q
  { s, l, s, n },  //R
  { s, s, s, n },  //S
  { l, n, n, n },  //T
  { s, s, l, n },  //U
  { s, s, s, l },  //V
  { s, l, l, n },  //W
  { l, s, s, l },  //X
  { l, s, l, l },  //Y
  { l, l, s, s },  //Z
  { n, n, n, n }   //NULL
};

void setup() {
  pinMode(tPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //Start transmission at LOW signal
  digitalWrite(tPin, _LOW);

  //Waiting to receive string from serial input
  while (Serial.available() == 0) {}
  String input = Serial.readString();

  int len = input.length();

  int *message[len];  //Array to hold letters from letterTable
  char str[len];      //Array to hold chars from serial input

  input.toCharArray(str, len);
  Serial.println(str);

  int message_size = 0;
  //Walk through 'str' and add corresponding letters to 'message' array
  for (int i = 0; i < len; i++) {
    //Only accepts alphabetical characters
    if (isalpha(str[i])) {
      Serial.println((char)toupper(str[i]));
      message[message_size] = letterTable[toupper(str[i]) - 65];  //It's -65 so the upper case ascii letters value will range from 0 to 25
      message_size++;
    }
  }

  sendMessage(message, message_size);
}

//Receives an aray of pointers that should correspond to the content from letterTable
void sendMessage(int *message[], int size) {
  for (int i = 0; i < size; i++) {
    sendLetter(message[i]);
    delay(e);
  }
}

//Must receive an array from letterTable and will send corresponding letter via tPin
void sendLetter(int letter[4]) {
  for (int i = 0; i < 4; i++) {
    //If there is an 'n' (null) value the letter will end
    if (letter[i] == n) {
      break;
    }

    //Sends signal from tPin, wait for signal time, deactivates signal from tPin, wait for time between signals
    digitalWrite(tPin, _HIGH);
    delay(letter[i]);
    digitalWrite(tPin, _LOW);
    delay(b);
  }
}
