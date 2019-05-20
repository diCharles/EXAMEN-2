/*
 * tetrisK64.h
 *
 *  Created on: 06/05/2019
 *      Author: Charles
 */

#ifndef TETRISK64_H_
#define TETRISK64_H_

#include "NVIC.h"										 //This library  handles interrupts
#include "PIT.h"										 //** the pit timer will make tetriminos fall at certain speed*/
#include "MK64F12.h"
#include "tetris_UART_screen.h"
#include "tetris_sounds.h"

#define SYSTEM_CLOCK			 (21000000U)  			 //CPU clock frequency at 21 MHz



#define PIXEL_ON 							  1u
#define PIXEL_OFF           	   			  0u
#define ROW_FULL_OF_PIXELS          		  10u
/** GAME SCREEN DIMENTIONS */
#define TOTAL_OF ROWS						  20u
#define ROW_LENGTH      					  8u
#define SPEED_OF_BLOCK_WHEN_DOWN_IS_PRESSED   3u

/** game input keys*/
/**Player 1  keys */
#define w_ASCII								  119u
#define a_ASCII								  97u
#define s_ASCII								  115u
#define d_ASCII             				  100

#define W_ASCII								  87u
#define A_ASCII								  65u
#define S_ASCII								  83u
#define D_ASCII             				  68u
/** Player 2 keys */
#define i_ASCII								  105u
#define j_ASCII								  106u
#define k_ASCII								  107u
#define l_ASCII							      108u /** ñ */

#define I_ASCII								  73u
#define J_ASCII								  74u
#define K_ASCII								  75u
#define L_ASCII 							  76u /** Ñ */

/** Pause keys */
#define p_ASCII								  112u
#define P_ASCII								  80u

#define NO_READ_KEY    						  0u
#define ROTATE 								  5u
#define LEFT								  4u
#define RIGHT								  7u
#define FALL 								  6u
#define START        						  6u
#define PAUSE								  80u
/***/
#define ERRASE_ON_TERM_AND_MATRIX             1u /** Erase tetrimino on terminal and game matrix*/
#define ERRASE_ON_MATRIX    				  0u /** Erase tetrimino only on game matrix*/

#define UNUSED_PAR							  0u
/** related to timer  */
#define PIT_DELAY					(0.800F)
/** tetrimino colors  index*/
#define RED_TETRI 					0u
#define GREEN_TETRI 				1u
#define YELLOW_TETRI 				2u
#define BLUE_TETRI 					3u
#define PURPLE_TETRI 				4u
#define CYAN_TETRI  				5u
#define NO_MOV_BY_USER				0u
#define ONE_POSITION       			1u
#define TOP_ROW						18u
#define GAME_SPACE_LEGNTH			20u
#define GAME_SPACE_WIDTH            10u
#define TETRIMINO_FIT				1u
#define TETRIMINO_CANT_FIT		    0u
#define INVALID_Y_POS      			30u
#define NUMBER_OF_TETRIMINOS		7u
#define SIZE_FOREGND_ESC_SEQ    	16u
#define NUMBER_COLORS_FOREGND       6u
#define TETRIMINO_MATRIX_ROWS		4u
#define TETRIMINO_MATRIX_COLS		4u
#define VT_DISPLAY_ORIGIN			0u
#define L_TETRIMINO_NUMBER	    	0u
#define J_TETRIMINO_NUMBER			1u
#define T_TETRIMINO_NUMBER			2u
#define S_TETRIMINO_NUMBER			3u
#define Z_TETRIMINO_NUMBER			4u
#define O_TETRIMINO_NUMBER			5u
#define I_TETRIMINO_NUMBER			6u


#define RED_TETRIMINO		0u
#define GREEN_TETRIMINO     1u
#define YELLOW_TETRIMINO    2u
#define BLUE_TETRIMINO      3u
#define PURPLE_TETRIMINO    4u
#define CYAN_TETRIMINO      5u

/** RELATED TO ROTATTION OF TETRMINO*/
#define X_ORIGIN_L_EXCEED_1 -1
#define X_ORIGIN_L_EXCEED_2 -2
#define X_ORIGIN_L_EXCEED_3 -3
#define X_ORIGIN_L_EXCEED_4 -4

#define X_ORIGIN_R_EXCEED_1 7u
#define X_ORIGIN_R_EXCEED_2 8u
#define X_ORIGIN_R_EXCEED_3 9u
#define X_ORIGIN_R_EXCEED_4 10u

/** related to score of players*/
#define INITIAL_SCORE 		0u
#define DECIMAL_BASE  		10u
#define SCORE_MULTPLIER 	1000u
#define TOTAL_OF_COMBOS     4u
/** related to player name read*/
#define ENTER 							0x0DU
#define ESCAPE						    0x1B
#define ESCAPE_UART4					63U
#define STRING_INGRESED 				1U
#define NO_STRING_INGRESED 				0U
#define ESCAPE_INGRESED                 3U
#define NULL_CHAR 						0U
#define RX_BUFFER_LENGTH 				30U

/** IDLE OF THE GAME*/
#define IDLE 100u
/* RELATED TO TETRIMONO MECHANICS SOUND GENERATION*/
#define LEFT_FREQ  								50000u
#define RIGTH_GREQ 								55000u
#define FALL_FREQ  								60000u
#define ROTATE_FREQ 							65000u

