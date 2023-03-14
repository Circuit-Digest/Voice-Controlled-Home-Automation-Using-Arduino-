#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>
#define DATA_PIN 12        //Define led data pin in
#define LED_TYPE NEOPIXEL  //define type of led
#define NUM_LEDS 16        //num of leds in strip
#define IRMP_INPUT_PIN 2   // To be compatible with interrupt example, pin 2 is chosen here.
#define IRSND_OUTPUT_PIN 3
#define TONE_PIN 4
#define IRSND_IR_FREQUENCY 38000
#define IRMP_PROTOCOL_NAMES 1         // Enable protocol number mapping to protocol strings - needs some program memory ~ 420 bytes here
#define IRMP_USE_COMPLETE_CALLBACK 1  // Enable callback functionality
// Decode protocol list!disable unwanted     Enable  Remarks      F_INTERRUPTS            program memory
#define IRMP_SUPPORT_SIRCS_PROTOCOL 1         // Sony SIRCS           >= 10000                 ~150 bytes
#define IRMP_SUPPORT_NEC_PROTOCOL 1           // NEC + APPLE + ONKYO  >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SAMSUNG_PROTOCOL 1       // Samsung + Samsg32    >= 10000                 ~300 bytes
#define IRMP_SUPPORT_KASEIKYO_PROTOCOL 1      // Kaseikyo             >= 10000                 ~250 bytes
#define IRMP_SUPPORT_JVC_PROTOCOL 1           // JVC                  >= 10000                 ~150 bytes
#define IRMP_SUPPORT_NEC16_PROTOCOL 1         // NEC16                >= 10000                 ~100 bytes
#define IRMP_SUPPORT_NEC42_PROTOCOL 1         // NEC42                >= 10000                 ~300 bytes
#define IRMP_SUPPORT_MATSUSHITA_PROTOCOL 1    // Matsushita           >= 10000                  ~50 bytes
#define IRMP_SUPPORT_DENON_PROTOCOL 1         // DENON, Sharp         >= 10000                 ~250 bytes
#define IRMP_SUPPORT_RC5_PROTOCOL 1           // RC5                  >= 10000                 ~250 bytes
#define IRMP_SUPPORT_RC6_PROTOCOL 1           // RC6 & RC6A           >= 10000                 ~250 bytes
#define IRMP_SUPPORT_IR60_PROTOCOL 1          // IR60 (SDA2008)       >= 10000                 ~300 bytes
#define IRMP_SUPPORT_GRUNDIG_PROTOCOL 1       // Grundig              >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SIEMENS_PROTOCOL 1       // Siemens Gigaset      >= 15000                 ~550 bytes
#define IRMP_SUPPORT_NOKIA_PROTOCOL 1         // Nokia                >= 10000                 ~300 bytes
#define IRMP_SUPPORT_BOSE_PROTOCOL 1          // BOSE                 >= 10000                 ~150 bytes
#define IRMP_SUPPORT_KATHREIN_PROTOCOL 1      // Kathrein             >= 10000                 ~200 bytes
#define IRMP_SUPPORT_NUBERT_PROTOCOL 1        // NUBERT               >= 10000                  ~50 bytes
#define IRMP_SUPPORT_FAN_PROTOCOL 0           // FAN (ventilator)     >= 10000                  ~50 bytes     conflicts with NUBERT
#define IRMP_SUPPORT_SPEAKER_PROTOCOL 1       // SPEAKER (~NUBERT)    >= 10000                  ~50 bytes
#define IRMP_SUPPORT_BANG_OLUFSEN_PROTOCOL 1  // Bang & Olufsen       >= 10000                 ~200 bytes
#define IRMP_SUPPORT_RECS80_PROTOCOL 1        // RECS80 (SAA3004)     >= 15000                  ~50 bytes
#define IRMP_SUPPORT_RECS80EXT_PROTOCOL 1     // RECS80EXT (SAA3008)  >= 15000                  ~50 bytes
#define IRMP_SUPPORT_THOMSON_PROTOCOL 1       // Thomson              >= 10000                 ~250 bytes
#define IRMP_SUPPORT_NIKON_PROTOCOL 1         // NIKON camera         >= 10000                 ~250 bytes
#define IRMP_SUPPORT_NETBOX_PROTOCOL 1        // Netbox keyboard      >= 10000                 ~400 bytes (PROTOTYPE!)
#define IRMP_SUPPORT_ORTEK_PROTOCOL 0         // ORTEK (Hama)         >= 10000                 ~150 bytes     conflicts with FDC and NETBOX
#define IRMP_SUPPORT_TELEFUNKEN_PROTOCOL 1    // Telefunken 1560      >= 10000                 ~150 bytes
#define IRMP_SUPPORT_FDC_PROTOCOL 1           // FDC3402 keyboard     >= 10000 (better 15000)  ~150 bytes (~400 in combination with RC5)
#define IRMP_SUPPORT_RCCAR_PROTOCOL 1         // RC Car               >= 10000 (better 15000)  ~150 bytes (~500 in combination with RC5)
#define IRMP_SUPPORT_ROOMBA_PROTOCOL 0        // iRobot Roomba        >= 10000                 ~150 bytes     conflicts with RC6
#define IRMP_SUPPORT_RUWIDO_PROTOCOL 0        // RUWIDO, T-Home       >= 15000                 ~550 bytes     conflicts with DENON
#define IRMP_SUPPORT_A1TVBOX_PROTOCOL 1       // A1 TV BOX            >= 15000 (better 20000)  ~300 bytes
#define IRMP_SUPPORT_LEGO_PROTOCOL 1          // LEGO Power RC        >= 20000                 ~150 bytes
#define IRMP_SUPPORT_RCMM_PROTOCOL 1          // RCMM 12,24, or 32    >= 20000                 ~150 bytes
#define IRMP_SUPPORT_LGAIR_PROTOCOL 1         // LG Air Condition     >= 10000                 ~300 bytes
#define IRMP_SUPPORT_SAMSUNG48_PROTOCOL 1     // Samsung48            >= 10000                 ~100 bytes (SAMSUNG must be enabled!)
#define IRMP_SUPPORT_MERLIN_PROTOCOL 1        // Merlin               >= 15000 (better 20000)  ~300 bytes (requires IRMP_32_BIT=1)
#define IRMP_SUPPORT_PENTAX_PROTOCOL 1        // Pentax               >= 10000 <=17000         ~150 bytes (<= 17000 due to 8 bit timing overflow issue)
#define IRMP_SUPPORT_S100_PROTOCOL 0          // S100                 >= 10000                 ~250 bytes     conflicts with RC5
#define IRMP_SUPPORT_ACP24_PROTOCOL 0         // ACP24                >= 10000                 ~250 bytes     conflicts with DENON
#define IRMP_SUPPORT_TECHNICS_PROTOCOL 1      // TECHNICS             >= 10000                 ~250 bytes
#define IRMP_SUPPORT_PANASONIC_PROTOCOL 0     // PANASONIC Beamer     >= 10000                 ~250 bytes     conflicts with KASEIKYO
#define IRMP_SUPPORT_MITSU_HEAVY_PROTOCOL 0   // Mitsubishi Aircond   >= 10000                 ~250 bytes     conflicts with KASEIKYO
#define IRMP_SUPPORT_VINCENT_PROTOCOL 1       // VINCENT              >= 10000                 ~250 bytes
#define IRMP_SUPPORT_SAMSUNGAH_PROTOCOL 1     // SAMSUNG AH           >= 10000                 ~250 bytes
#define IRMP_SUPPORT_IRMP16_PROTOCOL 0        // IRMP specific        >= 15000                 ~250 bytes
#define IRMP_SUPPORT_GREE_PROTOCOL 1          // GREE CLIMATE         >= 10000 <=17000         ~250 bytes
#define IRMP_SUPPORT_RCII_PROTOCOL 0          // RCII T+A             >= 15000                 ~250 bytes     conflicts with GRUNDIG and NOKIA
#define IRMP_SUPPORT_METZ_PROTOCOL 1
#define IRMP_SUPPORT_MELINERA_PROTOCOL 1  // MELINERA (Lidl)      >= 10000

