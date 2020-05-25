/*
 * flash.c
 *
 *  Created on: 2012/11/23
 *      Author: seigo13
 */

#include <jendefs.h>
#include <string.h>

#ifdef JN514x
#include <AppHardwareApi_JN514x.h>
#include <AppApi_JN514x.h>
#else
#include <AppHardwareApi.h>
#include <AppApi.h>
#endif

#include "flash.h"
#include "ccitt8.h"

#define FLASH_MAGIC_NUMBER 0xA501EF5A //!< フラッシュ書き込み時のマジック番号  @ingroup FLASH

/** @ingroup FLASH
 * フラッシュの読み込み
 * @param psFlash 読み込み格納データ
 * @param sector 読み出しセクタ
 * @param offset セクタ先頭からのオフセット
 * @return TRUE:読み出し成功 FALSE:失敗
 */
bool_t bFlash_Read(tsFlash *psFlash, uint8 sector, uint32 offset) {
    bool_t bRet = FALSE;
    offset += (uint32)sector * FLASH_SECTOR_SIZE; // calculate the absolute address

    if (bAHI_FlashInit(FLASH_TYPE, NULL) == TRUE) {
        if (bAHI_FullFlashRead(offset, sizeof(tsFlash), (uint8 *)psFlash)) {
            bRet = TRUE;
        }
    }

    // validate content
    if (bRet && psFlash->u32Magic != FLASH_MAGIC_NUMBER) {
    	bRet = FALSE;
    }
    if (bRet && psFlash->u8CRC != u8CCITT8((uint8*)&(psFlash->sData), sizeof(tsFlashApp))) {
    	bRet = FALSE;
    }

    return bRet;
}

/**  @ingroup FLASH
 * フラッシュ書き込み
 * @param psFlash 書き込みたいデータ
 * @param sector 書き込みセクタ
 * @param offset 書き込みセクタ先頭からのオフセット
 * @return TRUE:書き込み成功 FALSE:失敗
 */
bool_t bFlash_Write(tsFlash *psFlash, uint8 sector, uint32 offset)
{
    bool_t bRet = FALSE;
    offset += (uint32)sector * FLASH_SECTOR_SIZE; // calculate the absolute address

    if (bAHI_FlashInit(FLASH_TYPE, NULL) == TRUE) {
        if (bAHI_FlashEraseSector(sector) == TRUE) { // erase a corresponding sector.
        	psFlash->u32Magic = FLASH_MAGIC_NUMBER;
        	psFlash->u8CRC = u8CCITT8((uint8*)&(psFlash->sData), sizeof(tsFlashApp));
            if (bAHI_FullFlashProgram(offset, sizeof(tsFlash), (uint8 *)psFlash)) {
                bRet = TRUE;
            }
        }
    }

    return bRet;
}
