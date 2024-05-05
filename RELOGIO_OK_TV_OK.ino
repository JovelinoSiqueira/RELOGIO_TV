//start "sketch"
#include <TVout.h>
#include "Wire.h"
//#include <pollserial.h>
 #include <fontALL.h>

TVout TV;
//pollserial pserial;

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Global Variables
long previousMillis = 0;        // will store last time time was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
long interval = 200;
int tvx,tvy,tvradius,x2,y2,x3,y3;

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
return ( (val/16*10) + (val%16) );
}

// Gets the date and time from the ds1307 and prints result
void getDateDs1307()
{
// Reset the register pointer
Wire.beginTransmission(DS1307_I2C_ADDRESS);
Wire.write(0x00);                                     //MODIFICADO AQUI
Wire.endTransmission();

Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

// A few of these need masks because certain bits are control bits                    //AQUI
second     = bcdToDec(Wire.read() & 0x7f);                                            //AQUI
minute     = bcdToDec(Wire.read());                                                   //AQUI
hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm   //AQUI
dayOfWeek  = bcdToDec(Wire.read());                                                   //AQUI
dayOfMonth = bcdToDec(Wire.read());                                                   //AQUI
month      = bcdToDec(Wire.read());                                                   //AQUI
year       = bcdToDec(Wire.read());                                                   //AQUI

}


void printTime()
{
TV.set_cursor(0,0);
TV.print( char( hour/10 + 0x30) );
TV.print( char( hour%10 + 0x30) );
TV.print(":");
TV.print( char(minute/10 + 0x30));
TV.print( char(minute%10 + 0x30));
TV.print(":");
TV.print(char (second/10+0x30));
TV.print(char (second%10+0x30));


TV.set_cursor(8,9);
TV.print(char(dayOfMonth/10+0x30));
TV.print(char(dayOfMonth%10+0x30));
TV.print("/");
TV.print(char(month/10+0x30));
TV.print(char(month%10+0x30));
TV.set_cursor(10,18);
TV.print("20");
TV.print(char(year/10+0x30));
TV.print(char(year%10+0x30));

TV.draw_circle(tvx,tvy,tvradius-5,BLACK,BLACK);

float angle = second*6 ;
angle=(angle/57.29577951) ; //Convert degrees to radians  
x3=(tvx+(sin(angle)*(tvradius-6)));
y3=(tvy-(cos(angle)*(tvradius-6)));
TV.draw_line(tvx,tvy,x3,y3,WHITE);
angle = minute * 6 ;
angle=(angle/57.29577951) ; //Convert degrees to radians  
x3=(tvx+(sin(angle)*(tvradius-11)));
y3=(tvy-(cos(angle)*(tvradius-11)));
TV.draw_line(tvx,tvy,x3,y3,WHITE);
angle = hour * 30 + int((minute / 12) * 6 )   ;
angle=(angle/57.29577951) ; //Convert degrees to radians  
x3=(tvx+(sin(angle)*(tvradius-15)));
y3=(tvy-(cos(angle)*(tvradius-15)));
TV.draw_line(tvx,tvy,x3,y3,WHITE);
}


void setup()  {
Wire.begin();
getDateDs1307() ;
//TV.begin(_NTSC,184,72);
//the circle didn't look very round with 184,72
TV.begin(_NTSC,150,71);
TV.select_font(font6x8);
TV.clear_screen();
TV.println("TV Clock");
tvx= TV.hres()/2;
tvy=TV.vres()/2;
tvradius=TV.vres()/2 ;
//clock face
 TV.draw_circle(tvx,tvy,tvradius,WHITE);
//hour ticks
for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
  float angle = z ;
  angle=(angle/57.29577951) ; //Convert degrees to radians
  x2=(tvx+(sin(angle)*tvradius));
  y2=(tvy-(cos(angle)*tvradius));
  x3=(tvx+(sin(angle)*(tvradius-5)));
  y3=(tvy-(cos(angle)*(tvradius-5)));

  TV.draw_line(x2,y2,x3,y3,WHITE);
}
//TV.set_hbi_hook(pserial.begin(57600));
}

void loop() {
// if (pserial.available()) {
//   TV.print((char)pserial.read());
//  }

// other processing here maybe  

unsigned long currentMillis = millis();

if(currentMillis - previousMillis > interval) {
  // save the last time you printed and updated

  previousMillis = currentMillis;
  TV.delay(1);
  printTime();
  //read RTC
  getDateDs1307() ;
}


}
