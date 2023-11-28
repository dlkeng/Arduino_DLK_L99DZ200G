/** \file L99DZ200G.h */
/*
 * NAME: L99DZ200G.h
 *
 * WHAT:
 *  Header file for L99DZ200G defines.
 *
 * SPECIAL CONSIDERATIONS:
 *  Derived from: STMicro AutoDevKit examples
 *
 * AUTHOR:
 *  D.L. Karmann
 *
 */
#ifndef __L99DZ200G_H__
#define __L99DZ200G_H__

// L99DZ200G SPI operation codes
#define SPI_WR_CODE             0x00    // 00
#define SPI_RD_CODE             0x40    // 01
#define SPI_RD_CLR_CODE         0x80    // 10
#define SPI_DEV_INFO_CODE       0xc0    // 11
#define SPI_ADDR_MASK           0x3F

#define SET_SPI_WR(reg)         ((reg & SPI_ADDR_MASK) | SPI_WR_CODE)
#define SET_SPI_RD(reg)         ((reg & SPI_ADDR_MASK) | SPI_RD_CODE)
#define SET_SPI_RD_CLR(reg)     ((reg & SPI_ADDR_MASK) | SPI_RD_CLR_CODE)
#define SET_SPI_DEV_INFO(addr)  ((addr & SPI_ADDR_MASK) | SPI_DEV_INFO_CODE)

#define SPI_TRANSACTION_SIZE    4       // bytes

#define L99DZ200G_OK            0
#define L99DZ200G_FAIL          1

#define DISABLE                 0
#define ENABLE                  1

#define H_BRIDGE_CONTROL_A      0
#define H_BRIDGE_CONTROL_B      1

// Select which register to use for Watchdog Trigger
//#define WDOG_TRIGGER_CR1
#define WDOG_TRIGGER_CFR

#if !defined(WDOG_TRIGGER_CR1) && !defined(WDOG_TRIGGER_CFR)
#error "Must select a WDOG_TRIGGER register!"
#elif defined(WDOG_TRIGGER_CR1) && defined(WDOG_TRIGGER_CFR)
#error "Cannot select both WDOG_TRIGGER registers!"
#endif


#if 1   // L99DZ200G Registers Addresses
#define L99DZ200G_CR1       0x01
#define L99DZ200G_CR2       0x02
#define L99DZ200G_CR3       0x03
#define L99DZ200G_CR4       0x04
#define L99DZ200G_CR5       0x05
#define L99DZ200G_CR6       0x06
#define L99DZ200G_CR7       0x07
#define L99DZ200G_CR8       0x08
#define L99DZ200G_CR9       0x09
#define L99DZ200G_CR10      0x0a
#define L99DZ200G_CR11      0x0b
#define L99DZ200G_CR12      0x0c
#define L99DZ200G_CR13      0x0d
#define L99DZ200G_CR14      0x0e
#define L99DZ200G_CR15      0x0f
#define L99DZ200G_CR16      0x10
#define L99DZ200G_CR17      0x11
#define L99DZ200G_CR18      0x12
#define L99DZ200G_CR19      0x13
#define L99DZ200G_CR20      0x14
#define L99DZ200G_CR21      0x15
#define L99DZ200G_CR22      0x16
#define L99DZ200G_CR23      0x17
#define L99DZ200G_CR24      0x18
#define L99DZ200G_CR25      0x19
#define L99DZ200G_CR26      0x1A
#define L99DZ200G_CR27      0x1B
#define L99DZ200G_CR28      0x1C
#define L99DZ200G_CR29      0x1D
#define L99DZ200G_CR34      0x22

#define L99DZ200G_SR1       0x31
#define L99DZ200G_SR2       0x32
#define L99DZ200G_SR3       0x33
#define L99DZ200G_SR4       0x34
#define L99DZ200G_SR5       0x35
#define L99DZ200G_SR6       0x36
#define L99DZ200G_SR7       0x37
#define L99DZ200G_SR8       0x38
#define L99DZ200G_SR9       0x39
#define L99DZ200G_SR10      0x3A
#define L99DZ200G_SR11      0x3B
#define L99DZ200G_SR12      0x3C

#define L99DZ200G_CFR       0x3F
#endif


#if 1   // L99DZ200G Registers Defines
#define FULL_REG_MASK           0xFFFFFF

// -------- CR1 ------------------------------------------------------------------------------------

#define CR1_WU_EN_MASK          0x400000
#define CR1_WU_PU_MASK          0x100000
#define CR1_WU_FILTER_MASK      0x030000
#define CR1_TMR_NINT_SEL_MASK   0x008000
#define CR1_TMR_NINT_EN_MASK    0x004000
#define CR1_LIN_WU_EN_MASK      0x002000
#define CR1_CAN_WU_EN_MASK      0x001000
#define CR1_CAN_TOUT_IRQ_MASK   0x000800
#define CR1_CAN_RXEN_MASK       0x000400
#define CR1_CAN_TXEN_MASK       0x000200
#define CR1_CAN_TRXRDY_MASK     0x000100
#define CR1_HENA_MASK           0x000080
#define CR1_HENB_MASK           0x000040
#define CR1_V2_MODE_MASK        0x000030
#define CR1_STBY_MASK           0x00000E
#define CR1_TRIG_MASK           0x000001
#define CR1_WDC_MASK            0x000001

#define CR1_WU_EN_POS           22
#define CR1_WU_PU_POS           20
#define CR1_WU_FILTER_POS       16
#define CR1_TMR_NINT_SEL_POS    15
#define CR1_TMR_NINT_EN_POS     14
#define CR1_LIN_WU_EN_POS       13
#define CR1_CAN_WU_EN_POS       12
#define CR1_CAN_TOUT_IRQ_POS    11
#define CR1_CAN_RXEN_POS        10
#define CR1_CAN_TXEN_POS        9
#define CR1_CAN_TRXRDY_POS      8
#define CR1_HENA_POS            7
#define CR1_HENB_POS            6
#define CR1_V2_MODE_POS         4
#define CR1_STBY_POS            1
#define CR1_TRIG__POS           0

// CR1 V2 Configuration: Table 95
#define OFF_V2                  (0 << CR1_V2_MODE_POS)
#define ON_ACTIVEMODE           (1 << CR1_V2_MODE_POS)
#define ON_ACTIVEMODE_V1STANDBY (2 << CR1_V2_MODE_POS)
#define ALWAYS_ON               (3 << CR1_V2_MODE_POS)

// CR1 Standby Transition: Table 96
#define GOTO_V1_STANDBY         3
#define GOTO_VBAT_STANDBY       5
#define GOTO_ACTIVE             0

#define V1_STBY                 1
#define VBAT_STBY               2
#define ACTIVE_STBY             3

#define WU_EN_DIS               0
#define WU_PIN                  1

#define WU_PULL_DOWN            0
#define WU_PULL_UP              1

#define WU_WU                   0
#define WU_VBAT                 1

#define WU_FILTER_STATIC        0
#define WU_FILTER_T2            1
#define WU_FILTER_T1            2

#define WU_ENABLE_ITEM          0
#define WU_PIN_ITEM             1
#define WU_FILTER_ITEM          2

