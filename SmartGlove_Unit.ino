#include <SPI.h> // Include SPI library for communication with radio module
#include <nRF24L01.h> // Include nRF24L01 radio module library
#include <RF24.h> // Include RF24 library for radio communication

// Create an instance of the RF24 radio object with specific pins for CSN and CE
RF24 radio(9, 10);

// Define four analog input pins to read sensor values
int a0 = 0;
int a1 = 1;
int a2 = 2;
int a3 = 3;

// Initialize variables for state tracking
int i = 0; // Tracks state for button A
int j = 0; // Tracks state for button B

// Define the address for radio communication
const byte address[6] = "00001";

void setup() {
  // Start serial communication for debugging information
  Serial.begin(9600);

  // Initialize the radio module
  radio.begin();

  // Open a writing pipe to the specified address
  radio.openWritingPipe(address);

  // Set the power level to minimum for efficient communication
  radio.setPALevel(RF24_PA_MIN);

  // Stop listening for incoming data, only sending in this sketch
  radio.stopListening();
}

void loop() {
  // Define constant character arrays for radio messages
  const char text1[] = "A"; // Message for button A press
  const char text2[] = "B"; // Message for button B press
  const char text3[] = "C"; // Message for sensor A2 exceeding threshold
  const char text4[] = "D"; // Message for sensor A3 exceeding threshold
  const char text11[] = "P"; // Message for button A hold release
  const char text12[] = "Q"; // Message for button B hold release

  // Read analog values from each sensor
  a0 = analogRead(0);
  a1 = analogRead(1);
  a2 = analogRead(2);
  a3 = analogRead(3);

  // Button A logic: press and hold
  if ((a0 > 992) && (i == 0)) {
    i = 1; // Change state to indicate press
    radio.write(&text1, sizeof(text1)); // Send message "A" for button press
    Serial.println("Button A pressed");
    Serial.println(a0); // Print read value for debug
    delay(500); // Delay before holding check
    a0 = analogRead(0); // Read analog value again for hold detection
  } else if ((a0 > 992) && (i == 1)) {
    i = 0; // Change state to indicate release
    radio.write(&text11, sizeof(text11)); // Send message "P" for button hold release
    Serial.println("Button A hold released");
    Serial.println(a0); // Print read value for debug
    delay(500); // Delay before next detection
    a0 = analogRead(0); // Read analog value again for next cycle
  }

  // Button B logic: press and hold (similar to button A)
  // ... (Insert code for button B logic with your desired thresholds and messages)

  // Sensor A2 threshold exceeded
  if (a2 > 900) {
    radio.write(&text3, sizeof(text3)); // Send message "C" for sensor A2 exceeding threshold
    Serial.println("Sensor A2 exceeded threshold");
    Serial.println(a2); // Print read value for debug
    delay(500); // Delay before next check
  }

  // Sensor A3 threshold exceeded
  if (a3 > 500) {
    radio.write(&text4, sizeof(text4)); // Send message "D" for sensor A3 exceeding threshold
    Serial.println("Sensor A3 exceeded threshold");
    Serial.println(a3); // Print read value for debug
    delay(500); // Delay before next check
  }

  // Delay between sensor and button checks
  delay(500);
}
