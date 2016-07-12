/*****************************************************************************
Particle Maker Kit Tutorial #3: PIR Motion Sensor

This tutorial uses a Photon and the PIR motion sensor from the Particle Maker
Kit to determine whether a conference room is in use and post the status to
Slack. To see the full instructions, visit 
http://docs.particle.io/tutorials/topics/maker-kit
******************************************************************************/

int ledPin = D7;                 // choose the pin for the LED
int inputPin = D0;               // choose the PIR sensor pin
bool available;                  // status of conference room
int motionCounter = 0;           // variable to count motion events

Timer timer(30000, determineMotion); // software timer to check every 30s

void setup() {
  pinMode(ledPin, OUTPUT);       // set LED as output
  pinMode(inputPin, INPUT);      // set sensor as input

  timer.start(); // start the determineMotion timer
}

void determineMotion() {    // this function determines if there's motion
    if(motionCounter < 2) { // if very little motion was detected
        if(available == false) { // only publish if the status changed
            Particle.publish("conf_avail"); //publish to conf_avail webhook
            }
        available = true; // set the status to available
    } else if (motionCounter >= 2) {
        if(available == true) { // only publish if the status changed
            Particle.publish("conf_inuse"); //publish to conf_inuse webhook
            }
        available = false; // set the status to in use
    }
    motionCounter = 0; // reset motion counter
}

void loop() {
  if (digitalRead(inputPin) == HIGH) {  // check if the input is HIGH
    digitalWrite(ledPin, HIGH);         // turn LED ON if high
    motionCounter++;                    // increment motion counter
  } else {
    digitalWrite(ledPin, LOW);          // turn LED OFF if no input
  }
  delay(500);                           // wait 0.5s
}
