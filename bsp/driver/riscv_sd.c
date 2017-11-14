#include <string.h>
#include "bsp.h"

#define __spisd_getClk()    SPISD_WORK_CLK


volatile uint32_t* g_pu32Spisd;


static uint16_t _sd_crc16_round(uint16_t crc, uint8_t data) 
{
	crc = (uint8_t)(crc >> 8) | (crc << 8);
	crc ^= data;
	crc ^= (uint8_t)(crc >> 4) & 0xf;
	crc ^= crc << 12;
	crc ^= (crc & 0xff) << 5;
	return crc;
}

static uint8_t _sd_crc7_cal(uint8_t *pSrc, uint32_t cnt)
{
  uint8_t crc7 = 0x89;
  uint8_t count = 0, temp1 = 0, temp2 = 0, crc = pSrc[0];

  count = 8;

  while(count < cnt * 8)
  {
    temp1 = crc & 0x80;
    while(0 == temp1)
    {
      crc = (crc << 1) & 0xfe;
      temp2 = pSrc[count / 8];
      temp2 = (temp2 >> (7 - (count % 8))) & 0x01; 
      crc = crc | temp2;
      temp1 = crc & 0x80;
      count++;

      if(count == cnt * 8)
        break;
     }
    
     if((count == cnt * 8) && (0 == temp1))
      break;
     
     crc = crc ^ crc7; 
  }
  
  return crc;
}

static uint8_t _sd_crc_cmd(uint8_t u8Cmd, uint32_t u32Arg)
{
  uint8_t u8Crc, u8Buf[6];

  u8Buf[0] = u8Cmd;
  u8Buf[1] = u32Arg >> 24;
	u8Buf[2] = u32Arg >> 16;
	u8Buf[3] = u32Arg >> 8;
	u8Buf[4] = u32Arg;

  u8Crc = _sd_crc7_cal(u8Buf, 5);
  u8Crc = (u8Crc << 1) + 1;
  return u8Crc;
}

static uint8_t riscv_spi_xfer(uint8_t d)
{
	int32_t r;

  g_pu32Spisd[SPI_REG_TXFIFO] = d;

  do
  {
    r = g_pu32Spisd[SPI_REG_RXFIFO];
  }while(r < 0);
  
	return r;
}

static uint8_t riscv_sd_dummy(void)
{
	return riscv_spi_xfer(0xFF);
}

static inline void riscv_sd_cmd_end(void)
{
	riscv_sd_dummy();
  g_pu32Spisd[SPI_REG_CSMODE] = SPI_CSMODE_AUTO;
}

static uint8_t riscv_sd_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	unsigned long n = 1000;
	uint8_t r;

  g_pu32Spisd[SPI_REG_CSMODE] = SPI_CSMODE_HOLD;
  riscv_sd_dummy();
  riscv_spi_xfer(cmd);
  riscv_spi_xfer(arg >> 24);
	riscv_spi_xfer(arg >> 16);
	riscv_spi_xfer(arg >> 8);
	riscv_spi_xfer(arg);
	riscv_spi_xfer(crc);

	while(n--)
	{
		r = riscv_sd_dummy();
		if (!(r & 0x80))
			break;
	}
  
	return r;
}


int riscv_sd_enumerate(void)
{
  int rc;
  
  /* reset to idle */
  riscv_sd_cmd(0x40, 0, _sd_crc_cmd(0x40, 0)); // 0x95
	riscv_sd_cmd_end();

  /* v2.0 cards detection */
  riscv_sd_cmd(0x48, 0x000001AA, _sd_crc_cmd(0x48, 0x000001AA)); // 0x87);
	riscv_sd_dummy(); /* command version; reserved */
	riscv_sd_dummy(); /* reserved */
	rc = ((riscv_sd_dummy() & 0xF) != 0x1); /* voltage */
	rc |= (riscv_sd_dummy() != 0xAA); /* check pattern */
	riscv_sd_cmd_end();

  if(rc)
  {
    /* reset to idle for supporting v1.1 cards */
    riscv_sd_cmd(0x40, 0, _sd_crc_cmd(0x40, 0));
	  riscv_sd_cmd_end();
  }

  /* sd card initialization */
  do 
  {
		riscv_sd_cmd(0x77, 0, _sd_crc_cmd(0x77, 0)); // cmd55
	  riscv_sd_cmd_end();
  
		rc = riscv_sd_cmd(0x69, 0x40000000, _sd_crc_cmd(0x69, 0x40000000)); /* cmd41  HCS = 1 */
	} while (rc == 0x01);

  rc = (riscv_sd_cmd(0x7A, 0, _sd_crc_cmd(0x7a, 0)) != 0x00);  /* cmd58 */
	rc |= ((riscv_sd_dummy() & 0x80) != 0x80); /* Power up status */
	riscv_sd_dummy();
	riscv_sd_dummy();
	riscv_sd_dummy();
	riscv_sd_cmd_end();
  if(rc)
    return -1;

  riscv_sd_cmd(0x50, SD_BLOCK_SIZE, _sd_crc_cmd(0x50, SD_BLOCK_SIZE));  /* cmd16, set block size */
	riscv_sd_cmd_end();

  return 0;
}

int riscv_sd_read(uint32_t u32StartBlk, uint8_t *pu8Ptr, uint32_t u32BlockCnt)
{
	int32_t i, s32Res = 0;
  uint16_t u16Crc, u16CrcExp;

  g_pu32Spisd[SPI_REG_SCKDIV] = __spisd_getClk() / SD_RW_CLK;

  /*
   * cmd18 multi-block read start
   */
  if(riscv_sd_cmd(0x52, u32StartBlk, _sd_crc_cmd(0x52, u32StartBlk)) != 0)
  {
    riscv_sd_cmd_end();
    return -1;
  }

  do
  {
    u16Crc = 0;

    while(riscv_sd_dummy() != 0xFE);

    for(i=0; i<SD_BLOCK_SIZE; i++)
    {
      pu8Ptr[i] = riscv_sd_dummy();
      u16Crc = _sd_crc16_round(u16Crc, pu8Ptr[i]);
    }

    u16CrcExp = (uint16_t)riscv_sd_dummy() << 8;
    u16CrcExp |= riscv_sd_dummy();

    if(u16Crc != u16CrcExp)
    {
      s32Res = -1;
      break;
    }

    pu8Ptr += SD_BLOCK_SIZE;
    
  }while(--u32BlockCnt);

  riscv_sd_cmd_end();

  /*
   * cmd12 stop multi-block read
   */
	riscv_sd_cmd(0x4C, 0, _sd_crc_cmd(0x4c, 0));
	riscv_sd_cmd_end();
  return s32Res;
}

void riscv_spisd_init(void)
{
  int i; 
  
  g_pu32Spisd = (void*)(uintptr_t)SPI_BASE;

  /* power on */
  g_pu32Spisd[SPI_REG_SCKDIV] = __spisd_getClk() / SD_ENUM_CLK;
  g_pu32Spisd[SPI_REG_CSMODE] = SPI_CSMODE_OFF;
  for (i = 10; i > 0; i--)
		riscv_sd_dummy();
  g_pu32Spisd[SPI_REG_CSMODE] = SPI_CSMODE_AUTO;  
}



