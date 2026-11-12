#include "eeprom.h"
#include <string.h>

bool save_flash_nolib(const uint8_t *data, uint32_t length, uint32_t address)
{
  if (address < MCU_FLASH_START || address >= M030_FLASH_END ||
    length > M030_FLASH_END - address ||
    (length == 0 && address % DPM_PAGE_SIZE != 0)) {
    return false;
  }

  const uint32_t irq_mask = __get_PRIMASK();
  __disable_irq();
  FLASH_Unlock();
  FLASH->SR = 0x7EUL; /* Write one to clear the M030 error flags. */
  bool ok = true;

  if (length == 0) {
    ok = FLASH_ErasePage(address) == FLASH_OK;
  }
  while (length > 0 && ok) {
    const uint32_t page = address & ~(DPM_PAGE_SIZE - 1UL);
    const uint32_t offset = address - page;
    uint32_t count = DPM_PAGE_SIZE - offset;
    if (count > length) {
      count = length;
    }

    /* Preserve the other half of a page on 256-byte writes and retries. */
    uint32_t buffer[DPM_PAGE_SIZE / sizeof(uint32_t)];
    memcpy(buffer, (const void *)page, sizeof(buffer));
    memcpy((uint8_t *)buffer + offset, data, count);
    ok = FLASH_ErasePage(page) == FLASH_OK;
    for (uint32_t i = 0; i < DPM_PAGE_SIZE / sizeof(uint32_t) && ok; i++) {
      if (buffer[i] != 0xFFFFFFFFUL) {
        ok = FLASH_ProgramWord(page + i * sizeof(uint32_t), buffer[i]) == FLASH_OK;
      }
    }
    if (ok) {
      ok = memcmp(buffer, (const void *)page, sizeof(buffer)) == 0;
    }
    address += count;
    data += count;
    length -= count;
  }

  FLASH_Lock();
  __set_PRIMASK(irq_mask);
  return ok;
}

void read_flash_bin(uint8_t *data, uint32_t address, int length)
{
  memcpy(data, (const void *)address, (size_t)length);
}
