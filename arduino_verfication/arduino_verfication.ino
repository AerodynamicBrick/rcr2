//controller "begin polling" pin should be connected to pin 2
//controller "i'm ready to drop" pin should be connected to pin 3
//arduino output, trigger ARRD is on pin 9 (burn baby burn, get my reference?) 
####################ALL ABOVE NEEDS A PULL DOWN RESITOR, DO NOT FORGET###############
//arduino blinker to confirm function is on pin 1

//inputs from pixhawk
const byte beginPolling = 2; //this may be 2 or 3 for a mini
const byte userReady = 3; //this may be 2 or 3 for a mini

//outputs
volatile byte arrdOut = LOW; //drops the payload from chute when high..... do we want normal high or normal low?
volatile byte pixhawkConfirmOut = LOW; //notifys user when arduino is ready to go

void setup() {
  pinMode(1, OUTPUT); //config led blinker
  pinMode(beginPolling, INPUT);
  pinMode(userReady, INPUT);
  attachInterrupt(digitalPinToInterrupt(beginPolling), pollPots, RISING);
  attachInterrupt(digitalPinToInterrupt(userReady), pollPots, RISING);
}

void loop() {
  if(userReady && pollReady){BURNBABY();}
  
  //just to confirm functionality:
  digitalWrite(1, HIGH);
  delay(500);
  digitalWrite(1, LOW);
  delay(500);
}

void pollPots() {
  //write code to confirm MAXs and MINs here
  // a0, a1, a2, a3, are the control surfaces

  //data per each servo:
  //a0 min, a0 max, tolerence         //wing right
  //   #     #         #
  //a1 min, a1 max, tolerence         //wing left
  //   #     #         #
  //a2 min, a2 max, tolerence         //elevator
  //   #     #         #
  //a3 min, a3 max, tolerence         //rudder
  //   #     #         #

  
}

pollOne(int maximum, int minimum) {
  
}

void BURNBABY() {
  digitalWrite(9, HIGH);//this deploys the chute be CAREFUL
}
