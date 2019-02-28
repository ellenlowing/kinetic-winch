#include <Wire.h>
#include <ArduinoJson.h>

int t = 0;
char msg[32];
char *res;

void setR( int value ) {
  int digits = 0;
  int temp = value;
  
  while (temp) {
      temp /= 10;
      digits++;
  }

  char rd[4];
  itoa(value,rd,10);

//  Serial.print("red: ");
//  Serial.println(rd);
  
  if( digits == 1 ) {
    msg[0] = '0';
    msg[1] = '0';
    msg[2] = rd[0];
  }
  else if ( digits == 2 ) {
    msg[0] = '0';
    msg[1] = rd[0];
    msg[2] = rd[1];
  }
  else {
    msg[0] = rd[0];
    msg[1] = rd[1];
    msg[2] = rd[2];
  }
  
//  Serial.print("Red msg: ");
//  Serial.println(msg);
  delay(1000);
}

void setB( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }
  
  char bl[4];
  itoa(value,bl,10);

//  Serial.print("blue: ");
//  Serial.println(bl);

  if( digits == 1 ) {
    msg[3] = '0';
    msg[4] = '0';
    msg[5] = bl[0];
  }
  else if ( digits == 2 ) {
    msg[3] = '0';
    msg[4] = bl[0];
    msg[5] = bl[1];
  }
  else {
    msg[3] = bl[0];
    msg[4] = bl[1];
    msg[5] = bl[2];
  }
  
//  Serial.print("Blue msg: ");
//  Serial.println(msg);
  delay(1000);
}

void setG( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }

  char gr[4];
  itoa(value,gr,10);

//  Serial.print("green: ");
//  Serial.println(gr);

  if( digits == 1 ) {
    msg[6] = '0';
    msg[7] = '0';
    msg[8] = gr[0];
  }
  else if ( digits == 2 ) {
    msg[6] = '0';
    msg[7] = gr[0];
    msg[8] = gr[1];
  }
  else {
    msg[6] = gr[0];
    msg[7] = gr[1];
    msg[8] = gr[2];
  }
  
//  Serial.print("Green msg: ");
//  Serial.println(msg);
  delay(1000);
}

void setRPM( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }

  char sd[4];
  itoa(value,sd,10);

//  Serial.print("speed: ");
//  Serial.println(sd);

  if( digits == 1 ) {
    msg[9] = '0';
    msg[10] = '0';
    msg[11] = sd[0];
  }
  else if ( digits == 2 ) {
    msg[9] = '0';
    msg[10] = sd[0];
    msg[11] = sd[1];
  }
  else {
    msg[9] = sd[0];
    msg[10] = sd[1];
    msg[11] = sd[2];
  }
  
//  Serial.print("Speed msg: ");
//  Serial.println(msg);
  delay(1000);
}

void setDir( int value ) {
  char d = '0' + value;
  msg[12] = d;
  msg[13] = ' ';
//  Serial.print("Dir msg: ");
//  Serial.println(msg);
  res = msg;
  delay(1000);
}

void setup() {
  
  Wire.begin(); // join i2c bus (address optional for master)
//  Serial.begin(9600);
//  Serial.println("beginning");
}

void loop() {
  int r = random(0,255);
  int b = random(0,255);
  int g = random(0,255);
  int s = random(0,600);
  int d = t % 2;

  setR(r);
  setB(b);
  setG(g);
  setRPM(s);
  setDir(d);

//  Serial.print("Result: ");
//  Serial.println(res);
//  
//  delay(10000);
//  Serial.println("------------------------------------------------------");

  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(res);        // message
  Wire.endTransmission();    // stop transmitting
}
