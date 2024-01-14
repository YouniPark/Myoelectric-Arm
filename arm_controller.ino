#include <Servo.h>
#include <movingAvg.h>

// CONSTANTS
#define SIGNAL_PIN A0           // Analog pin for sensor input
#define SERVO_PIN 9             // Digital PWM pin signaling servo motor
#define SERVO_OPEN 45           // Servo angle for open position
#define SERVO_CLOSED 100          // Servo angle for closed position
#define N_SAMPLES 10            // Number of samples for calculating sensor moving average
#define ACTIVATION_DELTA 300    // Activation delta for idle / activated reading

enum class ClawState
{
  OPEN,
  CLOSED
};

// GLOBALS
ClawState state;
Servo servo;
movingAvg samples(N_SAMPLES);
int idleAvg;

// INIT
void setup() {
  state = ClawState::CLOSED;
  servo.attach(SERVO_PIN);
  delay(1000);
  samples.begin();
  for(int i = 0; i < N_SAMPLES; i += 1) samples.reading(analogRead(SIGNAL_PIN));
  idleAvg = 0; //samples.getAvg();
  Serial.begin(9600);
}

// MAIN LOOP
void loop() {
  
  samples.reading(analogRead(SIGNAL_PIN));
  Serial.println(samples.getAvg() - idleAvg);


  switch(state)
  {
    case ClawState::OPEN:
      // STATE
      servo.write(SERVO_OPEN);

      // TRANSITIONS
      if(samples.getAvg() - idleAvg > ACTIVATION_DELTA) state = ClawState::CLOSED;
      break;

    case ClawState::CLOSED:
      // STATE
      servo.write(SERVO_CLOSED);
      
      // TRANSITIONS
      if(samples.getAvg() - idleAvg < ACTIVATION_DELTA) state = ClawState::OPEN;
      break;
  }
}