// Encode protocol list!disable unwanted    Enable  Remarks        F_INTERRUPTS            program memory
#define IRSND_SUPPORT_SIRCS_PROTOCOL 1         // Sony SIRCS           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_NEC_PROTOCOL 1           // NEC + APPLE          >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL 1       // Samsung + Samsung32  >= 10000                 ~300 bytes
#define IRSND_SUPPORT_KASEIKYO_PROTOCOL 1      // Kaseikyo             >= 10000                 ~300 bytes
#define IRSND_SUPPORT_JVC_PROTOCOL 1           // JVC                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC16_PROTOCOL 1         // NEC16                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC42_PROTOCOL 1         // NEC42                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_MATSUSHITA_PROTOCOL 1    // Matsushita           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_DENON_PROTOCOL 1         // DENON, Sharp         >= 10000                 ~200 bytes
#define IRSND_SUPPORT_RC5_PROTOCOL 1           // RC5                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RC6_PROTOCOL 1           // RC6                  >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RC6A_PROTOCOL 1          // RC6A                 >= 10000                 ~250 bytes
#define IRSND_SUPPORT_GRUNDIG_PROTOCOL 1       // Grundig              >= 10000                 ~300 bytes
#define IRSND_SUPPORT_SIEMENS_PROTOCOL 1       // Siemens, Gigaset     >= 15000                 ~150 bytes
#define IRSND_SUPPORT_NOKIA_PROTOCOL 1         // Nokia                >= 10000                 ~400 bytes
#define IRSND_SUPPORT_IR60_PROTOCOL 0          // IR60 (SDA2008)       >= 10000                 ~250 bytes // IR frequency 30 kHz
#define IRSND_SUPPORT_BOSE_PROTOCOL 1          // BOSE                 >= 10000                 ~100 bytes
#define IRSND_SUPPORT_KATHREIN_PROTOCOL 0      // Kathrein             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_NUBERT_PROTOCOL 1        // NUBERT               >= 10000                 ~100 bytes
#define IRSND_SUPPORT_FAN_PROTOCOL 1           // FAN (ventilator)     >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SPEAKER_PROTOCOL 1       // SPEAKER              >= 10000                 ~100 bytes
#define IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL 0  // Bang&Olufsen         >= 10000                 ~250 bytes // IR frequency 455 kHz
#define IRSND_SUPPORT_RECS80_PROTOCOL 1        // RECS80               >= 15000                 ~100 bytes
#define IRSND_SUPPORT_RECS80EXT_PROTOCOL 1     // RECS80EXT            >= 15000                 ~100 bytes
#define IRSND_SUPPORT_THOMSON_PROTOCOL 1       // Thomson              >= 10000                 ~250 bytes
#define IRSND_SUPPORT_NIKON_PROTOCOL 1         // NIKON                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NETBOX_PROTOCOL 0        // Netbox keyboard      >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_ORTEK_PROTOCOL 0         // ORTEK (Hama)         >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_TELEFUNKEN_PROTOCOL 1    // Telefunken 1560      >= 10000                 ~150 bytes
#define IRSND_SUPPORT_FDC_PROTOCOL 1           // FDC IR keyboard      >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_RCCAR_PROTOCOL 1         // RC CAR               >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_ROOMBA_PROTOCOL 1        // iRobot Roomba        >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RUWIDO_PROTOCOL 1        // RUWIDO, T-Home       >= 15000                 ~250 bytes
#define IRSND_SUPPORT_A1TVBOX_PROTOCOL 1       // A1 TV BOX            >= 15000 (better 20000)  ~200 bytes
#define IRSND_SUPPORT_LEGO_PROTOCOL 1          // LEGO Power RC        >= 20000                 ~150 bytes
#define IRSND_SUPPORT_RCMM_PROTOCOL 0          // RCMM 12,24, or 32    >= 20000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_LGAIR_PROTOCOL 1         // LG Air Condition     >= 10000                 ~150 bytes.
#define IRSND_SUPPORT_SAMSUNG48_PROTOCOL 1     // Samsung48            >= 10000                 ~100 bytes
#define IRSND_SUPPORT_PENTAX_PROTOCOL 1        // Pentax               >= 10000                 ~150 bytes
#define IRSND_SUPPORT_S100_PROTOCOL 1          // S100                 >= 10000                 ~150 bytes
#define IRSND_SUPPORT_ACP24_PROTOCOL 1         // ACP24                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_TECHNICS_PROTOCOL 1      // TECHNICS             >= 10000                 ~200 bytes
#define IRSND_SUPPORT_PANASONIC_PROTOCOL 1     // PANASONIC Beamer     >= 10000                 ~150 bytes
#define IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL 1   // Mitsubishi-Heavy Aircondition, similar Timing to Panasonic beamer
#define IRSND_SUPPORT_IRMP16_PROTOCOL 0        // IRMP specific        >= 15000                 ~250 bytes