/** Struct that holds each the set of tetriminos */
typedef struct
{
	/** Each tetrimino is created inside a 4x4 matrix */
	uint8_t tetrimino_Matrix[TETRIMINO_MATRIX_ROWS][TETRIMINO_MATRIX_COLS];
}g_tetrimino_t;

/** For know when the game ends */
typedef enum{ GAME_CONTINUES , P1_LOST, P2_LOST }game_status_t;

/**--For printing the display---------------*/
/**
 * brief Displays game  in UART terminal
 * @param uart_channel
 * @param arr
 * @param repeat
 *
 * TODO: Remove repeat parameter
 */
void printScreen( uart_channel_t  uart_channel ,uint8_t arr[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH],uint8_t repeat);
/**
 * brief  Sets an specified array with zeros.
 * @param arr
 * @param lengthX
 * @param lengthY
 */
void ClearArray(uint8_t arr[8][20],uint8_t lengthX,uint8_t lengthY);
/**
 * Brief TETRIS MESSAGE
 *
 * TODO: check if is still convenient to use.
 */
void printTetris();
/**
 * Brief Prints a sad emoji in UART terminal when the player lose.
 *
 * @param uart_channel
 */
void printSadFace(uart_channel_t  uart_channel );    //Displays a GAME OVER condition in the dot matrix


/****************************** TETRIMINOS DRAWING/PRINTING/ERRASING FUNCTIONS ********************************************************************************************************************/
/**
 * brief : Initialize tetriminos figures,
 * the're 7 diferent tetriminos, each is composed by a 4*4 matriz
 * the function will draw the tetrino in his correspondig matrix
 * */
void init_tetriminos_set();
/**
 * brief: Clear tetrimino from matrix and UART terminal.
 * */
void erraseTetriminoOnScreen( uart_channel_t uart_channel,uint8_t someTetrimino,uint8_t direction, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);

/**
 * brief: Create tetrimino in matrix array of game and stores it in UART terminal
 * */
void drawTetrimino( uart_channel_t uart_channel,uint8_t someTetrimino, uint8_t tetriColor, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);

/****************************** TETRIMINOS MECHANICAL FUNCTIONS *********************************************************************************************************************************/
/**
 * brief: When a tetrimino is moved, this function checks if the piece fits in its landing position.
 * */
int8_t CheckIfTetriminoFits(uart_channel_t  uart_channel,uint8_t someTetrimino,uint8_t direction, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);
/**
 * brief: rotate tetrimino
 * */
void counter_clock_tetri_rotation(uart_channel_t  uart_channel,uint8_t * someTetrimino, int8_t  *originX,int8_t *originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);

/****************************** GAME PUNTUATIONS *****************************************************************************************************************************************************/
/**
 * brief: Computes player's score.
 * @param uart_channel
 * @param tetrisScreen
 * @return
 */
int8_t tetrisScore(uart_channel_t  uart_channel , uint8_t tetrisScreen[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);
/**
 * brief When player completes a row,  move the pieces that were on top of it.
 * @param tetrisScreen
 * @param fromLine
 */
void MakeScreenDescend(uint8_t tetrisScreen[8][20],int8_t fromLine);


/******************************* TETRIMINOS RANDOM SELECTION ***************************************************************************************************************************************/
/**
 * brief: Generates a random number based on the CVAL of PIT timer.
 * @param rangoInicial
 * @param rangoFinal
 * @return
 */
int ramdomNumber(uint8_t rangoInicial,uint8_t rangoFinal);
/**
 * brief: Generate the corresponding tetrimino to random number generated in previous function.
 * @param uart_channel
 * @param current_tetri
 * @param tetri_X
 * @param tetri_Y
 * @param tetri_color
 * @param gameSpace
 */
void generate_new_tetri(uart_channel_t uart_channel ,uint8_t * current_tetri, int8_t * tetri_X, int8_t* tetri_Y, uint8_t * tetri_color ,uint8_t  game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);

/****************************** HUMAN TETRINO CONTROL INTERACE FUNCIONS*******************************************************************************************************************************/
/**
 * brief  Reads key inputs from UART terminal for each player.
 * 		  And, changes move according to user's commands.
 * 		  The pause of system is taken as a non-move.
 * @param uart_channel Indicates which player need its tetrimino to move
 * @param move		   Indicates the type of move
 */
void read_tetri_movement_keys(uart_channel_t uart_channel , uint8_t * move);

void tetri_rigth(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);

void tetri_left(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);


void rotate_tetri(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);
game_status_t fall_tetri(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]);
void welcome_screen(uart_channel_t uart_channel);


void print_game_data_text_insole( uart_channel_t uart_channel,uint8_t player2_name[]);
/**
 * brief :  get player name from uart channel 0
 * @param name_string
 */
void get_players_name( 	uint8_t *p1_name , uint8_t * p2_name  );
uint8_t get_player1_name( uint8_t * rx_buffer);
/**
 * brief :  get player 2 name from uart channel 4
 * @param name_string
 */
uint8_t get_player2_name( uint8_t * rx_buffer);

void clear_rx_buffer( uint8_t * rx_buffer );

uint32_t get_score(uart_channel_t uart_channel);

#endif /* TETRISK64_H_ */
