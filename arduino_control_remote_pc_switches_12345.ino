#include <Servo.h> 

///////////////// motors config////////////


Servo right_thruster;
int right_thruster_pin = 10;
int right_thruster_code = 1;   //used to map the dof over the encrypted message sent via serial from the python code
int right_thruster_min_pwm = 1300.0;
int right_thruster_max_pwm = 1700.0;
int right_thruster_last_command = 0;


Servo left_thruster;
int left_thruster_pin = 9;
int left_thruster_code = 2;
int left_thruster_min_pwm = 1300.0;
int left_thruster_max_pwm = 1700.0;
int left_thruster_last_command = 0;


Servo bow_thruster;
int bow_thruster_pin = 11;
int bow_thruster_code = 3;
int bow_thruster_min_pwm = 1200.0;
int bow_thruster_max_pwm = 1800.0;
int bow_thruster_last_command = 0;


Servo right_servo;
int right_servo_pin = 5;
int right_servo_code = 4;
int right_servo_min_pwm = 500.0;
int right_servo_max_pwm = 2500.0;
int right_servo_last_command = 0;


Servo left_servo;
int left_servo_pin = 6;
int left_servo_code = 5;
int left_servo_min_pwm = 500.0;
int left_servo_max_pwm = 2500.0;
int left_servo_last_command = 0;


float thrusters_command = 0;
//////////////////////////////////////////



/////////// serial communication config ///////////////////////

int message = 0;
int code = 0;
int command_value = 0;
int translated_message[2];  //  [the wanted motor, the wanted command]

//////////////////// remote controller configs /////////////////////////////

int remote_right_thruster_pin = 3;   //need to assign channels to pins   
int remote_right_thruster_max_pwm = 2000;  //need to calibrate those values based on selected channel assigned to control the motor
int remote_right_thruster_min_pwm = 850;  //need to calibrate those values based on selected channel assigned to control the motor

int remote_left_thruster_pin = 3;
int remote_left_thruster_max_pwm = 2000;  //need to calibrate those values based on selected channel assigned to control the motor
int remote_left_thruster_min_pwm = 850;  //need to calibrate those values based on selected channel assigned to control the motor

int remote_bow_thruster_pin = 4;
int remote_bow_thruster_max_pwm = 2000;  //need to calibrate those values based on selected channel assigned to control the motor
int remote_bow_thruster_min_pwm = 850;   //need to calibrate those values based on selected channel assigned to control the motor

int remote_left_servo_pin = 2;
int remote_left_servo_max_pwm = 2000;   //need to calibrate those values based on selected channel assigned to control the motor
int remote_left_servo_min_pwm = 850;    //need to calibrate those values based on selected channel assigned to control the motor

int remote_right_servo_pin = 2;
int remote_right_servo_max_pwm = 2000;   //need to calibrate those values based on selected channel assigned to control the motor
int remote_right_servo_min_pwm = 850;     //need to calibrate those values based on selected channel assigned to control the motor

int remote_on_pin = 8;
int remote_cannon_pin = 6;
int remote_kill_switch_pin = 12;
bool remote_on = false;

float right_thruster_pwm_value = right_thruster_min_pwm + (right_thruster_max_pwm - right_thruster_min_pwm)/2.0;   //this is the middle of the pwm range of the motor in order to start the motor with zero volocity
float bow_thruster_pwm_value = bow_thruster_min_pwm + (bow_thruster_max_pwm - bow_thruster_min_pwm)/2.0;
float left_thruster_pwm_value = left_thruster_min_pwm + (left_thruster_max_pwm - left_thruster_min_pwm)/2.0;
float right_servo_pwm_value = right_servo_min_pwm + (right_servo_max_pwm - right_servo_min_pwm)/2.0;
float left_servo_pwm_value = left_servo_min_pwm + (left_servo_max_pwm - left_servo_min_pwm)/2.0;
float remote_kill_Switch_pwm_value = 1500.0;

int right_thruster_command = 0;
int left_thruster_command = 0;
int bow_thruster_command = 0;
int left_servo_command = 0;
int right_servo_command = 0;

int remote_on_pwm_value = 0;
int remote_cannon_pwm_value = 0;

///////////////////////////////relay configs/////////////////////////////
int kill_switch = 16;
int kill_switch_off_code = 6;  //6000
int kill_switch_on_code = 16;  //16000