#define WU_PIN_EN_ITEM          0
#define WU_PIN_PULL_ITEM        1
#define WU_PIN_CONFIG_ITEM      2
#define WU_PIN_FILTER_ITEM      3

#define TMR_NINT_SEL_T2         0
#define TMR_NINT_SEL_T1         1

#define TMR_NINT_ENABLE_ITEM    0
#define TMR_NINT_SELECT_ITEM    1

// -------- CR2 ------------------------------------------------------------------------------------

#define CR2_T1_RESTART_MASK     0x800000
#define CR2_T1_DIR_MASK         0x400000
#define CR2_T1_ON_MASK          0x380000
#define CR2_T1_PER_MASK         0x070000
#define CR2_T2_RESTART_MASK     0x008000
#define CR2_T2_DIR_MASK         0x004000
#define CR2_T2_ON_MASK          0x003800
#define CR2_T2_PER_MASK         0x000700

#define CR2_LIN_RX_ONLY_MASK    0x000080
#define CR2_LIN_TXD_TO_EN_MASK  0x000040
#define CR2_CAN_LOOP_EN_MASK    0x000020
#define CR2_CAN_PNW_EN_MASK     0x000010
#define CR2_V1_RST_LEV_MASK     0x00000C
#define CR2_WD_TIME_MASK        0x000003

#define CR2_T1_MASK             (CR2_T1_RESTART_MASK | CR2_T1_DIR_MASK | CR2_T1_ON_MASK | CR2_T1_PER_MASK)
#define CR2_T2_MASK             (CR2_T2_RESTART_MASK | CR2_T2_DIR_MASK | CR2_T2_ON_MASK | CR2_T2_PER_MASK)

#define TYPE_TIMER1             1
#define TYPE_TIMER2             2

#define CR2_T1_RESTART_POS      23
#define CR2_T1_DIR_POS          22
#define CR2_T1_ON_POS           19
#define CR2_T1_PER_POS          16

#define CR2_T2_RESTART_POS      15
#define CR2_T2_DIR_POS          14
#define CR2_T2_ON_POS           11
#define CR2_T2_PER_POS          8

#define CR2_LIN_RX_ONLY_POS     7
#define CR2_LIN_TXD_TO_EN_POS   6
#define CR2_CAN_LOOP_EN_POS     5
#define CR2_CAN_PNW_EN_POS      4
#define CR2_V1_RST_LEV_POS      2
#define CR2_WD_TIME_POS         0

// CR2 Timers: Table 59 / Table 98
#define TMR_ONTIME_TON1         0           // 0.1 mS
#define TMR_ONTIME_TON2         1           // 0.3 mS
#define TMR_ONTIME_TON3         2           // 1 mS
#define TMR_ONTIME_TON4         3           // 10 mS
#define TMR_ONTIME_TON5         4           // 20 mS

#define TMR_ONTIME_TON1_DIR     0x08        // 0.1 mS DIR-controlled
#define TMR_ONTIME_TON2_DIR     0x09        // 0.3 mS DIR-controlled
#define TMR_ONTIME_TON3_DIR     0x0a        // 1 mS   DIR-controlled
#define TMR_ONTIME_TON4_DIR     0x0b        // 10 mS  DIR-controlled
#define TMR_ONTIME_TON5_DIR     0x0c        // 20 mS  DIR-controlled

#define TON_100US               TMR_ONTIME_TON1
#define TON_300US               TMR_ONTIME_TON2
#define TON_1MS                 TMR_ONTIME_TON3
#define TON_10MS                TMR_ONTIME_TON4
#define TON_20MS                TMR_ONTIME_TON5

#define TMR_PERTIME_T1          0           // 10 mS
#define TMR_PERTIME_T2          1           // 20 mS
#define TMR_PERTIME_T3          2           // 50 mS
#define TMR_PERTIME_T4          3           // 100 mS
#define TMR_PERTIME_T5          4           // 200 mS
#define TMR_PERTIME_T6          5           // 500 mS
#define TMR_PERTIME_T7          6           // 1000 mS
#define TMR_PERTIME_T8          7           // 2000 mS

#define T_10MS                  TMR_PERTIME_T1
#define T_20MS                  TMR_PERTIME_T2
#define T_50MS                  TMR_PERTIME_T3
#define T_100MS                 TMR_PERTIME_T4
#define T_200MS                 TMR_PERTIME_T5
#define T_500MS                 TMR_PERTIME_T6
#define T_1S                    TMR_PERTIME_T7
#define T_2S                    TMR_PERTIME_T8

// CR2 V1 Reset: Table 14
#define V1_RESET_VRT1           3           // ~3.5 V -0.25/+0.2 V
#define V1_RESET_VRT2           2           // ~3.8 V -0.25/+0.2 V
#define V1_RESET_VRT3           1           // ~4.0 V -0.25/+0.2 V
#define V1_RESET_VRT4           0           // ~4.3 V -0.2/+0.2 V
#define V1_RESET_3_5V           V1_RESET_VRT1
#define V1_RESET_3_8V           V1_RESET_VRT2
#define V1_RESET_4_0V           V1_RESET_VRT3
#define V1_RESET_4_3V           V1_RESET_VRT4

#define LIN_WU_EN_ITEM          0
#define LIN_REC_ONLY_ITEM       1
#define LIN_TXD_TOUT_EN_ITEM    2
#define LIN_WU_CONFIG_ITEM      3
#define LIN_HS_EN_ITEM          4

#define CAN_WU_EN_ITEM          0
#define CAN_RX_EN_ITEM          1
#define CAN_TX_EN_ITEM          2
#define CAN_TO_IRQ_ITEM         3
#define CAN_GO_TX_RDY_ITEM      4
#define CAN_LOOP_EN_ITEM        5
#define CAN_PNW_EN_ITEM         6

#define CR2_WD_TIME_POS         0

// CR2 WDT Time: Table 14
#define WDOG_TIME_TSW1          0           // 10 mS  (7.5 to 12 mS)
#define WDOG_TIME_TSW2          1           // 50 mS  (37.5 to 60 mS)
#define WDOG_TIME_TSW3          2           // 100 mS (75 to 120 mS)
#define WDOG_TIME_TSW4          3           // 200 mS (150 to 240 mS)
#define WDOG_TIME_10MS          WDOG_TIME_TSW1
#define WDOG_TIME_50MS          WDOG_TIME_TSW2
#define WDOG_TIME_100MS         WDOG_TIME_TSW3
#define WDOG_TIME_200MS         WDOG_TIME_TSW4

// -------- CR3 ------------------------------------------------------------------------------------

#define CR3_VSREG_LOCK_MASK     0x800000
#define CR3_VS_LOCK_MASK        0x400000
#define CR3_VSREG_OVSHD_MASK    0x200000
#define CR3_VSREG_UVSHD_MASK    0x100000
#define CR3_VS_OVSHD_MASK       0x080000
#define CR3_VS_UVSHD_MASK       0x040000
#define CR3_VSREG_EW_MASK       0x0003FF

