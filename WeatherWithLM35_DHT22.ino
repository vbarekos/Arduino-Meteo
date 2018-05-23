/* ΜΕΤΕΩΡΟΛΟΓΙΚΟΣ ΣΤΑΘΜΟΣ ΜΕ ARDUINO
 *  2o ΓΥΜΝΑΣΙΟ ΣΑΜΟΥ
 *  
 *  Χρησιμοποιεί:
 *  τον αισθητήρα DHT22 για εξωτερική θερμοκρασία & υγρασία (Digital pin 3)
 *  τον αισθητήρα LM35 για εσωτερική θερμοκρασία (Analog pin 1)
 *  τον αισθητήρα υπερήχων HC-SR04 για τη βροχή - χαλάζι (Digital pins 9, 10)
 *  ένα μικρομοτερ ως ανεμογεννήτρια (Analog pin 2)
 *  
 *  Χρησιμοποιεί τη βιβλιοθήκη για τον DHT-22 από εδώ: 
 *  http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
 */

// Βιβλιοθήκες
#include <dht.h>;
dht DHT;

// Σταθερές
#define AlarmTemp 29
#define Buzzer 2
#define RedLed 13

#define DHT22_PIN 3

#define trigPin 9
#define echoPin 10

// Εσωτερικός αισθητήρας θερμοκρασίας LM35 (μαύρος)
#define TempSnsIn 1

// Ανεμογεννήτρια
#define WindSns 2

// Μεταβλητές
int val_in; // η εσωτερική θερμοκρασία
int wind_val; // μέτρηση ανέμου

float hum;  //Stores humidity value
float temp; //Stores temperature value

long duration;
int distance, initialDistance, currentDistance, i;

void setup()
{
Serial.begin(9600);
pinMode(RedLed, OUTPUT);

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

initialDistance = getDistance();
delay(3000);
}

void loop()
{

// Διάβασε τη θερμοκρασία του εσωτερικού αισθητήρα
val_in = analogRead(TempSnsIn);
float mv = ( val_in/1024.0)*5000;
float TempIn = mv/10; // Εσωτερική θερμοκρασία σε oC.

float farh = (TempIn*9)/5 + 32; // Τύπος μετατροπής Κελσίου σε Φαρεναιτ
currentDistance = getDistance();


// Διάβασε τιμή mV για την ανεμογεννήτρια
wind_val = analogRead(WindSns);
float wind_v = ( wind_val/1024.0)*100;

// Εαν ανιχνεύσει θερμοκρασία μεγαλύτερη από 29oC τότε ανάβει το LED και κάνει ήχο το buzzer
if (TempIn > AlarmTemp) {
  digitalWrite(RedLed, HIGH); // άναψε το κόκκινο LED
  tone(Buzzer, 1000, 500); // παίξε ήχο στο buzzer
  }
  else {
    digitalWrite(RedLed, LOW); // σβήσε το κόκκινο LED
  }

// Εαν ανιχνεύσει απόσταση διαφορετική από αυτή της αρχικοποιήσης σημαίνει ότι έχει παρεμβληθεί
// εμπόδιο όπως πχ βροχή - χαλαζόπτωση οπότε ανάβει το LED και κάνει ήχο το buzzer
if (currentDistance != initialDistance) {
  digitalWrite(RedLed, HIGH); // άναψε το κόκκινο LED
  tone(Buzzer, 1000, 500); // παίξε ήχο στο buzzer
  }
  else {
    digitalWrite(RedLed, LOW); // σβήσε το κόκκινο LED
  }

// Διάβασε τιμή από τον εξωτερικό αισθητήρα
int chk = DHT.read22(DHT22_PIN);
//Read data and store it to variables hum and temp
hum = DHT.humidity;
temp= DHT.temperature;
//Print temp and humidity values to serial monitor
Serial.print("Υγρασία: ");
Serial.print(hum);
Serial.print(" %, Θερμοκρ Εξωτ: ");
Serial.print(temp);
Serial.println(" *C  ");
    
Serial.print("Θερμοκρασία Εντός = ");
Serial.print(TempIn);
Serial.print("*C  ");
Serial.print(farh);
Serial.print("*F");
Serial.println();

Serial.print("Αισθ. Απόστασης χαλάζι/βροχή = ");
Serial.println();
Serial.print("Τρέχουσα απόσταση εκ.  ");
Serial.print(currentDistance);
Serial.println();

Serial.print("Αρχική απόσταση ");
Serial.print(initialDistance);
Serial.println();

Serial.print("Ταχύτητα Ανέμου ");
Serial.print(wind_v);
Serial.println();

Serial.print("=================");
Serial.println();

delay(6000); // Μετρήσεις κάθε 6 δευτερόλεπτα (Καθυστέρηση)

}

// Custom function for the Ultrasonic sensor
long getDistance(){
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
  return distance;
}
