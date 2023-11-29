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

// Defines for code to be included (will not all fit in smaller MCUs)

#define SHOW_RROM
#define SHOW_ADC        //
#define SHOW_CAN
#define SHOW_CCM
#define SHOW_CPUMP      //
#define SHOW_ECV        //
#define SHOW_HEATER     //
//#define SHOW_ICMP
//#define SHOW_LIN
//#define SHOW_MASK
//#define SHOW_MISC
#define SHOW_MODE       //
#define SHOW_MOTOR      //
#define SHOW_OCR        //
#define SHOW_OCT        //
#define SHOW_OLT        //
#define SHOW_PWM        //
#define SHOW_RDSON      //
#define SHOW_SCT        //
#define SHOW_STAT       //
//#define SHOW_STAT_DETAIL
#define SHOW_TEMP       //
#define SHOW_TIMER      //
#define SHOW_TNINT
#define SHOW_V1RESET
#define SHOW_V2         //
#define SHOW_VLED
#define SHOW_VOLT       //
//#define SHOW_VS
//#define SHOW_VSR
//#define SHOW_WAKEUP
//#define SHOW_WDOG
//#define SHOW_HBRIDGE

// local function prototypes
int8_t Cmd_help(int8_t argc, char * argv[]);
int8_t Cmd_rreg(int8_t argc, char * argv[]);
int8_t Cmd_wreg(int8_t argc, char * argv[]);
#ifdef SHOW_RROM
int8_t Cmd_rrom(int8_t argc, char * argv[]);
#endif
int8_t Cmd_rclr(int8_t argc, char * argv[]);

