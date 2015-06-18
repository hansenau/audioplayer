/*
 * sdcard.h
 *
 *  Created on: May 22, 2015
 *      Author: Austin.Hansen
 */

#ifndef SDCARD_SDCARD_H_
#define SDCARD_SDCARD_H_

uint8_t sd_initialize();
uint8_t sd_read(uint8_t *buffer, uint32_t address, uint16_t length);
uint8_t sd_write(uint8_t *buffer, uint32_t address, uint16_t length);

#endif /* SDCARD_SDCARD_H_ */