int cannon_switch = 14;
int cannon_switch_off_code = 7;  //7000
int cannon_switch_on_code = 17;  //17000

int change_battery_switch = 15;
int change_Battery_switch_off_code = 8; //8000
int change_Battery_switch_on_code = 18;  //18000



////////////////////////////////////////////////////////////////



void setup() {

  
    /////////// serial communication setup ///////////////////////
  Serial.begin(9600);
  Serial.setTimeout(4);
  Serial.flush();
 ////////////////////

 ////////////// relay setup ////////////////
  pinMode(cannon_switch, OUTPUT);
  pinMode(change_battery_switch, OUTPUT);
  pinMode(kill_switch, OUTPUT);
 
 ///////////////// motors setup////////////
 
  right_thruster.attach(right_thruster_pin);
  left_thruster.attach(left_thruster_pin); 
  bow_thruster.attach(bow_thruster_pin); 
  right_servo.attach(right_servo_pin); 
  left_servo.attach(left_servo_pin);



  ////////////////////////////////////////

//////////////// remote controller setup ///////////////////

  pinMode(remote_right_thruster_pin,INPUT);
  pinMode(remote_left_thruster_pin,INPUT);
  pinMode(remote_bow_thruster_pin,INPUT);
  pinMode(remote_left_servo_pin,INPUT);
  pinMode(remote_right_servo_pin,INPUT);
  pinMode(remote_cannon_pin,INPUT);
  pinMode(remote_on_pin, INPUT);
  pinMode(remote_kill_switch_pin,INPUT);
///////////////////////////////////////////////



  /////////////// motor initialise ///////////////
//  right_thruster.writeMicroseconds(1500);
//  delay(7000);
//  left_thruster.writeMicroseconds(1500);
//  delay(7000);
//  bow_thruster.writeMicroseconds(1500);
//  delay(7000);
  /////////////////////////////////////

  Serial.println("im working");
}



void loop() {
  remote_kill_Switch_pwm_value = pulseIn(remote_kill_switch_pin, HIGH);

  if (remote_kill_Switch_pwm_value > 1700) {
//     Serial.println("kill switch on");
     digitalWrite(kill_switch, LOW);  
    }       

  else {
//    Serial.println("kill switch off");
     digitalWrite(kill_switch, HIGH);
     delay(100);  
  } 
  
//  Serial.println(remote_kill_Switch_pwm_value);
  remote_on_pwm_value = pulseIn(remote_on_pin, HIGH);
//  Serial.println(remote_on_pwm_value);
  if (remote_on_pwm_value > 1500) { //this code is based on remote on pwm value is recieved from channel 5 of the remote. this channel is on off switch which sends pwm signal of 850 when off and 2000 when on.
    remote_on = true;
  }
  if (remote_on_pwm_value < 1500) { 
    remote_on = false;
  }
  if (remote_on == false) {
    command_from_pc();
  }
  if (remote_on == true) {
    command_from_remote_controller();
  }
}



void command_from_pc() {
  if (Serial.available() > 0);
  message = Serial.readString().toInt();
  code = message / 1000; 
  command_value = message - code*1000;  // substracting by the code leaves us with just the value 
   
  if (code == right_thruster_code) {
     if (command_value != right_thruster_last_command) {
       right_thruster_last_command == command_value;
       thrusters_command = map(command_value,0,180,right_thruster_min_pwm , right_thruster_max_pwm);  //function that converts degree value to microseconds command
       right_thruster.writeMicroseconds(thrusters_command); 
    }       
  }
  
  if (code == left_thruster_code) {
    if (command_value != left_thruster_last_command) {
       left_thruster_last_command == command_value;
       thrusters_command = map(command_value,0,180,left_thruster_min_pwm , left_thruster_max_pwm);
       left_thruster.writeMicroseconds(thrusters_command); 
    }       
  }
  
  if (code == bow_thruster_code) {
     if (command_value != bow_thruster_last_command) {
       bow_thruster_last_command == command_value;
       thrusters_command = map(command_value,0,180,bow_thruster_min_pwm , bow_thruster_max_pwm);
       bow_thruster.writeMicroseconds(thrusters_command); 
    }       
  }

  
  if (code == right_servo_code) {
     if (command_value != right_servo_last_command) {
       right_servo_last_command == command_value;
       thrusters_command = right_servo_min_pwm + 11.111111*float(command_value);
       right_servo.writeMicroseconds(thrusters_command);  
    }       
  }

  
  if (code == left_servo_code) {
     if (command_value != left_servo_last_command) {
       left_servo_last_command == command_value;
       thrusters_command = left_servo_min_pwm + 11.111111*float(command_value);
       left_servo.writeMicroseconds(thrusters_command); 
    }       
  }


  if (code == kill_switch_on_code) {
     Serial.println("kill switch on");
     digitalWrite(kill_switch, HIGH);  
    }       


  if (code == cannon_switch_on_code) {
     Serial.println("cannon on");
     digitalWrite(cannon_switch, HIGH);  
    }


  if (code == kill_switch_off_code) {
    Serial.println("kill switch off");
     digitalWrite(kill_switch, LOW);  
    }       


  if (code == cannon_switch_off_code) {
    Serial.println("cannon off");
     digitalWrite(cannon_switch, LOW);  
    }

  
}