#define CR3_VSREG_LOCK_POS      23
#define CR3_VS_LOCK_POS         22
#define CR3_VSREG_OVSHD_POS     21
#define CR3_VSREG_UVSHD_POS     20
#define CR3_VS_OVSHD_POS        19
#define CR3_VS_UVSHD_POS        18
#define CR3_VSREG_EW_POS        0

#define VS_LOCK                 0
#define VS_OVSHD                1
#define VS_UVSHD                2

#define VSREG_LOCK              3
#define VSREG_OVSHD             4
#define VSREG_UVSHD             5
#define VSREG_EW_THR            6

#define EW_THR_MAX              10.0F   // V

// -------- CR4 ------------------------------------------------------------------------------------

// output_type
#define OFF_OUT     0
#define ON_OUT      1
#define LO_OUT      0x11
#define HI_OUT      0x21

// output
#define OUT_1_HS    1
#define OUT_1_LS    2
#define OUT_2_HS    3
#define OUT_2_LS    4
#define OUT_3_HS    5
#define OUT_3_LS    6
#define OUT_6_HS    7
#define OUT_6_LS    8

#define CR4_HS_OUT1_MASK    0x200000
#define CR4_LS_OUT1_MASK    0x100000
#define CR4_HS_OUT2_MASK    0x020000
#define CR4_LS_OUT2_MASK    0x010000
#define CR4_HS_OUT3_MASK    0x002000
#define CR4_LS_OUT3_MASK    0x001000
#define CR4_HS_OUT6_MASK    0x000002
#define CR4_LS_OUT6_MASK    0x000001

#define CR4_HL_OUT1_MASK    (CR4_HS_OUT1_MASK | CR4_LS_OUT1_MASK)
#define CR4_HL_OUT2_MASK    (CR4_HS_OUT2_MASK | CR4_LS_OUT2_MASK)
#define CR4_HL_OUT3_MASK    (CR4_HS_OUT3_MASK | CR4_LS_OUT3_MASK)
#define CR4_HL_OUT6_MASK    (CR4_HS_OUT6_MASK | CR4_LS_OUT6_MASK)

#define CR4_HS_OUT1_POS     21
#define CR4_LS_OUT1_POS     20
#define CR4_HS_OUT2_POS     17
#define CR4_LS_OUT2_POS     16
#define CR4_HS_OUT3_POS     13
#define CR4_LS_OUT3_POS     12
#define CR4_HS_OUT6_POS     1
#define CR4_LS_OUT6_POS     0

// output_type
#define OFF_OUT         0
#define ON_OUT          1
#define TIMER1_OUT      2
#define TIMER2_OUT      3
#define PWM1_OUT        4
#define PWM2_OUT        5
#define PWM3_OUT        6
#define PWM4_OUT        7
#define PWM5_OUT        8
#define PWM6_OUT        9
#define PWM7_OUT        10
#define DIR_OUT         14

// output
#define OUT_1           1
#define OUT_2           2
#define OUT_3           3
#define OUT_6           6
#define OUT_7           7
#define OUT_8           8
#define OUT_9           9
#define OUT_10          10
#define OUT_13          13
#define OUT_14          14
#define OUT_15          15
#define OUT_GH          16
#define OUT_1_6         17
#define OUT_1_2_3_6     18

// motor outputs (X-motor, Y-motor, F-Fold motor)
#define OUT2_X          0
#define OUT3_Y          1
#define OUT6_F          2
#define OUT_XY          3
#define OUT_XF          4
#define OUT_YF          5

// motor direction
#define LEFT_DIRECTION      0
#define RIGHT_DIRECTION     1
#define BRAKE               2

// -------- CR5 ------------------------------------------------------------------------------------

#define CR5_HS_OUT7_MASK        0xF00000
#define CR5_HS_OUT8_MASK        0x0F0000
#define CR5_HS_OUT10_MASK       0x000F00
#define CR5_GH_MASK             0x000010

#define CR5_HS_OUT7_POS         20
#define CR5_HS_OUT8_POS         16
#define CR5_HS_OUT10_POS        8
#define CR5_GH_POS              4

// -------- CR6 ------------------------------------------------------------------------------------

#define CR6_HS_OUT9_MASK        0xF00000
#define CR6_HS_OUT13_MASK       0x0F0000
#define CR6_HS_OUT14_MASK       0x00F000
#define CR6_HS_OUT15_MASK       0x000F00

#define CR6_HS_OUT9_POS         20
#define CR6_HS_OUT13_POS        16
#define CR6_HS_OUT14_POS        12
#define CR6_HS_OUT15_POS        8

// -------- CR7 ------------------------------------------------------------------------------------

#define CR7_OUT1_OCR_MASK       0x800000
#define CR7_OUT2_OCR_MASK       0x400000
#define CR7_OUT3_OCR_MASK       0x200000
#define CR7_OUT6_OCR_MASK       0x100000
#define CR7_OUT7_OCR_MASK       0x080000
#define CR7_OUT8_OCR_MASK       0x040000
#define CR7_OUT15_OCR_MASK      0x020000
#define CR7_OUT1_SHORT_MASK     0x001000
#define CR7_OUT2_SHORT_MASK     0x000800
#define CR7_OUT3_SHORT_MASK     0x000400
#define CR7_OUT6_SHORT_MASK     0x000200
#define CR7_CM_DIR_MASK         0x000030
#define CR7_CM_SEL_MASK         0x00000F

#define CR7_OUT1_OCR_POS        23
#define CR7_OUT2_OCR_POS        22
#define CR7_OUT3_OCR_POS        21
#define CR7_OUT6_OCR_POS        20
#define CR7_OUT7_OCR_POS        19
#define CR7_OUT8_OCR_POS        18
#define CR7_OUT15_OCR_POS       17
#define CR7_OUT1_SHORT_POS      12
#define CR7_OUT2_SHORT_POS      11
#define CR7_OUT3_SHORT_POS      10
#define CR7_OUT6_SHORT_POS      9
#define CR7_CM_DIR_POS          4
#define CR7_CM_SEL_POS          0

#define CM_ALWAYS               0
#define DIR_STBY_CM_ACTIVE      1
#define DIR_ALWAYS              2
#define OFF_CM_DIR              3

#define SC_THRSH_OFF            1
#define SC_THRSH_ON             0

#define OUT_1_CM                0
#define OUT_2_CM                1
#define OUT_3_CM                2
#define OUT_6_CM                5
#define OUT_7_CM                6
#define OUT_8_CM                7
#define OUT_9_CM                8
#define OUT_10_CM               9
#define OUT_13_CM               12
#define OUT_14_CM               13
#define OUT_15_CM               14

// -------- CR8 ------------------------------------------------------------------------------------

#define CR8_OUT1_THX_EN_MASK        0x800000
#define CR8_OUT2_THX_EN_MASK        0x400000
#define CR8_OUT3_THX_EN_MASK        0x200000
#define CR8_OUT6_THX_EN_MASK        0x100000
#define CR8_OUT7_THX_EN_MASK        0x080000
#define CR8_OUT8_THX_EN_MASK        0x040000
#define CR8_OUT15_THX_EN_MASK       0x020000

