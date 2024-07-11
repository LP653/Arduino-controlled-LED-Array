/*
----------------------------------------------------------------------------------
TODO:

Change the displayImage function to accept a uint64_t for quicker computation
Physical switches for mode selection
Screensavers (explosion animation, stars)
Startup screens
----------------------------------------------------------------------------------
*/


int latchPin = 12; // connected to ST_CP (store) of 74HC595
int clockPin = 11; // connected to SH_CP (shift) of 74HC595
int dataPin = 8; // connected to DS of 74HC595

int m = 8; // # cols for time
int n = 8; // # rows for intensity

int panels = 1;
int shutter_period = 5; // 500us is the maximum
int shutters = 0;
int dt = 750; // timestep in us

int r; // The biased and amplified signal as read by the Arduino
int r_max = 0;
int r_min = 1024;
int r_max1 = 0;
int r_min1 = 1024;
int extreme_period = 100; // cycles to go to reset r_max and r_min

int I; // Intensity: the number of activated lights to be in the column, ranging from -4 to 4
int I_bits = 0; // Transformed intensity to a bit representation of the column

uint8_t V[] = {0, 0, 0, 0, 0, 0, 0, 0}; // An array to store each of the T's to display the signal

int x_dir = 10; // pos for right, neg for left
int y_dir = 10; // pos for down, neg for up

void setup() {
  Serial.begin(921600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(A0, INPUT);

  delay(2000);
}

// To display an image, pass a uint8_t array of m length
void displayImage(uint8_t V[]) {
  // Perform a shutter sweep
  for (int i = 0; i < m; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~(128>>i)); // 
    shiftOut(dataPin, clockPin, MSBFIRST, V[i]); // 
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(shutter_period);
  }
  shutters++;
}

void filled_signal(int r) {
  // Convert audio signal to integer intensity [-4, 4]
  I = round(8*(r - r_min)/(r_max-r_min) - 4);
  
  // Convert I to a bit representation
  I_bits = (int) (round(pow(2.0, abs(I)))-1);

  // Shift everything over -1 and insert visual bit representation at end
  for (int i = 1; i < m; i++) {
    V[i-1] = V[i];
  }
  V[m-1] = (I_bits<<4) + (I_bits<<(4-abs(I)));
}

void dotted_signal(int r) {
  // Convert audio signal to integer intensity [-4, 4]
  I = round(6*(r - r_min)/(r_max-r_min) - 3);
  
  // Convert I to a bit representation
  I_bits = (int) (round(pow(2.0, abs(I)))); // dotted
  
  // Shift everything over -1 and insert visual bit representation at end
  for (int i = 1; i < m; i++) {
    V[i-1] = V[i];
  }
  V[m-1] = (I_bits << 4) + (1 << (3-abs(I))); // dotted bottom half + top half
}

void check_extremes(int r) {
  if (r > r_max) r_max = r;
  if (r < r_min) r_min = r;
  if (r > r_max1) r_max1 = r;
  if (r < r_min1) r_min1 = r;

  if (shutters > extreme_period) {
    r_max = r_max1;
    r_min = r_min1;
  }
}

void panning() {
  if (shutters * shutter_period > dt) {
    /*
    // Produce the value
    int rand = (int) random(1, round(m/2+1));

    // Convert it to a bit representation
    int val = (int) round(pow(2.0, rand)) - 1;

    // Construct the visual bit representation
    int bottom_half = val<<4;
    int top_half = val<<(4-rand);

    // Shift everything over -1 and insert visual bit representation at end
    for (int i = 1; i < m; i++) {
      V[i-1] = V[i];
    }
    V[m-1] = (bottom_half + top_half);
    */
    filled_signal((int) random(1, round(m/2+1)));
    // Reset shutters
    shutters = 0;
  }
  displayImage(V);
  // Reset shutters
  shutters = 0;
}

void readAudio() {

  if (shutter_period * shutters > dt) {

    // Read audio signal
    r = analogRead(A0); // (in circuit) scale signal down to 3/10ths its current amount
    
    check_extremes(r);

    filled_signal(r);
    // dotted_signal(r);
    
    // Reset shutters
    shutters = 0;
  }

  displayImage(V);
}

