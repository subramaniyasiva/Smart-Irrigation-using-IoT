#include <DHT.h>

// Pin definitions
#define waterLevelPin D8
#define moisturePin A0
#define relayPin1 D1
#define relayPin2 D2
#define DHTPIN D3
#define DHTTYPE DHT11

// Threshold values
const int moistureThreshold = 50;      // Change the moisture threshold (0-100)

DHT dht(DHTPIN, DHTTYPE);

// Pump status variables
bool pump1Status = false;
bool pump2Status = false;

void setup() {
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Read water level
  int waterLevel = digitalRead(waterLevelPin);
  Serial.print("Water level: ");
  Serial.println(waterLevel);

  // Read soil moisture
  int moistureValue = analogRead(moisturePin) * 100 / 1023;    // Scale moisture value to 0-100
  Serial.print("Soil Moisture: ");
  Serial.println(moistureValue);

  // Check water level threshold
  if (waterLevel == LOW) {
    if (!pump1Status) {
      startPump(1); // Start pump connected to relayPin1
    }
  } else {
    if (pump1Status) {
      stopPump(1); // Stop pump connected to relayPin1
    }
  }

  // Check moisture threshold
  if (moistureValue < moistureThreshold) {
    if (!pump2Status) {
      startPump(2); // Start pump connected to relayPin2
    }
  } else {
    if (pump2Status) {
      stopPump(2); // Stop pump connected to relayPin2
    }
  }

  // Read temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again)
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature and humidity
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(3000); // Delay of 3 seconds between readings
}

void startPump(int relayChannel) {
  if (relayChannel == 1) {
    digitalWrite(relayPin1, HIGH);
    Serial.println("Pump 1 started");
    pump1Status = true;
  } else if (relayChannel == 2) {
    digitalWrite(relayPin2, HIGH);
    Serial.println("Pump 2 started");
    pump2Status = true;
  }
}

void stopPump(int relayChannel) {
  if (relayChannel == 1) {
    digitalWrite(relayPin1, LOW);
    Serial.println("Pump 1 stopped");
    pump1Status = false;
  } else if (relayChannel == 2) {
    digitalWrite(relayPin2, LOW);
    Serial.println("Pump 2 stopped");
    pump2Status = false;
  }
}
