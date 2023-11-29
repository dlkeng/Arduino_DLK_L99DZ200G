/*
 * NAME: DLK_L99DZ200G_Library.ino
 *
 * WHAT:
 *  Program to demo operation of the AEK-MOT-MR200G1 Vehicle mirror controller board with the
 *  DLK_L99DZ200G Arduino library for the L99DZ200G device.
 *
 *  This program controls 3 motors connected to the OUT1, OUT2, OUT3, OUT6 outputs, the 2
 *  lights outputs on OUT7 and OUT8, mirror heater output on OUTGH, and electrochromic
 *  mirror control output. This is via command line commands or CAN bus commands.
 *
 * SPECIAL CONSIDERATIONS:
 *  Inspired by:
 *          - SPC582Bxx_RLA_AEK-MOT-MR200G1_Doorzone\main.c
 *          - SPC58ECxx_RLA 4M_CAN_cmd_sender_for_MR200G\main.c
 *
 * AUTHOR:
 *  D.L. Karmann
 *
 * MODIFIED:
 *
 */

/*        AEK_MOT_MR200G1                     Nano / Nano Every
          ---------------                  _________________________
                                          |                         |
                                         -|TX0[D1]               VIN|-
                                         -|RX0[D0]               GND|-
             ________                    -|RST                   RST|-
            |        |                   -|GND                   +5V|-
            |    NINT|-------TP8--------->|PD2[D2]              [A7]|<-----TP13------ YPOS
            |        |                   -|PD3[D3]~             [A6]|<-----TP14------ XPOS
            |  NRESET|-------TP6--------->|PD4[D4]   [SCL/A5/D19]PC5|-
            |        |   R57 removed     -|PD5[D5]~  [SDA/A4/D18]PC4|-
            |        |                   -|PD6[D6]~      [A3/D17]PC3|------> DEBUG_PIN
            |     LIN|<------Qxx----------|PD7[D7]       [A2/D16]PC2|------------------.
            |        |       LED_HB <-----|PB0[D8]       [A1/D15]PC1|<--------------.  |
            |        |                   -|PB1[D9]~      [A0/D14]PC0|<----------.   |  |
            |        |                   -|PB2[D10]~            AREF|-          |   |  |
            |      DI|<------TP18---------|PB3[D11]~            3.3V|-          |   |  |
            |      DO|-------TP17-------->|PB4[D12]         [D13]PB5|------.    |   |  |
            |        |                    |         .-----.         |      |    |   |  |
            |        |                    |_________| USB |_________|      |    |   |  |
            |        |                              '-----'                |    |   |  |
            |        |                     Arduino Nano / Nano Every       |    |   |  |
            |     SCK|<------TP16------------------------------------------'    |   |  |
            |  CM/DIR|<------TP15-----------------------------------------------'   |  |
            |     5V1|-------TP5----------------------------------------------------'  |
            |     CSN|<------TP19------------------------------------------------------'
            |        |
            |________|                                | |
            L99DZ200G                                 | |
                                           _________________________
                                          |                         |
                                         -|TX0[D1]               VIN|-
                                         -|RX0[D0]               GND|-
                                         -|RST                   RST|-
                                         -|GND                   +5V|-
                                         -|PD2[D2]              [A7]|-
                                         -|PD3[D3]              [A6]|-
                                         -|PD4[D4]   [SCL/A5/D19]PC5|-
                                         -|PD5[D5]   [SDA/A4/D18]PC4|-
                                         -|PD6[D6]       [A3/D17]PC3|-
                                         -|PD7[D7]       [A2/D16]PC2|-
             ________        LED_HB <-----|PB0[D8]       [A1/D15]PC1|-
            |        |                   -|PB1[D9]       [A0/D14]PC0|-
            |     ~CS|<-------------------|PB2[D10]             AREF|-
            |      SI|<-------------------|PB3[D11]             3.3V|-
            |      SO|------------------->|PB4[D12]         [D13]PB5|------.
            |        |                    |         .-----.         |      |
            |        |                    |_________| USB |_________|      |
            |        |                              '-----'                |
            |        |                    Arduino Nano / Nano Every        |
            |        |                                                     |
            |     SCK|<----------------------------------------------------'
            |________|
             MCP2515
 */

