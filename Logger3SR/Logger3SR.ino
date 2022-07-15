#include <FlexCAN_T4.h>
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

//Define message from FlexCAN library
static CAN_message_t txmsg0;
static CAN_message_t txmsg1;
static CAN_message_t rxmsg0;
static CAN_message_t rxmsg1;

//Set up timing variables (Use prime numbers so they don't overlap)
#define TXPeriod 630
elapsedMicros TXTimer;
unsigned long previousMillis = 0;

//Define button pin, the button is soldered on SW21
#define button1 28
#define button2 53
//Create an on/off boolean for the button
bool buttonState1;
bool buttonState2;

bool toggle;
bool newData = false;

//Create a counter to keep track of message traffic
uint32_t TXCount0 = 0;
uint32_t TXCount1 = 0;
uint32_t RXCount0 = 0;
uint32_t RXCount1 = 0;

//Define LED
#define GREEN_LED_PIN 6
#define RED_LED_PIN 14
#define YELLOW_LED_PIN 5
#define BLUE_LED_PIN 39

boolean GREEN_LED_state;
boolean RED_LED_state;
boolean YELLOW_LED_state;
boolean BLUE_LED_state;

//Define CAN TXRX Transmission Silent pins
#define SILENT_0 42
#define SILENT_1 41
#define SILENT_2 40

//Define default baudrate
#define BAUDRATE250K 250000
#define BAUDRATE500K 500000

void setup() {
  // put your setup code here, to run once:
  //Set baudrate
  Can1.begin();
  Can0.begin();
  Can1.setBaudRate(BAUDRATE250K);
  Can0.setBaudRate(BAUDRATE250K);

  //Set message extension, ID, and length
  txmsg0.flags.extended = 1;
  txmsg0.id = 0x100;
  txmsg0.len = 8;

  txmsg1.flags.extended = 1;
  txmsg1.id = 0x101;
  txmsg1.len = 8;

  // Enable transmission for the CAN TXRX
  pinMode(SILENT_0, OUTPUT);
  pinMode(SILENT_1, OUTPUT);
  pinMode(SILENT_2, OUTPUT);
  digitalWrite(SILENT_0, LOW);
  digitalWrite(SILENT_1, LOW);
  digitalWrite(SILENT_2, LOW);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  //Pull button high
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  unsigned long currentMillis = micros();

  if (buttonState1 == false) {
    toggle = true;
  }

  if (buttonState2 == false) {
    toggle = false;
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  }

  if (toggle) {
    if (Can0.read(rxmsg0)) {
      previousMillis = currentMillis;
      newData = true;
      printFrame(rxmsg0, 0, RXCount0++);
      //RXCount0++;
      //Toggle the LED
      GREEN_LED_state = !GREEN_LED_state;
      digitalWrite(GREEN_LED_PIN, GREEN_LED_state);
    }
    if (Can1.read(rxmsg1)) {
      previousMillis = currentMillis;
      newData = true;
      printFrame(rxmsg1, 1, RXCount1++);
      //RXCount1++;
      //Toggle the LED
      RED_LED_state = !RED_LED_state;
      digitalWrite(RED_LED_PIN, RED_LED_state);
    }
    if (TXTimer >= TXPeriod) {
      TXTimer = 0;//Reset Timer

      //Convert the 32-bit timestamp into 4 bytes with the most significant byte (MSB) first (Big endian).
      uint32_t sysMicros = micros();
      txmsg0.buf[0] = (sysMicros & 0xFF000000) >> 24;
      txmsg0.buf[1] = (sysMicros & 0x00FF0000) >> 16;
      txmsg0.buf[2] = (sysMicros & 0x0000FF00) >>  8;
      txmsg0.buf[3] = (sysMicros & 0x000000FF);

      //Convert the 32-bit transmit counter into 4 bytes with the most significant byte (MSB) first (Big endian).
      txmsg0.buf[4] = (TXCount0 & 0xFF000000) >> 24;
      txmsg0.buf[5] = (TXCount0 & 0x00FF0000) >> 16;
      txmsg0.buf[6] = (TXCount0 & 0x0000FF00) >>  8;
      txmsg0.buf[7] = (TXCount0 & 0x000000FF);

      //Write the message on CAN channel 0
      //Can0.write(txmsg0);
      TXCount0++;

      //Toggle the LED
      BLUE_LED_state = !BLUE_LED_state;
      digitalWrite(BLUE_LED_PIN, BLUE_LED_state);
      //Serial.print("CAN0 Message Sent: ");
      //Serial.println(TXCount0);
      txmsg1.buf[0] = (sysMicros & 0xFF000000) >> 24;
      txmsg1.buf[1] = (sysMicros & 0x00FF0000) >> 16;
      txmsg1.buf[2] = (sysMicros & 0x0000FF00) >>  8;
      txmsg1.buf[3] = (sysMicros & 0x000000FF);

      //Convert the 32-bit transmit counter into 4 bytes with the most significant byte (MSB) first (Big endian).
      txmsg1.buf[4] = (TXCount1 & 0xFF000000) >> 24;
      txmsg1.buf[5] = (TXCount1 & 0x00FF0000) >> 16;
      txmsg1.buf[6] = (TXCount1 & 0x0000FF00) >>  8;
      txmsg1.buf[7] = (TXCount1 & 0x000000FF);

      //Write the message on CAN channel 1
      Can1.write(txmsg1);
      TXCount1++;
      //Toggle the LED
      YELLOW_LED_state = !YELLOW_LED_state;
      digitalWrite(YELLOW_LED_PIN, YELLOW_LED_state);
      //Serial.print("CAN1 Message Sent: ");
      //Serial.println(TXCount1);
    }

  }
  if (((unsigned long)(currentMillis - previousMillis) >= 9000) && newData == true) {
    Serial.println(RXCount0);
    Serial.println(RXCount1);
    RXCount0 = 0;
    RXCount1 = 0;
    newData = false;
  }
}

//A generic CAN Frame print function for the Serial terminal
void printFrame(CAN_message_t rxmsg, uint8_t channel, uint32_t RXCount)
{
  char CANdataDisplay[50];
  sprintf(CANdataDisplay, "%d %12lu %12lu %08X %d %d", channel, RXCount, micros(), rxmsg.id, rxmsg.flags.extended, rxmsg.len);
  Serial.print(CANdataDisplay);
  for (uint8_t i = 0; i < rxmsg.len; i++) {
    char CANBytes[4];
    sprintf(CANBytes, " %02X", rxmsg.buf[i]);
    Serial.print(CANBytes);
  }
  Serial.println();
}