#define USE_ONE_TIMER_FOR_IRMP_AND_IRSND
#include <irmp.hpp>
#include <irsnd.hpp>
IRMP_DATA irmp_data;
IRMP_DATA irsnd_data;
int Prortocol_Index[58] = { IRMP_UNKNOWN_PROTOCOL, IRMP_SIRCS_PROTOCOL, IRMP_NEC_PROTOCOL, IRMP_SAMSUNG_PROTOCOL, IRMP_MATSUSHITA_PROTOCOL, IRMP_KASEIKYO_PROTOCOL, IRMP_RECS80_PROTOCOL, IRMP_RC5_PROTOCOL, IRMP_DENON_PROTOCOL, IRMP_RC6_PROTOCOL, IRMP_SAMSUNG32_PROTOCOL, IRMP_APPLE_PROTOCOL, IRMP_RECS80EXT_PROTOCOL, IRMP_NUBERT_PROTOCOL, IRMP_BANG_OLUFSEN_PROTOCOL, IRMP_GRUNDIG_PROTOCOL, IRMP_NOKIA_PROTOCOL, IRMP_SIEMENS_PROTOCOL, IRMP_FDC_PROTOCOL, IRMP_RCCAR_PROTOCOL, IRMP_JVC_PROTOCOL, IRMP_RC6A_PROTOCOL, IRMP_NIKON_PROTOCOL, IRMP_RUWIDO_PROTOCOL, IRMP_IR60_PROTOCOL, IRMP_KATHREIN_PROTOCOL, IRMP_NETBOX_PROTOCOL, IRMP_NEC16_PROTOCOL, IRMP_NEC42_PROTOCOL, IRMP_LEGO_PROTOCOL, IRMP_THOMSON_PROTOCOL, IRMP_BOSE_PROTOCOL, IRMP_A1TVBOX_PROTOCOL, IRMP_ORTEK_PROTOCOL, IRMP_TELEFUNKEN_PROTOCOL, IRMP_ROOMBA_PROTOCOL, IRMP_RCMM32_PROTOCOL, IRMP_RCMM24_PROTOCOL, IRMP_RCMM12_PROTOCOL, IRMP_SPEAKER_PROTOCOL, IRMP_LGAIR_PROTOCOL, IRMP_SAMSUNG48_PROTOCOL, IRMP_MERLIN_PROTOCOL, IRMP_PENTAX_PROTOCOL, IRMP_FAN_PROTOCOL, IRMP_S100_PROTOCOL, IRMP_ACP24_PROTOCOL, IRMP_TECHNICS_PROTOCOL, IRMP_PANASONIC_PROTOCOL, IRMP_MITSU_HEAVY_PROTOCOL, IRMP_VINCENT_PROTOCOL, IRMP_SAMSUNGAH_PROTOCOL, IRMP_IRMP16_PROTOCOL, IRMP_GREE_PROTOCOL, IRMP_RCII_PROTOCOL, IRMP_METZ_PROTOCOL, IRMP_ONKYO_PROTOCOL };

