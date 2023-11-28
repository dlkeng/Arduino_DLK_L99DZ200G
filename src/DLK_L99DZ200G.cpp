/** \file DLK_L99DZ200G.cpp */
/*
 * NAME: DLK_L99DZ200G.cpp
 *
 * WHAT:
 *  Arduino L99DZ200G driver library functions for L99DZ200G driver SPI operations.
 *
 * SPECIAL CONSIDERATIONS:
 *  Derived from: STMicro AutoDevKit examples
 *
 * AUTHOR:
 *  D.L. Karmann
 *
 * MODIFIED:
 *
 */

#include <SPI.h>
#include "DLK_L99DZ200G.h"

// outside of DLK_L99DZ200G class

bool WatchdogRunning = true;

// private static class variable must be initialized outside of class
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266)
bool DLK_L99DZ200G::SPI_initted = false;
#endif

#if defined(PHILHOWER_RP2040) || defined(ESP32)
bool DLK_L99DZ200G::SPI0_initted = false;
bool DLK_L99DZ200G::SPI1_initted = false;
#endif

#if defined(ESP32)
SPIClass SPIH = SPIClass(HSPI); // SPI1 appears to be used somewhere and crashes ESP32 code if used here!!!
#endif

// DLK_L99DZ200G Class members

// Constructor
DLK_L99DZ200G::DLK_L99DZ200G(uint32_t spi_speed, uint8_t cs_pin, bool hw_cs_pin, uint8_t which_spi)
{
    CS_pin = cs_pin;
    HW_CS_pin = hw_cs_pin;
    WhichSPI = which_spi;

#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    HW_CS_pin = false;
#endif

    if (HW_CS_pin)
    {
        // the following SPI mode is OK for SW CS
        // the following SPI mode is needed by HW CS for continuous CS assertion during all bytes in a transaction
        SPISettings spi_settings(spi_speed, MSBFIRST, SPI_MODE3);
        SPI_Settings = spi_settings;
    }
    else    // SW CS
    {
        // the following SPI mode is OK for SW CS
        // but for HW CS, with this SPI mode, the CS is only asserted separately for each byte in the transaction
        SPISettings spi_settings(spi_speed, MSBFIRST, SPI_MODE0);
        SPI_Settings = spi_settings;
    }
}

// Initialize L99DZ200G
uint8_t DLK_L99DZ200G::L99DZ200G_Init(void)
{
    uint8_t rslt = L99DZ200G_OK;

   // setup SPI
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266)
    SPI_dev = &SPI;

    if (!SPI_initted)
    {
        SPI_dev->begin();
        SPI_initted = true;
    }
#endif

#ifdef PHILHOWER_RP2040
    if (WhichSPI == SPI1_NUM)
    {
        SPI_dev = &SPI1;        // SPI1
    }
    else    // SPI0_NUM
    {
        SPI_dev = &SPI;         // SPI0
    }

    if (SPI_dev == &SPI)
    {
        if (!SPI0_initted)
        {
            if (HW_CS_pin)
            {
                if (!SPI_dev->setCS(CS_pin))
                {
                    return L99DZ200G_FAIL;      // invalid CS pin for SPI port
                }
            }
            SPI_dev->begin(HW_CS_pin);
            SPI0_initted = true;
        }
    }
    else if (SPI_dev == &SPI1)
    {
        if (!SPI1_initted)
        {
            if (HW_CS_pin)
            {
                if (!SPI_dev->setCS(CS_pin))
                {
                    return L99DZ200G_FAIL;      // invalid CS pin for SPI port
                }
            }
            SPI_dev->begin(HW_CS_pin);
            SPI1_initted = true;
        }
    }
#endif

#ifdef ESP32
    if (WhichSPI == SPI1_NUM)
    {
        SPI_dev = &SPIH;        // HSPI
    }
    else    // SPI0_NUM
    {
        SPI_dev = &SPI;         // VSPI
    }

    if (SPI_dev == &SPI)
    {
        if (!SPI0_initted)
        {
            SPI_dev->begin(SCK, MISO, MOSI, CS_pin);
            SPI0_initted = true;
        }
    }
    else if (SPI_dev == &SPIH)
    {
        if (!SPI1_initted)
        {
            SPI_dev->begin();   // use default HSPI SPI pins
            SPI1_initted = true;
        }
    }
#endif

    if (!HW_CS_pin)
    {
        pinMode(CS_pin, OUTPUT);
        digitalWrite(CS_pin, HIGH);
    }

    return rslt;
}

// Initiate L99DZ200G SPI transaction
inline void DLK_L99DZ200G::L99DZ200G_StartSPI(void)
{
    SPI_dev->beginTransaction(SPI_Settings);
    if (!HW_CS_pin)
    {
        digitalWrite(CS_pin, LOW);
    }
}

// Terminate L99DZ200G SPI transaction
inline void DLK_L99DZ200G::L99DZ200G_EndSPI(void)
{
    if (!HW_CS_pin)
    {
        digitalWrite(CS_pin, HIGH);
    }
    SPI_dev->endTransaction();
#ifdef TEENSYDUINO
    delayMicroseconds(2);       // required delay between SPI transactions (not specified ???)
#endif
}

// Convert uint32_t value to array of bytes
void Uint32ToArray(uint32_t val, uint8_t * bytes)
{
    bytes[0] = (val >> 24) & 0xff;
    bytes[1] = (val >> 16) & 0xff;
    bytes[2] = (val >> 8) & 0xff;
    bytes[3] = (val >> 0) & 0xff;
}

// Convert array of bytes to uint32_t value
void ArrayToUint32(uint8_t * bytes, uint32_t * val)
{
    *val = ((uint32_t)bytes[0] << 24)  |
            ((uint32_t)bytes[1] << 16) |
            ((uint32_t)bytes[2] << 8)  |
            ((uint32_t)bytes[3] << 0);
}

// Read from specified L99DZ200G ROM address
uint8_t DLK_L99DZ200G::L99DZ200G_ReadRomAddress(uint8_t addr)
{
    uint8_t ret;

    L99DZ200G_StartSPI();
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    uint8_t spi_data[2] = { SPI_DUMMY_BYTE };

    spi_data[0] = SET_SPI_DEV_INFO(addr);
    SPI_dev->transfer(spi_data, sizeof(spi_data));
    ret = spi_data[1];
#else   // Raspberry Pi Pico
    uint8_t spi_txdata[2] = { SPI_DUMMY_BYTE };
    uint8_t spi_rxdata[2];

    spi_txdata[0] = SET_SPI_DEV_INFO(addr);
    SPI_dev->transfer(spi_txdata, spi_rxdata, sizeof(spi_txdata));
    ret = spi_rxdata[1];
#endif
    L99DZ200G_EndSPI();

    return ret;
}

// Read from specified L99DZ200G register
uint32_t DLK_L99DZ200G::L99DZ200G_ReadRegister(uint8_t reg)
{
    uint32_t ret;

    L99DZ200G_StartSPI();
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    uint8_t spi_data[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };

    spi_data[0] = SET_SPI_RD(reg);
    SPI_dev->transfer(spi_data, sizeof(spi_data));
    GlobalStatusRegister = spi_data[0];
    ArrayToUint32(spi_data, &ret);
#else   // Raspberry Pi Pico
    uint8_t spi_txdata[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };
    uint8_t spi_rxdata[SPI_TRANSACTION_SIZE];

    spi_txdata[0] = SET_SPI_RD(reg);
    SPI_dev->transfer(spi_txdata, spi_rxdata, sizeof(spi_txdata));
    GlobalStatusRegister = spi_rxdata[0];
    ArrayToUint32(spi_rxdata, &ret);
#endif
    L99DZ200G_EndSPI();

    return ret;
}

// Write specified value to specified L99DZ200G Control register
void DLK_L99DZ200G::L99DZ200G_WriteControlRegister(uint8_t reg, uint32_t val)
{
    L99DZ200G_StartSPI();
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    uint8_t spi_data[SPI_TRANSACTION_SIZE];

    Uint32ToArray(val, spi_data);
    spi_data[0] = SET_SPI_WR(reg);
    SPI_dev->transfer(spi_data, sizeof(spi_data));
    GlobalStatusRegister = spi_data[0];
#else   // Raspberry Pi Pico
    uint8_t spi_txdata[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };
    uint8_t spi_rxdata[SPI_TRANSACTION_SIZE];

    Uint32ToArray(val, spi_txdata);
    spi_txdata[0] = SET_SPI_WR(reg);
    SPI_dev->transfer(spi_txdata, spi_rxdata, sizeof(spi_txdata));
    GlobalStatusRegister = spi_rxdata[0];
#endif
    L99DZ200G_EndSPI();
}

// Modify specified L99DZ200G Control register with specified mask and specified data (read-modify-write)
void DLK_L99DZ200G::L99DZ200G_ModifyControlRegister(uint8_t reg, uint32_t mask, uint32_t data)
{
    uint32_t tmp_data;

    // read
    tmp_data = L99DZ200G_ReadRegister(reg);

    // modify
    tmp_data &= ~mask;
    tmp_data |= (data & mask);

    // write
    L99DZ200G_WriteControlRegister(reg, tmp_data);
}

// Read and clear specified bits in specified L99DZ200G register
void DLK_L99DZ200G::L99DZ200G_ReadClearRegister(uint8_t reg, uint32_t mask)
{
    L99DZ200G_StartSPI();
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    uint8_t spi_data[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };

    Uint32ToArray(mask, spi_data);
    spi_data[0] = SET_SPI_RD_CLR(reg);
    SPI_dev->transfer(spi_data, sizeof(spi_data));
    GlobalStatusRegister = spi_data[0];
#else   // Raspberry Pi Pico
    uint8_t spi_txdata[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };
    uint8_t spi_rxdata[SPI_TRANSACTION_SIZE];

    Uint32ToArray(mask, spi_txdata);
    spi_txdata[0] = SET_SPI_RD_CLR(reg);
    SPI_dev->transfer(spi_txdata, spi_rxdata, sizeof(spi_txdata));
    GlobalStatusRegister = spi_rxdata[0];
#endif
    L99DZ200G_EndSPI();
}

// Delays for the specified number of milliseconds, maintaining the device in ACTIVE state
void DLK_L99DZ200G::L99DZ200G_Delay(uint16_t msec)
{
    uint16_t time = 0;

    while (time < msec)
    {
        delay(1);
        L99DZ200G_CheckWdogExpired();
        ++time;
    }
}

// Check if watchdog is expired
bool DLK_L99DZ200G::L99DZ200G_CheckWdogExpired(void)
{
    if (TIMER_EXPIRED(WdogTick, WdogTriggerTime))
    {
        WdogTick = millis();
        if (WatchdogRunning)
        {
            L99DZ200G_WdogTrigger();
        }
        return true;
    }

    return false;
}

// Trigger watchdog - CR1
void DLK_L99DZ200G::L99DZ200G_WdogTrigger(void)
{
    uint32_t tmp_data;

#ifdef WDOG_TRIGGER_CR1
    // read CR1 (optionally, could instead use CONFIG register (L99DZ200G_CFR))
    tmp_data = L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // modify CR1 data
    tmp_data &= FULL_REG_MASK;
    tmp_data ^= CR1_WDC_MASK;       // toggle TRIG bit

    // write CR1
    L99DZ200G_WriteControlRegister(L99DZ200G_CR1, tmp_data);
#endif

#ifdef WDOG_TRIGGER_CFR
    // read CFR (optionally, could instead use CONFIG register (L99DZ200G_CR1))
    tmp_data = L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // modify CFR data
    tmp_data &= FULL_REG_MASK;
    tmp_data ^= CFR_WDC_MASK;       // toggle TRIG bit

    // write CFR
    L99DZ200G_WriteControlRegister(L99DZ200G_CFR, tmp_data);
#endif
}

// Set watchdog trigger time - CR2 (CFR)
// Table 14.
//  00: TSW1 = 7.5 to 12 mS   - early failure: 4.5 ms,  late failure: 20 mS (default) 100 Hz PIT (10 mS)
//  01: TSW2 = 37.5 to 60 mS  - early failure: 22.3 ms, late failure: 100 mS           20 Hz PIT (50 mS)
//  10: TSW3 = 75 to 120 mS   - early failure: 45 ms,   late failure: 200 mS           10 Hz PIT (100 mS)
//  11: TSW4 = 150 to 240 mS  - early failure: 90 ms,   late failure: 400 mS           5  Hz PIT (200 mS)
void DLK_L99DZ200G::L99DZ200G_SetWdogTime(uint8_t ttime)
{
    uint32_t tmp_dataCFR;
    uint32_t tmp_dataCR2;

    switch (ttime)
    {
        case WDOG_TIME_10MS:
            WdogTriggerTime = 10;
            break;
        case WDOG_TIME_50MS:
            WdogTriggerTime = 37;       //50;
            break;
        case WDOG_TIME_100MS:
            WdogTriggerTime = 75;       //100;
            break;
        case WDOG_TIME_200MS:
            WdogTriggerTime = 150;      //200;
            break;
    }

    // read CFR
    tmp_dataCFR = L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // modify CFR data to enable WDOG modifications
    tmp_dataCFR &= FULL_REG_MASK;
    tmp_dataCFR |= CFR_WD_CFG_EN_MASK;

    // read CR2
    tmp_dataCR2 = L99DZ200G_ReadRegister(L99DZ200G_CR2);

    // modify CR2 data
    tmp_dataCR2 &= ~CR2_WD_TIME_MASK;
    tmp_dataCR2 &= FULL_REG_MASK;
    tmp_dataCR2 |= (ttime & CR2_WD_TIME_MASK);

    // write CFR to enable modifying WDOG time
    L99DZ200G_WriteControlRegister(L99DZ200G_CFR, tmp_dataCFR);

    // write CR2 to modify WDOG time
    L99DZ200G_WriteControlRegister(L99DZ200G_CR2, tmp_dataCR2);

    WdogTick = millis();        // reset watchdog tick value
}

// Get watchdog trigger time - CR2
uint8_t DLK_L99DZ200G::L99DZ200G_GetWdogTime(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_CR2);
    reg_data = (reg_data & CR2_WD_TIME_MASK) >> CR2_WD_TIME_POS;

    return reg_data;
}

