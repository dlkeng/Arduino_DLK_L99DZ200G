/****************************************************************************
*
* Copyright Â© 2015-2020 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
****************************************************************************
*
* Copyright © 2015-2020 STMicroelectronics - All Rights Reserved
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
 * @file    CANCommunication.h
 * @brief   CAN communication commands.
 */

#ifndef _CANCommunication_LLD_H_
#define _CANCommunication_LLD_H_

#define SID_MR200G                              0x7f0U

/********************************************************************************
 *******************************CAN DICTIONARY***********************************
 ********************************************************************************/

#define ID_MR_200G                              0x7f0U

#define AEK_MOT_MR200G1_DEMO_ON_CMD             0x00001188UL
#define AEK_MOT_MR200G1_DEMO_OFF_CMD            0x88220000UL

#define AEK_MOT_MR200G1_MIRROR_FOLDING_CMD      0x11110000UL
#define AEK_MOT_MR200G1_MIRROR_UNFOLDING_CMD    0x11220000UL
#define AEK_MOT_MR200G1_MIRROR_X_CK_CMD         0x22110000UL
#define AEK_MOT_MR200G1_MIRROR_X_CCK_CMD        0x22220000UL
#define AEK_MOT_MR200G1_MIRROR_Y_CK_CMD         0x33110000UL
#define AEK_MOT_MR200G1_MIRROR_Y_CCK_CMD        0x33220000UL
#define AEK_MOT_MR200G1_MIRROR_BRK_CMD          0x33330000UL
#define AEK_MOT_MR200G1_MIRROR_CENTER_CMD       0x77770000UL

#define AEK_MOT_MR200G1_BULBS_ON_CMD            0x44110000UL
#define AEK_MOT_MR200G1_BULBS_OFF_CMD           0x44220000UL
#define AEK_MOT_MR200G1_BULBS_FLASH_CMD         0x44330000UL

#define AEK_MOT_MR200G1_HEATER_ON_CMD           0x11660000UL
#define AEK_MOT_MR200G1_HEATER_OFF_CMD          0x11770000UL

#define AEK_MOT_MR200G1_ECV_ON_CMD              0x11440000UL
#define AEK_MOT_MR200G1_ECV_OFF_CMD             0x11550000UL
#define AEK_MOT_MR200G1_ECV_TARGET_CMD          0x66CC0000UL // Target value may be written on 16 data LSB

#endif

