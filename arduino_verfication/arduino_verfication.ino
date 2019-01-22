//controller "begin polling" pin should be connected to pin 2
//controller "i'm ready to drop" pin should be connected to pin 3
//arduino output, trigger ARRD is on pin 9 (burn baby burn, get my reference?) 
//####################ALL ABOVE NEEDS A PULL DOWN RESITOR, DO NOT FORGET###############
//arduino blinker to confirm function is on pin 1

//inputs from pixhawk
const byte beginPollingPin = 2; //this may be 2 or 3 for a mini
const byte userReadyPin = 3; //this may be 2 or 3 for a mini

//outputs
const byte arrdOutPin = 9;
const byte pixhawkReturnPin = 4; 

//various init
  boolean userReady=false;
  boolean pollReady=false;
  
  boolean a0successMax=false;
  boolean a1successMax=false;
  boolean a2successMax=false;
  boolean a3successMax=false;
  boolean a0successMin=false;
  boolean a1successMin=false;
  boolean a2successMin=false;
  boolean a3successMin=false;

  boolean oldPoll=false;
  boolean oldReady=false;

void setup() {
  pinMode(0, OUTPUT); //config led blinker. this is visible on the arduino TX led
  
  //IO
  
  pinMode(beginPollingPin, INPUT);
  pinMode(userReadyPin, INPUT);
  
  pinMode(arrdOutPin, OUTPUT);
  digitalWrite(arrdOutPin, LOW);
  
  pinMode(pixhawkReturnPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setup complete.");
}

void loop()
{
  if(oldPoll!=digitalRead(beginPollingPin) && !pollReady)
  {
    oldPoll=!oldPoll;
    Serial.println("Begining polling.");
    pollPots();
  }
  if(oldReady!=digitalRead(userReadyPin))
  {
  oldReady=!oldReady; //detects change
  userReady=digitalRead(userReadyPin);
  if(userReady)
  {
    Serial.println("User has signaled readyness.");
  }
  else
  {
    Serial.println("User has unsignaled readyness.");
  }
  }
  
  Serial.println("Loop.");
  //just to confirm functionality:
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  delay(500);

  if(userReady && pollReady){BURNBABY();}
}

void pollPots()
{
  if(!digitalRead(beginPollingPin))
  {
    return;
  }
  Serial.println("Polling Pots.");
  
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
    if(oldReady!=digitalRead(userReadyPin))
    {
    oldReady=!oldReady; //detects change
    userReady=digitalRead(userReadyPin);
    if(userReady)
    {
      Serial.println("User has signaled readyness.");
    }
    else
    {
      Serial.println("User has unsignaled readyness.");
    }
    }
    a0successMax=pollOneMax(A0,a0max,a0min,a0tolLow,a0tolHigh);
    a1successMax=pollOneMax(A1,a1max,a1min,a1tolLow,a1tolHigh);
    a2successMax=pollOneMax(A2,a2max,a2min,a2tolLow,a2tolHigh);
    a3successMax=pollOneMax(A3,a3max,a3min,a3tolLow,a3tolHigh);
    
    a0successMin=pollOneMin(A0,a0max,a0min,a0tolLow,a0tolHigh);
    a1successMin=pollOneMin(A1,a1max,a1min,a1tolLow,a1tolHigh);
    a2successMin=pollOneMin(A2,a2max,a2min,a2tolLow,a2tolHigh);
    a3successMin=pollOneMin(A3,a3max,a3min,a3tolLow,a3tolHigh);

    
    if(a0successMax&&a1successMax&&a2successMax&&a3successMax&&a0successMin&&a1successMin&&a2successMin&&a3successMin)
    {
      digitalWrite(pixhawkReturnPin, HIGH); //tells pixhawk it's ready to deploy
      Serial.println("All pots ready.");
      pollReady=true;
      return;
    }
    
  }
  Serial.println("1");
}
boolean pollOneMax(int pin, int maximum, int minimum, int tolerenceLow, int tolerenceHigh)
{ 
  int sensorValue = analogRead(pin);
  
  //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
  Serial.print(sensorValue>=(maximum + tolerenceHigh));Serial.print(sensorValue);Serial.print(" ");Serial.println(maximum - tolerenceHigh);
  if(sensorValue<=(minimum + tolerenceLow)){return(true);}else{return(false);}
}
boolean pollOneMin(int pin, int maximum, int minimum, int tolerenceLow, int tolerenceHigh)
{
  int sensorValue = analogRead(pin);
  //0.0049 volts (4.9 mV) per unit according to https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
  Serial.print(sensorValue>=(minimum - tolerenceHigh));Serial.print(sensorValue);Serial.print(" ");Serial.println(maximum - tolerenceHigh);
  if(sensorValue>=(minimum - tolerenceHigh)) {return(true);}else{return(false);}
}

void BURNBABY()
{
  Serial.print("Droping payload.");
  digitalWrite(arrdOutPin, HIGH);//drops the payload from chute when high..... do we want normal high or normal low?
}
