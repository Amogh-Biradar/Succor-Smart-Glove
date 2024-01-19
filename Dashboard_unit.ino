// Include necessary libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Create instances for LCD, software serial, and radio communication
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(2, 3);
RF24 radio(9, 10);

// Define the address for radio communication
const byte address[6] = "00001";

void setup() {
  // Initialize LCD and backlight
  lcd.init();
  lcd.backlight();

  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize radio communication
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Initialize software serial for SMS
  mySerial.begin(9600);

  // Set pins 5 and 6 as outputs
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // Display welcome message on LCD
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("SMART GLOVE");
}

void loop() {
  // Check for incoming radio data
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    // Perform actions based on received character
    if (text[0] == 'A') {
      digitalWrite(5, HIGH); // Turn on something connected to pin 5
      delay(5000);
    } else if (text[0] == 'B') {
      digitalWrite(6, HIGH); // Turn on something connected to pin 6
      delay(500);
    } else if (text[0] == 'P') {
      digitalWrite(5, LOW); // Turn off something connected to pin 5
      delay(500);
    } else if (text[0] == 'Q') {
      digitalWrite(6, LOW); // Turn off something connected to pin 6
      delay(5000);
    } else if (text[0] == 'C') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HI I Want");
      lcd.setCursor(0, 1);
      lcd.print("to meet Doctor");
      SEND_SMS1(); // Send SMS to doctor
    } else if (text[0] == 'D') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HI I Want ");
      lcd.setCursor(0, 1);
      lcd.print("to meet Nurse");
      SEND_SMS2(); // Send SMS to nurse
    }
  }
}

// Function to send SMS to doctor
void SEND_SMS1() {
  // AT commands for SMS configuration
  mySerial.println("AT");
  delay(500);
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+NUMBER_HERE\"\r"); // Replace with phone number
  delay(500);
  mySerial.print("I want to meet Doctor");
  delay(500);
  mySerial.write(26); // Send Ctrl+Z to send the message
  delay(12000); // Wait for message to send
}

// Function to send SMS to nurse
void SEND_SMS2() {
  // Similar AT commands for SMS to nurse
  mySerial.println("AT");
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+NUMBER_HERE\"\r"); // Replace with phone number
  delay(500);
  mySerial.print("I want to meet Nurse");
  delay(500);
  mySerial.write(26); // Send Ctrl+Z to send the message
  delay(12000); // Wait for message to send
}
