#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance for RFID reader
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // LCD interface pins (connect LCD to these pins)

// List of authorized UIDs and corresponding messages (You can add more UIDs and names here)
const char* authorizedUIDs[] = {
  "B39E7403", // Replace this with your first card's UID
  "AAD48082"  // Replace this with your second card's UID
};

// Corresponding names or messages for each UID (Make sure these correspond to the UIDs above)
const char* authorizedNames[] = {
  "John Doe",    // Name for the first UID
  "Jane Smith"   // Name for the second UID
};

// Function declarations
String getCardUID();
bool isAuthorizedUID(String uid);
void displayMessage(String line1, String line2);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Approximate your card to the reader...");

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Ready to scan...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read UID
  String cardUID = getCardUID();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID: ");
  lcd.print(cardUID);

  Serial.print("UID Number: ");
  Serial.println(cardUID);

  // Check if UID is authorized
  int index = getAuthorizedIndex(cardUID);
  if (index != -1) {
    displayMessage("Access granted", authorizedNames[index]);  // Show the corresponding name
  } else {
    displayMessage("Access denied", "Unauthorized");  // Show this message if UID is not authorized
  }

  delay(3000);  // Pause to allow the message to be displayed
}

// Get card UID as a string (without spaces)
String getCardUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);  // Convert each byte to a hex string
  }
  uid.toUpperCase();  // Make sure the UID is in uppercase
  return uid;
}

// Check if UID is in the authorized list and return its index, or return -1 if not found
int getAuthorizedIndex(String uid) {
  for (int i = 0; i < sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]); i++) {
    if (uid == authorizedUIDs[i]) {
      return i;  // Return the index if UID matches
    }
  }
  return -1;  // Return -1 if UID is not found
}

// Display message on LCD
void displayMessage(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);  // Display message on the first line
  lcd.setCursor(0, 1);
  lcd.print(line2);  // Display the name or status on the second line
}





