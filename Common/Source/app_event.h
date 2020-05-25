/****************************************************************************
 * (C) Tokyo Cosmos Electric, Inc. (TOCOS) - 2013 all rights reserved.
 *
 * Condition to use:
 *   - The full or part of source code is limited to use for TWE (TOCOS
 *     Wireless Engine) as compiled and flash programmed.
 *   - The full or part of source code is prohibited to distribute without
 *     permission from TOCOS.
 *
 ****************************************************************************/

/*
 * event.h
 *
 *  Created on: 2013/01/10
 *      Author: seigo13
 */

#ifndef APP_EVENT_H_
#define APP_EVENT_H_

#include "ToCoNet_event.h"

/** @ingroup MASTER
 * アプリケーション内で使用するイベント
 */
typedef enum
{
	E_EVENT_APP_BASE = ToCoNet_EVENT_APP_BASE,//!< ToCoNet 組み込みイベントと重複させないため
    E_EVENT_APP_TICK_A,                    //!< 64FPS のタイマーイベント
    E_EVENT_APP_ADC_COMPLETE,              //!< ADC完了
    E_EVENT_APP_TX_COMPLETE                //!< TX完了
} teEventApp;

/** @ingroup MASTER
 * アプリケーション内で使用する状態名
 */
typedef enum
{
	E_STATE_APP_BASE = ToCoNet_STATE_APP_BASE,//!< ToCoNet 組み込み状態と重複させないため
	E_STATE_APP_WAIT_IO_FIRST_CAPTURE,        //!< 最初のADCやDIの状態確定を待つ
	E_STATE_APP_SLEEPING                      //!< スリープ処理
} teStateApp;

#endif /* EVENT_H_ */
