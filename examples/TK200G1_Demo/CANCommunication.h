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

#define SID_MOTOR               0x7f0U          // Standard ID for basic motor testing
#define STOP_MOTOR              0x1A1B1C1DUL
#define CLOCKWISE_MOTOR         0x2A2B2C2DUL
#define COUNTERCLOCKWISE_MOTOR  0x3A3B3C3DUL

#define SID_TRUNK_CMD           0x7f1U          // Standard ID for trunk operations
#define TRUNK_OPEN_CMD          0 
#define TRUNK_CLOSE_CMD         1
#define TRUNK_LOCK_CMD          2
#define TRUNK_UNLOCK_CMD        3
#define TRUNK_STOP_CMD          4
#define TRUNK_TEST_CMD          5

#endif