// Clear all L99DZ200G Status Registers
void DLK_L99DZ200G::L99DZ200G_ClearAllStatusRegisters(void)
{
    L99DZ200G_ReadClearRegister(L99DZ200G_CFR, SPI_DUMMY_BYTE);
//    L99DZ200G_ReadClearRegister(L99DZ200G_CFR, FULL_REG_MASK);
}

// Reset all L99DZ200G Control Registers to defaults
void DLK_L99DZ200G::L99DZ200G_ResetAllControlRegisters(void)
{
    L99DZ200G_StartSPI();
#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined(ESP32)
    uint8_t spi_data[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };

    spi_data[0] = SET_SPI_DEV_INFO(L99DZ200G_CFR);
    SPI_dev->transfer(spi_data, sizeof(spi_data));
    GlobalStatusRegister = spi_data[0];
#else   // Raspberry Pi Pico
    uint8_t spi_txdata[SPI_TRANSACTION_SIZE] = { SPI_DUMMY_BYTE };
    uint8_t spi_rxdata[SPI_TRANSACTION_SIZE];

    spi_txdata[0] = SET_SPI_DEV_INFO(L99DZ200G_CFR);
    SPI_dev->transfer(spi_txdata, spi_rxdata, sizeof(spi_txdata));
    GlobalStatusRegister = spi_rxdata[0];
#endif
    L99DZ200G_EndSPI();
}

// Set L99DZ200G V2 Voltage Regulator Configuration - CR1, CFR
void DLK_L99DZ200G::L99DZ200G_V2_Config(uint32_t mode_config, uint32_t type_config)
{
    // set V2 mode
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, CR1_V2_MODE_MASK, mode_config);

    // set V2 type
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, CFR_V2_CFG_MASK, type_config);
}

// Control for OUT1-OUT2-OUT3-OUT6 for motor control - CR4
void DLK_L99DZ200G::L99DZ200G_MotorDriver(uint8_t outputs, uint8_t motor_dir)
{
    uint32_t reg_data;

    switch (motor_dir)
    {
        case LEFT_DIRECTION:
            reg_data = ((uint32_t)ON_OUT << CR4_LS_OUT1_POS);   // OUT1 implied for opposite HS/LS
            switch (outputs)
            {
                case OUT2_X:
                    reg_data |= ((uint32_t)ON_OUT << CR4_HS_OUT2_POS);
                    break;
                case OUT3_Y:
                    reg_data |= ((uint32_t)ON_OUT << CR4_HS_OUT3_POS);
                    break;
                case OUT6_F:
                    reg_data |= ((uint32_t)ON_OUT << CR4_HS_OUT6_POS);
                    break;
                case OUT_XY:
                    reg_data |= (((uint32_t)ON_OUT << CR4_HS_OUT2_POS) | ((uint32_t)ON_OUT << CR4_HS_OUT3_POS));
                    break;
                case OUT_XF:
                    reg_data |= (((uint32_t)ON_OUT << CR4_HS_OUT2_POS) | ((uint32_t)ON_OUT << CR4_HS_OUT6_POS));
                    break;
                case OUT_YF:
                    reg_data |= (((uint32_t)ON_OUT << CR4_HS_OUT3_POS) | ((uint32_t)ON_OUT << CR4_HS_OUT6_POS));
                    break;

                default:
                    return;     // invalid output
            }
            break;

        case RIGHT_DIRECTION:
            reg_data = ((uint32_t)ON_OUT << CR4_HS_OUT1_POS);   // OUT1 implied for opposite HS/LS
            switch (outputs)
            {
                case OUT2_X:
                    reg_data |= ((uint32_t)ON_OUT << CR4_LS_OUT2_POS);
                    break;
                case OUT3_Y:
                    reg_data |= ((uint32_t)ON_OUT << CR4_LS_OUT3_POS);
                    break;
                case OUT6_F:
                    reg_data |= ((uint32_t)ON_OUT << CR4_LS_OUT6_POS);
                    break;
                case OUT_XY:
                    reg_data |= (((uint32_t)ON_OUT << CR4_LS_OUT2_POS) | ((uint32_t)ON_OUT << CR4_LS_OUT3_POS));
                    break;
                case OUT_XF:
                    reg_data |= (((uint32_t)ON_OUT << CR4_LS_OUT2_POS) | ((uint32_t)ON_OUT << CR4_LS_OUT6_POS));
                    break;
                case OUT_YF:
                    reg_data |= (((uint32_t)ON_OUT << CR4_LS_OUT3_POS) | ((uint32_t)ON_OUT << CR4_LS_OUT6_POS));
                    break;

                default:
                    return;     // invalid direction
            }
            break;

        case BRAKE:
            reg_data = ((uint32_t)OFF_OUT << CR4_HS_OUT1_POS) | ((uint32_t)OFF_OUT << CR4_LS_OUT1_POS) |
                       ((uint32_t)OFF_OUT << CR4_HS_OUT2_POS) | ((uint32_t)OFF_OUT << CR4_LS_OUT2_POS) |
                       ((uint32_t)OFF_OUT << CR4_HS_OUT3_POS) | ((uint32_t)OFF_OUT << CR4_LS_OUT3_POS) |
                       ((uint32_t)OFF_OUT << CR4_HS_OUT6_POS) | ((uint32_t)OFF_OUT << CR4_LS_OUT6_POS);
            break;

        default:
            return;     // invalid output
    }
    L99DZ200G_WriteControlRegister(L99DZ200G_CR4, reg_data);
}

// Set L99DZ200G High-Side/Low-Side Outputs Control - CR4
void DLK_L99DZ200G::L99DZ200G_HS_LS_OutputsControl(uint8_t output_type, uint8_t output)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    output_type &= 1;
    if ((output < OUT_1_HS) || (output > OUT_6_LS))
    {
        return;
    }

    switch (output)
    {
        case OUT_1_HS:
            reg_data = (uint32_t)output_type << CR4_HS_OUT1_POS;
            reg_mask = CR4_HL_OUT1_MASK;
            break;
        case OUT_1_LS:
            reg_data = (uint32_t)output_type << CR4_LS_OUT1_POS;
            reg_mask = CR4_HL_OUT1_MASK;
            break;

        case OUT_2_HS:
            reg_data = (uint32_t)output_type << CR4_HS_OUT2_POS;
            reg_mask = CR4_HL_OUT2_MASK;
            break;
        case OUT_2_LS:
            reg_data = (uint32_t)output_type << CR4_LS_OUT2_POS;
            reg_mask = CR4_HL_OUT2_MASK;
            break;

        case OUT_3_HS:
            reg_data = (uint32_t)output_type << CR4_HS_OUT3_POS;
            reg_mask = CR4_HL_OUT3_MASK;
            break;
        case OUT_3_LS:
            reg_data = (uint32_t)output_type << CR4_LS_OUT3_POS;
            reg_mask = CR4_HL_OUT3_MASK;
            break;

        case OUT_6_HS:
            reg_data = (uint32_t)output_type << CR4_HS_OUT6_POS;
            reg_mask = CR4_HL_OUT6_MASK;
            break;
        case OUT_6_LS:
            reg_data = (uint32_t)output_type << CR4_LS_OUT6_POS;
            reg_mask = CR4_HL_OUT6_MASK;
            break;

        default:
            return;
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR4, reg_mask, reg_data);
}

// Set L99DZ200G High-Side Outputs Control - CR5, CR6
void DLK_L99DZ200G::L99DZ200G_HSOutputsControl(uint8_t output_type, uint8_t output)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_7:
        case OUT_8:
        case OUT_9:
            switch (output_type)
            {
                case TIMER1_OUT:
                case TIMER2_OUT:
                case PWM1_OUT:
                case PWM2_OUT:
                case PWM3_OUT:
                case PWM4_OUT:
                case PWM5_OUT:
                case PWM6_OUT:
                case PWM7_OUT:
                    // Note: To set OUT_7, OUT_8, or OUT_9 to anything other than OFF_OUT, ON_OUT,
                    //       or DIR_OUT, the OUTn Constant Current Mode *must* be turned off!
                    //       (see 4.21) {this prevents SPI_INV_CMD error}
                    L99DZ200G_SetConstantCurrentModeControl(output, DISABLE);   // turn CCM off
                    break;
            }
    }

    switch (output)
    {
        case OUT_7:
            reg_data = (uint32_t)output_type << CR5_HS_OUT7_POS;
            reg_mask = CR5_HS_OUT7_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR5, reg_mask, reg_data);
            break;
        case OUT_8:
            reg_data = (uint32_t)output_type << CR5_HS_OUT8_POS;
            reg_mask = CR5_HS_OUT8_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR5, reg_mask, reg_data);
            break;
        case OUT_10:
            reg_data = (uint32_t)output_type << CR5_HS_OUT10_POS;
            reg_mask = CR5_HS_OUT10_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR5, reg_mask, reg_data);
            break;
        case OUT_GH:
            reg_data = ((uint32_t)output_type << CR5_GH_POS) & CR5_GH_MASK;
            reg_mask = CR5_GH_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR5, reg_mask, reg_data);
            break;

        case OUT_9:
            reg_data = (uint32_t)output_type << CR6_HS_OUT9_POS;
            reg_mask = CR6_HS_OUT9_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR6, reg_mask, reg_data);
            break;
        case OUT_13:
            reg_data = (uint32_t)output_type << CR6_HS_OUT13_POS;
            reg_mask = CR6_HS_OUT13_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR6, reg_mask, reg_data);
            break;
        case OUT_14:
            reg_data = (uint32_t)output_type << CR6_HS_OUT14_POS;
            reg_mask = CR6_HS_OUT14_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR6, reg_mask, reg_data);
            break;
        case OUT_15:
            reg_data = (uint32_t)output_type << CR6_HS_OUT15_POS;
            reg_mask = CR6_HS_OUT15_MASK;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR6, reg_mask, reg_data);
            break;

        default:
            break;
    }
}

// Set L99DZ200G Heater Output Control - CR5
void DLK_L99DZ200G::L99DZ200G_HeaterOutputControl(uint8_t output_type)
{
    L99DZ200G_HSOutputsControl(output_type, OUT_GH);
}

// Set L99DZ200G PWM Channel Frequency - CR12
void DLK_L99DZ200G::L99DZ200G_SetPWMFrequency(uint8_t pwm_chan, uint8_t pwm_freq)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (pwm_chan)
    {
        case PWM_CHAN1:
            reg_mask = CR12_PWM1_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM1_FREQ_POS;
            break;
        case PWM_CHAN2:
            reg_mask = CR12_PWM2_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM2_FREQ_POS;
            break;
        case PWM_CHAN3:
            reg_mask = CR12_PWM3_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM3_FREQ_POS;
            break;
        case PWM_CHAN4:
            reg_mask = CR12_PWM4_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM4_FREQ_POS;
            break;
        case PWM_CHAN5:
            reg_mask = CR12_PWM5_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM5_FREQ_POS;
            break;
        case PWM_CHAN6:
            reg_mask = CR12_PWM6_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM6_FREQ_POS;
            break;
        case PWM_CHAN7:
            reg_mask = CR12_PWM7_FREQ_MASK;
            reg_data = (uint32_t)pwm_freq << CR12_PWM7_FREQ_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR12, reg_mask, reg_data);
}

// Set L99DZ200G PWM Channel Duty Cycle - CR13 to CR16
void DLK_L99DZ200G::L99DZ200G_SetPWMDutyCycle(uint8_t pwm_chan, uint8_t pwm_duty)
{
    uint8_t reg;
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = map(pwm_duty, 0, 100, 0, 1023);  // map 0 to 100% duty cycle to 0 to 1023 register value

    switch (pwm_chan)
    {
        case PWM_CHAN1:
            reg = L99DZ200G_CR13;
            reg_mask = HI_PWM_DC_MASK;
            reg_data <<=  HI_PWM_DC_POS;
            break;
        case PWM_CHAN2:
            reg = L99DZ200G_CR13;
            reg_mask = LO_PWM_DC_MASK;
            reg_data <<= LO_PWM_DC_POS;
            break;
        case PWM_CHAN3:
            reg = L99DZ200G_CR14;
            reg_mask = HI_PWM_DC_MASK;
            reg_data <<=  HI_PWM_DC_POS;
            break;
        case PWM_CHAN4:
            reg = L99DZ200G_CR14;
            reg_mask = LO_PWM_DC_MASK;
            reg_data <<= LO_PWM_DC_POS;
            break;
        case PWM_CHAN5:
            reg = L99DZ200G_CR15;
            reg_mask = HI_PWM_DC_MASK;
            reg_data <<=  HI_PWM_DC_POS;
            break;
        case PWM_CHAN6:
            reg = L99DZ200G_CR15;
            reg_mask = LO_PWM_DC_MASK;
            reg_data <<= LO_PWM_DC_POS;
            break;
        case PWM_CHAN7:
            reg = L99DZ200G_CR16;
            reg_mask = HI_PWM_DC_MASK;
            reg_data <<=  HI_PWM_DC_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(reg, reg_mask, reg_data);
}

// Set Programmable timer Period and OnTime - CR2
void DLK_L99DZ200G::L99DZ200G_SetTimerConfig(uint8_t timer, uint8_t period, uint8_t restart, uint8_t dir, uint8_t ontime)
{
    uint32_t reg_data = 0;
    uint32_t reg_mask;

    switch (timer)
    {
        case TYPE_TIMER1:
            reg_mask = CR2_T1_MASK;
            if (restart == ENABLE)
            {
                reg_data |= (1UL << CR2_T1_RESTART_POS);
            }
            if (dir == ENABLE)
            {
                reg_data |= (1UL << CR2_T1_DIR_POS);
            }
            reg_data |= ((uint32_t)ontime << CR2_T1_ON_POS);
            reg_data |= ((uint32_t)period << CR2_T1_PER_POS);
        break;

        case TYPE_TIMER2:
            reg_mask = CR2_T2_MASK;
            if (restart == ENABLE)
            {
                reg_data |= (1UL << CR2_T2_RESTART_POS);
            }
            if (dir == ENABLE)
            {
                reg_data |= (1UL << CR2_T2_DIR_POS);
            }
            reg_data |= ((uint32_t)ontime << CR2_T2_ON_POS);
            reg_data |= ((uint32_t)period << CR2_T2_PER_POS);
            break;

        default:
            return;
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// Select L99DZ200G CM_DIR pin configuration - CR7
void DLK_L99DZ200G::L99DZ200G_CM_DIR_Config(uint8_t cm_dir)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR7_CM_DIR_MASK;
    reg_data = (uint32_t)cm_dir << CR7_CM_DIR_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR7, reg_mask, reg_data);
}

// Select L99DZ200G CM_DIR Current Monitor OUTn Selection - CR7
void DLK_L99DZ200G::L99DZ200G_CM_OUTn_Select(uint8_t out_cm)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR7_CM_SEL_MASK;
    reg_data = (uint32_t)out_cm << CR7_CM_SEL_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR7, reg_mask, reg_data);
}

