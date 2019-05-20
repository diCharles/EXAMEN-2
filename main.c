



/*
 * TetrisAVR.c
 *
 * Created: 03/11/2017 12:53:34 p. m.
 *  Author: Charles Suarez Diego / Charles Suarez Ruben
 */



//** the pit timer will make tetriminos fall at certain speed*/
#include "MK64F12.h"
#include "tetrisK64.h"
#include "Delay.h"
#include "hall_of_fame.h"
#include "string.h"
#include "rgb.h"


#define DELAY_GAME_OVER 300000
#define DELAY_HALL_OF_FAME 600000
/** For print elements of hall of fame*/
#define OFFSET_FOR_PRINT 2u
#define SIZE_OF_ITOA 10u


/** Flag for timer that controls the fall of the tetriminos */
volatile uint8_t g_timer_ISR_P1 = FALSE ;
volatile uint8_t g_timer_ISR_P2 = FALSE ;

void decode_string(uint8_t *rx_buffer);


int main(void)
{

	/** The UART 0 sends data to graph the player GUI and receives keystrokes from user.*/
	UART_init_terminal_with_k20( SYSTEM_CLK, UART0_BAUD_RATE);
	clear_screen(UART_0);
	UART_put_string(UART_0, " EXAMEN 2 PIZANO    ") ;
	/** acquiring players name by uart terminal 0 and 4 */
	uint8_t p1_name_arr[RX_BUFFER_LENGTH] = {0};
	init_rgb();

	while(1)
	{

		// read new sequence
		if(STRING_INGRESED  == get_player1_name( p1_name_arr))
		{
			decode_string(p1_name_arr);
			clear_rx_buffer( p1_name_arr ) ;
		}



	}
	/** never leave main*/
}



void decode_string(uint8_t *rx_buffer)
{
	uint8_t color_led[6]={OFF,BLUE,GREEN,RED,YELLOW,PURPLE};
	uint8_t colors_to_print[40]= {0};
	uint8_t colors_duration[40]={0};
	uint8_t founded_colors =0;
	uint8_t start_sequence = FALSE;
	while(  0 != *(rx_buffer))
	{
		if((uint8_t)'R' ==  *(rx_buffer)   )
		{
			rx_buffer = rx_buffer + 1;// 												color letter found
			if((uint8_t)',' ==  *(rx_buffer)   )
			{
				rx_buffer = rx_buffer + 1;// coma , found
				if((uint8_t)'0' <=  *(rx_buffer) && (uint8_t)'9' >=  *(rx_buffer)   )
				{
					rx_buffer = rx_buffer + 1;										 // number found
					if((uint8_t)';' ==  *(rx_buffer)   )
					{

						colors_to_print[founded_colors]= color_led[3];				// Color asignation
						uint8_t number =*(rx_buffer-1);

						colors_duration[founded_colors]=number-48;
						founded_colors++;
						rx_buffer = rx_buffer + 1;									 // ;  found
					}




				}
			}
		}
		else if((uint8_t)'A' ==  *(rx_buffer)  || (uint8_t)'B' ==  *(rx_buffer)  )
		{
			rx_buffer = rx_buffer + 1;// 												color letter found
			if((uint8_t)',' ==  *(rx_buffer)   )
			{
				rx_buffer = rx_buffer + 1;// coma , found
				if((uint8_t)'0' <=  *(rx_buffer) && (uint8_t)'9' >=  *(rx_buffer)   )
				{
					rx_buffer = rx_buffer + 1;										 // number found
					if((uint8_t)';' ==  *(rx_buffer)   )
					{

						colors_to_print[founded_colors]= color_led[1];
						uint8_t number =*(rx_buffer-1);// Color asignation
						colors_duration[founded_colors]=number-48;
						founded_colors++;
						rx_buffer = rx_buffer + 1;									 // ;  found
					}




				}
			}
		}
		else if((uint8_t)'G' ==  *(rx_buffer)  || (uint8_t)'V' ==  *(rx_buffer)  )
		{
			rx_buffer = rx_buffer + 1;// 												color letter found
			if((uint8_t)',' ==  *(rx_buffer)   )
			{
				rx_buffer = rx_buffer + 1;// coma , found
				if((uint8_t)'0' <=  *(rx_buffer) && (uint8_t)'9' >=  *(rx_buffer)   )
				{
					rx_buffer = rx_buffer + 1;										 // number found
					if((uint8_t)';' ==  *(rx_buffer)   )
					{

						colors_to_print[founded_colors]= color_led[2];				// Color asignation
						uint8_t number =*(rx_buffer-1);// Color asignation
						colors_duration[founded_colors]=number-48;
						founded_colors++;
						rx_buffer = rx_buffer + 1;									 // ;  found
					}




				}
			}
		}
		else if((uint8_t)'Y' ==  *(rx_buffer)   )
		{
			rx_buffer = rx_buffer + 1;// 												color letter found
			if((uint8_t)',' ==  *(rx_buffer)   )
			{
				rx_buffer = rx_buffer + 1;// coma , found
				if((uint8_t)'0' <=  *(rx_buffer) && (uint8_t)'9' >=  *(rx_buffer)   )
				{
					rx_buffer = rx_buffer + 1;										 // number found
					if((uint8_t)';' ==  *(rx_buffer)   )
					{

						colors_to_print[founded_colors]= color_led[4];				// Color asignation
						uint8_t number =*(rx_buffer-1);// Color asignation
						colors_duration[founded_colors]=number-48;
						founded_colors++;
						rx_buffer = rx_buffer + 1;									 // ;  found
					}




				}
			}
		}
		else if((uint8_t)'P' ==  *(rx_buffer) || (uint8_t)'M' ==  *(rx_buffer)  )
		{
			rx_buffer = rx_buffer + 1;// 												color letter found
			if((uint8_t)',' ==  *(rx_buffer)   )
			{
				rx_buffer = rx_buffer + 1;// coma , found
				if((uint8_t)'0' <=  *(rx_buffer) && (uint8_t)'9' >=  *(rx_buffer)   )
				{
					rx_buffer = rx_buffer + 1;										 // number found
					if((uint8_t)';' ==  *(rx_buffer)   )
					{

						colors_to_print[founded_colors]= color_led[5];				// Color asignation
						uint8_t number =*(rx_buffer-1);// Color asignation
						colors_duration[founded_colors]=number-48;
						founded_colors++;
						rx_buffer = rx_buffer + 1;									 // ;  found
					}




				}
			}
		}

		else if((uint8_t)'S' ==  *(rx_buffer) || (uint8_t)'s' ==  *(rx_buffer)  )
		{
			start_sequence = TRUE ;
			rx_buffer++;
		}
		else
		{
			rx_buffer ++;
		}

	}
	uint8_t iterations ;
	if( TRUE== start_sequence)
	{
		for(iterations =0; founded_colors>iterations; iterations++)
		{
			rgb_color(colors_to_print[iterations],ON);
			uint8_t repeat;
			for(repeat =0;colors_duration[iterations] > repeat; repeat++)
			{
				delay(25000);
			}
			rgb_color(colors_to_print[iterations],OFF);
		}
	}
	clear_rx_buffer( rx_buffer ) ;
}