/*        AEK_MOT_MR200G1      Teensy
       ---------------                                               ________ 
                                                                    |        |
                .--------------------------------------TP16-------->|SCK     |
                |      ________________________                     |        |
                |     |                        |                    |        |
                '-----|PC5[D13]        [D12]PC7|<------TP17---------|DO      |
     DEBUG_PIN <------|PD1[D14/A0]     [D11]PC6|-------TP18-------->|DI      |
  YPOS-----TP13------>|PC0[D15/A1]    ~[D10]PC4|-                   |        |
  XPOS-----TP14------>|PB0[D16/A2]     ~[D9]PC3|-                   |        |
                     -|PB1[D17/A3]      [D8]PD3|-----> LED_HB       |        |
                     -|PB3[D18/A4]      [D7]PD2|<------TP6----------|NRESET  |
                     -|PB2[D19/A5]     ~[D6]PD4|-   R57 removed     |        |
                     -|PD5[D20/A6]~    ~[D5]PD7|-                   |        |
                     -|PD6[D21/A7]~   ~[D4]PA13|-------Qxx--------->|LIN     |
                     -|PC1[D22/A8]~   ~[D3]PA12|-                   |        |
            .-------->|PC2[D23/A9]~     [D2]PD0|<------TP5----------|5V1     |
            |        -|3.3V            [D1]PB17|<------TP8----------|NINT    |
            |        -|AGND            [D0]PB16|-------TP35-------->|CSN     |
            |        -|VIN                  GND|-                   |        |
            |         |         .-----.        |                    |        |
            |         |_________| USB |________|                    |        |
            |                   '-----'                             |        |
            |                Teensy 3.1                             |        |
            |                                                       |        |
            '------------------------------------------TP15-------->|CM/DIR  |
                                                                    |________|
                               | |                                  L99DZ200G
                               | |
                               | |                                   ________ 
                                                                    |        |
                .-------------------------------------------------->|SCK     |
                |      ________________________                     |        |
                |     |                        |                    |        |
                '-----|PC5[D13]        [D12]PC7|<-------------------|SO      |
                     -|PD1[D14/A0]     [D11]PC6|------------------->|SI      |
                     -|PC0[D15/A1]    ~[D10]PC4|------------------->|~CS     |
                     -|PB0[D16/A2]     ~[D9]PC3|-                   |        |
                     -|PB1[D17/A3]      [D8]PD3|-----> LED_HB       |________|
                     -|PB3[D18/A4]      [D7]PD2|-                    MCP2515
                     -|PB2[D19/A5]     ~[D6]PD4|-
                     -|PD5[D20/A6]~    ~[D5]PD7|-
                     -|PD6[D21/A7]~   ~[D4]PA13|-
                     -|PC1[D22/A8]~   ~[D3]PA12|-
                     -|PC2[D23/A9]~     [D2]PD0|-
                     -|3.3V            [D1]PB17|-
                     -|AGND            [D0]PB16|-
                     -|VIN                  GND|-
                      |         .-----.        | 
                      |_________| USB |________| 
                                '-----'          
                             Teensy 3.1          
 */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#ifdef ARDUINO_AVR_NANO_EVERY
//    For Arduino Nano Every
#endif

#ifdef ARDUINO_AVR_ATmega4809
//    For Microchip Curiosity Nano using ATmega4809 in MegaCoreX in Arduino
#endif

#ifdef ARDUINO_ARCH_AVR
//    For standard Arduino AVR boards (UNO, Nano, etc.)
#include <PinChangeInterrupt.h>
#endif

#ifdef ARDUINO_ARCH_MEGAAVR
//    For Arduino Nano Every
//    For Microchip Curiosity Nano using ATmega4809 in MegaCoreX in Arduino
#endif

#include <CommandLine.h>
#include <DLK_L99DZ200G.h>
#include <DLK_MCP2515.h>    // MCP2515 CAN Bus library

#include "CANCommunication.h"

#define TITLE_MSG           "DLK AEK-MOT-MR200G1 Demo"