// Set maximum voltage of ECV Electrochromic Controller - CFR
void DLK_L99DZ200G::L99DZ200G_Set_ECV_MaxVoltage(uint8_t ecv_type)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_ECV_HV_MASK;
    reg_data = (uint32_t)ecv_type << CFR_ECV_H_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Set drive voltage of ECV Electrochromic Controller - CR11
void DLK_L99DZ200G::L99DZ200G_Set_ECV_DriveVoltage(float target_volts)
{
    float vctrl_max;
    uint32_t reg_data;
    uint32_t reg_mask;

    // read CFR
    reg_data = L99DZ200G_ReadRegister(L99DZ200G_CFR);
    if (reg_data & CFR_ECV_HV_MASK)
    {
        vctrl_max = ECV_1_5_VOLTS;
    }
    else
    {
        vctrl_max = ECV_1_2_VOLTS;
    }

    if (target_volts > vctrl_max)
    {
        target_volts = vctrl_max;
    }
    else if (target_volts < 0)
    {
        target_volts = 0.0;
    }

    reg_data = (uint8_t)((target_volts * CR11_EC_VALUE_MASK) / vctrl_max);

    reg_mask = CR11_EC_VALUE_MASK;
    reg_data = (reg_data & CR11_EC_VALUE_MASK) << CR11_EC_VALUE_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Switch ON/OFF the ECV fast discharge - CR11
void DLK_L99DZ200G::L99DZ200G_Set_ECV_FastDischargeControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR11_ECV_LS_MASK;
    reg_data = (uint32_t)en_dis << CR11_ECV_LS_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Switch ON/OFF the ECV Overcurrent Recovery - CR11
void DLK_L99DZ200G::L99DZ200G_Set_ECV_OvercurrentRecoveryControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR11_ECV_OCR_MASK;
    reg_data = (uint32_t)en_dis << CR11_ECV_OCR_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Switch ON/OFF the ECV functionality
void DLK_L99DZ200G::L99DZ200G_Set_ECV_Control(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR11_EC_ON_MASK;
    reg_data = (uint32_t)en_dis << CR11_EC_ON_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Get drive voltage of ECV Electrochromic Controller - CR11
float DLK_L99DZ200G::L99DZ200G_Get_ECV_DriveVoltage(void)
{
    float vctrl_max;
    float drive_volts;
    uint32_t reg_data;

    // read CFR
    reg_data = L99DZ200G_ReadRegister(L99DZ200G_CFR);
    if (reg_data & CFR_ECV_HV_MASK)
    {
        vctrl_max = ECV_1_5_VOLTS;
    }
    else
    {
        vctrl_max = ECV_1_2_VOLTS;
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_CR11);
    reg_data = (reg_data & CR11_EC_VALUE_MASK) >> CR11_EC_VALUE_POS;
    drive_volts = (vctrl_max * reg_data) / CR11_EC_VALUE_MASK;

    return drive_volts;
}

// Set Overcurrent Autorecovery programmable ON time - CR8
void DLK_L99DZ200G::L99DZ200G_Set_OCR_AutorecoveryTime(uint8_t output, uint8_t rec_time)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_7:
            reg_data = (uint32_t)rec_time << CR8_OUT7_TIME_POS;
            reg_mask = CR8_OUT7_TIME_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)rec_time << CR8_OUT8_TIME_POS;
            reg_mask = CR8_OUT8_TIME_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)rec_time << CR8_OUT15_TIME_POS;
            reg_mask = CR8_OUT15_TIME_MASK;
            break;
        case OUT_1_2_3_6:
            reg_data = (uint32_t)rec_time << CR8_OUT1_2_3_6_TIME_POS;
            reg_mask = CR8_OUT1_2_3_6_TIME_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR8, reg_mask, reg_data);
}

// Set Overcurrent Autorecovery programmable frequency - CR8
void DLK_L99DZ200G::L99DZ200G_Set_OCR_AutorecoveryFrequency(uint8_t output, uint8_t rec_freq)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_7:
            reg_data = (uint32_t)rec_freq << CR8_OUT7_FREQ_POS;
            reg_mask = CR8_OUT7_FREQ_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)rec_freq << CR8_OUT8_FREQ_POS;
            reg_mask = CR8_OUT8_FREQ_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)rec_freq << CR8_OUT15_FREQ_POS;
            reg_mask = CR8_OUT15_FREQ_MASK;
            break;
        case OUT_1_2_3_6:
            reg_data = (uint32_t)rec_freq << CR8_OUT1_2_3_6_FREQ_POS;
            reg_mask = CR8_OUT1_2_3_6_FREQ_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR8, reg_mask, reg_data);
}

// Switch ON/OFF Overcurrent Autorecovery - CR7
void DLK_L99DZ200G::L99DZ200G_Set_OCR_AutorecoveryControl(uint8_t output, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_1:
            reg_data = (uint32_t)en_dis << CR7_OUT1_OCR_POS;
            reg_mask = CR7_OUT1_OCR_MASK;
            break;
        case OUT_2:
            reg_data = (uint32_t)en_dis << CR7_OUT2_OCR_POS;
            reg_mask = CR7_OUT2_OCR_MASK;
            break;
        case OUT_3:
            reg_data = (uint32_t)en_dis << CR7_OUT3_OCR_POS;
            reg_mask = CR7_OUT3_OCR_MASK;
            break;
        case OUT_6:
            reg_data = (uint32_t)en_dis << CR7_OUT6_OCR_POS;
            reg_mask = CR7_OUT6_OCR_MASK;
            break;
        case OUT_7:
            reg_data = (uint32_t)en_dis << CR7_OUT7_OCR_POS;
            reg_mask = CR7_OUT7_OCR_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)en_dis << CR7_OUT8_OCR_POS;
            reg_mask = CR7_OUT8_OCR_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)en_dis << CR7_OUT15_OCR_POS;
            reg_mask = CR7_OUT15_OCR_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR7, reg_mask, reg_data);
}

// Switch ON/OFF OCR Thermal Expiration - CR8
void DLK_L99DZ200G::L99DZ200G_Set_OCR_ThermalExpirationControl(uint8_t output, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_1:
            reg_data = (uint32_t)en_dis << CR8_OUT1_THX_EN_POS;
            reg_mask = CR8_OUT1_THX_EN_MASK;
            break;
        case OUT_2:
            reg_data = (uint32_t)en_dis << CR8_OUT2_THX_EN_POS;
            reg_mask = CR8_OUT2_THX_EN_MASK;
            break;
        case OUT_3:
            reg_data = (uint32_t)en_dis << CR8_OUT3_THX_EN_POS;
            reg_mask = CR8_OUT3_THX_EN_MASK;
            break;
        case OUT_6:
            reg_data = (uint32_t)en_dis << CR8_OUT6_THX_EN_POS;
            reg_mask = CR8_OUT6_THX_EN_MASK;
            break;
        case OUT_7:
            reg_data = (uint32_t)en_dis << CR8_OUT7_THX_EN_POS;
            reg_mask = CR8_OUT7_THX_EN_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)en_dis << CR8_OUT8_THX_EN_POS;
            reg_mask = CR8_OUT8_THX_EN_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)en_dis << CR8_OUT15_THX_EN_POS;
            reg_mask = CR8_OUT15_THX_EN_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR8, reg_mask, reg_data);
}

// Switch ON/OFF Constant Current Mode - CR9
void DLK_L99DZ200G::L99DZ200G_SetConstantCurrentModeControl(uint8_t output, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    if (en_dis)
    {
        // Note: To enable Constant Current Mode, the OUTn *must* be turned off and
        //       its OCR autorecovery turned off! (see 4.21) {this prevents SPI_INV_CMD error}
        L99DZ200G_HSOutputsControl(OFF_OUT, output);                // turn output off
        L99DZ200G_Set_OCR_AutorecoveryControl(output, DISABLE);     // disable output autorecovery
    }

    switch (output)
    {
        case OUT_7:
            reg_data = (uint32_t)en_dis << CR9_OUT7_CCM_EN_POS;
            reg_mask = CR9_OUT7_CCM_EN_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)en_dis << CR9_OUT8_CCM_EN_POS;
            reg_mask = CR9_OUT8_CCM_EN_MASK;
            break;
        case OUT_9:
            reg_data = (uint32_t)en_dis << CR9_OUT9_CCM_EN_POS;
            reg_mask = CR9_OUT9_CCM_EN_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR9, reg_mask, reg_data);
}

// Set OUTn low/high Rdson setting - CR9
void DLK_L99DZ200G::L99DZ200G_Set_OUTn_Rdson(uint8_t output, uint8_t rds_on)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_7:
            reg_data = (uint32_t)rds_on << CR9_OUT7_RSDON_POS;
            reg_mask = CR9_OUT7_RSDON_MASK;
            break;
        case OUT_8:
            reg_data = (uint32_t)rds_on << CR9_OUT8_RSDON_POS;
            reg_mask = CR9_OUT8_RSDON_MASK;
            break;
        case OUT_1_6:
            reg_data = (uint32_t)rds_on << CR9_OUT1_6_RSDON_POS;
            reg_mask = CR9_OUT1_6_RSDON_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR9, reg_mask, reg_data);
}

// Switch ON/OFF short circuit threshold control - CR7
void DLK_L99DZ200G::L99DZ200G_SetShortCircuitControl(uint8_t output, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_1:
            reg_data = (uint32_t)en_dis << CR7_OUT1_SHORT_POS;
            reg_mask = CR7_OUT1_SHORT_MASK;
            break;
        case OUT_2:
            reg_data = (uint32_t)en_dis << CR7_OUT2_SHORT_POS;
            reg_mask = CR7_OUT2_SHORT_MASK;
            break;
        case OUT_3:
            reg_data = (uint32_t)en_dis << CR7_OUT3_SHORT_POS;
            reg_mask = CR7_OUT3_SHORT_MASK;
            break;
        case OUT_6:
            reg_data = (uint32_t)en_dis << CR7_OUT6_SHORT_POS;
            reg_mask = CR7_OUT6_SHORT_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR7, reg_mask, reg_data);
}

// Set L99DZ200G low/high current Open-load Threshold Control - CR9
void DLK_L99DZ200G::L99DZ200G_OpenLoadThresholdControl(uint8_t output, uint8_t hi_lo)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_9:
            reg_data = (uint32_t)hi_lo << CR9_OUT9_OL_POS;
            reg_mask = CR9_OUT9_OL_MASK;
            break;
        case OUT_10:
            reg_data = (uint32_t)hi_lo << CR9_OUT10_OL_POS;
            reg_mask = CR9_OUT10_OL_MASK;
            break;
        case OUT_13:
            reg_data = (uint32_t)hi_lo << CR9_OUT13_OL_POS;
            reg_mask = CR9_OUT13_OL_MASK;
            break;
        case OUT_14:
            reg_data = (uint32_t)hi_lo << CR9_OUT14_OL_POS;
            reg_mask = CR9_OUT14_OL_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)hi_lo << CR9_OUT15_OL_POS;
            reg_mask = CR9_OUT15_OL_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR9, reg_mask, reg_data);
}

// Set L99DZ200G low/high current Overcurrent Threshold Control - CR9
void DLK_L99DZ200G::L99DZ200G_OvercurrentThresholdControl(uint8_t output, uint8_t hi_lo)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_9:
            reg_data = (uint32_t)hi_lo << CR9_OUT9_OC_POS;
            reg_mask = CR9_OUT9_OC_MASK;
            break;
        case OUT_10:
            reg_data = (uint32_t)hi_lo << CR9_OUT10_OC_POS;
            reg_mask = CR9_OUT10_OC_MASK;
            break;
        case OUT_13:
            reg_data = (uint32_t)hi_lo << CR9_OUT13_OC_POS;
            reg_mask = CR9_OUT13_OC_MASK;
            break;
        case OUT_14:
            reg_data = (uint32_t)hi_lo << CR9_OUT14_OC_POS;
            reg_mask = CR9_OUT14_OC_MASK;
            break;
        case OUT_15:
            reg_data = (uint32_t)hi_lo << CR9_OUT15_OC_POS;
            reg_mask = CR9_OUT15_OC_MASK;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR9, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G Heater Open-load Diagnosis Control - CR11
void DLK_L99DZ200G::L99DZ200G_HeaterOpenLoadDiagnosisControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR11_GHOL_EN_POS;
    reg_mask = CR11_GHOL_EN_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Set L99DZ200G Drain source monitoring threshold voltage for external heater MOSFET - CR11
void DLK_L99DZ200G::L99DZ200G_SetHeaterMonitorThresholdVoltage(uint8_t mon_threshld)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)mon_threshld << CR11_GH_TH_POS;
    reg_mask = CR11_GH_TH_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR11, reg_mask, reg_data);
}

// Set L99DZ200G Vs Compensation VLED voltage - CR17 to CR20
void DLK_L99DZ200G::L99DZ200G_SetVsCompensationVLED(uint8_t output, float vled)
{
    uint8_t reg;
    uint32_t reg_data;
    uint32_t reg_mask;

    if (vled > VLED_MAX)
    {
        vled = VLED_MAX;
    }
    else if (vled < 0)
    {
        vled = 0.0;
    }

    reg_data = (uint32_t)((vled * 1024) / VAINVS);

    switch (output)
    {
        case OUT_7:
            reg = L99DZ200G_CR17;
            reg_mask = ODD_VLED_MASK;
            reg_data <<= ODD_VLED_POS;
            break;
        case OUT_8:
            reg = L99DZ200G_CR17;
            reg_mask = EVEN_VLED_MASK;
            reg_data <<= EVEN_VLED_POS;
            break;
        case OUT_9:
            reg = L99DZ200G_CR18;
            reg_mask = ODD_VLED_MASK;
            reg_data <<= ODD_VLED_POS;
            break;
        case OUT_10:
            reg = L99DZ200G_CR18;
            reg_mask = EVEN_VLED_MASK;
            reg_data <<= EVEN_VLED_POS;
            break;
        case OUT_13:
            reg = L99DZ200G_CR19;
            reg_mask = ODD_VLED_MASK;
            reg_data <<= ODD_VLED_POS;
            break;
        case OUT_14:
            reg = L99DZ200G_CR19;
            reg_mask = EVEN_VLED_MASK;
            reg_data <<= EVEN_VLED_POS;
            break;
        case OUT_15:
            reg = L99DZ200G_CR20;
            reg_mask = ODD_VLED_MASK;
            reg_data <<= ODD_VLED_POS;
            break;

        default:
            return;     // invalid output
    }
    L99DZ200G_ModifyControlRegister(reg, reg_mask, reg_data);
}

