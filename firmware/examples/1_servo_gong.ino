Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

int pos = 0;    // variable to store the servo position

void setup()
{
    Particle.function("gong", gong); // create a cloud function ("gong") and link it to the gong function below
    myservo.attach(D0);         // attaches the servo on the D0 pin to the servo object
    myservo.write(25);          // move the servo to 25°
    pinMode(D7, OUTPUT);        // set pin D7 to be an output so we can use it to flash the onboard LED
}

// the gong function below has two nearly identical behaviors:
// ring the gong once when the string "now" is received,
// or ring the gong three times when the string "alarm" is received.
int gong(String command)
{
    if(command == "now")        // ring the gong once
    {
        myservo.write(0);       // move servo to 0° - ding!
        digitalWrite(D7, HIGH); // flash the LED to indicate that it should be working
        delay(100);             // wait 100 ms
        myservo.write(25);      // move servo to 25°
        digitalWrite(D7, LOW);  // turn off LED
        return 1;               // tell the Particle cloud that this function completed
    }
    else if(command == "alarm")     // ring the gong 3 times
    {
        for (int i = 0; i < 3; i++)
        {
            myservo.write(0);       // move servo to 0° - ding!
            digitalWrite(D7, HIGH); // flash the LED to indicate that it should be working
            delay(100);             // wait 100 ms
            myservo.write(25);      // move servo to 25°
            digitalWrite(D7, LOW);  // turn off LED
            delay(1000);
        }
        return 2;               // tell the Particle cloud that this function completed
    }
}

void loop()
{
  // empty because we call the gong function via the cloud
}
