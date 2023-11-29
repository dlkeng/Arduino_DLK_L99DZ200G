/** \file DLK_L99DZ200G.h */
/*
 * NAME: DLK_L99DZ200G.h
 *
 * WHAT:
 *  Header file for DLK_L99DZ200G Arduino L99DZ200G driver class.
 *
 * SPECIAL CONSIDERATIONS:
 *  Derived from: STMicro AutoDevKit examples
 *
 * AUTHOR:
 *  D.L. Karmann
 *
 */
#ifndef __DLK_L99DZ200G_H__
#define __DLK_L99DZ200G_H__

#include <SPI.h>

#include "Arduino.h"
#include "L99DZ200G.h"

#define TIMER_EXPIRED(start, interval)  ((millis() - start) >= interval)

#define SPI_DUMMY_BYTE  0x00
#define FRAME_CNT       4

/**
 * DLK_L99DZ200G Arduino L99DZ200G driver library class. Version: "V1.0.2 12/29/2023"
 */
class DLK_L99DZ200G
{
    public:
        // Constructor
        /**
         *  A constructor that sets up the DLK_L99DZ200G L99DZ200G driver processing code.
         *
         *  \param spi_speed: the speed (bps) of the SPI interface to the L99DZ200G device
         *  \param cs_pin: the chip select Arduino pin (~CS) of the SPI interface to the L99DZ200G device
         *
         *  \return None.
         */
        DLK_L99DZ200G(uint32_t spi_speed, uint8_t cs_pin);

        /**
         * Initialize L99DZ200G.
         *
         * \return   L99DZ200G_FAIL = the L99DZ200G initialization failed
         * \return   L99DZ200G_OK = the L99DZ200G initialization was successful
         */
        uint8_t L99DZ200G_Init(void);

        /**
         * Read from specified L99DZ200G ROM address.
         *
         * \param addr: the L99DZ200G ROM address (0x00 to 0x3f) to read from
         *
         * \return   uint8_t = L99DZ200G ROM adddress value
         */
        uint8_t L99DZ200G_ReadRomAddress(uint8_t addr);

        /**
         * Read from specified L99DZ200G register.
         *
         * \param reg: the L99DZ200G register (0x01 to 0x16, 0x31 to 0x3c, 0x3f) to read from
         *
         * \return   uint32_t = L99DZ200G register value
         */
        uint32_t L99DZ200G_ReadRegister(uint8_t reg);

        /**
         * Write specified value to specified L99DZ200G Control register.
         *
         * \param reg: the L99DZ200G Control register (0x01 to 0x16, 0x3f) {CR1 to CR22, Config} to write to
         * \param val: the value to write to the L99DZ200G Control register
         *
         *  \return None.
         */
        void L99DZ200G_WriteControlRegister(uint8_t reg, uint32_t val);

        /**
         * Modify specified L99DZ200G Control register with specified mask and specified data.
         *
         * \param reg: the L99DZ200G Control register (0x01 to 0x16, 0x3f) to modify
         * \param mask: the bit mask of bits in the L99DZ200G Control register to modify
         * \param data: the value to modify in the L99DZ200G Control register
         *
         *  \return None.
         */
        void L99DZ200G_ModifyControlRegister(uint8_t reg, uint32_t mask, uint32_t data);

        /**
         * Read and clear specified bits in specified L99DZ200G Status register.
         *
         * \param reg: the L99DZ200G Status register to read and clear
         * \param mask: the bit mask of bits in the L99DZ200G Status register to clear
         *
         *  \return None.
         */
        void L99DZ200G_ReadClearRegister(uint8_t reg, uint32_t mask);

        /**
         * Delays for the specified number of milliseconds, maintaining the device in ACTIVE state.
         *
         * @param msec: the number of milliseconds to delay
         */
        void L99DZ200G_Delay(uint16_t msec);

        /**
         *  Check if watchdog is expired.
         *
         *  \return bool Watchdog retrigger status (true = retriggered, false not retriggered)
         */
        bool L99DZ200G_CheckWdogExpired(void);

        /**
         * Trigger L99DZ200G watchdog - CR1.
         *
         *  \return None.
         */
        void L99DZ200G_WdogTrigger(void);

        /**
         * Set watchdog trigger time - CR2 (CFR).
         *
         * \param ttime: the watchdog trigger time:
         *               (WDOG_TIME_TSW1 or WDOG_TIME_TSW2 or WDOG_TIME_TSW3 or WDOG_TIME_TSW4)
         *      -or-     (WDOG_TIME_10MS or WDOG_TIME_50MS or WDOG_TIME_100MS or WDOG_TIME_200MS)
         *
         *  \return None.
         */
        void L99DZ200G_SetWdogTime(uint8_t ttime);

        /**
         * Get Watchdog Trigger Time - CR2.
         *
         * @return uint8_t  Watchdog trigger time (WDOG_TIME_10MS, WDOG_TIME_50MS, WDOG_TIME_100MS, WDOG_TIME_200MS)
         */
        uint8_t L99DZ200G_GetWdogTime(void);

        /**
         * Clear all L99DZ200G Status Registers.
         *
         *  \return None.
         */
        void L99DZ200G_ClearAllStatusRegisters(void);

        /**
         * Reset all L99DZ200G Control Registers to defaults.
         *
         *  \return None.
         *
         *  \note The Watchdog Trigger Time is reset to the default in CR2, so this may
         *        result in Watchdog failures!
         */
        void L99DZ200G_ResetAllControlRegisters(void);

        /**
         * Set L99DZ200G V2 Voltage Regulator Configuration - CR1, CFR.
         *
         * \param mode_config: the V2 voltage regulator mode configuration:
         *                     (OFF_V2, ON_ACTIVEMODE, ON_ACTIVEMODE_V1STANDBY, ALWAYS_ON)
         * \param type_config: the V2 voltage regulator type configuration:
         *                     (V2_VREG_TYPE, V2_TRACK_V1_TYPE)
         *
         *  \return None.
         */
        void L99DZ200G_V2_Config(uint32_t mode_config, uint32_t type_config);

        /**
         * High-Side/Low-Side (HS/LS) motor controller for OUT1, OUT2, OUT3, OUT6 - CR4. \n
         * All the motors (up to three) may work together or 1 at time, but a physical limit
         * of the system is that if they run together, they have to turn in the same direction,
         * because the OUT1 is common to every current path.
         *
         * @param outputs: the motor outputs to control (OUT2_X, OUT3_Y, OUT6_F, OUT_XY, OUT_XF, OUT_YF)
         *                  {OUT1 is also implicitly controlled}
         * @param motor_dir: the motor direction (LEFT_DIRECTION, RIGHT_DIRECTION, BRAKE)
         *
         *  \note The following is the definitions of motor direction:
         *        - LEFT_DIRECTION = OUT1 Low-Side ON; OUT2, OUT3, and/or OUT6 High-Sides ON
         *        - RIGHT_DIRECTION = OUT1 High-Side ON; OUT2, OUT3, and/or OUT6 Low-Sides ON
         *
         *  \note The following is the connections of the motors:
         *        - Motor #1: OUT2 <-> OUT1 (called 'X' in the mirror controller application)
         *        - Motor #2: OUT3 <-> OUT1 (called 'Y' in the mirror controller application)
         *        - Motor #3: OUT6 <-> OUT1 (called 'F' (Fold) in the mirror controller application)
         */
        void L99DZ200G_MotorDriver(uint8_t outputs, uint8_t motor_dir);

