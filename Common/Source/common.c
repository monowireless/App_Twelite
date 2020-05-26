/****************************************************************************
 * (C) Mono Wireless Inc. - 2016 all rights reserved.
 *
 * Condition to use: (refer to detailed conditions in Japanese)
 *   - The full or part of source code is limited to use for TWE (The
 *     Wireless Engine) as compiled and flash programmed.
 *   - The full or part of source code is prohibited to distribute without
 *     permission from Mono Wireless.
 *
 * 利用条件:
 *   - 本ソースコードは、別途ソースコードライセンス記述が無い限りモノワイヤレスが著作権を
 *     保有しています。
 *   - 本ソースコードは、無保証・無サポートです。本ソースコードや生成物を用いたいかなる損害
 *     についてもモノワイヤレスは保証致しません。不具合等の報告は歓迎いたします。
 *   - 本ソースコードは、モノワイヤレスが販売する TWE シリーズ上で実行する前提で公開
 *     しています。他のマイコン等への移植・流用は一部であっても出来ません。
 *
 ****************************************************************************/

#include <jendefs.h>
#include <string.h>
#ifdef JN514x
#include <AppHardwareApi_JN514x.h>
#else
#include <AppHardwareApi.h>
#endif

#include "ToCoNet.h"

#include "config.h"
#include "utils.h"
#include "modbus_ascii.h"

#include "common.h"
#include "Version.h"

/** @ingroup MASTER
 * DI のポート番号のテーブル
 */
const uint8 au8PortTbl_DIn_std[4] = {
	PORT_INPUT1,
	PORT_INPUT2,
	PORT_INPUT3,
	PORT_INPUT4
};

/** @ingroup MASTER
 * DI のポート番号の副テーブル
 */
const uint8 au8PortTbl_DIn_swp[4] = {
	5,
	8,
	15,
	16
};

/** @ingroup MASTER
 * DI のポート番号のテーブルポインタ
 */
const uint8 * au8PortTbl_DIn = au8PortTbl_DIn_std;

/** @ingroup MASTER
 * DI のポート番号のテーブルポインタ
 */
uint32 u32PortInputMask; // auPortTbl_DIn をマスク化した

/** @ingroup MASTER
 * ボーレート変更用のポート番号
 */
uint8 u8PortBaud;

/** @ingroup MASTER
 * テーブルを定義する。
 * @param u8Tbl 0:主テーブル 1:副テーブル
 */
void vSetPortTblMap(uint8 u8Tbl) {
	// テーブル配列の設定
	if (u8Tbl == 1) {
		au8PortTbl_DIn = au8PortTbl_DIn_swp;
		u8PortBaud = 14;
	} else {
		au8PortTbl_DIn = au8PortTbl_DIn_std;
		u8PortBaud = PORT_BAUD;
	}

	// マスクの生成
	int i;
	u32PortInputMask = 0;
	for (i = 0; i < 4; i++) {
		u32PortInputMask |= (1UL << au8PortTbl_DIn[i]);
	}
}

/** @ingroup MASTER
 * DO のポート番号のテーブル
 */
const uint8 au8PortTbl_DOut[4] = {
	PORT_OUT1,
	PORT_OUT2,
	PORT_OUT3,
	PORT_OUT4
};

/** @ingroup MASTER
 * タイマーデバイス番号のテーブル
 */
const uint8 au8PortTbl_PWM_Timer[4] = {
#if defined(JN516x)
	E_AHI_DEVICE_TIMER1,
	E_AHI_DEVICE_TIMER2,
	E_AHI_DEVICE_TIMER3,
	E_AHI_DEVICE_TIMER4
#elif defined(JN514x)
	0xFF,
	0xFF,
	E_AHI_DEVICE_TIMER1,
	E_AHI_DEVICE_TIMER2
#endif
};

/** @ingroup MASTER
 * MODE設定ビットからデフォルト割り当てされる論理ＩＤテーブル
 */
const uint8 au8IoModeTbl_To_LogicalID[8] = {
	120, // CHILD
	0,   // PARENT
	254, // ROUTER
	123, // 32fps mode (7B)
	124, // 1sec sleep (7C)
	125, // RESPMODE (7D)
	255, // nodef
	127  // 10sec sleep (7F)
};

