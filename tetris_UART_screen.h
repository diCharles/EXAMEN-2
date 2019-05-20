/*
 * tetris_UART_screen.h
 *
 *  Created on: 03/05/2019
 *      Author: José Rodrigo
 */

#ifndef TETRIS_UART_SCREEN_H_
#define TETRIS_UART_SCREEN_H_

#include "string.h"
#include "stdlib.h"
#include "UART.h"


#define UART0_BAUD_RATE 						115200u
#define SYSTEM_CLK								21000000u




#define BLACK_PIXEL		        "\033[0;030;40m[]"
#define RED_PIXEL			    "\033[0;031;32m[]"
#define GREEN_PIXEL			    "\033[0;032;33m[]"
#define YELLOW_PIXEL		    "\033[0;033;34m[]"
#define BLUE_PIXEL			    "\033[0;034;35m[]"
#define PURPLE_PIXEL		    "\033[0;035;36m[]"
#define CYAN_PIXEL  		    "\033[0;036;31m[]"
#define WHITE_PIXEL       	    "\033[0;037;40m[]"
#define ASCII_DEGRADATED	    176u
#define ASCII_HORIZONTAL_PIPE 	205u
#define ASCII_VERTICAL_PIPE		186u


#define	TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS    3u
#define	TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS    20u
#define TETRIS_FIELD_HEIGHT                     TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS + 20u
#define TETRIS_FIELD_LENGTH						TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS + 8u
#define TETRIS_HEIGHT							20u
#define TETRIS_LENTHT							10u
#define PIXEL_WIDTH                             2u
#define TETRIS_FIELD_DRAW_X_POS     			TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS-1 + PIXEL_WIDTH
#define TETRIS_FIELD_DRAW_y_POS  				TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS+1u

void print_tetris_game( uart_channel_t uart_channel,uint8_t gameMatrix[TETRIS_LENTHT][TETRIS_HEIGHT]);
/**
 brief: clear completely selected uart terminal
 paramIn : UART channel of k64 MCU
 */
void clear_screen( uart_channel_t uart_channel);
/**
 brief: moves terminal cursor of terminal to selected x and y position
 paramIn : UART channel of k64 MCU
 */
void set_screen_cursor(uart_channel_t uart_channel , uint32_t x_pos, uint32_t y_pos);
/**
 brief: draw on terminal of selected uart channel the delimited field for tetris game
 paramIn : UART channel of k64 MCU
 */
void draw_tetris_field(uart_channel_t uart_channel);
/**
 brief:  every tetrino is composed by 4 blocks,
 paramIn : UART channel of k64 MCU
 */
void draw_tetrimino_block(uart_channel_t uart_channel,  char pixel_color[],  uint32_t x_pos, uint32_t y_pos);
void draw_tetris_game_screen(uart_channel_t uart_channel);

#endif /* TETRIS_UART_SCREEN_H_ */
