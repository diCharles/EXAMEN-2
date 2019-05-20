/*
 * EEPROM_24LC256.h
 *
 *  Created on: 19/04/2019
 *      Author: José Rodrigo
 */

#ifndef EEPROM_24LC256_H_
#define EEPROM_24LC256_H_

#include "EEPROM_24LC256.h"
#include "MK64F12.h"
#include "I2C.h"
#include "Delay.h"
#include "Queue.h"


/** 24LC256 address (control code) */
#define EEPROM_ADDRESS 0xA0
/** Delay used between I2C reads and writes */
#define I2C_DELAY 400
/** Delay intended for let EEPROM store/read information (PROGRAMMING) */
/** Contains all aspects to consider for initialization of EEPROM */
#define I2C_PROGRAMMING_DELAY 500
typedef struct
{
	const i2c_channel_t I2C_channel; /** Uses specified module of I2C from K64F*/
	const uint32_t I2C_baud_rate;    /** Bits per second of I2C communication */
	const uint32_t system_bus_clock; /** Bus clock of K64F, expected: 10.5MHz */
	      uint8_t EEPROM_address;    /** Address of EEPROM used.*/
}EEPROM_24LC256_t;

/** MASK of write bit sent after an address in I2C frame.  */
#define WRITE_SLAVE_MASK  0xFE
/** MASK of write bit sent after an address in I2C frame.  */
#define READ_SLAVE_MASK   0x01

/** Shift for get address HIGH/LOW byte for EEPROM read/write operation */
#define SHIFT_ADDRESS_HIGH_LOW 8
/** MASK for get address HIGH byte for EEPROM read/write operation */
#define ADDRESS_HIGH_BYTE_MASK 0xFF00
/** MASK for get address LOW byte for EEPROM read/write operation */
#define ADDRESS_LOW_BYTE_MASK 0x00FF

/** Used as condition to know when the I2C bus is IDLE or busy */
typedef enum{IDLE_EEPROM, BUSY} I2C_bus_state_t;
/** Used as condition to know when the memory acknowledges or not a byte transmitted */
typedef enum{ACK, NACK} I2C_ACK_t;
/** Return value of EEPROM WRITE functions */
typedef enum{WRITE_ERROR, WRITE_SUCCESS, BUS_IS_BUSY_WRITE} EEPROM_WRITE_status_t;

/** Attribute of EEPROM READ return */
typedef enum{ READ_SUCCESS,READ_ERROR, BUS_IS_BUSY_READ} EEPROM_READ_status_t;
/** For initialize a pointer as NULL */
//typedef enum{NULL} pointer_null_t;
/** Return value for EEPROM READ functions */


typedef struct
{
	EEPROM_READ_status_t EEPROM_READ_return_status;
	uint8_t EEPROM_byte_read;

} struct_EEPROM_READ_status_t;

/**
 * Uses a pointer to structure to configure I2C of board,
 * and address of EEPROM.
 * @param EEPROM_24LC256     Used to set driver of the memory.
 * @param chip_select_3_bits Used to generate the address of the memory used
 */
void EEPROM_24LC256_init(EEPROM_24LC256_t * EEPROM_24LC256, uint8_t chip_select_3_bits);

/****************************************************************************************************************************/
/************************ WRITE functions ***********************************************************************************/
/****************************************************************************************************************************/

/**
 * brief	Write one byte of data into given address of EEPROM.
 *
 * @param  EEPROM  	Pointer to structure used to get address of EEPROM to WRITE.
 * @param  address 	Range expected: 0 to 2^15 -1 (32767)
 * @param  data_in 	Data to be written into memory
 * @return          ERROR: when information could'nt be  written into memory (NACK) or the memory specified where not found.
 * 		            SUCCESS: When WRITE operation in memory is achieved.
 * 		            BUS_IS_BUSY_X: another device is using I2C bus
 *
 */