// Switch ON/OFF Automatic Vs Compensation control - CR17 to CR20
void DLK_L99DZ200G::L99DZ200G_SetAutoVsCompensationControl(uint8_t output, uint8_t en_dis)
{
    uint8_t reg;
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (output)
    {
        case OUT_7:
            reg = L99DZ200G_CR17;
            reg_mask = ODD_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << ODD_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_8:
            reg = L99DZ200G_CR17;
            reg_mask = EVEN_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << EVEN_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_9:
            reg = L99DZ200G_CR18;
            reg_mask = ODD_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << ODD_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_10:
            reg = L99DZ200G_CR18;
            reg_mask = EVEN_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << EVEN_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_13:
            reg = L99DZ200G_CR19;
            reg_mask = ODD_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << ODD_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_14:
            reg = L99DZ200G_CR19;
            reg_mask = EVEN_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << EVEN_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_15:
            reg = L99DZ200G_CR20;
            reg_mask = ODD_OUT_AUTOCOMP_EN;
            reg_data = (uint32_t)en_dis << ODD_OUT_AUTOCOMP_EN_POS;
            break;

        default:
            return;     // invalid output
    }

    L99DZ200G_ModifyControlRegister(reg, reg_mask, reg_data);
}

// Get the temperature of specified Thermal Cluster - SR7 to SR9
float DLK_L99DZ200G::L99DZ200G_GetThermalClusterTemp(uint8_t therm_cluster)
{
    uint8_t reg;
    uint32_t sum = 0;
    uint32_t reg_data;
    uint32_t reg_mask;
    uint32_t reg_pos;
    float temp;

    switch (therm_cluster)
    {
        case TEMP_CL1:
            reg = L99DZ200G_SR7;
            reg_mask = ODD_TEMP_CL_MASK;
            reg_pos = ODD_TEMP_CL_POS;
            break;
        case TEMP_CL2:
            reg = L99DZ200G_SR7;
            reg_mask = EVEN_TEMP_CL_MASK;
            reg_pos = EVEN_TEMP_CL_POS;
            break;
        case TEMP_CL3:
            reg = L99DZ200G_SR8;
            reg_mask = ODD_TEMP_CL_MASK;
            reg_pos = ODD_TEMP_CL_POS;
            break;
        case TEMP_CL4:
            reg = L99DZ200G_SR8;
            reg_mask = EVEN_TEMP_CL_MASK;
            reg_pos = EVEN_TEMP_CL_POS;
            break;
        case TEMP_CL5:
            reg = L99DZ200G_SR9;
            reg_mask = ODD_TEMP_CL_MASK;
            reg_pos = ODD_TEMP_CL_POS;
            break;
        case TEMP_CL6:
            reg = L99DZ200G_SR9;
            reg_mask = EVEN_TEMP_CL_MASK;
            reg_pos = EVEN_TEMP_CL_POS;
            break;

        default:
            return -999.0;    // invalid thermal cluster
    }

    // read SRx and convert to temperature
    for (uint8_t i = 0; i < 5; ++i)
    {
        reg_data = L99DZ200G_ReadRegister(reg);
        sum += (reg_data & reg_mask) >> reg_pos;
        L99DZ200G_CheckWdogExpired();
    }
    sum /= 5;
    temp = 350.0 - (0.488 * sum);              // see datasheet: Section 4.37

    return temp;
}

// Get the specified pin voltage value - SR10 to SR11
float DLK_L99DZ200G::L99DZ200G_GetPinVoltage(uint8_t volt_item)
{
    uint8_t reg;
    uint32_t sum = 0;
    uint32_t reg_data;
    uint32_t reg_mask;
    uint32_t reg_pos;
    float volts;

    switch (volt_item)
    {
        case VS_REG_V:
            reg = L99DZ200G_SR10;
            reg_mask = VS_REG_MASK;
            reg_pos = VS_REG_POS;
            break;
        case VS_V:
            reg = L99DZ200G_SR11;
            reg_mask = VS_MASK;
            reg_pos = VS_POS;
            break;
        case VWU_V:
            reg = L99DZ200G_SR11;
            reg_mask = VWU_MASK;
            reg_pos = VWU_POS;
            break;

        default:
            return -999.0;    // invalid thermal cluster
    }

    // read SRx and convert to averaged voltage
    for (uint8_t i = 0; i < 5; ++i)
    {
        reg_data = L99DZ200G_ReadRegister(reg);
        sum += (reg_data & reg_mask) >> reg_pos;
        L99DZ200G_CheckWdogExpired();
    }
    sum /= 5;                       // average
    volts = VAINVS * sum / 1024;

    return volts;
}

// Set L99DZ200G device for specified standby mode - CR1
void DLK_L99DZ200G::L99DZ200G_SetModeControl(uint8_t lp_mode)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_STBY_MASK;
    switch (lp_mode)
    {
        case V1_STBY:
            reg_data = (uint32_t)GOTO_V1_STANDBY << CR1_STBY_POS;
            WatchdogRunning = false;
            break;
        case VBAT_STBY:
            reg_data = (uint32_t)GOTO_VBAT_STANDBY << CR1_STBY_POS;
            WatchdogRunning = false;
            break;
        case ACTIVE_STBY:
            reg_data = (uint32_t)GOTO_ACTIVE << CR1_STBY_POS;
            break;
        default:
            return;         // invalid mode
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF charge pump and charge pump low and charge pump clock dithering control - CR22 (CFR)
void DLK_L99DZ200G::L99DZ200G_SetChargePumpControl(uint8_t en_dis, uint8_t ltch_live, uint8_t don_doff)
{
    uint32_t tmp_dataCFR;
    uint32_t tmp_dataCR22;

    // read CFR
    tmp_dataCFR = L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // modify CFR data to enable CP modifications
    tmp_dataCFR &= FULL_REG_MASK;
    tmp_dataCFR &= ~(CFR_CP_LOW_CFG_MASK | CFR_CP_DITH_DIS_MASK);
    tmp_dataCFR |= CFR_CP_OFF_EN_MASK;                      // set CP_OFF_EN bit
    tmp_dataCFR |= (ltch_live << CFR_CP_LOW_CFG_POS);       // get CP_LOW_CONFIG value
    tmp_dataCFR |= (don_doff << CFR_CP_DITH_DIS_POS);       // get CP_DITH_DIS value

    // read CR22
    tmp_dataCR22 = L99DZ200G_ReadRegister(L99DZ200G_CR22);

    // modify CR22 data
    tmp_dataCR22 &= ~CR22_CP_OFF_MASK;
    tmp_dataCR22 &= FULL_REG_MASK;
    tmp_dataCR22 |= (en_dis << CR22_CP_OFF_POS);            // get CP_OFF value

    // write CFR to enable modifying charge pump control
    L99DZ200G_WriteControlRegister(L99DZ200G_CFR, tmp_dataCFR);

    // write CR22 to modify charge pump control
    L99DZ200G_WriteControlRegister(L99DZ200G_CR22, tmp_dataCR22);
}

// Switch ON/OFF V1 load current supervision (ICMP) control - CR22 (CFR)
void DLK_L99DZ200G::L99DZ200G_Set_ICMP_Control(uint8_t en_dis)
{
    uint32_t tmp_dataCFR;
    uint32_t tmp_dataCR22;

    // read CFR
    tmp_dataCFR = L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // modify CFR data to enable V1 load current supervision modifications
    tmp_dataCFR &= FULL_REG_MASK;
    tmp_dataCFR |= CFR_ICMP_CFG_EN_MASK;

    // read CR22
    tmp_dataCR22 = L99DZ200G_ReadRegister(L99DZ200G_CR22);

    // modify CR22 data
    tmp_dataCR22 &= ~CR22_ICMP_MASK;
    tmp_dataCR22 &= FULL_REG_MASK;
    tmp_dataCR22 |= (en_dis << CR22_ICMP_POS);              // get ICMP value

    // write CFR to enable modifying V1 load current supervision control
    L99DZ200G_WriteControlRegister(L99DZ200G_CFR, tmp_dataCFR);

    // write CR22 to modify V1 load current supervision control
    L99DZ200G_WriteControlRegister(L99DZ200G_CR22, tmp_dataCR22);
}

// Set voltage regulator V1 reset threshold (VRTH) - CR2
void DLK_L99DZ200G::L99DZ200G_Set_V1_ResetThreshold(uint8_t v1_thrshld)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)v1_thrshld << CR2_V1_RST_LEV_POS;
    reg_mask = CR2_V1_RST_LEV_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// Set L99DZ200G status masked in GSB - CFR
void DLK_L99DZ200G::L99DZ200G_Set_GSB_MaskUnmask(uint8_t mask, uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (mask)
    {
        case HS_OL_ITEM:
            reg_mask = CFR_MASK_OL_HS_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_OL_HS_POS;
            break;
        case LS_OL_ITEM:
            reg_mask = CFR_MASK_OL_LS_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_OL_LS_POS;
            break;
        case TW_ITEM:
            reg_mask = CFR_MASK_TW_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_TW_POS;
            break;
        case EC_OL_ITEM:
            reg_mask = CFR_MASK_EC_OL_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_EC_OL_POS;
            break;
        case OL_ITEM:
            reg_mask = CFR_MASK_OL_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_OL_POS;
            break;
        case SPIE_ITEM:
            reg_mask = CFR_MASK_SPIE_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_SPIE_POS;
            break;
        case PLE_ITEM:
            reg_mask = CFR_MASK_PLE_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_PLE_POS;
            break;
        case GW_ITEM:
            reg_mask = CFR_MASK_GW_MASK;
            reg_data = (uint32_t)on_off << CFR_MASK_GW_POS;
            break;

        default:
            return;     // invalid mask
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VS supply voltage overvoltage/undervoltage lockout control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VS_LockoutControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VS_LOCK_POS;
    reg_mask = CR3_VS_LOCK_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VS supply voltage overvoltage shutdown control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VS_OvervoltageShutdownControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VS_OVSHD_POS;
    reg_mask = CR3_VS_OVSHD_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VS supply voltage undervoltage shutdown control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VS_UndervoltageShutdownControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VS_UVSHD_POS;
    reg_mask = CR3_VS_UVSHD_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VSREG supply voltage overvoltage/undervoltage lockout control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VSREG_LockoutControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VSREG_LOCK_POS;
    reg_mask = CR3_VSREG_LOCK_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VSREG supply voltage overvoltage shutdown control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VSREG_OvervoltageShutdownControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VSREG_OVSHD_POS;
    reg_mask = CR3_VSREG_OVSHD_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set ON/OFF L99DZ200G VSREG supply voltage undervoltage shutdown control - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VSREG_UndervoltageShutdownControl(uint8_t on_off)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_data = (uint32_t)on_off << CR3_VSREG_UVSHD_POS;
    reg_mask = CR3_VSREG_UVSHD_MASK;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Set L99DZ200G VSREG early warning threshold voltage - CR3
void DLK_L99DZ200G::L99DZ200G_Set_VSREG_EarlyEarningThreshold(float thshld_volts)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    if (thshld_volts > EW_THR_MAX)
    {
        thshld_volts = EW_THR_MAX;
    }
    else if (thshld_volts < 0)
    {
        thshld_volts = 0.0;
    }

    reg_data = (uint32_t)((thshld_volts * 1024) / VAINVS);

    reg_mask = CR3_VSREG_EW_MASK;
    reg_data <<= CR3_VSREG_EW_POS;

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR3, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G Generator mode functionality - CR22
void DLK_L99DZ200G::L99DZ200G_SetGeneratorModeControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR22_GEN_MODE_EN_MASK;
    reg_data = (uint32_t)en_dis << CR22_GEN_MODE_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR22, reg_mask, reg_data);
}

// Stay or Exit L99DZ200G SW-Debug mode - CR22
void DLK_L99DZ200G::L99DZ200G_StayExitSW_DebugModeControl(uint8_t stay_exit)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR22_DEBUG_EXIT_MASK;
    reg_data = (uint32_t)stay_exit << CR22_DEBUG_EXIT_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR22, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G Watchdog functionality - CR22
void DLK_L99DZ200G::L99DZ200G_WdogEnableControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR22_WDOG_EN_MASK;
    reg_data = (uint32_t)en_dis << CR22_WDOG_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR22, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G Thermal Shutdown functionality - CFR
void DLK_L99DZ200G::L99DZ200G_ThermalShutdownControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_TSD_CFG_MASK;
    reg_data = (uint32_t)en_dis << CFR_TSD_CFG_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G wake-up pin control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_WU_PinControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_WU_EN_MASK;
    reg_data = (uint32_t)en_dis << CR1_WU_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Set L99DZ200G wake-up pin pull-up/down functionality - CR1
void DLK_L99DZ200G::L99DZ200G_Set_WU_PinPullConfig(uint8_t config)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_WU_PU_MASK;
    reg_data = (uint32_t)config << CR1_WU_PU_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Set L99DZ200G wake-up pin mode functionality - CFR
void DLK_L99DZ200G::L99DZ200G_Set_WU_PinModeConfig(uint8_t config)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_WU_CFG_MASK;
    reg_data = (uint32_t)config << CFR_WU_CFG_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Set L99DZ200G wake-up pin filter functionality - CR1
void DLK_L99DZ200G::L99DZ200G_Set_WU_PinFilterConfig(uint8_t filter)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_WU_FILTER_MASK;
    reg_data = (uint32_t)filter << CR1_WU_FILTER_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G Timer for NINT/Wake control - CR1