#define LED_ON      HIGH
#define LED_OFF     LOW

#define HEARTBEAT_OFF_INTERVAL  950     // mS
#define HEARTBEAT_ON_INTERVAL   50      // mS
#define HB_OUT                  99
#define START_TIMEOUT           5000    // mS

#define IDLE                    0
#define BRAKE_ALL               1
#define X_CLOCKWISE             2
#define X_C_CLOCKWISE           3
#define Y_CLOCKWISE             4
#define Y_C_CLOCKWISE           5
#define CENTER_MIRROR           6
#define F_FOLD                  7
#define F_UNFOLD                8
#define HEATER_ON               9
#define HEATER_OFF              10
#define ECV_ON                  11
#define ECV_OFF                 12
#define ARROWS_FLASHING         13
#define DEMO_ON                 14
#define ARROWS_ON               15
#define ARROWS_OFF              16

// specify pins to use
#ifdef __AVR__      // this includes Arduino Nano Every MCU
#define LED_PIN                 8           // the heartbeat LED pin (LED_BUILTIN is used for SPI SCK)
#define L99DZ200G_INT_PIN       2           // INT0
#define L99DZ200G_NRST_PIN      4
#define L99DZ200G_DIR_PIN       A0
#define L99DZ200G_5V1_PIN       A1
#define L99DZ200G_CS_PIN        A2
#define MR200G1_X_POS_PIN       A6
#define MR200G1_Y_POS_PIN       A7
#define L99DZ200G_WAKE_PIN      7
#define MCP2515_CS_PIN          10
#define DEBUG_PIN               A3          // the Debug pin
#endif

// specify pins to use
#ifdef TEENSYDUINO
#define LED_PIN                 8           // the heartbeat LED pin (onboard LED pin is used for SPI SCK)
#define L99DZ200G_CS_PIN        0
#define L99DZ200G_INT_PIN       1
#define L99DZ200G_5V1_PIN       2
#define L99DZ200G_WAKE_PIN      4
#define L99DZ200G_NRST_PIN      7
#define MCP2515_CS_PIN          10
#define MR200G1_Y_POS_PIN       15  // A1
#define MR200G1_X_POS_PIN       16  // A2
#define L99DZ200G_DIR_PIN       23  // A9
#define DEBUG_PIN               14          // the Debug pin
#endif

#define DEBUG_HI()          digitalWrite(DEBUG_PIN, HIGH)
#define DEBUG_LO()          digitalWrite(DEBUG_PIN, LOW)
#define DEBUG_TOGL()        digitalWrite(DEBUG_PIN, !digitalRead(DEBUG_PIN))

#define SPI_CLOCK           250000          // 250 Kbps
#define CAN_SPEED           CAN_250KBPS

DLK_MCP2515 Mcp2515(SPI_CLOCK, MCP2515_CS_PIN);

uint8_t Len = 0;    // length of received buffer
uint8_t Buf[8];     // Buffer to hold up to 8 bytes of data
uint32_t CanID;     // Can message ID

CommandLine CmdLine(Serial);    // setup CommandLine to use standard Arduino Serial and echo on

DLK_L99DZ200G L99dz200g(SPI_CLOCK, L99DZ200G_CS_PIN);

uint8_t OutHB = 0;                  // OUTn to be used with heartbeat LED
volatile bool L99DZ200G_IntFlag = false;
volatile bool L99DZ200G_ResetFlag = false;

void L99DZ200G_Int(void)
{
    if (digitalRead(L99DZ200G_5V1_PIN) && digitalRead(L99DZ200G_NRST_PIN) && (!L99dz200g.L99DZ200G_WatchdogRunning()))
    {
        L99DZ200G_IntFlag = true;
    }
}

void L99DZ200G_Reset(void)
{
    static bool in_reset_cnt = false;

    if (digitalRead(L99DZ200G_5V1_PIN) && (!L99dz200g.L99DZ200G_WatchdogRunning()))
    {
        if (in_reset_cnt)   // real L99DZ200G wake from VBAT_Standby reset
        {
            L99DZ200G_ResetFlag = true;
            in_reset_cnt = false;
        }
        else    // power-on reset edge, awaiting real L99DZ200G wake from VBAT_Standby reset
        {
            in_reset_cnt = true;
        }
    }
}

