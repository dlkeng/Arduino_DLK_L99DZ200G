/****************************************************************************
*
* Copyright - 2021 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    AEK_MOT_TK200G1_for_TrunkLift.c
 * @brief   SPC582Bxx AEK_MOT_TK200G1 Driver code for TrunkLift.
 */

/**
 * Enable/disable H-bridges to drive three motors with two H-bridges.
 *
 * @param  trunk_cmd: (TRUNK_OPEN, TRUNK_CLOSE, TRUNK_LOCK, TRUNK_UNLOCK, TRUNK_STOP)
 */
void TrunkEnableHB(uint8_t trunk_cmd)
{
    switch (trunk_cmd)
    {
        case TRUNK_OPEN:
        case TRUNK_CLOSE:
        case TRUNK_LOCK:
        case TRUNK_UNLOCK:
            L99dz200g.L99DZ200G_Set_HB_Control(H_BRIDGE_CONTROL_A, ENABLE);
            L99dz200g.L99DZ200G_Set_HB_Control(H_BRIDGE_CONTROL_B, ENABLE);
            break;

        case TRUNK_STOP:
            L99dz200g.L99DZ200G_Set_HB_Control(H_BRIDGE_CONTROL_A, DISABLE);
            L99dz200g.L99DZ200G_Set_HB_Control(H_BRIDGE_CONTROL_B, DISABLE);
            break;

        default:
            break;
    }
}

/**
 * Enable the H-bridge motor mode for both H-bridges to drive three motors, depending
 * on the operation to do (set DMA and DMB in the register CFR).
 *
 * @param  trunk_cmd: (TRUNK_OPEN, TRUNK_CLOSE, TRUNK_LOCK, TRUNK_UNLOCK, TRUNK_STOP)
 */
void TrunkDualMotorMode(uint8_t trunk_cmd)
{
    switch (trunk_cmd)
    {
        case TRUNK_OPEN:    // H-bridge A: Single Mode, H-bridge B: Dual Mode
        case TRUNK_CLOSE:
            L99dz200g.L99DZ200G_Set_HB_MotorModeControl(H_BRIDGE_CONTROL_A, SINGLE_MOTOR);
            L99dz200g.L99DZ200G_Set_HB_MotorModeControl(H_BRIDGE_CONTROL_B, DUAL_MOTOR);
            break;

        case TRUNK_LOCK:    // H-bridge A: Dual Mode, H-bridge B: Dual Mode
        case TRUNK_UNLOCK:
            L99dz200g.L99DZ200G_Set_HB_MotorModeControl(H_BRIDGE_CONTROL_A, DUAL_MOTOR);
            L99dz200g.L99DZ200G_Set_HB_MotorModeControl(H_BRIDGE_CONTROL_B, DUAL_MOTOR);
            break;

        default:
            break;
    }
}

/**
 * Configuration to drive three motors with two H-bridges based on specified operation.
 *
 * @param  trunk_cmd: (TRUNK_OPEN, TRUNK_CLOSE, TRUNK_LOCK, TRUNK_UNLOCK, TRUNK_STOP)
 */
