//  copyleft
//  ed bear
//  Mar 2018
//
//  this file works with the Hardware Class #4 slides for SFPC spring 2018
//  

//this is "C"
//#define USE_PREPROCESSOR_PINS
#ifdef USE_PREPROCESSOR_PINS
  //why would I not want to make these const variables?
  //define the arduino pins 
  #define ADDER_INA_PIN   13
  #define ADDER_INB_PIN   12
  #define ADDER_OUT1_PIN  11
  #define ADDER_OUT2_PIN  10
  
  #define LATCH_S_PIN     9
  #define LATCH_R_PIN     8
  #define LATCH_OUT_PIN   7
  
  #define CLK_OUT_PIN     6
  //analog inputs are not counted on the same numbering as digital pins
  #define CLK_ANA_IN      0
#else
  //when would I want to use a variable?
  //define the arduino pin variables 
  const char ADDER_INA_PIN  = 13;
  const int ADDER_INB_PIN  = 12;
  const uint8_t ADDER_OUT1_PIN = 11;
  const byte ADDER_OUT2_PIN = 10;
  
  const uint8_t LATCH_S_PIN = 9;
  const uint8_t LATCH_R_PIN = 8;
  const byte LATCH_OUT_PIN = 7;
  
  const byte CLK_OUT_PIN = 6;
  //analog inputs are not counted on the same numbering as digital pins
  const byte CLK_ANA_IN = 0;
  //let's declare a "state variable" for the clock
  bool toggle = false;
#endif

void setup() {
  // put your setup code here, to run once:
  //setup the ADDER pins
  pinMode(ADDER_INA_PIN, INPUT);
  pinMode(ADDER_INB_PIN, INPUT);
  //why can't inputs be outputs or vice versa? cause Arduino says so (last I checked)
  pinMode(ADDER_OUT1_PIN, OUTPUT);
  pinMode(ADDER_OUT2_PIN, OUTPUT);

  //setup the CLK pins
  pinMode(CLK_OUT_PIN, OUTPUT);
  //analog inputs do not need a "setup" function

  //setup the Latch pins
  pinMode(LATCH_S_PIN, INPUT);
  pinMode(LATCH_R_PIN, INPUT);
  pinMode(LATCH_OUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //the most basic clock emulation
  //analogRead returns a value between 0 and 1023
  uint16_t potValue = analogRead(CLK_ANA_IN);
//two different clock outputs
#ifdef USE_PREPROCESSOR_PINS        //this code runs...
  delay(potValue/2);
  digitalWrite(CLK_OUT_PIN, LOW);
  delay(potValue/2);
  digitalWrite(CLK_OUT_PIN, HIGH);
#else                               //or this code runs
  //delay takes a value in milliseconds
  delay(potValue);
  toggle = !toggle;
  digitalWrite(CLK_OUT_PIN, toggle);
#endif

  //the most basic SR latch code:
  //check the reset pin latch state
  if (digitalRead(LATCH_R_PIN)) {
  //if the reset pin latch state is high then check the set pin
   if (digitalRead(LATCH_S_PIN)) {
    //if the set pin is also high then write a 1 to the output
    digitalWrite(LATCH_OUT_PIN, HIGH);
   }
  } else {
  //other wise the reset pin is low and the output should dynamically echo the set pin input
   digitalWrite(LATCH_OUT_PIN, digitalRead(LATCH_S_PIN));
  }

  //the most basic adder emulation - this is pedantic code only
  if (digitalRead(ADDER_INA_PIN) && digitalRead(ADDER_INB_PIN)) {
    digitalWrite(ADDER_OUT1_PIN, LOW);
    digitalWrite(ADDER_OUT2_PIN, HIGH);    
  } else if (digitalRead(ADDER_INA_PIN) || digitalRead(ADDER_INB_PIN)) {
    digitalWrite(ADDER_OUT1_PIN, HIGH);
    digitalWrite(ADDER_OUT2_PIN, HIGH);    
  } else {
    digitalWrite(ADDER_OUT1_PIN, LOW);
    digitalWrite(ADDER_OUT2_PIN, LOW);
  }
  //the above code can be reduced to a single line if we break out of Arduino's sandbox... if we are careful 
  // but sticking inside Arduino's API we can do a two line solution with BITWISE logic:
  //bitwise (not logical) AND
  digitalWrite(ADDER_OUT2_PIN, digitalRead(ADDER_INA_PIN) & digitalRead(ADDER_INB_PIN));
  //biwise (not logical) XOR
  digitalWrite(ADDER_OUT1_PIN, digitalRead(ADDER_INA_PIN) ^ digitalRead(ADDER_INB_PIN));
  
  //why won't these examples work smoothly all running together in the loop() function?
  //How can we interleave these three subroutines into a "tight loop" that does where each subroutine interferes
  // with the others?
}
