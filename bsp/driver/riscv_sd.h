#ifndef _RISCV_SPISD_H
#define _RISCV_SPISD_H

#include <stdint.h>

#define SPI_REG_SCKDIV          (0x00u >> 2)
#define SPI_REG_SCKMODE         (0x04u >> 2)
#define SPI_REG_CSID            (0x10u >> 2)
#define SPI_REG_CSDEF           (0x14u >> 2)
#define SPI_REG_CSMODE          (0x18u >> 2)
                                
#define SPI_REG_DCSSCK          (0x28u >> 2)
#define SPI_REG_DSCKCS          (0x2au >> 2)
#define SPI_REG_DINTERCS        (0x2cu >> 2)
#define SPI_REG_DINTERXFR       (0x2eu >> 2)
                                
#define SPI_REG_FMT             (0x40u >> 2)
#define SPI_REG_TXFIFO          (0x48u >> 2)
#define SPI_REG_RXFIFO          (0x4cu >> 2)
#define SPI_REG_TXCTRL          (0x50u >> 2)
#define SPI_REG_RXCTRL          (0x54u >> 2)
                                
#define SPI_REG_FCTRL           (0x60u >> 2)
#define SPI_REG_FFMT            (0x64u >> 2)
                                
#define SPI_REG_IE              (0x70u >> 2)
#define SPI_REG_IP              (0x74u >> 2)

#define SPI_CSMODE_AUTO         0
#define SPI_CSMODE_HOLD         2
#define SPI_CSMODE_OFF          3

#define SD_BLOCK_SIZE           512u

#define SPISD_WORK_CLK          50000000UL
#define SD_ENUM_CLK             (300 * 1000ul)
#define SD_RW_CLK               (5 * 1000ul * 1000ul)


void riscv_spisd_init(void);
int riscv_sd_enumerate(void);
int riscv_sd_read(uint32_t u32StartBlk, uint8_t *pu8Ptr, uint32_t u32BlockCnt);


#endif
