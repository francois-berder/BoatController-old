#include "mcc_generated_files/spi1.h"
#include "sd.h"


uint8_t sd_init(void)
{
	uint8_t i = 0;
	
	/* Reducing speed of SPI to 400kHz */
	SPI1BRGL = 19;
	
	while (1)
		for (i = 0; i < 16; ++i)
			printf("%02X\n", SPI1_Exchange8bit(0xA1));
	return 0;
}