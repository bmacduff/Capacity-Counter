#include "Adafruit_VL53L0X.h"

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

#define SHT_LOX1 7
#define SHT_LOX2 6

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

int CapacityCount = 0;


VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;


void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}



void setup() {
  Serial.begin(115200);

  
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  
  Serial.println(F("Starting..."));

  /*BTserial.begin(115200);     
  
  Serial.println("BTserial started at 115200");
  delay(100);*/
  
  setID();

  delay(1000);
  
  
  

 
  
}

void loop() {
  int BaseDistance1 = 1300;
  int BaseDistance2 = 1300;
  
  
  int j = 0;
  lox1.rangingTest(&measure1, false); 
  lox2.rangingTest(&measure2, false);
  
  int prevCapCount = CapacityCount;
  int L1 = 0;
  int L2 = 0;
  
  if(measure1.RangeStatus != 4) {
      L1 = measure1.RangeMilliMeter;
      if(L1 > 1000) {
        L1 = BaseDistance1;
      }    
    }
  else {
    L1 = BaseDistance1;
   }

  if(measure2.RangeStatus != 4) {
      L2 = measure2.RangeMilliMeter;
      if(L2 > 1000) {
        L2 = BaseDistance2;
      }
   }
   else {
    L2 = BaseDistance2;
   }
  
 // Going out
  if(L1 < BaseDistance1 - 50) {
    j = 0;
    while(j < 20) {
        lox1.rangingTest(&measure1, false);
        lox2.rangingTest(&measure2, false);
        
        if(measure1.RangeStatus != 4) {
          L1 = measure1.RangeMilliMeter;
          if(L1 > 1000  || L1 < 0 ) {
          L1 = BaseDistance1;
         }    
          }
          else {
          L1 = BaseDistance1;
         }    
       if(measure2.RangeStatus != 4) {
          L2 = measure2.RangeMilliMeter;
          if(L2 > 1000 || L2 < 0) {
            L2 = BaseDistance2;
                }
             }
             else {
              L2 = BaseDistance2;
             }
      if(L2 < BaseDistance2 - 50) {
        CapacityCount = CapacityCount - 1;
        delay(500);
        j = 20;
      }
      
      j++;
      delay(10);
    }
  }

  
  //Going in
  else if(L2 < BaseDistance2 - 50) {
    j = 0;
    while(j < 20) {
        lox1.rangingTest(&measure1, false);
        lox2.rangingTest(&measure2, false);
        
        if(measure2.RangeStatus != 4) {
          L2 = measure2.RangeMilliMeter;
          if(L2 > 1000  || L1 < 0 ) {
          L2 = BaseDistance2;
         }    
          }
          else {
          L2 = BaseDistance2;
         }    
       if(measure1.RangeStatus != 4) {
          L1 = measure1.RangeMilliMeter;
          if(L1 > 1000 || L2 < 0) {
            L1 = BaseDistance1;
                }
             }
             else {
              L1 = BaseDistance2;
             }
      if(L1 < BaseDistance1 - 50) {
        CapacityCount = CapacityCount + 1;
        delay(500);
        j = 20;
      }
      
      j++;
      delay(10);
    }
  }
  
  if(Serial.read() == 'R') {
    CapacityCount = 0 ;
  }
  
  if(CapacityCount != prevCapCount) {
   
    Serial.println(F("Capacity : "));
    Serial.println(CapacityCount);
   
  }
  delay(5);
}
