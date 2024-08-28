#include "libopencm3/stm32/flash.h"
#include "hal.h"
#include <stdint.h>

void FlashWrite(uint32_t addr, uint32_t* data) {
	flash_wait_for_last_operation();
    // There is no HAL_FLASH_Unlock() in this framework
    // so use similar one
	flash_unlock();
	flash_wait_for_last_operation();

    flash_program_word();


}