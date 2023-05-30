int tPin = D3;  //Transmitter pin

int bit_time = 10;        //Time each bit takes to be sent
const int byte_size = 8;  //Size of the bytes to be sent

//Personal values of HIGH and LOW
#define _HIGH 1
#define _LOW 0

int stop_byte[byte_size];  //Byte to end transmission
int start_bit = _HIGH;     //Bit to start transmission

void setup() {
  pinMode(tPin, OUTPUT);
  Serial.begin(9600);

  //Initialize stop_byte with all LOW values
  for (int i = 0; i < byte_size; i++) {
    stop_byte[i] = _LOW;
  }
}

void loop() {
  //Start transmission at LOW signal
  digitalWrite(tPin, _LOW);

  //Waiting to receive string from serial input
  while (Serial.available() == 0) {}
  String input = Serial.readString();

  int len = input.length();

  char str[len];  //Array to hold chars from serial input

  input.toCharArray(str, len);

  len--;  //Ignore last char ('\0')

  sendCharArray(str, len);
}

//Receives a char array and transforms each char in a binary array that will be transmitted
void sendCharArray(char* str, int size) {
  int message[size][byte_size] = {};
  int byte_index = 0;
  int n = 0;  //Used to convert each char into a binary number

  for (int i = 0; i < size; i++) {
    n = (int)str[i];

    byte_index = byte_size - 1;

    //Converts each char into a binary number and puts it in the byte array
    while (n != 0 && byte_index >= 0) {
      if (n % 2 == 1) {
        message[i][byte_index] = _HIGH;
      } else {
        message[i][byte_index] = _LOW;
      }

      byte_index--;
      n = n / 2;
    }
  }

  sendBit(start_bit);

  for (int i = 0; i < size; i++) {
    sendByte(message[i]);
  }

  sendByte(stop_byte);

  //Prints binary message
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < byte_size; j++) {
      Serial.print(message[i][j]);
    }
  }

  Serial.println('\n');
}

//Receives a byte and transmits all of its bits
void sendByte(int byte[byte_size]) {
  for (int i = 0; i < byte_size; i++) {
    sendBit(byte[i]);
  }
}

//Receives a bit and sends its value via tPin, then waits for bit_time to complete
void sendBit(int bit) {
  digitalWrite(tPin, bit);
  delay(bit_time);
}