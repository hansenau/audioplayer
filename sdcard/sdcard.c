/*
 * sdcard.c
 *
 *  Created on: May 22, 2015
 *      Author: Austin.Hansen
 */


//#include "diskio.h"
#include <stdint.h>
#include <stdbool.h>
#include <machine.h>
#include "sdcard.h"
#include "r_rspi_rx600.h"
#include "platform.h"

// Pack 'em tight!
#pragma pack

// COMMAND struct (holds a standard 6-byte SD SPI command)
typedef struct COMMAND {
	uint8_t command;
	uint32_t argument;
	uint8_t crc;
} COMMAND;

// Macro for initializing a COMMAND struct (argument must be reversed to big-endian)
#define CMD(X,Y,Z) (COMMAND) { 0x40|X, ((Y&0xFF000000)>>24) | \
									   ((Y&0x00FF0000)>>8)  | \
									   ((Y&0x0000FF00)<<8)  | \
									   ((Y&0x000000FF)<<24) , Z }
#define ACMD(X,Y,Z) (COMMAND) { 0x40|X, ((Y&0xFF000000)>>24) | \
									    ((Y&0x00FF0000)>>8)  | \
									    ((Y&0x0000FF00)<<8)  | \
									    ((Y&0x000000FF)<<24) , Z }

void sendCMD(uint8_t command, uint32_t argument, uint8_t crc, uint8_t *response);
void sendACMD(uint8_t command, uint32_t argument, uint8_t crc, uint8_t *response);
void waitForR1(uint8_t channel, uint8_t *response);

static uint8_t response;
static uint32_t timeout;
static uint16_t crc;

uint8_t sd_initialize() {

	// do/while for easy breaking (if something goes wrong)
	do {
		// 0x100000 loops at 96MHz = at least 11ms timeout (very long)
		timeout = 0x100000;

		// To be used for receiving R7 response from CMD8
		uint8_t r7[4];

		// Initialize RSPI channel 1 (if not already initialized)
		R_RSPI_Init(RSPI_CHANNEL_0);

		// Select SD card (pid ignored due to no locking)
		R_RSPI_Select(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);

		// Give the SDC some clocks to figure itself out
		for(timeout=0xFF; timeout; timeout--) R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);

		// Send CMD0 (CRC required)
		sendCMD(0, 0x00000000, 0x95, &response);
		if(response != 0x01) break;

		// Send CMD8 (CRC required)
		sendCMD(8, 0x000001AA, 0x87, &response);

		// In case of error, send no argument with ACMD41 (Card is SD v1)
		if(response != 0x01) {
			sendCMD(55, 0x00000000, 0x00, &response);
			sendACMD(41, 0x00000000, 0x00, &response);

			// In case of further error, send CMD1 instead (Card is MMC V3)
			if(response > 0x01) {
				for(timeout = 0xFFFF; timeout && (response == 0x01); timeout--)
					sendCMD(1, 0x00000000, 0x00, &response);

				// Reject the card if it doesn't take CMD1
				if(response != 0x00) {
					R_RSPI_Deselect(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);
					return response;
				}
			}

			//Set blocksize to 512 bytes
			sendCMD(16, 0x00000200, 0x00, &response);
		}

		// Otherwise, check R7 for 0x1AA match (card is SD v2)
		else {
			R_RSPI_Read(RSPI_CHANNEL_0, r7, 4, 0);

			// If matched, send ACMD41 with argument of 0x40000000
			if((r7[2]==0x01) && (r7[3]==0xAA)) {
				timeout = 0x01;
				for(timeout = 0xFFFF; timeout && (response == 0x01); timeout--) {
					sendCMD(55, 0x00000000, 0x00, &response);
					sendACMD(41, 0x40000000, 0x00, &response);
				}

				// Reject the card if command not accepted
				if(response != 0x00) {
					R_RSPI_Deselect(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);
					return response;
				}

				// Otherwise, check OCR bit 30 to see if SDXC/SDHC
				sendCMD(58, 0x00000000, 0x00, &response);
				R_RSPI_Read(RSPI_CHANNEL_0, r7, 4, 0);

				// If not SDXC/SDHC, force block size to 512 bytes
				if((r7[0]&0x40) != 0x40) {
					sendCMD(16, 0x00000200, 0x00, &response);
				}
			}
		}

	} while(0);

	// Deselect the SD CS line and return the response, whatever it may be (hopefully 0x00)
	R_RSPI_Deselect(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);
	return response;

}

