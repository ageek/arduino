#include <Wire.h>
#include <SPI.h>
#include <SdFat.h> //https://github.com/greiman/SdFat
#include <U8glib.h> //https://github.com/olikraus/u8glib
#include <BME280_MOD-1022.h> //https://github.com/embeddedadventures/BME280
#include <MAX17048.h> //https://github.com/mjbcopland/MAX17048
#include <LowPower.h> //https://github.com/rocketscream/Low-Power
#include <RTClib.h> //https://github.com/mizraith/RTClib
#include <RTC_DS3231.h> //https://github.com/mizraith/RTClib
#define DIGITALIO_NO_INTERRUPT_SAFETY
#define DIGITALIO_NO_MIX_ANALOGWRITE
#include "digitalIOPerformance.h" //https://github.com/mr700/digitalIOPerformance

U8GLIB_64128N u8g(10, 9, 8); //Constructor for U8glib
RTC_DS3231 RTC; // Object for RTC
SdFat SD; //Needed so we can use newer SdFat library
char filename[] = "00000000.CSV"; //Used for converting date to filename
File myFile; //Used for SD card datalogging

void setup()
{
  Wire.begin(); // Start the I2C Interface
  TWBR = 2; // I2C Set To 400Khz On 8Mhz Cpu
  Gauge.open(); //ake sure MAX17048 is there
  Gauge.hibernateThreshold(0xFFFF); //Force MAX17048 into hibernation mode read every 45 seconds)
  pinMode(6, OUTPUT); //Blink status LED on D6 for duration of BME280 measurements
  pinMode(2, INPUT);  // Interrupt Pin From DS3231 RTC
  pinMode(7, OUTPUT); // CS For MicroSD
  SPI.setClockDivider(SPI_CLOCK_DIV2); // Set SPI To 4Mhz On 8Mhz Cpu
  u8g.begin(); // Start LCD Display
  SD.begin(7); // Start MicroSD
  BME280.readCompensationParams(); //Setup BME280 settings
  BME280.writeOversamplingPressure(os1x);  //Setup BME280 settings
  BME280.writeOversamplingTemperature(os1x);  //Setup BME280 settings
  BME280.writeOversamplingHumidity(os1x);  //Setup BME280 settings
  BME280.writeMode(smForced);  //Setup BME280 settings

  PowerUpClock();
  RTC.begin(); // Start DS3231 RTC

  // Set Alarm 1 to XX:XX:00
  Wire.beginTransmission(0x68); // RTC address
  Wire.write(0x07); // Alarm 1 Seconds
  Wire.write(0x00);
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.endTransmission();

  // Enable Alarm 1 Interrupt
  Wire.beginTransmission(0x68);
  Wire.write(0x0E); // Control
  Wire.endTransmission(false);
  if (Wire.requestFrom(0x68, 1) == 1) {
    Wire.beginTransmission(0x68);
    Wire.write(0x0E);
    Wire.write(Wire.read() | 0x05);
  }
  Wire.endTransmission();

}

void loop()
{
  PowerUpClock();

  // Check Alarm 1 Status & reset for next loop
  Wire.beginTransmission(0x68);
  Wire.write(0x0F); // Status
  Wire.endTransmission(false);
  if (Wire.requestFrom(0x68, 1) == 1) {
    uint8_t sreg = Wire.read();
    if (sreg & 0x01) {
      Wire.beginTransmission(0x68);
      Wire.write(0x0F);
      Wire.write(sreg & ~0x01);
      Wire.endTransmission();
    }

    digitalWrite(6, HIGH); //Blink status LED on D6 for duration of BME280 measurements

    BME280.writeMode(smForced);
    while (BME280.isMeasuring());
    BME280.readMeasurements(); // Read newest results from BME280

    digitalWrite(6, LOW); //Blink status LED on D6 for duration of BME280 measurements

    DisplayAndLog();

    PowerDownClock();

    // Lowpower routine
    attachInterrupt(0, wakeUp, FALLING);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(0);
  }

}

