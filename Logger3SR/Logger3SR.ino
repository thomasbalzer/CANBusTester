#include <FlexCAN_T4.h>
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

//Define message from FlexCAN library
static CAN_message_t txmsg0;
static CAN_message_t txmsg1;
static CAN_message_t rxmsg0;
static CAN_message_t rxmsg1;

//Set up timing variables (Use prime numbers so they don't overlap)
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
uint32_t TXCount1 = 0;
uint32_t RXCount0 = 0;


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

void setup() {

  // put your setup code here, to run once:
  //Set baudrate
  Serial.begin(115200); delay(400);
  Can1.begin();
  Can0.begin();
  Can1.setBaudRate(BAUDRATE250K);
  Can0.setBaudRate(BAUDRATE250K);
  Can0.setMB((FLEXCAN_MAILBOX)0, RX, EXT);
  Can0.enableMBInterrupts();
  Can0.onReceive(MB0,canSniff);
  Can0.mailboxStatus();

  //Set message extension, ID, and length
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
  Can0.events();
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
    while (!Can1.write(txmsg1));
    uint32_t sysMicros = micros();
    YELLOW_LED_state = !YELLOW_LED_state;
    digitalWrite(YELLOW_LED_PIN, YELLOW_LED_state);

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
    //Serial.print("CAN1 Message Sent: ");
    //Serial.println(TXCount1);
  }

  if (((currentMillis - previousMillis) >= 90000) && newData == true) {
    Serial.println(RXCount0);
    RXCount0 = 0;
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

void canSniff(const CAN_message_t &rxmsg0) {
  newData = true;
  printFrame(rxmsg0, 0, RXCount0++);
  //RXCount0++;
  //Toggle the LED
  GREEN_LED_state = !GREEN_LED_state;
  digitalWrite(GREEN_LED_PIN, GREEN_LED_state);
}
