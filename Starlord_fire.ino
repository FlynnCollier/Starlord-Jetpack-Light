#include <FastLED.h>
#include <ezButton.h>
#define LED_PIN     0
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    20
#define FRAMES_PER_SECOND 60


ezButton button1(1);  // create ezButton object that attach to pin 1;
ezButton button2(2);  // create ezButton object that attach to pin 2;
ezButton button3(3);  // create ezButton object that attach to pin 3;
ezButton button4(4);  // create ezButton object that attach to pin 4;


bool amIturnedOn = true;
bool goWhite = false;
int lightMode = 1;
int BRIGHTNESS = 200; 


bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
 
  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds
  button3.setDebounceTime(50); // set debounce time to 50 milliseconds
  button4.setDebounceTime(50); // set debounce time to 50 milliseconds

}

void loop()
{
  
  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first
  button4.loop(); // MUST call the loop() function first
  


  if (button1.isPressed()){
  amIturnedOn = !amIturnedOn;}

  if (amIturnedOn == false){
  FastLED.clear();
  FastLED.show();
    return;
  }

 if (button2.isPressed()){
  goWhite = !goWhite;
 }
if (goWhite == true){
  fill_solid( leds, NUM_LEDS, CRGB::White);
   FastLED.show();
   return;
}
    
  
  
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());
  

    Fire2012(); // run simulation frame
  

  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}



// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200


void Fire2012()
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
