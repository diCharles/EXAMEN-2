#include "I2C.h"
#include "GPIO.h"
#include "Bits.h"

/* For an specific baud rate, the I2C init function, will look up
 * to the SCL value that mathces the given equation:
 *
 * Baud rate =   I2C_clock / (mul x SCL divider)
 *
 * For more information, check the K64 Sub Family Reference Manual.
 */
const uint32_t SCL_divider_look_up_table[64] = { 20, 22, 24, 26, 28, 30, 34, 40,
		28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64, 72, 80, 88, 104, 128, 80,
		96, 112, 128, 144, 160, 192, 240, 160, 192, 224, 256, 288, 320, 384,
		480, 320, 384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152,
		1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840 };

uint8_t ICR_computed_value(uint32_t system_clock, uint32_t baud_rate);

uint8_t ICR_computed_value(uint32_t system_clock, uint32_t baud_rate) {
	uint32_t mul_times_SCL = system_clock / baud_rate;
	uint8_t SCL_indexer = 0;
	bool_t match_flag = FALSE;

	//Return value (ICR) is equal to the most proximate value of SCL
	while (FALSE == match_flag && (SCL_LUT_SIZE - 1 > SCL_indexer)) {
		if (SCL_divider_look_up_table[SCL_indexer] > mul_times_SCL
				|| SCL_divider_look_up_table[SCL_indexer] == mul_times_SCL) {
			//Returns the index of the most proximate value of SCL
			match_flag = TRUE;
			break;
		}
		SCL_indexer += 1;
	}
	return (SCL_indexer);

}

void I2C_init(i2c_channel_t channel, uint32_t system_clock, uint32_t baud_rate) {

	/*Configures pins to work as I2C SCL and SDA*/
	gpio_pin_control_register_t I2C_alternative_2 = GPIO_MUX2;
	gpio_pin_control_register_t I2C_alternative_5 = GPIO_MUX5;

	switch (channel) {
	case I2C_0:

		SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;

		GPIO_clock_gating(GPIO_B);
		/** PTB2 works as SCL for I2C0*/
		GPIO_pin_control_register(GPIO_B, bit_2, &I2C_alternative_2);
		/** PTB3 works as SDA for I2C0*/
		GPIO_pin_control_register(GPIO_B, bit_3, &I2C_alternative_2);
		/*Writing in Frequency divider register*/
		I2C0->F = I2C_F_ICR(
				ICR_computed_value(system_clock,
						baud_rate)) | I2C_F_MULT(MUL_VALUE);
		/*Enabling I2C module and its interrupts. */
		I2C0->C1 = I2C_C1_IICIE_MASK | I2C_C1_IICEN_MASK;

		break;

	case I2C_1:
		/** WARNING: PTC 10 (SCL) and PTC11 are not connected in freedom K64 Board*/
		SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;

		GPIO_clock_gating(GPIO_C);
		/**PTC10 works as SCL for I2C1*/
		GPIO_pin_control_register(GPIO_C, bit_10, &I2C_alternative_2);
		/**PTC11 works as SDA for I2C0*/
		GPIO_pin_control_register(GPIO_C, bit_11, &I2C_alternative_2);

		I2C1->F = I2C_F_ICR(
				ICR_computed_value(system_clock,
						baud_rate)) | I2C_F_MULT(MUL_VALUE);
		/*Enabling I2C module and its interrupts.*/
		I2C1->C1 = I2C_C1_IICIE_MASK | I2C_C1_IICEN_MASK;

		break;

	case I2C_2:
		SIM->SCGC4 |= SIM_SCGC1_I2C2_MASK;

		GPIO_clock_gating(GPIO_A);
		/*PTA12 works as SCL for I2C1*/
		GPIO_pin_control_register(GPIO_C, bit_12, &I2C_alternative_5);
		/*PTA13 works as SDA for I2C0*/
		GPIO_pin_control_register(GPIO_C, bit_13, &I2C_alternative_5);

		I2C2->F = I2C_F_ICR(
				ICR_computed_value(system_clock,
						baud_rate)) | I2C_F_MULT(MUL_VALUE);
		/*Enabling I2C module and its interrupts. */
		I2C2->C1 = I2C_C1_IICIE_MASK | I2C_C1_IICEN_MASK | I2C_C1_TXAK_MASK;
		break;

	default:
		return;
		break;
		//Do nothing
	}

}