struct CRGB leds[NUM_LEDS];  // Initialize LED array
const int Relay1 = 5;        // Relay pin 1 (IN1)
const int Relay2 = 4;        // Relay pin 2 (IN2)
const int Relay3 = 6;        // Relay pin 3 (IN3)
const int Relay4 = 7;        // Relay pin 4 (IN4)
int irrec = 0;
String data;  //Variable for storing received data
int proto;
uint16_t addr, cmd, temp;
int eeadr;

void handleReceivedIRData();
void IRDecoder();
void SerialDecoder();
bool volatile sIRMPDataAvailable = false;

void setup() {
  Serial.begin(9600);  //Sets the baud for serial data transmission
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  // Set Relay pins as OUTPUT
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  // Init all relays to off
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);
  irmp_init();
  irmp_irsnd_LEDFeedback(true);  // Enable receive signal feedback at LED_BUILTIN
  irmp_register_complete_callback_function(&handleReceivedIRData);
  irsnd_init();
}

void loop() {

  // Read data from Bluetooth Module
  char ch = 0;
  data = "";
  while (1) { // When there is no command received the MCU will be in this loop. 
    while (Serial.available() <= 0) {
      if (irrec > 0) {
        IRDecoder();// Call IR decoder function when a valid command is detected
      }
      digitalWrite(13, irrec);
    };
    ch = Serial.read();
    if (ch == '#')
      break; // break the loop when a command is received
    data += ch;
  }
  SerialDecoder();// call serial Decoder function
}

