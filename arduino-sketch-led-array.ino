/**********************************************************************
Based on the following instructions:
  Filename    : LED Matrix Display
  Description : Use 2 74HC595 to drive the LED Matrix display
  Auther      : www.freenove.com
**********************************************************************/

int latchPin = 18;          // Pin connected to RCLK of 74HC595（Pin12）
int clockPin = 5;          // Pin connected to SRCLK of 74HC595（Pin11）
int dataPin  = 23;          // Pin connected to SER of 74HC595（Pin14)

int m = 8; // # cols
int n = 8; // # rows

// Snake parameters
int x_dir = 10;
int y_dir = 10;

// Shutter for signal drawing in display[H/V]Image functions
int shutter_period = 5;
int shutters = 0;

int dt = 500; // in us

// Signal initialization and parameters
int r;
int r_max = 1024;
int r_min = 0;

// Array to store list of pixels to display
uint8_t V[] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  // set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {

  // snake();

  // explosion();

  // random_signal();

  sin_signal();
}

// Shutter over rows
void displayHorizontalImage(uint8_t V[]) {
  for (int i = 0; i < m; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 128>>i); // row
    shiftOut(dataPin, clockPin, MSBFIRST, ~(V[i])); // column
    digitalWrite(latchPin, HIGH);
  }
}

// Shutters over columns
void displayVerticalImage(uint8_t V[]) {
  int frame_count = 0;
  int frame_iterations = 450;
  while (frame_count < frame_iterations) {
    for (int i = 0; i < m; i++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, V[i]); // column
      shiftOut(dataPin, clockPin, LSBFIRST, ~(128>>i)); // row
      digitalWrite(latchPin, HIGH);
    }
    frame_count++;
  }
  frame_count=0;
}

// An animation to check if all connections are properly made
void snake() {
  int dt = 50000;
  if (y_dir > 0) {
    for (int i = m-1; i >= 0; i--) { // traversing cols
      if (x_dir > 0) {
        for (int j = n-1; j >= 0; j--) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, LSBFIRST, 128 >> i);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
      else if (x_dir < 0) {
        for (int j = 0; j < n; j++) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, LSBFIRST, 128 >> i);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
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
          shiftOut(dataPin, clockPin, LSBFIRST, 128 >> i);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
      else if (x_dir < 0) {
        for (int j = 0; j < n; j++) { // traversing rows
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, LSBFIRST, 128 >> i);
          shiftOut(dataPin, clockPin, MSBFIRST, ~(128 >> j));
          digitalWrite(latchPin, HIGH);
          delayMicroseconds(dt);
        }
        x_dir = x_dir * -1;
      }
    }
    y_dir = y_dir * -1;
  }
}

// An animation to check if display[H/V]Image() functions work
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

  int num_images = 7;

  uint8_t images[num_images][m] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 24, 24, 0, 0, 0},
    {0, 0, 24, 36, 36, 24, 0, 0},
    {0, 24, 36, 66, 66, 36, 24, 0},
    {24, 102, 66, 129, 129, 66, 102, 24},
    {195, 129, 0, 0, 0, 0, 129, 195}
  };
  for (int i = 0; i < num_images; i++) {
    displayVerticalImage(images[i]);
    delayMicroseconds(dt);
  }
}

// X-axis symmetric filled signal
// r: [-4,4]
void sym_filled_signal(int r) {
  // Push all values to the right
  for (int i = m-1; i > 0; i--) {
    V[i] = V[i-1];
  }

  int u = abs(r);
  int val = (1<<u)-1;

  int bottom = (val<<(m/2));
  int top = (val<<(m/2-u));
  V[0] = (top + bottom);
  displayVerticalImage(V);
  delayMicroseconds(dt);
}

// X-axis symmetric dotted signal
// r: [-4,4]
void sym_dotted_signal(int r) {
  // Push all values to the right
  for (int i = m-1; i > 0; i--) {
    V[i] = V[i-1];
  }

  if (r == 0) {
    V[0] = 0;
  } else {
    // Convert signal into [0,3] range, so different style of signal don't need different inputs...all inputs should be [-4,4] ints
    int u = abs((int) 3*(r/4.0));
    
    int bottom = (1<<(m/2+u));
    int top = (1<<(m/2-u-1));
    V[0] = (bottom + top);
  }

    displayVerticalImage(V);
    delayMicroseconds(dt);
}

// Shows positive and negative values
// r: [-4, 4]
void filled_signal(int r) {

  // Push all values to the right
  for (int i = m-1; i > 0; i--) {
    V[i] = V[i-1];
  }

  int val = (1<<abs(r)) - 1; // Convert signal value to a bit representation

  if (r > 0) {
    V[0] = val<<(m/2-r); // Top
  } else if (r < 0) {
    V[0] = val<<(m/2); // Bottom
  } else {
    V[0] = 0;
  }

  displayVerticalImage(V);
  delayMicroseconds(dt);
}

// Shows positive and negative values
// r: [-4, 4]
void dotted_signal(int r) {
  // Push all values to the right
  for (int i = m-1; i > 0; i--) {
    V[i] = V[i-1];
  }

  if (r == 0) {
    V[0] = 0;
  } else {
    // Convert signal into [0,3] range, so different style of signal don't need different inputs...all inputs should be [-4,4] ints
    int u = abs((int) 3*(r/4.0));
    if (r > 0) {
      V[0] = (1<<(m/2-u-1));
    } else { // r < 0
      V[0] = (1<<(m/2+u));
    }
  }

  displayVerticalImage(V);
  delayMicroseconds(dt);
}

///////////////////////
///Simulated Signals///
///////////////////////

void random_signal() {
  // Produce the value
  int rand = (int) random(1, round(m/2+1));
  sym_filled_signal(rand);
}

void sin_signal() {
  int period = 3*n;
  for (int i = 0; i < period; i++) {
    filled_signal((int) (m/2)*sin(2*M_PI*i/(period)));
  }
}
