/**
 * Device control of 24LC256 Serial EEPROM
 * @author Rubén Charles Suárez
 * @date 19/04/2019
 */
#include "EEPROM_24LC256.h"



/**
 * brief Uses a pointer to structure to configure I2C of board,
 * and address of EEPROM.
 * @param EEPROM_24LC256     Used to set driver of the memory.
 * @param chip_select_3_bits Used to generate the address of the memory used
 */
void EEPROM_24LC256_init(EEPROM_24LC256_t * EEPROM_24LC256, uint8_t chip_select_3_bits)
{
	 /** Set I2C channel, clock rate (10.5 MHz expected and baud rate of I2C */
	 I2C_init(EEPROM_24LC256->I2C_channel, EEPROM_24LC256->system_bus_clock, EEPROM_24LC256->I2C_baud_rate);
	 /** There are 3 available bits for chip select, meaning that 8 the board can handle 8 EEPROMs*/
	 EEPROM_24LC256->EEPROM_address |= chip_select_3_bits<< 1;
}

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
 *
 */
EEPROM_WRITE_status_t EEPROM_24LC256_byte_WRITE(EEPROM_24LC256_t * EEPROM , uint16_t address, uint8_t data_in)
{
	delay(I2C_DELAY);
	I2C_tx_rx_mode(EEPROM->I2C_channel,  TRANSMITION_MODE); /* Enabling transmission mode of I2C */
	/** Following sequence of random read from 24LC256's datasheet. */


	if( BUSY == I2C_busy( EEPROM->I2C_channel) )
	{
		/** If the I2C bus is busy, the transmission is aborted*/
		return BUS_IS_BUSY_WRITE;
	}

	I2C_start(EEPROM->I2C_channel);

	/** Send CONTROL BYTE with write bit */
	I2C_write_byte( (EEPROM->EEPROM_address) & WRITE_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges CONTROL BYTE. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending address HIGH byte */
	I2C_write_byte( ( address & ADDRESS_HIGH_BYTE_MASK ) >> SHIFT_ADDRESS_HIGH_LOW);
	/** Wait until the byte is transmitted. */
	I2C_wait();


	/** Checking if EEPROM acknowledges address HIGH byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending address LOW byte */
	I2C_write_byte( (uint8_t)( address & ADDRESS_LOW_BYTE_MASK ));
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges address LOW byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending byte to write in EEPROM */
	I2C_write_byte( data_in );
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges byte to be written. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	I2C_stop(I2C_0);
	delay(I2C_DELAY);

	return ( WRITE_SUCCESS );
}

/**
 * brief	Write a given number of bytes contained in array of uin8_t type into the EEPROM.
 *
 * 			NOTE: If more than 64 data writes in EEPROM are performed, the 65th will be allocated in
 * 			the start address indicated as a parameter to the function. For more information,
 * 			check device's datasheet.
 *
 * @param EEPROM  		    		Pointer to structure used to get address of EEPROM to WRITE.
 * @param start_address 			Direction of the first byte to write into memory
 * @param data_IN_array				Array with Data to be written.
 * @param size_of_data_IN_array		Size of array, indicates the number of spaces to write into EEPROM.
 * @return 							ERROR: when information could'nt be  written into memory (NACK) or the memory specified where not found.
 * 		   							SUCCESS: When WRITE operation in memory is achieved.
 * 		   							BUS_IS_BUSY_X: another device is using I2C bus
 *
 */
EEPROM_WRITE_status_t EEPROM_24LC256_page_WRITE_with_array(EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint8_t  * data_IN_array, uint16_t size_of_data_IN_array)
{
	delay(I2C_DELAY);
	I2C_tx_rx_mode(EEPROM->I2C_channel,  TRANSMITION_MODE); /* Enabling transmission mode of I2C */
	/** Following sequence of page write from 24LC256's datasheet */


	//if( BUSY == I2C_busy( EEPROM->I2C_channel) )
	//{
		/** If the I2C bus is busy, the transmission is aborted*/
		//return BUS_IS_BUSY_WRITE;
	//}


	I2C_start(EEPROM->I2C_channel);

	/** Send CONTROL byte with write bit */
	I2C_write_byte( (EEPROM->EEPROM_address) & WRITE_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges CONTROL BYTE. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending START address to write in memory
	 *  The data of the next bit sent here will be stored in the address loaded here.
	 *  The following byte of data sent will be stored in the next position of memory.
	 *  And so on, until the master (board) sends an stop bit to indicate the EEPROM the end of writing.
	 */

	I2C_write_byte( ( start_address & ADDRESS_HIGH_BYTE_MASK ) >> SHIFT_ADDRESS_HIGH_LOW);
	/** Wait until the byte is transmitted. */
	I2C_wait();


	/** Checking if EEPROM acknowledges address HIGH byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending START address LOW byte */
	I2C_write_byte( (uint8_t)( start_address & ADDRESS_LOW_BYTE_MASK ));
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges START address LOW byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Used for write in memory the bytes allocated in the array passed to this function as a parameter*/
	uint8_t memory_writer_index;

	for ( memory_writer_index = 0; memory_writer_index < size_of_data_IN_array ; memory_writer_index++ )
	{
		I2C_write_byte( data_IN_array[memory_writer_index]  );
		/** Wait until the byte is transmitted. */
		I2C_wait();

		/** Checking if EEPROM acknowledges start_address byte. */
		if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
		{
			return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
		}
	}

	I2C_stop(I2C_0);
	delay(I2C_PROGRAMMING_DELAY);

	return (WRITE_SUCCESS);

}

/**
 * brief	Write a given number of bytes from a FIFO into the EEPROM
 *
 * @param EEPROM  		    	Pointer to structure used to get address of EEPROM to WRITE.
 * @param start_address			Direction of the first byte to write into memory
 * @param num_spaces_to_WRITE	Also Indicates how many times the FIFO is going to be used.
 * @param queue_p               Pointer that uses a FIFO to get the bytes to write.
 *
 * @return						ERROR: when information could'nt be  written into memory (NACK) or the memory specified where not found.
 * 		   						SUCCESS: When WRITE operation in memory is achieved.
 * 		   						BUS_IS_BUSY_X: another device is using I2C bus
 *
 */
EEPROM_WRITE_status_t EEPROM_24LC256_page_WRITE_with_FIFO(EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint16_t num_spaces_to_WRITE, queue_t * queue_p)
{
	delay(I2C_DELAY);
	I2C_tx_rx_mode(EEPROM->I2C_channel,  TRANSMITION_MODE); /* Enabling transmission mode of I2C */
	/** Following sequence of page write from 24LC256's datasheet */


	if( BUSY == I2C_busy( EEPROM->I2C_channel) )
	{
		/** If the I2C bus is busy, the transmission is aborted*/
		return BUS_IS_BUSY_WRITE;
	}

	I2C_start(EEPROM->I2C_channel);

	/** Send CONTROL byte with write bit */
	I2C_write_byte( (EEPROM->EEPROM_address) & WRITE_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges its address. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending START address to write in memory
	 *  The data of the next bit sent here will be stored in the address loaded here.
	 *  The following byte of data sent will be stored in the next position of memory.
	 *  And so on, until the master (board) sends an stop bit to indicate the EEPROM the end of writing.
	 */

	I2C_write_byte( ( start_address & ADDRESS_HIGH_BYTE_MASK ) >> SHIFT_ADDRESS_HIGH_LOW);
	/** Wait until the byte is transmitted. */
	I2C_wait();


	/** Checking if EEPROM acknowledges START address HIGH byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}



	/** Sending START address LOW byte */
	I2C_write_byte( (uint8_t)( start_address & ADDRESS_LOW_BYTE_MASK ));
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges START address LOW byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Used for write in memory the bytes allocated in the array passed to this function as a parameter*/
	uint8_t memory_writer_index;


	for ( memory_writer_index = 0; memory_writer_index < num_spaces_to_WRITE ; memory_writer_index++ )
	{
		I2C_write_byte( dequeue(queue_p));
		/** Wait until the byte is transmitted. */
		I2C_wait();

		/** Checking if EEPROM acknowledges start_address byte. */
		if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
		{
			return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
		}
	}

	I2C_stop(I2C_0);
	delay(I2C_PROGRAMMING_DELAY);

	return (WRITE_SUCCESS);
}



/****************************************************************************************************************************/
/************************ READ functions ***********************************************************************************/
/****************************************************************************************************************************/

/**
 * brief	Read a byte from memory with the address pointed by its internal address pointer.
 *
 * @param EEPROM		Pointer to structure used to get address of EEPROM to READ.
 * @return			    Pointer to structure with byte read and status of READ operation
 */
struct_EEPROM_READ_status_t  * EEPROM_24LC256_current_address_READ ( EEPROM_24LC256_t * EEPROM )
{
	/** Creating pointer to return byte read and status of function termination */
	struct_EEPROM_READ_status_t return_struct ={READ_ERROR,READ_ERROR};
	struct_EEPROM_READ_status_t * return_pointer = &return_struct;

	delay(I2C_DELAY);
	I2C_tx_rx_mode(EEPROM->I2C_channel,  TRANSMITION_MODE); /* Enabling transmission mode of I2C */

	/** Following sequence of page current address read from 24LC256's datasheet */

	//if( BUSY == I2C_busy( EEPROM->I2C_channel) )
	//{
		/** If the I2C bus is busy, the transmission is aborted*/
		//return_pointer->EEPROM_READ_return_status = BUS_IS_BUSY_READ;
		//return_pointer->EEPROM_byte_read = BUS_IS_BUSY_READ;
		//return  ( return_pointer );
	//}

	I2C_start(EEPROM->I2C_channel);

	/** Send CONTROL byte with READ bit */
	I2C_write_byte( (EEPROM->EEPROM_address) | READ_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges CONTROL BYTE. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return_pointer->EEPROM_READ_return_status = READ_ERROR;
		return_pointer->EEPROM_byte_read = READ_ERROR;
		return ( return_pointer ); /* No acknowledge received. EEPROM is not responding. */
	}

	/** Changing I2C module of board to reception mode to read data from EEPROM */
	I2C_tx_rx_mode( EEPROM->I2C_channel,  RECEIVER_MODE);

	/** Indicating EEPROM slave end of read*/
	I2C_nack();
	/** Dummy read */
	return_pointer->EEPROM_byte_read =  I2C_read_byte( EEPROM->I2C_channel);
	/** Wait for data to be transmitted */
	I2C_wait();

	I2C_stop(EEPROM->I2C_channel);

	return_pointer->EEPROM_byte_read =  I2C_read_byte( EEPROM->I2C_channel);
	return_pointer->EEPROM_READ_return_status = READ_SUCCESS;
	delay(I2C_DELAY);


	return (return_pointer);
}

/**
 * brief	Read a byte from memory with the address given.
 *
 * @param EEPROM	Pointer to structure used to get address of EEPROM to READ.
 * @param address	Direction of byte to read.
 * @return			Pointer to structure with byte read and status of READ operation.
 */
struct_EEPROM_READ_status_t  * EEPROM_24LC256_random_READ ( EEPROM_24LC256_t * EEPROM, uint16_t address )
{
	/** Creating pointer to return byte read and status of function termination */
	struct_EEPROM_READ_status_t return_struct = {READ_ERROR,READ_ERROR};
	struct_EEPROM_READ_status_t * return_pointer = &return_struct;

	delay(I2C_DELAY);
	I2C_tx_rx_mode(EEPROM->I2C_channel,  TRANSMITION_MODE); /* Enabling transmission mode of I2C */

	/** Following sequence of page current address read from 24LC256's datasheet */

	//if( BUSY == I2C_busy( EEPROM->I2C_channel) )
	//{
		/** If the I2C bus is busy, the transmission is aborted*/
	//	return_pointer->EEPROM_READ_return_status = BUS_IS_BUSY_READ;
		//return_pointer->EEPROM_byte_read = BUS_IS_BUSY_READ;
		//return  ( return_pointer );
	//}

	I2C_start(EEPROM->I2C_channel);

	/** Send CONTROL byte with WRITE bit */
	I2C_write_byte( (EEPROM->EEPROM_address) & WRITE_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges CONTROL BYTE . */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return_pointer->EEPROM_READ_return_status = READ_ERROR;
		return_pointer->EEPROM_byte_read = READ_ERROR;
		return ( return_pointer ); /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending address HIGH byte */
	I2C_write_byte( ( address & ADDRESS_HIGH_BYTE_MASK ) >> SHIFT_ADDRESS_HIGH_LOW);
	/** Wait until the byte is transmitted. */
	I2C_wait();


	/** Checking if EEPROM acknowledges address HIGH byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	/** Sending address LOW byte */
	I2C_write_byte( (uint8_t)( address & ADDRESS_LOW_BYTE_MASK ));
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges address LOW byte. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return WRITE_ERROR; /* No acknowledge received. EEPROM is not responding. */
	}

	I2C_repeated_start(EEPROM->I2C_channel);

	/** RE-send CONTROL byte with READ bit */
	I2C_write_byte( (EEPROM->EEPROM_address) | READ_SLAVE_MASK);
	/** Wait until the byte is transmitted. */
	I2C_wait();

	/** Checking if EEPROM acknowledges CONTROL BYTE. */
	if ( NACK == I2C_get_ack(EEPROM->I2C_channel) )
	{
		return_pointer->EEPROM_READ_return_status = READ_ERROR;
		return_pointer->EEPROM_byte_read = READ_ERROR;
		return ( return_pointer ); /* No acknowledge received. EEPROM is not responding. */
	}


	/** Changing I2C module of board to reception mode to read data from EEPROM */
	I2C_tx_rx_mode( EEPROM->I2C_channel,  RECEIVER_MODE);

	/** Indicating EEPROM slave end of read*/
	I2C_nack();
	/** Dummy read */
	uint8_t aux_for_dummy_READ =  I2C_read_byte( EEPROM->I2C_channel);
	/** Wait for data to be transmitted */
	I2C_wait();

	I2C_stop(EEPROM->I2C_channel);

	return_pointer->EEPROM_byte_read =  I2C_read_byte( EEPROM->I2C_channel);
	return_pointer->EEPROM_READ_return_status = READ_SUCCESS;
	delay(I2C_DELAY);

	return (return_pointer);
}

/**
 * brief	Store a given number of bytes read from EEPROM into an array.
 *
 * @param EEPROM	   				 Pointer to structure used to get address of EEPROM to READ.
 * @param start_address				 Direction of the first byte to be read from the memory.
 * @param data_OUT_array			 Array to allocate the bytes read
 * @param size_of_data_OUT_array	 Indicates how many reads need to be perform
 * @return							 Indicates status of READ operation.
 */
EEPROM_READ_status_t EEPROM_24LC256_sequential_READ_with_array ( EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint8_t * data_OUT_array, uint8_t size_of_data_OUT_array )
{
	/** Store the byte read in the first address
	 *
	 *  Then, use current read to store the following bytes until the given number of operations is reached.
	 *
	 *  This emulates the functionality of sequential read of EEPROM specified in 24CL256 datasheet.
	 * */
	/** Creating structure and pointer to use function of of random and current read address */
	struct_EEPROM_READ_status_t aux_current_read_struct;
	struct_EEPROM_READ_status_t * aux_read_p = &aux_current_read_struct;

	/** First byte to be allocated in  first position of array */
	aux_read_p = EEPROM_24LC256_random_READ ( EEPROM, start_address );

	/** Evaluating if read operation is successful */
	if( READ_SUCCESS != aux_read_p->EEPROM_READ_return_status)
	{
		return ( aux_read_p->EEPROM_READ_return_status ); /** Operation of read not performed.*/
	}

	data_OUT_array[0] = aux_read_p->EEPROM_byte_read;

	uint16_t read_counter;
	/** The next bytes to read are indicated in address by EEPROM's internal address pointer */
	for ( read_counter = 1 ; read_counter < size_of_data_OUT_array   ; read_counter ++)
	{
		aux_read_p = EEPROM_24LC256_current_address_READ ( EEPROM );

		/** Evaluating if read operation is successful */
		if( READ_SUCCESS != aux_read_p->EEPROM_READ_return_status)
		{
			return ( aux_read_p->EEPROM_READ_return_status ); /** Operation of read not performed.*/
		}

		data_OUT_array[read_counter] = aux_read_p->EEPROM_byte_read;
	}

	/** Sequential write is completed successfully */
	return ( READ_SUCCESS );



}
/**
 * brief    Store a given number of bytes read from EEPROM into a FIFO
 *
 * @param EEPROM				 Pointer to structure used to get address of EEPROM to READ.
 * @param start_address			 Direction of the first byte to be read from the memory.
 * @param num_spaces_to_READ     Indicates how many bytes of memory will be read and loaded into the FIFO.
 * @param fifo_enqueue_function  Pointer to use FIFO provided by user.
 * @return					     Indicates status of READ operation.
 */
EEPROM_READ_status_t EEPROM_24LC256_sequential_READ_with_FIFO ( EEPROM_24LC256_t * EEPROM , uint16_t start_address, uint16_t num_spaces_to_READ,  queue_t * queue )
{
	/** Store the byte read in the first address
		 *
		 *  Then, use current read to store the following bytes until the given number of operations is reached.
		 *
		 *  This emulates the functionality of sequential read of EEPROM specified in 24CL256 datasheet.
		 * */
		/** Creating structure and pointer to use function of of random and current read address */
		struct_EEPROM_READ_status_t aux_current_read_struct;
		struct_EEPROM_READ_status_t * aux_read_p = &aux_current_read_struct;

		/** First byte to be allocated in  first position of array */
		aux_read_p = EEPROM_24LC256_random_READ ( EEPROM, start_address );

		/** Evaluating if read operation is successful */
		if( READ_SUCCESS != aux_read_p->EEPROM_READ_return_status)
		{
			return ( aux_read_p->EEPROM_READ_return_status ); /** Operation of read not performed.*/
		}

		enqueue(queue, aux_read_p->EEPROM_byte_read);

		uint16_t read_counter;
		/** The next bytes to read are indicated in address by EEPROM's internal address pointer */
		for ( read_counter = 1 ; read_counter < num_spaces_to_READ   ; read_counter ++)
		{
			aux_read_p = EEPROM_24LC256_current_address_READ ( EEPROM );

			/** Evaluating if read operation is successful */
			if( READ_SUCCESS != aux_read_p->EEPROM_READ_return_status)
			{
				return ( aux_read_p->EEPROM_READ_return_status ); /** Operation of read not performed.*/
			}

			enqueue(queue, aux_read_p->EEPROM_byte_read );
		}

		/** Sequential write is completed successfully */
		return ( READ_SUCCESS );


}






