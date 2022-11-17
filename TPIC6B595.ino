
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

// CONSTANTS
// Good explanation here: https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
// Strobe/Latch/RCLK (also sometimes confusingly called "Clock", as in "clocking in") (pin 1) of TPIC6B595
int strobePin = 10;
// Ser/Data/SerIn/MOSI (pin 2) of TPIC6B595
int dataPin = 12;
// Clock/SRCLK Pin connected to Clock (pin 3) of TPIC6B595
int clockPin = 13;

// Set just the provided bit index on
byte individualSegment(int i){
  byte b = 1<<i;
  return b; 
}
// Set all bits up to the given index on
byte cumulativeSegments(int i){
  byte result = 0;
  for(int x=0; x<=i;x++){
    bitSet(result, x);
  }
  return result;
}
// Set bits based on predefined pattern
byte getPattern(int i){
  byte patterns[] = {B00111111, B10000110, B01011011, B11001111, B01100110, B11101101, B11111101, B00000111, B01111111, B11101111, B01110111, B01111100, B10111001, B01011110, B11111001, B01110001, B01001001}; 
  return patterns[i];
}

// If we have four daisy-chained shift registers, can represent their 32bits in a single long
unsigned long getLongPattern(int i) {
  unsigned long l;
  // Shift a 1 to the ith bit
  // unsigned long l = (unsigned long)1L<<i;
  bitSet(l, i);
  return l;
}

// GLOBALS
int i = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__ __DATE__);
  
  // Set pins to output because they are addressed in the main loop
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // If OE is not tied to GND, then you can manually enable the shift register by writing a LOW value to its pin
  // pinMode(oePin, OUTPUT);
  // digitalWrite(oePin, HIGH);
}


// See https://forum.arduino.cc/t/find-nearest-multiple-of-8-from-a-variable/255420/16
int numRegisters(int x){
  return (x + 7) >> 3;
}

void prntBits(byte b) {
  for(int i = 7; i >= 0; i--)
    Serial.print(bitRead(b,i));
  Serial.println();  
}

void loop() {

  byte outputPattern;

  for(uint8_t i=0;i<16;i++){
    // Display a digit from 0-9 (or A-F for 10-15)
    outputPattern = digitToSegment[i];

    // Set strobe pin low to begin storing bits  
    digitalWrite(strobePin, LOW);
  
    // Then shift in the values for each register - one byte per register, and last first
    shiftOut(dataPin, clockPin, MSBFIRST, outputPattern);

    // Set strobe pin high to stop storing bits
    digitalWrite(strobePin, HIGH);

    prntBits(outputPattern);
    Serial.println("---");
    
    delay(250);
  }

  for(uint8_t i=0;i<8;i++){
    // Display a particular segment
    outputPattern = individualSegment(i);

    // Set strobe pin low to begin storing bits  
    digitalWrite(strobePin, LOW);
  
    // Then shift in the values for each register - one byte per register, and last first
    shiftOut(dataPin, clockPin, MSBFIRST, outputPattern);

    // Set strobe pin high to stop storing bits
    digitalWrite(strobePin, HIGH);

    prntBits(outputPattern);
    Serial.println("---");
    
    delay(250);    
  }


  for(uint8_t i=0;i<8;i++){
    // Display a particular segment
    outputPattern = cumulativeSegments(i);

    // Set strobe pin low to begin storing bits  
    digitalWrite(strobePin, LOW);
  
    // Then shift in the values for each register - one byte per register, and last first
    shiftOut(dataPin, clockPin, MSBFIRST, outputPattern);

    // Set strobe pin high to stop storing bits
    digitalWrite(strobePin, HIGH);

    prntBits(outputPattern);
    Serial.println("---");
    
    delay(250);    
  }
  
}
