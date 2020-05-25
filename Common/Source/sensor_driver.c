/****************************************************************************
 * (C) Tokyo Cosmos Electric, Inc. (TOCOS) - 2012 all rights reserved.
 *
 * Condition to use:
 *   - The full or part of source code is limited to use for TWE (TOCOS
 *     Wireless Engine) as compiled and flash programmed.
 *   - The full or part of source code is prohibited to distribute without
 *     permission from TOCOS.
 *
 ****************************************************************************/


#include "jendefs.h"
#include <AppHardwareApi.h>

#include "ToCoNet_event.h"
#include "sensor_driver.h"

void vSnsObj_Process(tsSnsObj *pObj, teEvent eEv) {
	uint8 u8status_init;

	u8status_init = pObj->u8State;
	pObj->pvProcessSnsObj(pObj, eEv);

	// 連続的に状態遷移させるための処理。pvProcessSnsObj() 処理後に状態が前回と変化が有れば、
	// 再度 E_EVENT_NEW_STATE により実行する。
	while (pObj->u8State != u8status_init) {
		u8status_init = pObj->u8State;
		pObj->pvProcessSnsObj(pObj, E_EVENT_NEW_STATE);
	}
}

