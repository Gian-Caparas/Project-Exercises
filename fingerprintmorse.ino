#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(16, 17);
#else
#define mySerial Serial2
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define RELAY_PIN  8
#define ACCESS_DELAY    30000 // Keep lock unlocked for 30 seconds 
#define MAX_FAILED_ATTEMPTS 4
const int dash = 7;
const int dot = 6;
const int saveLetter = 5;
const int saveText = 4;
const int buzzer = 3;
const int ledPin = 2;
int counterFail = 0;
bool printed=false;
String PASSWORD ="WABS";
String morseCode = "";
String textMessage = "";
bool pass = false;
bool attempting = true; 

// Function prototypes
int getFingerPrint();
void unlockRelay();
void morseInput();
char morseToLetter(String code);
void blinkLED(int duration);

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  
  pinMode(dash, INPUT_PULLUP);
  pinMode(dot, INPUT_PULLUP);
  pinMode(saveLetter, INPUT_PULLUP);
  pinMode(saveText, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
   while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

}
}

void loop() {
  

  if (pass) {
   digitalWrite(RELAY_PIN, LOW); // Unlock the relay
    delay(30000); // Keep the relay unlocked for 30 seconds
    Serial.println("H");
    // Turn the relay off
    digitalWrite(RELAY_PIN, HIGH); // Lock the relay
    
  pass=false;
  }
  if (counterFail >= MAX_FAILED_ATTEMPTS && !printed){
        Serial.println("Maximum failed attempts reached. Enter Morse Code");
        attempting = false; // Exit the loop
        printed=true;
      }

  if (attempting){
    getFingerprintID() ;
  }

   
    delay(50); // Add some delay before next scan
  

  if (counterFail >= MAX_FAILED_ATTEMPTS) {
    morseInput();
  }


}

void morseInput() {
if (digitalRead(dash) == LOW) {
    morseCode += "-";
    Serial.println(morseCode);
    blinkLED(400);
    delay(500);
    
}

if (digitalRead(dot) == LOW) {
    morseCode += ".";
    Serial.println(morseCode);
    blinkLED(200);
    delay(500);
    
}

if (digitalRead(saveLetter) == LOW) {
    char letter = morseToLetter(morseCode);
    Serial.print(letter);
    textMessage += letter;
    morseCode = "";
    Serial.println("Text: ");
    Serial.print(textMessage);
    delay(500);
}
  if (digitalRead(saveText) == LOW) {
    Serial.println(textMessage);
    if (PASSWORD == textMessage) {
      Serial.println("Correct password!!");
      counterFail=0;
      pass=true;
      attempting=true;
      printed=false;
    }
    else 
    {Serial.println("Incorrect password!!");
    blinkLED(1000);
    }
    textMessage = "";
    morseCode = "";
    
    delay(200);
  }
}


char morseToLetter(String code) {
  if (code == ".-") return 'A';
  else if (code == "-...") return 'B';
  else if (code == "-.-.") return 'C';
  else if (code == "-..") return 'D';
  else if (code == ".") return 'E';
  else if (code == "..-.") return 'F';
  else if (code == "--.") return 'G';
  else if (code == "....") return 'H';
  else if (code == "..") return 'I';
  else if (code == ".---") return 'J';
  else if (code == "-.-") return 'K';
  else if (code == ".-..") return 'L';
  else if (code == "--") return 'M';
  else if (code == "-.") return 'N';
  else if (code == "---") return 'O';
  else if (code == ".--.") return 'P';
  else if (code == "--.-") return 'Q';
  else if (code == ".-.") return 'R';
  else if (code == "...") return 'S';
  else if (code == "-") return 'T';
  else if (code == "..-") return 'U';
  else if (code == "...-") return 'V';
  else if (code == ".--") return 'W';
  else if (code == "-..-") return 'X';
  else if (code == "-.--") return 'Y';
  else if (code == "--..") return 'Z';
  else return ' ';
}

void blinkLED(int duration) {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzer, LOW);
  delay(200);
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    pass=true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    counterFail++;
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