uint8_t I2C_busy(i2c_channel_t channel) {
	/* when start appears the bit  busy is set, when stop appears the bit is cleared*/
	switch (channel) {
	case I2C_0:/*************************************************/
		if (FALSE == ( I2C_S_BUSY_MASK & I2C0->S)) {
			return (TRUE);
		}

		else {
			return (TRUE);
		}
		break;

	case I2C_1:/*************************************************/
		if (FALSE == ( I2C_S_BUSY_MASK & I2C1->S)) {
			return (TRUE);
		}

		else {
			return (TRUE);
		}
		break;

	case I2C_2:/*************************************************/
		if (FALSE == ( I2C_S_BUSY_MASK & I2C2->S)) {
			return (TRUE);
		}

		else {
			return (TRUE);
		}
		break;/*************************************************/

	default:
		break;
	}
	return (FALSE); /* this return must never be reached*/
}

void I2C_mst_or_slv_mode(i2c_channel_t channel, uint8_t mst_or_slv) {
	switch (channel) {
	case I2C_0:
		if (SLAVE_MODE == mst_or_slv) {
			I2C0->C1 &= ~(I2C_C1_MST_MASK);
		}

		else if (MASTER_MODE == mst_or_slv) {
			I2C0->C1 |= (I2C_C1_MST_MASK);
		}
		break;

	case I2C_1:
		if (SLAVE_MODE == mst_or_slv) {
			I2C1->C1 &= ~(I2C_C1_MST_MASK);
		} else if (MASTER_MODE == mst_or_slv) {
			I2C1->C1 |= (I2C_C1_MST_MASK);
		}
		break;

	case I2C_2:
		if (SLAVE_MODE == mst_or_slv) {
			I2C2->C1 &= ~(I2C_C1_MST_MASK);
		} else if (MASTER_MODE == mst_or_slv) {
			I2C2->C1 |= (I2C_C1_MST_MASK);
		}
		break;
	}
}

void I2C_tx_rx_mode(i2c_channel_t channel, uint8_t tx_or_rx) {
	/* Configure TX flag in register I2Cx_C1*/

	switch (channel) {
	case I2C_0:/*************************************************/
		if (TRUE == tx_or_rx) {
			/*transmitter mode*/
			I2C0->C1 |= I2C_C1_TX_MASK;
		} else {
			/* receiver mode*/
			I2C0->C1 &= ~(I2C_C1_TX_MASK);
		}
		break;

	case I2C_1:/*************************************************/
		if (TRUE == tx_or_rx) {
			/*transmitter mode*/
			I2C0->C1 |= I2C_C1_TX_MASK;
		} else {
			/* receiver mode*/
			I2C0->C1 &= ~(I2C_C1_TX_MASK);
		}

	case I2C_2:/*************************************************/
		if (TRUE == tx_or_rx) {
			/*transmitter mode*/
			I2C0->C2 |= I2C_C1_TX_MASK;
		} else {
			/* receiver mode*/
			I2C0->C2 &= ~(I2C_C1_TX_MASK);
		}
		break;/*************************************************/

	default:
		break;
	}

}

void I2C_nack(void) {
	/* If the master does not acknowledge the slave transmitter after a data
	 * transmission, the slave interprets it as an end of  data transfer and
	 * release the SDA line.
	 *
	 * In other words. The master gives a NACK to the slave meaning that the
	 * data read is complete.
	 */
	I2C0->C1 |= I2C_C1_TXAK(NACK_SIGNAL);


}

void I2C_repeated_start(i2c_channel_t channel) {
	/*Repeat START
	 Writing 1 to this bit generates a repeated START condition provided it is the current master. This bit will
	 always be read as 0. Attempting a repeat at the wrong time results in loss of arbitration.*/

	switch (channel) {
	case I2C_0:
		I2C0->C1 |= I2C_C1_RSTA_MASK;
		break;
	case I2C_1:
		I2C1->C1 |= I2C_C1_RSTA_MASK;
		break;
	case I2C_2:
		I2C2->C1 |= I2C_C1_RSTA_MASK;
		break;
	}
}