#define CR8_OUT7_TIME_MASK          0x00C000
#define CR8_OUT8_TIME_MASK          0x003000
#define CR8_OUT15_TIME_MASK         0x000C00
#define CR8_OUT1_2_3_6_TIME_MASK    0x000300
#define CR8_OUT7_FREQ_MASK          0x0000C0
#define CR8_OUT8_FREQ_MASK          0x000030
#define CR8_OUT15_FREQ_MASK         0x00000C
#define CR8_OUT1_2_3_6_FREQ_MASK    0x000003

#define CR8_OUT1_THX_EN_POS         23
#define CR8_OUT2_THX_EN_POS         22
#define CR8_OUT3_THX_EN_POS         21
#define CR8_OUT6_THX_EN_POS         20
#define CR8_OUT7_THX_EN_POS         19
#define CR8_OUT8_THX_EN_POS         18
#define CR8_OUT15_THX_EN_POS        17

#define CR8_OUT7_TIME_POS           14
#define CR8_OUT8_TIME_POS           12
#define CR8_OUT15_TIME_POS          10
#define CR8_OUT1_2_3_6_TIME_POS     8
#define CR8_OUT7_FREQ_POS           6
#define CR8_OUT8_FREQ_POS           4
#define CR8_OUT15_FREQ_POS          2
#define CR8_OUT1_2_3_6_FREQ_POS     0

#define OCR_TON_88US                0
#define OCR_TON_80US                1
#define OCR_TON_72US                2
#define OCR_TON_64US                3

#define OCR_FREQ_1_7KHZ             0
#define OCR_FREQ_2_2KHZ             1
#define OCR_FREQ_3_0KHZ             2
#define OCR_FREQ_4_4KHZ             3

// -------- CR9 ------------------------------------------------------------------------------------

#define CR9_OUT8_RSDON_MASK     0x800000
#define CR9_OUT7_RSDON_MASK     0x400000
#define CR9_OUT1_6_RSDON_MASK   0x200000

#define CR9_OUT9_CCM_EN_MASK    0x100000
#define CR9_OUT8_CCM_EN_MASK    0x080000
#define CR9_OUT7_CCM_EN_MASK    0x040000

#define CR9_OUT15_OL_MASK       0x004000
#define CR9_OUT14_OL_MASK       0x002000
#define CR9_OUT13_OL_MASK       0x001000
#define CR9_OUT10_OL_MASK       0x000200
#define CR9_OUT9_OL_MASK        0x000100

#define CR9_OUT15_OC_MASK       0x000040
#define CR9_OUT14_OC_MASK       0x000020
#define CR9_OUT13_OC_MASK       0x000010
#define CR9_OUT10_OC_MASK       0x000002
#define CR9_OUT9_OC_MASK        0x000001

#define CR9_OUT8_RSDON_POS      23
#define CR9_OUT7_RSDON_POS      22
#define CR9_OUT1_6_RSDON_POS    21

#define CR9_OUT9_CCM_EN_POS     20
#define CR9_OUT8_CCM_EN_POS     19
#define CR9_OUT7_CCM_EN_POS     18

#define CR9_OUT15_OL_POS        14
#define CR9_OUT14_OL_POS        13
#define CR9_OUT13_OL_POS        12
#define CR9_OUT10_OL_POS        9
#define CR9_OUT9_OL_POS         8

#define CR9_OUT15_OC_POS        6
#define CR9_OUT14_OC_POS        5
#define CR9_OUT13_OC_POS        4
#define CR9_OUT10_OC_POS        1
#define CR9_OUT9_OC_POS         0

#define RDS_ON_LO               0
#define RDS_ON_HI               1

#define HI_CURRENT_MODE         0
#define LO_CURRENT_MODE         1

// -------- CR10 -----------------------------------------------------------------------------------

#define CR10_HB_DS_DIAG_MASK    0xE00000
#define CR10_HB_A_DIR_MASK      0x100000
#define CR10_HB_B_SD2_MASK      0x080000
#define CR10_HB_B_SDS2_MASK     0x040000
#define CR10_HB_B_SD1_MASK      0x020000
#define CR10_HB_B_SDS1_MASK     0x010000
#define CR10_HB_A_SD2_MASK      0x008000
#define CR10_HB_A_SDS2_MASK     0x004000
#define CR10_HB_A_SD1_MASK      0x002000
#define CR10_HB_A_SDS1_MASK     0x001000
#define CR10_HB_CCPT_MASK       0x000F00
#define CR10_HB_OLTH_HI_MASK    0x000080
#define CR10_HB_OLH1L2_MASK     0x000040
#define CR10_HB_OLH2L1_MASK     0x000020
#define CR10_HB_SLEW_MASK       0x00001F

#define CR10_HB_DS_DIAG_POS     21
#define CR10_HB_A_DIR_POS       20
#define CR10_HB_B_SD2_POS       19
#define CR10_HB_B_SDS2_POS      18
#define CR10_HB_B_SD1_POS       17
#define CR10_HB_B_SDS1_POS      16
#define CR10_HB_A_SD2_POS       15
#define CR10_HB_A_SDS2_POS      14
#define CR10_HB_A_SD1_POS       13
#define CR10_HB_A_SDS1_POS      12
#define CR10_HB_CCPT_POS        8
#define CR10_HB_OLTH_HI_POS     7
#define CR10_HB_OLH1L2_POS      6
#define CR10_HB_OLH2L1_POS      5
#define CR10_HB_SLEW_POS        0

#define LEFT_DIRECTION          0
#define RIGHT_DIRECTION         1

#define ACTIVE_FREEWHEELING     0
#define PASSIVE_FREEWHEELING    1

#define LOW_SIDE_FREEWHEELING   0
#define HIGH_SIDE_FREEWHEELING  1

#define LOW_SIDE_DRIVE          0
#define HIGH_SIDE_DRIVE         1

#define HB_LEG1                 1
#define HB_LEG2                 2

#define OL_H1L2                 0
#define OL_H2L1                 1

#define T_250ns                 0
#define T_500ns                 1
#define T_750ns                 2
#define T_1000ns                3
#define T_1250ns                4
#define T_1500ns                5
#define T_1750ns                6
#define T_2000ns                7
#define T_2250ns                8
#define T_2500ns                9
#define T_2750ns                10
#define T_3000ns                11
#define T_3250ns                12
#define T_3500ns                13
#define T_3750ns                14
#define T_4000ns                15

#define  DS_500mV               0
#define  DS_750mV               1
#define  DS_1000mV              2
#define  DS_1250mV              3
#define  DS_1500mV              4
#define  DS_1750mV              5
#define  DS_2000mV              6

#define  SLEW_MAX               32  // mA

// -------- CR11 -----------------------------------------------------------------------------------

#define CR11_GHOL_EN_MASK       0x020000
#define CR11_GH_TH_MASK         0x01C000
#define CR11_ECV_LS_MASK        0x002000
#define CR11_ECV_OCR_MASK       0x001000
#define CR11_EC_ON_MASK         0x000100
#define CR11_EC_VALUE_MASK      0x00003F

#define CR11_GHOL_EN_POS        17
#define CR11_GH_TH_POS          14
#define CR11_ECV_LS_POS         13
#define CR11_ECV_OCR_POS        12
#define CR11_EC_ON_POS          8
#define CR11_EC_VALUE_POS       0

