/*
  Autonomo

  by Grzegorz Gabryszak
   & Konstantin Braza
  modified 28.01.2021
*/

#define US_TRIGGER 10
#define US_ECHO 12
#define US_LED 9

#define IR_PIN A1

void setup() 
{
  Serial.begin(9600);
  pinMode(US_TRIGGER, OUTPUT);
  pinMode(US_ECHO, INPUT);
  pinMode(US_LED, OUTPUT);

  pinMode(IR_PIN, INPUT);
}

// Reads the current distance from the infrared sensor.
int read_ir_sensor(int sensor_id)
{
  // 5v
  float volts = analogRead(IR_PIN)*0.0048828125;  // value from sensor * (5/1024)
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

void set_motor_speed(int speed, int motor_nr)
{

}

int calculate_speed(int ir_result, int us_result_4, int us_result_2)
{
  return 0;
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
  int ir_result_1 = read_ir_sensor(1);
  //int ir_result_2 = read_ir_sensor(2);

  print_serial_log(us_result, ir_result_1);

  calculate_speed(ir_result_1, us_result, 20);

}
