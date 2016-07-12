/*****************************************************************************
Particle Maker Kit Tutorial #2: Next Bus Alert

This tutorial uses a Particle Photon and the OLED screen from the Particle
Maker Kit. It uses a webhook to retrieve bus prediction times from the
NextBus Public XML feed, which must be set up first along with the webhook.
See http://docs.particle.io/tutorials/topics/maker-kit to learn how!
NOTE: This code example requires the Adafruit_SSD1306 library to be included,
so make sure to add it via the Libraries tab in the left sidebar.
******************************************************************************/

// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

String busName = "57"; // name of your bus -- FILL THIS IN!
int leadTime = 5; // #minutes you need to get to your bus -- FILL THIS IN!
int soonestBusTime = 99; // #minutes until next bus (99 is a placeholder)
int nextSoonestBusTime = 88; // #minutes until bus after next (88 is a placeholder)
String soonestStr, nextSoonestStr; // strings for parsing
int gaveWarning = false; // variable to make sure we don't give bus warning too often
int  x, minX; // variables for scrolling code

void getBusTimes() {
    // publish the event that will trigger our Webhook
    Particle.publish("get_nextbus");
}

// create a software timer to get new prediction times every minute
Timer timer(60000, getBusTimes);

void setup()   {
  // start the data retrieval timer
  timer.start();

  //retrieve the webhook data and send it to the gotNextBusData function
  Particle.subscribe("hook-response/get_nextbus/0", gotNextBusData, MY_DEVICES);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(7);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false); // turn off text wrapping so we can do scrolling
  x    = display.width(); // set scrolling frame to display width
  minX = -1500; // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}

void loop() {

  // this code displays the next bus times on the OLED screen with fancy scrolling
  display.clearDisplay();
  display.setCursor(x/2, 7);
  display.print(busName);
  display.print(" in ");
  display.print(soonestBusTime);
  display.print(", ");
  display.print(nextSoonestBusTime);
  display.print(" min   ");
  display.display();
  if(--x < minX) x = display.width()*2;

  // give a "time to leave!" beeping warning, but only once per bus
  if(soonestBusTime <= leadTime && gaveWarning == false)
  {
      for (int i=0; i<3; i++)
      {
        tone(D0, 5000, 100);
        delay(200);
      }
      gaveWarning = true;
  }

  // reset the beeping warning flag so we can warn about the next bus
  else if(soonestBusTime > leadTime && gaveWarning == true)
  {
      gaveWarning = false;
  }
}

// This function will get called when NextBus webhook data comes in.
// It turns the full NextBus XML page into numbers to be displayed on the screen
void gotNextBusData(const char *name, const char *data) {

    // put the incoming data (the XML page) into a string called "str"
    String str = String(data);

    // send str to the tryExtractString function, looking for the first instance (0) of "minutes=\""
    soonestStr = tryExtractString(0, str, "minutes=\"", "\"");
    // turn the extracted bus time into an integer and store it in soonestBusTime
    soonestBusTime = soonestStr.toInt();

    // send str to the tryExtractString function, looking for the second instance (1) of "minutes=\""
    nextSoonestStr = tryExtractString(1, str, "minutes=\"", "\"");
    // turn the extracted bus time into an integer and store it in nextSoonestBusTime
    nextSoonestBusTime = nextSoonestStr.toInt();
}

// this function gets called by gotNextBusData to extract the bus times from the NextBus XML page
String tryExtractString(int matchNum, String str, const char* start, const char* end) {
    if (str == NULL) {
        return NULL;
    }

    int count = 0;
    int lastIdx = 0;

    while (count <= matchNum) {
        int idx = str.indexOf(start, lastIdx);

        int endIdx = str.indexOf(end, lastIdx + idx + strlen(start));

        lastIdx = endIdx;

        if (count == matchNum) {
            return str.substring(idx + strlen(start), endIdx);
        }
        count++;
    }
}
