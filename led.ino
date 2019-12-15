/**
   LED pwm driver
*/

// pwm output on esp8266 ESP-12 board pin D5
//#define LED_PIN1  14  // D5 gpio14
//#define LED_PIN2  12  // D6 gpio12
//#define LED_PIN3  13  // D7 gpio13
//#define LED_PIN4  4   // D2 gpio04

// NodeMCU "v3" pins around 3V, GND.
#define LED_PIN1   0  // D3 gpio0
#define LED_PIN2   2  // D4 gpio2  also internal blue LED (active low)
#define LED_PIN3  14  // D5 gpio14
#define LED_PIN4  12  // D6 gpio12

// NOTE: gpio15 must be LOW at boot; not using that any more, caused problems.
// NOTE: gpio6-11 used for SPI FLASH chip (not available to use)

//Ticker ledTicker[LED_CHAN_MAX];
//int ledIndex, ledProgram;
int bright_pct; // percent on:  100=bright -- 0=off
int ledPin[LED_CHAN_MAX] = { LED_PIN1, LED_PIN2, LED_PIN3, LED_PIN4 };

#define LED_PROGRAM_MAX  2
#define LED_EVENT_MAX  10

// define LED blink programs with multiple 'event' steps (pwm bright, msec duration)
int ledSchedule[LED_PROGRAM_MAX][LED_EVENT_MAX][2] = {
  {
    {0,   1200},
    {200,  300},
    {0,   2100},
    {50,   100},
    {0,   3500},
    {1000, 100},
    {0,   2200},
    {50,   200},
    {0,   2700},
    {50,   100},
  },
  {
    {0,   2900},
    {200,  500},
    {500,  300},
    {800,  200},
    {1023, 100},
    {800,  200},
    {500,  300},
    {200,  500},
    {0,   3700},
    {1023,  10},
  }
};

void ledPulse(int chan) {
  int ledProgram = chans[chan].ledProgram;
  int ledIndex = chans[chan].ledIndex;
  int pwm = map (bright_pct, 0, 100, 0, ledSchedule[ledProgram][ledIndex][0]);
  int ms = ledSchedule[ledProgram][ledIndex][1] + random(100);
  analogWrite(ledPin[chan], pwm );
  chans[chan].ledTicker.once_ms_scheduled( ms , std::bind(ledPulse, chan));
  if (++ledIndex >= LED_EVENT_MAX)
    ledIndex = 0;
  chans[chan].ledIndex = ledIndex;
}


void led_setup() {

  for (int c = 0; c < LED_CHAN_MAX; c++) {
    pinMode(ledPin[c], OUTPUT);
    chans[c].ledPin = ledPin[c];
    chans[c].ledIndex = random(0, LED_EVENT_MAX - 1);
    chans[c].ledProgram = random(0, LED_PROGRAM_MAX - 1);
    //chans[c].bright_pct = 100;
    chans[c].ledTicker.once_ms_scheduled( random(2000, 4000) + ledSchedule[0][0][1], std::bind(ledPulse, c) );
  }

  bright_pct = 10; // TODO make html slider, values: 0 - 100
  bright_pct = 50; // TODO make html slider, values: 0 - 100
  // TODO: make html selector for ledProgram
  Serial.println("led setup done");
}


void led_shutdown() {

  for (int c = 0; c < LED_CHAN_MAX; c++) {
    chans[c].ledTicker.detach();
  }
}


void led_update() { }