#define OL_DIAG_OFF             0
#define OL_DIAG_ON              1

// Table 24.
#define GH_TH_VSCD1_HE          0           // 160 200 250 mV
#define GH_TH_VSCD2_HE          1           // 200 250 305 mV
#define GH_TH_VSCD3_HE          2           // 240 300 360 mV
#define GH_TH_VSCD4_HE          3           // 280 350 420 mV
#define GH_TH_VSCD5_HE          4           // 320 400 480 mV
#define GH_TH_VSCD6_HE          5           // 360 450 540 mV
#define GH_TH_VSCD7_HE          6           // 400 500 600 mV
#define GH_TH_VSCD8_HE          7           // 440 550 660 mV - default
#define GH_TH_200_MV            GH_TH_VSCD1_HE
#define GH_TH_250_MV            GH_TH_VSCD2_HE
#define GH_TH_300_MV            GH_TH_VSCD3_HE
#define GH_TH_350_MV            GH_TH_VSCD4_HE
#define GH_TH_400_MV            GH_TH_VSCD5_HE
#define GH_TH_450_MV            GH_TH_VSCD6_HE
#define GH_TH_500_MV            GH_TH_VSCD7_HE
#define GH_TH_550_MV            GH_TH_VSCD8_HE

#define GH_200_MV_VAL           200         // mV
#define GH_250_MV_VAL           250         // mV
#define GH_300_MV_VAL           300         // mV
#define GH_350_MV_VAL           350         // mV
#define GH_400_MV_VAL           400         // mV
#define GH_450_MV_VAL           450         // mV
#define GH_500_MV_VAL           500         // mV
#define GH_550_MV_VAL           550         // mV

// -------- CR12 -----------------------------------------------------------------------------------

#define CR12_PWM1_FREQ_MASK     0xC00000
#define CR12_PWM2_FREQ_MASK     0x300000
#define CR12_PWM3_FREQ_MASK     0x0C0000
#define CR12_PWM4_FREQ_MASK     0x030000
#define CR12_PWM5_FREQ_MASK     0x00C000
#define CR12_PWM6_FREQ_MASK     0x003000
#define CR12_PWM7_FREQ_MASK     0x000C00

#define CR12_PWM1_FREQ_POS      22
#define CR12_PWM2_FREQ_POS      20
#define CR12_PWM3_FREQ_POS      18
#define CR12_PWM4_FREQ_POS      16
#define CR12_PWM5_FREQ_POS      14
#define CR12_PWM6_FREQ_POS      12
#define CR12_PWM7_FREQ_POS      10

// Table 18
#define PWM_FREQ_FPWM1          0           // 100 Hz
#define PWM_FREQ_FPWM2          1           // 200 Hz
#define PWM_FREQ_FPWM3          2           // 330 Hz
#define PWM_FREQ_FPWM4          3           // 500 Hz
#define PWM_FREQ_100HZ          PWM_FREQ_FPWM1
#define PWM_FREQ_200HZ          PWM_FREQ_FPWM2
#define PWM_FREQ_330HZ          PWM_FREQ_FPWM3
#define PWM_FREQ_500HZ          PWM_FREQ_FPWM4
#define PWM_FREQ_100HZ_VAL      100
#define PWM_FREQ_200HZ_VAL      200
#define PWM_FREQ_330HZ_VAL      330
#define PWM_FREQ_500HZ_VAL      500

#define PWM_CHAN1               1
#define PWM_CHAN2               2
#define PWM_CHAN3               3
#define PWM_CHAN4               4
#define PWM_CHAN5               5
#define PWM_CHAN6               6
#define PWM_CHAN7               7

// -------- CR13 - CR16 ----------------------------------------------------------------------------

// CR13 to CR16
#define HI_PWM_DC_MASK          0x3FF000
#define LO_PWM_DC_MASK          0x0003FF

#define HI_PWM_DC_POS           12
#define LO_PWM_DC_POS           0

// -------- CR17 - CR20 ----------------------------------------------------------------------------

// CR17 to CR20
#define ODD_VLED_MASK           0x3FF000
#define EVEN_VLED_MASK          0x0003FF

#define ODD_OUT_AUTOCOMP_EN     0x400000
#define EVEN_OUT_AUTOCOMP_EN    0x000400

#define ODD_VLED_POS            12
#define EVEN_VLED_POS           0

#define ODD_OUT_AUTOCOMP_EN_POS  22
#define EVEN_OUT_AUTOCOMP_EN_POS 10

#define VAINVS                  22.0F   // V
#define VLED_MAX                10.0F   // V

// -------- CR21 -----------------------------------------------------------------------------------

#define CR21_HB_DS_DIAG_MASK    0xE00000
#define CR21_HB_B_DIR_MASK      0x100000
#define CR21_HB_CCPT_MASK       0x000F00
#define CR21_HB_OLTH_HI_MASK    0x000080
#define CR21_HB_OLH1L2_MASK     0x000040
#define CR21_HB_OLH2L1_MASK     0x000020
#define CR21_HB_SLEW_MASK       0x00001F

#define CR21_HB_DS_DIAG_POS     21
#define CR21_HB_B_DIR_POS       20
#define CR21_HB_CCPT_POS        8
#define CR21_HB_OLTH_HI_POS     7
#define CR21_HB_OLH1L2_POS      6
#define CR21_HB_OLH2L1_POS      5
#define CR21_HB_SLEW_POS        0

// -------- CR22 -----------------------------------------------------------------------------------

#define CR22_GEN_MODE_EN_MASK   0x000010
#define CR22_DEBUG_EXIT_MASK    0x000008
#define CR22_CP_OFF_MASK        0x000004
#define CR22_ICMP_MASK          0x000002
#define CR22_WDOG_EN_MASK       0x000001

#define CR22_GEN_MODE_EN_POS    4
#define CR22_DEBUG_EXIT_POS     3
#define CR22_CP_OFF_POS         2
#define CR22_ICMP_POS           1
#define CR22_WDOG_EN_POS        0

#define DEBUG_STAY              0
#define DEBUG_EXIT              1

#define CP_ENABLE               0
#define CP_DISABLE              1

#define IC_ENABLE               0
#define IC_DISABLE              1

#define GEN_MODE_ENABLE         0
#define SW_DEBUG_EXIT           1
#define WDOG_ENABLE             2
#define TSD_ENABLE              3

// -------- CFR ------------------------------------------------------------------------------------

#define CFR_WU_CFG_MASK         0x800000
#define CFR_LIN_WU_CFG_MASK     0x400000
#define CFR_LIN_HS_EN_MASK      0x200000
#define CFR_TSD_CFG_MASK        0x100000
#define CFR_ECV_HV_MASK         0x080000
#define CFR_V2_CFG_MASK         0x040000
#define CFR_ICMP_CFG_EN_MASK    0x020000
#define CFR_WD_CFG_EN_MASK      0x010000
#define CFR_MASK_OL_HS_MASK     0x008000
#define CFR_MASK_OL_LS_MASK     0x004000
#define CFR_MASK_TW_MASK        0x002000
#define CFR_MASK_EC_OL_MASK     0x001000
#define CFR_MASK_OL_MASK        0x000800
#define CFR_MASK_SPIE_MASK      0x000400
#define CFR_MASK_PLE_MASK       0x000200
#define CFR_MASK_GW_MASK        0x000100
#define CFR_CP_OFF_EN_MASK      0x000080
#define CFR_CP_LOW_CFG_MASK     0x000040
#define CFR_CP_DITH_DIS_MASK    0x000020
#define CFR_FS_FORCED_MASK      0x000010
#define CFR_DM_HB_A_MASK        0x000004
#define CFR_DM_HB_B_MASK        0x000002
#define CFR_WDC_MASK            0x000001