/** @ingroup MBUSA
 * MODBUS ASCII シリアル出力用のバッファ
 */
extern uint8 au8SerOutBuff[];

/** @ingroup MBUSA
 * 自身のシリアル番号を出力する（起動時メッセージにも利用）
 * @param pSer 出力先ストリーム
 */
void vModbOut_MySerial(tsFILE *pSer) {
	uint8 *q = au8SerOutBuff;

	S_OCTET(VERSION_MAIN);
	S_OCTET(VERSION_SUB);
	S_OCTET(VERSION_VAR);

	S_BE_DWORD(ToCoNet_u32GetSerial());

	vSerOutput_ModbusAscii(pSer,
			SERCMD_ADDR_FR_MODULE,
			SERCMD_ID_INFORM_MODULE_ADDRESS,
			au8SerOutBuff,
			q - au8SerOutBuff);
}

/**
 * 文字列を区切り文字で分割する。pstr は NUL 文字で分割される。
 */
uint8 u8StrSplitTokens(uint8 *pstr, uint8 **auptr, uint8 u8max_entry) {
	uint8 u8num = 0;

	uint8 *p = pstr;
	if (pstr == NULL || *p == 0) {
		return 0;
	} else {

		auptr[0] = pstr;
		u8num = 1;

		while (*p) {
			if (*p == ',') {
				*p = 0;
				auptr[u8num] = p + 1;
				u8num++;
				if (u8num >= u8max_entry) {
					break;
				}
			}

			p++;
		}
	}

	return u8num;
}

#if 0
/** @ingroup MBUSA
 * ACK/NACK を出力する
 * @param pSer 出力先ストリーム
 * @param bAck TRUE:ACK, FALSE:NACK
 */
void vModbOut_AckNack(tsFILE *pSer, bool_t bAck) {
	uint8 *q = au8SerOutBuff;

	S_OCTET(bAck ? 1 : 0);

	vSerOutput_ModbusAscii(pSer,
			SERCMD_ADDR_FR_MODULE,
			bAck ? SERCMD_ID_ACK : SERCMD_ID_NACK,
			au8SerOutBuff,
			q - au8SerOutBuff);
}


/** @ingroup MBUSA
 * フラッシュ設定データ列を解釈する。入力は modbus のアドレス・コマンドを含むデータ列。
 * ※ 本実装は実験的で、フラッシュのデータ全てに対応していません。
 *
 * @param p 入力バイト列
 * @param pConfig データの書き出し先
 * @return TRUE: データが正しい
 */
bool_t bModbIn_Config(uint8 *p,  tsFlashApp *pConfig) {
	uint8 u8adr;
	uint8 u8cmd;
	OCTET(u8adr);
	OCTET(u8cmd);
	BE_DWORD(pConfig->u32appid);
	OCTET(pConfig->u8role);
	OCTET(pConfig->u8layer);
	OCTET(pConfig->u8ch);
	BE_DWORD(pConfig->u32chmask);

	// 必要ならデータの正当性チェックを行う！

	return TRUE;
}

/** @ingroup MBUSA
 * フラッシュ設定を出力する。
 * ※ 本実装は実験的で、フラッシュのデータ全てに対応していません。
 *
 * @param pSer 出力先ストリーム
 * @param pConfig 設定構造体
 */
void vModbOut_Config(tsFILE *pSer, tsFlashApp *pConfig) {
	uint8 *q = au8SerOutBuff;

	S_BE_DWORD(pConfig->u32appid);
	S_OCTET(pConfig->u8role);
	S_OCTET(pConfig->u8layer);
	S_OCTET(pConfig->u8ch);
	S_BE_DWORD(pConfig->u32chmask);

	vSerOutput_ModbusAscii(pSer,
			SERCMD_ADDR_FR_MODULE,
			SERCMD_ID_INFORM_NETWORK_CONFIG,
			au8SerOutBuff,
			q - au8SerOutBuff);
}
#endif
