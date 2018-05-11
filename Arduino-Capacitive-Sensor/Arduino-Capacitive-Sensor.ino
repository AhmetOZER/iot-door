#include <CapacitiveSensor.h>

/*
* Contact with https://ahmetozer.org
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 */


CapacitiveSensor   cs_4_5 = CapacitiveSensor(3,5);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired


int r1 = 0;
int r2;
int time1 = 0;
int dokun= 0;
void setup()                    
{
   cs_4_5.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(2, OUTPUT);
   r2 = cs_4_5.capacitiveSensor(30);
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_4_5.capacitiveSensor(30);
    r1 = total1;

    Serial.print(total1);                  // print sensor output 1
    Serial.print("  R2  ");
    Serial.print(r2);
    Serial.println("\t");

//time1 = time1 + 1;
   
  if (r1 - r2 < 300){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(2, HIGH);
        delay(200);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(2, LOW);
    }

/* if (time1 > 100) {
  time1 = 0;
  dokun = 0;
  } 

  if (total1 < 800) {
  dokun = dokun + 1 ;
  } 

if (dokun > 30) {
digitalWrite(LED_BUILTIN, HIGH);
digitalWrite(8, HIGH);
} else {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(8, HIGH);
}

*/
delay(10);                             // arbitrary delay to limit data to serial port 
}
