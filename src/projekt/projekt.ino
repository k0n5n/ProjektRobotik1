/*
  Autonomo

  by Grzegorz Gabryszak
   & Konstantin Braza
  modified 24.02.2021
*/

// Inkludierte Libraries: I2C mit Wire.h und Power Delivery Board mit SparkFun_STUSB5400.h.
#include <Wire.h>
#include <SparkFun_STUSB4500.h>

// Pins für Ultraschallsensor
#define US_TRIGGER 11
#define US_ECHO 12

// Pins für Infrarotsensoren
#define IR_PIN_1 A5
#define IR_PIN_2 A1

// Pins zur Motorsteuerung (PWM und Richtung)
#define PWM1 5
#define DIR1 3 
#define PWM2 6
#define DIR2 4 

STUSB4500 usb;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  
  // Pins zur Motorsteuerung als Output festlegen.
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  // Ultraschall: Trigger-Pin als Output, Echo als Input
  pinMode(US_TRIGGER, OUTPUT);
  pinMode(US_ECHO, INPUT);

  // Pins für Infrarotsensoren als Input festlegen
  pinMode(IR_PIN_1, INPUT);
  pinMode(IR_PIN_2, INPUT);

  // Richtung auf "vorwärts" festlegen.
  digitalWrite(DIR1, 1);
  digitalWrite(DIR2, 1);
  //charge();
}

// Liest die gemessene Distanz von einem Infrarotsensor.
// Existierende sensor_ids: 1, 2
int read_ir_sensor(int sensor_id)
{
  int pin;

  if (sensor_id == 1) {
    pin = IR_PIN_1;
    }
  else {
    pin = IR_PIN_2;
  }
  
  // Einlesen des analogen Signals und Berechnung der Distanz laut 
  float temp_result = analogRead(pin)*0.0048828125;  // Sensorwert * (5/1024)
  int distance = 13*pow(temp_result, -1); // Aus Datenblatt-Graph hergeleitet.

  return distance; 
}

// Konfiguration des USB Power-Delivery Boards
void charge()
{
  // Überprüfen ob PDB angeschlossen ist.
  if(!usb.begin())
    Serial.println("conn failed");
    return;
  else
    Serial.println("success");

  // Konfiguration: Das Power Delivery Board hat 3 Speicherplätze für Konfigurationen, sogenannte PDOs (Power Delivery Objects).
  // Ein PDO enthält Informationen über die Spannung, Stromstärke und Spannungsgrenzen und ist persistent am PDB gespeichert.
  usb.setPdoNumber(2);
  // Spannung auf 13V setzen, damit die 3 Akkus in Serie geladen werden können.
  usb.setVoltage(2, 13);
  // Stromstärke auf 1A.
  usb.setCurrent(2, 1);

  // Unteres und oberes Limit: Jeweils 10% über/unter der festgelegten Spannung.
  usb.setLowerVoltageLimit(2, 10);
  usb.setUpperVoltageLimit(2, 10);

  // Konfiguration persistieren.
  usb.write();
  Serial.println("PDO configured");
}

// Liest die gemessene Distanz von einem Ultraschallsensor.
long read_us_sensor()
{
  // Dauer des Impulses.
  long duration;
  // Berechnete Distanz.
  long distance;

  digitalWrite(US_TRIGGER, LOW);
  delayMicroseconds(2);

  // 10ms Impuls.
  digitalWrite(US_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIGGER, LOW);

  // Messen der Zeit bis Echo zurückkommt.
  duration = pulseIn(US_ECHO, HIGH); 

  // Berechnung der Distanz lt. Datenblatt.
  distance = (duration/3) / 29.1;

  return distance;
}

// Setzt die Geschwindigkeit für einen der beiden Motoren.
// motor_speed ist ein Integer zwischen 0 und 255.
// motor_nr ist ein Integer zwischen 1 und 2.
void set_motor_speed(int motor_speed, int motor_nr)
{
  int pin;
  
  if (motor_nr == 1) {
    pin = PWM2;
  }
  else {
    pin = PWM1;
  }

  analogWrite(pin, motor_speed);
}

// Berechnet die neue Motorgeschwindigkeit anhand des Ergebnisses von einem Infrarotsensor.
int calculate_speed(int ir_result)
{
  int x, speed;
  x=ir_result;

  // Berechnung wenn der Sensorwert valide und zuverlässig ist ("gute" Werte liegen zwischen 3 und 30 Zentimeter).
  if (3<x<30) {
    // Funktion zur Geschwindigkeitsberechnung
    speed=((x*x)/3.5)-3;
  }
  else {
    // Geschwindigkeit auf maximalen Wert setzen.
    speed=255;
  }

  return y;
}

// Ausgabe der aktuellen Daten über die Serielle Schnittstelle zum Testen und Überprüfen der Funktionalität.
void print_serial_log(long us_result, int ir_result_1, int ir_result_2)
{
  Serial.print("Ultraschall: ");
  Serial.print(us_result);
  Serial.print("  ");

  Serial.print("Infrarot 1: ");
  Serial.print(ir_result_1);
  Serial.print("  ");

  Serial.print("Infrarot 2: ");
  Serial.println(ir_result_2);
  Serial.print("  ");
}

// the loop function runs over and over again forever
void loop() {
  int speed_1, speed_2;

  // Einlesen der Sensorwerte.
  long us_result = read_us_sensor();
  int ir_result_1 = read_ir_sensor(1);
  int ir_result_2 = read_ir_sensor(2);

  // Ausgabe der aktuellen Sensorwerte über die serielle Schnittstelle.
  print_serial_log(us_result, ir_result_1, ir_result_2);

  // Berechnung der Motorgeschwindigkeiten, damit Autonomo Hindernissen ausweichen kann.
  speed_1=calculate_speed(ir_result_1);
  speed_2=calculate_speed(ir_result_2);

  // Setzen der Motorgeschwindigkeit
  set_motor_speed(speed_1, 1);
  set_motor_speed(speed_2, 2);

  // Pause, damit Arduino keinen Herzinfarkt bekommt.
  delay(250);
}