//Decode received serial command
void SerialDecoder() { 
  // Print Bluetooth Module data on serial monitor
  Serial.print(data);
  Serial.print("\n");

  // Control the devices using voice command
  if (data.startsWith("*turn on light"))  // turn on Device1
  {
    digitalWrite(Relay1, HIGH);
    Serial.println("  :   light on");
    delay(200);
  } else if (data.startsWith("*turn off light"))  // turn off Device1
  {
    digitalWrite(Relay1, LOW);
    Serial.println("  :   light off");
    delay(2000);
  } else if (data.startsWith("*turn on TV"))  // turn on Device2
  {
    digitalWrite(Relay2, HIGH);
    Serial.println("  :   TV on");
    delay(200);
  }

  else if (data.startsWith("*turn off TV"))  // turn off Device2
  {
    digitalWrite(Relay2, LOW);
    Serial.println("  :   TV off");
    delay(200);
  } else if (data.startsWith("*turn on fan"))  // turn on Device3
  {
    digitalWrite(Relay3, HIGH);
    Serial.println("  :   fan on");
    delay(200);
  } else if (data.startsWith("*turn off fan"))  // turn off Device3
  {
    digitalWrite(Relay3, LOW);
    Serial.println("  :   fan off");
    delay(200);
  } else if (data.startsWith("*turn on music"))  // turn on Device4
  {
    digitalWrite(Relay4, HIGH);
    Serial.println("  :   music on");
    delay(200);
  } else if (data.startsWith("*turn off music"))  // turn off Device4
  {
    digitalWrite(Relay4, LOW);
    Serial.println("  :   music off");
    delay(200);
  } else if (data.startsWith("*turn on all"))  // turn on all Device
  {
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay4, HIGH);
    Serial.println("  :   All Relays on");
    delay(200);
  } else if (data.startsWith("*turn off all"))  // turn off all Device
  {
    digitalWrite(Relay1, LOW);
    digitalWrite(Relay2, LOW);
    digitalWrite(Relay3, LOW);
    digitalWrite(Relay4, LOW);
    Serial.println("  :   All Relays off");
    delay(200);
  } else if (data.startsWith("*RGB red"))  // set the RGB leds to red
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;  //set the RGB leds to red
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*RGB green"))  //set the RGB leds to green
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Green;  //set the RGB leds to green
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*RGB blue"))  // set the RGB leds to blue
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;  //set the RGB leds to blue
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*RGB yellow"))  // set the RGB leds to yellow
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Yellow;  //set the RGB leds to yellow
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*RGB white"))  // // set the RGB leds to  white
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;  //set the RGB leds to white
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*turn off RGB"))  // turn off all RGB LEDS
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;  //turn off all RGB LEDS
    }
    FastLED.show();
    delay(200);
  } else if (data.startsWith("*record power"))  // record and save remote power button
  {
    irrec = 12;
    IRDecoder();
    irrec = 1;
    delay(200);
  } else if (data.startsWith("*record volume up"))  // record and save remote volume up button
  {
    irrec = 12;
    IRDecoder();
    irrec = 2;
    delay(200);
  } else if (data.startsWith("*record volume down"))  // record and save remote volume down button
  {
    irrec = 12;
    IRDecoder();
    irrec = 3;
    delay(200);
  } else if (data.startsWith("*record next"))  // record and save remote next button
  {
    irrec = 12;
    IRDecoder();
    irrec = 4;
    Serial.print("irrec :");
    Serial.println(irrec);
    delay(200);
  } else if (data.startsWith("*record previous"))  /// record and save remote previous button
  {
    irrec = 12;
    IRDecoder();
    irrec = 5;
    delay(200);
  }

  else if (data.startsWith("*remote power"))  // Recall and send remote power button signal
  {
    eeadr = 10;
    irsnd_data.protocol = EEPROM.read(eeadr);
    temp = (EEPROM.read(eeadr + 2) << 8);
    temp |= EEPROM.read(eeadr + 1);
    irsnd_data.address = temp;
    temp = (EEPROM.read(eeadr + 4) << 8);
    temp |= EEPROM.read(eeadr + 3);
    irsnd_data.command = temp;
    irsnd_data.flags = 0;  // repeat frame 1 time
    Serial.print(F("Sending data  "));
    Serial.println(proto);
    if (!irsnd_send_data(&irsnd_data, true)) {
      Serial.println(F("Protocol not found"));  
    }
    irsnd_data_print(&Serial, &irsnd_data);
  } else if (data.startsWith("*remote volume up"))  // Recall and send remote volume up button signal
  {
    eeadr = 20;
    irsnd_data.protocol = EEPROM.read(eeadr);
    temp = (EEPROM.read(eeadr + 2) << 8);
    temp |= EEPROM.read(eeadr + 1);
    irsnd_data.address = temp;
    temp = (EEPROM.read(eeadr + 4) << 8);
    temp |= EEPROM.read(eeadr + 3);
    irsnd_data.command = temp;
    irsnd_data.flags = 0;  // repeat frame 1 time
    Serial.print(F("Sending data  "));
    Serial.println(proto);
    if (!irsnd_send_data(&irsnd_data, true)) {
      Serial.println(F("Protocol not found")); 
    }
    irsnd_data_print(&Serial, &irsnd_data);
  } else if (data.startsWith("*remote volume down")) // Recall and send remote volume down button signal
  {
    eeadr = 30;
    irsnd_data.protocol = EEPROM.read(eeadr);
    temp = (EEPROM.read(eeadr + 2) << 8);
    temp |= EEPROM.read(eeadr + 1);
    irsnd_data.address = temp;
    temp = (EEPROM.read(eeadr + 4) << 8);
    temp |= EEPROM.read(eeadr + 3);
    irsnd_data.command = temp;
    irsnd_data.flags = 0;  // repeat frame 1 time
    Serial.print(F("Sending data  "));
    Serial.println(proto);
    if (!irsnd_send_data(&irsnd_data, true)) {
      Serial.println(F("Protocol not found"));  
    }
    irsnd_data_print(&Serial, &irsnd_data);
  } else if (data.startsWith("*remote next"))  // Recall and send remote next button signal
  {
    eeadr = 40;
    irsnd_data.protocol = EEPROM.read(eeadr);
    temp = (EEPROM.read(eeadr + 2) << 8);
    temp |= EEPROM.read(eeadr + 1);
    irsnd_data.address = temp;
    temp = (EEPROM.read(eeadr + 4) << 8);
    temp |= EEPROM.read(eeadr + 3);
    irsnd_data.command = temp;
    irsnd_data.flags = 0;  // repeat frame 1 time
    Serial.print(F("Sending data  "));
    Serial.println(proto);
    if (!irsnd_send_data(&irsnd_data, true)) {
      Serial.println(F("Protocol not found"));  
    }
    irsnd_data_print(&Serial, &irsnd_data);
  } else if (data.startsWith("*remote previous"))  // Recall and send remote previous button signal
  {
    eeadr = 50;
    irsnd_data.protocol = EEPROM.read(eeadr);
    temp = (EEPROM.read(eeadr + 2) << 8);
    temp |= EEPROM.read(eeadr + 1);
    irsnd_data.address = temp;
    temp = (EEPROM.read(eeadr + 4) << 8);
    temp |= EEPROM.read(eeadr + 3);
    irsnd_data.command = temp;
    irsnd_data.flags = 0;  // repeat frame 1 time
    Serial.print(F("Sending data  "));
    Serial.println(proto);
    if (!irsnd_send_data(&irsnd_data, true)) {
      Serial.println(F("Protocol not found"));  
    }
    irsnd_data_print(&Serial, &irsnd_data);
  }
}