void I2C_write_byte(uint8_t data) {
	/*
	 I2C0->C1 |=   (I2C_C1_MST_MASK) ;
	 I2C0->C1 |= I2C_C1_MST(1);
	 I2C_tx_rx_mode(I2C_0,  TRANSMITION_MODE);
	 I2C0->C1 |= I2C_C1_TX(1);
	 */

	/*TODO: modify API for specify wich I2C module of the board is in charge of writing the byte*/
	I2C0->D = data;
}

uint8_t I2C_read_byte(i2c_channel_t channel) {
	/* if the I2C module is configured for master transmit but a master
	 receive is desired, reading the Data register does not initiate the receive. */
	I2C_tx_rx_mode(channel, RECEIVER_MODE); /* MASTER CONFIGURED TO RECEIVE*/
	switch (channel) {
	case I2C_0:
		return ( I2C0->D);
		break;
	case I2C_1:
		return ( I2C1->D);
		break;
	case I2C_2:
		return ( I2C2->D);
		break;
	}
	return 0; /* this return must never be reached*/
}

void I2C_wait(void) {
	/* While a transmittion is not complete, wait*/
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0)
		;
	/*Slave transmit, master reading from slave */
	/*Clear interrupt flag*/
	I2C0->S |= I2C_S_SRW(TRUE);

}

uint8_t I2C_get_ack(i2c_channel_t channel) {
	/* RETURNS 1 NO ACK RECEIVED ( NO_ACK_RECEIVED),
	 * RETURNS 0 ACK RECEIVED  ( ACK_RECEIVED )*/
	/*
	 Receive Acknowledgev bit on I2Cx_S regaister
	 0 Acknowledge signal was received after the completion of one byte of data transmission on the bus
	 1 No acknowledge signal detected
	 */
	switch (channel) {
	case I2C_0:
		if (ACK_RECEIVED == ( I2C_S_RXAK_MASK & I2C0->S)) {
			return ACK_RECEIVED;
		} else {
			return NO_ACK_RECEIVED;
		}
		break;
	case I2C_1:
		if (ACK_RECEIVED == ( I2C_S_RXAK_MASK & I2C1->S)) {
			return ACK_RECEIVED;
		} else {
			return NO_ACK_RECEIVED;
		}
		break;
	case I2C_2:
		if (ACK_RECEIVED == ( I2C_S_RXAK_MASK & I2C2->S)) {
			return ACK_RECEIVED;
		} else {
			return NO_ACK_RECEIVED;
		}
		break;
	}
	return 0; /* this return must never be reached*/

}
void I2C_start(i2c_channel_t channel) {
	/* also sets master mode   */
	/* if MST bit was zero the start w*/
	switch (channel) {
	case I2C_0:
		I2C0->C1 |= (I2C_C1_MST_MASK);
		break;
	case I2C_1:
		I2C1->C1 |= (I2C_C1_MST_MASK);
		break;
	case I2C_2:
		I2C2->C1 |= (I2C_C1_MST_MASK);
		break;
	}
}

void I2C_stop(i2c_channel_t channel) {
	/*       This function Generates the stop signal. When this bit changes from 1 to 0, a STOP signal is generated
	 and the mode of operation changes from master to slave. Also, inside the function the I2C is
	 changed to receiver mode  		*/

	/*  operation on I2Cx module changed to slave*/
	/* Bit 5 MST Master Mode Select
	 When MST is changed from 0 to 1, a START signal is generated on the bus and master mode is selected.
	 When this bit changes from 1 to 0, a STOP signal is generated and the mode of operation changes from
	 master to slave.
	 0 Slave mode
	 1 Master mode*/
	switch (channel) {
	case I2C_0:
		I2C0->C1 &= ~(I2C_C1_MST_MASK);
		break;
	case I2C_1:
		I2C1->C1 &= ~(I2C_C1_MST_MASK);
		break;
	case I2C_2:
		I2C2->C1 &= ~(I2C_C1_MST_MASK);
		break;
	}

	/* i2c changed to receiver mode */
	I2C_tx_rx_mode(channel, RECEIVER_MODE);
}