        /**
         * Set L99DZ200G High-Side/Low-Side Outputs Control - CR4.
         *
         * \param output_type: the output control type: (OFF_OUT, LO_OUT, HI_OUT)
         * \param output: the output to control:
         *                (OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                 OUT_6_HS, OUT_6_LS)
         *
         *  \return None.
         */
        void L99DZ200G_HS_LS_OutputsControl(uint8_t output_type, uint8_t output);

        /**
         * Set L99DZ200G High-Side Outputs Control - CR5, CR6.
         *
         * \param output_type: the output control type:
         *                      (OFF_OUT, ON_OUT, TIMER1_OUT, TIMER2_OUT,
         *                       PWM1_OUT, PWM2_OUT, PWM3_OUT, PWM4_OUT,
         *                       PWM5_OUT, PWM6_OUT, PWM7_OUT, DIR_OUT)
         * \param output: the output to control:
         *                (OUT_7, OUT_8, OUT_9, OUT_10, OUT_13, OUT_14, OUT_15)
         *
         *  \return None.
         */
        void L99DZ200G_HSOutputsControl(uint8_t output_type, uint8_t output);

        /**
         * Set L99DZ200G Heater Output Control - CR5.
         *
         * \param output_type: the output control type: (OFF_OUT, ON_OUT)
         *
         *  \return None.
         */
        void L99DZ200G_HeaterOutputControl(uint8_t output_type);

        /**
         * Set L99DZ200G PWM Channel Frequency - CR12.
         *
         * \param pwm_chan: the PWM channel:
         *              (PWM_CHAN1, PWM_CHAN2, PWM_CHAN3, PWM_CHAN4, PWM_CHAN5, PWM_CHAN6, PWM_CHAN7)
         * \param pwm_freq: the PWM frequency:
         *              (PWM_FREQ_100HZ, PWM_FREQ_200HZ, PWM_FREQ_330HZ, PWM_FREQ_500HZ)
         *
         *  \return None.
         */
        void L99DZ200G_SetPWMFrequency(uint8_t pwm_chan, uint8_t pwm_freq);

        /**
         * Set L99DZ200G PWM Channel Duty Cycle - CR13 to CR16.
         *
         * \param pwm_chan: the PWM channel:
         *              (PWM_CHAN1, PWM_CHAN2, PWM_CHAN3, PWM_CHAN4, PWM_CHAN5, PWM_CHAN6, PWM_CHAN7)
         * \param pwm_duty: the PWM duty cycle: (0 to 100%)
         *
         *  \return None.
         */
        void L99DZ200G_SetPWMDutyCycle(uint8_t pwm_chan, uint8_t pwm_duty);

        /**
         * Set Programmable timer Period and OnTime - CR2.
         *
         * @param timer:   which Timer: (TYPE_TIMER1, TYPE_TIMER2)
         * @param period:  Timer period: (T_10MS, T_20MS, T_50MS, T_100MS, T_200MS, T_500MS, T_1S, T_2S)
         * @param restart: enable/disable Timer restart: (ENABLE, DISABLE)
         * @param dir:     enable/disable Timer DIR control: (ENABLE, DISABLE)
         * @param ontime:  Timer on time: (TON_100US, TON_300US, TON_1MS, TON_10MS, TON_20MS)
         *
         *  \return None.
         */
        void L99DZ200G_SetTimerConfig(uint8_t timer, uint8_t period, uint8_t restart, uint8_t dir, uint8_t ontime);

        /**
         * Select L99DZ200G CM_DIR pin configuration - CR7.
         *
         * @param cm_dir: CM/DIR configuration: (CM_ALWAYS, DIR_STBY_CM_ACTIVE, DIR_ALWAYS, OFF_CM_DIR)
         *
         *  \return None.
         */
        void L99DZ200G_CM_DIR_Config(uint8_t cm_dir);

        /**
         *  Select L99DZ200G CM_DIR Current Monitor OUTn Selection - CR7.
         *
         * @param out_cm: CM OUTn selection: (OUT_1_CM, OUT_2_CM, OUT_3_CM, OUT_6_CM, OUT_7_CM, OUT_8_CM,
         *                                    OUT_9_CM, OUT_10_CM, OUT_13_CM, OUT_14_CM, OUT_15_CM)
         *
         *  \return None.
         */
        void L99DZ200G_CM_OUTn_Select(uint8_t out_cm);

        /**
         * Set maximum voltage of ECV Electrochromic Controller - CFR.
         *
         * @param ecv_type: ECM maximum voltage: (ECV_1_2, ECV_1_5)
         *
         *  \return None.
         */
        void L99DZ200G_Set_ECV_MaxVoltage(uint8_t ecv_type);

        /**
         *  Set the drive voltage of ECV Electrochromic Controller for the ECV - CR11.
         *  Higher voltage means higher dulling.
         *
         * @param target_volts: the target voltage to be set (0 to ECV Max Voltage)
         *
         *  \return None.
         *
         *  \note The ECV Max Voltage (\ref L99DZ200G_Set_ECV_MaxVoltage) should be set before calling this method.
         */
        void L99DZ200G_Set_ECV_DriveVoltage(float target_volts);

        /**
         *  Switch ON/OFF the ECV fast discharge - CR11.
         *
         * @param en_dis: enable/disable ECV fast discharge: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_ECV_FastDischargeControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF the ECV Overcurrent Recovery - CR11.
         *
         * @param en_dis: enable/disable ECV Overcurrent Recovery: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_ECV_OvercurrentRecoveryControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF the ECV functionality - CR11.
         *
         * @param en_dis: enable/disable ECV functionality: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_ECV_Control(uint8_t en_dis);

        /**
         *  Get the drive voltage of ECV Electrochromic Controller for the ECV - CR11.
         *
         *  \return float drive voltage.
         */
        float L99DZ200G_Get_ECV_DriveVoltage(void);

        /**
         * Set Overcurrent Autorecovery programmable ON time - CR8.
         *
         * @param output: the output to control: (OUT_1_2_3_6, OUT_7, OUT_8, OUT_15)
         * @param rec_time: OCR autorecovery on time: (OCR_TON_88US, OCR_TON_80US, OCR_TON_72US, OCR_TON_64US)
         *
         *  \return None.
         */
        void L99DZ200G_Set_OCR_AutorecoveryTime(uint8_t output, uint8_t rec_time);