void setup()
{
    uint32_t start_time = millis();

    // init heartbeat LED
    pinMode(LED_PIN, OUTPUT);
    LED_off();

    // init Debug pin
    pinMode(DEBUG_PIN, OUTPUT);
    DEBUG_LO();

    // init L99DZ200G Int input pin
    pinMode(L99DZ200G_INT_PIN, INPUT_PULLUP);

    // init L99DZ200G NRESET input pin
    pinMode(L99DZ200G_NRST_PIN, INPUT_PULLUP);

    // init L99DZ200G 5V1 input pin
    pinMode(L99DZ200G_5V1_PIN, INPUT_PULLUP);

    // init L99DZ200G wakeup output pin
    pinMode(L99DZ200G_WAKE_PIN, OUTPUT);
    digitalWrite(L99DZ200G_WAKE_PIN, LOW);

    // init L99DZ200G CM_DIR output pin
    pinMode(L99DZ200G_DIR_PIN, OUTPUT);
    digitalWrite(L99DZ200G_DIR_PIN, LOW);

    // setup serial port
    Serial.begin(115200);

    // generate the sign-on banner
    Serial.println();
#if defined(TEENSYDUINO)
    delay(2000);                 // allow time for Arduino's serial window to re-connect
    Serial.println("Teensy MCU");
#elif defined(ARDUINO_AVR_NANO_EVERY)
    Serial.println("Arduino Nano Every MCU");
#elif defined(ARDUINO_ARCH_AVR)
    Serial.println("AVR MCU");
#endif

    Serial.println(F("MR200G1_Demo.ino"));

    Serial.println(F(TITLE_MSG));

    delay(50);

    PulseWake();    // initiate possible wake-up

    while (!digitalRead(L99DZ200G_5V1_PIN))
    {
        if ((millis() - start_time) == START_TIMEOUT)
        {
            Serial.println(F("MR200G1 5V1 Power On Failed!"));
        }
        delay(1);
    }

    // setup for L99DZ200G interrupt
    attachInterrupt(digitalPinToInterrupt(L99DZ200G_INT_PIN), L99DZ200G_Int, FALLING);

#if defined(ARDUINO_AVR_NANO_EVERY) || defined(TEENSYDUINO)
    // setup for L99DZ200G reset interrupt
    attachInterrupt(digitalPinToInterrupt(L99DZ200G_NRST_PIN), L99DZ200G_Reset, RISING);
#else
    // setup for L99DZ200G reset
    attachPCINT(digitalPinToPinChangeInterrupt(L99DZ200G_NRST_PIN), L99DZ200G_Reset, RISING);
#endif

    delay(5);       // allow power to stabilize in L99DZ200G

    // Initialize L99DZ200G
    if (L99DZ200G_Init() != L99DZ200G_OK)
    {
        Serial.println(F("L99DZ200G_Init() failed!"));
        while (1)
        {
            yield();
        }
    }

    // Initialize MCP2515 running at 8MHz with a baudrate of 250kb/s
    if (Mcp2515.MCP2515_Init(CAN_SPEED) == MCP2515_OK)
    {
        Serial.println(F("MCP2515 Initialized Successfully!"));
    }
    else
    {
        Serial.println(F("Error Initializing MCP2515..."));
        while (1)
        {
            yield();
        }
    }
}