//Decode and save IR signals
void IRDecoder() {
  if (sIRMPDataAvailable) {
    int eepaddr = irrec;
    irrec = 0;
    int rpt;
    sIRMPDataAvailable = false;
    irmp_result_print(&irmp_data);
    proto = irmp_data.protocol;
    addr = irmp_data.address;
    cmd = irmp_data.command;
    // save signal data to EEPROM
    if (eepaddr < 10) {
      if (proto == 0) {
        EEPROM.write(eepaddr * 10, 0x0);
      } else {
        EEPROM.write(eepaddr * 10, proto);
      }
      if (addr == 0) {

        EEPROM.write((eepaddr * 10) + 1, 0);
        EEPROM.write((eepaddr * 10) + 2, 0);
      } else {
        EEPROM.write((eepaddr * 10) + 1, addr);
        EEPROM.write((eepaddr * 10) + 2, addr >> 8);
      }
      if (cmd == 0) {
        EEPROM.write((eepaddr * 10) + 3, 0x0);
        EEPROM.write((eepaddr * 10) + 4, 0x0);
      } else {
        EEPROM.write((eepaddr * 10) + 3, cmd);
        EEPROM.write((eepaddr * 10) + 4, cmd >> 8);
      }
    }
  }
}

//IR interrupt handle routine
void handleReceivedIRData() {
  irmp_get_data(&irmp_data);
  sIRMPDataAvailable = true;
}
