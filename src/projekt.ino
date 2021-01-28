/*
  Autonomo

  by Grzegorz Gabryszak
   & Konstantin Braza
  modified 28.01.2021
*/

void setup() 
{
  Serial.begin(9600);
}

// Reads the value from the infrared sensor.
int read_ir_sensor()
{

}

int read_us_sensor(int sensor_id)
{

}

void set_motor_speed(int speed, int motor_nr)
{

}

int calculate_speed(int ir_result, int us_result_4, int us_result_2)
{

}

// the loop function runs over and over again forever
void loop() {
  int ir_result = read_ir_sensor();
  int us_result_1 = read_us_sensor(1);
  int us_result_2 = read_us_sensor(2);

  calculate_speed(ir_result, us_result_1, us_result_2);

}