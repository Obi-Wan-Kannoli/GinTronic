#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
HX711 scale(5, 6);
#define clk 2
#define dt 3
#define sw 4
#define in1 7
#define in2 8
#define in3 9
#define in4 10
#define in5 11
#define in6 12
volatile boolean TurnDetected;
volatile boolean up;
bool doonce = 0;
char screen = 0;
boolean pumpstate = 0;

void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

void setup() {
  lcd.begin(16, 2);
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  attachInterrupt (0, isr0, RISING);

}

void loop() {
  if (TurnDetected) {
    delay(200);
    if (pumpstate == 1) {
      TurnDetected = false;
    }
    else {
      doonce = 0;
      if (up) {
        screen++;
        if (screen > 2) {
          screen = 2;
        }
      }
      else {
        screen = screen - 1;
        if (screen < 0) {
          screen = 0;
        }
      }
      TurnDetected = false;
    }
  }

  if (digitalRead(sw) == LOW) {
    delay(200);
    pumpstate = !pumpstate;
    doonce = 0;
  }

  if (screen == 0 && doonce == 0) {
    lcd.clear();
    lcd.print("Pump 1");
    lcd.setCursor(0, 1);
    if (pumpstate == 0) {
      lcd.print("OFF");
      digitalWrite(in1, LOW);
    }
    else {
      lcd.print("ON");
      digitalWrite(in1, HIGH);
    }
    doonce = 1;
  }

  if (screen == 1 && doonce == 0) {
    lcd.clear();
    lcd.print("Pump 2");
    lcd.setCursor(0, 1);
    if (pumpstate == 0) {
      lcd.print("OFF");
      digitalWrite(in3, LOW);
    }
    else {
      lcd.print("ON");
      digitalWrite(in3, HIGH);
    }
    doonce = 1;
  }

  if (screen == 2 && doonce == 0) {
    lcd.clear();
    lcd.print("Pump 3");
    lcd.setCursor(0, 1);
    if (pumpstate == 0) {
      lcd.print("OFF");
      digitalWrite(in5, LOW);
    }
    else {
      lcd.print("ON");
      digitalWrite(in5, HIGH);
    }
    doonce = 1;
  }
}

void alloff() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
}


