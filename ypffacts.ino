
#include <LovyanGFX.hpp>
#include "lgfx_setup.h"
#include "NotoSansBold36.h"
#include "ypfFacts.h"

#define AA_FONT_LARGE NotoSansBold36

// **********************************************************************************
// i have foolishly been inconsistant with regards to which output i'm using across the installation 
// so check this carefully in code and on the board
// i have added a hard off to the screen connection not being used so that it doesn't work if wrong

// YPFFacts uses the screen 2 connection. 

#define Screen1_CS 21 
#define Screen2_CS 22

#define SCREENOFF 1
#define SCREENON 0

#define BLACK 0x0000
#define ORANGE 0xE4E0
#define SKYBLUE 0x55BD
#define BLUISHGREEN 0x04EE
#define YELLOW 0xF728
#define BLUE 0x0396
#define VERMILLION 0xD2E0
#define REDDISHPURPLE 0xCBD4

LGFX tft;
LGFX_Sprite spr(&tft);

int currentElement = 0;
int lastElement = NUMBER_OF_ELEMENTS;
int textDelay = 500; // delay time between facts
float textSpeed = 2.0; // Iteration speed of loop of text. 1 is slowest.

void setup(void)
{
  tft.init();

  Serial.begin(250000);

  pinMode(Screen1_CS, OUTPUT);
  pinMode(Screen2_CS, OUTPUT);

  tft.begin();

  tft.setRotation(1);

  spr.setColorDepth(16); // 16 bit colour needed to show antialiased fonts

  tft.fillScreen(VERMILLION);
}

void loop(void)
{
  writeScreen(currentElement);

  lastElement = currentElement;
  currentElement = (currentElement + 1) % NUMBER_OF_ELEMENTS;

  Serial.print("element is ");
  Serial.println(currentElement);
    
  delay(textDelay);
}

void writeScreen(int element) {
  int width = tft.width(); // Half the screen width
  int height = tft.height();
  int spriteHeight = 100;
  String factToDisplay = ypfFacts[currentElement];

  spr.loadFont(AA_FONT_LARGE); // Load another different font into the sprite instance

  int sentanceLength = spr.textWidth(factToDisplay); // in pixels

  spr.createSprite(width, spriteHeight);   // Create a sprite 100 pixels wide and 50 high
 
  spr.setTextColor(BLACK, VERMILLION); // Set the font colour and the background colour

  spr.setTextDatum(ML_DATUM); // Middle left datum

  spr.setTextWrap(false);

  digitalWrite(Screen1_CS, SCREENOFF);
  digitalWrite(Screen2_CS, SCREENON);

  spr.createSprite(width, spriteHeight);   // Create a sprite 100 pixels wide and 50 high

  for(int i = width; i > 0 - sentanceLength; i = i - textSpeed) {
    spr.fillSprite(VERMILLION);
    spr.drawString(factToDisplay, i, spriteHeight/2); // Make sure text fits in the Sprite!
    spr.pushSprite(0, height/2 - (spriteHeight/2) + 1);  // ok on this library it doesn't seem to be able to cope with the same number twice with two screens so this is a hack
  } 
  spr.unloadFont(); // Remove the font to recover memory used

  spr.deleteSprite(); // Recover memory
}