void loop()
{
    static bool new_prompt = true;
    static uint8_t last_gsb = 0xff;
    static uint32_t last_sr1 = 0xff0000;
    static uint32_t last_sr2 = 0xff0000;
    static uint32_t last_sr3 = 0xff0000;
    static uint32_t last_sr4 = 0xff0000;
    static uint32_t last_sr5 = 0xff0000;
    uint8_t gsb;
    uint32_t reg;
    CAN_FRAME frame;

    if (new_prompt)
    {
        new_prompt = false;
        //
        // Print a prompt to the console.
        //
        Serial.println();
        Serial.print(F("-> "));
    }

    new_prompt = CmdLine.DoCmdLine();

    if (L99dz200g.L99DZ200G_CheckWdogExpired() && L99dz200g.L99DZ200G_WatchdogRunning())  // process watchdog
    {
        gsb = L99dz200g.L99DZ200G_GlobalStatusByte();
        if (gsb != GSB_GSBN_MASK)
        {
            // Note: In case of a watchdog failure, the Fail Safe mode (GSB.FS)will be entered
            //       and the watchdog trigger time will be reset to 10 mS. In order to exit
            //       Fail Safe mode, the watchdog trigger time being used need to be set
            //       to 10 mS as well, and then the SR1 register needs to be cleared (Read & Clear).
            if (gsb != last_gsb)
            {
DEBUG_TOGL();
                last_gsb = gsb;
                Serial.print("GSB: ");
                Print0xHexByteln(gsb);
DEBUG_TOGL();
            }

            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR1) & FULL_REG_MASK;
            if ((reg != 0) && (reg != last_sr1))
            {
DEBUG_TOGL();
                last_sr1 = reg;
                Serial.print("SR1: ");
                Print0xHex24ln(reg);
DEBUG_TOGL();
            }

            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR2) & FULL_REG_MASK;
            if ((reg != 0) && (reg != last_sr2))
            {
                last_sr2 = reg;
                Serial.print("SR2: ");
                Print0xHex24ln(reg);
            }

            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR3) & FULL_REG_MASK;
            if ((reg != 0) && (reg != last_sr3))
            {
                last_sr3 = reg;
                Serial.print("SR3: ");
                Print0xHex24ln(reg);
            }

            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR4) & FULL_REG_MASK;
            if ((reg != 0) && (reg != last_sr4))
            {
                last_sr4 = reg;
                Serial.print("SR4: ");
                Print0xHex24ln(reg);
            }

            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR5) & FULL_REG_MASK;
            if ((reg != 0) && (reg != last_sr5))
            {
                last_sr5 = reg;
                Serial.print("SR5: ");
                Print0xHex24ln(reg);
            }
        }
    }

    if (L99DZ200G_IntFlag)
    {
        L99DZ200G_IntFlag = false;
        L99DZ200G_ResetFlag = false;

        // Re-initialize L99DZ200G
        if (L99DZ200G_Init() != L99DZ200G_OK)
        {
            Serial.println(F("L99DZ200G_Init() failed!"));
            while (1)
            {
                yield();    // needed by ESP8266 to prevent Soft WDT reset "rst cause:2, boot mode:(3,6)"
            }
        }
        Serial.println("L99DZ200G Interrupt!");
    }
    else if (L99DZ200G_ResetFlag)
    {
        L99DZ200G_ResetFlag = false;
        L99DZ200G_IntFlag = false;

        delay(5);       // allow power to stabilize in L99DZ200G

        // Re-initialize L99DZ200G
        if (L99DZ200G_Init() != L99DZ200G_OK)
        {
            Serial.println(F("L99DZ200G_Init() failed!"));
            while (1)
            {
                yield();    // needed by ESP8266 to prevent Soft WDT reset "rst cause:2, boot mode:(3,6)"
            }
        }
        Serial.println("L99DZ200G Reset!");
    }

    // uses polling of MCP2515 to determine if CAN data available
    if (Mcp2515.MCP2515_Recv(&frame) == MCP2515_OK) // check if data is coming in
    {
        ProcessCanMsg(&frame);
    }

    // do other stuff here

    // do Heartbeat
    DoHeartbeat();
}

