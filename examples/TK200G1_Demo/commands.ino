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
#define SHOW_PWM
#define SHOW_STAT
#define SHOW_TIMER
#define SHOW_HBRIDGE
#define SHOW_TRUNK

// local function prototypes
int8_t Cmd_help(int8_t argc, char * argv[]);
int8_t Cmd_rreg(int8_t argc, char * argv[]);
int8_t Cmd_wreg(int8_t argc, char * argv[]);
int8_t Cmd_rclr(int8_t argc, char * argv[]);

#ifdef SHOW_ADC
int8_t Cmd_adc(int8_t argc, char * argv[]);
#endif
int8_t Cmd_dir(int8_t argc, char * argv[]);
int8_t Cmd_gsb(int8_t argc, char * argv[]);
int8_t Cmd_init(int8_t argc, char * argv[]);
int8_t Cmd_out(int8_t argc, char * argv[]);
#ifdef SHOW_PWM
int8_t Cmd_pwm(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_STAT
int8_t Cmd_stat(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_TIMER
int8_t Cmd_tmr(int8_t argc, char * argv[]);
#endif
int8_t Cmd_wake(int8_t argc, char * argv[]);
#ifdef SHOW_HBRIDGE
int8_t Cmd_hb(int8_t argc, char * argv[]);
int8_t Cmd_hbm(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_TRUNK
int8_t Cmd_trunk(int8_t argc, char * argv[]);
#endif

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
const char MenuCmdDir[] PROGMEM   = "dir";
const char MenuCmdGsb[] PROGMEM   = "gsb";
const char MenuCmdInit[] PROGMEM  = "init";
const char MenuCmdOut[] PROGMEM   = "out";
#ifdef SHOW_PWM
const char MenuCmdPwm[] PROGMEM   = "pwm";
#endif
#ifdef SHOW_STAT
const char MenuCmdStat[] PROGMEM  = "stat";
#endif
#ifdef SHOW_TIMER
const char MenuCmdTmr[] PROGMEM   = "tmr";
#endif
const char MenuCmdWake[] PROGMEM  = "wake";
#ifdef SHOW_HBRIDGE
const char MenuCmdHb[] PROGMEM    = "hb";
const char MenuCmdHbm[] PROGMEM   = "hbm";
#endif
#ifdef SHOW_TRUNK
const char MenuCmdTrunk[] PROGMEM = "trunk";
#endif

// menu items individual command help strings
const char MenuHelp1[] PROGMEM     =    " [<cls>]                     : Display list of commands (clear screen)";
const char MenuHelp2[] PROGMEM     = "                                : alias for help";
const char MenuHelpRreg[] PROGMEM  =    " reg [cnt]                   : Show L99DZ200G register(s) value(s)";
const char MenuHelpWreg[] PROGMEM  =    " [reg val] | <all>           : Set(reset all) L99DZ200G control register value(s)";
const char MenuHelpRclr[] PROGMEM  =    " [reg val] | <all>           : Read/clear L99DZ200G status register(s)";
#ifdef SHOW_ADC
const char MenuHelpAdc[] PROGMEM   =   " [n | <A> | <B>] [<v> | <r>]  : Show analog input value (v = volts, r = raw)";
#endif
const char MenuHelpDir[] PROGMEM   =   " [lo | hi]                    : Show[set] DIR output pin";
const char MenuHelpGsb[] PROGMEM   =   "                              : Show L99DZ200G Global Status Byte";
const char MenuHelpInit[] PROGMEM  =    "                             : Init L99DZ200G";
const char MenuHelpOut[] PROGMEM   =   " [n [off | on | t1 | t2 | pwm1 | pwm2 | pwm3 | pwm4 | pwm5 | pwm6 | pwm7 | dir | hb]]"
                                "\r\n                                 : Show[set] L99DZ200G OUTn output control";
#ifdef SHOW_PWM
const char MenuHelpPwm[] PROGMEM   =   " [chan [freq [duty]]]         : Show[set] L99DZ200G PWM settings";
#endif
#ifdef SHOW_STAT
const char MenuHelpStat[] PROGMEM  =    "                             : Show all L99DZ200G status registers";
#endif
#ifdef SHOW_TIMER
const char MenuHelpTmr[] PROGMEM   =   " [n [per [ton [rst [dir]]]    : Show[set] L99DZ200G Timer settings";
#endif
const char MenuHelpWake[] PROGMEM  =    "                             : Pulse Wakeup output pin";
#ifdef SHOW_HBRIDGE
const char MenuHelpHb[] PROGMEM    =  " [AB [on | off] | [mode  [sng | dual]] | [clrds] | [ccpt Xns] | [dsth thr] | [dmside leg [L | H]] |"
                                "\r\n   [dmtype leg [A | P]] | [olht [on | off]] | [pwm %] | [smdir [L | R]] | [smside [L | H]] |"
                                "\r\n   [smtype [A | P]] | [slew %] | [oltst [h1l2 | h2l1] [on | off]]] : Show[set] L99DZ200G H-Bridge control";
const char MenuHelpHbm[] PROGMEM   =   " [AB [spd% [L | R]]]          : Show[set] L99DZ200G H-Bridge motor control";
#endif
#ifdef SHOW_TRUNK
const char MenuHelpTrunk[] PROGMEM =     " open | close | lock | unlock | stop | test | delay val: Do TK200G1 trunk control";
#endif

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
    { MenuCmdDir,     Cmd_dir,     MenuHelpDir     },
    { MenuCmdGsb,     Cmd_gsb,     MenuHelpGsb     },
    { MenuCmdInit,    Cmd_init,    MenuHelpInit    },
    { MenuCmdOut,     Cmd_out,     MenuHelpOut     },
#ifdef SHOW_PWM
    { MenuCmdPwm,     Cmd_pwm,     MenuHelpPwm     },
#endif
#ifdef SHOW_STAT
    { MenuCmdStat,    Cmd_stat,    MenuHelpStat    },
#endif
#ifdef SHOW_TIMER
    { MenuCmdTmr,     Cmd_tmr,     MenuHelpTmr     },
#endif
    { MenuCmdWake,    Cmd_wake,    MenuHelpWake    },
#ifdef SHOW_HBRIDGE
    { MenuCmdHb,      Cmd_hb,      MenuHelpHb      },
    { MenuCmdHbm,     Cmd_hbm,     MenuHelpHbm     },
#endif
#ifdef SHOW_TRUNK
    { MenuCmdTrunk,   Cmd_trunk,   MenuHelpTrunk   },
#endif
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
 *  OUTn Current Monitor: OUT7, OUT8
 *  A, B  H-Bridge A Current Sense Monitor: A, B
 *
 *  Two optional parameters supported.
 *   n = show OUTn current monitor
 *      -or-
 *   <A> = show H-Bridge A current sense
 *      -or-
 *   <B> = show H-Bridge B current sense
 *      -and-
 *   <v> = show OUTn voltage
 *      -or-
 *   <r> = show OUTn raw ADC hex value
 *
 *       1  2 3
 *     "adc 8"    - show OUT8 current
 *     "adc 7"    - show OUT7 current
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
        Serial.println("OUT: 7, 8,  A, B  [v | r]");
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
        if ((tolower(argv[ARG1][0]) == 'a') && (argv[ARG1][1] == '\0'))
        {
            item = 'a';
        }
        else if ((tolower(argv[ARG1][0]) == 'b') && (argv[ARG1][1] == '\0'))
        {
            item = 'b';
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
            case 'a':               // H-Bridge A current sense
                adc_value = GetAnalogValue(TK200G1_C_SENS_A_PIN);
                dec_digs = 1;
                break;
            case 'b':               // H-Bridge B current sense
                adc_value = GetAnalogValue(TK200G1_C_SENS_B_PIN);
                dec_digs = 1;
                break;
            case 'o':               // OUTn item
                // enable CM output on L99DZ200G CM_DIR pin
                L99dz200g.L99DZ200G_CM_DIR_Config(CM_ALWAYS);
                pinMode(L99DZ200G_DIR_PIN, INPUT);

                L99dz200g.L99DZ200G_CM_OUTn_Select(val - 1);
                adc_value = GetAnalogValue(L99DZ200G_DIR_PIN);
                if (adc_value < 5)
                {
                    dec_digs = 1;
                }

                // re-enable DIR input on L99DZ200G CM_DIR pin
                L99dz200g.L99DZ200G_CM_DIR_Config(DIR_ALWAYS);
                pinMode(L99DZ200G_DIR_PIN, OUTPUT);
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
                    case 'a':               // A current sense
                    case 'b':               // B current sense
                        working = (bit_val * adc_value) * 1000;     // mV
                        working = working / (RSENSE_VAL * CSENSE_GAIN);
                        Serial.print(working, dec_digs);
                        Serial.println(F(" mA"));
                        break;
                    case 'o':               // OUTn values
                    default:
                        // get RDSon register values
                        reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR9);
                        switch (output)
                        {
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
                                case OUT_7:     // 1/10000
                                    working = bit_val * adc_value * 2150;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                                case OUT_8:     // 1/6500
                                    working = bit_val * adc_value * 1360;   // constant by measurement
                                    Serial.print(working, dec_digs);
                                    break;
                            }
                        }
                        else    // high rdson
                        {
                            switch (output)
                            {
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

/*
 * NAME:
 *  int8_t Cmd_out(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "out" command to show/set OUT7 or OUT8 output control.
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
 *     "out 8"       - show OUT8 output state
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
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR5);
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
    }

    // show OUTn states
    Serial.print(F("OUT"));
    Serial.print(output);
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
 *        1   2  3
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

#ifdef SHOW_HBRIDGE
/*
 * NAME:
 *  int8_t Cmd_hb(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "hb" command to show/set L99DZ200G H-Bridge control.
 *
 *  Multiple optional parameters supported (one at a time).
 *   <on> = switch specified L99DZ200G H-Bridge control on
 *      -or-
 *   <off> = switch specified L99DZ200G H-Bridge control off
 *
 *      -or-
 *
 *   <clrds> = clear Drain-Source monitoring status for specified L99DZ200G H-bridge
 *
 *      -or-
 *
 *   <ccpt> = set cross current protection time for specified L99DZ200G H-bridge
 *      -and-
 *   <Xns> = set cross current protection time (nS)
 *
 *      -or-
 *
 *   <dsth> = set Drain-Source threshold of specified L99DZ200G H-bridge
 *      -and-
 *   <thr> = set Drain-Source threshold voltage (mV)
 *
 *      -or-
 *
 *   <dmside> = set dual motor mode drive side for specified L99DZ200G H-bridge and leg
 *      -and-
 *   <leg> = set dual motor mode drive side leg (1 or 2)
 *      -and-
 *   <side> = set dual motor mode drive side (L or H)
 *
 *      -or-
 *
 *   <dmtype> = set dual motor mode freewheeling type for specified L99DZ200G H-bridge and leg
 *      -and-
 *   <leg>  = set dual motor mode drive freewheeling type leg (1 or 2)
 *      -and-
 *   <act_pass> = set dual motor mode freewheeling type (A or P) {active or passive}
 *
 *      -or-
 *
 *   <olht> = set Open-Load high threshold for specified L99DZ200G H-bridge
 *      -and-
 *   <on>  = set Open-Load high threshold on
 *      -or-
 *   <off> = set Open-Load high threshold off
 *
 *      -or-
 *
 *   <smdir> = set single motor mode Direction of specified L99DZ200G H-bridge
 *      -and-
 *   <L>  = set single motor mode Direction left
 *      -or-
 *   <R> = set single motor mode Direction right
 *
 *      -or-
 *
 *   <smside> = set single motor mode freewheeling side of specified L99DZ200G H-bridge
 *      -and-
 *   <L>  = set single motor mode freewheeling side to low side
 *      -or-
 *   <H> = set single motor mode freewheeling side to high side
 *
 *      -or-
 *
 *   <smtype> = set single motor mode freewheeling type of specified L99DZ200G H-bridge
 *      -and-
 *   <act_pass> = set single motor mode freewheeling type (A or P) {active or passive}
 *
 *      -or-
 *
 *   <slew> = set slew rate current percentage for specified L99DZ200G H-bridge
 *      -and-
 *   <percent> = set slew rate current percentage (0 to 100%)
 *
 *      -or-
 *
 *   <oltst> = test open-load condition between H1 and L2 or H2 and L1 of specified L99DZ200G H-bridge
 *      -and-
 *   <legs>  = set H-bridge testing legs (h1l2 or h2l1)
 *      -and-
 *   <on>  = enable open-load H-bridge testing
 *      -or-
 *   <off> = disable open-load H-bridge testing
 *
 *      -or-
 *
 *   <mode> = set motor mode of specified L99DZ200G H-bridge
 *      -and-
 *   <sng>  = set single motor mode for H-bridge
 *      -or-
 *   <dual> = set dual motor mode for  H-bridge
 *
 *      1  2  3    4   5
 *     "hb A"                - show L99DZ200G H-Bridge A settings
 *     "hb A clrds"          - clear L99DZ200G H-Bridge A Drain-Source monitoring status
 *     "hb A ccpt 1250ns"    - set L99DZ200G H-Bridge A cross current protection time to 1250 nS
 *     "hb A dsth 750mv"     - set L99DZ200G H-Bridge A Drain-Source threshold voltage to 750 mV
 *     "hb A dmside 1 L"     - set L99DZ200G H-Bridge A dual motor mode drive side leg 1 low side
 *     "hb A dmtype 2 P"     - set L99DZ200G H-Bridge A dual motor mode freewheeling type leg 2 passive
 *     "hb B olht on"        - set L99DZ200G H-Bridge B Open-Load high threshold on
 *     "hb B smdir L"        - set L99DZ200G H-Bridge B single motor mode Direction left
 *     "hb B smside H"       - set L99DZ200G H-Bridge B single motor mode freewheeling side to high side
 *     "hb B smtype A"       - set L99DZ200G H-Bridge B single motor mode freewheeling to active
 *     "hb B slew 50"        - set L99DZ200G H-Bridge B slew rate current percentage to 50%
 *     "hb B oltst h2l1 on"  - enable test open-load condition between H2 and L1
 *     "hb B mode dual"      - set L99DZ200G H-Bridge B to dual motor mode
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_hb(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t hbridge = H_BRIDGE_CONTROL_A;
    uint8_t on_off = 99;
    uint8_t en_dis = 99;
    uint8_t clrds = 99;
    uint8_t ccpt = 99;
    uint8_t dsth = 99;
    uint8_t dmside = 99;
    uint8_t leg = 99;
    uint8_t dmtype = 99;
    uint8_t olht = 99;
    uint8_t smdir = 99;
    uint8_t smside = 99;
    uint8_t smtype = 99;
    uint8_t mode = 99;
    uint8_t oltst = 99;
    uint8_t pwm = 255;
    uint8_t slew = 255;

    if (argc > 5)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowHBridgeSettings(H_BRIDGE_CONTROL_A);
        ShowHBridgeSettings(H_BRIDGE_CONTROL_B);
    }
    else
    {
DEBUG_TOGL();
        if (argv[ARG1][1] == '\0')
        {
            // get H-Bridge ID
            if (tolower(argv[ARG1][0]) == 'a')
            {
                hbridge = H_BRIDGE_CONTROL_A;
            }
            else if (tolower(argv[ARG1][0]) == 'b')
            {
                hbridge = H_BRIDGE_CONTROL_B;
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

        if (argc > 2)
        {
            // get the H-Bridge on/off control
            if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                if (argc > 3)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else
                {
                    en_dis = ENABLE;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                if (argc > 3)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else
                {
                    en_dis = DISABLE;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("clrds")) == 0)
            {
                if (argc > 3)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else
                {
                    clrds = ENABLE;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("ccpt")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    if (strcmp_P(argv[ARG3], PSTR("250ns")) == 0)
                    {
                        ccpt = T_250ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("500ns")) == 0)
                    {
                        ccpt = T_500ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("750ns")) == 0)
                    {
                        ccpt = T_750ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1000ns")) == 0)
                    {
                        ccpt = T_1000ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1250ns")) == 0)
                    {
                        ccpt = T_1250ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1500ns")) == 0)
                    {
                        ccpt = T_1500ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1750ns")) == 0)
                    {
                        ccpt = T_1750ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("2000ns")) == 0)
                    {
                        ccpt = T_2000ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("2250ns")) == 0)
                    {
                        ccpt = T_2250ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("2500ns")) == 0)
                    {
                        ccpt = T_2500ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("2750ns")) == 0)
                    {
                        ccpt = T_2750ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("3000ns")) == 0)
                    {
                        ccpt = T_3000ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("3250ns")) == 0)
                    {
                        ccpt = T_3250ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("3500ns")) == 0)
                    {
                        ccpt = T_3500ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("3750ns")) == 0)
                    {
                        ccpt = T_3750ns;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("4000ns")) == 0)
                    {
                        ccpt = T_4000ns;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("dsth")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    if (strcmp_P(argv[ARG3], PSTR("500mv")) == 0)
                    {
                        dsth = DS_500mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("750mv")) == 0)
                    {
                        dsth = DS_750mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1000mv")) == 0)
                    {
                        dsth = DS_1000mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1250mv")) == 0)
                    {
                        dsth = DS_1250mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1500mv")) == 0)
                    {
                        dsth = DS_1500mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("1750mv")) == 0)
                    {
                        dsth = DS_1750mV;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("2000mv")) == 0)
                    {
                        dsth = DS_2000mV;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("dmside")) == 0)
            {
                if (argc > 3)
                {
                    if (argv[ARG3][1] == '\0')
                    {
                        // get H-Bridge leg
                        if (argv[ARG3][0] == '1')
                        {
                            leg = HB_LEG1;
                        }
                        else if (argv[ARG3][0] == '2')
                        {
                            leg = HB_LEG2;
                        }
                        else
                        {
                            return CMDLINE_INVALID_ARG;
                        }

                        if (argc > 4)
                        {
                            if (argv[ARG4][1] == '\0')
                            {
                                // get H-Bridge side (low/high)
                                if (toupper(argv[ARG4][0]) == 'L')
                                {
                                    dmside = LOW_SIDE_DRIVE;
                                }
                                else if (toupper(argv[ARG4][0]) == 'H')
                                {
                                    dmside = HIGH_SIDE_DRIVE;
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
                        else
                        {
                            return CMDLINE_TOO_FEW_ARGS;
                        }
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("dmtype")) == 0)
            {
                if (argc > 3)
                {
                    if (argv[ARG3][1] == '\0')
                    {
                        // get H-Bridge leg
                        if (argv[ARG3][0] == '1')
                        {
                            leg = HB_LEG1;
                        }
                        else if (argv[ARG3][0] == '2')
                        {
                            leg = HB_LEG2;
                        }
                        else
                        {
                            return CMDLINE_INVALID_ARG;
                        }

                        if (argc > 4)
                        {
                            if (argv[ARG4][1] == '\0')
                            {
                                // get H-Bridge freewheeling type (active/passive)
                                if (toupper(argv[ARG4][0]) == 'A')
                                {
                                    dmtype = ACTIVE_FREEWHEELING;
                                }
                                else if (toupper(argv[ARG4][0])  == 'P')
                                {
                                    dmtype = PASSIVE_FREEWHEELING;
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
                        else
                        {
                            return CMDLINE_TOO_FEW_ARGS;
                        }
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("olht")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    // get the H-Bridge on/off control
                    if (strcmp_P(argv[ARG3], PSTR("on")) == 0)
                    {
                        olht = ENABLE;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("off")) == 0)
                    {
                        olht = DISABLE;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("pwm")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    // get the H-Bridge pwm duty cycle percentage
                    paramtype = CmdLine.ParseParam(argv[ARG3], &val);
                    if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                        (val < 0) || (val > 100))
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    pwm = val;
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("smdir")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    // get the H-Bridge direction control
                    if (argv[ARG3][1] == '\0')
                    {
                        // get H-Bridge direction
                        if (toupper(argv[ARG3][0]) == 'L')
                        {
                            smdir = LEFT_DIRECTION;
                        }
                        else if (toupper(argv[ARG3][0]) == 'R')
                        {
                            smdir = RIGHT_DIRECTION;
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
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("smside")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    if (argv[ARG3][1] == '\0')
                    {
                        // get H-Bridge freewheeling side (low/high)
                        if (toupper(argv[ARG3][0]) == 'L')
                        {
                            smside = LOW_SIDE_FREEWHEELING;
                        }
                        else if (toupper(argv[ARG3][0]) == 'H')
                        {
                            smside = HIGH_SIDE_FREEWHEELING;
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
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("smtype")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    if (argv[ARG3][1] == '\0')
                    {
                        // get H-Bridge freewheeling type (active/passive)
                        if (toupper(argv[ARG3][0]) == 'A')
                        {
                            smtype = ACTIVE_FREEWHEELING;
                        }
                        else if (toupper(argv[ARG3][0]) == 'P')
                        {
                            smtype = PASSIVE_FREEWHEELING;
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
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("mode")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    // get the H-Bridge single/dual mode control
                    if (strcmp_P(argv[ARG3], PSTR("sng")) == 0)
                    {
                        mode = SINGLE_MOTOR;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("dual")) == 0)
                    {
                        mode = DUAL_MOTOR;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("oltst")) == 0)
            {
                if (argc > 3)
                {
                    // get the H-Bridge open load test control
                    if (strcmp_P(argv[ARG3], PSTR("h1l2")) == 0)
                    {
                        oltst = OL_H1L2;
                    }
                    else if (strcmp_P(argv[ARG3], PSTR("h2l1")) == 0)
                    {
                        oltst = OL_H2L1;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    if (argc > 4)
                    {
                        // get the H-Bridge open load test on/off control
                        if (strcmp_P(argv[ARG4], PSTR("on")) == 0)
                        {
                            on_off = ENABLE;
                        }
                        else if (strcmp_P(argv[ARG4], PSTR("off")) == 0)
                        {
                            on_off = DISABLE;
                        }
                        else
                        {
                            return CMDLINE_INVALID_ARG;
                        }
                    }
                    else
                    {
                        return CMDLINE_TOO_FEW_ARGS;
                    }
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else if (strcmp_P(argv[ARG2], PSTR("slew")) == 0)
            {
                if (argc > 4)
                {
                    return CMDLINE_TOO_MANY_ARGS;
                }
                else if (argc > 3)
                {
                    // get the H-Bridge slew rate current percentage
                    paramtype = CmdLine.ParseParam(argv[ARG3], &val);
                    if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                        (val < 0) || (val > 100))
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    slew = val;
                }
                else
                {
                    return CMDLINE_TOO_FEW_ARGS;
                }
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }

        if (en_dis != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_Control(hbridge, en_dis);
        }
        else if (clrds != 99)
        {
            L99dz200g.L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(hbridge);
        }
        else if (ccpt != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_CrossCurrentProtectionTime(hbridge, ccpt);
        }
        else if (dsth != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_DSMonitorThreshold(hbridge, dsth);
        }
        else if (dmside != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(hbridge, leg, dmside);
        }
        else if (dmtype != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(hbridge, leg, dmtype);
        }
        else if (olht != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_OL_HighThreshold(hbridge, olht);
        }
        else if (pwm != 255)
        {
            SetHBridgePwmSetting(hbridge, pwm);
        }
        else if (smdir != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_SingleMotorDirectionControl(hbridge, smdir);
        }
        else if (smside != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingSide(hbridge, smside);
        }
        else if (smtype != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingType(hbridge, smtype);
        }
        else if (mode != 99)
        {
            L99dz200g.L99DZ200G_Set_HB_MotorModeControl(hbridge, mode);
        }
        else if (oltst != 99)
        {
            L99dz200g.L99DZ200G_Test_HB_OL_HxandLy(hbridge, on_off, oltst);
        }
        else if (slew != 255)
        {
            L99dz200g.L99DZ200G_Set_HB_SlewRateCurrent(hbridge, slew);
        }
DEBUG_TOGL();

        ShowHBridgeSettings(hbridge);
    }

    // Return success.
    return 0;
}

// set H-Bridge PWM control
void SetHBridgePwmSetting(uint8_t hbridge, uint8_t pwm)
{
    uint8_t pwm_dc = 0;

    pwm_dc = map(pwm, 0, 100, 0, 255);
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            HBridgePWM[hbridge * 2] = pwm;
            HBridgePWM[(hbridge * 2) + 1] = pwm;
            if (pwm == 0)
            {
                digitalWrite(L99DZ200G_PWMH1A_PIN, LOW);
                digitalWrite(L99DZ200G_PWMH2A_PIN, LOW);
            }
            else if (pwm == 100)
            {
                digitalWrite(L99DZ200G_PWMH1A_PIN, HIGH);
                digitalWrite(L99DZ200G_PWMH2A_PIN, HIGH);
            }
            else
            {
                analogWrite(L99DZ200G_PWMH1A_PIN, pwm_dc);
                analogWrite(L99DZ200G_PWMH2A_PIN, pwm_dc);
            }
            break;
        case H_BRIDGE_CONTROL_B:
            HBridgePWM[hbridge * 2] = pwm;
            HBridgePWM[(hbridge * 2) + 1] = pwm;
            if (pwm == 0)
            {
                digitalWrite(L99DZ200G_PWMH1B_PIN, LOW);
                digitalWrite(L99DZ200G_PWMH2B_PIN, LOW);
            }
            else if (pwm == 100)
            {
                digitalWrite(L99DZ200G_PWMH1B_PIN, HIGH);
                digitalWrite(L99DZ200G_PWMH2B_PIN, HIGH);
            }
            else
            {
                analogWrite(L99DZ200G_PWMH1B_PIN, pwm_dc);
                analogWrite(L99DZ200G_PWMH2B_PIN, pwm_dc);
            }
            break;
    }
}

// get and show H-Bridge settings
void ShowHBridgeSettings(uint8_t hbridge)
{
    uint32_t reg_cr1;
    uint32_t reg_cr10;
    uint32_t reg_cr21 = 0;
    uint32_t reg_cfr;
    uint32_t reg_sr2 = 0;
    uint32_t reg_sr3 = 0;
    uint32_t reg = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR1
    reg_cr1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // get CFR
    reg_cfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // get CR10
    reg_cr10 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR10);

    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            // get SR2
            reg_sr2 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR2);
            break;
        case H_BRIDGE_CONTROL_B:
            // get SR3
            reg_sr3 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_SR3);
            // get CR21
            reg_cr21 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR21);
            break;
    }

    // show H-Bridge x settings
    Serial.print(F("H-Bridge "));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: CR10: "));
            Print0xHex24ln(reg_cr10);
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: CR21: "));
            Print0xHex24ln(reg_cr21);
            break;
    }

    // show H-Bridge enable state
    Serial.print(F(" HEN"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print('A');
            reg_cr1 = (reg_cr1 & CR1_HENA_MASK) >> CR1_HENA_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print('B');
            reg_cr1 = (reg_cr1 & CR1_HENB_MASK) >> CR1_HENB_POS;
            break;
    }
    Serial.print(F(": "));
    ShowEnabledDisabled(reg_cr1, true);

    // show H-Bridge motor mode
    Serial.print(F(" DM"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print('A');
            reg_cfr = (reg_cfr & CFR_DM_HB_A_MASK) >> CFR_DM_HB_A_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print('B');
            reg_cfr = (reg_cfr & CFR_DM_HB_B_MASK) >> CFR_DM_HB_B_POS;
            break;
    }
    Serial.print(F(": "));
    Serial.print(reg_cfr ? "Dual" : "Single");
    Serial.println();

    // show H-Bridge DS_MON_x state
    Serial.print(F(" DS_MON_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            // get SR2
            reg_sr2 &= (SR2_DS_MON_LS1_A | SR2_DS_MON_LS2_A | SR2_DS_MON_HS1_A | SR2_DS_MON_HS2_A);
            reg_sr2 >>= SR2_DS_MON_LS1_A_POS;
            Serial.print(F("A: "));
            Serial.print(reg_sr2, HEX);
            break;
        case H_BRIDGE_CONTROL_B:
            // get SR3
            reg_sr3 &= (SR3_DS_MON_LS1_B | SR3_DS_MON_LS2_B | SR3_DS_MON_HS1_B | SR3_DS_MON_HS2_B);
            reg_sr3 >>= SR3_DS_MON_LS1_B_POS;
            Serial.print(F("B: "));
            Serial.print(reg_sr3, HEX);
            break;
    }
    Serial.println();

    // show H-Bridge COPT_x state
    Serial.print(F(" COPT_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_CCPT_MASK) >> CR10_HB_CCPT_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_CCPT_MASK) >> CR21_HB_CCPT_POS;
            Serial.print(F("B: "));
            break;
    }
    reg = (reg * 250) + 250;
    Serial.print(reg);
    Serial.print(F(" nS"));
    Serial.println();

    // show H-Bridge DIAG_x state
    Serial.print(F(" DIAG_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_DS_DIAG_MASK) >> CR10_HB_DS_DIAG_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_DS_DIAG_MASK) >> CR21_HB_DS_DIAG_POS;
            Serial.print(F("B: "));
            break;
    }
    if (reg > 6)    // special case
    {
        reg = 6;
    }
    reg = (reg * 250) + 500;
    Serial.print(reg);
    Serial.print(F(" mV"));
    Serial.println();

    // show H-Bridge SD_x state
    Serial.print(F(" SD_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: SD1: "));
            Serial.print((reg_cr10 & CR10_HB_A_SD1_MASK) >> CR10_HB_A_SD1_POS);
            Serial.print(F(", SD2: "));
            Serial.print((reg_cr10 & CR10_HB_A_SD2_MASK) >> CR10_HB_A_SD2_POS);
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: SD1: "));
            Serial.print((reg_cr10 & CR10_HB_B_SD1_MASK) >> CR10_HB_B_SD1_POS);
            Serial.print(F(", SD2: "));
            Serial.print((reg_cr10 & CR10_HB_B_SD2_MASK) >> CR10_HB_B_SD2_POS);
            break;
    }
    Serial.println();

    // show H-Bridge SDS_x state
    Serial.print(F(" SDS_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: SDS1: "));
            Serial.print((reg_cr10 & CR10_HB_A_SDS1_MASK) >> CR10_HB_A_SDS1_POS);
            Serial.print(F(", SDS2: "));
            Serial.print((reg_cr10 & CR10_HB_A_SDS2_MASK) >> CR10_HB_A_SDS2_POS);
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: SDS1: "));
            Serial.print((reg_cr10 & CR10_HB_B_SDS1_MASK) >> CR10_HB_B_SDS1_POS);
            Serial.print(F(", SDS2: "));
            Serial.print((reg_cr10 & CR10_HB_B_SDS2_MASK) >> CR10_HB_B_SDS2_POS);
            break;
    }
    Serial.println();

    // show H-Bridge H_OLTH_HIGH_x state
    Serial.print(F(" H_OLTH_HIGH_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_OLTH_HI_MASK) >> CR10_HB_OLTH_HI_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_OLTH_HI_MASK) >> CR21_HB_OLTH_HI_POS;
            Serial.print(F("B: "));
            break;
    }
    Serial.print(reg ? '5' : '1');
    Serial.print(F("/6 VS"));
    Serial.println();

    // show H-Bridge DIRH_x state
    Serial.print(F(" DIRH"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_A_DIR_MASK) >> CR10_HB_A_DIR_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_B_DIR_MASK) >> CR21_HB_B_DIR_POS;
            Serial.print(F("B: "));
            break;
    }
    Serial.print(reg ? "Right" : "Left");
    Serial.println();

    // show H-Bridge OL_H1L2_x and OL_H2L1_x states
    Serial.print(F(" OL_H1L2_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: "));
            ShowEnabledDisabled(((reg_cr10 & CR10_HB_OLH1L2_MASK) >> CR10_HB_OLH1L2_POS), true);
            Serial.print(F(" OL_H2L1_A: "));
            ShowEnabledDisabled(((reg_cr10 & CR10_HB_OLH2L1_MASK) >> CR10_HB_OLH2L1_POS), true);
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: "));
            ShowEnabledDisabled(((reg_cr21 & CR21_HB_OLH1L2_MASK) >> CR21_HB_OLH1L2_POS), true);
            Serial.print(F(" OL_H2L1_B: "));
            ShowEnabledDisabled(((reg_cr21 & CR21_HB_OLH2L1_MASK) >> CR21_HB_OLH2L1_POS), true);
            break;
    }

    // show H-Bridge SLEW_x state
    Serial.print(F(" SLEW_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_SLEW_MASK) >> CR10_HB_SLEW_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_SLEW_MASK) >> CR21_HB_SLEW_POS;
            Serial.print(F("B: "));
            break;
    }
    if (reg == 0)
    {
        ShowOnOff(reg);
    }
    else
    {
        Serial.print(((reg + 1) * 100) / SLEW_MAX);
        Serial.println(F("%"));
    }

    // show H-Bridge PWM duty cycle
    Serial.print(F(" PWM_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: "));
            break;
    }
    Serial.print(HBridgePWM[hbridge * 2]);
    Serial.print(F("%, "));
    Serial.print(HBridgePWM[(hbridge * 2) + 1]);
    Serial.println(F("%"));
}

/*
 * NAME:
 *  int8_t Cmd_hbm(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "hb" command to show/set L99DZ200G H-Bridge motor control.
 *
 *  Two optional parameters supported.
 *   <percent> = L99DZ200G H-Bridge single motor mode motor speed PWM percent control
 *      -and-
 *   <L>  = set single motor mode Direction left
 *      -or-
 *   <R> = set single motor mode Direction right
 *
 *      1   2  3 4
 *     "hbm A"          - show L99DZ200G H-Bridge A motor control settings
 *     "hbm A 50"       - set L99DZ200G H-Bridge A motor control to 50% speed
 *     "hbm A 0"        - set L99DZ200G H-Bridge A motor control to 0% speed (stopped)
 *     "hbm B 25 L"     - set L99DZ200G H-Bridge B motor control to 25% speed, left direction
 *     "hbm A 75 R"     - set L99DZ200G H-Bridge A motor control to 75% speed, right direction
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  Assumes the specified L99DZ200G H-Bridge has been configured single motor mode.
 */
int8_t Cmd_hbm(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t hbridge = H_BRIDGE_CONTROL_A;
    uint8_t speed = 255;
    uint8_t smdir = 255;
    uint32_t reg = 0;

    if (argc > 4)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowHBridgeMotorSettings(H_BRIDGE_CONTROL_A);
        ShowHBridgeMotorSettings(H_BRIDGE_CONTROL_B);
    }
    else
    {
DEBUG_TOGL();
        if (argv[ARG1][1] == '\0')
        {
            // get H-Bridge ID
            if (tolower(argv[ARG1][0]) == 'a')
            {
                hbridge = H_BRIDGE_CONTROL_A;
            }
            else if (tolower(argv[ARG1][0]) == 'b')
            {
                hbridge = H_BRIDGE_CONTROL_B;
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

        if (argc > 2)
        {
            // get the H-Bridge motor control speed percentage
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 0) || (val > 100))
            {
                return CMDLINE_INVALID_ARG;
            }

            speed = val;

            if (argc > 3)
            {
                // get the H-Bridge direction control
                if (argv[ARG3][1] == '\0')
                {
                    // get H-Bridge direction
                    if (toupper(argv[ARG3][0]) == 'L')
                    {
                        smdir = LEFT_DIRECTION;
                    }
                    else if (toupper(argv[ARG3][0]) == 'R')
                    {
                        smdir = RIGHT_DIRECTION;
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

            SetHBridgePwmSetting(hbridge, speed);
            if (speed == 0)
            {
                L99dz200g.L99DZ200G_Set_HB_SingleMotorStop(hbridge);
            }
            else
            {
                if (smdir == 255)
                {
                    // get current direction setting
                    switch (hbridge)
                    {
                        case H_BRIDGE_CONTROL_A:
                            // get CR10
                            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR10);
                            reg = (reg & CR10_HB_A_DIR_MASK) >> CR10_HB_A_DIR_POS;
                            Serial.print(F("A: "));
                            break;
                        case H_BRIDGE_CONTROL_B:
                            // get CR21
                            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR21);
                            reg = (reg & CR21_HB_B_DIR_MASK) >> CR21_HB_B_DIR_POS;
                            Serial.print(F("B: "));
                            break;
                    }
                    if (reg)
                    {
                        smdir = RIGHT_DIRECTION;
                    }
                    else
                    {
                        smdir = LEFT_DIRECTION;
                    }
                }
                if (smdir == LEFT_DIRECTION)
                {
                    L99dz200g.L99DZ200G_Set_HB_SingleMotorCounterClockwise(hbridge);
                }
                else    // RIGHT_DIRECTION
                {
                    L99dz200g.L99DZ200G_Set_HB_SingleMotorClockwise(hbridge);
                }
            }

        }
DEBUG_TOGL();

        ShowHBridgeMotorSettings(hbridge);
    }

    // Return success.
    return 0;
}

// get and show H-Bridge motor settings
void ShowHBridgeMotorSettings(uint8_t hbridge)
{
    uint32_t reg_cr1;
    uint32_t reg_cr10 = 0;
    uint32_t reg_cr21 = 0;
    uint32_t reg = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR1
    reg_cr1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            // get CR10
            reg_cr10 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR10);
            break;
        case H_BRIDGE_CONTROL_B:
            // get CR21
            reg_cr21 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR21);
            break;
    }

    // show H-Bridge x settings
    Serial.print(F("H-Bridge "));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: CR10: "));
            Print0xHex24ln(reg_cr10);
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: CR21: "));
            Print0xHex24ln(reg_cr21);
            break;
    }

    // show H-Bridge enable state
    Serial.print(F(" HEN"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print('A');
            reg_cr1 = (reg_cr1 & CR1_HENA_MASK) >> CR1_HENA_POS;
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print('B');
            reg_cr1 = (reg_cr1 & CR1_HENB_MASK) >> CR1_HENB_POS;
            break;
    }
    Serial.print(F(": "));
    ShowEnabledDisabled(reg_cr1, true);

    // show H-Bridge DIRH_x state
    Serial.print(F(" DIRH"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            reg = (reg_cr10 & CR10_HB_A_DIR_MASK) >> CR10_HB_A_DIR_POS;
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            reg = (reg_cr21 & CR21_HB_B_DIR_MASK) >> CR21_HB_B_DIR_POS;
            Serial.print(F("B: "));
            break;
    }
    Serial.print(reg ? "Right" : "Left");
    Serial.println();

    // show H-Bridge PWM duty cycle
    Serial.print(F(" PWM_"));
    switch (hbridge)
    {
        case H_BRIDGE_CONTROL_A:
            Serial.print(F("A: "));
            break;
        case H_BRIDGE_CONTROL_B:
            Serial.print(F("B: "));
            break;
    }
    Serial.print(HBridgePWM[hbridge * 2]);
    Serial.print(F("%, "));
    Serial.print(HBridgePWM[(hbridge * 2) + 1]);
    Serial.println(F("%"));
}
#endif

#ifdef SHOW_TRUNK
/*
 * NAME:
 *  int8_t Cmd_trunk(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "trunk" command to do TK200G1 trunk control.
 *
 *  Seven parameters supported.
 *   <open> = open trunk liftgate
 *      -or-
 *   <close> = close trunk liftgate
 *      -or-
 *   <lock> = lock trunk liftgate
 *      -or-
 *   <unlock> = unlock trunk liftgate
 *      -or-
 *   <stop> = stop trunk liftgate open/close or lock/unlock
 *      -or-
 *   <test> = run trunk liftgate test operations
 *      -or-
 *   <delay> = trunk liftgate open/close or lock/unlock operational delay (mS)
 *      -and-
 *     val   = delay value (mS)
 *
 *       1    2     3
 *     "trunk open"         - open trunk liftgate
 *     "trunk close"        - close trunk liftgate
 *     "trunk lock"         - lock trunk liftgate
 *     "trunk unlock"       - unlock trunk liftgate
 *     "trunk stop"         - stop trunk liftgate open/close or lock/unlock
 *     "trunk test"         - run trunk liftgate test operations
 *     "trunk delay 1000"   - trunk liftgate operational delay of 1000 mS
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_trunk(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t cmd = TRUNK_STOP;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("delay")) == 0)
        {
            // get the operating delay
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 0) || (val > 65000))
            {
                return CMDLINE_INVALID_ARG;
            }
            TrunkDelay = val;
        }
        else if (argc > 2)
        {
            return CMDLINE_TOO_MANY_ARGS;
        }
        else
        {
            if (strcmp_P(argv[ARG1], PSTR("open")) == 0)
            {
                cmd = TRUNK_OPEN;
            }
            else if (strcmp_P(argv[ARG1], PSTR("close")) == 0)
            {
                cmd = TRUNK_CLOSE;
            }
            else if (strcmp_P(argv[ARG1], PSTR("lock")) == 0)
            {
                cmd = TRUNK_LOCK;
            }
            else if (strcmp_P(argv[ARG1], PSTR("unlock")) == 0)
            {
                cmd = TRUNK_UNLOCK;
            }
            else if (strcmp_P(argv[ARG1], PSTR("stop")) == 0)
            {
                cmd = TRUNK_STOP;
            }
            else if (strcmp_P(argv[ARG1], PSTR("test")) == 0)
            {
                cmd = TRUNK_TEST;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            DoTrunkOperation(cmd);
        }
    }

    ShowTrunkState();

    // Return success.
    return 0;
}

void DoTrunkOperation(uint8_t cmd)
{
    TrunkState = cmd;

    TrunkTesting = false;

    switch (cmd)
    {
        case TRUNK_OPEN:
            TrunkLightsControl(ON_OUT);
            TrunkLiftOpenWithDelay(TrunkPwmDutyCycle, TrunkDelay);
            if (TrunkDelay != 0)
            {
                TrunkLightsControl(OFF_OUT);
            }
            break;
        case TRUNK_CLOSE:
            TrunkLightsControl(ON_OUT);
            TrunkLiftCloseWithDelay(TrunkPwmDutyCycle, TrunkDelay);
            if (TrunkDelay != 0)
            {
                TrunkLightsControl(OFF_OUT);
            }
            break;
        case TRUNK_LOCK:
            TrunkLightsControl(ON_OUT);
            TrunkLockWithDelay(TrunkPwmDutyCycle, TrunkDelay);
            if (TrunkDelay != 0)
            {
                TrunkLightsControl(OFF_OUT);
            }
            break;
        case TRUNK_UNLOCK:
            TrunkLightsControl(ON_OUT);
            TrunkUnlockWithDelay(TrunkPwmDutyCycle, TrunkDelay);
            if (TrunkDelay != 0)
            {
                TrunkLightsControl(OFF_OUT);
            }
            break;
        case TRUNK_STOP:
            TrunkLiftStop();
            TrunkLockStop();
            TrunkLightsControl(OFF_OUT);
            break;
        case TRUNK_TEST:
            TrunkTesting = true;
            TrunkTesState = TRUNK_UNLOCK;
            break;
    }

    if ((TrunkDelay != 0) && !TrunkTesting)
    {
        TrunkState = TRUNK_STOP;
    }
}

void ShowTrunkState(void)
{
    Serial.print(F("TrunkState: TRUNK_"));
    switch (TrunkState)
    {
        case TRUNK_OPEN:
            Serial.println(F("OPEN"));
            break;
        case TRUNK_CLOSE:
            Serial.println(F("CLOSE"));
            break;
        case TRUNK_LOCK:
            Serial.println(F("LOCK"));
            break;
        case TRUNK_UNLOCK:
            Serial.println(F("UNLOCK"));
            break;
        case TRUNK_STOP:
            Serial.println(F("STOP"));
            break;
        case TRUNK_TEST:
            Serial.println(F("TEST"));
            break;
    }
}
#endif




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

#if 1   // Support routines
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
#endif

