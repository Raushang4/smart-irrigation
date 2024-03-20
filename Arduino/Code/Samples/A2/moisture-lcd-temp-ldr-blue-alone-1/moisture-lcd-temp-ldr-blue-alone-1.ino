#include <Arduino.h>
#include <Wire.h> // Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for I2C LCD
#include <OneWire.h> // Library for OneWire communication
#include <DallasTemperature.h> // Library for DS18B20 temperature sensor
#include <BluetoothSerial.h> // Library for Bluetooth Serial

// Constants
const int soilMoisturePin = A0; // Analog pin for soil moisture sensor
const int LDRPin = 2; // Analog pin for Light Dependent Resistor (LDR)
const int temperatureSensorPin = 17; // Digital pin for DS18B20 temperature sensor

// Bluetooth setup
BluetoothSerial SerialBT; 

OneWire oneWire(temperatureSensorPin);
DallasTemperature DS18B20(&oneWire);

// Initialize the LCD module
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address (0x27) if needed

void setup() {
  Serial.begin(9600);
  pinMode(soilMoisturePin, INPUT);
  pinMode(LDRPin, INPUT);

  // Initialize the LCD module
  lcd.begin(); // Adjust the column and row size according to your LCD module
  lcd.backlight(); // Turn on the LCD backlight

  // Initialize the temperature sensor
  DS18B20.begin();

  // Bluetooth setup
  SerialBT.begin("ESP32-Bluetooth"); // Change the name to your desired name
  Serial.println("Bluetooth device ready for pairing.");
}

void loop() {
  // Read soil moisture sensor
  int soilMoistureValue = analogRead(soilMoisturePin);

  // Read temperature from DS18B20 sensor
  DS18B20.requestTemperatures();
  float temperatureC = DS18B20.getTempCByIndex(0);

  // Read Light Dependent Resistor (LDR) value
  int ldrValue = analogRead(LDRPin);

  // Check if Bluetooth device is connected and read commands
  if (SerialBT.available()) {
    char command = SerialBT.read();
    if (command == '1') { // Turn on the system (send '1' from the Bluetooth device to turn on the system)
      // Add your code here to turn on the system
    } else if (command == '0') { // Turn off the system (send '0' from the Bluetooth device to turn off the system)
      // Add your code here to turn off the system
    }
  }

  // Print the soil moisture value, temperature, and LDR value to the Serial monitor
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoistureValue);
  Serial.print(", Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" °C, LDR Value: ");
  Serial.println(ldrValue);

  // Display the sensor data on the LCD
  lcd.clear(); // Clear the LCD screen

  lcd.setCursor(0, 0); // Set the cursor to the first column of the first row
  lcd.print("Moisture: ");
  lcd.print(soilMoistureValue); // Display the soil moisture value

  lcd.setCursor(0, 1); // Set the cursor to the first column of the second row
  lcd.print("Temp: ");
  lcd.print(temperatureC); // Display the temperature value
  lcd.print(" °C ");
  
  lcd.setCursor(0, 2); // Set the cursor to the first column of the third row
  lcd.print("LDR: ");
  lcd.print(ldrValue); // Display the LDR value

  // Send the sensor data to the Bluetooth device
  SerialBT.print("Moisture: ");
  SerialBT.print(soilMoistureValue);
  SerialBT.print(", Temperature: ");
  SerialBT.print(temperatureC);
  SerialBT.print(" °C, LDR Value: ");
  SerialBT.println(ldrValue);

  // Perform the actions based on the system state (ON/OFF)
  // For example, you can control relays or other components
  // based on the sensor readings and Bluetooth commands.

  // Add a delay before taking the next sensor reading
  delay(2000); // You can adjust the delay interval as needed
}
