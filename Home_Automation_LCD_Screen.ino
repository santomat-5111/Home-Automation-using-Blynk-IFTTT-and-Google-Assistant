#include <SFE_BMP180.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SFE_BMP180 pressure;

#define ALTITUDE 1655.0

void setup()
{
  lcd.begin();                      // initialize the lcd 
  lcd.begin();
  Serial.begin(9600);
  Serial.println("REBOOT");
 if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
 Serial.println("BMP180 init fail\n\n");
    while(1); // 
  }
}

void loop()
{
  char status;
  double T,P,p0,a;
   lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("provided altitude:  ");
  lcd.print(ALTITUDE,0);
  lcd.print("m, ");
  lcd.print(ALTITUDE*3.28084,0);
  lcd.print("ft");
  
  status = pressure.startTemperature();
  if (status != 0)
  {
    
    delay(3000);

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      lcd.backlight();
  lcd.setCursor(0,0);
      lcd.print("temperature: ");
      lcd.print(T,2);
      lcd.print("C,       ");
      lcd.print((9.0/5.0)*T+32.0,2);
      lcd.print("F");


      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(4000);

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement: 
          lcd.backlight();
  lcd.setCursor(0,0);
          lcd.print("absolute pressure:  ");
          lcd.print(P,2);
          lcd.print("mb, ");
          lcd.print(P*0.0295333727,2);
          lcd.print("inHg");


          p0 = pressure.sealevel(P,ALTITUDE);
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");


          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(3000);

  lcd.clear();
}