void snake() {
  int dt = 15000;
  if (y_dir > 0) {
    for (int i = m-1; i >= 0; i--) { // traversing cols
      if (x_dir > 0) {
        for (int j = n-1; j >= 0; j--) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          shiftOut(dataPin, clockPin, MSBFIRST, 128 >> i);
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
      else if (x_dir < 0) {
        for (int j = 0; j < n; j++) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          shiftOut(dataPin, clockPin, MSBFIRST, 128 >> i);
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
    }
    y_dir = y_dir * -1;
  } else if (y_dir < 0) {
    for (int i = 0; i < m; i++) { // traversing cols
      if (x_dir > 0) {
        for (int j = n-1; j >= 0; j--) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          shiftOut(dataPin, clockPin, MSBFIRST, 128 >> i);
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
      else if (x_dir < 0) {
        for (int j = 0; j < n; j++) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          shiftOut(dataPin, clockPin, MSBFIRST, 128 >> i);
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
    }
    y_dir = y_dir * -1;
  }
}

void explosion() {
  /*
    top to bottom
  {0, 0, 0, 0, 0, 0, 0, 0}
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |

 {0, 0, 0, 24, 24, 0, 0, 0}  
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |

{0, 0, 24, 36, 36, 24, 0, 0}
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 1 0 0 1 0 0 |
    | 0 0 1 0 0 1 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |

{0, 24, 36, 66, 66, 36, 24, 0}
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 1 0 0 1 0 0 |
    | 0 1 0 0 0 0 1 0 |
    | 0 1 0 0 0 0 1 0 |
    | 0 0 1 0 0 1 0 0 |
    | 0 0 0 1 1 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
 
{24, 102, 66, 129, 129, 66, 102, 24}
    | 0 0 0 1 1 0 0 0 |
    | 0 1 1 0 0 1 1 0 |
    | 0 1 0 0 0 0 1 0 |
    | 1 0 0 0 0 0 0 1 |
    | 1 0 0 0 0 0 0 1 |
    | 0 1 0 0 0 0 1 0 |
    | 0 1 1 0 0 1 1 0 |
    | 0 0 0 1 1 0 0 0 |
  
{195, 129, 0, 0, 0, 0, 129, 195}
    | 1 1 0 0 0 0 1 1 |
    | 1 0 0 0 0 0 0 1 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 0 0 0 0 0 0 0 0 |
    | 1 0 0 0 0 0 0 1 |
    | 1 1 0 0 0 0 1 1 |
    
  */

  int num_frames = 7;

  uint8_t frames[num_frames][m] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 24, 24, 0, 0, 0},
    {0, 0, 24, 36, 36, 24, 0, 0},
    {0, 24, 36, 66, 66, 36, 24, 0},
    {24, 102, 66, 129, 129, 66, 102, 24},
    {195, 129, 0, 0, 0, 0, 129, 195}
  };

  for (int i = 0; i < num_frames; i++) {
    displayImage(frames[i]);
  }
}

void loop() {
  
  // snake();

  // panning();

  // readAudio();

  // explosion();
  
  stars();

}




// IN PROGRESS
void stars() {

  // TODO: make the coord system on layer less than the led array

  int max_stars = 3;
  
  // initiate the coord sys
  uint64_t sys = 0; // 64-bit number to represent the 8x8 panel, every 8-bit byte is a row
  uint64_t dot = 0; // 64-bit num to represent location of star
  uint64_t dots[max_stars] = {0, 0, 0}; // A queue of stars to be processed
  uint64_t splosion = 0;
  uint64_t splosions[max_stars] = {0, 0, 0}; // A queue of the exploded stars
  

  // Produce random coord
  int randx = (int) random(0, m-1);
  int randy = (int) random(0, n-1);
  
  bool check = true;
  dot = pow(2, randx + m*randy);
  for (int i = 0; i < max_stars; i++) {
    // Check if dot is within the 3x3 square surrounding dots[i]
  /*if ( // above 
         // same
         // below
      ) {
      check = false;
      exit;
    }
    */
  }

  if (check) {
    // insert dot into the dots by shifting the queue and inserting new dot, removed dot is moved to splosions
    splosion = dots[max_stars-1];
    for (int i = 0; i < max_stars-1; i++) {
      dots[max_stars-1-i] = dots[max_stars-2-i];
    }
    dots[0] = dot;

    // Insert splosion into splosions similarly, this time discarding the removed splosion
    for (int i = 0; i < max_stars-1; i++) {
      splosions[max_stars-1-i] = splosions[max_stars-2-i];
    }
    splosions[0] = splosion;

    for (int i = 0; i < max_stars; i++) {
      // Add dots to coord sys
      sys += dots[i];

      // Add splosions to coord sys
      sys += splosions[i]/2 + splosions[i]*2 + splosions[i]*pow(2, 8) + splosions[i]/pow(2, 8);
    }
  }

  // Convert coord sys from uint64_t to uint8_t[m], each elem of V is an 8 bit section of sys
  uint8_t V[m] = {0, 0, 0, 0, 0, 0, 0, 0};
  // uint8_t V[m] = {, , , , , , ,};
  displayImage(V);
  
}



