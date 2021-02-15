/*
  Autonomo

  by Grzegorz Gabryszak
   & Konstantin Braza
  modified 28.01.2021
*/
#include <Wire.h>
#include <SparkFun_STUSB4500.h>

STUSB4500 usb;

#define US_TRIGGER 10
#define US_ECHO 12
#define US_LED 9

#define IR_PIN_1 A1
#define IR_PIN_2 A2

#define PWM1 5
#define PWM2 6
#define DIR2 5

int motor1_speed;
int motor2_speed;



void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  
  pinMode(US_TRIGGER, OUTPUT);
  pinMode(US_ECHO, INPUT);
  pinMode(US_LED, OUTPUT);

  pinMode(IR_PIN_1, INPUT);
  pinMode(IR_PIN_2, INPUT);

  
  charge();
}

// Reads the current distance from the infrared sensor.
int read_ir_sensor(int sensor_id)
{
  int pin;
  
  if (sensor_id = 0)
    pin = IR_PIN_1;
  else if (sensor_id = 1)
    pin = IR_PIN_2;
  else
    return -1;
  
  // 5v
  float volts = analogRead(pin)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  
  return distance; 
}

long read_us_sensor()
{
  long duration;
  long distance;

  digitalWrite(US_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIGGER, HIGH);
  
  delayMicroseconds(10);
  digitalWrite(US_TRIGGER, LOW);

  duration = pulseIn(US_ECHO, HIGH); 
  distance = (duration/3) / 29.1;

  return distance;
}

void charge()
{
  if(!usb.begin())
    Serial.println("conn failed");
  else
    Serial.println("success");
  usb.begin();
  usb.setPdoNumber(2);
  usb.setVoltage(2, 4.2);
  usb.setCurrent(2, 1);
  usb.setLowerVoltageLimit(2, 10);
  usb.setUpperVoltageLimit(2, 10);
  usb.write();
  Serial.println("PDO configured");
}

void set_motor_speed(int speed, int motor_nr)
{

}

// Calculates the speed of the motors based on the sensor readings.
void calculate_motor_speed(int ir_result_1, int ir_result_2, int us_result)
{
  if (us_result < 40)
  {
    motor1_speed = divide_integers(motor1_speed, 2);
    motor1_speed = divide_integers(motor2_speed, 2);
  }
  else
  {
    motor1_speed = 255;
    motor2_speed = 255;
  }

  if (ir_result_1 < 20)
    motor1_speed = divide_integers(motor1_speed, 2);

  if (ir_result_2 < 20)
    motor2_speed = divide_integers(motor2_speed, 2);
}


void print_serial_log(long us_result, int ir_result)
{
  Serial.print("Ultraschall: ");
  Serial.print(us_result);
  Serial.print("  ");
  
  Serial.print("Infrarot: ");
  Serial.println(ir_result);
}

// the loop function runs over and over again forever
void loop() {
  long us_result = read_us_sensor();
  int ir_result_1 = read_ir_sensor(0);
  int ir_result_2 = read_ir_sensor(1);

  int motor1_speed;
  int motor2_speed;

  calculate_motor_speed(ir_result_1, ir_result_2, us_result);
  set_motor_speed(motor1_speed, motor2_speed);
  
  print_serial_log(us_result, ir_result_1);

  delay(150);

}
