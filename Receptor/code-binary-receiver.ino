#include <cmath>

int rPin = D2;  //Receiver pin

//  IMPORTANT
//  Make sure that bit_time is the same from the transmitter
int bit_time = 10;              //Time each bit takes to be received
const int byte_size = 8;        //Size of the bytes to be received
int m = round(1.5 * bit_time);  //Time to wait after starter bit is received

//Personal values of HIGH and LOW
#define _HIGH 1
#define _LOW 0

int stop_byte[byte_size];  //Byte to end reception

const int max_message_size = 255;          //Max size of array used to hold received data
int message[max_message_size][byte_size];  //Array to hold received data

void setup() {
  pinMode(rPin, INPUT);
  Serial.begin(9600);

  //Initialize stop_byte with LOW values
  for (int i = 0; i < byte_size; i++) {
    stop_byte[i] = _LOW;
  }
}

void loop() {
  int reading = _LOW;              //Variable to dictate data reception
  int bit = _LOW;                  //Variable to hold each received bit
  int byte[byte_size];             //Array to hold each received byte (used only to verify stop_byte)
  int byte_index = byte_size - 1;  //Byte index starts at the least significant bit
  int message_index = 0;

  reading = digitalRead(rPin);

  if (reading == _HIGH) {
    delay(m);
  }
  while (reading == _HIGH) {
    bit = digitalRead(rPin);

    byte[byte_index] = bit;
    message[message_index][byte_index] = bit;
    delay(bit_time);

    byte_index--;

    if (byte_index < 0) {
      if (verifyStop(byte) || message_index >= max_message_size) {
        reading = _LOW;
        showCharMessage(message, message_index);
      }

      byte_index = byte_size - 1;
      message_index++;
    }
  }
}

//Receives an array of bytes and converts its values into char to be displayed in the Serial Monitor
void showCharMessage(int message[][byte_size], int size) {
  int byte_val;  //Variable used to convert binary to decimal

  for (int i = 0; i < size; i++) {
    byte_val = 0;
    for (int j = 0; j < byte_size; j++) {
      byte_val += message[i][j] * pow(2, j);  //Binary conversion
    }

    Serial.print((char)byte_val);
  }

  Serial.println();
}

//Receives a byte and compares it to stop_byte. Returns 1 if true, 0 otherwise.
int verifyStop(int byte[byte_size]) {
  int count = 0;

  for (int i = 0; i < byte_size; i++) {
    if (byte[i] == stop_byte[i])
      count++;
  }

  if (count == byte_size)
    return 1;
  else
    return 0;
}