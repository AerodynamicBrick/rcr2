//controller "begin polling" pin should be connected to pin 2
//controller "i'm ready to drop" pin should be connected to pin 3
//arduino output, trigger ARRD is on pin 9 (burn baby burn, get my reference?) 
//####################ALL ABOVE NEEDS A PULL DOWN RESITOR, DO NOT FORGET###############
//arduino blinker to confirm function is on pin 1

//inputs from pixhawk
const byte beginPolling = 2; //this may be 2 or 3 for a mini
const byte userReady = 3; //this may be 2 or 3 for a mini

//outputs
const byte arrdOutPin = 9;
const byte pixhawkReturnPin = 4; 

//various init
  boolean pollReady=false;
  boolean hitMins=false;
  boolean hitMaxs=false;
  boolean a0success=false;
  boolean a1success=false;
  boolean a2success=false;
  boolean a3success=false;

void setup() {
  pinMode(1, OUTPUT); //config led blinker. this is visible on the arduino TX led

  //IO
  
  pinMode(beginPolling, INPUT);
  pinMode(userReady, INPUT);
  pinMode(arrdOutPin, OUTPUT);
  pinMode(pixhawkReturnPin, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(beginPolling), pollPots, RISING);
  attachInterrupt(digitalPinToInterrupt(userReady), riseUserReady, RISING);
}

void loop()
{
  //Serial.println("loop");
  //just to confirm functionality:
  digitalWrite(1, HIGH);
  //delay(500);
  //digitalWrite(1, LOW);
  //delay(500);
}

void pollPots() {
  //write code to confirm MAXs and MINs here
  // a0, a1, a2, a3, are the control surfaces




  //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
  //scale this chart as according to the convension above.
  //data per each servo:

  //rudder
  //a0 min, a0 max, tolerenceLow    tolerenceHigh     //wing right
  //   .90     2.18      .2              .2
  int a0min=184;
  int a0max=445;
  int a0tolLow=41;
  int a0tolHigh=41;

  //elevator
  //a1 min, a1 max, tolerenceLow    tolerenceHigh     //wing left
  //   1.17  2.09        .2               .2
  int a1min=239;
  int a1max=427;
  int a1tolLow=41;
  int a1tolHigh=41;

  //right wing
  //a2 min, a2 max, tolerenceLow    tolerenceHigh     //elevator
  //  1.23    2.14      .2                .2
  int a2min=251;
  int a2max=437;
  int a2tolLow=41;
  int a2tolHigh=41;

  //left wing
  //a3 min, a3 max, tolerenceLow    tolerenceHigh     //rudder
  //  1.20   2.15         .2              .2
  int a3min=245;
  int a3max=439;
  int a3tolLow=41;
  int a3tolHigh=41;

  while(true)
  {
    a0success=pollOne(A0,a0max,a0min,a0tolLow,a0tolHigh);
    a1success=pollOne(A1,a1max,a1min,a1tolLow,a1tolHigh);
    a2success=pollOne(A2,a2max,a2min,a2tolLow,a2tolHigh);
    a3success=pollOne(A3,a3max,a3min,a3tolLow,a3tolHigh);
    if(a0success&&a1success&&a2success&&a3success)
    {
      digitalWrite(pixhawkReturnPin, HIGH); //tells pixhawk it's ready to deploy
      while(true)
      {
        if(digitalRead(userReady)==HIGH) //might be redundant but, why chance it?
        {
          BURNBABY();
        }
      }
    }
  }
}
void riseUserReady()
{
  userReady==HIGH;
}
boolean pollOne(int pin, int maximum, int minimum, int tolerenceLow, int tolerenceHigh) {
  while(true)
  {
    int sensorValue = analogRead(pin);
    //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    
    if(sensorValue<=minimum + tolerenceLow){hitMins=true;}
    if(sensorValue>=maximum - tolerenceHigh) {hitMaxs=true;}

    if(hitMins && hitMaxs)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  
}

void BURNBABY()
{
  digitalWrite(arrdOutPin, HIGH);//drops the payload from chute when high..... do we want normal high or normal low?
}
