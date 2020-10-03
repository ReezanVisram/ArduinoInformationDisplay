#include <dht.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"


#define DHT11_PIN 8


int receiver = 7;
bool isOn = false;
int currFunc = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

IRrecv irrecev(receiver);
RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;

decode_results results;

void translateIR(){
  switch (results.value) {
    case 0xFFA25D: if (isOn){
                    isOn = false; 
                    } else { 
                      isOn = true;
                      displayDateAndTime();
                    } break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK");    break;
    case 0xFF02FD: Serial.println("PAUSE");    break;
    case 0xFFC23D: Serial.println("FAST FORWARD");   break;
    case 0xFFE01F: Serial.println("DOWN");    break;
    case 0xFFA857: Serial.println("VOL-");    break;
    case 0xFF906F: Serial.println("UP");    break;
    case 0xFF9867: Serial.println("EQ");    break;
    case 0xFFB04F: Serial.println("ST/REPT");    break;
    case 0xFF6897: currFunc = 0; if (isOn) {
                                  lcd.clear();
                                  displayDateAndTime();
                                  } break;
    case 0xFF30CF: currFunc = 1; if (isOn) {
                                  lcd.clear();
                                  displayTemperature();
                                 } break;
    case 0xFF18E7: currFunc = 2; if (isOn) {
                                  lcd.clear();
                                  displayHumidity();
                                 } break;
    case 0xFF7A85: currFunc = 3; isOn = false; break;
    case 0xFF10EF: currFunc = 4; isOn = false; break;
    case 0xFF38C7: currFunc = 5; isOn = false; break;
    case 0xFF5AA5: currFunc = 6; isOn = false; break;
    case 0xFF42BD: currFunc = 7; isOn = false; break;
    case 0xFF4AB5: currFunc = 8; isOn = false; break;
    case 0xFF52AD: currFunc = 9; isOn = false; break;
    case 0xFFFFFFFF: Serial.println(" REPEAT");break; 


    
    default:
      Serial.println("Other button");
  }

  delay(500);
}

// The default, Date and Time
void displayDateAndTime() {
    DateTime now = rtc.now();

    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    lcd.setCursor(0, 1);
    lcd.print(now.year(), DEC);
    lcd.print("-");
    if (now.month() < 10) {
      lcd.print("0");
    }
    lcd.print(now.month(), DEC);
    lcd.print("-");
    if (now.day() < 10){
      lcd.print("0");
    }
    lcd.print(now.day(), DEC);

    lcd.print(" ");

    lcd.print(now.hour(), DEC);
    lcd.print(":");
    if (now.minute() < 10){
      lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
}

// Function 1, temperature
void displayTemperature() {
  int chk = DHT.read11(DHT11_PIN);

  lcd.setCursor(0, 0);

  lcd.print("Temperature:");

  lcd.setCursor(0, 1);

  lcd.print(DHT.temperature, 1);
  lcd.print((char)223);
  lcd.print("C");
}

void displayHumidity() {
  int chk = DHT.read11(DHT11_PIN);

  lcd.setCursor(0, 0);

  lcd.print("Humidity:");

  lcd.setCursor(0, 1);

  lcd.print(DHT.humidity, 1);
  lcd.print("%");
}

void setup() {
  Serial.begin(9000);
  delay(3000);
  lcd.begin(16, 2);
  Serial.println("IR Receiver button decode");

  if (!rtc.begin()) {
    Serial.println("Could not find RTC");
    while(1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, setting time");

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  irrecev.enableIRIn();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecev.decode(&results)) {
    translateIR();
    irrecev.resume();
  }
  
  if (!isOn) {
    lcd.clear();
  }
}