#define CFR_WU_CFG_POS          23
#define CFR_LIN_WU_CFG_POS      22
#define CFR_LIN_HS_EN_POS       21
#define CFR_TSD_CFG_POS         20
#define CFR_ECV_H_POS           19
#define CFR_V2_CFG_POS          18
#define CFR_ICMP_CFG_EN_POS     17
#define CFR_WD_CFG_EN_POS       16
#define CFR_MASK_OL_HS_POS      15
#define CFR_MASK_OL_LS_POS      14
#define CFR_MASK_TW_POS         13
#define CFR_MASK_EC_OL_POS      12
#define CFR_MASK_OL_POS         11
#define CFR_MASK_SPIE_POS       10
#define CFR_MASK_PLE_POS        9
#define CFR_MASK_GW_POS         8
#define CFR_CP_OFF_EN_POS       7
#define CFR_CP_LOW_CFG_POS      6
#define CFR_CP_DITH_DIS_POS     5
#define CFR_FS_FORCED_POS       4
#define CFR_DM_HB_A_POS         2
#define CFR_DM_HB_B_POS         1

#define REC_DOM_REC_WU          0
#define REC_DOM_TRANSITION_WU   1

#define SPD_LO_20KBS            0
#define SPD_LO_115KBS           1

#define ECV_1_2                 0
#define ECV_1_5                 1

#define ECV_1_2_VOLTS           1.2F
#define ECV_1_5_VOLTS           1.5F

#define V2_VREG                 0
#define V2_TRACK_V1             1

#define SINGLE_MOTOR            0
#define DUAL_MOTOR              1

// CFR V2 Configuration Type
#define V2_VREG_TYPE            ((uint32_t)V2_VREG << CFR_V2_CFG_POS)
#define V2_TRACK_V1_TYPE        ((uint32_t)V2_TRACK_V1 << CFR_V2_CFG_POS)

#define DO_UNMASK               0
#define DO_MASK                 1

#define HS_OL_ITEM              0
#define LS_OL_ITEM              1
#define TW_ITEM                 2
#define EC_OL_ITEM              3
#define OL_ITEM                 4
#define SPIE_ITEM               5
#define PLE_ITEM                6
#define GW_ITEM                 7
#define ALL_ITEMS               99

#define CP_LOW_CFG_LATCHED      0
#define CP_LOW_CFG_LIVE         1

#define IC_DITH_ENABLED         0
#define IC_DITH_DISABLED        1

// -------- GSB ------------------------------------------------------------------------------------

// Global Status Byte GSB
#define GSB_GSBN_MASK           0x000080
#define GSB_RSTB_MASK           0x000040
#define GSB_SPIE_MASK           0x000020
#define GSB_PLE_MASK            0x000010
#define GSB_FE_MASK             0x000008
#define GSB_DE_MASK             0x000004
#define GSB_GW_MASK             0x000002
#define GSB_FS_MASK             0x000001

// -------- SR1 ------------------------------------------------------------------------------------

#define SR1_VSPOR               0x000001
#define SR1_WD_FAIL             0x000002
#define SR1_F_SLEEP_WDC         0x000004
#define SR1_F_SLEEP_TSD         0x000008
#define SR1_TSD1                0x000010
#define SR1_TSD2                0x000020
#define SR1_DEV_STATE_MASK      0x0000C0
#define SR1_WDC_FAIL_CNT_MASK   0x000F00
#define SR1_V1_RESTRT_CNT_MASK  0x007000
#define SR1_V1_UV               0x008000
#define SR1_DEBUG_MODE          0x010000
#define SR1_WK_TIMER            0x020000
#define SR1_WK_LIN              0x040000
#define SR1_WK_CAN              0x080000
#define SR1_WK_WU               0x100000
#define SR1_SGND_LOSS           0x200000
#define SR1_WU_PIN_STATE        0x400000
#define SR1_WK_VS_OV            0x800000

#define SR1_VSPOR_POS           0
#define SR1_WD_FAIL_POS         1
#define SR1_F_SLEEP_WDC_POS     2
#define SR1_F_SLEEP_TSD_POS     3
#define SR1_TSD1_POS            4
#define SR1_TSD2_POS            5
#define SR1_DEV_STATE_POS       6
#define SR1_WDC_FAIL_CNT_POS    8
#define SR1_V1_RESTRT_CNT_POS   12
#define SR1_V1_UV_POS           15
#define SR1_DEBUG_MODE_POS      16
#define SR1_WK_TIMER_POS        17
#define SR1_WK_LIN_POS          18
#define SR1_WK_CAN_POS          19
#define SR1_WK_WU_POS           20
#define SR1_SGND_LOSS_POS       21
#define SR1_WU_PIN_STATE_POS    22
#define SR1_WK_VS_OV_POS        23

#define DEV_STATE_ACTIVE        0
#define DEV_STATE_WUV1          1
#define DEV_STATE_WUVBAT        2
#define DEV_STATE_FLASH         3

#define TSD1_ITEM               1
#define TSD2_ITEM               2

#define LOW_SIDE_1_MOSFET       0
#define HIGH_SIDE_1_MOSFET      1
#define LOW_SIDE_2_MOSFET       2
#define HIGH_SIDE_2_MOSFET      3
#define ALL_HB_SIDE_ITEMS       99

#define FORCED_SLEEP_WD_ITEM            0
#define FORCED_SLEEP_TSD2_V1SC_ITEM     1
#define ALL_FORCED_SLEEP_ITEMS          99

#define SGND_LOSS_ITEM          0
#define DEBUG_MODE_ITEM         1
#define VSPOR_ITEM              2
#define ALL_MISC_ITEMS          99

#define WK_TIMER_ITEM           0
#define WK_LIN_ITEM             1
#define WK_CAN_ITEM             2
#define WK_WU_ITEM              3
#define WK_VS_OV_ITEM           4
#define WK_CAN_WUP_ITEM         5
#define ALL_WK_ITEMS            99

#define PIN_LOW                 0
#define PIN_HIGH                1

// -------- SR2 ------------------------------------------------------------------------------------

