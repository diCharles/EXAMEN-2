/*
 * UART.c
 *
 *  Created on: 06/04/2019
 *      Author: Diego  Charles and Ruben Charles
 *    brief:  implementation driver for UART module
 */

#include "UART.h"
//global g_uart0_mail_box
uart_mail_box_t g_mail_box_uart_0  = {0,0};
uart_mail_box_t g_mail_box_uart_1  = {0,0};
uart_mail_box_t g_mail_box_uart_2  = {0,0};
uart_mail_box_t g_mail_box_uart_3  = {0,0};
uart_mail_box_t g_mail_box_uart_4  = {0,0};
uart_mail_box_t g_mail_box_uart_5  = {0,0};


void UART_init(uart_channel_t uart_channel, uint32_t system_clk , uart_baud_rate_t baud_rate)
{
	/* UART baud rate = UART module clock / (16 × (SBR[12:0] + BRFD))*/
	float float_SBR= (float)system_clk/ (16*baud_rate);
	float BFRD=  float_SBR -(float)((uint32_t)(float_SBR));
	uint32_t SBR=(uint32_t)(float_SBR);
	uint32_t BFRA= (BFRD*32) ;
	SBR = system_clk / (16* (uint32_t)baud_rate);
	/* initializing selected uart channel with her baud rate*/
	switch(uart_channel)
	{
	case  UART_0 :
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK ;/* enabling clock*/
		UART0->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART0->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART0->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART0->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART0->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART0_C2*/
		UART0->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART0->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_1 :
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK ;
		UART1->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART1->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART1->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART1->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART1->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART1_C2*/
		UART1->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART1->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_2 :
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK ;
		UART2->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART2->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART2->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART2->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART2->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART2_C2*/
		UART2->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART2->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_3 :
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK ;
		UART3->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART3->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART3->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART3->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART3->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART3_C2*/
		UART3->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART3->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_4 :
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK  ;
		UART4->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART4->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART4->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART4->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART4->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART4_C2*/
		UART4->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART4->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_5 :
		SIM->SCGC1 |= SIM_SCGC1_UART5_MASK ;
		UART5->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART5->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART5->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART5->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART5->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART5_C2*/
		UART5->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART5->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	default:
		break;
	}
}
void UART_interrupt_enable(uart_channel_t uart_channel)
{
	/* enabling correspondig reception interrupt in register UART_C2 ,RIE flag*/
	switch (uart_channel)
	{
	case UART_0:
		UART0->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_1:
		UART1->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_2:
		UART2->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_3:
		UART3->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_4:
		UART4->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_5:
		UART0->C2 |= UART_C2_RIE_MASK ;
		break;
	default:
		break;
	}
}
void UART_put_char (uart_channel_t uart_channel, uint8_t character)
{
	/* Before transmitting its important to check if the  UART channel
	 * is not transmitting already checking  flag*/

	switch (uart_channel)
	{
	case UART_0:
		while(0 == (UART0->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART0->D = character ;
		break;
	case UART_1:
		while(0 == (UART1->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART1->D = character ;
		break;
	case UART_2:
		while(0 == (UART2->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART2->D = character ;

		break;
	case UART_3:
		while(0 == (UART3->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART3->D = character ;
		break;
	case UART_4:
		while(0 == (UART4->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART4->D = character ;
		break;
	case UART_5:
		while(0 == (UART5->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/
		UART5->D = character ;
		break;
	default:
		break;
	}
}
void UART_put_string(uart_channel_t uart_channel, char* string)
{
	/* send string till null character*/
	while (0 != *string )
	{
		UART_put_char ( uart_channel, *(string) );

		string++;

	}

}

void UART_init_terminal_with_k20( uint32_t module_clk, uart_baud_rate_t baud_rate)
{
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	GPIO_clock_gating( GPIO_B );
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	gpio_pin_control_register_t uart_RX_TX_pin_mode = GPIO_MUX3;
	GPIO_pin_control_register(GPIO_B, 16, &uart_RX_TX_pin_mode);
	/**Configures the pin control register of pin17 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_B, 17, &uart_RX_TX_pin_mode);
	/**Configures UART 0 to transmit/receive at desired baudrate bauds with a 21 MHz of clock core*/
	UART_init (UART_0,  module_clk, baud_rate);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_8);
	/**Enables interrupts*/
	NVIC_global_enable_interrupts;
}
void UART0_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{

		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART0->S1;
		//dummyRead = UART0->D;

		g_mail_box_uart_0.mailBox = UART0->D;

		/*Set mailbox flag to one*/
		g_mail_box_uart_0.flag = 1;
	}

}
void UART1_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{

		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART1->S1;


		g_mail_box_uart_1.mailBox = UART1->D;

		/*Set mailbox flag to one*/
		g_mail_box_uart_1.flag = 1;
	}

}
void UART2_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{

		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART2->S1;
		//dummyRead = UART0->D;

		g_mail_box_uart_2.mailBox = UART2->D;

		/*Set mailbox flag to one*/
		g_mail_box_uart_2.flag = 1;
	}

}

void UART3_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{

		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART3->S1;
		//dummyRead = UART0->D;

		g_mail_box_uart_3.mailBox = UART3->D;

		/*Set mailbox flag to one*/
		g_mail_box_uart_3.flag = 1;
	}

}

void UART4_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{

		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART4->S1;
		//dummyRead = UART0->D;

		g_mail_box_uart_4.mailBox = UART4->D;

		/*Set mailbox flag to one*/
		g_mail_box_uart_4.flag = 1;
	}

}

void UART5_RX_TX_IRQHandler(void)
{
	/* Check if the reception is complete*/
	if(RECEIVE_BUFFER_EMPTY == UART_S1_RDRF(0))
	{
		/*To clear RFRD flag*/
		volatile uint32_t dummyRead;
		dummyRead = UART5->S1;
		g_mail_box_uart_5.mailBox = UART5->D;
		/*Set mailbox flag to one*/
		g_mail_box_uart_5.flag = 1;
	}

}
void UART_init_UART4( uint32_t module_clk, uart_baud_rate_t baud_rate)
{
	GPIO_clock_gating( GPIO_E );
	gpio_pin_control_register_t uart_RX_TX_pin_mode = GPIO_MUX3;
	GPIO_pin_control_register(GPIO_E, 24, &uart_RX_TX_pin_mode);
	/**Configures the pin control register of pin17 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_E, 25, &uart_RX_TX_pin_mode);
	UART_init (UART_4,  module_clk, BD_115200);
	UART_interrupt_enable(UART_4);
	/**Enables the UART 4 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_10);
	/**Enables interrupts*/
	NVIC_global_enable_interrupts;
}
