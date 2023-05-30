int rPin = D2;  //Receiver pin

// *IMPORTANT*
//    MAKE SURE THAT THE TIMES ARE THE
//    SAME FROM THE TRASMITTER
int s = 70;   //Short time
int l = 100;  //Long time
int b = 40;   //Time between letter signals
int e = 70;   //Time between letters
int n = 0;    //No time

int m = s + round((l - s) / 2);  //Time to measure if a signal is short or long
int s_delay = (s + b) - m + 1;   //Time to wait if a signal is short
int l_delay = (l + b) - m + 1;   //Time to wait if a signal is long

//Personal values of HIGH and LOW signals
#define _HIGH 1
#define _LOW 0

/*
Initialize array with all morse code letters in order.
The letters are made with 's', 'l' or 'n' chars and are used to return their corresponding ascii letter.
*/
char letterTable[][4] = {
  { 's', 'l', 'n', 'n' },  //A
  { 'l', 's', 's', 's' },  //B
  { 'l', 's', 'l', 's' },  //C
  { 'l', 's', 's', 'n' },  //D
  { 's', 'n', 'n', 'n' },  //E
  { 's', 's', 'l', 's' },  //F
  { 'l', 'l', 's', 'n' },  //G
  { 's', 's', 's', 's' },  //H
  { 's', 's', 'n', 'n' },  //I
  { 's', 'l', 'l', 'l' },  //J
  { 'l', 's', 'l', 'n' },  //K
  { 's', 'l', 's', 's' },  //L
  { 'l', 'l', 'n', 'n' },  //M
  { 'l', 's', 'n', 'n' },  //N
  { 'l', 'l', 'l', 'n' },  //O
  { 's', 'l', 'l', 's' },  //P
  { 'l', 'l', 's', 'l' },  //Q
  { 's', 'l', 's', 'n' },  //R
  { 's', 's', 's', 'n' },  //S
  { 'l', 'n', 'n', 'n' },  //T
  { 's', 's', 'l', 'n' },  //U
  { 's', 's', 's', 'l' },  //V
  { 's', 'l', 'l', 'n' },  //W
  { 'l', 's', 's', 'l' },  //X
  { 'l', 's', 'l', 'l' },  //Y
  { 'l', 'l', 's', 's' },  //Z
  { 'n', 'n', 'n', 'n' }   //NULL
};

void setup() {
  pinMode(rPin, INPUT);
  Serial.begin(9600);
}

int receiving = _LOW;  //Used to store the value of the receiver pin

void loop() {
  char letter[4] = { 'n', 'n', 'n', 'n' };  //Used to store the received letter
  receiving = digitalRead(rPin);

  int i = 0;  //letter array index

  //When a HIGH signal is received, this loop will be entered.
  //The loop will measure if a signal is 's' short or 'l' long, and store it's correspondig char in the 'letter' array.
  //If the array is full or if there is no signal to receive, the 'letter' array will be
  //converted into an ascii character wich will be shown in the serial monitor.
  while (receiving == _HIGH && i < 4) {
    delay(m);

    receiving = digitalRead(rPin);

    if (receiving == _HIGH) {
      letter[i] = 'l';
      delay(l_delay);
    } else {
      letter[i] = 's';
      delay(s_delay);
    }

    i++;

    receiving = digitalRead(rPin);

    if (receiving == _LOW || i >= 4) {
      Serial.print(compareLetter(letter));
    }
  }
}

/*
Receives a char array of size 4 that will be compared to the content of 
letterTable and returns it's corresponding ascii char.
*/
char compareLetter(char letter[4]) {
  int count;

  for (int i = 0; i < 26; i++) {
    count = 0;
    for (int j = 0; j < 4; j++) {
      if (letter[j] == letterTable[i][j]) {
        count++;
      }
    }

    if (count == 4) {
      return (char)i + 65;
    }
  }

  return ' ';
}
