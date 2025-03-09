
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR_1 2
#define IR_2 3
Servo myservo;

int CountIn = 0;
int CountOut = 0;
int spaceAvailable = 4;
int spaceAvailableTotal = 4;
bool sensor1 = false;
bool sensor2 = false;
bool in = false;
bool out = false;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" PARKING SYSTEM ");
  lcd.setCursor(0, 1);
  lcd.print("  LAUNCHING...  ");
  delay (1000);
  lcd.clear();
  lcdUpdate();

  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  myservo.attach(9);
  myservo.write(90);
}

void loop() {
  checkIn();
  checkOut();
  if (spaceAvailable == 0) {
    ParkingFull();
  }
  delay(10);
}

void checkIn() {
  if (digitalRead(IR_1) == LOW && digitalRead(IR_2) == HIGH && in == false && spaceAvailable > 0) {
    myservo.write(0);
    if (sensor1 == false && sensor2 == false) {
      sensor1 = true;
      in = true;
      Serial.println("checkIn 1");
    }
  }
  if (digitalRead(IR_1) == HIGH && digitalRead(IR_2) == LOW) {
    if (sensor1 == true && sensor2 == false) {
      sensor2 = true;
      Serial.println("checkIn 2");
    }
  }
  if (digitalRead(IR_1) == HIGH && digitalRead(IR_2) == HIGH && sensor1 && sensor2 && in) {
    doorClose();
    spaceAvailable--;
    CountIn++;
    lcdUpdate();
    Serial.println(" spaceAvailable--");
    Serial.println(" CountIn++");
  }
}

void checkOut() {
  if (digitalRead(IR_1) == HIGH && digitalRead(IR_2) == LOW && spaceAvailableTotal - spaceAvailable != 0 ) {
    myservo.write(0);
    if (sensor1 == false && sensor2 == false) {
      sensor2 = true;
      out = true;
      Serial.println("checkOut 1");
    }
  }
  if (digitalRead(IR_1) == LOW && digitalRead(IR_2) == HIGH) {
    if (sensor1 == false && sensor2 == true) {
      sensor1 = true;
      Serial.println("checkOut 2");
    }
  }
  if (digitalRead(IR_1) == HIGH && digitalRead(IR_2) == HIGH && sensor1 && sensor2 && out ) {
    doorClose();
    spaceAvailable++;
    CountOut++;
    lcdUpdate();
    Serial.println(" spaceAvailable++");
    Serial.println(" CountOut++");
  }
}

void doorClose() {
  myservo.write(90);
  delay(1000);
  sensor1 = false;
  sensor2 = false;
  in = false;
  out = false;
  lcdUpdate();
}

void ParkingFull() {
  lcd.setCursor (12, 0);
  lcd.print("Full");
}

void lcdUpdate() {
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("Slot Enable:   ");
  lcd.setCursor (13, 0);
  lcd.print(spaceAvailable);
  lcd.setCursor(0 ,1);
  lcd.print("In:");
  lcd.setCursor(3 ,1);
  lcd.print(CountIn);
  lcd.setCursor(7 ,1);
  lcd.print("Out:");
  lcd.setCursor(11 ,1);
  lcd.print(CountOut);

}