void TrunkConfiguration(uint8_t trunk_cmd)
{
//#define USE_ORIGINAL_VALS
    switch (trunk_cmd)
    {
        case TRUNK_OPEN:    // M1 = Single (H1A-L2A), M2 = Dual (H2B-L2A), M3 = off Dual (H1B/L1B)
            // 0x0 10 000; 
            // SD 2B = 0
            // SDS2B = 0    Dual Active freewheeling LS
            // SD 1B = 0
            // SDS1B = 1    Dual Passive freewheeling LS, Single Passive freewheeling through LS1 diode
            // SD 2A = 0
            // SDS2A = 0    Dual Active freewheeling LS
            // SD 1A = 0
            // SDS1A = 0    Dual Active freewheeling LS, Single Active freewheeling LS1 and LS2 on
#ifdef USE_ORIGINAL_VALS
            L99dz200g.L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, 0x0FF000, 0x010000);
#else
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG2, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG2, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG1, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG1, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingSide(H_BRIDGE_CONTROL_A, LOW_SIDE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingType(H_BRIDGE_CONTROL_A, ACTIVE_FREEWHEELING);
#endif
ShowReg(L99DZ200G_CR10);
            break;

        case TRUNK_CLOSE:   // M1 = Single (H2A-L1A), M2 = Dual (H1A-L2B), M3 = off Dual (H1B/L1B)
            // 0x0 92 000;    // 0x080000 | 0x010000 | 0x002000
            // SD 2B = 1
            // SDS2B = 0    Dual Active freewheeling HS
            // SD 1B = 0
            // SDS1B = 1    Dual Passive freewheeling LS, Single Passive freewheeling through LS1 diode
            // SD 2A = 0
            // SDS2A = 0    Dual Active freewheeling LS
            // SD 1A = 1
            // SDS1A = 0    Dual Active freewheeling HS, Single Active freewheeling HS1 and HS2 on
#ifdef USE_ORIGINAL_VALS
            L99dz200g.L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, 0x0FF000, 0x092000);
#else
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG2, HIGH_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG2, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG1, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG1, PASSIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingSide(H_BRIDGE_CONTROL_A, HIGH_SIDE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_SingleMotorFreewheelingType(H_BRIDGE_CONTROL_A, ACTIVE_FREEWHEELING);
#endif
ShowReg(L99DZ200G_CR10);
            break;

        case TRUNK_LOCK:    // M1 = off Dual, M2 = off Dual, M3 = Dual (H1B-L2A)
            // 0x0 49 000;    // 0x040000 | 0x008000 | 0x001000
            // SD 2B = 0
            // SDS2B = 1    Dual Passive freewheeling LS
            // SD 1B = 0
            // SDS1B = 0    Dual Active freewheeling LS, Single Active freewheeling LS1 and LS2 on
            // SD 2A = 1
            // SDS2A = 0    Dual Active freewheeling HS
            // SD 1A = 0
            // SDS1A = 1    Dual Passive freewheeling LS, Single Passive freewheeling through LS1/LS2 diode
#ifdef USE_ORIGINAL_VALS
            L99dz200g.L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, 0x0FF000, 0x049000);
#else
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG2, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG2, PASSIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG1, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG1, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_A, HB_LEG2, HIGH_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_A, HB_LEG2, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_A, HB_LEG1, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_A, HB_LEG1, PASSIVE_FREEWHEELING);
#endif
ShowReg(L99DZ200G_CR10);
            break;

        case TRUNK_UNLOCK:  // M1 = off Dual, M2 = off Dual, M3 = Dual (H2A-L1B)
            // 0x0 61 000;    // 0x040000 | 0x020000 | 0x001000
            // SD 2B = 0
            // SDS2B = 1    Dual Passive freewheeling LS
            // SD 1B = 1
            // SDS1B = 0    Dual Active freewheeling HS, Single Active freewheeling HS1 and HS2 on
            // SD 2A = 0
            // SDS2A = 0    Dual Active freewheeling LS
            // SD 1A = 0
            // SDS1A = 1    Dual Passive freewheeling LS, Single Passive freewheeling through LS1/LS2 diode
#ifdef USE_ORIGINAL_VALS
            L99dz200g.L99DZ200G_ModifyControlRegister(L99DZ200G_CR10, 0x0FF000, 0x061000);
#else
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG2, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG2, PASSIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_B, HB_LEG1, HIGH_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_B, HB_LEG1, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_A, HB_LEG2, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_A, HB_LEG2, ACTIVE_FREEWHEELING);
            L99dz200g.L99DZ200G_Set_HB_DualMotorDrive(H_BRIDGE_CONTROL_A, HB_LEG1, LOW_SIDE_DRIVE);
            L99dz200g.L99DZ200G_Set_HB_DualMotorFreewheelingType(H_BRIDGE_CONTROL_A, HB_LEG1, PASSIVE_FREEWHEELING);
#endif
ShowReg(L99DZ200G_CR10);
            break;

        default:
            break;
    }
}


// HB_PWMH1A, HB_PWMH2A, HB_PWMH1B, HB_PWMH2B
void SetTrunkHBridgePwmSetting(uint8_t which, uint8_t pwm)
{
    uint8_t pwm_dc = 0;
    uint8_t pwm_pin = 0;

    switch (which)
    {
        case HB_PWMH1A:
            pwm_pin = L99DZ200G_PWMH1A_PIN;
            break;
        case HB_PWMH2A:
            pwm_pin = L99DZ200G_PWMH2A_PIN;
            break;
        case HB_PWMH1B:
            pwm_pin = L99DZ200G_PWMH1B_PIN;
            break;
        case HB_PWMH2B:
            pwm_pin = L99DZ200G_PWMH2B_PIN;
            break;
        default:
            return;
    }

    HBridgePWM[which] = pwm;
    pwm_dc = map(pwm, 0, 100, 0, 255);
#if 1
    if (pwm)
    {
        digitalWrite(pwm_pin, HIGH);
    }
    else
    {
        digitalWrite(pwm_pin, LOW);
    }
#else
    analogWrite(pwm_pin, pwm_dc);
#endif
}

/**
 * Open trunk lift.
 *
 * @param duty:  Set duty cycle of the H-Bridge PWM (0 % to 100%)
 */
void TrunkLiftOpen(uint8_t duty)
{

    L99dz200g.L99DZ200G_Set_HB_SingleMotorDirectionControl(H_BRIDGE_CONTROL_A, RIGHT_DIRECTION);
    TrunkDualMotorMode(TRUNK_OPEN);
    TrunkConfiguration(TRUNK_OPEN);
    TrunkEnableHB(TRUNK_OPEN);
    SetTrunkHBridgePwmSetting(HB_PWMH1A, duty);
    SetTrunkHBridgePwmSetting(HB_PWMH2B, duty);
}

