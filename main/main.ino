#include <LiquidCrystal.h>
#include <FastLED.h>

// INITIALIZE LCD
LiquidCrystal LCD(5, 6, 7, 8, 9, 10);

// BUTTON PINS
#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4

// LED STRIP PINS
#define STRIP_1 11
#define STRIP_2 12
#define STRIP_3 13

// LED AMOUNT ON EACH STRIP
#define NUM_LEDS 10

// Button variables
int pressed1 = 0;
int pressed2 = 0;
int pressed3 = 0;

// Colour variables
int red = 0;
int green = 255;
int blue = 200;

// Note maps
int strip1[200] = {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0};
int strip2[200] = {0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int strip3[200] = {0,0,0,0,0,3,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,3,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,3,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,3,0,0,3,0,0,0,0,3,0,3,3,3,0,3,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,3,0,3,3,0,0,0,0,0,0,3,3,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,0,3,0,0,3,3,0,3,0,3,3,0,3,0,0,3,3,3,0,0,0,0,3,0,3,0,0,0,0,0,0,3,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0};
 
// Game variables
int score = 0;
int combo = 0;

// Current 10 notes to focus on and display initialized as blank
int strip1Focus[10];
int strip2Focus[10];
int strip3Focus[10];

// LED strip arrays
CRGB strip1LED[NUM_LEDS];
CRGB strip2LED[NUM_LEDS];
CRGB strip3LED[NUM_LEDS];


void setup() {
  Serial.begin(9600);

  // Initialize buttons
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  // Initialize LCD
  LCD.begin(16, 2);
  LCD.setCursor(0, 0);

  // Initialize LED strips
  FastLED.addLeds<WS2812, STRIP_1, GRB>(strip1LED, NUM_LEDS);
  FastLED.addLeds<WS2812, STRIP_2, GRB>(strip2LED, NUM_LEDS);
  FastLED.addLeds<WS2812, STRIP_3, GRB>(strip3LED, NUM_LEDS);
}


void startPanel() {
  // Display starting screen message
  LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0, 1);
  LCD.print("                ");
  LCD.setCursor(0, 0);
  LCD.print("PUSH TO START:");

  // Reset button presses
  pressed1 = 0;
  pressed2 = 0;
  pressed3 = 0;

  // Wait until user presses any button before starting game
  while (!(pressed1 || pressed2 || pressed3)) {
    pressed1 = digitalRead(BUTTON_1);
    pressed2 = digitalRead(BUTTON_2);
    pressed3 = digitalRead(BUTTON_3);
    delay(50);
  }

  // Send message to processing code using serial
  Serial.print("start\n");
}


void displayNotes() {
  // Loop through each LED and display notes
  for (int i = 0; i < NUM_LEDS; i++) {
    // Display notes for each LED strip
    if (strip1Focus[i]) {
      strip1LED[i] = CRGB(red, green, blue);
    } else {
      strip1LED[i] = CRGB(3, 3, 3);
      strip1LED[0] = CRGB(0, 0, 0);
    }

    if (strip2Focus[i]) {
      strip2LED[i] = CRGB(red, green, blue);
    } else {
      strip2LED[i] = CRGB(3, 3, 3);
      strip2LED[0] = CRGB(0, 0, 0);
    }

    if (strip3Focus[i]) {
      strip3LED[i] = CRGB(red, green, blue);
    } else {
      strip3LED[i] = CRGB(3, 3, 3);
      strip3LED[0] = CRGB(0, 0, 0);
    }
  }

  // Display notes
  FastLED.show();
}


