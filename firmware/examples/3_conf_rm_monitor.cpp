/*****************************************************************************
Particle Maker Kit Tutorial #3: Slack-Enabled Conference Room Monitor

This tutorial uses a Photon and the PIR motion sensor from the Particle Maker
Kit to determine whether a conference room is in use and post the status to
Slack. To see the full instructions, visit
http://docs.particle.io/tutorials/topics/maker-kit.
******************************************************************************/

int ledPin = D7;                 // choose the pin for the LED
int inputPin = D0;               // choose the PIR sensor pin
int motionCounter = 0;           // variable to count motion events
String status = "test";          // string to hold "available" or "in use"

Timer timer(30000, determineMotion); // software timer to check every 30s

void setup() {
  pinMode(ledPin, OUTPUT);       // set LED as output
  pinMode(inputPin, INPUT);      // set sensor as input

  Particle.publish("conf_avail", status, PRIVATE); // send test publish

  timer.start(); // start the determineMotion timer
}

void determineMotion() {  // this function determines if there's motion.
    // if little or no motion detected and if status has changed:
    if(motionCounter < 2 && status != "available") {
        status = "available";                     // set status to "available",
        Particle.publish("conf_avail", status, PRIVATE);   // then publish to webhook
    } // if motion was detected and status has changed:
    else if (motionCounter >= 2 && status != "in use") {
        status = "in use";                       // set status to "in use",
        Particle.publish("conf_avail", status, PRIVATE); // then publish to webhook
    }
    motionCounter = 0; // reset motion counter
}

void loop() {
  if (digitalRead(inputPin) == HIGH) {  // if the sensor detected motion,
    digitalWrite(ledPin, HIGH);         // turn the LED on
    motionCounter++;                    // increment motion counter
  } else {
    digitalWrite(ledPin, LOW);          // turn LED OFF if no motion
  }
  delay(500);                           // wait 0.5s
}
