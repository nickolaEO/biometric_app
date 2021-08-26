/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/



#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);
int ledY = 6;
int ledG = 7;
int ledR = 8;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
//  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
//  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
//  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
//  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
//  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
//  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
//  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
     Serial.println("Waiting for valid finger...");
     Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{int a = Serial.parseInt();
  switch (a){
    case 1:{
      Serial.println("Ready to enroll a fingerprint!");
      Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
      id = readnumber();
      if (id == 0) {// ID #0 not allowed, try again!
         return;
      }
      Serial.print("Enrolling ID #");
      Serial.println(id);

      while (!  getFingerprintEnroll() );
      break;}
    case 2:{
      Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
      uint8_t id = readnumber();
      if (id == 0) {// ID #0 not allowed, try again!
         return;
      }
    
      Serial.print("Deleting ID #");
      Serial.println(id);
    
      deleteFingerprint(id);
      break;}
    case 3:{
      int i = 0;
      finger.emptyDatabase();
      while(i<10){
        digitalWrite(ledG, HIGH);
        delay(70);
        digitalWrite(ledG, LOW);
        delay(70);
        i++;
        }
      break;
      } 
    default:{
      getFingerprintID();
      delay(30);
    }
    }
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(ledY, HIGH);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      digitalWrite(ledY, HIGH);
      delay(300);
      digitalWrite(ledY, LOW);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      delay(1500);
      digitalWrite(ledY, LOW);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      digitalWrite(ledY, HIGH);
      delay(300);
      digitalWrite(ledY, LOW);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      Serial.println("Fingerprints did not match");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else {
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    int i = 0;
    while (i < 10) {
      digitalWrite (ledG, HIGH);
      delay(200);
      digitalWrite (ledG, LOW);
      delay(200);
      i++;
    }
      
    digitalWrite (ledG, HIGH);
    delay(2500);
    digitalWrite (ledG, LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not store in that location");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else {
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  }

  return true;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(ledY, HIGH);
      delay(1500);
      digitalWrite(ledY, LOW);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
//      digitalWrite(ledR, HIGH);
//      delay(1500);
//      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
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
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
    default:
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    digitalWrite(ledR, HIGH);
    delay(1500);
    digitalWrite(ledR, LOW);
    return p;
  } else {
      Serial.println("Unknown error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite (ledG, HIGH);
  delay(2500);
  digitalWrite (ledG, LOW);
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

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    digitalWrite(ledG, HIGH);
    delay(1500);
    digitalWrite(ledG, LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
  } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not delete in that location");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
  } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
  } else {
      Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
      digitalWrite(ledR, HIGH);
      delay(1500);
      digitalWrite(ledR, LOW);
  }
  return p;
}