void game() {
  // Reset score and combo and streak
  score = 0;
  combo = 0;

  // Reset button presses
  pressed1 = 0;
  pressed2 = 0;
  pressed3 = 0;

  // Current index of the array to focus on
  int index = 0;

  // Main game loop
  for (int i = 0; i < 190; i++) {
    // Update score and combo
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0, 0);
    LCD.print("SCORE: ");
    LCD.print(score);
    LCD.setCursor(0, 1);
    LCD.print("                ");
    LCD.setCursor(0, 1);
    LCD.print("COMBO: ");
    LCD.print(combo);

    // Update the notes displaying on the LED strips
    for (int j = 0; j < 10; j++) {
      strip1Focus[j] = strip1[i + j];
      strip2Focus[j] = strip2[i + j];
      strip3Focus[j] = strip3[i + j];
    }

    // Call method to update LED strip notes
    displayNotes();

    // Reset button presses
    pressed1 = 0;
    pressed2 = 0;
    pressed3 = 0;

    // Read user input on whether buttons are pressed
    for (int n = 0; n < 40; n++) {
      pressed1 = pressed1 || digitalRead(BUTTON_1);
      pressed2 = pressed2 || digitalRead(BUTTON_2);
      pressed3 = pressed3 || digitalRead(BUTTON_3);
      delay(5);
    }

    // Calculate score and update LED strips based on user input
    // Strip 1
    if (pressed1 == HIGH && strip1Focus[0] != 0) {
      // Increase score and combo
      score += (5 + combo);
      combo++;
      strip1LED[0] = CRGB(0, 255, 0);
    } else if (pressed1 == LOW && strip1Focus[0] == 0) {
      // No penalty
      strip1LED[0] = CRGB(0, 0, 0);
    } else {
      // Missed: decrease score, reset combo, display red
      strip1LED[0] = CRGB(255, 0, 0);
      score--;
      combo = 0;
    }
    // Strip 2
    if (pressed2 == HIGH && strip2Focus[0] != 0) {
      // Increase score and combo
      score += (5 + combo);
      combo++;
      strip2LED[0] = CRGB(0, 255, 0);
    } else if (pressed2 == LOW && strip2Focus[0] == 0) {
      // No penalty
      strip2LED[0] = CRGB(0, 0, 0);
    } else {
      // Missed: decrease score, reset combo, display red
      strip2LED[0] = CRGB(255, 0, 0);
      score--;
      combo = 0;
    }
    // Strip 3
    if (pressed3 == HIGH && strip3Focus[0] != 0) {
      // Increase score and combo
      score += (5 + combo);
      combo++;
      strip3LED[0] = CRGB(0, 255, 0);
    } else if (pressed3 == LOW && strip3Focus[0] == 0) {
      // No penalty
      strip3LED[0] = CRGB(0, 0, 0);
    } else {
      // Missed: decrease score, reset combo, display red
      strip3LED[0] = CRGB(255, 0, 0);
      score--;
      combo = 0;
    }
    
    // Reset score to 0 if score is negative
    if (score < 0) {
      score = 0;
    }

    // Show updated notes
    FastLED.show();

    delay(10);
  }
}


void endPanel() {
  // Send message to processing using serial
  Serial.print("end\n");
  
  // Clear notes
  for (int i = 0; i < NUM_LEDS; i++) {
    strip1LED[i] = CRGB(0, 0, 0);
    strip2LED[i] = CRGB(0, 0, 0);
    strip3LED[i] = CRGB(0, 0, 0);
  }
  FastLED.show();

  // Display score
  LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0, 0);
  LCD.print("SONG COMPLETED!");
  LCD.setCursor(0, 1);
  LCD.print("                ");
  LCD.setCursor(0, 1);
  LCD.print("SCORE: ");
  LCD.print(score);

  delay(2000);

  // Display option to go back to the start
  LCD.setCursor(0, 0);
  LCD.print("                ");
  LCD.setCursor(0, 0);
  LCD.print("SCORE: ");
  LCD.print(score);
  LCD.setCursor(0, 1);
  LCD.print("                ");
  LCD.setCursor(0, 1);
  LCD.print("GO BACK?");

  // Wait for user to go back
  pressed1 = 0;
  pressed2 = 0;
  pressed3 = 0;

  while (!(pressed1 || pressed2 || pressed3)) {
    pressed1 = digitalRead(BUTTON_1);
    pressed2 = digitalRead(BUTTON_2);
    pressed3 = digitalRead(BUTTON_3);
    delay(50);
  }
}


void loop() {
  startPanel();

  game();

  endPanel();

  delay(300);
}