void DLK_L99DZ200G::L99DZ200G_SetTimer_NINT_EnableControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_TMR_NINT_EN_MASK;
    reg_data = (uint32_t)en_dis << CR1_TMR_NINT_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Set L99DZ200G Timer for NINT/Wake control - CR1
void DLK_L99DZ200G::L99DZ200G_SetTimer_NINT_SelectControl(uint8_t tmr_sel)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_TMR_NINT_SEL_MASK;
    reg_data = (uint32_t)tmr_sel << CR1_TMR_NINT_SEL_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G LIN wake-up control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_LIN_WakeupControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_LIN_WU_EN_MASK;
    reg_data = (uint32_t)en_dis << CR1_LIN_WU_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G LIN receive-only control - CR2
void DLK_L99DZ200G::L99DZ200G_Set_LIN_ReceiveOnlyControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR2_LIN_RX_ONLY_MASK;
    reg_data = (uint32_t)en_dis << CR2_LIN_RX_ONLY_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G LIN transmit timout control - CR2
void DLK_L99DZ200G::L99DZ200G_Set_LIN_TransmitTimoutControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR2_LIN_TXD_TO_EN_MASK;
    reg_data = (uint32_t)en_dis << CR2_LIN_TXD_TO_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// Set L99DZ200G LIN bit rate control - CFR
void DLK_L99DZ200G::L99DZ200G_Set_LIN_BitRateControl(uint8_t hi_lo)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_LIN_HS_EN_MASK;
    reg_data = (uint32_t)hi_lo << CFR_LIN_HS_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Set L99DZ200G LIN wake-up behavior configuration - CFR
void DLK_L99DZ200G::L99DZ200G_Set_LIN_WakeupBehaviorControl(uint8_t rdr_rdt)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_LIN_WU_CFG_MASK;
    reg_data = (uint32_t)rdr_rdt << CFR_LIN_WU_CFG_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN wake-up control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_CAN_WakeupControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_CAN_WU_EN_MASK;
    reg_data = (uint32_t)en_dis << CR1_CAN_WU_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN Rx control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_CAN_ReceiveControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_CAN_RXEN_MASK;
    reg_data = (uint32_t)en_dis << CR1_CAN_RXEN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN Tx control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_CAN_TransmitControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_CAN_TXEN_MASK;
    reg_data = (uint32_t)en_dis << CR1_CAN_TXEN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN timeout IRQ control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_CAN_TimeoutIrqControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_CAN_TOUT_IRQ_MASK;
    reg_data = (uint32_t)en_dis << CR1_CAN_TOUT_IRQ_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN go TRX READY mode control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_CAN_GoTxReadyControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR1_CAN_TRXRDY_MASK;
    reg_data = (uint32_t)en_dis << CR1_CAN_TRXRDY_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN Tx->Rx loopback control - CR2
void DLK_L99DZ200G::L99DZ200G_Set_CAN_LoopbackControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR2_CAN_LOOP_EN_MASK;
    reg_data = (uint32_t)en_dis << CR2_CAN_LOOP_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// Switch ON/OFF L99DZ200G CAN pretended networking control - CR2
void DLK_L99DZ200G::L99DZ200G_Set_CAN_PretendedNetworkingControl(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CR2_CAN_PNW_EN_MASK;
    reg_data = (uint32_t)en_dis << CR2_CAN_PNW_EN_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR2, reg_mask, reg_data);
}

// H-Bridge Stuff

 // Switch ON/OFF specified L99DZ200G H-Bridge control - CR1
void DLK_L99DZ200G::L99DZ200G_Set_HB_Control(uint8_t H_bridge, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR1_HENA_MASK;
            reg_data = (uint32_t)en_dis << CR1_HENA_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR1_HENB_MASK;
            reg_data = (uint32_t)en_dis << CR1_HENB_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR1, reg_mask, reg_data);
}

// Set single motor mode Direction of specified L99DZ200G H-bridge - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorDirectionControl(uint8_t H_bridge, uint8_t mdir)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_A_DIR_MASK;
            reg_data = (uint32_t)mdir << CR10_HB_A_DIR_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR21_HB_B_DIR_MASK;
            reg_data = (uint32_t)mdir << CR21_HB_B_DIR_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Set motor mode of specified L99DZ200G H-bridge - CFR
void DLK_L99DZ200G::L99DZ200G_Set_HB_MotorModeControl(uint8_t H_bridge, uint8_t mode)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CFR_DM_HB_A_MASK;
            reg_data = (uint32_t)mode << CFR_DM_HB_A_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CFR_DM_HB_B_MASK;
            reg_data = (uint32_t)mode << CFR_DM_HB_B_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Set cross current protection time for specified L99DZ200G H-bridge - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Set_HB_CrossCurrentProtectionTime(uint8_t H_bridge, uint8_t ccpt_time)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_CCPT_MASK;
            reg_data = (uint32_t)ccpt_time << CR10_HB_CCPT_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR21_HB_CCPT_MASK;
            reg_data = (uint32_t)ccpt_time << CR21_HB_CCPT_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Set slew rate current percentage for specified L99DZ200G H-bridge - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Set_HB_SlewRateCurrent(uint8_t H_bridge, uint8_t percent)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    if (percent >= 100)
    {
        percent = (SLEW_MAX - 1);
    }
    else if (percent > (100 / SLEW_MAX))    // > 3%
    {
        percent = (uint8_t)(((SLEW_MAX * 1.0) * percent) / 100) - 1;
    }
    else    // <= 3%
    {
        percent = 0;
    }

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_SLEW_MASK;
            reg_data = (uint32_t)percent << CR10_HB_SLEW_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR21_HB_SLEW_MASK;
            reg_data = (uint32_t)percent << CR21_HB_SLEW_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Set OL high threshold for specified L99DZ200G H-bridge - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Set_HB_OL_HighThreshold(uint8_t H_bridge, uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_OLTH_HI_MASK;
            reg_data = (uint32_t)en_dis << CR10_HB_OLTH_HI_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR21_HB_OLTH_HI_MASK;
            reg_data = (uint32_t)en_dis << CR21_HB_OLTH_HI_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Switch ON/OFF L99DZ200G forced LSx_FSO outputs control - CFR
void DLK_L99DZ200G::L99DZ200G_Set_HB_Forced_LSx_FSO_Control(uint8_t en_dis)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    reg_mask = CFR_FS_FORCED_MASK;
    reg_data = (uint32_t)en_dis << CFR_FS_FORCED_POS;
    L99DZ200G_ModifyControlRegister(L99DZ200G_CFR, reg_mask, reg_data);
}

// Test open-load condition between H1 and L2 or H2 and L1 of the H-bridge A or H-bridge B - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Test_HB_OL_HxandLy(uint8_t H_bridge, uint8_t en_dis, uint8_t ol_hxly)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            switch (ol_hxly)
            {
                case OL_H1L2:
                    reg_mask = CR10_HB_OLH1L2_MASK;
                    reg_data = (uint32_t)en_dis << CR10_HB_OLH1L2_POS;
                    break;
                case OL_H2L1:
                    reg_mask = CR10_HB_OLH2L1_MASK;
                    reg_data = (uint32_t)en_dis << CR10_HB_OLH2L1_POS;
                    break;
                default:
                    return;
            }
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;

        case H_BRIDGE_CONTROL_B:
            switch (ol_hxly)
            {
                case OL_H1L2:
                    reg_mask = CR21_HB_OLH1L2_MASK;
                    reg_data = (uint32_t)en_dis << CR21_HB_OLH1L2_POS;
                    break;
                case OL_H2L1:
                    reg_mask = CR21_HB_OLH2L1_MASK;
                    reg_data = (uint32_t)en_dis << CR21_HB_OLH2L1_POS;
                    break;
                default:
                    return;
            }
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Set Drain-Source threshold of specified L99DZ200G H-bridge - CR10, CR21
void DLK_L99DZ200G::L99DZ200G_Set_HB_DSMonitorThreshold(uint8_t H_bridge, uint8_t ds_threshold)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_DS_DIAG_MASK;
            reg_data = (uint32_t)ds_threshold << CR10_HB_DS_DIAG_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR21_HB_DS_DIAG_MASK;
            reg_data = (uint32_t)ds_threshold << CR21_HB_DS_DIAG_POS;
            L99DZ200G_ModifyControlRegister(L99DZ200G_CR21, reg_mask, reg_data);
            break;
    }
}

// Set single motor mode freewheeling type for specified L99DZ200G H-bridge - CR10
//  - SDS1 = 0 (Active), SDS1 = 1 (Passive)
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorFreewheelingType(uint8_t H_bridge, uint8_t act_pass)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_A_SDS1_MASK;                     // SDS1A
            reg_data = (uint32_t)act_pass << CR10_HB_A_SDS1_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR10_HB_B_SDS1_MASK;                     // SDS1B
            reg_data = (uint32_t)act_pass << CR10_HB_B_SDS1_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
}

// Set single motor mode freewheeling side for specified L99DZ200G H-bridge - CR10
//  - SD1 = 0 (Low-Side), SD1 = 1 (High-Side)
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorFreewheelingSide(uint8_t H_bridge, uint8_t low_high)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg_mask = CR10_HB_A_SD1_MASK;                      // SD1A
            reg_data = (uint32_t)low_high << CR10_HB_A_SD1_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            reg_mask = CR10_HB_B_SD1_MASK;                      // SD1B
            reg_data = (uint32_t)low_high << CR10_HB_B_SD1_POS;
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
}

// Set dual motor mode drive side for specified L99DZ200G H-bridge and leg - CR10
//  - SD = 0 (Low-Side), SD = 1 (High-Side)
void DLK_L99DZ200G::L99DZ200G_Set_HB_DualMotorDrive(uint8_t H_bridge, uint8_t leg, uint8_t low_high)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            switch (leg)
            {
                case HB_LEG1:
                    reg_mask = CR10_HB_A_SD1_MASK;                      // SD1A
                    reg_data = (uint32_t)low_high << CR10_HB_A_SD1_POS;
                    break;
                case HB_LEG2:
                    reg_mask = CR10_HB_A_SD2_MASK;                      // SD2A
                    reg_data = (uint32_t)low_high << CR10_HB_A_SD2_POS;
                    break;
                default:
                    return;
            }
            break;
        case H_BRIDGE_CONTROL_B:
            switch (leg)
            {
                case HB_LEG1:
                    reg_mask = CR10_HB_B_SD1_MASK;                      // SD1B
                    reg_data = (uint32_t)low_high << CR10_HB_B_SD1_POS;
                    break;
                case HB_LEG2:
                    reg_mask = CR10_HB_B_SD2_MASK;                      // SD2B
                    reg_data = (uint32_t)low_high << CR10_HB_B_SD2_POS;
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }
    L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
}

// Set dual motor mode freewheeling type for specified L99DZ200G H-bridge and leg - CR10
//  - SDS = 0 (Active), SDS = 1 (Passive)
void DLK_L99DZ200G::L99DZ200G_Set_HB_DualMotorFreewheelingType(uint8_t H_bridge, uint8_t leg, uint8_t act_pass)
{
    uint32_t reg_data;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            switch (leg)
            {
                case HB_LEG1:
                    reg_mask = CR10_HB_A_SDS1_MASK;                     // SDS1A
                    reg_data = (uint32_t)act_pass << CR10_HB_A_SDS1_POS;
                    break;
                case HB_LEG2:
                    reg_mask = CR10_HB_A_SDS2_MASK;                     // SDS2A
                    reg_data = (uint32_t)act_pass << CR10_HB_A_SDS2_POS;
                    break;
                default:
                    return;
            }
            break;
        case H_BRIDGE_CONTROL_B:
            switch (leg)
            {
                case HB_LEG1:
                    reg_mask = CR10_HB_B_SDS1_MASK;                     // SDS1B
                    reg_data = (uint32_t)act_pass << CR10_HB_B_SDS1_POS;
                    break;
                case HB_LEG2:
                    reg_mask = CR10_HB_B_SDS2_MASK;                     // SDS2B
                    reg_data = (uint32_t)act_pass << CR10_HB_B_SDS2_POS;
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }

    L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, reg_mask, reg_data);
}


// Turn the specified L99DZ200G H-bridge motor counterclockwise
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorCounterClockwise(uint8_t H_bridge)
{
    L99DZ200G_Set_HB_SingleMotorDirectionControl(H_bridge, LEFT_DIRECTION);
    L99DZ200G_Set_HB_Control(H_bridge, ENABLE);
}

// Turn the specified L99DZ200G H-bridge motor clockwise
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorClockwise(uint8_t H_bridge)
{
    L99DZ200G_Set_HB_SingleMotorDirectionControl(H_bridge, RIGHT_DIRECTION);
    L99DZ200G_Set_HB_Control(H_bridge, ENABLE);
}

// Stop the specified L99DZ200G H-bridge motor
void DLK_L99DZ200G::L99DZ200G_Set_HB_SingleMotorStop(uint8_t H_bridge)
{
    L99DZ200G_Set_HB_Control(H_bridge, DISABLE);
}


// Status Stuff

// Get the specified SPI item status - SR2
uint8_t DLK_L99DZ200G::L99DZ200G_Get_SPI_Status(uint8_t spi_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (spi_item)
    {
        case SPI_CLK_CNT:
            reg_mask = SR2_SPI_CLK_CNT;
            reg_pos = SR2_SPI_CLK_CNT_POS;
            break;
        case SPI_INV_CMD:
            reg_mask = SR2_SPI_INV_CMD;
            reg_pos = SR2_SPI_INV_CMD_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid SPI item
    }

    // read SR2
    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR2);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the SPI status - SR2
void DLK_L99DZ200G::L99DZ200G_Clear_SPI_Status(void)
{
    uint32_t reg_mask;

    reg_mask = SR2_SPI_INV_CMD;
    L99DZ200G_ReadClearRegister(L99DZ200G_SR2, reg_mask);
}

