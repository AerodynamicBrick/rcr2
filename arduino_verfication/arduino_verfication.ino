//controller "begin polling" pin should be connected to pin 2
//controller "i'm ready to drop" pin should be connected to pin 3
//arduino output, trigger ARRD is on pin 9 (burn baby burn, get my reference?) 
//####################ALL ABOVE NEEDS A PULL DOWN RESITOR, DO NOT FORGET###############
//arduino blinker to confirm function is on pin 1

//inputs from pixhawk
const byte beginPolling = 2; //this may be 2 or 3 for a mini
const byte userReady = 3; //this may be 2 or 3 for a mini

//outputs
volatile byte arrdOut = LOW; //drops the payload from chute when high..... do we want normal high or normal low?
volatile byte pixhawkConfirmOut = LOW; //notifys user when arduino is ready to go

//various init
  boolean pollReady=false;
  boolean hitMins=false;
  boolean hitMaxs=false;
  boolean a0success=false;
  boolean a1success=false;
  boolean a2success=false;
  boolean a3success=false;

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




  //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
  //scale this chart as according to the convension above.
  //data per each servo:
  //a0 min, a0 max, tolerenceLow    tolerenceHigh     //wing right
  //   #     #         #                  #
  int a0min=69;
  int a0max=420;
  int a0tolLow=69;
  int a0tolHigh=69;
  //a1 min, a1 max, tolerenceLow    tolerenceHigh     //wing left
  //   #     #         #                  #
  int a1min=69;
  int a1max=420;
  int a1tolLow=69;
  int a1tolHigh=69;
  //a2 min, a2 max, tolerenceLow    tolerenceHigh     //elevator
  //   #     #         #                  #
  int a2min=69;
  int a2max=420;
  int a2tolLow=69;
  int a2tolHigh=69;
  //a3 min, a3 max, tolerenceLow    tolerenceHigh     //rudder
  //   #     #         #                  #
  int a3min=69;
  int a3max=420;
  int a3tolLow=69;
  int a3tolHigh=69;

  while(true)
  {
    a0success=pollOne(A0,a0max,a0min,a0tolLow,a0tolHigh);
    a1success=pollOne(A1,a1max,a1min,a1tolLow,a1tolHigh);
    a2success=pollOne(A2,a2max,a2min,a2tolLow,a2tolHigh);
    a3success=pollOne(A3,a3max,a3min,a3tolLow,a3tolHigh);
    if(a0success&&a1success&&a2success&&a3success)
    {
      BURNBABY();
    }
  }
}

boolean pollOne(int pin, int maximum, int minimum, int tolerenceLow, int tolerenceHigh) {
  while(true)
  {
    int sensorValue = analogRead(pin);
    //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    
    if(sensorValue<=minimum + tolerenceLow){hitMins=true;}
    if(sensorValue>=maximum - tolerenceHigh){hitMaxs=true;}

    if(hitMins && hitMaxs){return true;}
    else{return false;}
  }
  
}

void BURNBABY() {
  digitalWrite(9, HIGH);//this deploys the chute be CAREFUL
}
