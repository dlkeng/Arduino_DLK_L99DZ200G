/*
 * NAME: commands.ino
 *
 * WHAT:
 *  Commands for serial command line I/F.
 *
 * SPECIAL CONSIDERATIONS:
 *  None
 *
 * AUTHOR:
 *  D.L. Karmann
 *
 * MODIFIED:
 *
 */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Defines for code to be included

#define SHOW_ADC
#define SHOW_CCM
#define SHOW_ECV
#define SHOW_HEATER
#define SHOW_MIRROR
#define SHOW_MOTOR
#define SHOW_OCR
#define SHOW_OCT
#define SHOW_OLT
#define SHOW_PWM
#define SHOW_RDSON
#define SHOW_SCT
#define SHOW_STAT
#define SHOW_TIMER

// local function prototypes
int8_t Cmd_help(int8_t argc, char * argv[]);
int8_t Cmd_rreg(int8_t argc, char * argv[]);
int8_t Cmd_wreg(int8_t argc, char * argv[]);
int8_t Cmd_rclr(int8_t argc, char * argv[]);

#ifdef SHOW_ADC
int8_t Cmd_adc(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_CCM
int8_t Cmd_ccm(int8_t argc, char * argv[]);
#endif
int8_t Cmd_dir(int8_t argc, char * argv[]);
#ifdef SHOW_ECV
int8_t Cmd_ecv(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_HEATER
int8_t Cmd_ghol(int8_t argc, char * argv[]);
int8_t Cmd_ght(int8_t argc, char * argv[]);
#endif
int8_t Cmd_gsb(int8_t argc, char * argv[]);
int8_t Cmd_init(int8_t argc, char * argv[]);
#ifdef SHOW_MOTOR
int8_t Cmd_motor(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_OCR
int8_t Cmd_ocr(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_OCT
int8_t Cmd_oct(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_OLT
int8_t Cmd_olt(int8_t argc, char * argv[]);
#endif
int8_t Cmd_out(int8_t argc, char * argv[]);
#ifdef SHOW_HEATER
int8_t Cmd_outgh(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_MIRROR
int8_t Cmd_mirror(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_MOTOR
int8_t Cmd_outm(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_PWM
int8_t Cmd_pwm(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_RDSON
int8_t Cmd_rdson(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_SCT
int8_t Cmd_sct(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_STAT
int8_t Cmd_stat(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_TIMER
int8_t Cmd_tmr(int8_t argc, char * argv[]);
#endif
int8_t Cmd_wake(int8_t argc, char * argv[]);

//*****************************************************************************
//
// These are the command names and brief descriptions.
//
// To add a menu item: (to keep all items in Flash)
//   1) add the command string to the 'MenuCmd#' item
//   2) add the command help string to the 'MenuHelp#' item
//   3) add the function prototype for the command's function above
//   4) add the 'MenuCmd#', function's name, and 'MenuHelp#' to the 'g_sCmdTable[]' array
//   5) add the function for processing the command to this file
//
//*****************************************************************************

// menu items individual command name strings
const char MenuCmdHelp1[] PROGMEM = "help";
const char MenuCmdHelp2[] PROGMEM = "h";
const char MenuCmdHelp3[] PROGMEM = "?";
const char MenuCmdRreg[] PROGMEM  = "rreg";
const char MenuCmdWreg[] PROGMEM  = "wreg";
const char MenuCmdRclr[] PROGMEM  = "rclr";
#ifdef SHOW_ADC
const char MenuCmdAdc[] PROGMEM   = "adc";
#endif
#ifdef SHOW_CCM
const char MenuCmdCcm[] PROGMEM   = "ccm";
#endif
const char MenuCmdDir[] PROGMEM   = "dir";
#ifdef SHOW_ECV
const char MenuCmdEcv[] PROGMEM   = "ecv";
#endif
#ifdef SHOW_HEATER
const char MenuCmdGhol[] PROGMEM  = "ghol";
const char MenuCmdGht[] PROGMEM   = "ght";
#endif
const char MenuCmdGsb[] PROGMEM   = "gsb";
const char MenuCmdInit[] PROGMEM  = "init";
#ifdef SHOW_MIRROR
const char MenuCmdMirror[] PROGMEM = "mirror";
#endif
#ifdef SHOW_MOTOR
const char MenuCmdMotor[] PROGMEM = "motor";
#endif
#ifdef SHOW_OCR
const char MenuCmdOcr[] PROGMEM   = "ocr";
#endif
#ifdef SHOW_OCT
const char MenuCmdOct[] PROGMEM   = "oct";
#endif
#ifdef SHOW_OLT
const char MenuCmdOlt[] PROGMEM   = "olt";
#endif
const char MenuCmdOut[] PROGMEM   = "out";
#ifdef SHOW_HEATER
const char MenuCmdOutgh[] PROGMEM = "outgh";
#endif
#ifdef SHOW_MOTOR
const char MenuCmdOutm[] PROGMEM  = "outm";
#endif
#ifdef SHOW_PWM
const char MenuCmdPwm[] PROGMEM   = "pwm";
#endif
#ifdef SHOW_RDSON
const char MenuCmdRdson[] PROGMEM = "rdson";
#endif
#ifdef SHOW_SCT
const char MenuCmdSct[] PROGMEM   = "sct";
#endif
#ifdef SHOW_STAT
const char MenuCmdStat[] PROGMEM  = "stat";
#endif
#ifdef SHOW_TIMER
const char MenuCmdTmr[] PROGMEM   = "tmr";
#endif
const char MenuCmdWake[] PROGMEM  = "wake";

// menu items individual command help strings
const char MenuHelp1[] PROGMEM     =    " [<cls>]                     : Display list of commands (clear screen)";
const char MenuHelp2[] PROGMEM     = "                                : alias for help";
const char MenuHelpRreg[] PROGMEM  =    " reg [cnt]                   : Show L99DZ200G register(s) value(s)";
const char MenuHelpWreg[] PROGMEM  =    " [reg val] | <all>           : Set(reset all) L99DZ200G control register value(s)";
const char MenuHelpRclr[] PROGMEM  =    " [reg val] | <all>           : Read/clear L99DZ200G status register(s)";
#ifdef SHOW_ADC
const char MenuHelpAdc[] PROGMEM   =   " [n | <A> | <B> | <x> | <y>] [<v> | <r>]  : Show analog input value (v = volts, r = raw)";
#endif
#ifdef SHOW_CCM
const char MenuHelpCcm[] PROGMEM   =   " [n [off | on]]               : Show[set] L99DZ200G OUTn constant current mode control";
#endif
const char MenuHelpDir[] PROGMEM   =   " [lo | hi]                    : Show[set] DIR output pin";
#ifdef SHOW_ECV
const char MenuHelpEcv[] PROGMEM   =   " [[ecoff | econ] | [vm12 | vm15] | [lsoff | lson] | [ocoff | ocon] | volts]"
                                "\r\n                                 : Show[set] L99DZ200G ECV output control";
#endif
#ifdef SHOW_HEATER
const char MenuHelpGhol[] PROGMEM  =    " [off | on]                  : Show[set] L99DZ200G Heater open-load diagnosis control";
const char MenuHelpGht[] PROGMEM   =   " [threshold]                  : Show[set] L99DZ200G Heater Drain monitor threshold control";
#endif
const char MenuHelpGsb[] PROGMEM   =   "                              : Show L99DZ200G Global Status Byte";
const char MenuHelpInit[] PROGMEM  =    "                             : Init L99DZ200G";
#ifdef SHOW_MIRROR
const char MenuHelpMirror[] PROGMEM = " idle | brake | xcw | xccw | ycw | yccw | centr | fold | unfold |"
                            "\r\n       htron | htroff | ecvon | ecvoff | farrw | demo : Do MR200G1 mirror control";
#endif
#ifdef SHOW_MOTOR
const char MenuHelpMotor[] PROGMEM =     " [X | Y | F | XY | XF | YF | B] [[L | R | B | S]] : Show[set] L99DZ200G OUTn motor output control";
#endif
#ifdef SHOW_OCR
const char MenuHelpOcr[] PROGMEM   =   " [n [ocon | ocoff] | [thxon | thxoff] | [freq] | [time]] : Show[set] L99DZ200G OUTn overcurrent recovery control";
#endif
#ifdef SHOW_OCT
const char MenuHelpOct[] PROGMEM   =   " [n [lo | hi]]                : Show[set] L99DZ200G OUTn overcurrent threshold control";
#endif
#ifdef SHOW_OLT
const char MenuHelpOlt[] PROGMEM   =   " [n [lo | hi]]                : Show[set] L99DZ200G OUTn open-load threshold control";
#endif
const char MenuHelpOut[] PROGMEM   =   " [n [off | on | t1 | t2 | pwm1 | pwm2 | pwm3 | pwm4 | pwm5 | pwm6 | pwm7 | dir | hb]]"
                                "\r\n                                 : Show[set] L99DZ200G OUTn output control";
#ifdef SHOW_HEATER
const char MenuHelpOutgh[] PROGMEM =     " [off | on]                 : Show[set] L99DZ200G Heater output control";
#endif
#ifdef SHOW_MOTOR
const char MenuHelpOutm[] PROGMEM  =    " [n [off | lo | hi]]         : Show[set] L99DZ200G OUTm motor output control";
#endif
#ifdef SHOW_PWM
const char MenuHelpPwm[] PROGMEM   =   " [chan [freq [duty]]]         : Show[set] L99DZ200G PWM settings";
#endif
#ifdef SHOW_RDSON
const char MenuHelpRdson[] PROGMEM =     " [n [lo | hi]]              : Show[set] L99DZ200G OUTn Rdson output control";
#endif
#ifdef SHOW_SCT
const char MenuHelpSct[] PROGMEM   =   " [n [off | on]]               : Show[set] L99DZ200G OUTn short circuit threshold control";
#endif
#ifdef SHOW_STAT
const char MenuHelpStat[] PROGMEM  =    "                             : Show all or specific L99DZ200G status registers";
#endif
#ifdef SHOW_TIMER
const char MenuHelpTmr[] PROGMEM   =   " [n [per [ton [rst [dir]]]    : Show[set] L99DZ200G Timer settings";
#endif
const char MenuHelpWake[] PROGMEM  =    "                             : Pulse Wakeup output pin";

//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description. (Required by the 'CommandLine' command processor.)
//
//*****************************************************************************
const tCmdLineEntry g_sCmdTable[] PROGMEM =
{
    //  command       function     help info
    { MenuCmdHelp1,   Cmd_help,    MenuHelp1       },
    { MenuCmdHelp2,   Cmd_help,    MenuHelp2       },
    { MenuCmdHelp3,   Cmd_help,    MenuHelp2       },
    { MenuCmdRreg,    Cmd_rreg,    MenuHelpRreg    },
    { MenuCmdWreg,    Cmd_wreg,    MenuHelpWreg    },
    { MenuCmdRclr,    Cmd_rclr,    MenuHelpRclr    },
#ifdef SHOW_ADC
    { MenuCmdAdc,     Cmd_adc,     MenuHelpAdc     },
#endif
#ifdef SHOW_CCM
    { MenuCmdCcm,     Cmd_ccm,     MenuHelpCcm     },
#endif
    { MenuCmdDir,     Cmd_dir,     MenuHelpDir     },
#ifdef SHOW_ECV
    { MenuCmdEcv,     Cmd_ecv,     MenuHelpEcv     },
#endif
#ifdef SHOW_HEATER
    { MenuCmdGhol,    Cmd_ghol,    MenuHelpGhol    },
    { MenuCmdGht,     Cmd_ght,     MenuHelpGht     },
#endif
    { MenuCmdGsb,     Cmd_gsb,     MenuHelpGsb     },
    { MenuCmdInit,    Cmd_init,    MenuHelpInit    },
#ifdef SHOW_MIRROR
    { MenuCmdMirror,  Cmd_mirror,  MenuHelpMirror  },
#endif
#ifdef SHOW_MOTOR
    { MenuCmdMotor,   Cmd_motor,   MenuHelpMotor   },
#endif
#ifdef SHOW_OCR
    { MenuCmdOcr,     Cmd_ocr,     MenuHelpOcr     },
#endif
#ifdef SHOW_OCT
    { MenuCmdOct,     Cmd_oct,     MenuHelpOct     },
#endif
#ifdef SHOW_OLT
    { MenuCmdOlt,     Cmd_olt,     MenuHelpOlt     },
#endif
    { MenuCmdOut,     Cmd_out,     MenuHelpOut     },
#ifdef SHOW_HEATER
    { MenuCmdOutgh,   Cmd_outgh,   MenuHelpOutgh   },
#endif
#ifdef SHOW_MOTOR
    { MenuCmdOutm,    Cmd_outm,    MenuHelpOutm    },
#endif
#ifdef SHOW_PWM
    { MenuCmdPwm,     Cmd_pwm,     MenuHelpPwm     },
#endif
#ifdef SHOW_RDSON
    { MenuCmdRdson,   Cmd_rdson,   MenuHelpRdson   },
#endif
#ifdef SHOW_SCT
    { MenuCmdSct,     Cmd_sct,     MenuHelpSct     },
#endif
#ifdef SHOW_STAT
    { MenuCmdStat,    Cmd_stat,    MenuHelpStat    },
#endif
#ifdef SHOW_TIMER
    { MenuCmdTmr,     Cmd_tmr,     MenuHelpTmr     },
#endif
    { MenuCmdWake,    Cmd_wake,    MenuHelpWake    },
    { 0, 0, 0 }     // end of commands
};

/*
 * NAME:
 *  int8_t Cmd_rreg(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "rreg" command to read and show L99DZ200G register(s).
 *
 *  One required parameter supported.
 *   reg = the register(s) to read from
 *  One optional parameter supported.
 *   cnt = the number of register(s) to read from
 *
 *        1   2    3
 *     "rreg 0x10"      - read L99DZ200G register 0x10 (CR16)
 *     "rreg 0x02 10"   - read L99DZ200G registers 0x02 to 0x0b (CR2 to CR11)
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_rreg(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t regaddr;
    uint8_t regcnt = 1;

    if (argc < 2)
    {
        return CMDLINE_TOO_FEW_ARGS;
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
DEBUG_TOGL();
        //                           CR1     CR22  SR1     SR12  CFR
        // get the register address (0x01 to 0x16, 0x31 to 0x3c, 0x3f)
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL)  ||
            (val < L99DZ200G_CR1)                             ||
            ((val > L99DZ200G_CR22) && (val < L99DZ200G_SR1)) ||
            ((val > L99DZ200G_SR12) && (val < L99DZ200G_CFR)) ||
            (val > L99DZ200G_CFR))
        {
            return CMDLINE_INVALID_ARG;
        }
        regaddr = val;
        if (argc > 2)
        {
            // get the count of registers to read from
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 1) || (val > 20))
            {
                return CMDLINE_INVALID_ARG;
            }
            regcnt = val;
        }

        for (uint8_t i = 0; i < regcnt; ++i)
        {
            if ((regcnt > 1) && (i > 0))
            {
                // check within bounds
                if (regaddr < L99DZ200G_CR22)
                {
                    if ((regaddr + i) > L99DZ200G_CR22)
                    {
                        break;      // invalid register
                    }
                }
                else if ((regaddr >= L99DZ200G_SR1) && (regaddr <= L99DZ200G_SR12))
                {
                    if ((regaddr + i) > L99DZ200G_SR12)
                    {
                        break;      // invalid register
                    }
                }
                else if (regaddr == L99DZ200G_CFR)
                {
                    if ((regaddr + i) > L99DZ200G_CFR)
                    {
                        break;      // invalid register
                    }
                }
            }
            ShowReg(regaddr + i);
        }
DEBUG_TOGL();
    }

    // Return success.
    return 0;
}

// get and show L99DZ200G register
void ShowReg(uint8_t regaddr)
{
    uint32_t regval;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    Serial.print(F("L99DZ200G "));
    if ((regaddr >= L99DZ200G_CR1) && (regaddr <= L99DZ200G_CR34))
    {
        Serial.print(F("CR"));
        Serial.print(regaddr);
    }
    else if ((regaddr >= L99DZ200G_SR1) && (regaddr <= L99DZ200G_SR12))
    {
        Serial.print(F("SR"));
        Serial.print(regaddr - L99DZ200G_SR1 + 1);
    }
    else if ((regaddr == L99DZ200G_CFR))
    {
        Serial.print(F("CFR"));
    }
    Serial.print(F(": "));
    Print0xHexByte(regaddr);
    Serial.print(F(": "));
    regval = L99dz200g.L99DZ200G_ReadRegister(regaddr);
    Print0xHex24ln(regval);
    switch (regaddr)
    {
        case L99DZ200G_CR1:
            ShowCR1_Reg(regval);
            break;
        case L99DZ200G_CFR:
            ShowConfig_Reg(regval);
            break;
    }
}

// show L99DZ200G CR1 register
void ShowCR1_Reg(uint32_t regval)
{
    uint32_t regcfr;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    regcfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);

    Serial.print(F(" - [22]WU_EN:               "));
    Serial.print(F("Wake-up Input ("));
    if (regcfr & CFR_WU_CFG_MASK)
    {
        Serial.print(F("VBAT"));
    }
    else
    {
        Serial.print(F("wake-up"));
    }
    Serial.print(F(") = "));
    ShowEnabledDisabled((regval & CR1_WU_EN_MASK), true);

    Serial.print(F(" - [20]WU_PU:               "));
    Serial.print(F("Wake-up Pull = pull-"));
    if (regval & CR1_WU_PU_MASK)
    {
        Serial.print(F("up"));
    }
    else
    {
        Serial.print(F("down"));
    }
    Serial.println();

    Serial.print(F(" - [17:16]WU_FILT[1:0]:     "));
    Serial.print(F("Wake-up Filter = "));
    switch ((regval & CR1_WU_FILTER_MASK) >> CR1_WU_FILTER_POS)
    {
        case WU_FILTER_STATIC:
            Serial.print(F("Static"));
            break;
        case WU_FILTER_T1:
            Serial.print(F("Cyclic T1"));
            break;
        case WU_FILTER_T2:
            Serial.print(F("Cyclic T2"));
            break;
        default:
            break;
    }
    Serial.println();

    Serial.print(F(" - [15]TIMER_NINT_WAKE_SEL: "));
    Serial.print(F("Timer NINT/wake = T"));
    if (regval & CR1_TMR_NINT_SEL_MASK)
    {
        Serial.print(1);
    }
    else
    {
        Serial.print(2);
    }
    Serial.println();

    Serial.print(F(" - [14]TIMER_NINT_EN:       "));
    Serial.print(F("Timer NINT = "));
    ShowEnabledDisabled((regval & CR1_TMR_NINT_EN_MASK), true);

    Serial.print(F(" - [13]LIN_WU_EN:           "));
    Serial.print(F("LIN Wake-up = "));
    ShowEnabledDisabled((regval & CR1_LIN_WU_EN_MASK), true);

    Serial.print(F(" - [12]CAN_WU_EN:           "));
    Serial.print(F("CAN Wake-up = "));
    ShowEnabledDisabled((regval & CR1_CAN_WU_EN_MASK), true);

    Serial.print(F(" - [11]CANTO_IRQ_EN:        "));
    Serial.print(F("CAN Timeout IRQ = "));
    ShowEnabledDisabled((regval & CR1_CAN_TOUT_IRQ_MASK), true);

    Serial.print(F(" - [10]CAN RXEN:            "));
    Serial.print(F("CAN RX = "));
    ShowEnabledDisabled(((regval & CR1_CAN_RXEN_MASK) >> CR1_CAN_RXEN_POS), true);

    Serial.print(F(" - [9]CAN TXEN:             "));
    Serial.print(F("CAN TX = "));
    ShowEnabledDisabled(((regval & CR1_CAN_TXEN_MASK) >> CR1_CAN_TXEN_POS), true);

    Serial.print(F(" - [8]CAN_GO_TRX_RDY:       "));
    Serial.print(F("CAN Xcvr = TRX "));
    if (regval & CR1_WU_EN_MASK)
    {
        Serial.print(F("BIAS"));
    }
    else
    {
        Serial.print(F("READY"));
    }
    Serial.println();

    Serial.print(F(" - [7]HENA:                 "));
    Serial.print(F("H-bridge A = "));
    ShowEnabledDisabled((regval & CR1_HENA_MASK), true);

    Serial.print(F(" - [6]HENB:                 "));
    Serial.print(F("H-bridge B = "));
    ShowEnabledDisabled((regval & CR1_HENB_MASK), true);

    Serial.print(F(" - [5:4]V2_[1:0]:           "));
    Serial.print(F("5V2 Config = "));
    switch (regval & CR1_V2_MODE_MASK)
    {
        case OFF_V2:
            Serial.print("Off");
            break;
        case ON_ACTIVEMODE:
            Serial.print("Standby Off");
            break;
        case ON_ACTIVEMODE_V1STANDBY:
            Serial.print("VBAT Standby Off");
            break;
        case ALWAYS_ON:
            Serial.print("On");
            break;
    }
    Serial.println();

    Serial.print(F(" - [3:1]STBY_[2:0]:         "));
    Serial.print(F("Standby = "));
    switch((regval & CR1_STBY_MASK) >> CR1_STBY_POS)
    {
        case GOTO_V1_STANDBY:
            Serial.print(F("V1"));
            break;
        case GOTO_VBAT_STANDBY:
            Serial.print(F("Vbat"));
            break;
        default:
            Serial.print(F("Active"));
            break;
    }
    Serial.println();

    Serial.print(F(" - [0]TRIG:                 "));
    Serial.print(F("WDOG Trig = "));
    Serial.println((regval & CR1_TRIG_MASK) >> CR1_TRIG__POS);
}

// show L99DZ200G Config register
void ShowConfig_Reg(uint32_t regval)
{
    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    if (regval)
    {
        Serial.print(F(" -"));
    }
    else
    {
        return;
    }
    if (regval & CFR_WU_CFG_MASK)
    {
        Serial.print(F(" WU_CONFIG"));
    }
    if (regval & CFR_LIN_WU_CFG_MASK)
    {
        Serial.print(F(" LIN_WU_CONFIG"));
    }
    if (regval & CFR_LIN_HS_EN_MASK)
    {
        Serial.print(F(" LIN_HS_EN"));
    }
    if (regval & CFR_TSD_CFG_MASK)
    {
        Serial.print(F(" TSD_CONFIG"));
    }
    if (regval & CFR_ECV_HV_MASK)
    {
        Serial.print(F(" ECV_HV"));
    }
    if (regval & CFR_V2_CFG_MASK)
    {
        Serial.print(F(" V2_CONFIG"));
    }
    if (regval & CFR_ICMP_CFG_EN_MASK)
    {
        Serial.print(F(" ICMP_CONFIG_EN"));
    }
    if (regval & CFR_WD_CFG_EN_MASK)
    {
        Serial.print(F(" WD_CONFIG_EN"));
    }
    if (regval & CFR_MASK_OL_HS_MASK)
    {
        Serial.print(F(" MASK_OL_HS"));
    }
    if (regval & CFR_MASK_OL_LS_MASK)
    {
        Serial.print(F(" MASK_OL_LS"));
    }
    if (regval & CFR_MASK_TW_MASK)
    {
        Serial.print(F(" MASK_TW"));
    }
    if (regval & CFR_MASK_EC_OL_MASK)
    {
        Serial.print(F(" MASK_EC_OL"));
    }
    if (regval & CFR_MASK_OL_MASK)
    {
        Serial.print(F(" MASK_OL"));
    }
    if (regval & CFR_MASK_SPIE_MASK)
    {
        Serial.print(F(" MASK_SPIE"));
    }
    if (regval & CFR_MASK_PLE_MASK)
    {
        Serial.print(F(" MASK_PLE"));
    }
    if (regval & CFR_MASK_GW_MASK)
    {
        Serial.print(F(" MASK_GW"));
    }
    if (regval & CFR_CP_OFF_EN_MASK)
    {
        Serial.print(F(" CP_OFF_EN"));
    }
    if (regval & CFR_CP_LOW_CFG_MASK)
    {
        Serial.print(F(" CP_LOW_CONFIG"));
    }
    if (regval & CFR_CP_DITH_DIS_MASK)
    {
        Serial.print(F(" CP_DITH_DIS"));
    }
    if (regval & CFR_FS_FORCED_MASK)
    {
        Serial.print(F(" FS_FORCED"));
    }
    if (regval & CFR_DM_HB_A_MASK)
    {
        Serial.print(F(" DMA"));
    }
    if (regval & CFR_DM_HB_B_MASK)
    {
        Serial.print(F(" DMB"));
    }
    if (regval & CFR_WDC_MASK)
    {
        Serial.print(F(" TRIG"));
    }
    Serial.println();
}

/*
 * NAME:
 *  int8_t Cmd_wreg(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "wreg" command to write (or reset all) L99DZ200G register(s).
 *
 *  Two required parameters supported.
 *   reg = the register to write to
 *   val = the value to write to the register
 *  One optional parameter supported.
 *   <all> = reset all L99DZ200G Control Registers to defaults
 *
 *        1   2      3
 *     "wreg 0x10 0xaa55a5"  - write L99DZ200G register 0x10 with value 0xaa55a5
 *     "wreg all"            - reset all L99DZ200G Control Registers to defaults
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_wreg(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t wd_ttime;
    uint8_t regaddr;
    uint32_t regval;
    uint32_t tmp_data;

    if (argc < 2)
    {
        return CMDLINE_TOO_FEW_ARGS;
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        if (!isdigit(argv[ARG1][0]))
        {
            if (strcmp_P(argv[ARG1], PSTR("all")) == 0)
            {
                if (argc > 2)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else
                {
                    // save the Watchdog Trigger Time
                    wd_ttime = L99dz200g.L99DZ200G_GetWdogTime();
DEBUG_TOGL();
                    // special case - reset all Control Registers to defaults
                    L99dz200g.L99DZ200G_ResetAllControlRegisters();
DEBUG_TOGL();
                    // restore the Watchdog Trigger Time to prevent Watchdog failures
                    L99dz200g.L99DZ200G_SetWdogTime(wd_ttime);
                }
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }
        else if (argc < 3)
        {
            return CMDLINE_TOO_FEW_ARGS;
        }
        else
        {
            // get the register address
            paramtype = CmdLine.ParseParam(argv[ARG1], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) || (val > L99DZ200G_CFR))
            {
                return CMDLINE_INVALID_ARG;
            }
            if (L99dz200g.L99DZ200G_CheckRegisterWritable(val) != L99DZ200G_OK)
            {
                return CMDLINE_INVALID_ARG;
            }
            regaddr = val;

            // get the value to write
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 0) || (val > FULL_REG_MASK))
            {
                return CMDLINE_INVALID_ARG;
            }

            regval = val;

            // special considerations for TRIG bits
            if (regaddr == L99DZ200G_CR1)
            {
                tmp_data = L99dz200g.L99DZ200G_ReadRegister(regaddr);
                tmp_data &= CR1_WDC_MASK;       // save TRIG bit
                regval &= ~CR1_WDC_MASK;        // clear TRIG bit
                tmp_data |= regval;             // merge other write data
                L99dz200g.L99DZ200G_WriteControlRegister(regaddr, tmp_data);
            }
            else if (regaddr == L99DZ200G_CFR)
            {
                tmp_data = L99dz200g.L99DZ200G_ReadRegister(regaddr);
                tmp_data &= CFR_WDC_MASK;       // save TRIG bit
                regval &= ~CFR_WDC_MASK;        // clear TRIG bit
                tmp_data |= regval;             // merge other write data
                L99dz200g.L99DZ200G_WriteControlRegister(regaddr, tmp_data);
            }
            else
            {
                // write the value to the register
                L99dz200g.L99DZ200G_WriteControlRegister(regaddr, regval);
            }
        }
    }

    // Return success.
    return 0;
}

/*
 * NAME:
 *  int8_t Cmd_rclr(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "rclr" command to read and clear an L99DZ200G status register.
 *
 *  Two required parameters supported.
 *   reg = the status register to be read and cleared
 *   val = the mask value of bits to be cleared in the register
 *      -or-
 *   <all> = read/clear all L99DZ200G status registers
 *
 *        1    2      3
 *     "rclr 0x32 0xc00000" - read/clear L99DZ200G status register 0x32 with mask value 0xc00000 (hi 2 bits)
 *     "rclr all"           - read/clear all L99DZ200G status registers
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_rclr(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t regaddr;
    uint32_t regmask;

    if (argc < 2)
    {
        return CMDLINE_TOO_FEW_ARGS;
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        if (!isdigit(argv[ARG1][0]))
        {
            if (strcmp_P(argv[ARG1], PSTR("all")) == 0)
            {
                if (argc > 2)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else
                {
                    // special case - clear all status registers
                    L99dz200g.L99DZ200G_ClearAllStatusRegisters();
                }
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }
        else if (argc < 3)
        {
            return CMDLINE_TOO_FEW_ARGS;
        }
        else
        {
            // get the register address
            paramtype = CmdLine.ParseParam(argv[ARG1], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL))
            {
                return CMDLINE_INVALID_ARG;
            }
            if ((val < L99DZ200G_SR1) || ((val > L99DZ200G_SR12) && (val != L99DZ200G_CFR)))
            {
                return CMDLINE_INVALID_ARG;
            }
            regaddr = val;

            // get the mask value to use
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 0) || (val > FULL_REG_MASK))
            {
                return CMDLINE_INVALID_ARG;
            }
            regmask = val;

            // read and clear the mask value in the register
            L99dz200g.L99DZ200G_ReadClearRegister(regaddr, regmask);
        }
    }

    // Return success.
    return 0;
}

/*
 * NAME:
 *  int8_t Cmd_init(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "init" command to reset and initialize L99DZ200G.
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_init(int8_t argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    return L99DZ200G_Init();
}

// do L99DZ200G initialization
uint8_t L99DZ200G_Init(void)
{
    if (L99dz200g.L99DZ200G_Init() != L99DZ200G_OK)
    {
        return L99DZ200G_FAIL;
    }

    L99dz200g.L99DZ200G_WdogTrigger();

    // Ensure Forced FSO outputs is disabled
    L99dz200g.L99DZ200G_Set_HB_Forced_LSx_FSO_Control(DISABLE);

    L99dz200g.L99DZ200G_ClearAllStatusRegisters();

    // Read and clear SR1 (exit from fail safe)
//    L99dz200g.L99DZ200G_ReadClearRegister(L99DZ200G_SR1, 0);
    L99dz200g.L99DZ200G_ReadClearRegister(L99DZ200G_SR1, FULL_REG_MASK);

    // clear standby mode setting
    L99dz200g.L99DZ200G_SetModeControl(ACTIVE_STBY);

    L99dz200g.L99DZ200G_SetWdogTime(WDOG_TIME_200MS);

    L99dz200g.L99DZ200G_SetWatchdogRunning(true);

#if 1   // for testing
    // Disable V1 load current supervision
    L99dz200g.L99DZ200G_Set_ICMP_Control(IC_DISABLE);

    // Set WU pin to wake-up input
    L99dz200g.L99DZ200G_Set_WU_PinModeConfig(WU_WU);
#endif

#if 1
    // Set OCR Overcurrent recovery to protect for overcurrents
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_1, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_2, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_3, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_6, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_7, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_8, ENABLE);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(OUT_15, ENABLE);

    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryTime(OUT_1_2_3_6, OCR_TON_64US);
    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryFrequency(OUT_1_2_3_6, OCR_FREQ_4_4KHZ);
#endif

    // ECV fast discharge activated - this causes -> GSB: 0x08, SR5: 0x000080
//    L99dz200g.L99DZ200G_Set_ECV_FastDischargeControl(ENABLE);

    // Set V2 on as 5V regulator
    L99dz200g.L99DZ200G_V2_Config(ON_ACTIVEMODE, V2_VREG_TYPE);

    L99dz200g.L99DZ200G_Delay(500);

#if 0
    // Setting the Constant Current mode for OUT7 and OUT8
    // Note: This is only in effect for the first 10 mS after the OUTn goes high!
    L99dz200g.L99DZ200G_SetConstantCurrentModeControl(OUT_7, ENABLE);
    L99dz200g.L99DZ200G_SetConstantCurrentModeControl(OUT_8, ENABLE);
#endif

    // enable DIR input on L99DZ200G CM_DIR pin
    L99dz200g.L99DZ200G_CM_DIR_Config(DIR_ALWAYS);

    // Setting the Maximum voltage of Electrochromic
    L99dz200g.L99DZ200G_Set_ECV_MaxVoltage(ECV_1_5);

    // Enabling of the Fast Discharge - this causes -> GSB: 0x08, SR5: 0x000080
//    L99dz200g.L99DZ200G_Set_ECV_FastDischargeControl(ENABLE);

    return L99DZ200G_OK;
}

#ifdef SHOW_ADC
/*
 * NAME:
 *  int8_t Cmd_adc(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "adc" command to show analog input value.
 *
 *  OUTn Current Monitor: OUT1, OUT2, OUT3, OUT6, OUT7, OUT8, OUT10, OUT15, OUTGH
 *  X, Y Position Voltage Monitor: X, Y
 *
 *  Two optional parameters supported.
 *   n = show OUTn current monitor
 *      -or-
 *   <x> = show X position voltage
 *      -or-
 *   <y> = show Y position voltage
 *      -and-
 *   <v> = show OUTn voltage
 *      -or-
 *   <r> = show OUTn raw ADC hex value
 *
 *       1  2 3
 *     "adc 2"    - show OUT2 current
 *     "adc 7"    - show OUT7 current
 *     "adc x"    - show X postion voltage
 *     "adc y"    - show Y postion voltage
 *     "adc 7 v"  - show OUT7 voltage
 *     "adc 7 r"  - show OUT7 raw ADC hex value
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  Temporarily enables L99DZ200G CM/DIR pin as CM output always for OUTn current monitoring,
 *  reads the CM analog input, and then restores the CM/DIR pin as DIR input always.
 */
int8_t Cmd_adc(int8_t argc, char * argv[])
{
#define RSENSE_VAL      0.4
#define CSENSE_GAIN     50
#if defined(TEENSYDUINO)
    const float bit_val = 3.3 / 1024;   // voltage per bit
#else
    const float bit_val = 5.0 / 1024;   // voltage per bit
#endif
    int32_t val;
    int8_t paramtype;
    uint8_t dec_digs = 0;
    uint8_t output = 1;
    char item = 'o';        // default is OUTn item
    char type = 'a';        // default is processed A/D value
    uint16_t adc_value = 0;
    uint32_t reg;
    bool low_rdson = true;
    float working;

    if (argc < 2)
    {
        Serial.println("OUT: 1, 2, 3,  6, 7, 8, 15,  X, Y  [v | r]");
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        if ((tolower(argv[ARG1][0]) == 'x') && (argv[ARG1][1] == '\0'))
        {
            item = 'x';
        }
        else if ((tolower(argv[ARG1][0]) == 'y') && (argv[ARG1][1] == '\0'))
        {
            item = 'y';
        }
        else
        {
            // get the OUTn number
            paramtype = CmdLine.ParseParam(argv[ARG1], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL))
            {
                return CMDLINE_INVALID_ARG;
            }
            switch (val)
            {
                case OUT_1:
                case OUT_2:
                case OUT_3:
                case OUT_6:
                case OUT_7:
                case OUT_8:
                case OUT_10:
                case OUT_15:
                    output = val;
                    break;                          // OK
                default:
                    return CMDLINE_INVALID_ARG;     // invalid
            }
        }

        if (argc > 2)
        {
            if (argv[ARG2][1] == '\0')
            {
                if (tolower(argv[ARG2][0]) == 'v')      // volts
                {
                    type = 'v';
                }
                else if (tolower(argv[ARG2][0]) == 'r') // raw hex
                {
                    type = 'r';
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }

        switch (item)
        {
            case 'x':               // X position
                adc_value = ReadDataVoltage_X();
                dec_digs = 3;
                break;
            case 'y':               // Y position
                adc_value = ReadDataVoltage_Y();
                dec_digs = 3;
                break;
            case 'o':               // OUTn item
                adc_value = ReadDataVoltage_OUTn();
                if (adc_value < 5)
                {
                    dec_digs = 1;
                }
                break;
        }

        switch (type)
        {
            case 'v':               // volts
                working = bit_val * adc_value;
                Serial.print(working, dec_digs);
                Serial.println(F(" V"));
                break;
            case 'r':               // raw hex (dec)
                Print0xHex12(adc_value);
                Serial.print(F(" ("));
                Serial.print(adc_value);
                Serial.println(F(")"));
                break;
            case 'a':               // processed A/D value
                switch (item)
                {
                    case 'x':               // X position
                    case 'y':               // Y position
                        working = bit_val * adc_value;
                        Serial.print(working, dec_digs);
                        Serial.println(F(" V"));
                        break;
                    case 'o':               // OUTn values
                    default:
                        // get RDSon register values
                        reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
                        switch (output)
                        {
                            case OUT_1:
                            case OUT_6:
                                if (reg & CR9_OUT1_6_RSDON_MASK)
                                {
                                    low_rdson = false;
                                }
                                break;
                            case OUT_7:
                                if (reg & CR9_OUT7_RSDON_MASK)
                                {
                                    low_rdson = false;
                                }
                                break;
                            case OUT_8:
                                if (reg & CR9_OUT8_RSDON_MASK)
                                {
                                    low_rdson = false;
                                }
                                break;
                        }

                        if (low_rdson)
                        {
                            switch (output)
                            {
                                case OUT_2:
                                case OUT_3:     // 1/2000
                                    working = bit_val * adc_value * 420;    // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_1:
                                case OUT_6:     // 1/10000
                                    working = bit_val * adc_value * 2620;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_7:     // 1/10000
                                    working = bit_val * adc_value * 2150;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_8:     // 1/6500
                                    working = bit_val * adc_value * 1360;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_10:
                                case OUT_15:    // 1/1000
                                    working = bit_val * adc_value * 210;    // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                            }
                        }
                        else    // high rdson
                        {
                            switch (output)
                            {
                                case OUT_1:
                                case OUT_6:     // 1/5000
                                    working = bit_val * adc_value * 1160;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_7:
                                case OUT_8:     // 1/2000
                                    working = bit_val * adc_value * 380;    // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                            }
                        }
                        Serial.println(F(" mA"));
                        break;
                }
                break;
        }
    }

    // Return success.
    return 0;
}

#define NUM_READINGS     50

// get averaged analog value for adc_input
uint16_t GetAnalogValue(uint8_t adc_input)
{
    uint32_t total = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    for (uint8_t i = 0; i < NUM_READINGS; ++i)
    {
        total += analogRead(adc_input);
    }

    return (uint16_t)(total / NUM_READINGS);    // average
}
#endif

#ifdef SHOW_CCM
/*
 * NAME:
 *  int8_t Cmd_ccm(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "ccm" command to show/set OUT7, OUT8, or OUT9 constant current mode control.
 *
 *  One optional parameter supported.
 *   <on>  = turn constant current mode on
 *      -or-
 *   <off> = turn constant current mode off
 *
 *       1  2  3
 *     "ccm 7"       - show OUT7 constant current mode on
 *     "ccm 8 on"    - turn OUT8 constant current mode on
 *     "ccm 9 off"   - turn OUT9 constant current mode off
 *
 *  Constant Current values:
 *    OUT7 - ~175 mA (80 to 250 mA)
 *    OUT8 - ~175 mA (100 to 250 mA)
 *    OUT9 - ~175 mA (100 to 250 mA)
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_ccm(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t en_dis;
    uint8_t output = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowConstantCurrentMode(OUT_7);
        ShowConstantCurrentMode(OUT_8);
        ShowConstantCurrentMode(OUT_9);
    }
    else
    {
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_7:
            case OUT_8:
            case OUT_9:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                en_dis = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                en_dis = ENABLE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_SetConstantCurrentModeControl(output, en_dis);
        }
        ShowConstantCurrentMode(output);
    }

    // Return success.
    return 0;
}

// get and show OUTn output constant current mode
void ShowConstantCurrentMode(uint8_t output)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn CCM register values
    switch (output)
    {
        case OUT_7:
        case OUT_8:
        case OUT_9:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
            break;
        default:
            return;     // invalid output
    }

    // get OUTn register constant current mode
    switch (output)
    {
        case OUT_7:
            reg &= CR9_OUT7_CCM_EN_MASK;
            break;
        case OUT_8:
            reg &= CR9_OUT8_CCM_EN_MASK;
            break;
        case OUT_9:
            reg &= CR9_OUT9_CCM_EN_MASK;
            break;
    }

    // show OUTn constant current mode
    Serial.print(F("OUT"));
    Serial.print(output);
    Serial.print(F(": "));
    ShowOnOff(reg);
}
#endif

/*
 * NAME:
 *  int8_t Cmd_dir(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "dir" command to set/show DIR output pin.
 *
 *  DIR-control: OUT7, OUT8, OUT9, OUT10, OUT13, OUT14, OUT15, Timer1, Timer2
 *
 *  One optional parameter supported.
 *   <lo> = set DIR output state low
 *      -or-
 *   <hi> = set DIR output state high
 *
 *       1   2
 *     "dir"      - show DIR output state
 *     "dir lo"   - set DIR output state low
 *     "dir hi"   - set DIR output state high
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  Assumes the L99DZ200G CM/DIR pin has been configured as a DIR input pin.
 */
int8_t Cmd_dir(int8_t argc, char * argv[])
{
    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("lo")) == 0)
        {
            digitalWrite(L99DZ200G_DIR_PIN, LOW);
        }
        else if (strcmp_P(argv[ARG1], PSTR("hi")) == 0)
        {
            digitalWrite(L99DZ200G_DIR_PIN, HIGH);
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }
    }

    // If watchdog is not running, assumes L99DZ200G is in a standby mode
    // and should not be disturbed by SPI communications!
    if (L99dz200g.L99DZ200G_WatchdogRunning())
    {
        Show_DIR_CM_DIR_States();
    }

    // Return success.
    return 0;
}

// get and show DIR output pin and CM_DIR pin states
void Show_DIR_CM_DIR_States(void)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR7 register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR7);
    reg &= CR7_CM_DIR_MASK;
    Serial.print(F("CM_DIR_CONF: "));
    switch (reg >> CR7_CM_DIR_POS)
    {
        case CM_ALWAYS:
            Serial.print(F("CM Always"));
            break;
        case DIR_STBY_CM_ACTIVE:
            Serial.print(F("DIR Standby, CM Active"));
            break;
        case DIR_ALWAYS:
            Serial.print(F("DIR Always"));
            break;
        case OFF_CM_DIR:
            Serial.print(F("CM_DIR Off"));
            break;
    }
    Serial.println();

    // show DIR output pin state
    Serial.print(F("DIR: "));
    ShowHighLow(digitalRead(L99DZ200G_DIR_PIN));
}

#ifdef SHOW_ECV
/*
 * NAME:
 *  int8_t Cmd_ecv(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "ecv" command to show/set electrochromic mirror output control.
 *
 *  Five optional parameters supported (one at a time).
 *   <econ>  = turn ECV output on
 *      -or-
 *   <ecoff> = turn ECV output off
 *      -or-
 *   <vm12>  = set ECV max volts output to 1.2 V
 *      -or-
 *   <vm15>  = set ECV max volts output to 1.5 V
 *      -or-
 *   <ocon>  = set ECV OCR (Overcurrent Recovery) on
 *      -or-
 *   <ocoff> = set ECV OCR (Overcurrent Recovery) off
 *      -or-
 *   <lson>  = turn ECV LS (low-side) switch on
 *      -or-
 *   <lsoff> = turn ECV LS (low-side) switch off
 *      -or-
 *   volts   = set ECV output to volts value (0.00 to 1.50 V)
 *
 *       1   2
 *     "ecv econ"       - turn ECV output on
 *     "ecv ecoff"      - turn ECV output off
 *     "ecv vm12"       - set ECV max output at 1.2 V
 *     "ecv vm15"       - set ECV max output at 1.5 V
 *     "ecv lson"       - turn ECV low-side switch on
 *     "ecv lsoff"      - turn ECV low-side switch off
 *     "ecv ocon"       - set ECV Overcurrent Recovery on
 *     "ecv ocoff"      - set ECV Overcurrent Recovery off
 *     "ecv 1.15"       - set ECV output at 1.15 V output
 *     "ecv 0.25"       - set ECV output at 0.25 V output
  *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_ecv(int8_t argc, char * argv[])
{
    uint8_t len;
    float drive_volts;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("ecoff")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_Control(OFF_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("econ")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_Control(ON_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("lsoff")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_FastDischargeControl(OFF_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("lson")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_FastDischargeControl(ON_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("ocoff")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_OvercurrentRecoveryControl(OFF_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("ocon")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_OvercurrentRecoveryControl(ON_OUT);
        }
        else if (strcmp_P(argv[ARG1], PSTR("vm12")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_MaxVoltage(ECV_1_2);
        }
        else if (strcmp_P(argv[ARG1], PSTR("vm15")) == 0)
        {
            L99dz200g.L99DZ200G_Set_ECV_MaxVoltage(ECV_1_5);
        }
        else
        {
            // get the voltage value (n.nn)
            len = strnlen(argv[ARG1], 5);
            if ((len > 4) || ((argv[ARG1][0] != '0') && (argv[ARG1][0] != '1')))
            {
                return CMDLINE_INVALID_ARG;
            }
            else if ((len > 1) && (argv[ARG1][1] != '.'))
            {
                return CMDLINE_INVALID_ARG;
            }
            else if ((len > 2) && (!isdigit(argv[ARG1][2])))
            {
                return CMDLINE_INVALID_ARG;
            }
            else if ((len > 3) && (!isdigit(argv[ARG1][3])))
            {
                return CMDLINE_INVALID_ARG;
            }

            drive_volts = atof(argv[ARG1]);
            if (drive_volts > ECV_1_5_VOLTS)
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_Set_ECV_DriveVoltage(drive_volts);
        }
    }

    ShowECV_Settings();

    // Return success.
    return 0;
}

// get and show ECV settings - CR11, CFR
void ShowECV_Settings(void)
{
    float drive_volts;
    float vctrl_max;
    uint32_t reg;
    uint8_t ecv_ls;
    uint8_t ecv_ocr;
    uint8_t ecv_econ;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // read CFR
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
    if (reg & CFR_ECV_HV_MASK)
    {
        vctrl_max = ECV_1_5_VOLTS;
    }
    else
    {
        vctrl_max = ECV_1_2_VOLTS;
    }

    drive_volts =  L99dz200g.L99DZ200G_Get_ECV_DriveVoltage();

    // read CR11
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR11);

    ecv_ls = (reg & CR11_ECV_LS_MASK) >> CR11_ECV_LS_POS;
    ecv_ocr = (reg & CR11_ECV_OCR_MASK) >> CR11_ECV_OCR_POS;
    ecv_econ = (reg & CR11_EC_ON_MASK) >> CR11_EC_ON_POS;

    // show ECV settings
    Serial.print(F("ECV VMAX: "));
    Serial.print(vctrl_max, 1);
    Serial.println(F(" V"));
    Serial.print(F("ECV LS: "));
    ShowOnOff(ecv_ls);
    Serial.print(F("ECV OCR: "));
    ShowOnOff(ecv_ocr);
    Serial.print(F("ECV ECON: "));
    ShowOnOff(ecv_econ);
    Serial.print(F("ECV Volts: "));
    Serial.print(drive_volts);
    Serial.println(F(" V"));
}
#endif

#ifdef SHOW_HEATER
/*
 * NAME:
 *  int8_t Cmd_ghol(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "ghol" command to show/set Heater open-load diagnosis control.
 *
 *  One optional parameter supported.
 *   <off> = set Heater open-load diagnosis off
 *      -or-
 *   <on> = set Heater open-load diagnosis on
 *
 *        1  2
 *     "ghol"       - show Heater open-load diagnosis enable
 *     "ghol off"   - set Heater open-load diagnosis off
 *     "ghol on"    - set Heater open-load diagnosis on
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_ghol(int8_t argc, char * argv[])
{
    uint8_t on_off;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("off")) == 0)
        {
            on_off = OL_DIAG_OFF;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            on_off = OL_DIAG_ON;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_HeaterOpenLoadDiagnosisControl(on_off);
    }

    ShowHeaterOpenLoadDiagnosisControl();

    // Return success.
    return 0;
}

// get and show Heater open-load diagnosis enable
void ShowHeaterOpenLoadDiagnosisControl(void)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR11 register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR11);
    reg &= CR11_GHOL_EN_MASK;

    // show Heater open-load diagnosis enable
    Serial.print(F("GH_OL_EN: "));
    ShowOnOff(reg);
}

/*
 * NAME:
 *  int8_t Cmd_ght(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "ght" command to show/set Heater Drain monitor threshold settings.
 *
 *  One optional parameter supported.
 *   <threshold> = the Heater Drain monitor threshold setting to use
 *                 (200 mV to 550 mV in 50 mV increments)
 *
 *       1   2
 *     "ght"        - show Heater Drain monitor threshold setting
 *     "ght 200     - set Heater Drain monitor threshold setting to 200 mV
 *     "ght 500     - set Heater Drain monitor threshold setting to 500 mV
 *                    (GH_TH_200_MV, GH_TH_250_MV, GH_TH_300_MV, GH_TH_350_MV,
 *                     GH_TH_400_MV, GH_TH_450_MV, GH_TH_500_MV, GH_TH_550_MV)
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_ght(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint16_t gh_thrshld;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        // get the Heater Drain monitor threshold
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }

        switch (val )
        {
            case GH_200_MV_VAL:
                gh_thrshld = GH_TH_200_MV;
                break;
            case GH_250_MV_VAL:
                gh_thrshld = GH_TH_250_MV;
                break;
            case GH_300_MV_VAL:
                gh_thrshld = GH_TH_300_MV;
                break;
            case GH_350_MV_VAL:
                gh_thrshld = GH_TH_350_MV;
                break;
            case GH_400_MV_VAL:
                gh_thrshld = GH_TH_400_MV;
                break;
            case GH_450_MV_VAL:
                gh_thrshld = GH_TH_450_MV;
                break;
            case GH_500_MV_VAL:
                gh_thrshld = GH_TH_500_MV;
                break;
            case GH_550_MV_VAL:
                gh_thrshld = GH_TH_550_MV;
                break;
            default:
                return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_SetHeaterMonitorThresholdVoltage(gh_thrshld);
    }

    ShowHeaterMonitorThresholdVoltage();
    if (argc < 2)
    {
        Serial.println(F("Thresholds: 200, 250, 300, 350, 400, 450, 500, 550 mV"));
    }

    // Return success.
    return 0;
}

// get and show Heater monitor threshold voltage
void ShowHeaterMonitorThresholdVoltage(void)
{
    uint32_t reg;
    uint16_t threshld;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR11 register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR11);
    reg &= CR11_GH_TH_MASK;
    reg >>= CR11_GH_TH_POS;

    // show Heater monitor threshold voltage
    Serial.print(F("GH_TH: "));
    switch (reg)
    {
        case GH_TH_200_MV:
            threshld = GH_200_MV_VAL;
            break;
        case GH_TH_250_MV:
            threshld = GH_250_MV_VAL;
            break;
        case GH_TH_300_MV:
            threshld = GH_300_MV_VAL;
            break;
        case GH_TH_350_MV:
            threshld = GH_350_MV_VAL;
            break;
        case GH_TH_400_MV:
            threshld = GH_400_MV_VAL;
            break;
        case GH_TH_450_MV:
            threshld = GH_450_MV_VAL;
            break;
        case GH_TH_500_MV:
            threshld = GH_500_MV_VAL;
            break;
        case GH_TH_550_MV:
            threshld = GH_550_MV_VAL;
            break;
    }
    Serial.print(threshld);
    Serial.println(F(" mV"));
}
#endif

/*
 * NAME:
 *  int8_t Cmd_gsb(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "gsb" command to show L99DZ200G Global Status Byte.
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_gsb(int8_t argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    uint8_t gsb;

    gsb = L99dz200g.L99DZ200G_GlobalStatusByte();
    Serial.print(F("GSB: "));
    Print0xHexByteln(gsb);
    ShowGlobalStatusByte(gsb);

    // Return success.
    return 0;
}

// show L99DZ200G Global Status Byte
void ShowGlobalStatusByte(uint8_t gsb)
{
    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    if (gsb & GSB_GSBN_MASK)     // Global Status Inverted status
    {
        return;
    }
    else
    {
        Serial.print(F(" -"));
    }
    if (gsb & GSB_RSTB_MASK)     // Reset status - SR1
    {
        Serial.print(F(" RSTB"));
    }
    if (gsb & GSB_SPIE_MASK)     // SPI Error status - SR2
    {
        Serial.print(F(" SPIE"));
    }
    if (gsb & GSB_PLE_MASK)      // Physical Layer Error status - SR2, SR12
    {
        Serial.print(F(" PLE"));
    }
    if (gsb & GSB_FE_MASK)       // Functional Error status - SR2, SR3, SR4, SR5
    {
        Serial.print(F(" FE"));
    }
    if (gsb & GSB_DE_MASK)       // Device Error status - SR2, SR6
    {
        Serial.print(F(" DE"));
    }
    if (gsb & GSB_GW_MASK)       // Global Warning status - SR2
    {
        Serial.print(F(" GW"));
    }
    if (gsb & GSB_FS_MASK)       // Fail Safe status - SR1
    {
        Serial.print(F(" FS"));
    }
    if (gsb == 0)               // unknown error
    {
        Serial.print(F(" Unknown Error"));
    }
    Serial.println();
}

#ifdef SHOW_MOTOR
/*
 * NAME:
 *  int8_t Cmd_motor(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "motor" command to show/set OUT1, OUT2, OUT3, and OUT6 motors output control.
 *  This is based on the vehicle mirror controller operation.
 *
 *  Two required parameters supported.
 *   <X> = select X (OUT1 <-> OUT2) motor to control (mirror X movement)
 *      -or-
 *   <Y> = select Y (OUT1 <-> OUT3) motor to control (mirror X movement)
 *      -or-
 *   <F> = select Fold (OUT1 <-> OUT6) motor to control (mirror folding movement)
 *      -or-
 *   <XY> = select X (OUT1 <-> OUT2) and Y (OUT1 <-> OUT3) motors to control
 *      -or-
 *   <XF> = select X (OUT1 <-> OUT2) and Fold (OUT1 <-> OUT6) motors to control
 *      -or-
 *   <YF> = select Y (OUT1 <-> OUT2) and Fold (OUT1 <-> OUT6) motors to control
 *      -or-
 *   <B> = select motors brake (coast)
 *
 *      -and-
 *
 *   <L> = select motor direction left
 *      -or-
 *   <R> = select motor direction right
 *      -or-
 *   <B> = select motors brake (coast)
 *      -or-
 *   <S> = select motors stop (same as brake)
 *
 *       1    2  3
 *     "motor x"     - show X motor state (OUT1 <-> OUT3)
 *     "motor y l"   - set X motor direction left (OUT1 <-> OUT2)
 *     "motor y r"   - set Y motor direction right (OUT1 <-> OUT3)
 *     "motor f r"   - set Fold motor direction right (OUT1 <-> OUT6)
 *     "motor xy r"  - set X motor and Y motor direction right (OUT1 <-> OUT2) (OUT1 <-> OUT3)
 *     "motor xf r"  - set X motor and Fold motor direction right (OUT1 <-> OUT2) (OUT1 <-> OUT6)
 *     "motor yf r"  - set Y motor and Fold motor direction right (OUT1 <-> OUT3) (OUT1 <-> OUT6)
 *     "motor b"     - set all motors brake
 *     "motor y b"   - set all motors brake (any motor can be selected)
 *     "motor s"     - set all motors brake
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_motor(int8_t argc, char * argv[])
{
    uint8_t outputs = 99;
    uint8_t mot_dir = 99;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowMotorState(OUT2_X);
        ShowMotorState(OUT3_Y);
        ShowMotorState(OUT6_F);
    }
    else
    {
        // get the OUTn item
        if (tolower(argv[ARG1][0]) == 'x')          // 'x' or "xy" or "xf"
        {
            if (argv[ARG1][1] != '\0')
            {
                if (argv[ARG1][2] == '\0')
                {
                    switch (tolower(argv[ARG1][1]))
                    {
                        case 'y':
                            outputs = OUT_XY;
                            break;
                        case 'f':
                            outputs = OUT_XF;
                            break;
                        default:
                            return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else
            {
                outputs = OUT2_X;
            }
        }
        else if (tolower(argv[ARG1][0]) == 'y')     // 'y' or "yf"
        {
            if (argv[ARG1][1] != '\0')
            {
                if (argv[ARG1][2] == '\0')
                {
                    switch (tolower(argv[ARG1][1]))
                    {
                        case 'f':
                            outputs = OUT_YF;
                            break;
                        default:
                            return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else
            {
                outputs = OUT3_Y;
            }
        }
        else if (tolower(argv[ARG1][0]) == 'f')
        {
            if (argv[ARG1][1] != '\0')
            {
                return CMDLINE_INVALID_ARG;
            }
            else
            {
                outputs = OUT6_F;
            }
        }
        else if ((tolower(argv[ARG1][0]) == 'b') || // brake
                 (tolower(argv[ARG1][0]) == 's'))   // stop
        {
            if (argv[ARG1][1] != '\0')
            {
                return CMDLINE_INVALID_ARG;
            }
            else
            {
                mot_dir = BRAKE;
            }
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc == 3)
        {
            if (mot_dir == BRAKE)
            {
                return CMDLINE_TOO_MANY_ARGS;
            }
            if (argv[ARG2][1] != '\0')
            {
                return CMDLINE_INVALID_ARG;
            }

            // get the motor direction
            switch (tolower(argv[ARG2][0]))
            {
                case 'l':
                    mot_dir = LEFT_DIRECTION;
                    break;
                case 'r':
                    mot_dir = RIGHT_DIRECTION;
                    break;
                case 'b':
                case 's':
                    mot_dir = BRAKE;
                    break;

                default:
                    return CMDLINE_INVALID_ARG;     // invalid direction
            }

            L99dz200g.L99DZ200G_MotorDriver(outputs, mot_dir);
        }
        else if (mot_dir == BRAKE)
        {
            L99dz200g.L99DZ200G_MotorDriver(outputs, mot_dir);
        }

        ShowMotorState(outputs);
    }

    // Return success.
    return 0;
}

// get and show OUTn motor control states
void ShowMotorState(uint8_t outputs)
{
    ShowReg(L99DZ200G_CR4);

    switch (outputs)
    {
        case OUT2_X:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_2);
            break;
        case OUT3_Y:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_3);
            break;
        case OUT6_F:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_6);
            break;
        case OUT_XY:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_2);
            ShowOutputStateM(OUT_3);
           break;
        case OUT_XF:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_2);
            ShowOutputStateM(OUT_6);
            break;
        case OUT_YF:
            ShowOutputStateM(OUT_1);
            ShowOutputStateM(OUT_3);
            ShowOutputStateM(OUT_6);
            break;

        default:
            return;     // invalid output
    }
}
#endif

#ifdef SHOW_OCR
/*
 * NAME:
 *  int8_t Cmd_ocr(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "ocr" command to show/set OUT1, OUT2, OUT3, OUT6, OUT7, OUT8, or OUT15
 *  overcurrent recovery control.
 *
 *  Four optional parameters supported (one at a time).
 *   <ocoff> = set overcurrent recovery off
 *      -or-
 *   <ocon>  = set overcurrent recovery on
 *      -or-
 *   <thxoff> = set overcurrent recovery thermal expiration off
 *      -or-
 *   <thxon> = set overcurrent recovery thermal expiration on
 *      -or-
 *    freq   = set overcurrent recovery programmable frequency - Khz (1.7k 2.2k 3.0k 4.4k)
 *      -or-
 *    time = set overcurrent recovery programmable on time (64us 72us 80us 88us)
 *
 *       1  2  3
 *     "ocr 8"         - show OUT8 overcurrent recovery settings
 *     "ocr 1 ocoff"   - set OUT1 overcurrent recovery off
 *     "ocr 2 ocon"    - set OUT2 overcurrent recovery on
 *     "ocr 3 thxoff"  - set OUT3 overcurrent recovery thermal expiration off
 *     "ocr 6 thxon"   - set OUT6 overcurrent recovery thermal expiration on
 *     "ocr 7 1.7k"    - set OUT7 overcurrent recovery programmable frequency 1.7 Khz
 *     "ocr 15 80us"   - set OUT15 overcurrent recovery programmable on time 80 uS
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_ocr(int8_t argc, char * argv[])
{
#define OCR_EN_DIS      0
#define OCR_THX         1
#define OCR_FREQ        2
#define OCR_TON         3
    int32_t val;
    int8_t paramtype;
    uint8_t output = 0;
    uint8_t alt_output = 0;
    uint8_t ctrl = 0;
    uint8_t en_dis = 0;
    uint8_t freq = 0;
    uint8_t ton = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowOvercurrentRecoverySetting(OUT_1);
        ShowOvercurrentRecoverySetting(OUT_2);
        ShowOvercurrentRecoverySetting(OUT_3);
        ShowOvercurrentRecoverySetting(OUT_6);
        ShowOvercurrentRecoverySetting(OUT_7);
        ShowOvercurrentRecoverySetting(OUT_8);
        ShowOvercurrentRecoverySetting(OUT_15);
    }
    else
    {
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_1:
            case OUT_2:
            case OUT_3:
            case OUT_6:
                output = val;                   // OK
                alt_output = OUT_1_2_3_6;       // freq and time are grouped together on these OUTs
                break;
            case OUT_7:
            case OUT_8:
            case OUT_15:
                output = val;                   // OK
                alt_output = output;
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("ocoff")) == 0)
            {
                ctrl = OCR_EN_DIS;
                en_dis = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("ocon")) == 0)
            {
                ctrl = OCR_EN_DIS;
                en_dis = ENABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("thxoff")) == 0)
            {
                ctrl = OCR_THX;
                en_dis = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("thxon")) == 0)
            {
                ctrl = OCR_THX;
                en_dis = ENABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("1.7k")) == 0)
            {
                ctrl = OCR_FREQ;
                freq = OCR_FREQ_1_7KHZ;
            }
            else if (strcmp_P(argv[ARG2], PSTR("2.2k")) == 0)
            {
                ctrl = OCR_FREQ;
                freq = OCR_FREQ_2_2KHZ;
            }
            else if (strcmp_P(argv[ARG2], PSTR("3.0k")) == 0)
            {
                ctrl = OCR_FREQ;
                freq = OCR_FREQ_3_0KHZ;
            }
            else if (strcmp_P(argv[ARG2], PSTR("4.4k")) == 0)
            {
                ctrl = OCR_FREQ;
                freq = OCR_FREQ_4_4KHZ;
            }
            else if (strcmp_P(argv[ARG2], PSTR("64us")) == 0)
            {
                ctrl = OCR_TON;
                ton = OCR_TON_64US;
            }
            else if (strcmp_P(argv[ARG2], PSTR("72us")) == 0)
            {
                ctrl = OCR_TON;
                ton = OCR_TON_72US;
            }
            else if (strcmp_P(argv[ARG2], PSTR("80us")) == 0)
            {
                ctrl = OCR_TON;
                ton = OCR_TON_80US;
            }
            else if (strcmp_P(argv[ARG2], PSTR("88us")) == 0)
            {
                ctrl = OCR_TON;
                ton = OCR_TON_88US;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            switch (ctrl)
            {
                case OCR_EN_DIS:
                    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryControl(output, en_dis);
                    break;
                case OCR_THX:
                    L99dz200g.L99DZ200G_Set_OCR_ThermalExpirationControl(output, en_dis);
                    break;
                case OCR_FREQ:
                    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryFrequency(alt_output, freq);
                    break;
                case OCR_TON:
                    L99dz200g.L99DZ200G_Set_OCR_AutorecoveryTime(alt_output, ton);
                    break;
            }
        }
        ShowOvercurrentRecoverySetting(output);
    }

    // Return success.
    return 0;
}

// get and show OUTn output overcurrent recovery settings
void ShowOvercurrentRecoverySetting(uint8_t output)
{
    uint32_t reg7;
    uint32_t reg8;
    uint8_t ocr_en = 0;
    uint8_t ocr_thx_en = 0;
    uint8_t ocr_ton = 0;
    uint8_t ocr_freq = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn OCR registers values
    switch (output)
    {
            case OUT_1:
            case OUT_2:
            case OUT_3:
            case OUT_6:
            case OUT_7:
            case OUT_8:
            case OUT_15:
            reg7 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR7);
            reg8 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR8);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register overcurrent recovery enable
    // get OUTn register overcurrent recovery thermal expiration
    // get OUTn register overcurrent recovery programmable frequency
    // get OUTn register overcurrent recovery programmable on time
    switch (output)
    {
        case OUT_1:
            ocr_en = (reg7 & CR7_OUT1_OCR_MASK) >> CR7_OUT1_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT1_THX_EN_MASK) >> CR8_OUT1_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT1_2_3_6_FREQ_MASK) >> CR8_OUT1_2_3_6_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT1_2_3_6_TIME_MASK) >> CR8_OUT1_2_3_6_TIME_POS;
            break;
        case OUT_2:
            ocr_en = (reg7 & CR7_OUT2_OCR_MASK) >> CR7_OUT2_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT2_THX_EN_MASK) >> CR8_OUT2_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT1_2_3_6_FREQ_MASK) >> CR8_OUT1_2_3_6_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT1_2_3_6_TIME_MASK) >> CR8_OUT1_2_3_6_TIME_POS;
            break;
        case OUT_3:
            ocr_en = (reg7 & CR7_OUT3_OCR_MASK) >> CR7_OUT3_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT3_THX_EN_MASK) >> CR8_OUT3_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT1_2_3_6_FREQ_MASK) >> CR8_OUT1_2_3_6_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT1_2_3_6_TIME_MASK) >> CR8_OUT1_2_3_6_TIME_POS;
            break;
        case OUT_6:
            ocr_en = (reg7 & CR7_OUT6_OCR_MASK) >> CR7_OUT6_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT6_THX_EN_MASK) >> CR8_OUT6_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT1_2_3_6_FREQ_MASK) >> CR8_OUT1_2_3_6_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT1_2_3_6_TIME_MASK) >> CR8_OUT1_2_3_6_TIME_POS;
            break;
        case OUT_7:
            ocr_en = (reg7 & CR7_OUT7_OCR_MASK) >> CR7_OUT7_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT7_THX_EN_MASK) >> CR8_OUT7_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT7_FREQ_MASK) >> CR8_OUT7_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT7_TIME_MASK) >> CR8_OUT7_TIME_POS;
            break;
        case OUT_8:
            ocr_en = (reg7 & CR7_OUT8_OCR_MASK) >> CR7_OUT8_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT8_THX_EN_MASK) >> CR8_OUT8_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT8_FREQ_MASK) >> CR8_OUT8_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT8_TIME_MASK) >> CR8_OUT8_TIME_POS;
            break;
        case OUT_15:
            ocr_en = (reg7 & CR7_OUT15_OCR_MASK) >> CR7_OUT15_OCR_POS;
            ocr_thx_en = (reg8 & CR8_OUT15_THX_EN_MASK) >> CR8_OUT15_THX_EN_POS;
            ocr_freq = (reg8 & CR8_OUT15_FREQ_MASK) >> CR8_OUT15_FREQ_POS;
            ocr_ton = (reg8 & CR8_OUT15_TIME_MASK) >> CR8_OUT15_TIME_POS;
            break;
    }

    // show OUTn OCR settings
    Serial.print(F("OUT"));
    Serial.print(output);
    Serial.print(F(": "));
    if (output != OUT_15)
    {
        Serial.print(F(" "));
    }

    Serial.print(F("OCR: "));
    if (ocr_en)
    {
        Serial.print("On, ");
    }
    else
    {
        Serial.print("Off,");
    }

    Serial.print(F(" THX_EN: "));
    if (ocr_thx_en)
    {
        Serial.print("On, ");
    }
    else
    {
        Serial.print("Off,");
    }

    Serial.print(F(" Freq: "));
    switch (ocr_freq)
    {
        case OCR_FREQ_1_7KHZ:
            Serial.print("1.7 Khz");
            break;
        case OCR_FREQ_2_2KHZ:
            Serial.print("2.2 Khz");
            break;
        case OCR_FREQ_3_0KHZ:
            Serial.print("3.0 Khz");
            break;
        case OCR_FREQ_4_4KHZ:
            Serial.print("4.4 Khz");
            break;
    }

    Serial.print(F(", Time-on: "));
    switch (ocr_ton)
    {
        case OCR_TON_64US:
            Serial.print("64 uS");
            break;
        case OCR_TON_72US:
            Serial.print("72 uS");
            break;
        case OCR_TON_80US:
            Serial.print("80 uS");
            break;
        case OCR_TON_88US:
            Serial.print("88 uS");
            break;
    }
    Serial.println();
}
#endif

#ifdef SHOW_OCT
/*
 * NAME:
 *  int8_t Cmd_oct(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "oct" command to show/set OUT9, OUT10, OUT13, OUT14, or OUT15 overcurrent
 *  threshold control.
 *
 *  One optional parameter supported.
 *   <lo>  = set overcurrent threshold low current mode
 *      -or-
 *   <hi> = set overcurrent threshold high current mode
 *
 *       1  2  3
 *     "oct 9"        - show OUT9 overcurrent threshold setting
 *     "oct 10 hi"    - set OUT10 high current mode
 *     "oct 10 lo"    - set OUT10 low current mode
 *     "oct 13 hi"    - set OUT13 high current mode
 *     "oct 15 lo"    - set OUT15 low current mode
 *
 *  Overcurrent threshold settings:
 *    OUT9, OUT13, OUT14 low current mode = 0.15 to 0.3 mA
 *    OUT9, OUT13, OUT14 high current mode = 0.35 to  0.7 mA
 *
 *    OUT10 low current mode = 0.25 to 0.5 mA
 *    OUT10 high current mode = 0.5 to 1 mA
 *
 *    OUT15 low current mode = 0.15 to 0.3 mA
 *    OUT15 high current mode = 0.25 to 0.5 mA
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_oct(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t oc_thresh;
    uint8_t output = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowOvercurrentThresholdSetting(OUT_9);
        ShowOvercurrentThresholdSetting(OUT_10);
        ShowOvercurrentThresholdSetting(OUT_13);
        ShowOvercurrentThresholdSetting(OUT_14);
        ShowOvercurrentThresholdSetting(OUT_15);
    }
    else
    {
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_9:
            case OUT_10:
            case OUT_13:
            case OUT_14:
            case OUT_15:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("lo")) == 0)
            {
                oc_thresh = LO_CURRENT_MODE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("hi")) == 0)
            {
                oc_thresh = HI_CURRENT_MODE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_OvercurrentThresholdControl(output, oc_thresh);
        }
        ShowOvercurrentThresholdSetting(output);
    }

    // Return success.
    return 0;
}

// get and show OUTn output overcurrent threshold mode
void ShowOvercurrentThresholdSetting(uint8_t output)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn OCT register values
    switch (output)
    {
            case OUT_9:
            case OUT_10:
            case OUT_13:
            case OUT_14:
            case OUT_15:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register overcurrent threshold
    switch (output)
    {
        case OUT_9:
            reg &= CR9_OUT9_OC_MASK;
            break;
        case OUT_10:
            reg &= CR9_OUT10_OC_MASK;
            break;
        case OUT_13:
            reg &= CR9_OUT13_OC_MASK;
            break;
        case OUT_14:
            reg &= CR9_OUT14_OC_MASK;
            break;
        case OUT_15:
            reg &= CR9_OUT15_OC_MASK;
            break;
    }

    // show OUTn overcurrent threshold
    Serial.print(F("OUT"));
    if (output == OUT_9)
    {
        Serial.print(' ');
    }
    Serial.print(output);
    Serial.print(F(": "));
    ShowEnabledDisabledRev(reg, true);        // Note: is backward from usual low/high
}
#endif

#ifdef SHOW_OLT
/*
 * NAME:
 *  int8_t Cmd_olt(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "olt" command to show/set OUT9, OUT10, OUT13, OUT14, or OUT15 open-load
 *  threshold control.
 *
 *  One optional parameter supported.
 *   <lo>  = set open-load threshold low current mode
 *      -or-
 *   <hi> = set open-load threshold high current mode
 *
 *       1  2  3
 *     "olt 9"        - show OUT9 open-load threshold setting
 *     "olt 10 hi"    - set OUT10 high current mode
 *     "olt 10 lo"    - set OUT10 low current mode
 *     "olt 13 hi"    - set OUT13 high current mode
 *     "olt 15 lo"    - set OUT15 low current mode
 *
 *  Open-load threshold settings: (under-current threshold)
 *    OUT9, OUT13, OUT14 low current mode = 0.25 to 4 mA
 *    OUT9, OUT13, OUT14 high current mode = 6 to 12 mA
 *
 *    OUT10 low current mode = 0.8 to 4 mA
 *    OUT10 high current mode = 10 to 30 mA
 *
 *    OUT15 low current mode = 0.25 to 4 mA
 *    OUT15 high current mode = 6 to 12 mA
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_olt(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t ol_thresh;
    uint8_t output = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowOpenLoadThresholdSetting(OUT_9);
        ShowOpenLoadThresholdSetting(OUT_10);
        ShowOpenLoadThresholdSetting(OUT_13);
        ShowOpenLoadThresholdSetting(OUT_14);
        ShowOpenLoadThresholdSetting(OUT_15);
    }
    else
    {
DEBUG_TOGL();
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_9:
            case OUT_10:
            case OUT_13:
            case OUT_14:
            case OUT_15:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("lo")) == 0)
            {
                ol_thresh = LO_CURRENT_MODE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("hi")) == 0)
            {
                ol_thresh = HI_CURRENT_MODE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_OpenLoadThresholdControl(output, ol_thresh);
        }
        ShowOpenLoadThresholdSetting(output);
DEBUG_TOGL();
    }

    // Return success.
    return 0;
}

// get and show OUTn output open-load threshold mode
void ShowOpenLoadThresholdSetting(uint8_t output)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn open-load threshold register values
    switch (output)
    {
            case OUT_9:
            case OUT_10:
            case OUT_13:
            case OUT_14:
            case OUT_15:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register open-load threshold
    switch (output)
    {
        case OUT_9:
            reg &= CR9_OUT9_OL_MASK;
            break;
        case OUT_10:
            reg &= CR9_OUT10_OL_MASK;
            break;
        case OUT_13:
            reg &= CR9_OUT13_OL_MASK;
            break;
        case OUT_14:
            reg &= CR9_OUT14_OL_MASK;
            break;
        case OUT_15:
            reg &= CR9_OUT15_OL_MASK;
            break;
    }

    // show OUTn open-load threshold
    Serial.print(F("OUT"));
    if (output == OUT_9)
    {
        Serial.print(' ');
    }
    Serial.print(output);
    Serial.print(F(": "));
    ShowEnabledDisabledRev(reg, true);        // Note: is backward from usual low/high
}
#endif

/*
 * NAME:
 *  int8_t Cmd_out(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "out" command to show/set OUT7, OUT8, OUT10, or OUT15 output control.
 *
 *  One optional parameter supported.
 *   <off> = set OUTn output state off
 *      -or-
 *   <on> = set OUTn output state on
 *      -or-
 *   <t1> to <t2> = set to OUTn output state T1 or T2 controlled
 *      -or-
 *   <pwm1> to <pwm7> = set to OUTn output state PWM1, PWM2, PWM3, PWM4, PWM5, PWM6 or PWM7 controlled
 *      -or-
 *   <dir> = set to OUTn output state DIR-controlled
 *      -or-
 *   <hb> = set to OUTn with heartbeat
 *
 *        1 2  3
 *     "out 10"      - show OUT10 output state
 *     "out 7 off"   - set OUT7 output state off
 *     "out 7 on"    - set OUT7 output state on
 *     "out 8 t1"    - set OUT8 output state T1-controlled
 *     "out 8 t2"    - set OUT8 output state T2-controlled
 *     "out 7 pwm1"  - set OUT7 output state PWM1-controlled
 *     "out 7 pwm2"  - set OUT7 output state PWM2-controlled
 *     "out 8 pwm3"  - set OUT8 output state PWM3-controlled
 *     "out 8 pwm4"  - set OUT8 output state PWM4-controlled
 *     "out 7 pwm5"  - set OUT7 output state PWM5-controlled
 *     "out 7 pwm6"  - set OUT7 output state PWM6-controlled
 *     "out 7 pwm7"  - set OUT7 output state PWM7-controlled
 *     "out 7 dir"   - set OUT7 output state DIR-controlled
 *     "out 7 hb"    - set OUT7 use with heartbeat
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_out(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t output_type = OFF_OUT;
    uint8_t output = OUT_10;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowOutputState(OUT_7);
        ShowOutputState(OUT_8);
        ShowOutputState(OUT_10);
        ShowOutputState(OUT_15);
        Serial.println("type: off, on, t1, t2, pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, dir, hb");
    }
    else
    {
DEBUG_TOGL();
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_7:
            case OUT_8:
            case OUT_10:
            case OUT_15:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            // get the OUTn output type
            output_type = GetOutputType(argv);
            if ((int8_t)output_type == CMDLINE_INVALID_ARG)
            {
                return CMDLINE_INVALID_ARG;
            }
            else if ((int8_t)output_type == HB_OUT)
            {
                OutHB = output;
            }
            else
            {
                if (OutHB == output)
                {
                    OutHB = 0;
                }
                L99dz200g.L99DZ200G_HSOutputsControl(output_type, output);
            }
        }
        ShowOutputState(output);
 DEBUG_TOGL();
   }

    // Return success.
    return 0;
}

// get OUTn output pin output type
uint8_t GetOutputType(char * argv[])
{
    uint8_t output_type = OFF_OUT;

    if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
    {
        output_type = OFF_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
    {
        output_type = ON_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("t1")) == 0)
    {
        output_type = TIMER1_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("t2")) == 0)
    {
        output_type = TIMER2_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm1")) == 0)
    {
        output_type = PWM1_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm2")) == 0)
    {
        output_type = PWM2_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm3")) == 0)
    {
        output_type = PWM3_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm4")) == 0)
    {
        output_type = PWM4_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm5")) == 0)
    {
        output_type = PWM5_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm6")) == 0)
    {
        output_type = PWM6_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("pwm7")) == 0)
    {
        output_type = PWM7_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("dir")) == 0)
    {
        output_type = DIR_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("hb")) == 0)
    {
        output_type = HB_OUT;
    }
    else
    {
        return CMDLINE_INVALID_ARG;
    }

    return output_type;
}

// get and show OUTn output pin output state
void ShowOutputState(uint8_t output)
{
    uint32_t reg;
    uint8_t output_state = OFF_OUT;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn register values
    switch (output)
    {
        case OUT_7:
        case OUT_8:
        case OUT_10:
        case OUT_GH:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR5);
            break;
        case OUT_15:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR6);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register states
    switch (output)
    {
        case OUT_7:
            reg &= CR5_HS_OUT7_MASK;
            output_state = reg >> CR5_HS_OUT7_POS;
            break;
        case OUT_8:
            reg &= CR5_HS_OUT8_MASK;
            output_state = reg >> CR5_HS_OUT8_POS;
            break;
        case OUT_10:
            reg &= CR5_HS_OUT10_MASK;
            output_state = reg >> CR5_HS_OUT10_POS;
            break;
        case OUT_15:
            reg &= CR6_HS_OUT15_MASK;
            output_state = reg >> CR6_HS_OUT15_POS;
            break;
        case OUT_GH:
            reg &= CR5_GH_MASK;
            output_state = reg >> CR5_GH_POS;
            break;
    }

    // show OUTn states
    Serial.print(F("OUT"));
    if (output == OUT_GH)
    {
        Serial.print(F("GH"));
    }
    else
    {
        Serial.print(output);
    }
    Serial.print(F(": "));
    if (OutHB == output)
    {
        Serial.println(F("HB"));
        return;
    }

    switch (output_state)
    {
        case OFF_OUT:
            Serial.println(F("Off"));
            break;
        case ON_OUT:
            Serial.println(F("On"));
            break;
        case TIMER1_OUT:
            Serial.println(F("T1"));
            break;
        case TIMER2_OUT:
            Serial.println(F("T2"));
            break;
        case PWM1_OUT:
            Serial.println(F("PWM1"));
            break;
        case PWM2_OUT:
            Serial.println(F("PWM2"));
            break;
        case PWM3_OUT:
            Serial.println(F("PWM3"));
            break;
        case PWM4_OUT:
            Serial.println(F("PWM4"));
            break;
        case PWM5_OUT:
            Serial.println(F("PWM5"));
            break;
        case PWM6_OUT:
            Serial.println(F("PWM6"));
            break;
        case PWM7_OUT:
            Serial.println(F("PWM7"));
            break;
        case DIR_OUT:
            Serial.println(F("DIR"));
            break;
    }
}

#ifdef SHOW_HEATER
/*
 * NAME:
 *  int8_t Cmd_outgh(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "outgh" command to show/set Heater output control.
 *
 *  One optional parameter supported.
 *   <off> = set OUTGH output state off
 *      -or-
 *   <on> = set OUTGH output state on
 *
 *        1    2
 *     "outgh"       - show OUTGH output state
 *     "outgh off"   - set OUTGH output state off
 *     "outgh on"    - set OUTGH output state on
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_outgh(int8_t argc, char * argv[])
{
    uint8_t output_type = OFF_OUT;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("off")) == 0)
        {
            output_type = OFF_OUT;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            output_type = ON_OUT;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_HeaterOutputControl(output_type);
    }

    ShowOutputState(OUT_GH);

    // Return success.
    return 0;
}
#endif

#ifdef SHOW_MIRROR
/*
 * NAME:
 *  int8_t Cmd_mirror(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "mirror" command to do MR200G1 mirror control.
 *
 *  Seven parameters supported.
 *   <idle> = go to idle state
 *      -or-
 *   <brake> = brake all mirror motors
 *      -or-
 *   <xcw> = run mirror X motor CW
 *      -or-
 *   <xccw> = run mirror X motor CCW
 *      -or-
 *   <ycw> = run mirror Y motor CW
 *      -or-
 *   <yccw> = run mirror Y motor CCW
 *      -or-
 *   <centr> = center mirror X-Y position
 *      -or-
 *   <fold> = fold mirror
 *      -or-
 *   <unfold> = unfold mirror
 *      -or-
 *   <htron> = turn on mirror heater
 *      -or-
 *   <htroff> = turn off mirror heater
 *      -or-
 *   <ecvon> = turn on mirror electrochromic
 *      -or-
 *   <ecvoff> = turn off mirror electrochromic
 *      -or-
 *   <farrw> = flash mirror arrow lights
 *      -or-
 *   <demo> = run mirror demo
 *
 *       1     2
 *     "mirror"         - show ???
 *     "mirror brake"   - brake all mirror motors
 *     "mirror xcw"     - run mirror X motor CW
 *     "mirror xccw"    - run mirror X motor CCW
 *     "mirror ycw"     - run mirror Y motor CW
 *     "mirror yccw"    - run mirror Y motor CCW
 *     "mirror centr"   - center mirror X-Y position
 *     "mirror fold"    - fold mirror
 *     "mirror unfold"  - unfold mirror
 *     "mirror htron"   - turn on mirror heater
 *     "mirror htroff"  - turn off mirror heater
 *     "mirror ecvon"   - turn on mirror electrochromic
 *     "mirror ecvoff"  - turn off mirror electrochromic
 *     "mirror farrw"   - flash mirror arrow lights
 *     "mirror demo"    - run mirror demo
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_mirror(int8_t argc, char * argv[])
{
    uint8_t cmd = IDLE;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
    }
    else
    {
        if (strcmp_P(argv[ARG1], PSTR("idle")) == 0)
        {
            cmd = IDLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("brake")) == 0)
        {
            cmd = BRAKE_ALL;
        }
        else if (strcmp_P(argv[ARG1], PSTR("xcw")) == 0)
        {
            cmd = X_CLOCKWISE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("xccw")) == 0)
        {
            cmd = X_C_CLOCKWISE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ycw")) == 0)
        {
            cmd = Y_CLOCKWISE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("yccw")) == 0)
        {
            cmd = Y_C_CLOCKWISE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("centr")) == 0)
        {
            cmd = CENTER_MIRROR;
        }
        else if (strcmp_P(argv[ARG1], PSTR("fold")) == 0)
        {
            cmd = F_FOLD;
        }
        else if (strcmp_P(argv[ARG1], PSTR("unfold")) == 0)
        {
            cmd = F_UNFOLD;
        }
        else if (strcmp_P(argv[ARG1], PSTR("htron")) == 0)
        {
            cmd = HEATER_ON;
        }
        else if (strcmp_P(argv[ARG1], PSTR("htroff")) == 0)
        {
            cmd = HEATER_OFF;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ecvon")) == 0)
        {
            cmd = ECV_ON;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ecvoff")) == 0)
        {
            cmd = ECV_OFF;
        }
        else if (strcmp_P(argv[ARG1], PSTR("farrw")) == 0)
        {
            cmd = ARROWS_FLASHING;
        }
        else if (strcmp_P(argv[ARG1], PSTR("demo")) == 0)
        {
            cmd = DEMO_ON;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        DoMirrorOperation(cmd);
    }

    // Return success.
    return 0;
}

void DoMirrorOperation(uint8_t cmd)
{
    const float XY_V_THRESHOLD = 2.5;
    float current;
    float voltage_x;
    float voltage_y;

    switch (cmd)
    {
        case IDLE:
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            break;

        case F_UNFOLD:
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, RIGHT_DIRECTION);
            break;
        case F_FOLD:
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, LEFT_DIRECTION);
            break;

        case X_CLOCKWISE:
            L99dz200g.L99DZ200G_MotorDriver(OUT2_X, RIGHT_DIRECTION);
            break;
        case X_C_CLOCKWISE:
            L99dz200g.L99DZ200G_MotorDriver(OUT2_X, LEFT_DIRECTION);
            break;

        case Y_CLOCKWISE:
            L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, RIGHT_DIRECTION);
            break;
        case Y_C_CLOCKWISE:
            L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, LEFT_DIRECTION);
            break;

        case BRAKE_ALL:
            // THE MOTORS RUN UNTIL WE SEND A BRAKE
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            break;

        case HEATER_ON:
            L99dz200g.L99DZ200G_HeaterOutputControl(ON_OUT);
            break;
        case HEATER_OFF:
            L99dz200g.L99DZ200G_HeaterOutputControl(OFF_OUT);
            break;

        case ECV_ON:
            L99dz200g.L99DZ200G_Set_ECV_Control(ENABLE);
            L99dz200g.L99DZ200G_Set_ECV_DriveVoltage(1.5);
            break;
        case ECV_OFF:
            L99dz200g.L99DZ200G_Set_ECV_DriveVoltage(0);
            L99dz200g.L99DZ200G_Set_ECV_Control(DISABLE);
            break;

        case ARROWS_ON:
            SetArrowsState(ON_OUT);
            break;
        case ARROWS_OFF:
            SetArrowsState(OFF_OUT);
            break;

        case ARROWS_FLASHING:
            SetArrowsState(ON_OUT);
            L99dz200g.L99DZ200G_Delay(950);
            SetArrowsState(OFF_OUT);
            L99dz200g.L99DZ200G_Delay(950);
            break;

        case CENTER_MIRROR:
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            // ----------folding
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, LEFT_DIRECTION);
            L99dz200g.L99DZ200G_Delay(6000);
            // -----------X
            voltage_x = ReadDataVoltage_X();
            if (voltage_x > XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT2_X, RIGHT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_x = ReadDataVoltage_X();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_x < XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            else if (voltage_x < XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT2_X, LEFT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_x = ReadDataVoltage_X();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_x > XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            // ------------ Y
            voltage_y = ReadDataVoltage_Y();
            if (voltage_y > XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, RIGHT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_y = ReadDataVoltage_Y();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_y < XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            else if (voltage_y < XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, LEFT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_y = ReadDataVoltage_Y();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_y > XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            break;

        case DEMO_ON:
            SetArrowsState(OFF_OUT);

            // ----------OPENING ROUTINE
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, LEFT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_1_CM);

            for (uint8_t j = 0; j < 11; j++)
            {
                current = ReadDataVoltage_OUTn();
                L99dz200g.L99DZ200G_Delay(300);
                if (current > 0.022)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);

            // ============================= ELECTROCHROME/HEATER ON
            L99dz200g.L99DZ200G_Set_ECV_Control(ENABLE);
            L99dz200g.L99DZ200G_HeaterOutputControl(ON_OUT);
            L99dz200g.L99DZ200G_Set_ECV_DriveVoltage(1.5);
            L99dz200g.L99DZ200G_Delay(6000);

            // ============================================================================
            // ------------X
            voltage_x = ReadDataVoltage_X();
            if (voltage_x > XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT2_X, RIGHT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_x = ReadDataVoltage_X();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_x < XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            else if (voltage_x < XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT2_X, LEFT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_x = ReadDataVoltage_X();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_x > XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            // ------------ Y
            voltage_y = ReadDataVoltage_Y();
            if (voltage_y > XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, RIGHT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_y = ReadDataVoltage_Y();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_y < XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }
            else if (voltage_y < XY_V_THRESHOLD)
            {
                L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, LEFT_DIRECTION);
                for (uint8_t j = 0; j < 50; j++)
                {
                    voltage_y = ReadDataVoltage_Y();
                    L99dz200g.L99DZ200G_Delay(100);
                    if (voltage_y > XY_V_THRESHOLD)
                    {
                        break;
                    }
                }
                L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            }

            // =================================================
            L99dz200g.L99DZ200G_HeaterOutputControl(OFF_OUT);
            L99dz200g.L99DZ200G_Delay(1000);
            L99dz200g.L99DZ200G_Set_ECV_DriveVoltage(0);
            L99dz200g.L99DZ200G_Delay(300);
            L99dz200g.L99DZ200G_Set_ECV_Control(DISABLE);

            // ============================================================================
            // TESTING ELECTROCHROME -----// TESTING HEATER PROCEDURE
            L99dz200g.L99DZ200G_Delay(1500);

            // // DEMO MIRROR POSITIONING
            L99dz200g.L99DZ200G_MotorDriver(OUT2_X, RIGHT_DIRECTION);   // Left
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_1_CM);
            for (uint8_t j = 0; j < 13; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.1)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT2_X, LEFT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_2_CM);
            for (uint8_t j = 0; j < 27; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.23)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT2_X, RIGHT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_1_CM);
            for (uint8_t j = 0; j < 13; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.1)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);
            L99dz200g.L99DZ200G_Delay(1500);

            /////////////////------------------------------------------///////////////////
            L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, RIGHT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_1_CM);
            for (uint8_t j = 0; j < 13; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.1)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, LEFT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_3_CM);
            for (uint8_t j = 0; j < 26; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.2)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT3_Y, RIGHT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_1_CM);
            for (uint8_t j = 0; j < 13; j++)
            {
                L99dz200g.L99DZ200G_Delay(300);
                current = ReadDataVoltage_OUTn();
                if (current > 0.1)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);

            // ============================================================================
            // MIRROR UNFOLDING
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, RIGHT_DIRECTION);
            L99dz200g.L99DZ200G_CM_OUTn_Select(OUT_6_CM);
            for (uint8_t j = 0; j < 11; j++)
            {
                current = ReadDataVoltage_OUTn();
                L99dz200g.L99DZ200G_Delay(300);
                if (current > 0.2)
                {
                    break;
                }
            }
            L99dz200g.L99DZ200G_MotorDriver(OUT6_F, BRAKE);

            SetArrowsState(ON_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(OFF_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(ON_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(OFF_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(ON_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(OFF_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(ON_OUT);
            L99dz200g.L99DZ200G_Delay(500);
            SetArrowsState(OFF_OUT);
            break;
    }
}

void SetArrowsState(uint8_t on_off)
{
    if (on_off)
    {
        L99dz200g.L99DZ200G_HSOutputsControl(ON_OUT, OUT_7);
        L99dz200g.L99DZ200G_HSOutputsControl(ON_OUT, OUT_8);
    }
    else
    {
        L99dz200g.L99DZ200G_HSOutputsControl(OFF_OUT, OUT_7);
        L99dz200g.L99DZ200G_HSOutputsControl(OFF_OUT, OUT_8);
    }
}

float ReadDataVoltage_X(void)
{
    return GetAnalogValue(MR200G1_X_POS_PIN);
}

float ReadDataVoltage_Y(void)
{
    return GetAnalogValue(MR200G1_Y_POS_PIN);
}

// OUTn item
float ReadDataVoltage_OUTn(void)
{
    float adc_value;

    // enable CM output on L99DZ200G CM_DIR pin
    L99dz200g.L99DZ200G_CM_DIR_Config(CM_ALWAYS);
    pinMode(L99DZ200G_DIR_PIN, INPUT);

    adc_value = GetAnalogValue(L99DZ200G_DIR_PIN);

    // re-enable DIR input on L99DZ200G CM_DIR pin
    L99dz200g.L99DZ200G_CM_DIR_Config(DIR_ALWAYS);
    pinMode(L99DZ200G_DIR_PIN, OUTPUT);

    return adc_value;
}
#endif

#ifdef SHOW_MOTOR
/*
 * NAME:
 *  int8_t Cmd_outm(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "outm" command to show/set OUT1, OUT2, OUT3, or OUT6 motor output control.
 *
 *  One optional parameter supported.
 *   <off> = set to OUTn output state off
 *      -or-
 *   <lo> = set to OUTn output state low
 *      -or-
 *   <hi> = set to OUTn output state high
 *
 *       1   2  3
 *     "outm 6"       - show OUT6 output state
 *     "outm 1 off"   - set OUT1 output state off (undriven)
 *     "outm 2 hi"    - set OUT2 output state high
 *     "outm 3 lo"    - set OUT3 output state low
 *     "outm 6 hi"    - set OUT6 output state high
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_outm(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t output_type = OFF_OUT;
    uint8_t output = 0;
    uint8_t out_item = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowOutputStateM(OUT_1);
        ShowOutputStateM(OUT_2);
        ShowOutputStateM(OUT_3);
        ShowOutputStateM(OUT_6);
    }
    else
    {
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_1:
            case OUT_2:
            case OUT_3:
            case OUT_6:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            // get the OUTn output type
            output_type = GetOutputTypeM(argv);
            if ((int8_t)output_type == CMDLINE_INVALID_ARG)
            {
                return CMDLINE_INVALID_ARG;
            }
            else
            {
                switch (val)
                {
                    case OUT_1:
                        switch (output_type)
                        {
                            case OFF_OUT:
                                out_item = OUT_1_LS;
                                break;
                            case LO_OUT:
                                out_item = OUT_1_LS;
                                break;
                            case HI_OUT:
                                out_item = OUT_1_HS;
                                break;
                        }
                        break;
                    case OUT_2:
                        switch (output_type)
                        {
                            case OFF_OUT:
                                out_item = OUT_2_LS;
                                break;
                            case LO_OUT:
                                out_item = OUT_2_LS;
                                break;
                            case HI_OUT:
                                out_item = OUT_2_HS;
                                break;
                        }
                        break;
                    case OUT_3:
                        switch (output_type)
                        {
                            case OFF_OUT:
                                out_item = OUT_3_LS;
                                break;
                            case LO_OUT:
                                out_item = OUT_3_LS;
                                break;
                            case HI_OUT:
                                out_item = OUT_3_HS;
                                break;
                        }
                        break;
                    case OUT_6:
                        switch (output_type)
                        {
                            case OFF_OUT:
                                out_item = OUT_6_LS;
                                break;
                            case LO_OUT:
                                out_item = OUT_6_LS;
                                break;
                            case HI_OUT:
                                out_item = OUT_6_HS;
                                break;
                        }
                        break;
                }
                L99dz200g.L99DZ200G_HS_LS_OutputsControl(output_type, out_item);
            }
        }

        ShowOutputStateM(output);
    }

    // Return success.
    return 0;
}

// get OUTn motor output pin output type
uint8_t GetOutputTypeM(char * argv[])
{
    uint8_t output_type = OFF_OUT;

    if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
    {
        output_type = OFF_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("lo")) == 0)
    {
        output_type = LO_OUT;
    }
    else if (strcmp_P(argv[ARG2], PSTR("hi")) == 0)
    {
        output_type = HI_OUT;
    }
    else
    {
        return CMDLINE_INVALID_ARG;
    }

    return output_type;
}

// get and show OUTn motor output pin output state
void ShowOutputStateM(uint8_t output)
{
    uint32_t reg;
    uint8_t output_state = OFF_OUT;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn motor register values
    switch (output)
    {
        case OUT_1:
        case OUT_2:
        case OUT_3:
        case OUT_6:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR4);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register states
    switch (output)
    {
        case OUT_1:
            reg &= (CR4_HS_OUT1_MASK | CR4_LS_OUT1_MASK);
            output_state = reg >> CR4_LS_OUT1_POS;
            break;
        case OUT_2:
            reg &= (CR4_HS_OUT2_MASK | CR4_LS_OUT2_MASK);
            output_state = reg >> CR4_LS_OUT2_POS;
            break;
        case OUT_3:
            reg &= (CR4_HS_OUT3_MASK | CR4_LS_OUT3_MASK);
            output_state = reg >> CR4_LS_OUT3_POS;
            break;
        case OUT_6:
            reg &= (CR4_HS_OUT6_MASK | CR4_LS_OUT6_MASK);
            output_state = reg >> CR4_LS_OUT6_POS;
            break;
    }

    // show OUTn states
    Serial.print(F("OUT"));
    Serial.print(output);
    Serial.print(F(": "));

    switch (output_state)
    {
        case OFF_OUT:
            Serial.println(F("Off"));
            break;
        case 1:
            Serial.println(F("Low"));
            break;
        case 2:
            Serial.println(F("High"));
            break;
    }
}
#endif

#ifdef SHOW_PWM
/*
 * NAME:
 *  int8_t Cmd_pwm(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "pwm" command to show/set PWM settings.
 *
 *  Three optional parameters supported.
 *   <chan> = the PWM channel to use
 *   <freq> = the PWM frequency to use
 *   <duty> = the PWM duty cycle (%) to use
 *
 *        1  2  3  4
 *     "pwm"           - show all channels PWM settings
 *     "pwm  1"        - show PWM channel 1 settings
 *     "pwm  1 100     - set PWM channel 1 to 100 Hz
 *     "pwm  2 200     - set PWM channel 2 to 200 Hz
 *     "pwm  3 330     - set PWM channel 3 to 330 Hz
 *     "pwm  4 500 25  - set PWM channel 4 to 500 Hz, 25% duty cycle
 *     "pwm  5 100 50  - set PWM channel 5 to 100 Hz, 50% duty cycle
 *     "pwm  6 200 75  - set PWM channel 6 to 200 Hz, 75% duty cycle
 *     "pwm  7 330 90  - set PWM channel 7 to 330 Hz, 90% duty cycle
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_pwm(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t pwm_chan = 99;
    uint8_t pwm_freq = 99;
    uint8_t pwm_duty = 255;

    if (argc > 4)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        // get the PWM channel
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
            (val < PWM_CHAN1) || (val > PWM_CHAN7))
        {
            return CMDLINE_INVALID_ARG;
        }
        pwm_chan = val;

        if (argc > 2)
        {
            // get the PWM frequency
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL))
            {
                return CMDLINE_INVALID_ARG;
            }
            switch (val )
            {
                case PWM_FREQ_100HZ_VAL:
                    pwm_freq = PWM_FREQ_100HZ;
                    break;
                case PWM_FREQ_200HZ_VAL:
                    pwm_freq = PWM_FREQ_200HZ;
                    break;
                case PWM_FREQ_330HZ_VAL:
                    pwm_freq = PWM_FREQ_330HZ;
                    break;
                case PWM_FREQ_500HZ_VAL:
                    pwm_freq = PWM_FREQ_500HZ;
                    break;
                default:
                    return CMDLINE_INVALID_ARG;
            }

            if (argc > 3)
            {
                // get the PWM duty cycle
                paramtype = CmdLine.ParseParam(argv[ARG3], &val);
                if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                    (val < 0) || (val > 100))
                {
                    return CMDLINE_INVALID_ARG;
                }
                pwm_duty = val;
            }
            L99dz200g.L99DZ200G_SetPWMFrequency(pwm_chan, pwm_freq);
            if (pwm_duty <= 100)
            {
                L99dz200g.L99DZ200G_SetPWMDutyCycle(pwm_chan, pwm_duty);
            }
        }
    }

    ShowPWM_Settings(pwm_chan);

    // Return success.
    return 0;
}

// get and show PWM settings for pwm_chan
void ShowPWM_Settings(uint8_t pwm_chan)
{
    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    if (pwm_chan > PWM_CHAN7)
    {
        // show all channels PWM settings
        for (uint8_t chan = PWM_CHAN1; chan <= PWM_CHAN7; ++chan)
        {
            ShowPWM_ChannelSettings(chan);
        }
        Serial.println("freq: 100, 200, 330, 500 Hz");
        return;
    }
    else
    {
        ShowPWM_ChannelSettings(pwm_chan);
    }
}

// get and show PWM settings for pwm_chan
void ShowPWM_ChannelSettings(uint8_t pwm_chan)
{
    uint32_t reg;
    uint8_t pwm_freq = PWM_FREQ_100HZ;
    uint16_t pwm_duty = 0;

    // get PWMn frequency register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR12);

    switch (pwm_chan)
    {
        case PWM_CHAN1:
            reg &= CR12_PWM1_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM1_FREQ_POS;
            break;
        case PWM_CHAN2:
            reg &= CR12_PWM2_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM2_FREQ_POS;
            break;
        case PWM_CHAN3:
            reg &= CR12_PWM3_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM3_FREQ_POS;
            break;
        case PWM_CHAN4:
            reg &= CR12_PWM4_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM4_FREQ_POS;
            break;
        case PWM_CHAN5:
            reg &= CR12_PWM5_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM5_FREQ_POS;
            break;
        case PWM_CHAN6:
            reg &= CR12_PWM6_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM6_FREQ_POS;
            break;
        case PWM_CHAN7:
            reg &= CR12_PWM7_FREQ_MASK;
            pwm_freq = reg >> CR12_PWM7_FREQ_POS;
            break;
    }

    // get PWMn duty cycle register values
    switch (pwm_chan)
    {
        case PWM_CHAN1:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR13);
            reg &= HI_PWM_DC_MASK;
            pwm_duty = reg >> HI_PWM_DC_POS;
            break;
        case PWM_CHAN2:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR13);
            reg &= LO_PWM_DC_MASK;
            pwm_duty = reg >> LO_PWM_DC_POS;
            break;
        case PWM_CHAN3:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR14);
            reg &= HI_PWM_DC_MASK;
            pwm_duty = reg >> HI_PWM_DC_POS;
            break;
        case PWM_CHAN4:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR14);
            reg &= LO_PWM_DC_MASK;
            pwm_duty = reg >> LO_PWM_DC_POS;
            break;
        case PWM_CHAN5:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR15);
            reg &= HI_PWM_DC_MASK;
            pwm_duty = reg >> HI_PWM_DC_POS;
            break;
        case PWM_CHAN6:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR15);
            reg &= LO_PWM_DC_MASK;
            pwm_duty = reg >> LO_PWM_DC_POS;
            break;
        case PWM_CHAN7:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR16);
            reg &= HI_PWM_DC_MASK;
            pwm_duty = reg >> HI_PWM_DC_POS;
            break;
    }

    // show PWMn settings
    Serial.print(F("PWM"));
    Serial.print(pwm_chan);
    Serial.print(F(": "));
    switch (pwm_freq)
    {
        case PWM_FREQ_100HZ:
            Serial.print(PWM_FREQ_100HZ_VAL);
            break;
        case PWM_FREQ_200HZ:
            Serial.print(PWM_FREQ_200HZ_VAL);
            break;
        case PWM_FREQ_330HZ:
            Serial.print(PWM_FREQ_330HZ_VAL);
            break;
        case PWM_FREQ_500HZ:
            Serial.print(PWM_FREQ_500HZ_VAL);
            break;
    }
    Serial.print(F(" Hz, "));

    pwm_duty = map(pwm_duty, 0, 1023, 0, 100);  // map 0 to 1023 register value to 0 to 100% duty cycle
    Serial.print(pwm_duty);
    Serial.println(F("%"));
}
#endif

#ifdef SHOW_RDSON
/*
 * NAME:
 *  int8_t Cmd_rdson(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "rdson" command to show/set OUT1_6, OUT7, or OUT8 Rdson output control.
 *
 *  One optional parameter supported.
 *   <lo>  = set Rdson output low
 *      -or-
 *   <hi> = set Rdson output high
 *
 *       1    2 3
 *     "rdson 7"       - show OUT7 Rdson setting
 *     "rdson 8 hi"    - set OUT8 high Rdson
 *     "rdson 7 lo"    - set OUT7 low Rdson
 *     "rdson 1 hi"    - set OUT1, OUT6 high Rdson (OUT1 and OUT6 cannot be configured separately)
 *     "rdson 6 lo"    - set OUT1, OUT6 low Rdson (OUT1 and OUT6 cannot be configured separately)
 *
 *  Low/High Rdson settings:
 *    OUT1 and OUT6 low Rdson = ~150 mohm
 *    OUT1 and OUT6 high Rdson = ~300 mohm
 *
 *    OUT7 low Rdson = ~500 mohm
 *    OUT7 high Rdson = ~1600 mohm
 *
 *    OUT8 low Rdson = ~800 mohm
 *    OUT8 high Rdson = ~1600 mohm
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_rdson(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t rdson;
    uint8_t output = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowRdsonSetting(OUT_1_6);
        ShowRdsonSetting(OUT_7);
        ShowRdsonSetting(OUT_8);
    }
    else
    {
DEBUG_TOGL();
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_1:
            case OUT_6:
                output = OUT_1_6;               // OK
                break;
            case OUT_7:
            case OUT_8:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("lo")) == 0)
            {
                rdson = RDS_ON_LO;
            }
            else if (strcmp_P(argv[ARG2], PSTR("hi")) == 0)
            {
                rdson = RDS_ON_HI;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_Set_OUTn_Rdson(output, rdson);
        }
DEBUG_TOGL();
        ShowRdsonSetting(output);
    }

    // Return success.
    return 0;
}

// get and show OUTn Rdson setting
void ShowRdsonSetting(uint8_t output)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn Rdson register values
    switch (output)
    {
        case OUT_1_6:
        case OUT_7:
        case OUT_8:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
            break;
        default:
            return;     // invalid output
    }

    // get OUTn register Rdson setting
    switch (output)
    {
        case OUT_1_6:
            reg &= CR9_OUT1_6_RSDON_MASK;
            break;
        case OUT_7:
            reg &= CR9_OUT7_RSDON_MASK;
            break;
        case OUT_8:
            reg &= CR9_OUT8_RSDON_MASK;
            break;
    }

    // show OUTn Rdson setting
    Serial.print(F("OUT"));
    if (output == OUT_1_6)
    {
        Serial.print(F("1_6"));
    }
    else
    {
        Serial.print(output);
    }

    Serial.print(F(": "));
    ShowHighLow(reg);
}
#endif

#ifdef SHOW_SCT
/*
 * NAME:
 *  int8_t Cmd_sct(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "sct" command to show/set OUT1, OUT2, OUT3, or OUT6 short circuit
 *  threshold control.
 *
 *  One optional parameter supported.
 *   <on>  = turn short circuit threshold control on
 *      -or-
 *   <off> = turn short circuit threshold control off
 *
 *       1  2  3
 *     "sct 6"      - show OUT6 short circuit threshold control setting
 *     "sct 1 on"   - set OUT1 short circuit threshold control on
 *     "sct 2 off"  - set OUT2 short circuit threshold control off
 *     "sct 3 on"   - set OUT3 short circuit threshold control on
 *     "sct 6 off"  - set OUT6 short circuit threshold control off
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_sct(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t en_dis;
    uint8_t output = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowShortCircuitThresholdSetting(OUT_1);
        ShowShortCircuitThresholdSetting(OUT_2);
        ShowShortCircuitThresholdSetting(OUT_3);
        ShowShortCircuitThresholdSetting(OUT_6);
    }
    else
    {
DEBUG_TOGL();
        // get the OUTn number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL))
        {
            return CMDLINE_INVALID_ARG;
        }
        switch (val)
        {
            case OUT_1:
            case OUT_2:
            case OUT_3:
            case OUT_6:
                output = val;                   // OK
                break;
            default:
                return CMDLINE_INVALID_ARG;     // invalid
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                en_dis = SC_THRSH_OFF;
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                en_dis = SC_THRSH_ON;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
            L99dz200g.L99DZ200G_SetShortCircuitControl(output, en_dis);
        }
        ShowShortCircuitThresholdSetting(output);
DEBUG_TOGL();
    }

    // Return success.
    return 0;
}

// get and show OUTn short circuit threshold control
void ShowShortCircuitThresholdSetting(uint8_t output)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn short circuit threshold register values
    switch (output)
    {
            case OUT_1:
            case OUT_2:
            case OUT_3:
            case OUT_6:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR7);
            break;
        default:
            return;     // invalid output
            break;
    }

    // get OUTn register short circuit threshold
    switch (output)
    {
        case OUT_1:
            reg &= CR7_OUT1_SHORT_MASK;
            break;
        case OUT_2:
            reg &= CR7_OUT2_SHORT_MASK;
            break;
        case OUT_3:
            reg &= CR7_OUT3_SHORT_MASK;
            break;
        case OUT_6:
            reg &= CR7_OUT6_SHORT_MASK;
            break;
    }

    // show OUTn short circuit threshold enable
    Serial.print(F("OUT"));
    Serial.print(output);
    Serial.print(F(": "));
    ShowEnabledDisabledRev(reg, true);        // Note: is backward from usual enable/disable
}
#endif

#ifdef SHOW_STAT
/*
 * NAME:
 *  int8_t Cmd_stat(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "stat" command to get and show all L99DZ200G status registers.
 *
 *        1
 *     "stat"        - show all L99DZ200G status registers (SR1 to SR6 and SR12)
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_stat(int8_t argc, char * argv[])
{
    bool clear = false;
    bool status = false;
    uint32_t regval;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        (void)argv;
        (void)clear;
        (void)status;

        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        for (uint8_t i = L99DZ200G_SR1; i <= L99DZ200G_SR12; ++i)
        {
            L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

            if ((i < L99DZ200G_SR7) || (i == L99DZ200G_SR12))
            {
                Serial.print(F("L99DZ200G SR"));
                Serial.print(i - L99DZ200G_SR1 + 1);
                Serial.print(F(": "));
                if (i < L99DZ200G_SR7)
                {
                    Serial.print(F(" "));
                }
                Print0xHexByte(i);
                Serial.print(F(": "));
                regval = L99dz200g.L99DZ200G_ReadRegister(i);
                Print0xHex24(regval);
                if (regval & FULL_REG_MASK)
                {
                    switch (i)
                    {
                        case L99DZ200G_SR3:
                            Serial.print(F(" (_OC)"));
                            break;
                        case L99DZ200G_SR4:
                            Serial.print(F(" (_OCR_ALERT, OUTn_SHORT)"));
                            break;
                        case L99DZ200G_SR5:
                            Serial.print(F(" (_OL)"));
                            break;
                    }
                }
                Serial.println();
            }
        }
    }

    // Return success.
    return 0;
}
#endif

#ifdef SHOW_TIMER
/*
 * NAME:
 *  int8_t Cmd_tmr(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "tmr" command to show/set Timer settings.
 *
 *  Three optional parameters supported.
 *   <n> = the Timer to use             (1 or 2)
 *   <per> = the Timer period to use    (10ms or 20ms or 50ms or 100ms or 200ms or 500ms or 1s or 2s)
 *   <ton> = the Timer time-on to use   (100us or 300us or 1ms or 10ms or 20ms)
 *   <rst> = restart the Timer          (<on> or <off>)
 *   <dir> = DIR-control the Timer      (<on> or <off>)
 *
 *       1  2  3     4    5   6
 *     "tmr"                      - show all Timers settings
 *     "tmr 1"                    - show Timer 1 settings
 *     "tmr 2"                    - show Timer 2 settings
 *     "tmr 1 10ms                - set Timer 1 to 10 mS period
 *     "tmr 2 20ms  100us         - set Timer 2 to 20 mS period, 100 uS time-on (restart)
 *     "tmr 1 50ms  300us         - set Timer 1 to 50 mS period, 300 uS time-on (restart)
 *     "tmr 1 100ms 1ms           - set Timer 1 to 100 mS period, 1 mS time-on (restart)
 *     "tmr 2 200ms 10ms          - set Timer 2 to 200 mS period, 10 mS time-on (restart)
 *     "tmr 2 500ms 20ms          - set Timer 2 to 500 mS period, 20 mS time-on (restart)
 *     "tmr 1 1s    20ms  on      - set Timer 1 to 1 S period, 20 mS time-on, restart
 *     "tmr 2 20ms  100us on  on  - set Timer 2 to 20 mS period, 100 uS time-on, restart, DIR-controlled
 *     "tmr 1 2s    20ms  off on  - set Timer 1 to 2 S period, 20 mS time-on, no restart, DIR-controlled
 *     "tmr 2 20ms  100us on  off - set Timer 2 to 20 mS period, 100 uS time-on, restart, not DIR-controlled
 *       1  2  3     4    5   6
 *              ________                                         _____
 *  Tn: _______|   ton  |_______________________________________| ton |_________________________
 *             |<--------------------- per -------------------->|     ^-- when DIR-controlled
 *           _________________________________________________________
 *  DIR: ___|                                                         |_________________________
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_tmr(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t timer = 255;
    uint8_t tmr_per = 255;
    uint8_t tmr_ton = 255;
    uint8_t tmr_rst = 255;
    uint8_t tmr_dir = 255;
    uint32_t reg;

    if (argc > 6)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowTimerSettings(TYPE_TIMER1);
        ShowTimerSettings(TYPE_TIMER2);
        Serial.println("per: 10ms, 20ms, 50ms, 100ms, 200ms, 500ms, 1s, 2s");
        Serial.println("ton: 100us, 300us, 1ms, 10ms, 20ms");
    }
    else
    {
        // get the Timer number
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
            (val < TYPE_TIMER1) || (val > TYPE_TIMER2))
        {
            return CMDLINE_INVALID_ARG;
        }
        timer = val;

        if (argc > 2)
        {
            // get the Timer period
            if (strcmp_P(argv[ARG2], PSTR("10ms")) == 0)
            {
                tmr_per = T_10MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("20ms")) == 0)
            {
                tmr_per = T_20MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("50ms")) == 0)
            {
                tmr_per = T_50MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("100ms")) == 0)
            {
                tmr_per = T_100MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("200ms")) == 0)
            {
                tmr_per = T_200MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("500ms")) == 0)
            {
                tmr_per = T_500MS;
            }
            else if (strcmp_P(argv[ARG2], PSTR("1s")) == 0)
            {
                tmr_per = T_1S;
            }
            else if (strcmp_P(argv[ARG2], PSTR("2s")) == 0)
            {
                tmr_per = T_2S;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            if (argc > 3)
            {
                // get the Timer time-on
                if (strcmp_P(argv[ARG3], PSTR("100us")) == 0)
                {
                    tmr_ton = TON_100US;
                }
                else if (strcmp_P(argv[ARG3], PSTR("300us")) == 0)
                {
                    tmr_ton = TON_300US;
                }
                else if (strcmp_P(argv[ARG3], PSTR("1ms")) == 0)
                {
                    tmr_ton = TON_1MS;
                }
                else if (strcmp_P(argv[ARG3], PSTR("10ms")) == 0)
                {
                    tmr_ton = TON_10MS;
                }
                else if (strcmp_P(argv[ARG3], PSTR("20ms")) == 0)
                {
                    tmr_ton = TON_20MS;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }

                if (argc > 4)
                {
                    // get the Timer restart
                    if (strcmp_P(argv[ARG4], PSTR("on")) == 0)
                    {
                        tmr_rst = ENABLE;
                    }
                    else if (strcmp_P(argv[ARG4], PSTR("off")) == 0)
                    {
                        tmr_rst = DISABLE;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    if (argc > 5)
                    {
                        // get the Timer DIR-controlled
                        if (strcmp_P(argv[ARG5], PSTR("on")) == 0)
                        {
                            tmr_dir = ENABLE;
                        }
                        else if (strcmp_P(argv[ARG5], PSTR("off")) == 0)
                        {
                            tmr_dir = DISABLE;
                        }
                        else
                        {
                            return CMDLINE_INVALID_ARG;
                        }
                    }
                }
            }

            if ((tmr_per == 255) || (tmr_ton == 255) ||
                (tmr_rst == 255) || (tmr_dir == 255))
            {
                // get current Timers settings register values
                reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR2) & (CR2_T1_MASK | CR2_T2_MASK);

                if (tmr_per == 255)
                {
                    switch (timer)
                    {
                        case TYPE_TIMER1:
                            tmr_per = (reg & CR2_T1_PER_MASK) >> CR2_T1_PER_POS;
                            break;
                        case TYPE_TIMER2:
                            tmr_per = (reg & CR2_T2_PER_MASK) >> CR2_T2_PER_POS;
                            break;
                    }
                }
                if (tmr_ton == 255)
                {
                    switch (timer)
                    {
                        case TYPE_TIMER1:
                            tmr_ton = (reg & CR2_T1_ON_MASK) >> CR2_T1_ON_POS;
                            break;
                        case TYPE_TIMER2:
                            tmr_ton = (reg & CR2_T2_ON_MASK) >> CR2_T2_ON_POS;
                            break;
                    }
                }
                if (tmr_rst == 255)
                {
                    tmr_rst = ENABLE;   // recommended - see Table 97, Note 1
                }
                if (tmr_dir == 255)
                {
                    switch (timer)
                    {
                        case TYPE_TIMER1:
                            tmr_dir = (reg & CR2_T1_DIR_MASK) >> CR2_T1_DIR_POS;
                            break;
                        case TYPE_TIMER2:
                            tmr_dir = (reg & CR2_T2_DIR_MASK) >> CR2_T2_DIR_POS;
                            break;
                    }
                }
            }

            L99dz200g.L99DZ200G_SetTimerConfig(timer, tmr_per, tmr_rst, tmr_dir, tmr_ton);
        }

        ShowTimerSettings(timer);
    }

    // Return success.
    return 0;
}

// get and show Timer settings for timer
void ShowTimerSettings(uint8_t timer)
{
    uint32_t reg;
    uint8_t tmr_per = 0;
    uint8_t tmr_ton = 0;
    uint8_t tmr_rst = 0;
    uint8_t tmr_dir = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get Timers settings register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR2) & (CR2_T1_MASK | CR2_T2_MASK);

    // get Timers period values
    // get Timers time-on values
    // get Timers restart values
    // get Timers DIR-controlled values
    switch (timer)
    {
        case TYPE_TIMER1:
            tmr_per = (reg & CR2_T1_PER_MASK) >> CR2_T1_PER_POS;
            tmr_ton = (reg & CR2_T1_ON_MASK) >> CR2_T1_ON_POS;
            tmr_rst = (reg & CR2_T1_RESTART_MASK) >> CR2_T1_RESTART_POS;
            tmr_dir = (reg & CR2_T1_DIR_MASK) >> CR2_T1_DIR_POS;
            break;
        case TYPE_TIMER2:
            tmr_per = (reg & CR2_T2_PER_MASK) >> CR2_T2_PER_POS;
            tmr_ton = (reg & CR2_T2_ON_MASK) >> CR2_T2_ON_POS;
            tmr_rst = (reg & CR2_T2_RESTART_MASK) >> CR2_T2_RESTART_POS;
            tmr_dir = (reg & CR2_T2_DIR_MASK) >> CR2_T2_DIR_POS;
            break;
    }

    // show Timer settings
    Serial.print(F("T"));
    Serial.print(timer);

    Serial.print(F(": Period: "));
    switch (tmr_per)
    {
        case T_10MS:
            Serial.print("10 mS");
            break;
        case T_20MS:
            Serial.print("20 mS");
            break;
        case T_50MS:
            Serial.print("50 mS");
            break;
        case T_100MS:
            Serial.print("100 mS");
            break;
        case T_200MS:
            Serial.print("200 mS");
            break;
        case T_500MS:
            Serial.print("500 mS");
            break;
        case T_1S:
            Serial.print("1 S");
            break;
        case T_2S:
            Serial.print("2 S");
            break;
    }

    Serial.print(F(", Time-on: "));
    switch (tmr_ton)
    {
        case TON_100US:
            Serial.print("100 uS");
            break;
        case TON_300US:
            Serial.print("300 uS");
            break;
        case TON_1MS:
            Serial.print("1 mS");
            break;
        case TON_10MS:
            Serial.print("10 mS");
            break;
        case TON_20MS:
            Serial.print("20 mS");
            break;
    }

    Serial.print(F(", Restart: "));
    if (tmr_rst)
    {
        Serial.print("On");
    }
    else
    {
        Serial.print("Off");
    }

    Serial.print(F(", DIR: "));
    ShowOnOff(tmr_dir);
}
#endif

/*
 * NAME:
 *  int8_t Cmd_wake(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "wake" command to pulse Wake output pin.
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_wake(int8_t argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    if (argc > 1)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        PulseWake();
    }

    // Return success.
    return 0;
}

// pulse Wake output pin
void PulseWake(void)
{
    digitalWrite(L99DZ200G_WAKE_PIN, HIGH);
    delay(1);
    digitalWrite(L99DZ200G_WAKE_PIN, LOW);
}




/*
 * NAME:
 *  int8_t Cmd_help(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "help" command to display a simple list of the available
 *  commands with a brief description of each command.
 *
 *  One optional parameter supported.
 *   <cls> = clears the output screen using ANSI escape sequence
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_help(int8_t argc, char * argv[])
{
    if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("cls")) == 0)
        {
            Serial.println(F(CLS_HOME));
            return 0;
        }
    }

    // Print some header text.
    Serial.println();
    Serial.println(TITLE_MSG);
    Serial.println(F("Available commands"));
    Serial.println(F("------------------"));

    while (!L99dz200g.L99DZ200G_CheckWdogExpired())     // process watchdog
    {
        delay(1);       // wait for watchdog to expire
    }

    CmdLine.ShowCommands();     // show commands menu with help information

    Serial.print(F("WatchdogRunning: "));
    Serial.println(L99dz200g.L99DZ200G_WatchdogRunning());

    // Return success.
    return 0;
}

/*
 * NAME:
 *  void PrintHexByte(uint8_t the_byte)
 *
 * PARAMETERS:
 *  uint8_t the_byte = the byte to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 8-bit byte value to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void PrintHexByte(uint8_t the_byte)
{
    if (the_byte < 0x10)
    {
        Serial.print('0');          // leading '0'
    }
    Serial.print(the_byte, HEX);
}

void PrintHexByteln(uint8_t the_byte)
{
    PrintHexByte(the_byte);
    Serial.println();
}

/*
 * NAME:
 *  void Print0xHexByte(uint8_t the_byte)
 *
 * PARAMETERS:
 *  uint8_t the_byte = the byte to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 8-bit byte value with leading "0x" to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void Print0xHexByte(uint8_t the_byte)
{
    Serial.print(F("0x"));          // leading "0x"
    PrintHexByte(the_byte);
}

void Print0xHexByteln(uint8_t the_byte)
{
    Print0xHexByte(the_byte);
    Serial.println();
}

/*
 * NAME:
 *  void PrintHex12(uint16_t the_12bitword)
 *
 * PARAMETERS:
 *  uint16_t the_12bitword = the 12-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 12-bit word value to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void PrintHex12(uint16_t the_12bitword)
{
    Serial.print((the_12bitword >> 8) & 0xff, HEX);
    PrintHexByte(the_12bitword & 0xff);
}

void PrintHex12ln(uint16_t the_12bitword)
{
    PrintHex12(the_12bitword);
    Serial.println();
}

/*
 * NAME:
 *  void Print0xHex12(uint16_t the_12bitword)
 *
 * PARAMETERS:
 *  uint16_t the_12bitword = the 12-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 12-bit word value with leading "0x" to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void Print0xHex12(uint16_t the_12bitword)
{
    Serial.print(F("0x"));          // leading "0x"
    PrintHex12(the_12bitword);
}

void Print0xHex12ln(uint16_t the_12bitword)
{
    Print0xHex12(the_12bitword);
    Serial.println();
}

/*
 * NAME:
 *  void PrintHex16(uint16_t the_16bitword)
 *
 * PARAMETERS:
 *  uint16_t the_16bitword = the 16-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 16-bit word value to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void PrintHex16(uint16_t the_16bitword)
{
    PrintHexByte((the_16bitword >> 8) & 0xff);
    PrintHexByte(the_16bitword & 0xff);
}

void PrintHex16ln(uint16_t the_16bitword)
{
    PrintHex16(the_16bitword);
    Serial.println();
}

/*
 * NAME:
 *  void Print0xHex16(uint16_t the_16bitword)
 *
 * PARAMETERS:
 *  uint16_t the_16bitword = the 16-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 16-bit word value with leading "0x" to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void Print0xHex16(uint16_t the_16bitword)
{
    Serial.print(F("0x"));          // leading "0x"
    PrintHex16(the_16bitword);
}

void Print0xHex16ln(uint16_t the_16bitword)
{
    Print0xHex16(the_16bitword);
    Serial.println();
}

/*
 * NAME:
 *  void PrintHex24(uint32_t the_24bitword)
 *
 * PARAMETERS:
 *  uint32_t the_24bitword = the 24-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 24-bit word value to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void PrintHex24(uint32_t the_24bitword)
{
    PrintHexByte((the_24bitword >> 16) & 0xff);
    PrintHexByte((the_24bitword >> 8) & 0xff);
    PrintHexByte(the_24bitword & 0xff);
}

void PrintHex24ln(uint32_t the_24bitword)
{
    PrintHex24(the_24bitword);
    Serial.println();
}

/*
 * NAME:
 *  void Print0xHex24(uint32_t the_24bitword)
 *
 * PARAMETERS:
 *  uint32_t the_24bitword = the 24-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 24-bit word value with leading "0x" to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void Print0xHex24(uint32_t the_24bitword)
{
    Serial.print(F("0x"));          // leading "0x"
    PrintHex24(the_24bitword);
}

void Print0xHex24ln(uint32_t the_24bitword)
{
    Print0xHex24(the_24bitword);
    Serial.println();
}

/*
 * NAME:
 *  void PrintHex32(uint32_t the_32bitword)
 *
 * PARAMETERS:
 *  uint32_t the_32bitword = the 32-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 32-bit word value to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void PrintHex32(uint32_t the_32bitword)
{
    PrintHexByte((the_32bitword >> 24) & 0xff);
    PrintHexByte((the_32bitword >> 16) & 0xff);
    PrintHexByte((the_32bitword >> 8) & 0xff);
    PrintHexByte(the_32bitword & 0xff);
}

void PrintHex32ln(uint32_t the_32bitword)
{
    PrintHex32(the_32bitword);
    Serial.println();
}

/*
 * NAME:
 *  void Print0xHex32(uint32_t the_32bitword)
 *
 * PARAMETERS:
 *  uint32_t the_32bitword = the 32-bit word to print
 *
 * WHAT:
 *  Helper function that prints formatted hex 32-bit word value with leading "0x" to console.
 *
 * RETURN VALUES:
 *  None.
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
void Print0xHex32(uint32_t the_32bitword)
{
    Serial.print(F("0x"));          // leading "0x"
    PrintHex32(the_32bitword);
}

void Print0xHex32ln(uint32_t the_32bitword)
{
    Print0xHex32(the_32bitword);
    Serial.println();
}

void ShowEnabledDisabled(uint32_t val, bool crlf)
{
    if (val != 0)
    {
        Serial.print(F("En"));
    }
    else
    {
        Serial.print(F("Dis"));
    }
    Serial.print(F("abled"));
    if (crlf)
    {
        Serial.println();
    }
}

void ShowEnabledDisabledRev(uint32_t val, bool crlf)
{
    if (val != 0)
    {
        Serial.print(F("Dis"));
    }
    else
    {
        Serial.print(F("En"));
    }
    Serial.print(F("abled"));
    if (crlf)
    {
        Serial.println();
    }
}

void ShowHighLow(uint32_t val)
{
    if (val != 0)
    {
        Serial.print(F("High"));
    }
    else
    {
        Serial.print(F("Low"));
    }
    Serial.println();
}

void ShowOnOff(uint32_t val)
{
    if (val != 0)
    {
        Serial.print(F("On"));
    }
    else
    {
        Serial.print(F("Off"));
    }
    Serial.println();
}

void ShowMaskedUnmasked(uint32_t val)
{
    if (val == 0)
    {
        Serial.print(F("un"));
    }
    Serial.println(F("masked"));
}

