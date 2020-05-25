/****************************************************************************
 * (C) Tokyo Cosmos Electric, Inc. (TOCOS) - 2013 all rights reserved.
 *
 * Condition to use: (refer to detailed conditions in Japanese)
 *   - The full or part of source code is limited to use for TWE (TOCOS
 *     Wireless Engine) as compiled and flash programmed.
 *   - The full or part of source code is prohibited to distribute without
 *     permission from TOCOS.
 *
 * 利用条件:
 *   - 本ソースコードは、別途ソースコードライセンス記述が無い限り東京コスモス電機が著作権を
 *     保有しています。
 *   - 本ソースコードは、無保証・無サポートです。本ソースコードや生成物を用いたいかなる損害
 *     についても東京コスモス電機は保証致しません。不具合等の報告は歓迎いたします。
 *   - 本ソースコードは、東京コスモス電機が販売する TWE シリーズ上で実行する前提で公開
 *     しています。他のマイコン等への移植・流用は一部であっても出来ません。
 *
 ****************************************************************************/

#ifndef  CONFIG_H_INCLUDED
#define  CONFIG_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <AppHardwareApi.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Serial Configuration */
#define UART_BAUD			115200UL //!< UART のボーレート（デフォルト）
#define UART_BAUD_SAFE		38400UL //!< UART のボーレート（他の設定）
#define UART_PARITY_ENABLE	E_AHI_UART_PARITY_DISABLE //!< パリティは none
#define UART_PARITY_TYPE 	E_AHI_UART_EVEN_PARITY //!< E_AHI_UART_PARITY_ENABLE 時の指定
#define UART_STOPBITS 		E_AHI_UART_1_STOP_BIT //!< ストップビットは１

/* Specify which serial port to use when outputting debug information */
#define UART_PORT_MASTER    E_AHI_UART_0 //!< UARTポートの指定

/* Specify the PAN ID and CHANNEL to be used by tags, readers and gateway */
#define APP_ID              0x67720102 //!< アプリケーションID。同じIDでないと通信しない。
//#define CHANNEL             17
//#define CHMASK              ((1UL << 11) | (1UL << 17) | (1UL << 25))
#define CHANNEL 18 //!< 使用するチャネル
#define CHMASK (1UL << CHANNEL) //!< チャネルマスク（３つまで指定する事が出来る）

// Coordinator specific settings
#define PARENT_ADDR     	0x8001

// SERIAL BUFFERS
#define SERCMD_SER_PKTLEN 80 //!< シリアルメッセージのデータ部の最大バイト数
#define SERCMD_SER_PKTNUM 13 //!< シリアルメッセージの最大送信パケット数
#define SERCMD_MAXPAYLOAD (SERCMD_SER_PKTLEN*SERCMD_SER_PKTNUM) //!< シリアルメッセージのバッファサイズ

// ADC
#ifdef JN516x
#define ADC_MAC_MV 2470 //!< ADCの最大電圧（実際は bandgap 電圧によって変わる）
#define ADC_MAX_THRES (2050) //!< ADC の振り切れ判定値
#elif defined(JN514x)
#define ADC_MAC_MV 2400 //!< ADCの最大電圧（実際は bandgap 電圧によって変わる）
#define ADC_MAX_THRES (2050) //!< ADC の振り切れ判定値
#endif
#define IS_ADC_RANGE_OVER(c) ((c) >= ADC_MAX_THRES) //!< ADCの振り切れ判定
#define ADC_DELTA_COARSE 64 //!< ADCの変化を判定する幅（粗）  0-2V スケールを想定し、その 5%=100mV
#define ADC_DELTA_FINE (ADC_DELTA_COARSE/2) //!< ADCの変化を判定する幅（細）
#define ADC_TIMEOUT_TO_FINE_CHECK 300 //!< ADCの変化判定を細かくする時間判定

#undef IGNORE_ADC_CHAGNE //!< ADC の変化判定を無効にする（デバッグ用）

// スリープ間隔
#define MODE4_SLEEP_DUR_ms 1000UL
#define MODE7_SLEEP_DUR_ms 10000UL

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	uint8 u8IdSender;
	uint8 u8IdReceiver;
	bool_t bPktStatus[SERCMD_SER_PKTNUM];
	uint8 u8PktNum;
	uint16 u16DataLen;
	uint8 u8Seq, u8SeqNext;
	uint8 u8ReqNum;
	bool_t bWaitComplete;
	bool_t bRelayPacket; //!< 中継パケットが含まれる？この場合再中継しない。

	uint32 u32Tick;
	uint32 u32SrcAddr;
} tsSerSeq;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* CONFIG_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