        /**
        * Set Overcurrent Autorecovery programmable frequency - CR8.
         *
         * @param output: the output to control: (OUT_1_2_3_6, OUT_7, OUT_8, OUT_15)
         * @param rec_freq: OCR autorecovery frequency:
         *                      (OCR_FREQ_1_7KHZ, OCR_FREQ_2_2KHZ, OCR_FREQ_3_0KHZ, OCR_FREQ_4_4KHZ)
         *
         *  \return None.
         */
        void L99DZ200G_Set_OCR_AutorecoveryFrequency(uint8_t output, uint8_t rec_freq);

        /**
         * Switch ON/OFF Overcurrent Autorecovery - CR7.
         *
         * @param output: the output to control: (OUT_1, OUT_2, OUT_3, OUT_6, OUT_7, OUT_8, OUT_15)
         * @param en_dis: enable/disable Overcurrent Autorecovery: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_OCR_AutorecoveryControl(uint8_t output, uint8_t en_dis);

        /**
         * Switch ON/OFF OCR Thermal Expiration - CR8.
         *
         * @param output: the output to control: (OUT_1, OUT_2, OUT_3, OUT_6, OUT_7, OUT_8, OUT_15)
         * @param en_dis: enable/disable OCR Thermal Expiration: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_OCR_ThermalExpirationControl(uint8_t output, uint8_t en_dis);

        /**
         * Switch ON/OFF Constant Current Mode - CR9.
         *
         * @param output: the output to control: (OUT_7, OUT_8, OUT_9)
         * @param en_dis: enable/disable Constant Current Mode: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note The 'output' OUTn is automatically turned off and its OCR autorecovery is turned off.
         */
        void L99DZ200G_SetConstantCurrentModeControl(uint8_t output, uint8_t en_dis);

        /**
         * Set OUTn low/high Rdson setting - CR9.
         *
         *  @param output: the output to control: (OUT_1_6, OUT_7, OUT_8)
         *  @param rds_on: the Rdson setting: (RDS_ON_LO, RDS_ON_HI)
         *
         *  \return None.
         */
        void L99DZ200G_Set_OUTn_Rdson(uint8_t output, uint8_t rds_on);

        /**
         * Switch ON/OFF short circuit threshold control - CR7.
         *
         * @param output: the output to control: (OUT_1, OUT_2, OUT_3, OUT_6)
         * @param en_dis: enable/disable short circuit threshold: (SC_THRSH_ON, SC_THRSH_OFF)
         *
         *  \return None.
         */
        void L99DZ200G_SetShortCircuitControl(uint8_t output, uint8_t en_dis);

        /**
         * Set L99DZ200G low/high current Open-load Threshold Control - CR9.
         *
         *  @param output: the output to control: (OUT_9, OUT_10, OUT_13, OUT_14, OUT_15)
         *  @param hi_lo: the low/high current setting: (LO_CURRENT_MODE, HI_CURRENT_MODE)
         *
         *  \return None.
         */
        void L99DZ200G_OpenLoadThresholdControl(uint8_t output, uint8_t hi_lo);

        /**
         * Set L99DZ200G low/high current Overcurrent Threshold Control - CR9.
         *
         *  @param output: the output to control: (OUT_9, OUT_10, OUT_13, OUT_14, OUT_15)
         *  @param hi_lo: the low/high current setting: (LO_CURRENT_MODE, HI_CURRENT_MODE)
         *
         *  \return None.
         */
        void L99DZ200G_OvercurrentThresholdControl(uint8_t output, uint8_t hi_lo);

        /**
         * Set ON/OFF L99DZ200G Heater Open-load Diagnosis Control - CR11.
         *
         * \param on_off: the Heater Diagnosis control type: (OL_DIAG_OFF, OL_DIAG_ON)
         *
         *  \return None.
         */
        void L99DZ200G_HeaterOpenLoadDiagnosisControl(uint8_t on_off);

        /**
         * Set L99DZ200G Drain source monitoring threshold voltage for external heater MOSFET - CR11.
         *
         * \param mon_threshld: the monitoring threshold voltage:
         *                      (GH_TH_200_MV, GH_TH_250_MV, GH_TH_300_MV, GH_TH_350_MV,
         *                       GH_TH_400_MV, GH_TH_450_MV, GH_TH_500_MV, GH_TH_550_MV)
         *
         *  \return None.
         */
        void L99DZ200G_SetHeaterMonitorThresholdVoltage(uint8_t mon_threshld);

        /**
         * Set L99DZ200G Vs Compensation VLED Voltage - CR17 to CR20.
         *
         * \param output: the output to control:
         *                (OUT_7, OUT_8, OUT_9, OUT_10, OUT_13, OUT_14, OUT_15)
         * \param vled: the VLED voltage value: (0 to 10 V)
         *
         *  \return None.
         */
        void L99DZ200G_SetVsCompensationVLED(uint8_t output, float vled);

        /**
         * Switch ON/OFF Automatic Vs Compensation control - CR17 to CR20.
         *
         * \param output: the output to control:
         *                (OUT_7, OUT_8, OUT_9, OUT_10, OUT_13, OUT_14, OUT_15)
         * @param en_dis: enable/disable Automatic Vs Compensation: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_SetAutoVsCompensationControl(uint8_t output, uint8_t en_dis);

        /**
         *  Get the temperature of specified Thermal Cluster - SR7 to SR9.
         *
         * \param therm_cluster: the Thermal Cluster to read temperature:
         *                       (TEMP_CL1, TEMP_CL2, TEMP_CL3, TEMP_CL4, TEMP_CL5, TEMP_CL6)
         *
         *  \return cluster temperature - Celsius
         */
        float L99DZ200G_GetThermalClusterTemp(uint8_t therm_cluster);

        /**
         *  Get the specified pin voltage value - SR10, SR11.
         *
         * \param volt_item: the voltage item to read: (VS_REG_V, VS_V, VWU_V) \n
         *                   {VSREG pin supply voltage, VS pins supply voltage, WU pin voltage}
         *
         *  \return voltage - Volts
         */
        float L99DZ200G_GetPinVoltage(uint8_t volt_item);

        /**
         * Set L99DZ200G device for specified standby mode (VBAT Standby or V1 Standby) - CR1.
         *
         * @param lp_mode: the low power mode to go to (V1_STBY, VBAT_STBY, ACTIVE_STBY)
         *
         *  \return None.
         */
        void L99DZ200G_SetModeControl(uint8_t lp_mode);

        /**
         * Switch ON/OFF charge pump  and charge pump low and charge pump clock dithering control - CR22 (CFR).
         *
         * @param en_dis: enable/disable charge pump: (CP_ENABLE, CP_DISABLE)
         * @param ltch_live: charge pump low latch/live handling: (CP_LOW_CFG_LATCHED, CP_LOW_CFG_LIVE)
         * @param don_doff: enable/disable charge pump clock dithering: (IC_DITH_ENABLED, IC_DITH_DISABLED)
         *
         *  \return None.
         */
        void L99DZ200G_SetChargePumpControl(uint8_t en_dis, uint8_t ltch_live, uint8_t don_doff);

