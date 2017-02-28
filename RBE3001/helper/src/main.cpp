#include "Arduino.h"
#include "SharpIR.h"

#define IR_1 A0
#define IR_2 A1


SharpIR ir1(IR_1, 25, 93, 20150);
SharpIR ir2(IR_2, 25, 93, 20150);

void setup(){
  Serial.begin(115200);
}

void loop(){
  int ir1val = ir1.distance();
  int ir2val = ir2.distance();

  Serial.println(String(ir1val, DEC) + "," + String(ir2val, DEC));
}