EEPROM_WRITE_status_t EEPROM_24LC256_byte_WRITE(EEPROM_24LC256_t * EEPROM , uint16_t address, uint8_t data_in);

/**
 * brief	Write a given number of bytes contained in array of uin8_t type into the EEPROM.
 *
 * @param EEPROM  		    		Pointer to structure used to get address of EEPROM to WRITE.
 * @param start_address 			Direction of the first byte to write into memory
 * @param data_IN_array				Array with Data to be written.
 * @param size_of_data_IN_array		Size of array, indicates the number of spaces to write into EEPROM.
 * @return 							ERROR: when information could'nt be  written into memory (NACK) or the memory specified where not found.
 * 		   							SUCCESS: When WRITE operation in memory is achieved.
 * 		   							BUS_IS_BUSY_X: another device is using I2C bus
 */
EEPROM_WRITE_status_t EEPROM_24LC256_page_WRITE_with_array(EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint8_t  * data_IN_array, uint16_t size_of_data_IN_array);

/**
 * brief	Write a given number of bytes from a FIFO into the EEPROM
 *
 * @param EEPROM  		    	Pointer to structure used to get address of EEPROM to WRITE.
 * @param start_address			Direction of the first byte to write into memory
 * @param num_spaces_to_WRITE	Also Indicates how many times the FIFO is going to be used.
 * @param queue_p                Pointer that uses a FIFO to get the bytes to write.
 * @return						ERROR: when information could'nt be  written into memory (NACK) or the memory specified where not found.
 * 		   						SUCCESS: When WRITE operation in memory is achieved.
 * 		   						BUS_IS_BUSY_X: another device is using I2C bus
 */
EEPROM_WRITE_status_t EEPROM_24LC256_page_WRITE_with_FIFO(EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint16_t num_spaces_to_WRITE, queue_t * queue_p);



/****************************************************************************************************************************/
/************************ READ *functions ***********************************************************************************/
/****************************************************************************************************************************/

/**
 * brief	Read a byte from memory with the address pointed by its internal address pointer.
 *
 * @param EEPROM		Pointer to structure used to get address of EEPROM to READ.
 * @return				Pointer to structure with byte read and status of READ operation
 */
struct_EEPROM_READ_status_t  * EEPROM_24LC256_current_address_READ ( EEPROM_24LC256_t * EEPROM );

/**
 * brief	Read a byte from memory with the add.
 *
 * @param EEPROM	Pointer to structure used to get address of EEPROM to READ.
 * @param address	Direction of byte to read.
 * @return			Pointer to structure with byte read and status of READ operation.
 */
struct_EEPROM_READ_status_t  * EEPROM_24LC256_random_READ ( EEPROM_24LC256_t * EEPROM, uint16_t address );

/**
 * brief	Store a given number of bytes read from EEPROM into an array.
 *
 * @param EEPROM	   				 Pointer to structure used to get address of EEPROM to READ.
 * @param start_address				 Direction of the first byte to be read from the memory.
 * @param data_OUT_array			 Array to allocate the bytes read
 * @param size_of_data_OUT_array	 Indicates how many reads need to be perform
 * @return							 Indicates status of READ operation.
 */
EEPROM_READ_status_t EEPROM_24LC256_sequential_READ_with_array ( EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint8_t * data_OUT_array, uint8_t size_of_data_OUT_array );

/**
 * brief Store a given number of bytes read from EEPROM into a FIFO
 *
 * @param EEPROM				 Pointer to structure used to get address of EEPROM to READ.
 * @param start_address			 Direction of the first byte to be read from the memory.
 * @param num_spaces_to_READ     Indicates how many bytes of memory will be read and loaded into the FIFO.
 * @param queue   		     Pointer to use FIFO provided by user.
 * @return					     Indicates status of READ operation.
 */
EEPROM_READ_status_t EEPROM_24LC256_sequential_READ_with_FIFO ( EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint16_t num_spaces_to_READ, queue_t * queue);




#endif /* EEPROM_24LC256_H_ */