/**
 * Stop trunk lift.
 */
void TrunkLiftStop(void)
{
    TrunkEnableHB(TRUNK_STOP);
    SetTrunkHBridgePwmSetting(HB_PWMH1A, 0);
    SetTrunkHBridgePwmSetting(HB_PWMH2B, 0);

    SetTrunkHBridgePwmSetting(HB_PWMH1B, 0);
    SetTrunkHBridgePwmSetting(HB_PWMH2A, 0);
}

/**
 * Close trunk lift.
 *
 * @param duty:  Set duty cycle of the H-Bridge PWM (0 % to 100%)
 */
void TrunkLiftClose(uint8_t duty)
{
    L99dz200g.L99DZ200G_Set_HB_SingleMotorDirectionControl(H_BRIDGE_CONTROL_A, LEFT_DIRECTION);
    TrunkDualMotorMode(TRUNK_CLOSE);
    TrunkConfiguration(TRUNK_CLOSE);
    TrunkEnableHB(TRUNK_CLOSE);
    SetTrunkHBridgePwmSetting(HB_PWMH1A, duty);
    SetTrunkHBridgePwmSetting(HB_PWMH2B, duty);
}

/**
 * Unlock trunk lift.
 *
 * @param duty:  Set duty cycle of the H-Bridge PWM (0 % to 100%)
 */
void TrunkUnlock(uint8_t duty)
{
    TrunkDualMotorMode(TRUNK_UNLOCK);
    TrunkConfiguration(TRUNK_UNLOCK);
    TrunkEnableHB(TRUNK_UNLOCK);
    SetTrunkHBridgePwmSetting(HB_PWMH1B, duty);
}

/**
 * Stop lock trunk lift.
 */
void TrunkLockStop(void)
{
    TrunkEnableHB(TRUNK_STOP);
    SetTrunkHBridgePwmSetting(HB_PWMH1B, 0);
    SetTrunkHBridgePwmSetting(HB_PWMH2A, 0);

    SetTrunkHBridgePwmSetting(HB_PWMH1A, 0);
    SetTrunkHBridgePwmSetting(HB_PWMH2B, 0);
}

/**
 * Lock trunk lift.
 *
 * @param duty:  Set duty cycle of the H-Bridge PWM (0 % to 100%)
 */
void TrunkLock(uint8_t duty)
{
    TrunkDualMotorMode(TRUNK_LOCK);
    TrunkConfiguration(TRUNK_LOCK);
    TrunkEnableHB(TRUNK_LOCK);
    SetTrunkHBridgePwmSetting(HB_PWMH2A, duty);
}

/**
 * Open trunk lift for 'ms_delay' milliseconds.
 *
 * @param duty:         Set duty cycle of the H-Bridge PWM (0 % to 100%)
 * @param  ms_delay     Time in milliseconds
 */
void TrunkLiftOpenWithDelay(uint8_t duty, uint32_t ms_delay)
{
    TrunkLiftOpen(duty);
    if (ms_delay != 0)
    {
        L99dz200g.L99DZ200G_Delay(ms_delay);
        TrunkLiftStop();
    }
}

/**
 * Close trunk lift for 'ms_delay' milliseconds.
 *
 * @param duty:         Set duty cycle of the H-Bridge PWM (0 % to 100%)
 * @param  ms_delay     Time in milliseconds
 */
void TrunkLiftCloseWithDelay(uint8_t duty, uint32_t ms_delay)
{
    TrunkLiftClose(duty);
    if (ms_delay != 0)
    {
        L99dz200g.L99DZ200G_Delay(ms_delay);
        TrunkLiftStop();
    }
}

/**
 * Unlock trunk lift for 'ms_delay' milliseconds.
 *
 * @param duty:         Set duty cycle of the H-Bridge PWM (0 % to 100%)
 * @param  ms_delay     Time in milliseconds
 */
void TrunkUnlockWithDelay(uint8_t duty, uint32_t ms_delay)
{
    TrunkUnlock(duty);
    if (ms_delay != 0)
    {
        L99dz200g.L99DZ200G_Delay(ms_delay);
        TrunkLockStop();
    }
}

/**
 * Lock trunk lift for 'ms_delay' milliseconds.
 *
 * @param duty:         Set duty cycle of the H-Bridge PWM (0 % to 100%)
 * @param  ms_delay     Time in milliseconds
 */
void TrunkLockWithDelay(uint8_t duty, uint32_t ms_delay)
{
    TrunkLock(duty);
    if (ms_delay != 0)
    {
        L99dz200g.L99DZ200G_Delay(ms_delay);
        TrunkLockStop();
    }
}

// ON_OUT or OFF_OUT
void TrunkLightsControl(uint8_t output_type)
{
    L99dz200g.L99DZ200G_HSOutputsControl(output_type, OUT_7);
    L99dz200g.L99DZ200G_HSOutputsControl(output_type, OUT_8);
}