void DisplayAndLog(void) {

  float temp, humidity, pressure, HI, dP;
  char buffer[80];

  temp      = (BME280.getTemperature() * 9.0 / 5.0 + 32.0 - 3);
  humidity  = BME280.getHumidity();
  pressure  = BME280.getPressure();
  HI        = (heatIndex(temp, humidity));
  dP        = (dewPointFast(temp, humidity));
  DateTime now = RTC.now();

  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_lucasfont_alternater);
    u8g.setContrast(55);

    u8g.drawFrame(0, 9, 128, 55);
    u8g.drawHLine(1, 27, 126);
    u8g.drawHLine(1, 45, 126);
    u8g.drawVLine(64, 10, 54);

    u8g.setPrintPos(0, 7);
    u8g.print(now.hour(), DEC);
    u8g.print(':');
    if (now.minute() < 10)
    {
      u8g.print('0');
    }
    u8g.print(now.minute());

    u8g.setPrintPos(30, 7);
    u8g.print(now.month(), DEC);
    u8g.print('/');
    u8g.print(now.day(), DEC);
    u8g.print('/');
    u8g.print(now.year(), DEC);

    u8g.setPrintPos(20, 18);
    u8g.print(F("Temp"));
    u8g.setPrintPos(23, 26);
    u8g.print(temp, 0);
    u8g.print(F("'F"));
    u8g.setPrintPos(75, 18);
    u8g.print(F("Humidity"));
    u8g.setPrintPos(88, 26);
    u8g.print(humidity, 0);
    u8g.print(F("%"));
    u8g.setPrintPos(9, 36);
    u8g.print(F("Pressure"));
    u8g.setPrintPos(21, 44);
    u8g.print(pressure * 0.0295333727, 2);
    u8g.setPrintPos(6, 54);
    u8g.print(F("Heat Index"));
    u8g.setPrintPos(21, 62);
    if (temp < 80)
    {
      u8g.print(F("----"));
    }
    else //if (temp > 80 & humidity > 40)
    {
      u8g.print(HI, 0);
      u8g.print(F("'F"));
    }
    u8g.setPrintPos(72, 54);
    u8g.print(F("Dew Point"));
    u8g.setPrintPos(88, 62);
    u8g.print(dP, 0);
    u8g.print(F("'F"));

    u8g.drawBox(100, 1, 5, 7); //Battery Icon
    u8g.drawBox(101, 0, 3, 1); //Battery Icon
    u8g.setPrintPos(107, 8);
    u8g.print(Gauge.soc_int());  // Gets the battery's state of charge
    u8g.print(F("%"));
  } while (u8g.nextPage());

  // MicroSD Datalogging
  if (((now.minute() == 0) || (now.minute() == 15) || (now.minute() == 30) || (now.minute() == 45)) && (now.second() == 0)) {
    getFileName();
    myFile = SD.open(filename, FILE_WRITE);
    myFile.print(now.month());
    myFile.print("-");
    myFile.print(now.day());
    myFile.print("-");
    myFile.print(now.year());
    myFile.print(",");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    if (now.minute() < 10)
    {
      myFile.print('0');
    }
    myFile.print(now.minute());
    myFile.print(",");
    myFile.print(temp, 0);
    myFile.print(",");
    myFile.print(humidity, 0);
    myFile.print(",");
    myFile.print(pressure * 0.0295333727, 2);
    myFile.print(",");
    myFile.print(Gauge.soc_int());
    myFile.print(",");
    myFile.println(Gauge.vcell());
    myFile.close();
  }
}

// Turn RTC power On
void PowerUpClock() {
  digitalWrite (4, HIGH);
  pinMode (4, OUTPUT);
}

// Turn RTC power off
void PowerDownClock() {
  pinMode (4, INPUT);
  digitalWrite (4, LOW);
}

// Get date from RTC and make filename for MicroSD datalogging
void getFileName() {

  DateTime now = RTC.now();
  filename[0] = (now.year() / 1000) % 10 + '0'; //To get 1st digit from year()
  filename[1] = (now.year() / 100) % 10 + '0'; //To get 2nd digit from year()
  filename[2] = (now.year() / 10) % 10 + '0'; //To get 3rd digit from year()
  filename[3] = now.year() % 10 + '0'; //To get 4th digit from year()
  filename[4] = now.month() / 10 + '0'; //To get 1st digit from month()
  filename[5] = now.month() % 10 + '0'; //To get 2nd digit from month()
  filename[6] = now.day() / 10 + '0'; //To get 1st digit from day()
  filename[7] = now.day() % 10 + '0'; //To get 2nd digit from day()
}

// Heat Index and Dew Point calculations
double heatIndex(double temp, double humidity)
{
  double c1 = -42.38, c2 = 2.049, c3 = 10.14, c4 = -0.2248, c5 = -6.838e-3, c6 = -5.482e-2, c7 = 1.228e-3, c8 = 8.528e-4, c9 = -1.99e-6 ;
  double T = temp;
  double R = humidity;
  double T2 = T * T;
  double R2 = R * R;
  double TR = T * R;
  double rv = c1 + c2 * T + c3 * R + c4 * T * R + c5 * T2 + c6 * R2 + c7 * T * TR + c8 * TR * R + c9 * T2 * R2;
  return rv;
}
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}

//Lowpower routine
void wakeUp()
{ // Just a handler for the pin interrupt.
}