#define SR2_VS_UV               0x000001
#define SR2_VS_OV               0x000002
#define SR2_VSREG_UV            0x000004
#define SR2_VSREG_OV            0x000008
#define SR2_VSREG_EW            0x000010
#define SR2_V1_FAIL             0x000020
#define SR2_V2_FAIL             0x000040
#define SR2_V2_SC               0x000080
#define SR2_TW                  0x000100
#define SR2_CP_LOW              0x000200
#define SR2_SPI_CLK_CNT         0x000400
#define SR2_SPI_INV_CMD         0x000800
#define SR2_DS_MON_LS1_A        0x001000
#define SR2_DS_MON_LS2_A        0x002000
#define SR2_DS_MON_HS1_A        0x004000
#define SR2_DS_MON_HS2_A        0x008000
#define SR2_CAN_SUP_LOW         0x010000
#define SR2_CAN_TXD_DOM         0x020000
#define SR2_CAN_PERM_DOM        0x040000
#define SR2_CAN_PERM_REC        0x080000
#define SR2_CAN_RXD_REC         0x100000
#define SR2_LIN_PERM_REC        0x200000
#define SR2_LIN_TXD_DOM         0x400000
#define SR2_LIN_PERM_DOM        0x800000

#define SR2_VS_UV_POS           0
#define SR2_VS_OV_POS           1
#define SR2_VSREG_UV_POS        2
#define SR2_VSREG_OV_POS        3
#define SR2_VSREG_EW_POS        4
#define SR2_V1_FAIL_POS         5
#define SR2_V2_FAIL_POS         6
#define SR2_V2_SC_POS           7
#define SR2_TW_POS              8
#define SR2_CP_LOW_POS          9
#define SR2_SPI_CLK_CNT_POS     10
#define SR2_SPI_INV_CMD_POS     11
#define SR2_DS_MON_LS1_A_POS    12
#define SR2_DS_MON_LS2_A_POS    13
#define SR2_DS_MON_HS1_A_POS    14
#define SR2_DS_MON_HS2_A_POS    15
#define SR2_CAN_SUP_LOW_POS     16
#define SR2_CAN_TXD_DOM_POS     17
#define SR2_CAN_PERM_DOM_POS    18
#define SR2_CAN_PERM_REC_POS    19
#define SR2_CAN_RXD_REC_POS     20
#define SR2_LIN_PERM_REC_POS    21
#define SR2_LIN_TXD_DOM_POS     22
#define SR2_LIN_PERM_DOM_POS    23

#define SPI_CLK_CNT             0
#define SPI_INV_CMD             1

#define V1_UV                   0
#define VS_UV                   1
#define VS_OV                   2
#define VSREG_UV                3
#define VSREG_OV                4
#define VSREG_EW                5
#define V1_FAIL                 6
#define V2_FAIL                 7
#define V2_SC                   8
#define CP_LOW                  9
#define ALL_VOLT_ITEMS          99

#define CAN_SUP_LOW_ITEM        0
#define CAN_TXD_DOM_ITEM        1
#define CAN_PERM_DOM_ITEM       2
#define CAN_PERM_REC_ITEM       3
#define CAN_RXD_REC_ITEM        4
#define ALL_CAN_ITEMS           99

#define LIN_PERM_REC_ITEM       0
#define LIN_TXD_DOM_ITEM        1
#define LIN_PERM_DOM_ITEM       2
#define ALL_LIN_ITEMS           99

#define THERMW_ITEM             1

// -------- SR3 ------------------------------------------------------------------------------------

#define SR3_LSA_FSO_OC          0x000001
#define SR3_LSB_FSO_OC          0x000002
#define SR3_DS_MON_LS1_B        0x000008
#define SR3_DS_MON_LS2_B        0x000010
#define SR3_DS_MON_HS1_B        0x000020
#define SR3_DS_MON_HS2_B        0x000040
#define SR3_OUT15_OCTHX         0x000200
#define SR3_OUT14_OCTHX         0x000400
#define SR3_OUT13_OCTHX         0x000800
#define SR3_OUT10_OCTHX         0x001000
#define SR3_OUT9_OCTHX          0x002000
#define SR3_OUT8_OCTHX          0x004000
#define SR3_OUT7_OCTHX          0x008000
#define SR3_OUT6L_OCTHX         0x010000
#define SR3_OUT6H_OCTHX         0x020000
#define SR3_OUT3L_OCTHX         0x040000
#define SR3_OUT3H_OCTHX         0x080000
#define SR3_OUT2L_OCTHX         0x100000
#define SR3_OUT2H_OCTHX         0x200000
#define SR3_OUT1L_OCTHX         0x400000
#define SR3_OUT1H_OCTHX         0x800000

#define SR3_LSA_FSO_OC_POS      0
#define SR3_LSB_FSO_OC_POS      1
#define SR3_DS_MON_LS1_B_POS    3
#define SR3_DS_MON_LS2_B_POS    4
#define SR3_DS_MON_HS1_B_POS    5
#define SR3_DS_MON_HS2_B_POS    6
#define SR3_OUT15_OCTHX_POS     9
#define SR3_OUT14_OCTHX_POS     10
#define SR3_OUT13_OCTHX_POS     11
#define SR3_OUT10_OCTHX_POS     12
#define SR3_OUT9_OCTHX_POS      13
#define SR3_OUT8_OCTHX_POS      14
#define SR3_OUT7_OCTHX_POS      15
#define SR3_OUT6L_OCTHX_POS     16
#define SR3_OUT6H_OCTHX_POS     17
#define SR3_OUT3L_OCTHX_POS     18
#define SR3_OUT3H_OCTHX_POS     19
#define SR3_OUT2L_OCTHX_POS     20
#define SR3_OUT2H_OCTHX_POS     21
#define SR3_OUT1L_OCTHX_POS     22
#define SR3_OUT1H_OCTHX_POS     23

#define OUT_7_OC                9
#define OUT_8_OC                10
#define OUT_9_OC                11
#define OUT_10_OC               12
#define OUT_13_OC               13
#define OUT_14_OC               14
#define OUT_15_OC               15
#define LSA_FSO_OC              30
#define LSB_FSO_OC              31

// -------- SR4 ------------------------------------------------------------------------------------

#define SR4_OUT6L_SHORT         0x00001
#define SR4_OUT6H_SHORT         0x00002
#define SR4_OUT3L_SHORT         0x00004
#define SR4_OUT3H_SHORT         0x00008
#define SR4_OUT2L_SHORT         0x00010
#define SR4_OUT2H_SHORT         0x00020
#define SR4_OUT1L_SHORT         0x00040
#define SR4_OUT1H_SHORT         0x00080
#define SR4_OUT15_OCRAL         0x002000
#define SR4_OUT8_OCRAL          0x004000
#define SR4_OUT7_OCRAL          0x008000
#define SR4_OUT6L_OCRAL         0x010000
#define SR4_OUT6H_OCRAL         0x020000
#define SR4_OUT3L_OCRAL         0x040000
#define SR4_OUT3H_OCRAL         0x080000
#define SR4_OUT2L_OCRAL         0x100000
#define SR4_OUT2H_OCRAL         0x200000
#define SR4_OUT1L_OCRAL         0x400000
#define SR4_OUT1H_OCRAL         0x800000