        /**
         * Switch ON/OFF V1 load current supervision (ICMP) control - CR22 (CFR).
         *
         * @param en_dis: enable/disable V1 load current supervision: (IC_ENABLE, IC_DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_ICMP_Control(uint8_t en_dis);

        /**
         * Set voltage regulator V1 reset threshold (VRTH), monitored in Active
         * or V1_Standby mode - CR2.
         *
         * @param v1_thrshld: the V1 reset threshold (V1_RESET_3_5V, V1_RESET_3_8V, V1_RESET_4_0V, V1_RESET_4_3V)
         *
         *  \return None.
         */
        void L99DZ200G_Set_V1_ResetThreshold(uint8_t v1_thrshld);

        /**
         * Set L99DZ200G status masked in GSB - CFR.
         *
         * @param mask: the mask to apply
         *              (HS_OL_ITEM, LS_OL_ITEM, TW_ITEM, EC_OL_ITEM, OL_ITEM, SPIE_ITEM, PLE_ITEM, GW_ITEM)
         * @param on_off: mask/unmask item (DO_MASK, DO_UNMASK)
         *
         *  \return None.
         */
        void L99DZ200G_Set_GSB_MaskUnmask(uint8_t mask, uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VS supply voltage overvoltage/undervoltage lockout control - CR3.
         *
         * \param on_off: the VS overvoltage/undervoltage lockout control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VS_LockoutControl(uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VS supply voltage overvoltage shutdown control - CR3.
         *
         * \param on_off: the VS overvoltage shutdown control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VS_OvervoltageShutdownControl(uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VS supply voltage undervoltage shutdown control - CR3.
         *
         * \param on_off: the VS undervoltage shutdown control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VS_UndervoltageShutdownControl(uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VSREG supply voltage overvoltage/undervoltage lockout control - CR3.
         *
         * \param on_off: the VSREG overvoltage/undervoltage lockout control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VSREG_LockoutControl(uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VSREG supply voltage overvoltage shutdown control - CR3.
         *
         * \param on_off: the VSREG overvoltage shutdown control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VSREG_OvervoltageShutdownControl(uint8_t on_off);

        /**
         * Set ON/OFF L99DZ200G VSREG supply voltage undervoltage shutdown control - CR3.
         *
         * \param on_off: the VSREG undervoltage shutdown control (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VSREG_UndervoltageShutdownControl(uint8_t on_off);

        /**
         * Set L99DZ200G VSREG supply voltage early warning threshold voltage - CR3.
         *
         * \param thshld_volts: the VSREG early warning threshold voltage (0.0 to 10 V)
         *
         *  \return None.
         */
        void L99DZ200G_Set_VSREG_EarlyEarningThreshold(float thshld_volts);

        /**
         *  Switch ON/OFF L99DZ200G Generator mode functionality - CR22.
         *
         * @param en_dis: enable/disable Generator mode functionality: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_SetGeneratorModeControl(uint8_t en_dis);

        /**
         *  Stay or Exit L99DZ200G SW-Debug mode - CR22.
         *
         * @param stay_exit: stay in or exit SW-Debug mode: (DEBUG_STAY, DEBUG_EXIT)
         *
         *  \return None.
         *
         *  \note This has no effect if the L99DZ200G is not in \b SW-Debug mode.
         */
        void L99DZ200G_StayExitSW_DebugModeControl(uint8_t stay_exit);

        /**
         *  Switch ON/OFF L99DZ200G Watchdog functionality - CR22.
         *
         * @param en_dis: enable/disable watchdog: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note This has no effect if the L99DZ200G is not in \b CAN-Flash mode.
         */
        void L99DZ200G_WdogEnableControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G Thermal Shutdown functionality - CFR.
         *
         *  @param en_dis: enable/disable selective thermal shutdown: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_ThermalShutdownControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G wake-up pin control - CR1.
         *
         *  @param en_dis: enable/disable wake-up pin control: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note This has no effect if the L99DZ200G WU pin is not configured as wake-up input (Config Reg).
         */
        void L99DZ200G_Set_WU_PinControl(uint8_t en_dis);

        /**
         *  Set L99DZ200G wake-up pin pull-up/down functionality - CR1.
         *
         *  @param config: wake-up pin pull-up/down: (WU_PULL_UP, WU_PULL_DOWN)
         *
         *  \return None.
         *
         *  \note This has no effect if the L99DZ200G is not in \b Standby modes.
         */
        void L99DZ200G_Set_WU_PinPullConfig(uint8_t config);

        /**
         *  Set L99DZ200G wake-up pin mode functionality - CFR.
         *
         *  @param config: set wake-up pin mode: (WU_WU, WU_VBAT) {wake-up input or VBAT monitor}
         *
         *  \return None.
         */
        void L99DZ200G_Set_WU_PinModeConfig(uint8_t config);

        /**
         *  Set L99DZ200G wake-up pin filter functionality - CR1.
         *
         *  @param filter: set wake-up pin filter: (WU_FILTER_STATIC, WU_FILTER_T1, WU_FILTER_T2)
         *
         *  \return None.
         *
         *  \note This has no effect if the L99DZ200G WU pin is not configured as wake-up input (Config Reg).
         */
        void L99DZ200G_Set_WU_PinFilterConfig(uint8_t filter);

        /**
         *  Switch ON/OFF L99DZ200G Timer for NINT/Wake control - CR1.
         *
         *  @param en_dis: enable/disable Timer for NINT/Wake control: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note This only applies to L99DZ200G standby modes operation.
         */
        void L99DZ200G_SetTimer_NINT_EnableControl(uint8_t en_dis);

        /**
         *  Set L99DZ200G Timer for NINT/Wake control - CR1.
         *
         *  @param tmr_sel: select Timer for NINT/Wake control: (TMR_NINT_SEL_T1, TMR_NINT_SEL_T2)
         *
         *  \return None.
         *
         *  \note This only applies to L99DZ200G standby modes operation.
         */
        void L99DZ200G_SetTimer_NINT_SelectControl(uint8_t tmr_sel);

        /**
         *  Switch ON/OFF L99DZ200G LIN wake-up control - CR1.
         *
         *  @param en_dis: enable/disable LIN wake-up control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_LIN_WakeupControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G LIN receive-only control - CR2.
         *
         *  @param en_dis: enable/disable LIN receive-only control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_LIN_ReceiveOnlyControl(uint8_t en_dis);

         /**
          *  Switch ON/OFF L99DZ200G LIN transmit timeout control - CR2.
          *
          *  @param en_dis: enable/disable LIN transmit timeout control: (ENABLE, DISABLE)
          *
          *  \return None.
          */
       void L99DZ200G_Set_LIN_TransmitTimoutControl(uint8_t en_dis);

        /**
         *  Set L99DZ200G LIN bit rate control - CFR.
         *
         *  @param hi_lo: set LIN bit rate control: (SPD_LO_20KBS, SPD_LO_115KBS)
         *
         *  \return None.
         */
        void L99DZ200G_Set_LIN_BitRateControl(uint8_t hi_lo);

        /**
         *  Set L99DZ200G LIN wake-up behavior configuration - CFR.
         *
         * @param rdr_rdt: set wake-up behavior configuration: (REC_DOM_REC_WU, REC_DOM_TRANSITION_WU)
         *
         *  \return None.
         */
        void L99DZ200G_Set_LIN_WakeupBehaviorControl(uint8_t rdr_rdt);

        /**
         *  Switch ON/OFF L99DZ200G CAN wake-up control - CR1.
         *
         *  @param en_dis: enable/disable CAN wake-up control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_CAN_WakeupControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN Rx control - CR1.
         *
         *  @param en_dis: enable/disable CAN Rx control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_CAN_ReceiveControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN Tx control - CR1.
         *
         *  @param en_dis: enable/disable CAN Tx control: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note This can only be enabled if L99DZ200G CAN Rx is enabled
         *        (see \ref L99DZ200G_Set_CAN_ReceiveControl() ).
         */
        void L99DZ200G_Set_CAN_TransmitControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN timeout IRQ control - CR1.
         *
         *  @param en_dis: enable/disable CAN timeout IRQ control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_CAN_TimeoutIrqControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN go TRX READY mode control - CR1.
         *
         *  @param en_dis: enable/disable CAN go TRX READY mode control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_CAN_GoTxReadyControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN Tx->Rx loopback control - CR2.
         *
         *  @param en_dis: enable/disable CAN Tx->Rx loopback control: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_CAN_LoopbackControl(uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G CAN pretended networking control - CR2.
         *
         *  @param en_dis: enable/disable CAN pretended networking control: (ENABLE, DISABLE)
         *
         *  \return None.
         *
         *  \note This can only be enabled if L99DZ200G CAN Rx is enabled
         *        (see \ref L99DZ200G_Set_CAN_ReceiveControl() ).
         */
        void L99DZ200G_Set_CAN_PretendedNetworkingControl(uint8_t en_dis);

// H-Bridge Stuff

        /**
         * Switch ON/OFF specified L99DZ200G H-Bridge control - CR1.
         *
         * @param H_bridge: the H-bridge to enable/disable: (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param en_dis: enable/disable the specified H-bridge: (DISABLE, ENABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_Control(uint8_t H_bridge, uint8_t en_dis);

        /**
         * Set single motor mode Direction of specified L99DZ200G H-bridge - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to enable/disable: (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param mdir: direction of specified H-bridge: (LEFT_DIRECTION, RIGHT_DIRECTION)
         *
         *  \note The following is the definitions of motor direction:
         *        - LEFT_DIRECTION = High-Side 2, Low-Side 1 ON; High-Side 1, Low-Side 2 OFF (Direct)
         *        - RIGHT_DIRECTION = High-Side 1, Low-Side 2 ON; High-Side 2, Low-Side 1 OFF (Reverse)
         */
        void L99DZ200G_Set_HB_SingleMotorDirectionControl(uint8_t H_bridge, uint8_t mdir);

        /**
         * Set motor mode of specified L99DZ200G H-bridge - CFR.
         *
         * @param H_bridge: the H-bridge to set motor mode: (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param mode: the H-bridge motor mode: {SINGLE_MOTOR, DUAL_MOTOR}
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_MotorModeControl(uint8_t H_bridge, uint8_t mode);

        /**
         * Set cross current protection time for specified L99DZ200G H-bridge - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to set cross current protection time:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param ccpt_time: the cross current protection time:
         *                      (T_250ns, T_500ns, T_750ns, T_1000ns,
         *                       T_1250ns, T_1500ns, T_1750ns, T_2000ns,
         *                       T_2250ns, T_2500ns, T_2750ns, T_3000ns,
         *                       T_3250ns, T_3500ns, T_3750ns, T_4000ns)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_CrossCurrentProtectionTime(uint8_t H_bridge, uint8_t ccpt_time);

        /**
         * Set slew rate current percentage for specified L99DZ200G H-bridge - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to set slew rate current percentage:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param percent: the slew rate current percentage of maximum: (0% to 100%)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_SlewRateCurrent(uint8_t H_bridge, uint8_t percent);

        /**
         * Set OL high threshold for specified L99DZ200G H-bridge - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to set OL high threshold:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param en_dis: enable/disable the OL high threshold:
         *                  (DISABLE, ENABLE) {1/6 VS, 5/6 VS}
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_OL_HighThreshold(uint8_t H_bridge, uint8_t en_dis);

        /**
         *  Switch ON/OFF L99DZ200G forced LSx_FSO outputs control - CFR.
         *
         *  @param en_dis: enable/disable forced LSx_FSO outputs: (ENABLE, DISABLE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_Forced_LSx_FSO_Control(uint8_t en_dis);

        /**
         * Test open-load condition between H1 and L2 or H2 and L1 of the H-bridge A or H-bridge B - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to test open-load condition:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param en_dis: enable/disable test open-load condition: (DISABLE, ENABLE)
         * @param ol_hxly: select test open-load condition:
         *                  (OL_H1L2, OL_H2L1)
         *                  {between H1 and L2 of the H-Bridge, between H2 and L1 of the H-Bridge}
         *
         *  \return None.
         */
        void L99DZ200G_Test_HB_OL_HxandLy(uint8_t H_bridge, uint8_t en_dis, uint8_t ol_hxly);

        /**
         * Set Drain-Source monitoring threshold of specified L99DZ200G H-bridge - CR10, CR21.
         *
         * @param H_bridge: the H-bridge to set Drain-Source monitoring threshold:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param ds_threshold: select Drain-Source monitoring threshold:
         *                  (DS_500mV, DS_750mV, DS_1000mV, DS_1250mV, DS_1500mV, DS_1750mV, DS_2000mV)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_DSMonitorThreshold(uint8_t H_bridge, uint8_t ds_threshold);

        /**
         * Set single motor mode freewheeling type for specified L99DZ200G H-bridge - CR10.
         *
         * @param H_bridge: the H-bridge to set single motor mode freewheeling type:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param act_pass: set active or passive single motor mode freewheeling type:
         *                      (ACTIVE_FREEWHEELING, PASSIVE_FREEWHEELING)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_SingleMotorFreewheelingType(uint8_t H_bridge, uint8_t act_pass);

        /**
         * Set single motor mode freewheeling side for specified L99DZ200G H-bridge - CR10.
         *
         * @param H_bridge: the H-bridge to set single motor mode freewheeling side:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param low_high: set Low-Side or High-Side single motor mode freewheeling side:
         *                      (LOW_SIDE_FREEWHEELING, HIGH_SIDE_FREEWHEELING)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_SingleMotorFreewheelingSide(uint8_t H_bridge, uint8_t low_high);

        /**
         * Set dual motor mode drive side for specified L99DZ200G H-bridge and leg - CR10.
         *
         * @param H_bridge: the H-bridge to set dual motor mode drive side:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param leg: set leg 1 or leg 2 dual motor mode drive HB leg: (HB_LEG1, HB_LEG2)
         * @param low_high: set Low-Side or High-Side dual motor mode drive:
         *                      (LOW_SIDE_DRIVE, HIGH_SIDE_DRIVE)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_DualMotorDrive(uint8_t H_bridge, uint8_t leg, uint8_t low_high);

        /**
         * Set dual motor mode freewheeling type for specified L99DZ200G H-bridge and leg - CR10.
         *
         * @param H_bridge: the H-bridge to set dual motor mode freewheeling type:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param leg: set leg 1 or leg 2 dual motor mode freewheeling type HB leg: (HB_LEG1, HB_LEG2)
         * @param act_pass: set active or passive dual motor mode freewheeling type:
         *                      (ACTIVE_FREEWHEELING, PASSIVE_FREEWHEELING)
         *
         *  \return None.
         */
        void L99DZ200G_Set_HB_DualMotorFreewheelingType(uint8_t H_bridge, uint8_t leg, uint8_t act_pass);

        /**
         * Turn the specified L99DZ200G H-bridge motor counterclockwise.
         *
         * @param H_bridge: the H-bridge motor to run counterclockwise:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         *
         *  \return None.
         *
         *  \note Assumes the specified H-Bridge is configured as single motor mode.
         */
        void L99DZ200G_Set_HB_SingleMotorCounterClockwise(uint8_t H_bridge);

        /**
         * Turn the specified L99DZ200G H-bridge motor clockwise.
         *
         * @param H_bridge: the H-bridge motor to run clockwise:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         *
         *  \return None.
         *
         *  \note Assumes the specified H-Bridge is configured as single motor mode.
         */
        void L99DZ200G_Set_HB_SingleMotorClockwise(uint8_t H_bridge);

        /**
         * Stop the specified L99DZ200G H-bridge motor.
         *
         * @param H_bridge: the H-bridge motor to stop:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         *
         *  \return None.
         *
         *  \note Assumes the specified H-Bridge is configured as single motor mode.
         */
        void L99DZ200G_Set_HB_SingleMotorStop(uint8_t H_bridge);

// Status Stuff

        /**
         *  Get the specified SPI item status - SR2.
         *
         * \param spi_item: the SPI status item to get: (SPI_CLK_CNT, SPI_INV_CMD) \n
         *
         * \return   L99DZ200G_OK = the SPI item status was OK
         * \return   L99DZ200G_FAIL = the SPI item status was not OK
         */
        uint8_t L99DZ200G_Get_SPI_Status(uint8_t spi_item);

        /**
         *  Clear the SPI status - SR2.
         *
         *  \return None.
         */
        void L99DZ200G_Clear_SPI_Status(void);

        /**
         *  Get the specified voltage item status - SR1, SR2.
         *
         * \param volt_item: the voltage status item to get:
         *                   (V1_UV, VS_UV, VS_OV, VSREG_UV, VSREG_OV, VSREG_EW,
         *                    V1_FAIL, V2_FAIL, V2_SC, CP_LOW)
         *
         * \return   L99DZ200G_OK = the voltage item status was OK
         * \return   L99DZ200G_FAIL = the voltage item status was not OK
         */
        uint8_t L99DZ200G_GetVoltageStatus(uint8_t volt_item);

        /**
         *  Clear the specified voltage item status - SR1, SR2.
         *
         * \param volt_item: the voltage status item to clear:
         *                   (ALL_VOLT_ITEMS, V1_UV, VS_UV, VS_OV, VSREG_UV, VSREG_OV, VSREG_EW,
         *                    V1_FAIL, V2_FAIL, V2_SC, CP_LOW)
         *
         *  \return None.
         */
        void L99DZ200G_ClearVoltageStatus(uint8_t volt_item);

        /**
         *  Get the specified Electrochromic voltage item status - SR6.
         *
         * \param ecv_item: the Electrochromic voltage status item to get:
         *                  (ECV_VNR, ECV_VHI)
         *
         * \return   L99DZ200G_OK = the Electrochromic voltage item status was OK
         * \return   L99DZ200G_FAIL = the Electrochromicvoltage item status was not OK
         */
        uint8_t L99DZ200G_GetElectrochromicVoltageStatus(uint8_t ecv_item);

        /**
         *  Get the specified short circuit alert item status - SR4, SR5.
         *
         * \param sca_item: the short circuit alert status item to get:
         *                  (OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                   OUT_6_HS, OUT_6_LS, DS_MON_HEAT)
         *
         * \return   L99DZ200G_OK = the short circuit alert item status was OK
         * \return   L99DZ200G_FAIL = the short circuit alert item status was not OK
         */
        uint8_t L99DZ200G_GetShortCircuitAlertStatus(uint8_t sca_item);

        /**
         *  Clear the specified short circuit alert item status - SR4, SR5.
         *
         * \param sca_item: the short circuit alert status item to clear:
         *                  (OUT_ALL_SC_ITEMS, OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                   OUT_6_HS, OUT_6_LS, DSMON_HEAT)
         *
         *  \return None.
         */
        void L99DZ200G_ClearShortCircuitAlertStatus(uint8_t sca_item);

        /**
         *  Get the specified open-load item status - SR5.
         *
         * \param opld_item: the open-load status item to get:
         *                   (OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                    OUT_6_HS, OUT_6_LS, OUT_7_OL, OUT_8_OL, OUT_9_OL, OUT_10_OL,
         *                    OUT_13_OL, OUT_14_OL, OUT_15_OL, OUT_GH, OUT_ECV)
         *
         * \return   L99DZ200G_OK = the open-load item status was OK
         * \return   L99DZ200G_FAIL = the open-load item status was not OK
         */
        uint8_t L99DZ200G_GetOpenLoadStatus(uint8_t opld_item);

        /**
         *  Clear the specified open-load item status - SR5.
         *
         * \param opld_item: the open-load status item to clear:
         *                   (OUT_ALL_OL_ITEMS, OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS,
         *                    OUT_3_LS, OUT_6_HS, OUT_6_LS, OUT_7_OL, OUT_8_OL, OUT_9_OL, OUT_10_OL,
         *                    OUT_13_OL, OUT_14_OL, OUT_15_OL, OUT_GH, OUT_ECV)
         *
         *  \return None.
         */
        void L99DZ200G_ClearOpenLoadStatus(uint8_t opld_item);

        /**
         *  Get the specified thermal shutdown item status - SR1, SR6.
         *
         * \param tshdwn_item: the thermal shutdown status item to get:
         *                     (TSD1_ITEM, TSD2_ITEM,
         *                      TSD1_CL1_ITEM, TSD1_CL2_ITEM, TSD1_CL3_ITEM, TSD1_CL4_ITEM, TSD1_CL5_ITEM, TSD1_CL6_ITEM)
         *
         * \return   L99DZ200G_OK = the thermal shutdown item status was OK
         * \return   L99DZ200G_FAIL = the thermal shutdown item status was not OK
         */
        uint8_t L99DZ200G_GetThermalShutdownStatus(uint8_t tshdwn_item);

        /**
         *  Clear the specified thermal shutdown item status - SR1, SR6.
         *
         * \param tshdwn_item: the thermal shutdown status item to clear:
         *                     (TH_ALL_ITEMS, TSD1_ITEM, TSD2_ITEM,
         *                      TSD1_CL1_ITEM, TSD1_CL2_ITEM, TSD1_CL3_ITEM, TSD1_CL4_ITEM, TSD1_CL5_ITEM, TSD1_CL6_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_ClearThermalShutdownStatus(uint8_t tshdwn_item);

        /**
         *  Get the specified thermal warning item status - SR2, SR6.
         *
         * \param twarn_item: the thermal warning status item to get:
         *                    (THERMW_ITEM,
         *                     TW_CL1_ITEM, TW_CL2_ITEM, TW_CL3_ITEM, TW_CL4_ITEM, TW_CL5_ITEM, TW_CL6_ITEM)
         *
         * \return   L99DZ200G_OK = the thermal warning item status was OK
         * \return   L99DZ200G_FAIL = the thermal warning item status was not OK
         */
        uint8_t L99DZ200G_GetThermalWarningStatus(uint8_t twarn_item);

        /**
         *  Clear the specified thermal warning item status - SR2, SR6.
         *
         * \param twarn_item: the thermal warning status item to clear:
         *                    (TH_ALL_ITEMS, THERMW_ITEM,
         *                     TW_CL1_ITEM, TW_CL2_ITEM, TW_CL3_ITEM, TW_CL4_ITEM, TW_CL5_ITEM, TW_CL6_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_ClearThermalWarningStatus(uint8_t twarn_item);

        /**
         *  Get the specified overcurrent recovery alert item status - SR4.
         *
         * \param ocra_item: the overcurrent recovery alert status item to get:
         *                   (OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                    OUT_6_HS, OUT_6_LS, OUT_7_OCR, OUT_8_OCR, OUT_15_OCR)
         *
         * \return   L99DZ200G_OK = the overcurrent recovery alert item status was OK
         * \return   L99DZ200G_FAIL = the overcurrent recovery alert item status was not OK
         */
        uint8_t L99DZ200G_GetOvercurrentRecoveryAlertStatus(uint8_t ocra_item);

        /**
         *  Get the specified overcurrent shutdown item status - SR3, SR5.
         *
         * \param ocs_item: the overcurrent shutdown status item to get:
         *                   (OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS, OUT_3_HS, OUT_3_LS,
         *                    OUT_6_HS, OUT_6_LS, OUT_7_OC, OUT_8_OC, OUT_9_OC,
         *                    OUT_10_OC, OUT_13_OC, OUT_14_OC, OUT_15_OC,
         *                    LSA_FSO_OC, LSB_FSO_OC, OUT_ECV)
         *
         * \return   L99DZ200G_OK = the overcurrent shutdown item status was OK
         * \return   L99DZ200G_FAIL = the overcurrent shutdown item status was not OK
         */
        uint8_t L99DZ200G_GetOvercurrentShutdownStatus(uint8_t ocs_item);

        /**
         *  Clear the specified overcurrent shutdown item status - SR3, SR5.
         *
         * \param ocs_item: the overcurrent shutdown status item to clear:
         *                   (OUT_ALL_OCS_ITEMS, OUT_1_HS, OUT_1_LS, OUT_2_HS, OUT_2_LS,
         *                    OUT_3_HS, OUT_3_LS, OUT_6_HS, OUT_6_LS, OUT_7_OC, OUT_8_OC,
         *                    OUT_10_OC, OUT_13_OC, OUT_14_OC, OUT_15_OC,
         *                    OUT_9_OC, LSA_FSO_OC, LSB_FSO_OC, OUT_ECV)
         *
         *  \return None.
         */
        void L99DZ200G_ClearOvercurrentShutdownStatus(uint8_t ocs_item);

        /**
         *  Get the count of voltage regulator V1 restarts due to thermal shutdown - SR1.
         *
         * \return   uint8_t: the count of voltage regulator V1 restarts
         */
        uint8_t L99DZ200G_GetV1ResetCount(void);

        /**
         *  Get the specified H-bridge MOSFET device short circuit/open-load status - SR2, SR3.
         *
         * @param H_bridge: the H-bridge to get status of:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param side_item: the H-bridge MOSFET device to get status of:
         *                      (LOW_SIDE_1_MOSFET, HIGH_SIDE_1_MOSFET,
         *                       LOW_SIDE_2_MOSFET, HIGH_SIDE_2_MOSFET)
         *
         * \return   L99DZ200G_OK = the H-bridge MOSFET device short circuit/open-load item status was OK
         * \return   L99DZ200G_FAIL = the H-bridge MOSFET device short circuit/open-load status was not OK
         */
        uint8_t L99DZ200G_Get_HB_DrainSourceMonitoringStatus(uint8_t H_bridge, uint8_t side_item);

        /**
         * Clear Drain-Source monitoring status for specified L99DZ200G H-bridge  - SR2, SR3.
         *
         * @param H_bridge: the H-bridge to clear Drain-Source monitoring status:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         *
         *  \return None.
         */
        void L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(uint8_t H_bridge);

        /**
         *  Clear the specified H-bridge MOSFET device short circuit/open-load status - SR2, SR3.
         *
         * @param H_bridge: the H-bridge to clear status of:
         *                      (H_BRIDGE_CONTROL_A, H_BRIDGE_CONTROL_B)
         * @param side_item: the H-bridge MOSFET device to clear status of:
         *                      (ALL_HB_SIDE_ITEMS, LOW_SIDE_1_MOSFET, HIGH_SIDE_1_MOSFET,
         *                       LOW_SIDE_2_MOSFET, HIGH_SIDE_2_MOSFET)
         *
         *  \return None.
         */
        void L99DZ200G_Clear_HB_DrainSourceMonitoringStatus(uint8_t H_bridge, uint8_t side_item);

        /**
         *  Get the specified LIN item status - SR2.
         *
         * \param lin_item: the LIN interface item status to get:
         *                   (LIN_PERM_REC_ITEM, LIN_TXD_DOM_ITEM, LIN_PERM_DOM_ITEM)
         *
         * \return   L99DZ200G_OK = the LIN interface item status was OK
         * \return   L99DZ200G_FAIL = the LIN interface item status was not OK
         */
        uint8_t L99DZ200G_Get_LIN_Status(uint8_t lin_item);

        /**
         *  Clear the specified LIN item status - SR2.
         *
         * \param lin_item: the LIN interface item status to clear:
         *                   (ALL_LIN_ITEMS, LIN_PERM_REC_ITEM, LIN_TXD_DOM_ITEM, LIN_PERM_DOM_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_Clear_LIN_Status(uint8_t lin_item);

        /**
         *  Get the specified CAN item status - SR2, SR12.
         *
         * \param can_item: the CAN interface item status to get:
         *                   (CAN_SUP_LOW_ITEM, CAN_TXD_DOM_ITEM, CAN_PERM_DOM_ITEM,
         *                    CAN_PERM_REC_ITEM, CAN_RXD_REC_ITEM, CANTO_ITEM, CAN_SILENT_ITEM)
         *
         * \return   L99DZ200G_OK = the CAN interface item status was OK
         * \return   L99DZ200G_FAIL = the CAN interface item status was not OK
         */
        uint8_t L99DZ200G_Get_CAN_Status(uint8_t can_item);

        /**
         *  Clear the specified CAN item status - SR2, SR12.
         *
         * \param can_item: the CAN interface item status to clear:
         *                   (ALL_CAN_ITEMS, CAN_SUP_LOW_ITEM, CAN_TXD_DOM_ITEM,
         *                    CAN_PERM_DOM_ITEM, CAN_PERM_REC_ITEM, CAN_RXD_REC_ITEM, CANTO_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_Clear_CAN_Status(uint8_t can_item);

        /**
         *  Get the count of watchdog failures - SR1.
         *
         * \return   uint8_t: the count of watchdog failures
         */
        uint8_t L99DZ200G_GetWdogFailCount(void);

        /**
         *  Get the watchdog timer status - SR6.
         *
         * \return   uint8_t: the watchdog timer status
         *                   (WD_TMR_STATE_0_33_PCT, WD_TMR_STATE_33_66_PCT, WD_TMR_STATE_66_100_PCT)
         */
        uint8_t L99DZ200G_GetWdogTimerStatus(void);

        /**
         *  Get the specified forced sleep (Forced VBAT_Standby) item status - SR1.
         *
         * \param forced_item: the forced sleep item status to get:
         *                     (FORCED_SLEEP_WD_ITEM, FORCED_SLEEP_TSD2_V1SC_ITEM)
         *
         * \return   L99DZ200G_OK = the forced sleep item status was OK
         * \return   L99DZ200G_FAIL = the forced sleep item status was not OK
         */
        uint8_t L99DZ200G_GetForcedSleepStatus(uint8_t forced_item);

        /**
         *  Clear the specified forced sleep (Forced VBAT_Standby) item status - SR1.
         *
         * \param forced_item: the forced sleep item status to clear:
         *                     (ALL_FORCED_SLEEP_ITEMS, FORCED_SLEEP_WD_ITEM, FORCED_SLEEP_TSD2_V1SC_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_ClearForcedSleepStatus(uint8_t forced_item);

        /**
         *  Get the specified miscellaneous item status - SR1.
         *
         * \param misc_item: the miscellaneous item status to get:
         *                   (SGND_LOSS_ITEM, DEBUG_MODE_ITEM, VSPOR_ITEM)
         *
         * \return   L99DZ200G_OK = the miscellaneous item status was OK
         * \return   L99DZ200G_FAIL = the miscellaneous item status was not OK
         */
        uint8_t L99DZ200G_GetMiscellaneousStatus(uint8_t misc_item);

        /**
         *  Clear the specified miscellaneous item status - SR1.
         *
         * \param misc_item: the miscellaneous item status to clear:
         *                   (ALL_MISC_ITEMS, SGND_LOSS_ITEM, VSPOR_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_ClearMiscellaneousStatus(uint8_t misc_item);

        /**
         *  Get the watchdog failure status - SR1.
         *
         * \return   L99DZ200G_OK = no watchdog failures
         * \return   L99DZ200G_FAIL = watchdog failures occurred
         */
        uint8_t L99DZ200G_GetWdogFailStatus(void);

        /**
         *  Clear the watchdog failure status - SR1.
         *
         *  \return None.
         */
        void L99DZ200G_ClearWdogFailStatus(void);

        /**
         *  Get the specified wake-up item status - SR1, SR12.
         *
         * \param wake_item: the wake-up item status to get:
         *                   (WK_TIMER_ITEM, WK_LIN_ITEM, WK_CAN_ITEM,
         *                    WK_WU_ITEM, WK_VS_OV_ITEM, WK_CAN_WUP_ITEM)
         *
         * \return   L99DZ200G_OK = the wake-up item was not the wake-up source
         * \return   L99DZ200G_FAIL = the wake-up item was the wake-up source
         */
        uint8_t L99DZ200G_GetWakeUpStatus(uint8_t wake_item);

        /**
         *  Clear the specified wake-up item status - SR1, SR12.
         *
         * \param wake_item: the wake-up item status to clear:
         *                   (ALL_WK_ITEMS, WK_TIMER_ITEM, WK_LIN_ITEM, WK_CAN_ITEM,
         *                    WK_WU_ITEM, WK_VS_OV_ITEM, WK_CAN_WUP_ITEM)
         *
         *  \return None.
         */
        void L99DZ200G_ClearWakeUpStatus(uint8_t wake_item);

        /**
         *  Get the device wake-up state - SR1.
         *
         * \return   uint8_t: the device wake-up state:
         *                   (DEV_STATE_ACTIVE, DEV_STATE_WUV1, DEV_STATE_WUVBAT, DEV_STATE_FLASH)
         */
        uint8_t L99DZ200G_GetDeviceWakeUpState(void);

        /**
         *  Clear the device wake-up state - SR1.
         *
         *  \return None.
         */
        void L99DZ200G_ClearDeviceWakeUpState(void);

        /**
         *  Get the WU wake-up pin state - SR1.
         *
         * \return   uint8_t: the WU wake-up pin state: (PIN_LOW, PIN_HIGH)
         */
        uint8_t L99DZ200G_Get_WU_PinState(void);


        /**
         * Check if specified L99DZ200G register is writable.
         *
         * \param reg: the L99DZ200G register (0x01 to 0x3F) to check
         *
         * \return   L99DZ200G_FAIL = register not writable
         * \return   L99DZ200G_OK = register is writable
         */
        uint8_t L99DZ200G_CheckRegisterWritable(uint8_t reg);

        /**
         * Retrieve Global Status Byte (GSB).
         *
         * @return uint8_t     Global Status Byte
         */
        uint8_t L99DZ200G_GlobalStatusByte(void);

        /**
         * Set specified watchdog running state.
         *
         * \param wd_state: true = watchdog is running, false watchdog is not running
         *
         *  \return None.
         */
        void L99DZ200G_SetWatchdogRunning(bool wd_state);

        /**
         * Retrieve watchdog running state.
         *
         * @return bool  true = watchdog is running, false watchdog is not running
         */
        bool L99DZ200G_WatchdogRunning(void);

    private:
        /// Pointer to SPI device
        SPIClass * SPI_dev;

        /// Flag for SPI initialization
        static bool SPI_initted;

        /// Chip Select pin number
        uint8_t CS_pin;

        /// SPI configuration settings
        SPISettings SPI_Settings;

        /// L99DZ200G operations Global Status Register
        uint8_t GlobalStatusRegister;

        /// L99DZ200G watchdog trigger interval
        uint32_t WdogTriggerTime = 10;      // mS

        /// L99DZ200G watchdog tick value
        uint32_t WdogTick;

        /// L99DZ200G watchdog running status
        bool WatchdogRunning;

        /// Initiate L99DZ200G SPI transaction
        inline void L99DZ200G_StartSPI(void);

        /// Terminate L99DZ200G SPI transaction
        inline void L99DZ200G_EndSPI(void);
};
#endif  // __DLK_L99DZ200G_H__

