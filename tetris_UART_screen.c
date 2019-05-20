/*
 * tetris_UART_screen.c
 *
 *  Created on: 03/05/2019
 *      Author: José Rodrigo
 */

#include "tetris_UART_screen.h"

char g_blackPixel[]= "\033[0;030;40m";
char g_redPixel[]= "\033[0;031;40m";
char g_greenPixel[]= "\033[0;032;40m";
char g_yellowPixel[]= "\033[0;033;40m";
char g_bluePixel[]= "\033[0;034;40m";
char g_purplePixel[] = "\033[0;035;40m";
char g_cyanPixel[]= "\033[0;036;40m";
char g_whitePixel[]= "\033[0;037;40m";



void clear_screen(uart_channel_t uart_channel)
{
	/*VT100 command for clearing the screen*/
	UART_put_string(uart_channel,"\033[2J");
}
void set_screen_cursor(uart_channel_t  uart_channel ,uint32_t x_pos, uint32_t y_pos)
{
	char str1[12] = "\033[";

	char y_pos_char [6];
	itoa( y_pos, y_pos_char , 10);
	strcat(str1,y_pos_char);
	char coma[]= ";";
	strcat(str1,coma);

	char x_pos_char [6];
	itoa( x_pos, x_pos_char , 10);
	strcat(str1,x_pos_char);

	char H[]= "H";
	strcat(str1, H);
	UART_put_string(uart_channel, str1);


}

void print_tetris_game( uart_channel_t uart_channel,uint8_t gameMatrix[TETRIS_LENTHT][TETRIS_HEIGHT])
{
	uint32_t x_pos;
	uint32_t y_pos;
	for(y_pos = 0 ; TETRIS_HEIGHT > y_pos ; y_pos++ )
	{
		for(x_pos=0 ; TETRIS_LENTHT > x_pos; x_pos++)
		{
			if(0 != gameMatrix[x_pos][y_pos])
			{
				draw_tetrimino_block( uart_channel, g_yellowPixel,TETRIS_FIELD_DRAW_X_POS +x_pos*PIXEL_WIDTH ,TETRIS_FIELD_DRAW_y_POS+ y_pos);
			}
		}
	}

	draw_tetrimino_block( uart_channel, g_redPixel,TETRIS_FIELD_DRAW_X_POS +7 ,TETRIS_FIELD_DRAW_y_POS+ 19);






}

void draw_tetris_field( uart_channel_t uart_channel)
{
	uint32_t y_pos;
	uint32_t x_pos;

	set_screen_cursor(uart_channel , TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS, TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS);
	/** Printing the x upper horizontal line*/
	for(x_pos = TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS ;(TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS +TETRIS_LENTHT) > x_pos ; x_pos++ )
	{
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
	}
	/** printing the  left vertical line*/
	for(y_pos = TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS ; TETRIS_FIELD_HEIGHT+2 > y_pos ; y_pos++)
	{
		/** printing a black pixel*/
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
		/** jump to a new pixel */

		set_screen_cursor(uart_channel , TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS,y_pos );
	}

	set_screen_cursor(uart_channel ,(TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS )+ (TETRIS_LENTHT *PIXEL_WIDTH)+1 , TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS);
	/** printing the  right vertical line*/
	for(y_pos = TETRIS_FIELD_LEFT_UPPER_CORNER_Y_POS ; TETRIS_FIELD_HEIGHT+2 > y_pos ; y_pos++)
	{
		/** printing a black pixel*/
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
		/** jump to a new pixel */
		set_screen_cursor(uart_channel ,(TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS )+ (TETRIS_LENTHT *PIXEL_WIDTH)+1,y_pos );
	}

	UART_put_string(uart_channel,g_whitePixel);
	UART_put_char (uart_channel, ASCII_DEGRADATED);
	set_screen_cursor( uart_channel ,TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS,y_pos-1 );

	/**printing the bottom horizontal line*/
	for(x_pos = TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS ;(TETRIS_FIELD_LEFT_UPPER_CORNER_X_POS +TETRIS_LENTHT) > x_pos ; x_pos++ )
	{
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
		UART_put_string(uart_channel,g_whitePixel);
		UART_put_char (uart_channel, ASCII_DEGRADATED);
	}

	/** printing missing pixel */
	UART_put_string(uart_channel,g_whitePixel);
	UART_put_char (uart_channel, ASCII_DEGRADATED);
}


void draw_tetrimino_block( uart_channel_t uart_channel, char pixel_color[], uint32_t x_pos, uint32_t y_pos)
{
	set_screen_cursor(uart_channel , x_pos, y_pos);
	UART_put_string(uart_channel,pixel_color);

}

