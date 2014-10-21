#include <SCoop.h>

/**************************************************************************/
/*! 
    @file     iso14443a_uid.pde
    @author   Adafruit Industries
    @license  BSD (see license.txt)

    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.   
   
    Note that you need the baud rate to be 115200 because we need to print
    out the data and read from the card at the same time!

    This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
    This library works with the Adafruit NFC breakout 
    ----> https://www.adafruit.com/products/364
 
    Check out the links above for our tutorials and wiring diagrams 
    These chips use I2C to communicate, 4 pins required to interface:
    SDA (I2C Data) and SCL (I2C Clock), IRQ and RESET (any digital line)

    Adafruit invests time and resources providing this open source code, 
    please support Adafruit and open-source hardware by purchasing 
    products from Adafruit!
*/
/**************************************************************************/
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield
#define LED_RED (5)
#define LED_GREEN (6)
Adafruit_NFCShield_I2C nfc(IRQ, RESET);
int previous = LOW;
int reading;

long time = 0;
long debounce = 200;



defineTask(task1)
void task1::setup() {
 Serial.begin(115200);
 pinMode(7, INPUT);
 pinMode(LED_GREEN, OUTPUT);
 pinMode(LED_RED, OUTPUT);
 digitalWrite(LED_GREEN, LOW);
 digitalWrite(LED_RED, HIGH);
}

void task1::loop() {
 // We can do stuff here :D
 // send data only when you receive data:
char inData[20]; 
char inChar=-1;
byte index = 0; 
  while  (Serial.available() > 0) {
    if(index < 19) // One less than the size of the array
     {
       inChar = Serial.read(); // Read a character
       inData[index] = inChar; // Store it
       index++; // Increment where to write next
       inData[index] = '\0'; // Null terminate the string
     }
     if(strcmp(inData, "busy") == 0){
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW); 
     }else if(strcmp(inData, "ready") == 0){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH); 
     }
 }
 
 
 
 reading = digitalRead(7);
 if(reading != previous && millis() - time > debounce && reading == HIGH){
   Serial.println("Post");
   time = millis();
 }
 previous = reading;
 sleep(10); 
}

void setup(void) {
  mySCoop.start();
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");
}



void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX); 
    }
    Serial.println("");
    Serial.flush();
    // Wait 1 second before continuing
    delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}
