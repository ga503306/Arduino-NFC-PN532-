#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"
#include "Wire.h"

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];
int ledPin = 5;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    
    Serial.println("NFC Peer to Peer Example - Receive Message");
}

void loop() {
    Serial.println("Waiting for message from a peer");
    int msgSize = nfc.read(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
        //msg.print();
            
        NdefRecord record = msg.getRecord(0);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);        
        
        int startChar = 0;        
        if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "T") { // text message
          // skip the language code
          startChar = payload[0] + 1;
        } else if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "U") { // URI
          startChar = 1;
        }
                         
        String payloadAsString = "";
        for (int c = startChar; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }

        Serial.print("read " + payloadAsString);
        if(payloadAsString=="10224076")
        { 
          Serial.println("\n yes 10224076"); //really
          digitalWrite(ledPin, HIGH);
          delay(1000);
          digitalWrite(ledPin, LOW);
          }
          else{
             Serial.println("\n not 10224076");
            }
          
          Serial.println("\nSuccess");
    } else {
        Serial.println("Failed");
    }
    delay(1000);
}