#ifdef SHOW_ADC
int8_t Cmd_adc(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_CAN
int8_t Cmd_can(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_CCM
int8_t Cmd_ccm(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_CPUMP
int8_t Cmd_cp(int8_t argc, char * argv[]);
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
#ifdef SHOW_ICMP
int8_t Cmd_icmp(int8_t argc, char * argv[]);
#endif
int8_t Cmd_init(int8_t argc, char * argv[]);
#ifdef SHOW_LIN
int8_t Cmd_lin(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_MASK
int8_t Cmd_mask(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_MISC
int8_t Cmd_misc(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_MODE
int8_t Cmd_mode(int8_t argc, char * argv[]);
#endif
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
#ifdef SHOW_TEMP
int8_t Cmd_temp(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_TIMER
int8_t Cmd_tmr(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_TNINT
int8_t Cmd_tnint(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_V1RESET
int8_t Cmd_v1t(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_V2
int8_t Cmd_v2(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_VLED
int8_t Cmd_vled(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_VOLT
int8_t Cmd_volt(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_VS
int8_t Cmd_vs(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_VSR
int8_t Cmd_vsr(int8_t argc, char * argv[]);
#endif
int8_t Cmd_wake(int8_t argc, char * argv[]);
#ifdef SHOW_WDOG
int8_t Cmd_wd(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_WAKEUP
int8_t Cmd_wu(int8_t argc, char * argv[]);
#endif
#ifdef SHOW_HBRIDGE
int8_t Cmd_fso(int8_t argc, char * argv[]);
int8_t Cmd_hb(int8_t argc, char * argv[]);
int8_t Cmd_hbm(int8_t argc, char * argv[]);
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
#ifdef SHOW_RROM
const char MenuCmdRrom[] PROGMEM  = "rrom";
#endif
const char MenuCmdRclr[] PROGMEM  = "rclr";
#ifdef SHOW_ADC
const char MenuCmdAdc[] PROGMEM   = "adc";
#endif
#ifdef SHOW_CAN
const char MenuCmdCan[] PROGMEM   = "can";
#endif
#ifdef SHOW_CCM
const char MenuCmdCcm[] PROGMEM   = "ccm";
#endif
#ifdef SHOW_CPUMP
const char MenuCmdCp[] PROGMEM    = "cp";
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
#ifdef SHOW_ICMP
const char MenuCmdIcmp[] PROGMEM  = "icmp";
#endif
const char MenuCmdInit[] PROGMEM  = "init";
#ifdef SHOW_LIN
const char MenuCmdLin[] PROGMEM   = "lin";
#endif
#ifdef SHOW_MASK
const char MenuCmdMask[] PROGMEM  = "mask";
#endif
#ifdef SHOW_MISC
const char MenuCmdMisc[] PROGMEM  = "misc";
#endif
#ifdef SHOW_MODE
const char MenuCmdMode[] PROGMEM  = "mode";
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
#ifdef SHOW_TEMP
const char MenuCmdTemp[] PROGMEM  = "temp";
#endif
#ifdef SHOW_TIMER
const char MenuCmdTmr[] PROGMEM   = "tmr";
#endif
#ifdef SHOW_TNINT
const char MenuCmdTnint[] PROGMEM = "tnint";
#endif
#ifdef SHOW_V1RESET
const char MenuCmdV1t[] PROGMEM   = "v1t";
#endif
#ifdef SHOW_V2
const char MenuCmdV2[] PROGMEM    = "v2";
#endif
#ifdef SHOW_VLED
const char MenuCmdVled[] PROGMEM  = "vled";
#endif
#ifdef SHOW_VOLT
const char MenuCmdVolt[] PROGMEM  = "volt";
#endif
#ifdef SHOW_VS
const char MenuCmdVs[] PROGMEM    = "vs";
#endif
#ifdef SHOW_VSR
const char MenuCmdVsr[] PROGMEM   = "vsr";
#endif
const char MenuCmdWake[] PROGMEM  = "wake";
#ifdef SHOW_WDOG
const char MenuCmdWd[] PROGMEM    = "wd";
#endif
#ifdef SHOW_WAKEUP
const char MenuCmdWu[] PROGMEM    = "wu";
#endif
#ifdef SHOW_HBRIDGE
const char MenuCmdFso[] PROGMEM   = "fso";
const char MenuCmdHb[] PROGMEM    = "hb";
const char MenuCmdHbm[] PROGMEM   = "hbm";
#endif

// menu items individual command help strings
const char MenuHelp1[] PROGMEM     =    " [<cls>]                     : Display list of commands (clear screen)";
const char MenuHelp2[] PROGMEM     = "                                : alias for help";
const char MenuHelpRreg[] PROGMEM  =    " reg [cnt]                   : Show L99DZ200G register(s) value(s)";
const char MenuHelpWreg[] PROGMEM  =    " [reg val] | <all>           : Set(reset all) L99DZ200G control register value(s)";
#ifdef SHOW_RROM
const char MenuHelpRrom[] PROGMEM  =    " addr [cnt]                  : Show L99DZ200G ROM address(es) value(s)";
#endif
const char MenuHelpRclr[] PROGMEM  =    " [reg val] | <all>           : Read/clear L99DZ200G status register(s)";
#ifdef SHOW_ADC
const char MenuHelpAdc[] PROGMEM   =   " [n | <A> | <B> | <x> | <y>] [<v> | <r>]  : Show analog input value (v = volts, r = raw)";
#endif
#ifdef SHOW_CAN
const char MenuHelpCan[] PROGMEM   =   " [[wuen | rxen | txen | toirq | gotx | loop | pnw] [on | off]] : Show[set] L99DZ200G CAN control";
#endif
#ifdef SHOW_CCM
const char MenuHelpCcm[] PROGMEM   =   " [n [off | on]]               : Show[set] L99DZ200G OUTn constant current mode control";
#endif
#ifdef SHOW_CPUMP
const char MenuHelpCp[] PROGMEM    =  " [[off | on] [ltch | live] [doff | don]] : Show[set] L99DZ200G charge pump control";
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
#ifdef SHOW_ICMP
const char MenuHelpIcmp[] PROGMEM  =    " [off | on]                  : Show[set] L99DZ200G V1 load current supervision (ICMP)";
#endif
const char MenuHelpInit[] PROGMEM  =    "                             : Init L99DZ200G";
#ifdef SHOW_LIN
const char MenuHelpLin[] PROGMEM   =   " [[[wuen | rec | txto] [on | off]] | [spd [lo | hi]] | [cfg [rdr | rdt]]] : Show[set] L99DZ200G LIN control";
#endif
#ifdef SHOW_MASK
const char MenuHelpMask[] PROGMEM  =    " [[spie | ple | hsol | lsol | ecol | ol | tw | gw | all] [y|n]]"
                                "\r\n                                 : Show[set] L99DZ200G status masked in GSB";
#endif
#ifdef SHOW_MISC
const char MenuHelpMisc[] PROGMEM  =    " [[gen | dbg | wd | tsd] [on | off]] : Show[set] L99DZ200G Gen/Debug/Watchdog/TSD control";
#endif
#ifdef SHOW_MODE
const char MenuHelpMode[] PROGMEM  =    " [v1 | vbat | act]           : Show[set] L99DZ200G standby mode";
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
const char MenuHelpStat[] PROGMEM  =    " [[can | ecv | hb | lin | ms | oc | ocr | ol | sc | spi | tsd | tw | vo | wd | wu] [clr]]"
                                "\r\n                                 : Show all or specific L99DZ200G status registers";
#endif
#ifdef SHOW_TEMP
const char MenuHelpTemp[] PROGMEM  =    "                             : Show L99DZ200G thermal clusters temperatures";
#endif
#ifdef SHOW_TIMER
const char MenuHelpTmr[] PROGMEM   =   " [n [per [ton [rst [dir]]]    : Show[set] L99DZ200G Timer settings";
#endif
#ifdef SHOW_TNINT
const char MenuHelpTnint[] PROGMEM =     " [[ten [on | off]] | [sel [t1 | t2]]] : Show[set] L99DZ200G Timer NINT control";
#endif
#ifdef SHOW_V1RESET
const char MenuHelpV1t[] PROGMEM   =   " [threshold]                  : Show[set] L99DZ200G V1 reset threshold";
#endif
#ifdef SHOW_V2
const char MenuHelpV2[] PROGMEM    =  " [[off | act | actv1 | on] [reg | trk]] : Show[set] L99DZ200G V2 regulator mode";
#endif
#ifdef SHOW_VLED
const char MenuHelpVled[] PROGMEM  =    " [n [vled] | [off | on]]     : Show[set] L99DZ200G OUTn Vs VLED Compensation control";
#endif
#ifdef SHOW_VOLT
const char MenuHelpVolt[] PROGMEM  =    "                             : Show L99DZ200G pins voltages";
#endif
#ifdef SHOW_VS
const char MenuHelpVs[] PROGMEM    =  " [[lck | ovsd | uvsd] [on | off]] : Show[set] L99DZ200G VS lockout/shutdown control";
#endif
#ifdef SHOW_VSR
const char MenuHelpVsr[] PROGMEM   =   " [[[lck | ovsd | uvsd] [on | off]] | [ewt thshld]] : Show[set] L99DZ200G VSreg lockout/shutdown control";
#endif
const char MenuHelpWake[] PROGMEM  =    "                             : Pulse Wakeup output pin";
#ifdef SHOW_WDOG
const char MenuHelpWd[] PROGMEM    =  " [time]                        : Show[set] L99DZ200G watchdog trigger time";
#endif
#ifdef SHOW_WAKEUP
const char MenuHelpWu[] PROGMEM    =  " [[en [on | off]] | [pin [pu | pd | wu | vbat]] | [filt [st | t1 | t2]]]"
                                "\r\n                                 : Show[set] L99DZ200G WU pin control";
#endif
#ifdef SHOW_HBRIDGE
const char MenuHelpFso[] PROGMEM   =   " [off | on]                   : Show[set] L99DZ200G LSx_FSO low-side outputs forced ON control";
const char MenuHelpHb[] PROGMEM    =  " [AB [on | off] | [mode  [sng | dual]] | [clrds] | [ccpt Xns] | [dsth thr] | [dmside leg [L | H]] |"
                                "\r\n   [dmtype leg [A | P]] | [olht [on | off]] | [pwm %] | [smdir [L | R]] | [smside [L | H]] |"
                                "\r\n   [smtype [A | P]] | [slew %] | [oltst [h1l2 | h2l1] [on | off]]] : Show[set] L99DZ200G H-Bridge control";
const char MenuHelpHbm[] PROGMEM   =   " [AB [spd% [L | R]]]          : Show[set] L99DZ200G H-Bridge motor control";
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
#ifdef SHOW_RROM
    { MenuCmdRrom,    Cmd_rrom,    MenuHelpRrom    },
#endif
    { MenuCmdRclr,    Cmd_rclr,    MenuHelpRclr    },
#ifdef SHOW_ADC
    { MenuCmdAdc,     Cmd_adc,     MenuHelpAdc     },
#endif
#ifdef SHOW_CAN
    { MenuCmdCan,     Cmd_can,     MenuHelpCan     },
#endif
#ifdef SHOW_CCM
    { MenuCmdCcm,     Cmd_ccm,     MenuHelpCcm     },
#endif
#ifdef SHOW_CPUMP
    { MenuCmdCp,      Cmd_cp,      MenuHelpCp      },
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
#ifdef SHOW_ICMP
    { MenuCmdIcmp,    Cmd_icmp,    MenuHelpIcmp    },
#endif
    { MenuCmdInit,    Cmd_init,    MenuHelpInit    },
#ifdef SHOW_LIN
    { MenuCmdLin,     Cmd_lin,     MenuHelpLin     },
#endif
#ifdef SHOW_MASK
    { MenuCmdMask,    Cmd_mask,    MenuHelpMask    },
#endif
#ifdef SHOW_MISC
    { MenuCmdMisc,    Cmd_misc,    MenuHelpMisc    },
#endif
#ifdef SHOW_MODE
    { MenuCmdMode,    Cmd_mode,    MenuHelpMode    },
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
#ifdef SHOW_TEMP
    { MenuCmdTemp,    Cmd_temp,    MenuHelpTemp    },
#endif
#ifdef SHOW_TIMER
    { MenuCmdTmr,     Cmd_tmr,     MenuHelpTmr     },
#endif
#ifdef SHOW_TNINT
    { MenuCmdTnint,   Cmd_tnint,   MenuHelpTnint   },
#endif
#ifdef SHOW_V1RESET
    { MenuCmdV1t,     Cmd_v1t,     MenuHelpV1t     },
#endif
#ifdef SHOW_V2
    { MenuCmdV2,      Cmd_v2,      MenuHelpV2      },
#endif
#ifdef SHOW_VLED
    { MenuCmdVled,    Cmd_vled,    MenuHelpVled    },
#endif
#ifdef SHOW_VOLT
    { MenuCmdVolt,    Cmd_volt,    MenuHelpVolt    },
#endif
#ifdef SHOW_VS
    { MenuCmdVs,      Cmd_vs,      MenuHelpVs      },
#endif
#ifdef SHOW_VSR
    { MenuCmdVsr,     Cmd_vsr,     MenuHelpVsr     },
#endif
    { MenuCmdWake,    Cmd_wake,    MenuHelpWake    },
#ifdef SHOW_WDOG
    { MenuCmdWd,      Cmd_wd,      MenuHelpWd      },
#endif
#ifdef SHOW_WAKEUP
    { MenuCmdWu,      Cmd_wu,      MenuHelpWu      },
#endif
#ifdef SHOW_HBRIDGE
    { MenuCmdFso,     Cmd_fso,     MenuHelpFso     },
    { MenuCmdHb,      Cmd_hb,      MenuHelpHb      },
    { MenuCmdHbm,     Cmd_hbm,     MenuHelpHbm     },
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

#ifdef SHOW_RROM
/*
 * NAME:
 *  int8_t Cmd_rrom(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "rrom" command to read and show an L99DZ200G Device Information Register address.
 *
 *  One required parameter supported.
 *   addr = the Device Information Registers address(es) to read from
 *  One optional parameter supported.
 *   cnt = the number of Device Information Registers address(es) to read from
 *
 *        1   2    3
 *     "rrom 0x10"      - read L99DZ200G Device Information Registers address 0x10
 *     "rrom 0x02 10"   - read L99DZ200G Device Information Registers addresses 0x02 to 0x0b
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_rrom(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t romaddr;
    uint8_t romcnt = 1;
    uint8_t romval;

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
        // get the Device Information Registers ROM address (0x00 to 0x3f)
        paramtype = CmdLine.ParseParam(argv[ARG1], &val);
        if ((paramtype == BADPARAM) || (paramtype == STRVAL)  ||
            (val > L99DZ200G_CFR))
        {
            return CMDLINE_INVALID_ARG;
        }
        romaddr = val;
        if (argc > 2)
        {
            // get the count of Device Information Registers addresses to read from
            paramtype = CmdLine.ParseParam(argv[ARG2], &val);
            if ((paramtype == BADPARAM) || (paramtype == STRVAL) ||
                (val < 1) || (val > 20))
            {
                return CMDLINE_INVALID_ARG;
            }
            romcnt = val;
        }

        for (uint8_t i = 0; i < romcnt; ++i)
        {
            if ((romaddr + i) > L99DZ200G_CFR)
            {
                break;      // invalid Device Information Registers ROM address
            }

            Serial.print(F("L99DZ200G Address "));
            Print0xHexByte(romaddr + i);
            Serial.print(F(": "));
            romval = L99dz200g.L99DZ200G_ReadRomAddress(romaddr + i);
            Print0xHexByteln(romval);
        }
    }

    // Return success.
    return 0;
}
#endif

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
 *  OUTn Current Monitor: OUT1, OUT2, OUT3, OUT6, OUT7, OUT8, OUT9, OUT10, OUT13, OUT14, OUT15
 *  X, Y Position Voltage Monitor: X, Y
 *  A, B  H-Bridge A Current Sense Monitor: A, B
 *
 *  Two optional parameters supported.
 *   n = show OUTn current monitor
 *      -or-
 *   <A> = show H-Bridge A current sense
 *      -or-
 *   <B> = show H-Bridge B current sense
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
        Serial.println("OUT: 1, 2, 3,  6, 7, 8, 9, 10,  13, 14, 15,  A, B, X, Y  [v | r]");
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
        else if ((tolower(argv[ARG1][0]) == 'x') && (argv[ARG1][1] == '\0'))
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
                case OUT_9:
                case OUT_10:
                case OUT_13:
                case OUT_14:
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
            case 'a':               // H-Bridge A current sense
                adc_value = GetAnalogValue(TK200G1_C_SENS_A_PIN);
                dec_digs = 1;
                break;
            case 'b':               // H-Bridge B current sense
                adc_value = GetAnalogValue(TK200G1_C_SENS_B_PIN);
                dec_digs = 1;
                break;
            case 'x':               // X position
                adc_value = GetAnalogValue(MR200G1_X_POS_PIN);
                dec_digs = 3;
                break;
            case 'y':               // Y position
                adc_value = GetAnalogValue(MR200G1_Y_POS_PIN);
                dec_digs = 3;
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
                                case OUT_9:
                                case OUT_10:
                                case OUT_13:
                                case OUT_14:
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

#ifdef SHOW_CAN
/*
 * NAME:
 *  int8_t Cmd_can(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "can" command to show/set L99DZ200G CAN control.
 *
 *  Seven optional on/off parameters supported (one at a time).
 *   <wuen>  = enable/disable CAN wake-up
 *      -or-
 *   <rxen> = enable/disable CAN Rx
 *      -or-
 *   <txen> = enable/disable CAN Tx
 *      -or-
 *   <toirq> = enable/disable CAN timeout IRQ
 *      -or-
 *   <gotx>  = enable/disable CAN go TRX READY mode
 *      -or-
 *   <loop> = enable/disable CAN Tx->Rx loopback
 *      -or-
 *   <pnw> = enable/disable CAN pretended networking
 *      -and-
 *   <on>  = CAN wake-up, CAN Rx, CAN Tx, CAN Tx->Rx loopback, CAN pretended networking is enabled
 *      -or-
 *   <off> = CAN wake-up, CAN Rx, CAN Tx, CAN Tx->Rx loopback, CAN pretended networking is disabled
 *
 *      1  2    3
 *     "can"            - show current CAN control settings
 *     "can wuen on"    - enable CAN wake-up
 *     "can wuen off"   - disable CAN wake-up
 *     "can rxen on"    - enable CAN Rx
 *     "can rxen off"   - disable CAN Rx
 *     "can txen on"    - enable CAN Tx
 *     "can txen off"   - disable CAN Tx
 *     "can toirq on"   - enable CAN timeout IRQ
 *     "can toirq off"  - disable CAN timeout IRQ
 *     "can gotx on"    - enable CAN go TRX READY mode
 *     "can gotx off"   - disable CAN go TRX READY mode
 *     "can loop on"    - enable CAN Tx->Rx loopback
 *     "can loop off"   - disable CAN Tx->Rx loopback
 *     "can pnw on"     - enable CAN pretended networking
 *     "can pnw off"    - disable CAN pretended networking
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_can(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("wuen")) == 0)
        {
            ctrl = CAN_WU_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("rxen")) == 0)
        {
            ctrl = CAN_RX_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("txen")) == 0)
        {
            ctrl = CAN_TX_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("toirq")) == 0)
        {
            ctrl = CAN_TO_IRQ_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("gotx")) == 0)
        {
            ctrl = CAN_GO_TX_RDY_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("loop")) == 0)
        {
            ctrl = CAN_LOOP_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("pnw")) == 0)
        {
            ctrl = CAN_PNW_EN_ITEM;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                on_off = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                on_off = ENABLE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            switch (ctrl)
            {
                case CAN_WU_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_WakeupControl(on_off);
                    break;
                case CAN_RX_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_ReceiveControl(on_off);
                    break;
                case CAN_TX_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_TransmitControl(on_off);
                    break;
                case CAN_TO_IRQ_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_TimeoutIrqControl(on_off);
                    break;
                case CAN_GO_TX_RDY_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_GoTxReadyControl(on_off);
                    break;
                case CAN_LOOP_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_LoopbackControl(on_off);
                    break;
                case CAN_PNW_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_CAN_PretendedNetworkingControl(on_off);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_CAN_Settings(ctrl);

    // Return success.
    return 0;
}

// get and show CAN settings
void Show_CAN_Settings(uint8_t ctrl)
{
    uint32_t reg1;
    uint32_t reg2;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CAN wake-up, Rx, Tx, timeout IRQ, go TX_READY enable values
    reg1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // get CAN loopback and pretended networking values
    reg2 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR2);

    if ((CAN_WU_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CAN_WU_EN: "));
        ShowEnabledDisabled((reg1 & CR1_CAN_WU_EN_MASK), true);
    }
    if ((CAN_RX_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CAN_RXEN: "));
        ShowEnabledDisabled((reg1 & CR1_CAN_RXEN_MASK), true);
    }
    if ((CAN_TX_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CAN_TXEN: "));
        ShowEnabledDisabled((reg1 & CR1_CAN_TXEN_MASK), true);
    }
    if ((CAN_TO_IRQ_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CANTO_IRQ_EN: "));
        ShowEnabledDisabled((reg1 & CR1_CAN_TOUT_IRQ_MASK), true);
    }
    if ((CAN_GO_TX_RDY_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CAN_GO_TRX_RDY: "));
        ShowEnabledDisabled((reg1 & CR1_CAN_TRXRDY_MASK), true);
    }
    if ((CAN_LOOP_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("CAN_LOOP_EN: "));
        ShowEnabledDisabled((reg2 & CR2_CAN_LOOP_EN_MASK), true);
    }
    if ((CAN_PNW_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("PNW_EN: "));
        ShowEnabledDisabled((reg2 & CR2_CAN_PNW_EN_MASK), true);
    }
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

#ifdef SHOW_CPUMP
/*
 * NAME:
 *  int8_t Cmd_cp(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "cp" command to show/set L99DZ200G charge pump state.
 *
 *  Three optional parameters supported.
 *   <off> = set charge pump off
 *      -or-
 *   <on> = set charge pump on
 *      -and-
 *   <ltch> = set charge pump low latched
 *      -or-
 *   <live> = set charge pump low live
 *      -and-
 *   <doff> = set charge pump clock dithering off
 *      -or-
 *   <don>  = set charge pump clock dithering on
 *
 *      1  2   3    4
 *     "cp"              - show current charge pump state/settings
 *     "cp off"          - set charge pump off
 *     "cp on"           - set charge pump on
 *     "cp on ltch"      - set charge pump on, charge pump low latched
 *     "cp on live"      - set charge pump on, charge pump low live
 *     "cp on live doff" - set charge pump on, charge pump low live, clock dithering off
 *     "cp on live don"  - set charge pump on, charge pump low live, clock dithering on
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_cp(int8_t argc, char * argv[])
{
    uint32_t reg;
    uint8_t on_off;
    uint8_t ltch_live;
    uint8_t don_doff;

    if (argc > 4)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        if (strcmp_P(argv[ARG1], PSTR("off")) == 0)
        {
            on_off = CP_DISABLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            on_off = CP_ENABLE;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        // get current charge pump settings
        reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
        if (reg & CFR_CP_LOW_CFG_MASK)
        {
            ltch_live = CP_LOW_CFG_LIVE;
        }
        else
        {
            ltch_live = CP_LOW_CFG_LATCHED;
        }
        if (reg & CFR_CP_DITH_DIS_MASK)
        {
            don_doff = IC_DITH_DISABLED;
        }
        else
        {
            don_doff = IC_DITH_ENABLED;
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("ltch")) == 0)
            {
                ltch_live = CP_LOW_CFG_LATCHED;
            }
            else if (strcmp_P(argv[ARG2], PSTR("live")) == 0)
            {
                ltch_live = CP_LOW_CFG_LIVE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            if (argc > 3)
            {
                if (strcmp_P(argv[ARG3], PSTR("doff")) == 0)
                {
                    don_doff = IC_DITH_DISABLED;
                }
                else if (strcmp_P(argv[ARG3], PSTR("don")) == 0)
                {
                    don_doff = IC_DITH_ENABLED;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
        }

        L99dz200g.L99DZ200G_SetChargePumpControl(on_off, ltch_live, don_doff);
    }

    ShowChargePumpControl();

    // Return success.
    return 0;
}

// get and show charge pump state/settings
void ShowChargePumpControl(void)
{
    uint32_t reg22;
    uint32_t regcfr;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get charge pump registers values
    reg22 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR22);
    reg22 &= CR22_CP_OFF_MASK;
    regcfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
    regcfr &= (CFR_CP_LOW_CFG_MASK | CFR_CP_DITH_DIS_MASK);

    // show charge pump run state
    Serial.print(F("CP_OFF: "));
    ShowEnabledDisabledRev(reg22, false);   // Note: is backward from usual enable/disable

    // show charge pump low config
    Serial.print(F(", CP_LOW_CONFIG: "));
    if (regcfr & CFR_CP_LOW_CFG_MASK)
    {
        Serial.print(F("Live"));
    }
    else
    {
        Serial.print(F("Latched"));
    }
    Serial.print(F(", "));

    // show charge pump clock dithering
    Serial.print(F("CP_DITH_DIS: "));
    ShowEnabledDisabledRev(regcfr & CFR_CP_DITH_DIS_MASK, true);  // Note: is backward from usual enable/disable
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

#ifdef SHOW_ICMP
/*
 * NAME:
 *  int8_t Cmd_icmp(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "icmp" command to show/set L99DZ200G V1 load current supervision (ICMP) state.
 *
 *  One optional parameter supported.
 *   <off> = set V1 load current supervision off
 *      -or-
 *   <on>  = set V1 load current supervision on
 *
 *      1    2
 *     "icmp"       - show current V1 load current supervision state
 *     "icmp off"   - set V1 load current supervision off
 *     "icmp on"    - set V1 load current supervision on
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_icmp(int8_t argc, char * argv[])
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
            on_off = IC_DISABLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            on_off = IC_ENABLE;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_Set_ICMP_Control(on_off);
    }

    Show_ICMP_Control();

    // Return success.
    return 0;
}

// get and show V1 load current supervision state/settings
void Show_ICMP_Control(void)
{
    uint32_t reg22;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get V1 load current supervision value
    reg22 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR22);
    reg22 &= CR22_ICMP_MASK;

    // show V1 load current supervision state
    Serial.print(F("ICMP: "));
    ShowEnabledDisabledRev(reg22, true);      // Note: is backward from usual enable/disable
}
#endif

#ifdef SHOW_LIN
/*
 * NAME:
 *  int8_t Cmd_lin(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "lin" command to show/set L99DZ200G LIN control.
 *
 *  Five optional parameters supported (one at a time).
 *   <wuen>  = enable/disable LIN wake-up
 *      -or-
 *   <rec> = enable/disable LIN receive only mode
 *      -or-
 *   <txto> = enable/disable LIN TxD timeout
 *      -and-
 *   <on>  = LIN wake-up, LIN receive only mode, LIN TxD timeout is enabled
 *      -or-
 *   <off> = LIN wake-up, LIN receive only mode, LIN TxD timeout is disabled
 *
 *      -or-
 *
 *   <spd> = select LIN bit rate
 *      -and-
 *   <lo> = 20 Kbs LIN bit rate
 *      -or-
 *   <hi> = 115 Kbs LIN bit rate
 *
 *      -or-
 *
 *   <cfg> = config wake-up behavior from LIN bus
 *      -and-
 *   <rdr> = wake-up at recessive-dominant-recessive LIN signal
 *      -or-
 *   <rdt> = wake-up at recessive-dominant transition of LIN signal
 *
 *      1  2    3
 *     "lin"           - show current LIN control settings
 *     "lin wuen on"   - enable LIN wake-up
 *     "lin wuen off"  - disable LIN wake-up
 *     "lin rec on"    - enable LIN receive only mode
 *     "lin rec off"   - disable LIN receive only mode
 *     "lin txto on"   - enable LIN TxD timeout
 *     "lin txto off"  - disable LIN TxD timeout
 *     "lin spd lo"    - select 20 Kbs LIN bit rate
 *     "lin spd hi"    - select 115 Kbs LIN bit rate
 *     "lin cfg rdr"   - wake-up at recessive-dominant-recessive LIN signal
 *     "lin cfg rdt"   - wake-up at recessive-dominant transition of LIN signal
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_lin(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off = 0;
    uint8_t hi_lo = 0;
    uint8_t rdr_rdt = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("wuen")) == 0)
        {
            ctrl = LIN_WU_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("rec")) == 0)
        {
            ctrl = LIN_REC_ONLY_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("txdto")) == 0)
        {
            ctrl = LIN_TXD_TOUT_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("spd")) == 0)
        {
            ctrl = LIN_HS_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("cfg")) == 0)
        {
            ctrl = LIN_WU_CONFIG_ITEM;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (ctrl <= LIN_TXD_TOUT_EN_ITEM)
            {
                if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
                {
                    on_off = DISABLE;
                }
                else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
                {
                    on_off = ENABLE;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else if (ctrl == LIN_HS_EN_ITEM)
            {
                if (strcmp_P(argv[ARG2], PSTR("lo")) == 0)
                {
                    hi_lo = SPD_LO_20KBS;
                }
                else if (strcmp_P(argv[ARG2], PSTR("hi")) == 0)
                {
                    hi_lo = SPD_LO_115KBS;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else if (ctrl == LIN_WU_CONFIG_ITEM)
            {
                if (strcmp_P(argv[ARG2], PSTR("rdr")) == 0)
                {
                    rdr_rdt = REC_DOM_REC_WU;
                }
                else if (strcmp_P(argv[ARG2], PSTR("rdt")) == 0)
                {
                    rdr_rdt = REC_DOM_TRANSITION_WU;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }

            switch (ctrl)
            {
                case LIN_WU_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_LIN_WakeupControl(on_off);
                    break;
                case LIN_REC_ONLY_ITEM:
                    L99dz200g.L99DZ200G_Set_LIN_ReceiveOnlyControl(on_off);
                    break;
                case LIN_TXD_TOUT_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_LIN_TransmitTimoutControl(on_off);
                    break;
                case LIN_HS_EN_ITEM:
                    L99dz200g.L99DZ200G_Set_LIN_BitRateControl(hi_lo);
                    break;
                case LIN_WU_CONFIG_ITEM:
                    L99dz200g.L99DZ200G_Set_LIN_WakeupBehaviorControl(rdr_rdt);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_LIN_Settings(ctrl);

    // Return success.
    return 0;
}

// get and show LIN settings
void Show_LIN_Settings(uint8_t ctrl)
{
    uint32_t reg1;
    uint32_t reg2;
    uint32_t regcfr;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get LIN wake-up enable value
    reg1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // get LIN RxD/TxD values
    reg2 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR2);

    // get LIN config values
    regcfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // show LIN wake-up and configuration states
    if ((LIN_WU_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("LIN_WU_EN: "));
        ShowEnabledDisabled((reg1 & CR1_LIN_WU_EN_MASK), true);
    }
    if ((LIN_REC_ONLY_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("LIN_REC_ONLY: "));
        ShowEnabledDisabled((reg2 & CR2_LIN_RX_ONLY_MASK), true);
    }
    if ((LIN_TXD_TOUT_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("LIN_TXD_TOUT_EN: "));
        ShowEnabledDisabled((reg2 & CR2_LIN_TXD_TO_EN_MASK), true);
    }
    if ((LIN_HS_EN_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("LIN_HS_EN: "));
        if (regcfr & CFR_LIN_HS_EN_MASK)
        {
            Serial.print(F("115"));
        }
        else
        {
            Serial.print(F("20"));
        }
        Serial.println(F(" Kbs"));
    }
    if ((LIN_WU_CONFIG_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("LIN_WU_CONFIG: Wake-Up: recessive-dominant"));
        if (regcfr & CFR_LIN_WU_CFG_MASK)
        {
            Serial.print(F(" transition"));
        }
        else
        {
            Serial.print(F("-recessive"));
        }
        Serial.println();
    }
}
#endif

#ifdef SHOW_MASK
/*
 * NAME:
 *  int8_t Cmd_mask(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "mask" command to show/set L99DZ200G status masked in GSB.
 *
 *  Eight optional parameters supported (one at a time).
 *   <spie> = mask/unmask SPI errors (invalid cmd, incorrect SCK count)
 *      -or-
 *   <ple>  = mask/unmask physical layer errors (CAN/LIN)
 *      -or-
 *   <hsol>  = mask/unmask high-side open-load
 *      -or-
 *   <lsol>  = mask/unmask low-side open-load
 *      -or-
 *   <ecol>  = mask/unmask electrochromic open-load
 *      -or-
 *   <ol>  = mask/unmask other open-load
 *      -or-
 *   <tw>  = mask/unmask thermal warnings
 *      -or-
 *   <gw>  = mask/unmask global warnings
 *      -and-
 *   <y> = status is masked
 *      -or-
 *   <n>  = status is not masked
 *
 *      1    2   3
 *     "mask"          - show current GSB status masked/unmasked
 *     "mask spie y"   - mask SPI errors
 *     "mask ple n"    - unmask physical layer errors
 *     "mask hsol y"   - mask high-side open-load
 *     "mask lsol n"   - unmask low-side open-load
 *     "mask ecol y"   - mask electrochromic open-load
 *     "mask ol n"     - unmask other open-load
 *     "mask tw y"     - mask thermal warnings
 *     "mask gw n"     - unmask global warnings
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_mask(int8_t argc, char * argv[])
{
    uint8_t mask = ALL_ITEMS;
    uint8_t on_off;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("spie")) == 0)
        {
            mask = SPIE_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ple")) == 0)
        {
            mask = PLE_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("hsol")) == 0)
        {
            mask = HS_OL_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("lsol")) == 0)
        {
            mask = LS_OL_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ecol")) == 0)
        {
            mask = EC_OL_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ol")) == 0)
        {
            mask = OL_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("tw")) == 0)
        {
            mask = TW_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("gw")) == 0)
        {
            mask = GW_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("all")) == 0)
        {
            mask = ALL_ITEMS;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("y")) == 0)
            {
                on_off = DO_MASK;
            }
            else if (strcmp_P(argv[ARG2], PSTR("n")) == 0)
            {
                on_off = DO_UNMASK;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            if (mask == ALL_ITEMS)
            {
                for (uint8_t i = HS_OL_ITEM; i < ALL_ITEMS; ++i)
                {
                    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

                    L99dz200g.L99DZ200G_Set_GSB_MaskUnmask(i, on_off);
                }
            }
            else
            {
                L99dz200g.L99DZ200G_Set_GSB_MaskUnmask(mask, on_off);
            }
        }
DEBUG_TOGL();
    }

    Show_GSB_Masks(mask);

    // Return success.
    return 0;
}

// get and show Global Status Byte (GSB) masks settings
void Show_GSB_Masks(uint8_t mask)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get masks value
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // show masks states
    if ((SPIE_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("SPIE: "));                      // SPI errors (invalid cmd, incorrect SCK count)
        ShowMaskedUnmasked(reg & CFR_MASK_SPIE_MASK);
    }
    if ((PLE_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("PLE:  "));                      // physical layer errors (CAN/LIN)
        ShowMaskedUnmasked(reg & CFR_MASK_PLE_MASK);
    }
    if ((HS_OL_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("HSOL: "));                      // high-side open-load
        ShowMaskedUnmasked(reg & CFR_MASK_OL_HS_MASK);
    }
    if ((LS_OL_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("LSOL: "));                      // low-side open-load
        ShowMaskedUnmasked(reg & CFR_MASK_OL_LS_MASK);
    }
    if ((EC_OL_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("ECOL: "));                      // electrochromic open-load
        ShowMaskedUnmasked(reg & CFR_MASK_EC_OL_MASK);
    }
    if ((OL_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("OL:   "));                      // other open-load
        ShowMaskedUnmasked(reg & CFR_MASK_OL_MASK);
    }
    if ((TW_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("TW:   "));                      // thermal warnings
        ShowMaskedUnmasked(reg & CFR_MASK_TW_MASK);
    }
    if ((GW_ITEM == mask) || (mask == ALL_ITEMS))
    {
        Serial.print(F("GW:   "));                      // global warnings
        ShowMaskedUnmasked(reg & CFR_MASK_GW_MASK);
    }
}
#endif

#ifdef SHOW_MISC
/*
 * NAME:
 *  int8_t Cmd_misc(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "misc" command to show/set L99DZ200G Generator/Debug/Watchdog control.
 *
 *  Four optional parameters supported (one at a time).
 *   <gen> = enable/disable Generator mode
 *      -or-
 *   <dbg>  = stay in or exit SW-Debug mode
 *      -or-
 *   <wd>  = enable/disable the watchdog (CAN-Flash mode only)
 *      -or-
 *   <tsd>  = enable/disable selective thermal shutdown
 *      -and-
 *   <on>  = enable Generator mode, stay in SW-Debug mode, enable the watchdog, enable selective thermal shutdown
 *      -or-
 *   <off> = disable Generator mode, exit SW-Debug mode, disable the watchdog, disable selective thermal shutdown
 *
 *      1     2  3
 *     "misc"          - show current Generator/Debug/Watchdog settings
 *     "misc gen on"   - enable Generator mode
 *     "misc gen off"  - disable Generator mode
 *     "misc dbg off"  - exit SW-Debug mode
 *     "misc dbg on"   - stay in SW-Debug mode
 *     "misc wd on"    - enable the watchdog
 *     "misc wd off"   - disable the watchdog
 *     "misc tsd on"   - enable selective thermal shutdown
 *     "misc tsd off"  - disable selective thermal shutdown
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_misc(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off = DISABLE;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("gen")) == 0)
        {
            ctrl = GEN_MODE_ENABLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("dbg")) == 0)
        {
            ctrl = SW_DEBUG_EXIT;
        }
        else if (strcmp_P(argv[ARG1], PSTR("wd")) == 0)
        {
            ctrl = WDOG_ENABLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("tsd")) == 0)
        {
            ctrl = TSD_ENABLE;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                on_off = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                on_off = ENABLE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            switch (ctrl)
            {
                case GEN_MODE_ENABLE:
                    L99dz200g.L99DZ200G_SetGeneratorModeControl(on_off);
                    break;
                case SW_DEBUG_EXIT:
                    L99dz200g.L99DZ200G_StayExitSW_DebugModeControl(on_off);
                    break;
                case WDOG_ENABLE:
                    L99dz200g.L99DZ200G_WdogEnableControl(on_off);
                    break;
                case TSD_ENABLE:
                    L99dz200g.L99DZ200G_ThermalShutdownControl(on_off);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_VSREG_GenDebugWdogTsdSettings(ctrl);

    // Return success.
    return 0;
}

// get and show Generator/Debug/Watchdog/Thermal Shutdown settings
void Show_VSREG_GenDebugWdogTsdSettings(uint8_t ctrl)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get Generator/Debug/Watchdog values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR22);

    if ((GEN_MODE_ENABLE == ctrl) || (ctrl == ALL_ITEMS))
    {
        // Note: This remains unchanged after exiting VBAT_Standby mode!!!
        Serial.print(F("GENERATOR_MODE_EN: "));
        ShowOnOff(reg & CR22_GEN_MODE_EN_MASK);
    }
    if ((SW_DEBUG_EXIT == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("DEBUG_EXIT: "));
        ShowOnOff(reg & CR22_DEBUG_EXIT_MASK);
    }
    if ((WDOG_ENABLE == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("WD_EN: "));
        ShowOnOff(reg & CR22_WDOG_EN_MASK);
    }
    if ((TSD_ENABLE == ctrl) || (ctrl == ALL_ITEMS))
    {
        // get selective Thermal Shutdown value
        reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
        Serial.print(F("TSD_CONFIG: Selective thermal shutdown "));
        ShowOnOff(reg & CFR_TSD_CFG_MASK);
    }
}
#endif

#ifdef SHOW_MODE
/*
 * NAME:
 *  int8_t Cmd_mode(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "mode" command to set/show L99DZ200G standby mode.
 *
 *  One optional parameter supported.
 *   <mode> = L99DZ200G standby mode ("v1" or "vbat" or "act")
 *
 *       1   2
 *     "mode"          - show L99DZ200G mode
 *     "mode v1"       - set L99DZ200G V1 standby mode
 *     "mode vbat"     - set L99DZ200G VBAT standby mode
 *     "mode act"      - set L99DZ200G CR1 to active mode
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_mode(int8_t argc, char * argv[])
{
    uint8_t mode = 0;

    if (argc < 2)
    {
        // OK, just show current mode
        ShowStandbyModeSetting();
    }
    else if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("v1")) == 0)
        {
            mode = V1_STBY;
        }
        else if (strcmp_P(argv[ARG1], PSTR("vbat")) == 0)
        {
            mode = VBAT_STBY;
        }
        else if (strcmp_P(argv[ARG1], PSTR("act")) == 0)
        {
            mode = ACTIVE_STBY;
        }
        else    // invalid mode
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_SetModeControl(mode);
DEBUG_TOGL();
    }

   // Return success.
    return 0;
}

// get and show L99DZ200G mode
void ShowStandbyModeSetting(void)
{
    uint32_t reg = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get standby mode register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);
    reg = (reg & CR1_STBY_MASK) >> CR1_STBY_POS;

    // show show L99DZ200G mode
    Serial.print(F("Mode: "));
    switch (reg)
    {
        case GOTO_V1_STANDBY:
            Serial.print(F("V1 Standby"));
            break;
        case GOTO_VBAT_STANDBY:
            Serial.print(F("Vbat Standby"));
            break;
        default:
            Serial.print(F("Active"));
            break;
    }
    Serial.println();
}
#endif

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
 *  Implements the "out" command to show/set OUT7, OUT8, OUT9, OUT13, OUT14, or OUT15 output control.
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
        ShowOutputState(OUT_9);
        ShowOutputState(OUT_10);
        ShowOutputState(OUT_13);
        ShowOutputState(OUT_14);
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
        case OUT_9:
        case OUT_13:
        case OUT_14:
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
        case OUT_9:
            reg &= CR6_HS_OUT9_MASK;
            output_state = reg >> CR6_HS_OUT9_POS;
            break;
        case OUT_10:
            reg &= CR5_HS_OUT10_MASK;
            output_state = reg >> CR5_HS_OUT10_POS;
            break;
        case OUT_13:
            reg &= CR6_HS_OUT13_MASK;
            output_state = reg >> CR6_HS_OUT13_POS;
            break;
        case OUT_14:
            reg &= CR6_HS_OUT14_MASK;
            output_state = reg >> CR6_HS_OUT14_POS;
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
 *  Many optional parameters supported.
 *   <can> = CAN status
 *   <ecv> = Electrochromic Voltage status
 *   <hb>  = H-bridge status
 *   <lin> = LIN status
 *   <ms>  = Misc status
 *   <oc>  = Overcurrent shutdown status
 *   <ocr> = Overcurrent recovery status
 *   <ol>  = Open-load status
 *   <sc>  = Short circuit status
 *   <spi> = SPI errors status
 *   <tsd> = thermal shutdown status
 *   <tw>  = thermal warning status
 *   <vo>  = VS, VSREG, V1, V2, CP voltages status
 *   <wd>  = Watchdog status
 *   <wu>  = Wake-up status
 *
 *   <clr> = clear specified status item
 *
 *        1   2  3
 *     "stat"        - show all L99DZ200G status registers (SR1 to SR6 and SR12)
 *     "stat can"    - show CAN status
 *     "stat vo"     - VS, VSREG, V1, V2, CP voltages status
 *     "stat wu clr" - clear wake-up status
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
#ifndef SHOW_STAT_DETAIL
        (void)argv;
        (void)clear;
        (void)status;

        return CMDLINE_TOO_MANY_ARGS;
#else
DEBUG_TOGL();
        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("clr")) == 0)
            {
                clear = true;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }

        if (strcmp_P(argv[ARG1], PSTR("can")) == 0)
        {
            status = Show_CAN_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("ecv")) == 0)
        {
            if (clear)
            {
                return CMDLINE_INVALID_ARG;     // ECV cannot be cleared!
            }
            status = Show_ECV_Status();
        }
        else if (strcmp_P(argv[ARG1], PSTR("hb")) == 0)
        {
            status = Show_HB_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("lin")) == 0)
        {
            status = Show_LIN_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("ms")) == 0)
        {
            status = Show_MS_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("oc")) == 0)
        {
            status = Show_OC_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("ocr")) == 0)
        {
            if (clear)
            {
                return CMDLINE_INVALID_ARG;     // OCR cannot be cleared!
            }
            status = Show_OCR_Status();
        }
        else if (strcmp_P(argv[ARG1], PSTR("ol")) == 0)
        {
            status = Show_OL_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("sc")) == 0)
        {
            status = Show_SC_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("spi")) == 0)
        {
            status = Show_SPI_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("tsd")) == 0)
        {
            status = Show_TSD_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("tw")) == 0)
        {
            status = Show_TW_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("vo")) == 0)
        {
            status = Show_VO_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("wd")) == 0)
        {
            status = Show_WD_Status(clear);
        }
        else if (strcmp_P(argv[ARG1], PSTR("wu")) == 0)
        {
            status = Show_WU_Status(clear);
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (!status && !clear)
        {
            Serial.println(F("OK"));
        }
DEBUG_TOGL();
#endif
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

#ifdef SHOW_STAT_DETAIL
// Get and show/clear CAN status
bool Show_CAN_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_Clear_CAN_Status(ALL_CAN_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_SUP_LOW_ITEM);
        stat2 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_TXD_DOM_ITEM);
        stat3 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_PERM_DOM_ITEM);
        stat4 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_PERM_REC_ITEM);
        stat5 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_RXD_REC_ITEM);
        stat6 = L99dz200g.L99DZ200G_Get_CAN_Status(CANTO_ITEM);
        stat7 = L99dz200g.L99DZ200G_Get_CAN_Status(CAN_SILENT_ITEM);
        if (stat1)
        {
            Serial.println(F("CAN_SUP_LOW"));
        }
        if (stat2)
        {
            Serial.println(F("CAN_TXD_DOM"));
        }
        if (stat3)
        {
            Serial.println(F("CAN_PERM_DOM"));
        }
        if (stat4)
        {
            Serial.println(F("CAN_PERM_REC"));
        }
        if (stat5)
        {
            Serial.println(F("CAN_RXD_REC"));
        }
        if (stat6)
        {
            Serial.println(F("CANTO"));
        }
        if (stat7)
        {
            Serial.println(F("CAN_SILENT"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7)
        {
            return true;
        }
    }
    return false;
}

// Get and show Electrochromic Voltage status
bool Show_ECV_Status(void)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;

    stat1 = L99dz200g.L99DZ200G_GetElectrochromicVoltageStatus(ECV_VNR);
    stat2 = L99dz200g.L99DZ200G_GetElectrochromicVoltageStatus(ECV_VHI);
    if (stat1)
    {
        Serial.println(F("ECV_VNR"));
    }
    if (stat2)
    {
        Serial.println(F("ECV_VHI"));
    }

    if (stat1 || stat2)
    {
        return true;
    }
    return false;
}

// Get and show/clear H-bridge status
bool Show_HB_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_A);
        L99dz200g.L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_B);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_A, LOW_SIDE_1_MOSFET);
        stat2 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_A, HIGH_SIDE_1_MOSFET);
        stat3 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_A, LOW_SIDE_2_MOSFET);
        stat4 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_A, HIGH_SIDE_2_MOSFET);

        stat5 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_B, LOW_SIDE_1_MOSFET);
        stat6 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_B, LOW_SIDE_1_MOSFET);
        stat7 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_B, LOW_SIDE_1_MOSFET);
        stat8 = L99dz200g.L99DZ200G_Get_HB_DrainSourceMonitoringStatus(H_BRIDGE_CONTROL_B, LOW_SIDE_1_MOSFET);
        if (stat1)
        {
            Serial.println(F("HB_LS1_A"));
        }
        if (stat2)
        {
            Serial.println(F("HB_HS1_A"));
        }
        if (stat3)
        {
            Serial.println(F("HB_LS2_A"));
        }
        if (stat4)
        {
            Serial.println(F("HB_HS2_A"));
        }

        if (stat5)
        {
            Serial.println(F("HB_LS1_B"));
        }
        if (stat6)
        {
            Serial.println(F("HB_HS1_B"));
        }
        if (stat7)
        {
            Serial.println(F("HB_LS2_B"));
        }
        if (stat8)
        {
            Serial.println(F("HB_HS2_B"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear LIN status
bool Show_LIN_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_Clear_LIN_Status(ALL_LIN_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_Get_LIN_Status(LIN_PERM_REC_ITEM);
        stat2 = L99dz200g.L99DZ200G_Get_LIN_Status(LIN_TXD_DOM_ITEM);
        stat3 = L99dz200g.L99DZ200G_Get_LIN_Status(LIN_PERM_DOM_ITEM);
        if (stat1)
        {
            Serial.println(F("LIN_PERM_REC"));
        }
        if (stat2)
        {
            Serial.println(F("(LIN_TXD_DOM"));
        }
        if (stat3)
        {
            Serial.println(F("LIN_PERM_DOM"));
        }

        if (stat1 || stat2 || stat3)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear Misc status
bool Show_MS_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearMiscellaneousStatus(ALL_MISC_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetMiscellaneousStatus(SGND_LOSS_ITEM);
        stat2 = L99dz200g.L99DZ200G_GetMiscellaneousStatus(DEBUG_MODE_ITEM);
        stat3 = L99dz200g.L99DZ200G_GetMiscellaneousStatus(VSPOR_ITEM);
        if (stat1)
        {
            Serial.println(F("SGND_LOSS"));
        }
        if (stat2)
        {
            Serial.println(F("(DEBUG_MODE"));
        }
        if (stat3)
        {
            Serial.println(F("VSPOR"));
        }

        if (stat1 || stat2 || stat3)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear Overcurrent shutdown status
bool Show_OC_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;
    uint8_t stat9 = L99DZ200G_OK;
    uint8_t stat10 = L99DZ200G_OK;
    uint8_t stat11 = L99DZ200G_OK;
    uint8_t stat12 = L99DZ200G_OK;
    uint8_t stat13 = L99DZ200G_OK;
    uint8_t stat14 = L99DZ200G_OK;
    uint8_t stat15 = L99DZ200G_OK;
    uint8_t stat16 = L99DZ200G_OK;
    uint8_t stat17 = L99DZ200G_OK;
    uint8_t stat18 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearOvercurrentShutdownStatus(OUT_ALL_OCS_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_1_HS);
        stat2 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_1_LS);
        stat3 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_2_HS);
        stat4 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_2_LS);
        stat5 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_3_HS);
        stat6 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_3_LS);
        stat7 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_6_HS);
        stat8 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_6_LS);
        stat9 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_7_OC);
        stat10 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_8_OC);
        stat11 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_9_OC);
        stat12 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_10_OC);
        stat13 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_13_OC);
        stat14 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_14_OC);
        stat15 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_15_OC);
        stat16 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(LSA_FSO_OC);
        stat17 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(LSB_FSO_OC);
        stat18 = L99dz200g.L99DZ200G_GetOvercurrentShutdownStatus(OUT_ECV);
        if (stat1)
        {
            Serial.println(F("OUT_1_HS"));
        }
        if (stat2)
        {
            Serial.println(F("(OUT_1_LS"));
        }
        if (stat3)
        {
            Serial.println(F("(OUT_2_HS"));
        }
        if (stat4)
        {
            Serial.println(F("(OUT_2_LS"));
        }
        if (stat5)
        {
            Serial.println(F("OUT_3_HS"));
        }
        if (stat6)
        {
            Serial.println(F("OUT_3_LS"));
        }
        if (stat7)
        {
            Serial.println(F("OUT_6_HS"));
        }
        if (stat8)
        {
            Serial.println(F("OUT_6_LS"));
        }
        if (stat9)
        {
            Serial.println(F("OUT_7_OC"));
        }
        if (stat10)
        {
            Serial.println(F("OUT_8_OC"));
        }
        if (stat11)
        {
            Serial.println(F("OUT_9_OC"));
        }
        if (stat12)
        {
            Serial.println(F("OUT_10_OC"));
        }
        if (stat13)
        {
            Serial.println(F("OUT_13_OC"));
        }
        if (stat14)
        {
            Serial.println(F("OUT_14_OC"));
        }
        if (stat15)
        {
            Serial.println(F("OUT_15_OC"));
        }
        if (stat16)
        {
            Serial.println(F("LSA_FSO_OC"));
        }
        if (stat17)
        {
            Serial.println(F("LSB_FSO_OC"));
        }
        if (stat18)
        {
            Serial.println(F("OUT_ECV"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8 ||
            stat9 || stat10 || stat11 || stat12 || stat13 || stat14 || stat15 ||
            stat16 || stat17 || stat18)
        {
            return true;
        }
    }
    return false;
}

// Get and show Overcurrent recovery status
bool Show_OCR_Status(void)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;
    uint8_t stat9 = L99DZ200G_OK;
    uint8_t stat10 = L99DZ200G_OK;
    uint8_t stat11 = L99DZ200G_OK;

    stat1 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_1_HS);
    stat2 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_1_LS);
    stat3 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_2_HS);
    stat4 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_2_LS);
    stat5 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_3_HS);
    stat6 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_3_LS);
    stat7 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_6_HS);
    stat8 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_6_LS);
    stat9 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_7_OCR);
    stat10 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_8_OCR);
    stat11 = L99dz200g.L99DZ200G_GetOvercurrentRecoveryAlertStatus(OUT_15_OCR);
    if (stat1)
    {
        Serial.println(F("OUT_1_HS"));
    }
    if (stat2)
    {
        Serial.println(F("(OUT_1_LS"));
    }
    if (stat3)
    {
        Serial.println(F("(OUT_2_HS"));
    }
    if (stat4)
    {
        Serial.println(F("(OUT_2_LS"));
    }
    if (stat5)
    {
        Serial.println(F("OUT_3_HS"));
    }
    if (stat6)
    {
        Serial.println(F("OUT_3_LS"));
    }
    if (stat7)
    {
        Serial.println(F("OUT_6_HS"));
    }
    if (stat8)
    {
        Serial.println(F("OUT_6_LS"));
    }
    if (stat9)
    {
        Serial.println(F("OUT_7_OCR"));
    }
    if (stat10)
    {
        Serial.println(F("OUT_8_OCR"));
    }
    if (stat11)
    {
        Serial.println(F("OUT_15_OCR"));
    }

    if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8 ||
        stat9 || stat10 || stat11)
    {
        return true;
    }
    return false;
}

// Get and show/clear Open-load status
bool Show_OL_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;
    uint8_t stat9 = L99DZ200G_OK;
    uint8_t stat10 = L99DZ200G_OK;
    uint8_t stat11 = L99DZ200G_OK;
    uint8_t stat12 = L99DZ200G_OK;
    uint8_t stat13 = L99DZ200G_OK;
    uint8_t stat14 = L99DZ200G_OK;
    uint8_t stat15 = L99DZ200G_OK;
    uint8_t stat16 = L99DZ200G_OK;
    uint8_t stat17 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearOpenLoadStatus(OUT_ALL_OL_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_1_HS);
        stat2 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_1_LS);
        stat3 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_2_HS);
        stat4 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_2_LS);
        stat5 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_3_HS);
        stat6 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_3_LS);
        stat7 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_6_HS);
        stat8 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_6_LS);
        stat9 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_7_OC);
        stat10 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_8_OL);
        stat11 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_9_OL);
        stat12 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_10_OL);
        stat13 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_13_OL);
        stat14 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_14_OL);
        stat15 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_15_OL);
        stat16 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_GH);
        stat17 = L99dz200g.L99DZ200G_GetOpenLoadStatus(OUT_ECV);
        if (stat1)
        {
            Serial.println(F("OUT_1_HS"));
        }
        if (stat2)
        {
            Serial.println(F("(OUT_1_LS"));
        }
        if (stat3)
        {
            Serial.println(F("(OUT_2_HS"));
        }
        if (stat4)
        {
            Serial.println(F("(OUT_2_LS"));
        }
        if (stat5)
        {
            Serial.println(F("OUT_3_HS"));
        }
        if (stat6)
        {
            Serial.println(F("OUT_3_LS"));
        }
        if (stat7)
        {
            Serial.println(F("OUT_6_HS"));
        }
        if (stat8)
        {
            Serial.println(F("OUT_6_LS"));
        }
        if (stat9)
        {
            Serial.println(F("OUT_7_OL"));
        }
        if (stat10)
        {
            Serial.println(F("OUT_8_OL"));
        }
        if (stat11)
        {
            Serial.println(F("OUT_9_OL"));
        }
        if (stat12)
        {
            Serial.println(F("OUT_10_OL"));
        }
        if (stat13)
        {
            Serial.println(F("OUT_13_OL"));
        }
        if (stat14)
        {
            Serial.println(F("OUT_14_OL"));
        }
        if (stat15)
        {
            Serial.println(F("OUT_15_OL"));
        }
        if (stat16)
        {
            Serial.println(F("OUT_GH"));
        }
        if (stat17)
        {
            Serial.println(F("OUT_ECV"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8 ||
            stat9 || stat10 || stat11 || stat12 || stat13 || stat14 || stat15 ||
            stat16 || stat17)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear Short circuit status
bool Show_SC_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;
    uint8_t stat9 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearShortCircuitAlertStatus(OUT_ALL_SC_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_1_HS);
        stat2 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_1_LS);
        stat3 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_2_HS);
        stat4 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_2_LS);
        stat5 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_3_HS);
        stat6 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_3_LS);
        stat7 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_6_HS);
        stat8 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(OUT_6_LS);
        stat9 = L99dz200g.L99DZ200G_GetShortCircuitAlertStatus(DS_MON_HEAT);
        if (stat1)
        {
            Serial.println(F("OUT_1_HS"));
        }
        if (stat2)
        {
            Serial.println(F("(OUT_1_LS"));
        }
        if (stat3)
        {
            Serial.println(F("(OUT_2_HS"));
        }
        if (stat4)
        {
            Serial.println(F("(OUT_2_LS"));
        }
        if (stat5)
        {
            Serial.println(F("OUT_3_HS"));
        }
        if (stat6)
        {
            Serial.println(F("OUT_3_LS"));
        }
        if (stat7)
        {
            Serial.println(F("OUT_6_HS"));
        }
        if (stat8)
        {
            Serial.println(F("OUT_6_LS"));
        }
        if (stat9)
        {
            Serial.println(F("DS_MON_HEAT"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8 || stat9)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear SPI errors status
bool Show_SPI_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_Clear_SPI_Status();
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_Get_SPI_Status(SPI_CLK_CNT);
        stat2 = L99dz200g.L99DZ200G_Get_SPI_Status(SPI_INV_CMD);
        if (stat1)
        {
            Serial.println(F("SPI_CLK_CNT"));
        }
        if (stat2)
        {
            Serial.println(F("SPI_INV_CMD"));
        }

        if (stat1 || stat2)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear thermal shutdown status
bool Show_TSD_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearThermalShutdownStatus(TH_ALL_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_ITEM);
        stat2 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD2_ITEM);
        stat3 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL1_ITEM);
        stat4 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL2_ITEM);
        stat5 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL3_ITEM);
        stat6 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL4_ITEM);
        stat7 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL5_ITEM);
        stat8 = L99dz200g.L99DZ200G_GetThermalShutdownStatus(TSD1_CL6_ITEM);
        if (stat1)
        {
            Serial.println(F("TSD1"));
        }
        if (stat2)
        {
            Serial.println(F("(TSD2"));
        }
        if (stat3)
        {
            Serial.println(F("(TSD1_CL1"));
        }
        if (stat4)
        {
            Serial.println(F("(TSD1_CL2"));
        }
        if (stat5)
        {
            Serial.println(F("TSD1_CL3"));
        }
        if (stat6)
        {
            Serial.println(F("TSD1_CL4"));
        }
        if (stat7)
        {
            Serial.println(F("TSD1_CL5"));
        }
        if (stat8)
        {
            Serial.println(F("TSD1_CL6"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear thermal warning status
bool Show_TW_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearThermalWarningStatus(TH_ALL_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetThermalWarningStatus(THERMW_ITEM);
        stat2 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL1_ITEM);
        stat3 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL2_ITEM);
        stat4 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL3_ITEM);
        stat5 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL4_ITEM);
        stat6 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL5_ITEM);
        stat7 = L99dz200g.L99DZ200G_GetThermalWarningStatus(TW_CL6_ITEM);
        if (stat1)
        {
            Serial.println(F("THERMW"));
        }
        if (stat2)
        {
            Serial.println(F("(TW_CL1"));
        }
        if (stat3)
        {
            Serial.println(F("(TW_CL2"));
        }
        if (stat4)
        {
            Serial.println(F("(TW_CL3"));
        }
        if (stat5)
        {
            Serial.println(F("TW_CL4"));
        }
        if (stat6)
        {
            Serial.println(F("TW_CL5"));
        }
        if (stat7)
        {
            Serial.println(F("TW_CL6"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear VS, VSREG, V1, V2, CP voltages status
bool Show_VO_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;
    uint8_t stat7 = L99DZ200G_OK;
    uint8_t stat8 = L99DZ200G_OK;
    uint8_t stat9 = L99DZ200G_OK;
    uint8_t stat10 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearVoltageStatus(ALL_VOLT_ITEMS);
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetVoltageStatus(V1_UV);
        stat2 = L99dz200g.L99DZ200G_GetVoltageStatus(VS_UV);
        stat3 = L99dz200g.L99DZ200G_GetVoltageStatus(VS_OV);
        stat4 = L99dz200g.L99DZ200G_GetVoltageStatus(VSREG_UV);
        stat5 = L99dz200g.L99DZ200G_GetVoltageStatus(VSREG_OV);
        stat6 = L99dz200g.L99DZ200G_GetVoltageStatus(VSREG_EW);
        stat7 = L99dz200g.L99DZ200G_GetVoltageStatus(V1_FAIL);
        stat8 = L99dz200g.L99DZ200G_GetVoltageStatus(V2_FAIL);
        stat9 = L99dz200g.L99DZ200G_GetVoltageStatus(V2_SC);
        stat10 = L99dz200g.L99DZ200G_GetVoltageStatus(CP_LOW);
        if (stat1)
        {
            Serial.println(F("V1_UV"));
        }
        if (stat2)
        {
            Serial.println(F("(VS_UV"));
        }
        if (stat3)
        {
            Serial.println(F("(VS_OV"));
        }
        if (stat4)
        {
            Serial.println(F("(VSREG_UV"));
        }
        if (stat5)
        {
            Serial.println(F("VSREG_OV"));
        }
        if (stat6)
        {
            Serial.println(F("VSREG_EW"));
        }
        if (stat7)
        {
            Serial.println(F("V1_FAIL"));
        }
        if (stat8)
        {
            Serial.println(F("V2_FAIL"));
        }
        if (stat9)
        {
            Serial.println(F("V2_SC"));
        }
        if (stat10)
        {
            Serial.println(F("CP_LOW"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6 || stat7 || stat8 ||
            stat9 || stat10)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear Watchdog status
bool Show_WD_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearWdogFailStatus();
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetWdogFailStatus();
        stat2 = L99dz200g.L99DZ200G_GetWdogFailCount();
        if (stat1)
        {
            Serial.println(F("WDFAIL"));
        }
        if (stat2)
        {
            Serial.print(F("WDFAIL_CNT = "));
            Serial.println(stat2);
        }

        if (stat1 || stat2)
        {
            return true;
        }
    }
    return false;
}

// Get and show/clear Wake-up status
bool Show_WU_Status(bool clear)
{
    uint8_t stat1 = L99DZ200G_OK;
    uint8_t stat2 = L99DZ200G_OK;
    uint8_t stat3 = L99DZ200G_OK;
    uint8_t stat4 = L99DZ200G_OK;
    uint8_t stat5 = L99DZ200G_OK;
    uint8_t stat6 = L99DZ200G_OK;

    if (clear)
    {
        L99dz200g.L99DZ200G_ClearWakeUpStatus(ALL_WK_ITEMS);
        L99dz200g.L99DZ200G_ClearDeviceWakeUpState();
    }
    else
    {
        stat1 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_TIMER_ITEM);
        stat2 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_LIN_ITEM);
        stat3 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_CAN_ITEM);
        stat4 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_WU_ITEM);
        stat5 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_VS_OV_ITEM);
        stat6 = L99dz200g.L99DZ200G_GetWakeUpStatus(WK_CAN_WUP_ITEM);
        if (stat1)
        {
            Serial.println(F("WK_TIMER"));
        }
        if (stat2)
        {
            Serial.println(F("(WK_LIN"));
        }
        if (stat3)
        {
            Serial.println(F("(WK_CAN"));
        }
        if (stat4)
        {
            Serial.println(F("(WK_WU"));
        }
        if (stat5)
        {
            Serial.println(F("WK_VS_OV"));
        }
        if (stat6)
        {
            Serial.println(F("WK_CAN_WUP"));
        }

        if (stat1 || stat2 || stat3 || stat4 || stat5 || stat6)
        {
            return true;
        }
    }
    return false;
}
#endif
#endif

#ifdef SHOW_TEMP
/*
 * NAME:
 *  int8_t Cmd_temp(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "temp" command to get and show L99DZ200G thermal clusters temperatures.
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_temp(int8_t argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    float temp;

    for (uint8_t i = TEMP_CL1; i <= TEMP_CL6; ++i)
    {
        L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

        temp = L99dz200g.L99DZ200G_GetThermalClusterTemp(i);
        Serial.print(F("TEMP_CL"));
        Serial.print(i);
        Serial.print(F(": "));
        Serial.print(temp);
        Serial.print(F(" C ("));
        Serial.print(((temp * 9) / 5) + 32);
        Serial.println(F(" F)"));
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

#ifdef SHOW_TNINT
/*
 * NAME:
 *  int8_t Cmd_tnint(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "tnint" command to show/set L99DZ200G Timer NINT control.
 *
 *  Two optional parameters supported (one at a time).
 *   <ten> = enable/disable Timer for NINT/Wake control
 *      -and-
 *   <on>  = enable Timer for NINT/Wake control
 *      -or-
 *   <off> = disable Timer for NINT/Wake control
 *
 *      -or-
 *
 *   <sel>  = select Timer for NINT/Wake control
 *      -and-
 *   <t1>  = select Timer1 for NINT/Wake control
 *      -or-
 *   <t2>  = select Timer2 for NINT/Wake control
 *
 *       1    2   3
 *     "tnint"          - show current Timer for NINT/Wake control settings
 *     "tnint ten on"   - enable Timer for NINT/Wake control
 *     "tnint ten off"  - disable Timer for NINT/Wake control
 *     "tnint sel t1"   - select Timer1 for NINT/Wake control
 *     "tnint sel t2"   - select Timer2 for NINT/Wake control
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_tnint(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off = 0;
    uint8_t tmr_sel = 0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("ten")) == 0)
        {
            ctrl = TMR_NINT_ENABLE_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("sel")) == 0)
        {
            ctrl = TMR_NINT_SELECT_ITEM;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            switch (ctrl)
            {
                case TMR_NINT_ENABLE_ITEM:
                    if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
                    {
                        on_off = DISABLE;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
                    {
                        on_off = ENABLE;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    break;
                case TMR_NINT_SELECT_ITEM:
                    if (strcmp_P(argv[ARG2], PSTR("t1")) == 0)
                    {
                        tmr_sel = TMR_NINT_SEL_T1;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("t2")) == 0)
                    {
                        tmr_sel = TMR_NINT_SEL_T2;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    break;
           }

            switch (ctrl)
            {
                case TMR_NINT_ENABLE_ITEM:
                    L99dz200g.L99DZ200G_SetTimer_NINT_EnableControl(on_off);
                    break;
                case TMR_NINT_SELECT_ITEM:
                    L99dz200g.L99DZ200G_SetTimer_NINT_SelectControl(tmr_sel);
                    break;
            }
        }
DEBUG_TOGL();
    }

    ShowTimer_NINT_Settings(ctrl);

    // Return success.
    return 0;
}

// get and show Timer enable and select for NINT/Wake control settings
void ShowTimer_NINT_Settings(uint8_t ctrl)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get Timer enable and select for NINT/Wake control values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // show Timer enable for NINT/Wake control
    if ((TMR_NINT_ENABLE_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("TIMER_NINT_EN:  "));
        ShowOnOff(reg & CR1_TMR_NINT_EN_MASK);
    }

    // show selected Timer for NINT/Wake control
    if ((TMR_NINT_SELECT_ITEM == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("TIMER_NINT_WAKE_SEL: Timer"));
        if (reg & CR1_TMR_NINT_SEL_MASK)
        {
            Serial.print(1);
        }
        else
        {
            Serial.print(2);
        }
        Serial.println();
    }
}
#endif

#ifdef SHOW_V1RESET
/*
 * NAME:
 *  int8_t Cmd_v1t(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "v1t" command to show/set L99DZ200G V1 reset threshold settings.
 *
 *  One optional parameter supported.
 *   <threshold> = the V1 reset threshold setting to use
 *                 (3.5 V, 3.8 V, 4.0 V, 4.3 V)
 *
 *       1   2
 *     "v1t"        - show Heater Drain monitor threshold setting
 *     "v1t 3.5v"   - set Heater Drain monitor threshold setting to 200 mV
 *     "v1t 3.8v"   - set Heater Drain monitor threshold setting to 500 mV
 *     "v1t 4.0v"   - set Heater Drain monitor threshold setting to 500 mV
 *     "v1t 4.3v"   - set Heater Drain monitor threshold setting to 500 mV
 *                    (V1_RESET_3_5V, V1_RESET_3_8V, V1_RESET_4_0V, V1_RESET_4_3V)
 *                    ("3.5v",        "3.8v",        "4.0v",        "4.3v")
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_v1t(int8_t argc, char * argv[])
{
    uint8_t v1_thrshld = 0;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
        // get the V1 reset threshold
        if (strcmp_P(argv[ARG1], PSTR("3.5v")) == 0)
        {
            v1_thrshld = V1_RESET_3_5V;
        }
        else if (strcmp_P(argv[ARG1], PSTR("3.8v")) == 0)
        {
            v1_thrshld = V1_RESET_3_8V;
        }
        else if (strcmp_P(argv[ARG1], PSTR("4.0v")) == 0)
        {
            v1_thrshld = V1_RESET_4_0V;
        }
        else if (strcmp_P(argv[ARG1], PSTR("4.3v")) == 0)
        {
            v1_thrshld = V1_RESET_4_3V;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_Set_V1_ResetThreshold(v1_thrshld);
    }

    Show_V1_ResetThresholdVoltage();
    if (argc < 2)
    {
        Serial.println(F("(3.5v, 3.8v, 4.0v 4.3v)"));
    }

    // Return success.
    return 0;
}

// get and show V1 reset threshold voltage
void Show_V1_ResetThresholdVoltage(void)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CR2 register values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR2);
    reg &= CR2_V1_RST_LEV_MASK;
    reg >>= CR2_V1_RST_LEV_POS;

    // show Heater monitor threshold voltage
    Serial.print(F("V1_RESET: "));
    switch (reg)
    {
        case V1_RESET_3_5V:
            Serial.print(F("3.5"));
            break;
        case V1_RESET_3_8V:
            Serial.print(F("3.8"));
            break;
        case V1_RESET_4_0V:
            Serial.print(F("4.0"));
            break;
        case V1_RESET_4_3V:
            Serial.print(F("4.3"));
            break;
    }
    Serial.println(F(" V"));
}
#endif

#ifdef SHOW_V2
/*
 * NAME:
 *  int8_t Cmd_v2(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "v2" command to show/set L99DZ200G V2 regulator mode settings.
 *
 *  Two optional parameters supported.
 *   <off>   = set V2 regulator off in all modes
 *      -or-
 *   <act>   = set V2 regulator on in Active mode, off in Standby modes
 *      -or-
 *   <actv1> = set V2 regulator on in Active and V1_Standby modes, off in VBAT_Standby mode
 *      -or-
 *   <on>    = set V2 regulator on in all modes
 *
 *      -and-
 *
 *    <reg>  = set V2 regulator as voltage regulator
 *      -or-
 *    <trk>  = set V2 regulator as as V1 voltage tracker
 *
 *       1  2   3
 *     "v2"           - show V2 regulator settings
 *     "v2 on"        - set V2 regulator on in all modes
 *     "v2 off"       - set V2 regulator off in all modes
 *     "v2 act"       - set V2 regulator on in Active mode, off in Standby modes
 *     "v2 off"       - set V2 regulator off in all modes
 *     "v2 actv1"     - set V2 regulator on in Active and V1_Standby mode, off in VBAT_Standby mode
 *     "v2 on reg"    - set V2 regulator on as voltage regulator in all modes
 *     "v2 actv1 trk" - set V2 regulator as as V1 voltage tracker in Active and V1_Standby modes
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_v2(int8_t argc, char * argv[])
{
    uint32_t mode = 0;
    uint32_t type = 0;

    if (argc < 2)
    {
        // OK, just show L99DZ200G V2 regulator mode settings
    }
    else if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else
    {
DEBUG_TOGL();
        // get the V2 regulator mode
        if (strcmp_P(argv[ARG1], PSTR("off")) == 0)
        {
            mode = OFF_V2;
        }
        else if (strcmp_P(argv[ARG1], PSTR("act")) == 0)
        {
            mode = ON_ACTIVEMODE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("actv1")) == 0)
        {
            mode = ON_ACTIVEMODE_V1STANDBY;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            mode = ALWAYS_ON;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            // get the V2 regulator type
            if (strcmp_P(argv[ARG2], PSTR("reg")) == 0)
            {
                type = V2_VREG_TYPE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("trk")) == 0)
            {
                type = V2_TRACK_V1_TYPE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }
        }
        else
        {
            // get current V2 regulator type
            type = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
            type &= CFR_V2_CFG_MASK;
        }

        L99dz200g.L99DZ200G_V2_Config(mode, type);
DEBUG_TOGL();
    }

    Show_V2_RegulatorSettings();

    // Return success.
    return 0;
}

// get and show L99DZ200G V2 regulator mode settings
void Show_V2_RegulatorSettings(void)
{
    uint32_t reg1;
    uint32_t regcfr;
    uint8_t type = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get V2 regulator registers values
    reg1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);
    regcfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);

    // get V2 regulator type
    type = (regcfr & CFR_V2_CFG_MASK) >> CFR_V2_CFG_POS;

    Serial.print(F("V2: "));

    // show V2 regulator mode setting
    Serial.print(F("Mode: "));
    switch (reg1 & CR1_V2_MODE_MASK)
    {
        case OFF_V2:
            Serial.print("Off");
            break;
        case ON_ACTIVEMODE:
            Serial.print("On Active Mode");
            break;
        case ON_ACTIVEMODE_V1STANDBY:
            Serial.print("On Active & V1_Standby Modes");
            break;
        case ALWAYS_ON:
            Serial.print("Always On");
            break;
    }

    // show V2 regulator type setting
    Serial.print(F(", Type: "));
    switch (type)
    {
        case V2_VREG:
            Serial.print("Reg");
            break;
        case V2_TRACK_V1:
            Serial.print("V1 Track");
            break;
    }
    Serial.println();
}
#endif

#ifdef SHOW_VLED
/*
 * NAME:
 *  int8_t Cmd_vled(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "vled" command to show/set OUT7, OUT8, OUT9, OUT10, OUT13, OUT14, or OUT15
 *  Vs VLED Compensation control.
 *
 *  Two optional parameters supported.
 *   <off> = set Automatic Vs VLED Compensation off
 *      -or-
 *   <on>  = set Automatic Vs VLED Compensation on
 *      -or-
 *   vled = the VLED LED voltage drop (0 V to 10 V)
 *
 *       1   2 3
 *     "vled 7"       - show OUT7 LED voltage drop setting
 *     "vled 8 on"    - set OUT8 Automatic Vs VLED Compensation on
 *     "vled 9 off"   - set OUT9 Automatic Vs VLED Compensation off
 *     "vled 7 10"    - set OUT7 VLED LED voltage drop to 10 V
 *     "vled 6 2.54"  - set OUT6 VLED LED voltage drop to 2.54 V
 *     "vled 15 1.2"  - set OUT15 VLED LED voltage drop to 1.2 V
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_vled(int8_t argc, char * argv[])
{
    int32_t val;
    int8_t paramtype;
    uint8_t output = 0;
    uint8_t len;
    float led_volts;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc < 2)
    {
        ShowVsVledSetting(OUT_7);
        ShowVsVledSetting(OUT_8);
        ShowVsVledSetting(OUT_9);
        ShowVsVledSetting(OUT_10);
        ShowVsVledSetting(OUT_13);
        ShowVsVledSetting(OUT_14);
        ShowVsVledSetting(OUT_15);
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
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                L99dz200g.L99DZ200G_SetAutoVsCompensationControl(output, DISABLE);
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                L99dz200g.L99DZ200G_SetAutoVsCompensationControl(output, ENABLE);
            }
            else if (!isdigit(argv[ARG2][0]))
            {
                return CMDLINE_INVALID_ARG;
            }
            else
            {
                // get the LED voltage value
                if (strcmp_P(argv[ARG2], PSTR("10")) == 0)      // max 10 V
                {
                    led_volts = VLED_MAX;
                }
                else
                {
                    // get the LED voltage value (n.nn)
                    len = strnlen(argv[ARG2], 5);
                    if (len > 4)
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 1) && (argv[ARG2][1] != '.'))
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 2) && (!isdigit(argv[ARG2][2])))
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 3) && (!isdigit(argv[ARG2][3])))
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    led_volts = atof(argv[ARG2]);
                }
                L99dz200g.L99DZ200G_SetVsCompensationVLED(output, led_volts);
            }
        }
DEBUG_TOGL();
        ShowVsVledSetting(output);
    }

    // Return success.
    return 0;
}

// get and show OUTn Vs VLED Compensation settings
void ShowVsVledSetting(uint8_t output)
{
    uint32_t reg;
    uint16_t vled = 0;
    uint8_t auto_en = 0;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get OUTn VLED registers values
    switch (output)
    {
        case OUT_7:
        case OUT_8:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR17);
            break;
        case OUT_9:
        case OUT_10:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR18);
            break;
        case OUT_13:
        case OUT_14:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR19);
            break;
        case OUT_15:
            reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR20);
            break;
        default:
            return;     // invalid output
    }

    // get OUTn register VLED
    switch (output)
    {
        case OUT_7:
        case OUT_9:
        case OUT_13:
        case OUT_15:
            vled = (reg & ODD_VLED_MASK) >> ODD_VLED_POS;
            auto_en = (reg & ODD_OUT_AUTOCOMP_EN) >> ODD_OUT_AUTOCOMP_EN_POS;
            break;
        case OUT_8:
        case OUT_10:
        case OUT_14:
            vled = (reg & EVEN_VLED_MASK) >> EVEN_VLED_POS;
            auto_en = (reg & EVEN_OUT_AUTOCOMP_EN) >> EVEN_OUT_AUTOCOMP_EN_POS;
            break;
    }

    // show OUTn Vs VLED Compensation settings
    Serial.print(F("OUT"));
    Serial.print(output);
    Serial.print(F(": "));
    if (output < OUT_10)
    {
        Serial.print(F(" "));
    }

    Serial.print(F("VLED: "));
    Serial.print((VAINVS * vled) / 1024);

    Serial.print(F(" V, Auto: "));
    ShowEnabledDisabled(auto_en, true);
}
#endif

#ifdef SHOW_VOLT
/*
 * NAME:
 *  int8_t Cmd_volt(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "volt" command to get and show L99DZ200G pins voltages.
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_volt(int8_t argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    float volt;

    for (uint8_t i = VS_REG_V; i <= VWU_V; ++i)
    {
        L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

        volt = L99dz200g.L99DZ200G_GetPinVoltage(i);
        switch (i)
        {
            case VS_REG_V:
                Serial.print(F("VS_REG"));
                break;
            case VS_V:
                Serial.print(F("VS"));
                break;
            case VWU_V:
                Serial.print(F("WU"));
                break;
        }
        Serial.print(F(": "));
        Serial.print(volt);
        Serial.println(F(" V"));
    }

    // Return success.
    return 0;
}
#endif

#ifdef SHOW_VS
/*
 * NAME:
 *  int8_t Cmd_vs(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "vs" command to show/set L99DZ200G VS supply overvoltage/undervoltage control.
 *
 *  Three optional parameters supported (one at a time).
 *   <lck> = enable/disable lockout of VS related outputs after VS overvoltage/undervoltage shutdown
 *      -or-
 *   <ovsd>  = enable/disable shutdown of VS related outputs in case of VS overvoltage
 *      -or-
 *   <uvsd>  = enable/disable shutdown of VS related outputs in case of VS undervoltage
 *      -and-
 *   <on> = lockout/shutdown is enabled
 *      -or-
 *   <off>  = lockout/shutdown is disabled
 *
 *      1  2    3
 *     "vs"           - show current VS overvoltage/undervoltage lockout/shutdown control
 *     "vs lck on"    - enable lockout of VS related outputs after VS overvoltage/undervoltage shutdown
 *     "vs ovsd off"  - disable shutdown of VS related outputs in case of VS overvoltage
 *     "vs uvsd on"   - enable shutdown of VS related outputs in case of VS undervoltage
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_vs(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("lck")) == 0)
        {
            ctrl = VS_LOCK;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ovsd")) == 0)
        {
            ctrl = VS_OVSHD;
        }
        else if (strcmp_P(argv[ARG1], PSTR("uvsd")) == 0)
        {
            ctrl = VS_UVSHD;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
            {
                on_off = DISABLE;
            }
            else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
            {
                on_off = ENABLE;
            }
            else
            {
                return CMDLINE_INVALID_ARG;
            }

            switch (ctrl)
            {
                case VS_LOCK:
                    L99dz200g.L99DZ200G_Set_VS_LockoutControl(on_off);
                    break;
                case VS_OVSHD:
                    L99dz200g.L99DZ200G_Set_VS_OvervoltageShutdownControl(on_off);
                    break;
                case VS_UVSHD:
                    L99dz200g.L99DZ200G_Set_VS_UndervoltageShutdownControl(on_off);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_VS_UvOvSettings(ctrl);

    // Return success.
    return 0;
}

// get and show VS supply overvoltage/undervoltage settings
void Show_VS_UvOvSettings(uint8_t ctrl)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get overvoltage/undervoltage lockout/shutdown values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR3);

    // show lockout/shutdown states
    if ((VS_LOCK == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VS_LOCK_EN:  "));               // overvoltage/undervoltage lockout
        ShowOnOff(reg & CR3_VS_LOCK_MASK);
    }
    if ((VS_OVSHD == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VS_OV_SD_EN: "));               // undervoltage shutdown
        ShowOnOff(reg & CR3_VS_OVSHD_MASK);
    }
    if ((VS_UVSHD == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VS_UV_SD_EN: "));               // overvoltage shutdown
        ShowOnOff(reg & CR3_VS_UVSHD_MASK);
    }
}
#endif

#ifdef SHOW_VSR
/*
 * NAME:
 *  int8_t Cmd_vsr(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "vsr" command to show/set L99DZ200G VSREG supply overvoltage/undervoltage control.
 *
 *  Four optional parameters supported (one at a time).
 *   <lck> = enable/disable lockout of VSREG related outputs after VSREG overvoltage/undervoltage shutdown
 *      -or-
 *   <ovsd>  = enable/disable shutdown of VSREG related outputs in case VSREG VS overvoltage
 *      -or-
 *   <uvsd>  = enable/disable shutdown of VSREG related outputs in case of VSREG undervoltage
 *      -and-
 *   <on> = lockout/shutdown is enabled
 *      -or-
 *   <off>  = lockout/shutdown is disabled
 *      -or-
 *   <ewt>  = set VSREG early warning threshold of NINT interrupt
 *      -and-
 *   thshld = VSREG early warning threshold voltage
 *
 *      1   2    3
 *     "vsr"           - show current VSREG overvoltage/undervoltage lockout/shutdown control
 *     "vsr lck on"    - enable lockout of VSREG related outputs after VSREG overvoltage/undervoltage shutdown
 *     "vsr ovsd off"  - disable shutdown of VSREG related outputs in case of VSREG overvoltage
 *     "vsr uvsd on"   - enable shutdown of VSREG related outputs in case of VSREG undervoltage
 *     "vsr ewt 9.8"   - set VSREG early warning threshold of NINT interrupt to 9.8 volts
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_vsr(int8_t argc, char * argv[])
{
    uint8_t ctrl = ALL_ITEMS;
    uint8_t on_off = DISABLE;
    uint8_t len;
    float thshld_volts = 0.0;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("lck")) == 0)
        {
            ctrl = VSREG_LOCK;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ovsd")) == 0)
        {
            ctrl = VSREG_OVSHD;
        }
        else if (strcmp_P(argv[ARG1], PSTR("uvsd")) == 0)
        {
            ctrl = VSREG_UVSHD;
        }
        else if (strcmp_P(argv[ARG1], PSTR("ewt")) == 0)
        {
            ctrl = VSREG_EW_THR;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            if (ctrl != VSREG_EW_THR)
            {
                if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
                {
                    on_off = DISABLE;
                }
                else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
                {
                    on_off = ENABLE;
                }
                else
                {
                    return CMDLINE_INVALID_ARG;
                }
            }
            else
            {
                if (!isdigit(argv[ARG2][0]))
                {
                    return CMDLINE_INVALID_ARG;
                }

                // get the VSREG early warning threshold voltage value
                if (strcmp_P(argv[ARG2], PSTR("10")) == 0)      // max 10 V
                {
                    thshld_volts = EW_THR_MAX;
                }
                else
                {
                    // get the LED voltage value (n.nn)
                    len = strnlen(argv[ARG2], 5);
                    if (len > 4)
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 1) && (argv[ARG2][1] != '.'))
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 2) && (!isdigit(argv[ARG2][2])))
                    {
                        return CMDLINE_INVALID_ARG;
                    }
                    else if ((len > 3) && (!isdigit(argv[ARG2][3])))
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    thshld_volts = atof(argv[ARG2]);
                }
            }

            switch (ctrl)
            {
                case VSREG_LOCK:
                    L99dz200g.L99DZ200G_Set_VSREG_LockoutControl(on_off);
                    break;
                case VSREG_OVSHD:
                    L99dz200g.L99DZ200G_Set_VSREG_OvervoltageShutdownControl(on_off);
                    break;
                case VSREG_UVSHD:
                    L99dz200g.L99DZ200G_Set_VSREG_UndervoltageShutdownControl(on_off);
                    break;
                case VSREG_EW_THR:
                    L99dz200g.L99DZ200G_Set_VSREG_EarlyEarningThreshold(thshld_volts);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_VSREG_UvOvSettings(ctrl);

    // Return success.
    return 0;
}

// get and show VSREG supply overvoltage/undervoltage settings
void Show_VSREG_UvOvSettings(uint8_t ctrl)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get overvoltage/undervoltage lockout/shutdown/early warning threshold values
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR3);

    // show lockout/shutdown states
    if ((VSREG_LOCK == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VSREG_LOCK_EN:  "));                // overvoltage/undervoltage lockout
        ShowOnOff(reg & CR3_VSREG_LOCK_MASK);
    }
    if ((VSREG_OVSHD == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VSREG_OV_SD_EN: "));                // undervoltage shutdown
        ShowOnOff(reg & CR3_VSREG_OVSHD_MASK);
    }
    if ((VSREG_UVSHD == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VSREG_UV_SD_EN: "));                // overvoltage shutdown
        ShowOnOff(reg & CR3_VSREG_UVSHD_MASK);
    }
    if ((VSREG_EW_THR == ctrl) || (ctrl == ALL_ITEMS))
    {
        Serial.print(F("VSREG_EW_TH: "));                   // early warning threshold
        reg = (reg & CR3_VSREG_EW_MASK) >> CR3_VSREG_EW_POS;
        Serial.print((VAINVS * reg) / 1024);
        Serial.println(F(" V"));
    }
}
#endif

#ifdef SHOW_WDOG
/*
 * NAME:
 *  int8_t Cmd_wd(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "wd" command to show/set L99DZ200G watchdog trigger time.
 *
 *  One optional parameter supported.
 *   <time> = the watchdog trigger time setting to use
 *                 (10ms, 50ms, 100ms, 200ms)
 *
 *       1  2
 *     "wd"        - show watchdog trigger time setting
 *     "wd 10ms"   - set watchdog trigger time setting to 10 mS
 *     "wd 50ms"   - set watchdog trigger time setting to 50 mS
 *     "wd 100ms"  - set watchdog trigger time setting to 100 mS
 *     "wd 200ms"  - set watchdog trigger time setting to 200 mS
 *                   (WDOG_TIME_50MS, WDOG_TIME_50MS, WDOG_TIME_100MS, WDOG_TIME_200MS)
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_wd(int8_t argc, char * argv[])
{
    uint8_t ttime = 0;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        // get the watchdog trigger time
        if (strcmp_P(argv[ARG1], PSTR("10ms")) == 0)
        {
            ttime = WDOG_TIME_10MS;
        }
        else if (strcmp_P(argv[ARG1], PSTR("50ms")) == 0)
        {
            ttime = WDOG_TIME_50MS;
        }
        else if (strcmp_P(argv[ARG1], PSTR("100ms")) == 0)
        {
            ttime = WDOG_TIME_100MS;
        }
        else if (strcmp_P(argv[ARG1], PSTR("200ms")) == 0)
        {
            ttime = WDOG_TIME_200MS;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_SetWdogTime(ttime);
DEBUG_TOGL();
    }

    Show_WatchdogTriggerTime();
    if (argc < 2)
    {
        Serial.println(F("(10ms, 50ms, 100ms, 200ms)"));
    }

    // Return success.
    return 0;
}

// get and show watchdog trigger time
void Show_WatchdogTriggerTime(void)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get watchdog trigger time
    reg = L99dz200g.L99DZ200G_GetWdogTime();

    // show watchdog trigger time
    Serial.print(F("WD_TIME: "));
    switch (reg)
    {
        case WDOG_TIME_10MS:
            Serial.print(10);
            break;
        case WDOG_TIME_50MS:
            Serial.print(50);
            break;
        case WDOG_TIME_100MS:
            Serial.print(100);
            break;
        case WDOG_TIME_200MS:
            Serial.print(200);
            break;
    }
    Serial.println(F(" mS"));
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

#ifdef SHOW_WAKEUP
/*
 * NAME:
 *  int8_t Cmd_wu(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "wu" command to show/set L99DZ200G WU pin control.
 *
 *  Three optional parameters supported (one at a time).
 *   <en>  = enable/disable WU pin as wake-up input
 *      -and-
 *   <on>  = WU pin is enabled for wake-up
 *      -or-
 *   <off> = WU pin is disabled for wake-up
 *
 *      -or-
 *
 *   <pin> = config WU pin for pull-up/pull-down, wake-up/VBAT voltage measurement
 *      -and-
 *   <pu> = WU pin has pull-up input (Standby modes only)
 *      -or-
 *   <pd>  = WU pin has pull-down input
 *      -or-
 *   <wu>  = WU pin is wake-up input
 *      -or-
 *   <vbat>  = WU pin is VBAT voltage measurement input
 *
 *      -or-
 *
 *   <filt> = config WU pin filter mode
 *      -and-
 *   <st> = WU pin is monitored in static mode (64 uS)
 *      -or-
 *   <t1>  = WU pin is monitored in cyclic mode with T1 (16 uS after 80% ton blanking time)
 *      -or-
 *   <t2>  = WU pin is monitored in cyclic mode with T2 (16 uS after 80% ton blanking time)
 *
 *      1  2   3
 *     "wu"           - show current WU pin control settings
 *     "wu en on"     - enable WU pin as wake-up input (if configured in CFR)
 *     "wu en off"    - disable WU pin as wake-up input
 *     "wu pin pu"    - set WU pin as pull-up input
 *     "wu pin pd"    - set WU pin as pull-down input
 *     "wu pin wu"    - set WU pin as wake-up input
 *     "wu pin vbat"  - set WU pin as VBAT voltage measurement input
 *     "wu filt st"   - set WU filtered in static mode
 *     "wu filt t1"   - set WU filtered in cyclic mode with T1
 *     "wu filt t2"   - set WU filtered in cyclic mode with T2
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  None.
 */
int8_t Cmd_wu(int8_t argc, char * argv[])
{
    uint8_t item = ALL_ITEMS;
    uint8_t config;
    bool mode = false;

    if (argc > 3)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("en")) == 0)
        {
            item = WU_PIN_EN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("pin")) == 0)
        {
            item = WU_PIN_ITEM;
        }
        else if (strcmp_P(argv[ARG1], PSTR("filt")) == 0)
        {
            item = WU_FILTER_ITEM;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        if (argc > 2)
        {
            switch (item)
            {
                case WU_PIN_EN_ITEM:
                    if (strcmp_P(argv[ARG2], PSTR("off")) == 0)
                    {
                        config = DISABLE;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("on")) == 0)
                    {
                        config = ENABLE;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    L99dz200g.L99DZ200G_Set_WU_PinControl(config);
                    break;

                case WU_PIN_ITEM:
                    if (strcmp_P(argv[ARG2], PSTR("pu")) == 0)
                    {
                        config = WU_PULL_UP;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("pd")) == 0)
                    {
                        config = WU_PULL_DOWN;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("wu")) == 0)
                    {
                        config = WU_WU;
                        mode = true;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("vbat")) == 0)
                    {
                        config = WU_VBAT;
                        mode = true;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    if (mode)
                    {
                        L99dz200g.L99DZ200G_Set_WU_PinModeConfig(config);
                    }
                    else
                    {
                        L99dz200g.L99DZ200G_Set_WU_PinPullConfig(config);
                    }
                    break;

                case WU_FILTER_ITEM:
                    if (strcmp_P(argv[ARG2], PSTR("st")) == 0)
                    {
                        config = WU_FILTER_STATIC;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("t1")) == 0)
                    {
                        config = WU_FILTER_T1;
                    }
                    else if (strcmp_P(argv[ARG2], PSTR("t2")) == 0)
                    {
                        config = WU_FILTER_T2;
                    }
                    else
                    {
                        return CMDLINE_INVALID_ARG;
                    }

                    L99dz200g.L99DZ200G_Set_WU_PinFilterConfig(config);
                    break;
            }
        }
DEBUG_TOGL();
    }

    Show_WU_PinSettings(item);

    // Return success.
    return 0;
}

// get and show enable/disable and pull settings
void Show_WU_PinSettings(uint8_t item)
{
    uint32_t reg1;
    uint32_t regcfr;
    uint8_t pin_cfg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get wake-up config value
    regcfr = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
    pin_cfg = (regcfr & CFR_WU_CFG_MASK) >> CFR_WU_CFG_POS;

    // get wake-up enable/disable and pull values
    reg1 = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CR1);

    // show wake-up enable/disable states
    if ((WU_ENABLE_ITEM == item) || (item == ALL_ITEMS))
    {

        Serial.print(F("WU_EN: "));
        ShowEnabledDisabled((reg1 & CR1_WU_EN_MASK), false);
        if ((WU_ENABLE_ITEM == item) && (pin_cfg != WU_WU))
        {
            Serial.print(F(" (not WakeUp)"));
        }
        Serial.println();
    }

    // show wake-up pull and config states
    if ((WU_PIN_ITEM == item) || (item == ALL_ITEMS))
    {

        Serial.print(F("WU_CONFIG: "));
        if (regcfr & CFR_WU_CFG_MASK)
        {
            Serial.print(F("VBAT"));
        }
        else
        {
            Serial.print(F("wake-up"));
        }
        Serial.println();

        Serial.print(F("WU_PU: Pull-"));
        if (reg1 & CR1_WU_PU_MASK)
        {
            Serial.print(F("up"));
        }
        else
        {
            Serial.print(F("down"));
        }
        if ((WU_PIN_ITEM == item) && (pin_cfg != WU_WU))
        {
            Serial.print(F(" (not WakeUp)"));
        }
        Serial.println();
    }

    // show wake-up filter state
    if ((WU_FILTER_ITEM == item) || (item == ALL_ITEMS))
    {
        reg1 = (reg1 & CR1_WU_FILTER_MASK) >> CR1_WU_FILTER_POS;
        Serial.print(F("WU_FILT: "));
        switch (reg1)
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
    }
}
#endif

#ifdef SHOW_HBRIDGE
/*
 * NAME:
 *  int8_t Cmd_fso(int8_t argc, char * argv[])
 *
 * PARAMETERS:
 *  int8_t argc = number of command line arguments for the command
 *  char * argv[] = pointer to array of parameters associated with the command
 *
 * WHAT:
 *  Implements the "fso" command to show/set the L99DZ200G LSx_FSO low-side outputs forced ON.
 *
 *  One optional parameter supported.
 *   <off> = set LSx_FSO low-side outputs state forced off
 *      -or-
 *   <on> = set LSx_FSO low-side outputs state forced on
 *
 *       1   2
 *     "fso"       - show LSx_FSO low-side outputs forced state
 *     "fso off"   - set LSx_FSO low-side outputs state forced off
 *     "fso on"    - set LSx_FSO low-side outputs state forced on
 *
 * RETURN VALUES:
 *  int8_t = 0 = command successfully processed
 *
 * SPECIAL CONSIDERATIONS:
 *  Note: When forced on, this causes the L99DZ200G to enter Fail-Safe mode (leading to the
 *         change of the watchdog timer trigger time and eventual watchdog failures reported).
 */
int8_t Cmd_fso(int8_t argc, char * argv[])
{
    uint8_t on_off = DISABLE;

    if (argc > 2)
    {
        return CMDLINE_TOO_MANY_ARGS;
    }
    else if (argc > 1)
    {
DEBUG_TOGL();
        if (strcmp_P(argv[ARG1], PSTR("off")) == 0)
        {
            on_off = DISABLE;
        }
        else if (strcmp_P(argv[ARG1], PSTR("on")) == 0)
        {
            on_off = ENABLE;
        }
        else
        {
            return CMDLINE_INVALID_ARG;
        }

        L99dz200g.L99DZ200G_Set_HB_Forced_LSx_FSO_Control(on_off);
DEBUG_TOGL();
    }

    ShowLSx_FSO_ForcedStateControl();

    // Return success.
    return 0;
}

// get and show LSx_FSO low-side outputs forced state
void ShowLSx_FSO_ForcedStateControl(void)
{
    uint32_t reg;

    L99dz200g.L99DZ200G_CheckWdogExpired();     // process watchdog

    // get CFR register value
    reg = L99dz200g.L99DZ200G_ReadRegister(L99DZ200G_CFR);
    reg &= CFR_FS_FORCED_MASK;

    // show LSx_FSO low-side outputs forced enable
    Serial.print(F("FS_FORCED: "));
    ShowOnOff(reg);
}

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