// Get the specified voltage item status - SR1, SR2
uint8_t DLK_L99DZ200G::L99DZ200G_GetVoltageStatus(uint8_t volt_item)
{
    uint8_t reg = L99DZ200G_SR2;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (volt_item)
    {
        case V1_UV:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_V1_UV;
            reg_pos = SR1_V1_UV_POS;
            break;
        case VS_UV:
            reg_mask = SR2_VS_UV;
            reg_pos = SR2_VS_UV_POS;
            break;
        case VS_OV:
            reg_mask = SR2_VS_OV;
            reg_pos = SR2_VS_OV_POS;
            break;
        case VSREG_UV:
            reg_mask = SR2_VSREG_UV;
            reg_pos = SR2_VSREG_UV_POS;
            break;
        case VSREG_OV:
            reg_mask = SR2_VSREG_OV;
            reg_pos = SR2_VSREG_OV_POS;
            break;
        case VSREG_EW:
            reg_mask = SR2_VSREG_EW;
            reg_pos = SR2_VSREG_EW_POS;
            break;
        case V1_FAIL:
            reg_mask = SR2_V1_FAIL;
            reg_pos = SR2_V1_FAIL_POS;
            break;
        case V2_FAIL:
            reg_mask = SR2_V2_FAIL;
            reg_pos = SR2_V2_FAIL_POS;
            break;
        case V2_SC:
            reg_mask = SR2_V2_SC;
            reg_pos = SR2_V2_SC_POS;
            break;
        case CP_LOW:
            reg_mask = SR2_CP_LOW;
            reg_pos = SR2_CP_LOW_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid voltage item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified voltage item status - SR1, SR2
void DLK_L99DZ200G::L99DZ200G_ClearVoltageStatus(uint8_t volt_item)
{
    uint8_t reg = L99DZ200G_SR2;
    uint32_t reg_mask;

    switch (volt_item)
    {
        case ALL_VOLT_ITEMS:
            reg_mask = SR2_VS_UV | SR2_VS_OV | SR2_VSREG_UV | SR2_VSREG_OV | SR2_VSREG_EW |
                       SR2_V1_FAIL | SR2_V2_FAIL | SR2_V2_SC | SR2_CP_LOW;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case V1_UV:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_V1_UV;
            break;
        case VS_UV:
            reg_mask = SR2_VS_UV;
            break;
        case VS_OV:
            reg_mask = SR2_VS_OV;
            break;
        case VSREG_UV:
            reg_mask = SR2_VSREG_UV;
            break;
        case VSREG_OV:
            reg_mask = SR2_VSREG_OV;
            break;
        case VSREG_EW:
            reg_mask = SR2_VSREG_EW;
            break;
        case V1_FAIL:
            reg_mask = SR2_V1_FAIL;
            break;
        case V2_FAIL:
            reg_mask = SR2_V2_FAIL;
            break;
        case V2_SC:
            reg_mask = SR2_V2_SC;
            break;
        case CP_LOW:
            reg_mask = SR2_CP_LOW;
            break;

        default:
            return;         // invalid voltage item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the specified Electrochromic voltage item status - SR6
uint8_t DLK_L99DZ200G::L99DZ200G_GetElectrochromicVoltageStatus(uint8_t ecv_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (ecv_item)
    {
        case ECV_VNR:
            reg_mask = SR6_ECV_VNR_MASK;
            reg_pos = SR6_ECV_VNR_POS;
            break;
        case ECV_VHI:
            reg_mask = SR6_ECV_VHI_MASK;
            reg_pos = SR6_ECV_VHI_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid voltage item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR6);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Get the specified short circuit alert item status - SR4, SR5
uint8_t DLK_L99DZ200G::L99DZ200G_GetShortCircuitAlertStatus(uint8_t sca_item)
{
    uint8_t reg = L99DZ200G_SR4;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (sca_item)
    {
        case OUT_1_HS:
            reg_mask = SR4_OUT1H_SHORT;
            reg_pos = SR4_OUT1H_SHORT_POS;
            break;
        case OUT_1_LS:
            reg_mask = SR4_OUT1L_SHORT;
            reg_pos = SR4_OUT1L_SHORT_POS;
            break;
        case OUT_2_HS:
            reg_mask = SR4_OUT2H_SHORT;
            reg_pos = SR4_OUT2H_SHORT_POS;
            break;
        case OUT_2_LS:
            reg_mask = SR4_OUT2L_SHORT;
            reg_pos = SR4_OUT2L_SHORT_POS;
            break;
        case OUT_3_HS:
            reg_mask = SR4_OUT3H_SHORT;
            reg_pos = SR4_OUT3H_SHORT_POS;
            break;
        case OUT_3_LS:
            reg_mask = SR4_OUT3L_SHORT;
            reg_pos = SR4_OUT3L_SHORT_POS;
            break;
        case OUT_6_HS:
            reg_mask = SR4_OUT6H_SHORT;
            reg_pos = SR4_OUT6H_SHORT_POS;
            break;
        case OUT_6_LS:
            reg_mask = SR4_OUT6L_SHORT;
            reg_pos = SR4_OUT6L_SHORT_POS;
            break;
        case DS_MON_HEAT:
            reg = L99DZ200G_SR5;
            reg_mask = SR5_DS_MON_HEAT;
            reg_pos = SR5_DS_MON_HEAT_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid short circuit alert item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified short circuit alert item status - SR4, SR5
void DLK_L99DZ200G::L99DZ200G_ClearShortCircuitAlertStatus(uint8_t sca_item)
{
    uint8_t reg = L99DZ200G_SR4;
    uint32_t reg_mask;

    switch (sca_item)
    {
        case OUT_ALL_SC_ITEMS:
            reg_mask = SR4_OUT1H_SHORT | SR4_OUT1L_SHORT | SR4_OUT2H_SHORT | SR4_OUT2L_SHORT |
                       SR4_OUT3H_SHORT | SR4_OUT3L_SHORT | SR4_OUT6H_SHORT | SR4_OUT6L_SHORT;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case DS_MON_HEAT:
            reg = L99DZ200G_SR5;
            reg_mask = SR5_DS_MON_HEAT;
            break;
        case OUT_1_HS:
            reg_mask = SR4_OUT1H_SHORT;
            break;
        case OUT_1_LS:
            reg_mask = SR4_OUT1L_SHORT;
            break;
        case OUT_2_HS:
            reg_mask = SR4_OUT2H_SHORT;
            break;
        case OUT_2_LS:
            reg_mask = SR4_OUT2L_SHORT;
            break;
        case OUT_3_HS:
            reg_mask = SR4_OUT3H_SHORT;
            break;
        case OUT_3_LS:
            reg_mask = SR4_OUT3L_SHORT;
            break;
        case OUT_6_HS:
            reg_mask = SR4_OUT6H_SHORT;
            break;
        case OUT_6_LS:
            reg_mask = SR4_OUT6L_SHORT;
            break;

        default:
            return;         // invalid short circuit alert item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the specified open-load item status - SR5
uint8_t DLK_L99DZ200G::L99DZ200G_GetOpenLoadStatus(uint8_t opld_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (opld_item)
    {
        case OUT_1_HS:
            reg_mask = SR5_OUT1H_OL;
            reg_pos = SR5_OUT1H_OL_POS;
            break;
        case OUT_1_LS:
            reg_mask = SR5_OUT1L_OL;
            reg_pos = SR5_OUT1L_OL_POS;
            break;
        case OUT_2_HS:
            reg_mask = SR5_OUT2H_OL;
            reg_pos = SR5_OUT2H_OL_POS;
            break;
        case OUT_2_LS:
            reg_mask = SR5_OUT2L_OL;
            reg_pos = SR5_OUT2L_OL_POS;
            break;
        case OUT_3_HS:
            reg_mask = SR5_OUT3H_OL;
            reg_pos = SR5_OUT3H_OL_POS;
            break;
        case OUT_3_LS:
            reg_mask = SR5_OUT3L_OL;
            reg_pos = SR5_OUT3L_OL_POS;
            break;
        case OUT_6_HS:
            reg_mask = SR5_OUT6H_OL;
            reg_pos = SR5_OUT6H_OL_POS;
            break;
        case OUT_6_LS:
            reg_mask = SR5_OUT6L_OL;
            reg_pos = SR5_OUT6L_OL_POS;
            break;
        case OUT_7_OL:
            reg_mask = SR5_OUT7_OL;
            reg_pos = SR5_OUT7_OL_POS;
            break;
        case OUT_8_OL:
            reg_mask = SR5_OUT8_OL;
            reg_pos = SR5_OUT8_OL_POS;
            break;
        case OUT_9_OL:
            reg_mask = SR5_OUT9_OL;
            reg_pos = SR5_OUT9_OL_POS;
            break;
        case OUT_10_OL:
            reg_mask = SR5_OUT10_OL;
            reg_pos = SR5_OUT10_OL_POS;
            break;
        case OUT_13_OL:
            reg_mask = SR5_OUT13_OL;
            reg_pos = SR5_OUT13_OL_POS;
            break;
        case OUT_14_OL:
            reg_mask = SR5_OUT14_OL;
            reg_pos = SR5_OUT14_OL_POS;
            break;
        case OUT_15_OL:
            reg_mask = SR5_OUT15_OL;
            reg_pos = SR5_OUT15_OL_POS;
            break;
        case OUT_GH:
            reg_mask = SR5_OUTGH_OL;
            reg_pos = SR5_OUTGH_OL_POS;
            break;
        case OUT_ECV:
            reg_mask = SR5_OUTECV_OL;
            reg_pos = SR5_OUTECV_OL_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid short circuit alert item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR5);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified open-load item status - SR5
void DLK_L99DZ200G::L99DZ200G_ClearOpenLoadStatus(uint8_t opld_item)
{
    uint32_t reg_mask;

    switch (opld_item)
    {
        case OUT_ALL_OL_ITEMS:
            reg_mask = SR5_OUT1H_OL | SR5_OUT1L_OL | SR5_OUT2H_OL | SR5_OUT2L_OL | SR5_OUT3H_OL |
                       SR5_OUT3L_OL | SR5_OUT6H_OL | SR5_OUT6L_OL | SR5_OUT7_OL | SR5_OUT8_OL |
                       SR5_OUT9_OL | SR5_OUT10_OL | SR5_OUT13_OL | SR5_OUT14_OL | SR5_OUT15_OL |
                       SR5_OUTGH_OL | SR5_OUTECV_OL;
            break;
        case OUT_1_HS:
            reg_mask = SR5_OUT1H_OL;
            break;
        case OUT_1_LS:
            reg_mask = SR5_OUT1L_OL;
            break;
        case OUT_2_HS:
            reg_mask = SR5_OUT2H_OL;
            break;
        case OUT_2_LS:
            reg_mask = SR5_OUT2L_OL;
            break;
        case OUT_3_HS:
            reg_mask = SR5_OUT3H_OL;
            break;
        case OUT_3_LS:
            reg_mask = SR5_OUT3L_OL;
            break;
        case OUT_6_HS:
            reg_mask = SR5_OUT6H_OL;
            break;
        case OUT_6_LS:
            reg_mask = SR5_OUT6L_OL;
            break;
        case OUT_7_OL:
            reg_mask = SR5_OUT7_OL;
            break;
        case OUT_8_OL:
            reg_mask = SR5_OUT8_OL;
            break;
        case OUT_9_OL:
            reg_mask = SR5_OUT9_OL;
            break;
        case OUT_10_OL:
            reg_mask = SR5_OUT10_OL;
            break;
        case OUT_13_OL:
            reg_mask = SR5_OUT13_OL;
            break;
        case OUT_14_OL:
            reg_mask = SR5_OUT14_OL;
            break;
        case OUT_15_OL:
            reg_mask = SR5_OUT15_OL;
            break;
        case OUT_GH:
            reg_mask = SR5_OUTGH_OL;
            break;
        case OUT_ECV:
            reg_mask = SR5_OUTECV_OL;
            break;

        default:
            return;         // invalid short circuit alert item
    }

    L99DZ200G_ReadClearRegister(L99DZ200G_SR5, reg_mask);
}

// Get the specified thermal shutdown item status - SR1, SR6
uint8_t DLK_L99DZ200G::L99DZ200G_GetThermalShutdownStatus(uint8_t tshdwn_item)
{
    uint8_t reg = L99DZ200G_SR6;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (tshdwn_item)
    {
        case TSD1_ITEM:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_TSD1;
            reg_pos = SR1_TSD1_POS;
            break;
        case TSD2_ITEM:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_TSD2;
            reg_pos = SR1_TSD2_POS;
            break;
        case TSD1_CL1_ITEM:
            reg_mask = SR6_TSD_CL1_MASK;
            reg_pos = SR6_TSD1_CL1_POS;
            break;
        case TSD1_CL2_ITEM:
            reg_mask = SR6_TSD_CL2_MASK;
            reg_pos = SR6_TSD1_CL2_POS;
            break;
        case TSD1_CL3_ITEM:
            reg_mask = SR6_TSD_CL3_MASK;
            reg_pos = SR6_TSD1_CL3_POS;
            break;
        case TSD1_CL4_ITEM:
            reg_mask = SR6_TSD_CL4_MASK;
            reg_pos = SR6_TSD1_CL4_POS;
            break;
        case TSD1_CL5_ITEM:
            reg_mask = SR6_TSD_CL5_MASK;
            reg_pos = SR6_TSD1_CL5_POS;
            break;
        case TSD1_CL6_ITEM:
            reg_mask = SR6_TSD_CL6_MASK;
            reg_pos = SR6_TSD1_CL6_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid thermal shutdown item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified thermal shutdown item status - SR1, SR6
void DLK_L99DZ200G::L99DZ200G_ClearThermalShutdownStatus(uint8_t tshdwn_item)
{
    uint8_t reg = L99DZ200G_SR6;
    uint32_t reg_mask;

    switch (tshdwn_item)
    {
        case TH_ALL_ITEMS:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_TSD2;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case TSD1_ITEM:
            reg = L99DZ200G_SR6;
            reg_mask = SR6_TSD_CL6_MASK | SR6_TSD_CL5_MASK | SR6_TSD_CL4_MASK |
                       SR6_TSD_CL3_MASK | SR6_TSD_CL2_MASK | SR6_TSD_CL1_MASK;
            break;
        case TSD2_ITEM:
            reg = L99DZ200G_SR1;
            reg_mask = SR1_TSD2;
            break;
        case TSD1_CL1_ITEM:
            reg_mask = SR6_TSD_CL1_MASK;
            break;
        case TSD1_CL2_ITEM:
            reg_mask = SR6_TSD_CL2_MASK;
            break;
        case TSD1_CL3_ITEM:
            reg_mask = SR6_TSD_CL3_MASK;
            break;
        case TSD1_CL4_ITEM:
            reg_mask = SR6_TSD_CL4_MASK;
            break;
        case TSD1_CL5_ITEM:
            reg_mask = SR6_TSD_CL5_MASK;
            break;
        case TSD1_CL6_ITEM:
            reg_mask = SR6_TSD_CL6_MASK;
            break;

        default:
            return;         // invalid thermal shutdown item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the specified thermal warning item status - SR2, SR6
uint8_t DLK_L99DZ200G::L99DZ200G_GetThermalWarningStatus(uint8_t twarn_item)
{
    uint8_t reg = L99DZ200G_SR6;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (twarn_item)
    {
        case THERMW_ITEM:
            reg = L99DZ200G_SR2;
            reg_mask = SR2_TW;
            reg_pos = SR2_TW_POS;
            break;
        case TW_CL1_ITEM:
            reg_mask = SR6_TW_CL1_MASK;
            reg_pos = SR6_TW_CL1_POS;
            break;
        case TW_CL2_ITEM:
            reg_mask = SR6_TW_CL2_MASK;
            reg_pos = SR6_TW_CL2_POS;
            break;
        case TW_CL3_ITEM:
            reg_mask = SR6_TW_CL3_MASK;
            reg_pos = SR6_TW_CL3_POS;
            break;
        case TW_CL4_ITEM:
            reg_mask = SR6_TW_CL4_MASK;
            reg_pos = SR6_TW_CL4_POS;
            break;
        case TW_CL5_ITEM:
            reg_mask = SR6_TW_CL5_MASK;
            reg_pos = SR6_TW_CL5_POS;
            break;
        case TW_CL6_ITEM:
            reg_mask = SR6_TW_CL6_MASK;
            reg_pos = SR6_TW_CL6_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid thermal shutdown item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified thermal warning item status - SR2, SR6
void DLK_L99DZ200G::L99DZ200G_ClearThermalWarningStatus(uint8_t twarn_item)
{
    uint8_t reg = L99DZ200G_SR6;
    uint32_t reg_mask;

    switch (twarn_item)
    {
        case TH_ALL_ITEMS:
        case THERMW_ITEM:
            reg_mask = SR6_TW_CL6_MASK | SR6_TW_CL5_MASK | SR6_TW_CL4_MASK |
                       SR6_TW_CL3_MASK | SR6_TW_CL2_MASK | SR6_TW_CL1_MASK;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            reg = L99DZ200G_SR2;
            reg_mask = SR2_TW;
            break;
        case TW_CL1_ITEM:
            reg_mask = SR6_TW_CL1_MASK;
            break;
        case TW_CL2_ITEM:
            reg_mask = SR6_TW_CL2_MASK;
            break;
        case TW_CL3_ITEM:
            reg_mask = SR6_TW_CL3_MASK;
            break;
        case TW_CL4_ITEM:
            reg_mask = SR6_TW_CL4_MASK;
            break;
        case TW_CL5_ITEM:
            reg_mask = SR6_TW_CL5_MASK;
            break;
        case TW_CL6_ITEM:
            reg_mask = SR6_TW_CL6_MASK;
            break;

        default:
            return;         // invalid thermal shutdown item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the specified overcurrent recovery alert item status - SR4
uint8_t DLK_L99DZ200G::L99DZ200G_GetOvercurrentRecoveryAlertStatus(uint8_t ocra_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (ocra_item)
    {
        case OUT_1_HS:
            reg_mask = SR4_OUT1H_OCRAL;
            reg_pos = SR4_OUT1H_OCRAL_POS;
            break;
        case OUT_1_LS:
            reg_mask = SR4_OUT1L_OCRAL;
            reg_pos = SR4_OUT1L_OCRAL_POS;
            break;
        case OUT_2_HS:
            reg_mask = SR4_OUT2H_OCRAL;
            reg_pos = SR4_OUT2H_OCRAL_POS;
            break;
        case OUT_2_LS:
            reg_mask = SR4_OUT2L_OCRAL;
            reg_pos = SR4_OUT2L_OCRAL_POS;
            break;
        case OUT_3_HS:
            reg_mask = SR4_OUT3H_OCRAL;
            reg_pos = SR4_OUT3H_OCRAL_POS;
            break;
        case OUT_3_LS:
            reg_mask = SR4_OUT3L_OCRAL;
            reg_pos = SR4_OUT3L_OCRAL_POS;
            break;
        case OUT_6_HS:
            reg_mask = SR4_OUT6H_OCRAL;
            reg_pos = SR4_OUT6H_OCRAL_POS;
            break;
        case OUT_6_LS:
            reg_mask = SR4_OUT6L_OCRAL;
            reg_pos = SR4_OUT6L_OCRAL_POS;
            break;
        case OUT_7_OCR:
            reg_mask = SR4_OUT7_OCRAL;
            reg_pos = SR4_OUT7_OCRAL_POS;
            break;
        case OUT_8_OCR:
            reg_mask = SR4_OUT8_OCRAL;
            reg_pos = SR4_OUT8_OCRAL_POS;
            break;
        case OUT_15_OCR:
            reg_mask = SR4_OUT15_OCRAL;
            reg_pos = SR4_OUT15_OCRAL_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid short circuit alert item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR4);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Get the specified overcurrent shutdown item status - SR3, SR5
uint8_t DLK_L99DZ200G::L99DZ200G_GetOvercurrentShutdownStatus(uint8_t ocs_item)
{
    uint8_t reg = L99DZ200G_SR3;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (ocs_item)
    {
        case OUT_1_HS:
            reg_mask = SR3_OUT1H_OCTHX;
            reg_pos = SR3_OUT1H_OCTHX_POS;
            break;
        case OUT_1_LS:
            reg_mask = SR3_OUT1L_OCTHX;
            reg_pos = SR3_OUT1L_OCTHX_POS;
            break;
        case OUT_2_HS:
            reg_mask = SR3_OUT2H_OCTHX;
            reg_pos = SR3_OUT2H_OCTHX_POS;
            break;
        case OUT_2_LS:
            reg_mask = SR3_OUT2L_OCTHX;
            reg_pos = SR3_OUT2L_OCTHX_POS;
            break;
        case OUT_3_HS:
            reg_mask = SR3_OUT3H_OCTHX;
            reg_pos = SR3_OUT3H_OCTHX_POS;
            break;
        case OUT_3_LS:
            reg_mask = SR3_OUT3L_OCTHX;
            reg_pos = SR3_OUT3L_OCTHX_POS;
            break;
        case OUT_6_HS:
            reg_mask = SR3_OUT6H_OCTHX;
            reg_pos = SR3_OUT6H_OCTHX_POS;
            break;
        case OUT_6_LS:
            reg_mask = SR3_OUT6L_OCTHX;
            reg_pos = SR3_OUT6L_OCTHX_POS;
            break;
        case OUT_7_OC:
            reg_mask = SR3_OUT7_OCTHX;
            reg_pos = SR3_OUT7_OCTHX_POS;
            break;
        case OUT_8_OC:
            reg_mask = SR3_OUT8_OCTHX;
            reg_pos = SR3_OUT8_OCTHX_POS;
            break;
        case OUT_9_OC:
            reg_mask = SR3_OUT9_OCTHX;
            reg_pos = SR3_OUT9_OCTHX_POS;
            break;
        case OUT_10_OC:
            reg_mask = SR3_OUT10_OCTHX;
            reg_pos = SR3_OUT10_OCTHX_POS;
            break;
        case OUT_13_OC:
            reg_mask = SR3_OUT13_OCTHX;
            reg_pos = SR3_OUT13_OCTHX_POS;
            break;
        case OUT_14_OC:
            reg_mask = SR3_OUT14_OCTHX;
            reg_pos = SR3_OUT14_OCTHX_POS;
            break;
        case OUT_15_OC:
            reg_mask = SR3_OUT15_OCTHX;
            reg_pos = SR3_OUT15_OCTHX_POS;
            break;
        case LSA_FSO_OC:
            reg_mask = SR3_LSA_FSO_OC;
            reg_pos = SR3_LSA_FSO_OC_POS;
            break;
        case LSB_FSO_OC:
            reg_mask = SR3_LSB_FSO_OC;
            reg_pos = SR3_LSB_FSO_OC_POS;
            break;
        case OUT_ECV:
            reg = L99DZ200G_SR5;
            reg_mask = SR5_ECV_OC;
            reg_pos = SR5_ECV_OC_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid short circuit alert item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified overcurrent shutdown item status - SR3, SR5
void DLK_L99DZ200G::L99DZ200G_ClearOvercurrentShutdownStatus(uint8_t ocs_item)
{
    uint8_t reg = L99DZ200G_SR3;
    uint32_t reg_mask;

    switch (ocs_item)
    {
        case OUT_ALL_OCS_ITEMS:
            reg_mask = SR3_OUT1H_OCTHX | SR3_OUT1L_OCTHX | SR3_OUT2H_OCTHX | SR3_OUT2L_OCTHX |
                       SR3_OUT3H_OCTHX | SR3_OUT3L_OCTHX | SR3_OUT6H_OCTHX | SR3_OUT6L_OCTHX |
                       SR3_OUT7_OCTHX | SR3_OUT8_OCTHX | SR3_OUT9_OCTHX | SR3_OUT10_OCTHX |
                       SR3_OUT13_OCTHX | SR3_OUT14_OCTHX | SR3_OUT15_OCTHX |
                       SR3_LSA_FSO_OC | SR3_LSB_FSO_OC;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case OUT_ECV:
            reg = L99DZ200G_SR5;
            reg_mask = SR5_ECV_OC;
            break;
        case OUT_1_HS:
            reg_mask = SR3_OUT1H_OCTHX;
            break;
        case OUT_1_LS:
            reg_mask = SR3_OUT1L_OCTHX;
            break;
        case OUT_2_HS:
            reg_mask = SR3_OUT2H_OCTHX;
            break;
        case OUT_2_LS:
            reg_mask = SR3_OUT2L_OCTHX;
            break;
        case OUT_3_HS:
            reg_mask = SR3_OUT3H_OCTHX;
            break;
        case OUT_3_LS:
            reg_mask = SR3_OUT3L_OCTHX;
            break;
        case OUT_6_HS:
            reg_mask = SR3_OUT6H_OCTHX;
            break;
        case OUT_6_LS:
            reg_mask = SR3_OUT6L_OCTHX;
            break;
        case OUT_7_OC:
            reg_mask = SR3_OUT7_OCTHX;
            break;
        case OUT_8_OC:
            reg_mask = SR3_OUT8_OCTHX;
            break;
        case OUT_9_OC:
            reg_mask = SR3_OUT9_OCTHX;
            break;
        case OUT_10_OC:
            reg_mask = SR3_OUT10_OCTHX;
            break;
        case OUT_13_OC:
            reg_mask = SR3_OUT13_OCTHX;
            break;
        case OUT_14_OC:
            reg_mask = SR3_OUT14_OCTHX;
            break;
        case OUT_15_OC:
            reg_mask = SR3_OUT15_OCTHX;
            break;
        case LSA_FSO_OC:
            reg_mask = SR3_LSA_FSO_OC;
            break;
        case LSB_FSO_OC:
            reg_mask = SR3_LSB_FSO_OC;
            break;

        default:
            return;         // invalid short circuit alert item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the count of voltage regulator V1 restarts due to thermal shutdown - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetV1ResetCount(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & SR1_V1_RESTRT_CNT_MASK) >> SR1_V1_RESTRT_CNT_POS;

    return reg_data;
}

// Get the specified H-bridge MOSFET device short circuit/open-load status - SR2, SR3
uint8_t DLK_L99DZ200G::L99DZ200G_Get_HB_DrainSourceMonitoringStatus(uint8_t H_bridge, uint8_t side_item)
{
    uint8_t reg;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = L99DZ200G_SR2;
            switch (side_item)
            {
                case LOW_SIDE_1_MOSFET:
                    reg_mask = SR2_DS_MON_LS1_A;
                    reg_pos = SR2_DS_MON_LS1_A_POS;
                    break;
                case HIGH_SIDE_1_MOSFET:
                    reg_mask = SR2_DS_MON_HS1_A;
                    reg_pos = SR2_DS_MON_HS1_A_POS;
                    break;
                case LOW_SIDE_2_MOSFET:
                    reg_mask = SR2_DS_MON_LS2_A;
                    reg_pos = SR2_DS_MON_LS2_A_POS;
                    break;
                case HIGH_SIDE_2_MOSFET:
                    reg_mask = SR2_DS_MON_HS2_A;
                    reg_pos = SR2_DS_MON_HS2_A_POS;
                    break;
                default:
                    return L99DZ200G_FAIL;    // invalid MOSFET device item
            }
            break;
        case H_BRIDGE_CONTROL_B:
            reg = L99DZ200G_SR3;
            switch (side_item)
            {
                case LOW_SIDE_1_MOSFET:
                    reg_mask = SR3_DS_MON_LS1_B;
                    reg_pos = SR3_DS_MON_LS1_B_POS;
                    break;
                case HIGH_SIDE_1_MOSFET:
                    reg_mask = SR3_DS_MON_HS1_B;
                    reg_pos = SR3_DS_MON_HS1_B_POS;
                    break;
                case LOW_SIDE_2_MOSFET:
                    reg_mask = SR3_DS_MON_LS2_B;
                    reg_pos = SR3_DS_MON_LS2_B_POS;
                    break;
                case HIGH_SIDE_2_MOSFET:
                    reg_mask = SR3_DS_MON_HS2_B;
                    reg_pos = SR3_DS_MON_HS2_B_POS;
                    break;
                default:
                    return L99DZ200G_FAIL;    // invalid MOSFET device item
            }
            break;
        default:
            return L99DZ200G_FAIL;    // invalid H-bridge item
    }
    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear Drain-Source monitoring status for specified L99DZ200G H-bridge  - SR2, SR3
void DLK_L99DZ200G::L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(uint8_t H_bridge)
{
    uint8_t reg;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = L99DZ200G_SR2;
            reg_mask = SR2_DS_MON_LS1_A | SR2_DS_MON_LS2_A | SR2_DS_MON_HS1_A | SR2_DS_MON_HS2_A;
            break;
        case H_BRIDGE_CONTROL_B:
            reg = L99DZ200G_SR3;
            reg_mask = SR3_DS_MON_LS1_B | SR3_DS_MON_LS2_B | SR3_DS_MON_HS1_B | SR3_DS_MON_HS2_B;
            break;
        default:
            return;                 // invalid H-bridge item
    }
    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Clear the specified H-bridge MOSFET device short circuit/open-load status - SR2, SR3
void DLK_L99DZ200G::L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(uint8_t H_bridge, uint8_t side_item)
{
    uint8_t reg;
    uint32_t reg_mask;

    switch (H_bridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = L99DZ200G_SR2;
            switch (side_item)
            {
                case ALL_HB_SIDE_ITEMS:
                    L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(H_bridge);
                    return;
                    break;
                case LOW_SIDE_1_MOSFET:
                    reg_mask = SR2_DS_MON_LS1_A;
                    break;
                case HIGH_SIDE_1_MOSFET:
                    reg_mask = SR2_DS_MON_HS1_A;
                    break;
                case LOW_SIDE_2_MOSFET:
                    reg_mask = SR2_DS_MON_LS2_A;
                    break;
                case HIGH_SIDE_2_MOSFET:
                    reg_mask = SR2_DS_MON_HS2_A;
                    break;
                default:
                    return;         // invalid MOSFET device item
            }
            break;
        case H_BRIDGE_CONTROL_B:
            reg = L99DZ200G_SR3;
            switch (side_item)
            {
                case ALL_HB_SIDE_ITEMS:
                    L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(H_bridge);
                    return;
                    break;
                case LOW_SIDE_1_MOSFET:
                    reg_mask = SR3_DS_MON_LS1_B;
                    break;
                case HIGH_SIDE_1_MOSFET:
                    reg_mask = SR3_DS_MON_HS1_B;
                    break;
                case LOW_SIDE_2_MOSFET:
                    reg_mask = SR3_DS_MON_LS2_B;
                    break;
                case HIGH_SIDE_2_MOSFET:
                    reg_mask = SR3_DS_MON_HS2_B;
                    break;
                default:
                    return;         // invalid MOSFET device item
            }
            break;
        default:
            return;                 // invalid H-bridge item
    }
    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the specified LIN item status - SR2
uint8_t DLK_L99DZ200G::L99DZ200G_Get_LIN_Status(uint8_t lin_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (lin_item)
    {
        case LIN_PERM_REC_ITEM:
            reg_mask = SR2_LIN_PERM_REC;
            reg_pos = SR2_LIN_PERM_REC_POS;
            break;
        case LIN_TXD_DOM_ITEM:
            reg_mask = SR2_LIN_TXD_DOM;
            reg_pos = SR2_LIN_TXD_DOM_POS;
            break;
        case LIN_PERM_DOM_ITEM:
            reg_mask = SR2_LIN_PERM_DOM;
            reg_pos = SR2_LIN_PERM_DOM_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid LIN item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR2);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified LIN item status - SR2
void DLK_L99DZ200G::L99DZ200G_Clear_LIN_Status(uint8_t lin_item)
{
    uint32_t reg_mask;

    switch (lin_item)
    {
        case ALL_LIN_ITEMS:
            reg_mask = SR2_LIN_PERM_REC | SR2_LIN_TXD_DOM | SR2_LIN_PERM_DOM;
            break;
        case LIN_PERM_REC_ITEM:
            reg_mask = SR2_LIN_PERM_REC;
            break;
        case LIN_TXD_DOM_ITEM:
            reg_mask = SR2_LIN_TXD_DOM;
            break;
        case LIN_PERM_DOM_ITEM:
            reg_mask = SR2_LIN_PERM_DOM;
            break;

        default:
            return;         // invalid LIN item
    }

    L99DZ200G_ReadClearRegister(L99DZ200G_SR2, reg_mask);
}

// Get the specified CAN item status - SR2, SR12
uint8_t DLK_L99DZ200G::L99DZ200G_Get_CAN_Status(uint8_t can_item)
{
    uint8_t reg = L99DZ200G_SR2;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (can_item)
    {
        case CAN_SUP_LOW_ITEM:
            reg_mask = SR2_CAN_SUP_LOW;
            reg_pos = SR2_CAN_SUP_LOW_POS;
            break;
        case CAN_TXD_DOM_ITEM:
            reg_mask = SR2_CAN_TXD_DOM;
            reg_pos = SR2_CAN_TXD_DOM_POS;
            break;
        case CAN_PERM_DOM_ITEM:
            reg_mask = SR2_CAN_PERM_DOM;
            reg_pos = SR2_CAN_PERM_DOM_POS;
            break;
        case CAN_PERM_REC_ITEM:
            reg_mask = SR2_CAN_PERM_REC;
            reg_pos = SR2_CAN_PERM_REC_POS;
            break;
        case CAN_RXD_REC_ITEM:
            reg_mask = SR2_CAN_RXD_REC;
            reg_pos = SR2_CAN_RXD_REC_POS;
            break;
        case CANTO_ITEM:
            reg = L99DZ200G_SR12;
            reg_mask = SR12_CANTO;
            reg_pos = SR12_CANTO_POS;
            break;
        case CAN_SILENT_ITEM:
            reg = L99DZ200G_SR12;
            reg_mask = SR12_CAN_SILENT;
            reg_pos = SR12_CAN_SILENT_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid CAN item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified CAN item status - SR2, SR12
void DLK_L99DZ200G::L99DZ200G_Clear_CAN_Status(uint8_t can_item)
{
    uint8_t reg = L99DZ200G_SR2;
    uint32_t reg_mask;

    switch (can_item)
    {
        case ALL_CAN_ITEMS:
            reg_mask = SR2_CAN_SUP_LOW | SR2_CAN_TXD_DOM | SR2_CAN_PERM_DOM |
                       SR2_CAN_PERM_REC | SR2_CAN_RXD_REC;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case CANTO_ITEM:
            reg = L99DZ200G_SR12;
            reg_mask = SR12_CANTO;
            break;
        case CAN_SUP_LOW_ITEM:
            reg_mask = SR2_CAN_SUP_LOW;
            break;
        case CAN_TXD_DOM_ITEM:
            reg_mask = SR2_CAN_TXD_DOM;
            break;
        case CAN_PERM_DOM_ITEM:
            reg_mask = SR2_CAN_PERM_DOM;
            break;
        case CAN_PERM_REC_ITEM:
            reg_mask = SR2_CAN_PERM_REC;
            break;
        case CAN_RXD_REC_ITEM:
            reg_mask = SR2_CAN_RXD_REC;
            break;

        default:
            return;         // invalid CAN item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the count of watchdog failures - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetWdogFailCount(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & SR1_WDC_FAIL_CNT_MASK) >> SR1_WDC_FAIL_CNT_POS;

    return reg_data;
}

// Get the watchdog timer status - SR6
uint8_t DLK_L99DZ200G::L99DZ200G_GetWdogTimerStatus(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR6);
    reg_data = (reg_data & SR6_WD_TMR_STATE_MASK) >> SR6_WD_TMR_STATE_POS;

    return reg_data;
}

// Get the specified forced sleep (Forced VBAT_Standby) item status - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetForcedSleepStatus(uint8_t forced_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (forced_item)
    {
        case FORCED_SLEEP_WD_ITEM:
            reg_mask = SR1_F_SLEEP_WDC;
            reg_pos = SR1_F_SLEEP_WDC_POS;
            break;
        case FORCED_SLEEP_TSD2_V1SC_ITEM:
            reg_mask = SR1_F_SLEEP_TSD;
            reg_pos = SR1_F_SLEEP_TSD_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid forced sleep item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified forced sleep (Forced VBAT_Standby) item status - SR1
void DLK_L99DZ200G::L99DZ200G_ClearForcedSleepStatus(uint8_t forced_item)
{
    uint32_t reg_mask;

    switch (forced_item)
    {
        case ALL_FORCED_SLEEP_ITEMS:
            reg_mask = SR1_F_SLEEP_WDC | SR1_F_SLEEP_TSD;
            break;
        case FORCED_SLEEP_WD_ITEM:
            reg_mask = SR1_F_SLEEP_WDC;
            break;
        case FORCED_SLEEP_TSD2_V1SC_ITEM:
            reg_mask = SR1_F_SLEEP_TSD;
            break;

        default:
            return;         // invalid forced sleep item
    }

    L99DZ200G_ReadClearRegister(L99DZ200G_SR1, reg_mask);
}

// Get the specified miscellaneous item status - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetMiscellaneousStatus(uint8_t misc_item)
{
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (misc_item)
    {
        case SGND_LOSS_ITEM:
            reg_mask = SR1_SGND_LOSS;
            reg_pos = SR1_SGND_LOSS_POS;
            break;
        case DEBUG_MODE_ITEM:
            reg_mask = SR1_DEBUG_MODE;
            reg_pos = SR1_DEBUG_MODE_POS;
            break;
        case VSPOR_ITEM:
            reg_mask = SR1_VSPOR;
            reg_pos = SR1_VSPOR_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid miscellaneous item
    }

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified miscellaneous item status - SR1
void DLK_L99DZ200G::L99DZ200G_ClearMiscellaneousStatus(uint8_t misc_item)
{
    uint32_t reg_mask;

    switch (misc_item)
    {
        case ALL_MISC_ITEMS:
            reg_mask = SR1_SGND_LOSS | SR1_VSPOR;
            break;
        case SGND_LOSS_ITEM:
            reg_mask = SR1_SGND_LOSS;
            break;
        case VSPOR_ITEM:
            reg_mask = SR1_VSPOR;
            break;

        default:
            return;         // invalid miscellaneous item
    }

    L99DZ200G_ReadClearRegister(L99DZ200G_SR1, reg_mask);
}

// Get the watchdog failure status - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetWdogFailStatus(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & SR1_WD_FAIL) >> SR1_WD_FAIL_POS;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the watchdog failure status - SR1
void DLK_L99DZ200G::L99DZ200G_ClearWdogFailStatus()
{
    L99DZ200G_ReadClearRegister(L99DZ200G_SR1, SR1_WD_FAIL);
}

// Get the specified specified wake-up item status - SR1, SR12
uint8_t DLK_L99DZ200G::L99DZ200G_GetWakeUpStatus(uint8_t wake_item)
{
    uint8_t reg = L99DZ200G_SR1;
    uint32_t reg_mask;
    uint32_t reg_pos;
    uint32_t reg_data;

    switch (wake_item)
    {
        case WK_TIMER_ITEM:
            reg_mask = SR1_WK_TIMER;
            reg_pos = SR1_WK_TIMER_POS;
            break;
        case WK_LIN_ITEM:
            reg_mask = SR1_WK_LIN;
            reg_pos = SR1_WK_LIN_POS;
            break;
        case WK_CAN_ITEM:
            reg_mask = SR1_WK_CAN;
            reg_pos = SR1_WK_CAN_POS;
            break;
        case WK_WU_ITEM:
            reg_mask = SR1_WK_WU;
            reg_pos = SR1_WK_WU_POS;
            break;
        case WK_VS_OV_ITEM:
            reg_mask = SR1_WK_VS_OV;
            reg_pos = SR1_WK_VS_OV_POS;
            break;
        case WK_CAN_WUP_ITEM:
            reg = L99DZ200G_SR12;
            reg_mask = SR12_CAN_WUP;
            reg_pos = SR12_CAN_WUP_POS;
            break;

        default:
            return L99DZ200G_FAIL;    // invalid miscellaneous item
    }

    reg_data = L99DZ200G_ReadRegister(reg);
    reg_data = (reg_data & reg_mask) >> reg_pos;
    if (reg_data)
    {
        return L99DZ200G_FAIL;
    }
    else
    {
        return L99DZ200G_OK;
    }
}

// Clear the specified specified wake-up item status - SR1, SR12
void DLK_L99DZ200G::L99DZ200G_ClearWakeUpStatus(uint8_t wake_item)
{
    uint8_t reg = L99DZ200G_SR1;
    uint32_t reg_mask;

    switch (wake_item)
    {
        case ALL_WK_ITEMS:
            reg_mask = SR1_WK_TIMER | SR1_WK_LIN | SR1_WK_CAN | SR1_WK_WU | SR1_WK_VS_OV;
            L99DZ200G_ReadClearRegister(reg, reg_mask);
            // Fall-Thru
        case WK_CAN_WUP_ITEM:
            reg = L99DZ200G_SR12;
            reg_mask = SR12_CAN_WUP;
            break;
        case WK_TIMER_ITEM:
            reg_mask = SR1_WK_TIMER;
            break;
        case WK_LIN_ITEM:
            reg_mask = SR1_WK_LIN;
            break;
        case WK_CAN_ITEM:
            reg_mask = SR1_WK_CAN;
            break;
        case WK_WU_ITEM:
            reg_mask = SR1_WK_WU;
            break;
        case WK_VS_OV_ITEM:
            reg_mask = SR1_WK_VS_OV;
            break;

        default:
            return;         // invalid miscellaneous item
    }

    L99DZ200G_ReadClearRegister(reg, reg_mask);
}

// Get the device wake-up state - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_GetDeviceWakeUpState(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & SR1_DEV_STATE_MASK) >> SR1_DEV_STATE_POS;

    return reg_data;
}

// Clear the device wake-up state - SR1
void DLK_L99DZ200G::L99DZ200G_ClearDeviceWakeUpState(void)
{
    L99DZ200G_ReadClearRegister(L99DZ200G_SR1, SR1_DEV_STATE_MASK);
}

// Get the WU wake-up pin state - SR1
uint8_t DLK_L99DZ200G::L99DZ200G_Get_WU_PinState(void)
{
    uint32_t reg_data;

    reg_data = L99DZ200G_ReadRegister(L99DZ200G_SR1);
    reg_data = (reg_data & SR1_WU_PIN_STATE) >> SR1_WU_PIN_STATE_POS;

    return reg_data;
}


// Check if specified L99DZ200G register is writable
uint8_t DLK_L99DZ200G::L99DZ200G_CheckRegisterWritable(uint8_t reg)
{
    if (((reg >= L99DZ200G_CR1) && (reg <= L99DZ200G_CR22)) ||
        (reg == L99DZ200G_CFR))
    {
        return L99DZ200G_OK;
    }
    else
    {
        return L99DZ200G_FAIL;
    }
}

// Retrieve Global Status Byte (GSB)
uint8_t DLK_L99DZ200G::L99DZ200G_GlobalStatusByte(void)
{
    return GlobalStatusRegister;
}