uint8_t sd_read(uint8_t *buffer, uint32_t address, uint16_t length) {

	COMMAND cmd;

	// Select SD CS
	R_RSPI_Select(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);

	switch(length) {
		case 0:
			response = 0;
			break;

		case 1:
			// Write CMD17 (single block read)
			cmd = CMD(17, address+0x10E00, 0);
			R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);

			// Read until timeout or R1 response
			response = 0xFF;
			for(timeout=0; (timeout<0xFFFFF) && (response == 0xFF); timeout++) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}
			if(response != 0x00) break;

			// Read until timeout or data token received
			response = 0xFF;
			for(timeout=0; (timeout<0xFFFF) && (response == 0xFF); timeout++) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}
			if(response != 0xFE) break;

			// Read block and CRC, return success
			R_RSPI_Read(RSPI_CHANNEL_0, buffer, 512, 0);
			R_RSPI_Read(RSPI_CHANNEL_0, &crc, 2, 0);
			
			break;

		// Since length is unsigned, default handles cases >1
		default:
			// Write CMD18
			cmd = CMD(18, address+0x10E00, 0);
			R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);

			// Read until timeout or R1 response
			response = 0xFF;
			for(timeout=0; (timeout<0xFFFFF) && (response == 0xFF); timeout++) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}
			if(response != 0x00) break;

			do {

				// Read until timeout or data token received
				response = 0xFF;
				for(timeout=0; (timeout<0xFFFF) && (response == 0xFF); timeout++) {
					R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
				}
				if(response != 0xFE) break;

				// Read block and CRC, return success
				R_RSPI_Read(RSPI_CHANNEL_0, buffer, 512, 0);
				R_RSPI_Read(RSPI_CHANNEL_0, &crc, 2, 0);

				buffer += 512;
				length--;

			} while(length > 0);

			// Send STOP_TRAN command (CMD12)
			cmd = CMD(12, 0x00000000, 0);
			R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);

			// Receive stuff byte, then wait for R1
			R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			response = 0xFF;
			for(timeout=0; (timeout<0xFFFF) && (response == 0xFF); timeout++) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}
			if(response != 0x00) break;

			// Wait until no longer busy
			while(response != 0xFF) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}

	}

	// Deselect SD CS
	R_RSPI_Deselect(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);

	return response;
}

uint8_t sd_write(uint8_t *buffer, uint32_t address, uint16_t length) {

	COMMAND cmd;
	uint16_t smallcmd;

	// Select SD CS
	R_RSPI_Select(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);

	switch(length) {
		case 0:
			response = 0;
			break;

		case 1:
			// Write CMD17 (single block read)
			sendCMD(17, address, 0, &response);
			if(response != 0x00) break;

			// Send 8 clocks
			R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);

			// Send data token
			smallcmd = 0xFEFE;
			R_RSPI_Write(RSPI_CHANNEL_0, &smallcmd, 1, 0);

			// Send data packet
			R_RSPI_Write(RSPI_CHANNEL_0, buffer, 512, 0);

			// Send CRC
			smallcmd = 0;
			R_RSPI_Write(RSPI_CHANNEL_0, &smallcmd, 2, 0);

			// Receive data response
			R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			if((response&0x0F) != 0x05) break;

			// Wait until no longer busy
			while(response != 0xFF) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}
			
			break;

		// (case length > 1)
		default:
			// Send ACMD23 to specify pre-erase blocks for faster write
			sendCMD(55, 0x00000000, 0x00, &response);
			sendACMD(23, length, 0x00, &response);

			// Send CMD25 (WRITE_MULTIPLE_BLOCK)
			sendCMD(25, address, 0x00, &response);

			do {

				// Send 8 clocks
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);

				// Send data token
				smallcmd = 0xFEFE;
				R_RSPI_Write(RSPI_CHANNEL_0, &smallcmd, 1, 0);

				// Send data packet
				R_RSPI_Write(RSPI_CHANNEL_0, buffer, 512, 0);

				// Send CRC
				smallcmd = 0;
				R_RSPI_Write(RSPI_CHANNEL_0, &smallcmd, 2, 0);

				// Receive data response
				response = 0;
				for(timeout=0; (timeout<0xFFFFF) && (response != 0xFF); timeout++)
					R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);

				if((response&0x0F) != 0x05) break;

				buffer = (uint32_t)buffer + 512;
				length--;

			} while(length > 0);

			// 8-clock delay
			R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);

			// Send STOP_TRAN command (CMD12)
			cmd = CMD(12, 0x00000000, 0);
			R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);

			// Receive stuff byte, then wait until no longer busy
			R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			response = 0;
			while(response != 0xFF) {
				R_RSPI_Read(RSPI_CHANNEL_0, &response, 1, 0);
			}

	}

	// Deselect SD CS
	R_RSPI_Deselect(RSPI_CHANNEL_0, SDMICRO_SELECTED, 0);

	return response;
}

void sendCMD(uint8_t command, uint32_t argument, uint8_t crc, uint8_t *response) {
	COMMAND cmd = CMD(command, argument, crc);
	R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);
	timeout = 0xFFFF;
	*response = 0xFF;
	waitForR1(RSPI_CHANNEL_0, response);
}

void sendACMD(uint8_t command, uint32_t argument, uint8_t crc, uint8_t *response) {
	COMMAND cmd = ACMD(command, argument, crc);
	R_RSPI_Write(RSPI_CHANNEL_0, &cmd, 6, 0);
	timeout = 0xFFFF;
	*response = 0xFF;
	waitForR1(RSPI_CHANNEL_0, response);
}

void waitForR1(uint8_t channel, uint8_t *response) {
	for( ; timeout && (*response == 0xFF); timeout--) R_RSPI_Read(channel, response, 1, 0);
}