#define SR4_OUT6L_SHORT_POS     0
#define SR4_OUT6H_SHORT_POS     1
#define SR4_OUT3L_SHORT_POS     2
#define SR4_OUT3H_SHORT_POS     3
#define SR4_OUT2L_SHORT_POS     4
#define SR4_OUT2H_SHORT_POS     5
#define SR4_OUT1L_SHORT_POS     6
#define SR4_OUT1H_SHORT_POS     7
#define SR4_OUT15_OCRAL_POS     13
#define SR4_OUT8_OCRAL_POS      14
#define SR4_OUT7_OCRAL_POS      15
#define SR4_OUT6L_OCRAL_POS     16
#define SR4_OUT6H_OCRAL_POS     17
#define SR4_OUT3L_OCRAL_POS     18
#define SR4_OUT3H_OCRAL_POS     19
#define SR4_OUT2L_OCRAL_POS     20
#define SR4_OUT2H_OCRAL_POS     21
#define SR4_OUT1L_OCRAL_POS     22
#define SR4_OUT1H_OCRAL_POS     23

#define OUT_7_OCR               9
#define OUT_8_OCR               10
#define OUT_15_OCR              15

#define OUT_ALL_SC_ITEMS        99
#define OUT_ALL_OCS_ITEMS       99

// -------- SR5 ------------------------------------------------------------------------------------

#define SR5_ECV_OC              0x000001
#define SR5_DS_MON_HEAT         0x000002
#define SR5_OUTECV_OL           0x000080
#define SR5_OUTGH_OL            0x000100
#define SR5_OUT15_OL            0x000200
#define SR5_OUT14_OL            0x000400
#define SR5_OUT13_OL            0x000800
#define SR5_OUT10_OL            0x001000
#define SR5_OUT9_OL             0x002000
#define SR5_OUT8_OL             0x004000
#define SR5_OUT7_OL             0x008000
#define SR5_OUT6L_OL            0x010000
#define SR5_OUT6H_OL            0x020000
#define SR5_OUT3L_OL            0x040000
#define SR5_OUT3H_OL            0x080000
#define SR5_OUT2L_OL            0x100000
#define SR5_OUT2H_OL            0x200000
#define SR5_OUT1L_OL            0x400000
#define SR5_OUT1H_OL            0x800000

#define SR5_ECV_OC_POS          0
#define SR5_DS_MON_HEAT_POS     1
#define SR5_OUTECV_OL_POS       7
#define SR5_OUTGH_OL_POS        8
#define SR5_OUT15_OL_POS        9
#define SR5_OUT14_OL_POS        10
#define SR5_OUT13_OL_POS        11
#define SR5_OUT10_OL_POS        12
#define SR5_OUT9_OL_POS         13
#define SR5_OUT8_OL_POS         14
#define SR5_OUT7_OL_POS         15
#define SR5_OUT6L_OL_POS        16
#define SR5_OUT6H_OL_POS        17
#define SR5_OUT3L_OL_POS        18
#define SR5_OUT3H_OL_POS        19
#define SR5_OUT2L_OL_POS        20
#define SR5_OUT2H_OL_POS        21
#define SR5_OUT1L_OL_POS        22
#define SR5_OUT1H_OL_POS        23

#define OUT_7_OL                9
#define OUT_8_OL                10
#define OUT_9_OL                11
#define OUT_10_OL               12
#define OUT_13_OL               13
#define OUT_14_OL               14
#define OUT_15_OL               15
#define OUT_ECV                 19
#define DS_MON_HEAT             20

#define OUT_ALL_OL_ITEMS        99

// -------- SR6 ------------------------------------------------------------------------------------

#define SR6_WD_TMR_STATE_MASK   0xC00000
#define SR6_ECV_VNR_MASK        0x020000
#define SR6_ECV_VHI_MASK        0x010000
#define SR6_TW_CL6_MASK         0x002000
#define SR6_TW_CL5_MASK         0x001000
#define SR6_TW_CL4_MASK         0x000800
#define SR6_TW_CL3_MASK         0x000400
#define SR6_TW_CL2_MASK         0x000200
#define SR6_TW_CL1_MASK         0x000100
#define SR6_TSD_CL6_MASK        0x000020
#define SR6_TSD_CL5_MASK        0x000010
#define SR6_TSD_CL4_MASK        0x000008
#define SR6_TSD_CL3_MASK        0x000004
#define SR6_TSD_CL2_MASK        0x000002
#define SR6_TSD_CL1_MASK        0x000001

#define SR6_WD_TMR_STATE_POS    22
#define SR6_ECV_VNR_POS         17
#define SR6_ECV_VHI_POS         16
#define SR6_TW_CL6_POS          13
#define SR6_TW_CL5_POS          12
#define SR6_TW_CL4_POS          11
#define SR6_TW_CL3_POS          10
#define SR6_TW_CL2_POS          9
#define SR6_TW_CL1_POS          8
#define SR6_TSD1_CL6_POS        5
#define SR6_TSD1_CL5_POS        4
#define SR6_TSD1_CL4_POS        3
#define SR6_TSD1_CL3_POS        2
#define SR6_TSD1_CL2_POS        1
#define SR6_TSD1_CL1_POS        0

#define WD_TMR_STATE_0_33_PCT   0
#define WD_TMR_STATE_33_66_PCT  1
#define WD_TMR_STATE_66_100_PCT 3

#define ECV_VNR                 0
#define ECV_VHI                 1

#define TSD1_CL1_ITEM           11
#define TSD1_CL2_ITEM           12
#define TSD1_CL3_ITEM           13
#define TSD1_CL4_ITEM           14
#define TSD1_CL5_ITEM           15
#define TSD1_CL6_ITEM           16

#define TW_CL1_ITEM             11
#define TW_CL2_ITEM             12
#define TW_CL3_ITEM             13
#define TW_CL4_ITEM             14
#define TW_CL5_ITEM             15
#define TW_CL6_ITEM             16

#define TH_ALL_ITEMS            99


// -------- SR7 - SR9 ------------------------------------------------------------------------------

// Thermal Clusters
#define TEMP_CL1   0
#define TEMP_CL2   1
#define TEMP_CL3   2
#define TEMP_CL4   3
#define TEMP_CL5   4
#define TEMP_CL6   5

// SR7 to SR9
#define EVEN_TEMP_CL_MASK       0x3FF000
#define ODD_TEMP_CL_MASK        0x0003FF

#define EVEN_TEMP_CL_POS        12
#define ODD_TEMP_CL_POS         0

// -------- SR10 -----------------------------------------------------------------------------------

#define VS_REG_MASK             0x3FF000
#define VS_REG_POS              12

// -------- SR11 -----------------------------------------------------------------------------------

#define VS_MASK                 0x3FF000
#define VWU_MASK                0x0003FF

#define VS_POS                  12
#define VWU_POS                 0

#define VS_REG_V                0
#define VS_V                    1
#define VWU_V                   2

// -------- SR12 -----------------------------------------------------------------------------------

#define SR12_CAN_WUP            0x000004
#define SR12_CANTO              0x000008
#define SR12_CAN_SILENT         0x000020

#define SR12_CAN_WUP_POS        2
#define SR12_CANTO_POS          3
#define SR12_CAN_SILENT_POS     5

#define CAN_WUP_ITEM            0
#define CANTO_ITEM              5
#define CAN_SILENT_ITEM         6

#endif

#endif  // ____L99DZ200G_H___H__