void command_from_remote_controller() {
//  message = Serial.readString().toInt();
//  code = message / 1000; 
//  command_value = message - code*1000;  // substracting by the code leaves us with just the value   right_thruster_pwm_value = pulseIn(remote_right_thruster_pin, HIGH);
//

  remote_cannon_pwm_value = pulseIn(remote_cannon_pin, HIGH);
  right_thruster_pwm_value = pulseIn(remote_right_thruster_pin, HIGH);
  left_thruster_pwm_value = pulseIn(remote_left_thruster_pin, HIGH);
  bow_thruster_pwm_value = pulseIn(remote_bow_thruster_pin, HIGH);
//  right_servo_pwm_value = pulseIn(remote_right_servo_pin, HIGH);
//  left_servo_pwm_value = pulseIn(remote_left_servo_pin, HIGH);
//
//  Serial.println("right_thruster_pwm_value");
//  Serial.println(right_thruster_pwm_value);
//  
//  Serial.println("left_thruster_command");
//  Serial.println(left_thruster_command);
//  
//  Serial.println("bow_thruster_command");
//  Serial.println(bow_thruster_command);


  right_thruster_command = map(right_thruster_pwm_value, remote_right_thruster_min_pwm, remote_right_thruster_max_pwm, right_thruster_min_pwm, right_thruster_max_pwm);
  left_thruster_command = map(left_thruster_pwm_value, remote_left_thruster_min_pwm, remote_left_thruster_max_pwm, left_thruster_min_pwm, left_thruster_max_pwm);
  bow_thruster_command = map(bow_thruster_pwm_value, remote_bow_thruster_min_pwm, remote_bow_thruster_max_pwm, bow_thruster_min_pwm, bow_thruster_max_pwm);

//  Serial.println("right_thruster_command");
//  Serial.println(right_thruster_command);
//  
//  Serial.println("left_thruster_command");
//  Serial.println(left_thruster_command);
//  
//  Serial.println("bow_thruster_command");
//  Serial.println(bow_thruster_command);
  
  Serial.println(remote_cannon_pwm_value);
  if (remote_cannon_pwm_value > 1100) { //this code is based on remote on pwm value is recieved from channel 5 of the remote. this channel is on off switch which sends pwm signal of 850 when off and 2000 when on.
    digitalWrite(cannon_switch, HIGH);
    Serial.println("high");
  }
  if (remote_cannon_pwm_value < 1700) { 
    digitalWrite(cannon_switch, LOW);
    Serial.println("low");
  }

//  if (left_servo_command != left_servo_last_command) {
//       left_servo_last_command == left_servo_command;
//       left_servo.writeMicroseconds(left_servo_command); 
//    }
//
//    
//  if (right_servo_command != right_servo_last_command) {
//       right_servo_last_command == right_servo_command;
//       right_servo.writeMicroseconds(right_servo_command);  
//    }


  if (left_thruster_command != left_thruster_last_command) {
       left_thruster_last_command == left_thruster_command;
       left_thruster.writeMicroseconds(left_thruster_command); 
    }


  if (bow_thruster_command != bow_thruster_last_command) {
       bow_thruster_last_command == bow_thruster_command;
       bow_thruster.writeMicroseconds(bow_thruster_command);  
    }


  if (right_thruster_command != right_thruster_last_command) {
       right_thruster_last_command == right_thruster_command;
       right_thruster.writeMicroseconds(right_thruster_command); 
    }
  
}