void ProcessCanMsg(CAN_FRAME * frame)
{
    uint32_t * cmd_ptr;
    uint32_t cmd_data;
    uint8_t cmd = IDLE;

    // Read data, Len: data length, Buf: data buffer
    CanID = frame->can_id;
    Len = frame->can_dlc;
    memcpy(Buf, frame->can_data, Len);

    Serial.print(F("CAN ID: 0x"));
    Serial.print(CanID, HEX);       // print the CAN ID in HEX

    Serial.print(F("    Data Length: ")); // Print the length of the received data
    Serial.print(Len);
    Serial.print(F("    "));

    for (int i = 0; i < Len; i++)    // loop on the incoming data to print each byte
    {
        Serial.print(Buf[i]);
//        Serial.print(Buf[i], HEX);
        if (i < Len - 1)
        {
            Serial.print(',');      // Separate the numbers for readability
        }
    }
    Serial.print(F(" ("));
    cmd_ptr = (uint32_t *)frame->can_data;
    cmd_data = *cmd_ptr;
    Print0xHex32(cmd_data);
    Serial.println(')');

    if (CanID == ID_MR_200G)
    {
        switch (cmd_data)
        {
            case AEK_MOT_MR200G1_DEMO_ON_CMD:
                cmd = DEMO_ON;
                break;
            case AEK_MOT_MR200G1_DEMO_OFF_CMD:
                cmd = IDLE;
                break;
            case AEK_MOT_MR200G1_MIRROR_FOLDING_CMD:
                cmd = F_FOLD;
                break;
            case AEK_MOT_MR200G1_MIRROR_UNFOLDING_CMD:
                cmd = F_UNFOLD;
                break;
            case AEK_MOT_MR200G1_MIRROR_X_CK_CMD:
                cmd = X_CLOCKWISE;
                break;
            case AEK_MOT_MR200G1_MIRROR_X_CCK_CMD:
                cmd = X_C_CLOCKWISE;
                break;
            case AEK_MOT_MR200G1_MIRROR_Y_CK_CMD:
                cmd = Y_CLOCKWISE;
                break;
            case AEK_MOT_MR200G1_MIRROR_Y_CCK_CMD:
                cmd = Y_C_CLOCKWISE;
                break;
            case AEK_MOT_MR200G1_MIRROR_BRK_CMD:
                cmd = BRAKE_ALL;
                break;
            case AEK_MOT_MR200G1_MIRROR_CENTER_CMD:
                cmd = CENTER_MIRROR;
                break;
            case AEK_MOT_MR200G1_BULBS_ON_CMD:
                cmd = ARROWS_ON;
                break;
            case AEK_MOT_MR200G1_BULBS_OFF_CMD:
                cmd = ARROWS_OFF;
                break;
            case AEK_MOT_MR200G1_BULBS_FLASH_CMD:
                cmd = ARROWS_FLASHING;
                break;
            case AEK_MOT_MR200G1_HEATER_ON_CMD:
                cmd = HEATER_ON;
                break;
            case AEK_MOT_MR200G1_HEATER_OFF_CMD:
                cmd = HEATER_OFF;
                break;
            case AEK_MOT_MR200G1_ECV_ON_CMD:
                cmd = ECV_ON;
                break;
            case AEK_MOT_MR200G1_ECV_OFF_CMD:
                cmd = ECV_OFF;
                break;

            default:
                return;
        }
        DoMirrorOperation(cmd);
    }
}

// do Heartbeat
void DoHeartbeat(void)
{
    static uint32_t last_HB_tick = 0;
    static uint8_t last_HB_state = false;

    if (last_HB_state)
    {
        if (TIMER_EXPIRED(last_HB_tick, HEARTBEAT_ON_INTERVAL))
        {
            LED_off();      // off
            if (OutHB && L99dz200g.L99DZ200G_WatchdogRunning())
            {
                L99dz200g.L99DZ200G_HSOutputsControl(OFF_OUT, OutHB);
            }
            last_HB_state = false;
            last_HB_tick = millis();
        }
    }
    else
    {
        if (TIMER_EXPIRED(last_HB_tick, HEARTBEAT_OFF_INTERVAL))
        {
            LED_on();       // on
            if (OutHB && L99dz200g.L99DZ200G_WatchdogRunning())
            {
                L99dz200g.L99DZ200G_HSOutputsControl(ON_OUT, OutHB);
            }
            last_HB_state = true;
            last_HB_tick = millis();
        }
    }
}

void LED_on(void)
{
    digitalWrite(LED_PIN, LED_ON);
}

void LED_off(void)
{
    digitalWrite(LED_PIN, LED_OFF);
}

