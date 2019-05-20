/*
 * tetrisK64.c
 *
 *  Created on: 06/05/2019
 *      Author: Charles
 */

#include "tetrisK64.h"

/** Global variables */

/** Matrix that contains a message to print in UART terminal */
uint8_t g_Message[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH]={ 	};

/**This is mailbox to receive the information from the serial port*/
extern uart_mail_box_t g_mail_box_uart_0;
extern uart_mail_box_t g_mail_box_uart_4;


static uint32_t g_score_p1 = INITIAL_SCORE;
static uint32_t g_score_p2 = INITIAL_SCORE;

char g_combo_names [TOTAL_OF_COMBOS][10]={"NAC   ","DOUBLE", "TRIPLE","QUAD!!!"};
uint32_t combo_score[TOTAL_OF_COMBOS] = {1,4,8,16};



/** Array that contains strings with ESCAPE sequences for change foreground color in UART terminal */
char  avalaible_tetrimino_colors[NUMBER_COLORS_FOREGND][SIZE_FOREGND_ESC_SEQ] ={RED_PIXEL,GREEN_PIXEL,YELLOW_PIXEL,BLUE_PIXEL,PURPLE_PIXEL,CYAN_PIXEL};




g_tetrimino_t g_tetriminos_set_P1[NUMBER_OF_TETRIMINOS]; /** Array that holds  tetriminos set for player 1. */
g_tetrimino_t g_tetriminos_set_P2[NUMBER_OF_TETRIMINOS]; /** Array that holds  tetriminos set for player 2. */



void printScreen( uart_channel_t  uart_channel ,uint8_t arr[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH],uint8_t repeat)
{
	/** X an Y positions of cursor in VIRTUAL TERMINAL (UART TERMINAL) */
	uint32_t x_pos;
	uint32_t y_pos;


	for(x_pos = VT_DISPLAY_ORIGIN ; TETRIS_LENTHT > x_pos; x_pos++)
	{
		for(y_pos = VT_DISPLAY_ORIGIN ; TETRIS_HEIGHT > y_pos ; y_pos++ )
		{
			if(VT_DISPLAY_ORIGIN != arr[x_pos][y_pos])
			{

				draw_tetrimino_block( uart_channel, WHITE_PIXEL,TETRIS_FIELD_DRAW_X_POS +x_pos*PIXEL_WIDTH ,  TETRIS_FIELD_DRAW_y_POS+ (19-y_pos));

			}
			else
			{
				draw_tetrimino_block( uart_channel, "\033[0;030;40m[]",TETRIS_FIELD_DRAW_X_POS +x_pos*PIXEL_WIDTH ,  TETRIS_FIELD_DRAW_y_POS+ (19-y_pos));

			}
		}

	}


}

void counter_clock_tetri_rotation(uart_channel_t  uart_channel,uint8_t * someTetrimino, int8_t * originX,int8_t* originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	g_tetrimino_t  *set_ptr;/** ponter to tetriminos set pointer for player 1 or player 2*/

	if(UART_0 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P1;
	}
	else if(UART_4 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P2;
	}

	/** Copying tetrimino,in order to rotate it. */
	uint8_t x,y,CopyMatrix[4][4],aux;
	for (x = 0 ; x < 4 ; x++ )
	{
		for (y=0;y<4;y++)
		{
			if(UART_0 == uart_channel )
			{
				CopyMatrix[x][y]=set_ptr[*(someTetrimino)].tetrimino_Matrix[x][y];
			}
			else if (UART_4 == uart_channel)
			{
				CopyMatrix[x][y]=set_ptr[*(someTetrimino)].tetrimino_Matrix[x][y];
			}
		}
	}

	/** Rotate tetrimino using previous copy */
	for ( y = 0, aux = 3 ; y < 4 ; y++ , aux--)
	{
		for ( x = 0 ; x < 4 ; x++)
		{
			if(UART_0 == uart_channel )
			{
				set_ptr[*(someTetrimino)].tetrimino_Matrix[aux][x]=CopyMatrix[x][y];
			}
			else if (UART_4 == uart_channel)
			{
				g_tetriminos_set_P2[*(someTetrimino)].tetrimino_Matrix[aux][x]=CopyMatrix[x][y];
			}
		}
	}
	uint8_t aux_origin = *(originX);
	switch( *(originX))
	{
	/** Adjustment for left side */
	case X_ORIGIN_L_EXCEED_1:
		*(originX)+=1;
		break;

	case X_ORIGIN_L_EXCEED_2:
		*(originX)+=2;
		break;


		/** Adjustment for right side */
	case X_ORIGIN_R_EXCEED_1:
		*(originX)-=1;
		break;

	case X_ORIGIN_R_EXCEED_2:
		*(originX)-=2;
		break;


	default:
		/** Not move */
		break;
	}


	/** Checking if the tetrimino fits */

	if(TETRIMINO_CANT_FIT == CheckIfTetriminoFits(uart_channel,*(someTetrimino),0, *( originX),*(originY),game_space))//if the new rotation doesnot fit, we revert  the rotation
	{
		*(originX) = aux_origin;
		for ( x = 0 ; x < 4 ; x++)
		{
			for ( y = 0 ;y < 4 ; y++ )
			{

				set_ptr[*(someTetrimino)].tetrimino_Matrix[x][y] = CopyMatrix[x][y];

			}
		}

	}
}

void init_tetriminos_set(){
	/** L  block
	 *
	 *  	o
	 * 	    o
	 *      o o
	 */

	g_tetriminos_set_P1[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P1[L_TETRIMINO_NUMBER].tetrimino_Matrix[2][0] = PIXEL_ON;
	g_tetriminos_set_P1[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;

	g_tetriminos_set_P2[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P2[L_TETRIMINO_NUMBER].tetrimino_Matrix[2][0] = PIXEL_ON;
	g_tetriminos_set_P2[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[L_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;
	/** J block
	 *
	 * 	 	  o
	 * 	 	  o
	 * 	   o  o
	 */
	g_tetriminos_set_P1[J_TETRIMINO_NUMBER].tetrimino_Matrix[0][0] = PIXEL_ON;
	g_tetriminos_set_P1[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P1[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;

	g_tetriminos_set_P2[J_TETRIMINO_NUMBER].tetrimino_Matrix[0][0] = PIXEL_ON;
	g_tetriminos_set_P2[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P2[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[J_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;
	/** T block
	 *
	 * 		  o
	 * 		o o o
	 */
	g_tetriminos_set_P1[T_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[T_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P1[T_TETRIMINO_NUMBER].tetrimino_Matrix[3][1] = PIXEL_ON;
	g_tetriminos_set_P1[T_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;

	g_tetriminos_set_P2[T_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[T_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P2[T_TETRIMINO_NUMBER].tetrimino_Matrix[3][1] = PIXEL_ON;
	g_tetriminos_set_P2[T_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;
	/** S block
	 *
	 *        o o
	 *      o o
	 */
	g_tetriminos_set_P1[S_TETRIMINO_NUMBER].tetrimino_Matrix[0][0] = PIXEL_ON;
	g_tetriminos_set_P1[S_TETRIMINO_NUMBER].tetrimino_Matrix[0][1] = PIXEL_ON;
	g_tetriminos_set_P1[S_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[S_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;

	g_tetriminos_set_P2[S_TETRIMINO_NUMBER].tetrimino_Matrix[0][0] = PIXEL_ON;
	g_tetriminos_set_P2[S_TETRIMINO_NUMBER].tetrimino_Matrix[0][1] = PIXEL_ON;
	g_tetriminos_set_P2[S_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[S_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;
	/** Z block
	 *
	 *         o o
	 *           o o
	 */
	g_tetriminos_set_P1[Z_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P1[Z_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[Z_TETRIMINO_NUMBER].tetrimino_Matrix[0][1] = PIXEL_ON;
	g_tetriminos_set_P1[Z_TETRIMINO_NUMBER].tetrimino_Matrix[0][2] = PIXEL_ON;

	g_tetriminos_set_P2[Z_TETRIMINO_NUMBER].tetrimino_Matrix[1][0] = PIXEL_ON;
	g_tetriminos_set_P2[Z_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[Z_TETRIMINO_NUMBER].tetrimino_Matrix[0][1] = PIXEL_ON;
	g_tetriminos_set_P2[Z_TETRIMINO_NUMBER].tetrimino_Matrix[0][2] = PIXEL_ON;
	/** O block
	 *
	 * 		   o o
	 * 		   o o
	 */
	g_tetriminos_set_P1[O_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P1[O_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;
	g_tetriminos_set_P1[O_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P1[O_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;

	g_tetriminos_set_P2[O_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P2[O_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;
	g_tetriminos_set_P2[O_TETRIMINO_NUMBER].tetrimino_Matrix[1][1] = PIXEL_ON;
	g_tetriminos_set_P2[O_TETRIMINO_NUMBER].tetrimino_Matrix[1][2] = PIXEL_ON;
	/**
	 *
	 * 		   o
	 * 		   o
	 * 		   o
	 * 		   o
	 */
	g_tetriminos_set_P1[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][0] = PIXEL_ON;
	g_tetriminos_set_P1[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P1[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;
	g_tetriminos_set_P1[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][3] = PIXEL_ON;

	g_tetriminos_set_P2[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][0] = PIXEL_ON;
	g_tetriminos_set_P2[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][1] = PIXEL_ON;
	g_tetriminos_set_P2[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][2] = PIXEL_ON;
	g_tetriminos_set_P2[I_TETRIMINO_NUMBER].tetrimino_Matrix[2][3] = PIXEL_ON;

}

void ClearArray(uint8_t arr[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH],uint8_t lengthX,uint8_t lengthY)
{
	uint8_t x_pos,y_pos;
	for (x_pos=0;x_pos<lengthX;x_pos++)
		for (y_pos=0;y_pos<lengthY;y_pos++)
			arr[x_pos][y_pos]=0;
}



int ramdomNumber(uint8_t rangoInicial,uint8_t rangoFinal)
{
	/**use the timer for generate ramdom numbers */

	return  ( (PIT->CHANNEL[1].CVAL % (rangoFinal+1)) + rangoInicial  ) ;
}

void drawTetrimino( uart_channel_t uart_channel,uint8_t someTetrimino, uint8_t tetriColor, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	g_tetrimino_t  *set_ptr;/** ponter to tetriminos set poiter for player 1 or player 2*/

	uint8_t tetri_X = 0 , tetri_Y = 0 ;

	if(UART_0 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P1;
	}
	else if(UART_4 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P2;

	}

	for(int8_t x_pos = originX ; x_pos < originX + 4 ; x_pos++ ,tetri_X++ )
	{
		tetri_Y = 0;
		for (int8_t y_pos = originY ; y_pos < originY+4 ; y_pos++ , tetri_Y++)
		{
			/** Only draw the tetrimino if it fits in screen (game space or game area) "*/
			if( x_pos < GAME_SPACE_WIDTH && y_pos < GAME_SPACE_LEGNTH && x_pos >= 0 && y_pos>= 0)//setting only the tetrimino figure
			{
				/** Draw if there's nothing in that position */
				if( 0 == game_space[x_pos][y_pos])
				{
					/** Draw tetrimino according to the matrix that contains its piece */
					if( PIXEL_OFF !=  set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y] )
					{
						game_space[x_pos][y_pos]= PIXEL_ON;
						/** Draw each part of tetrimino */
						draw_tetrimino_block( uart_channel, avalaible_tetrimino_colors[tetriColor], TETRIS_FIELD_DRAW_X_POS +x_pos *PIXEL_WIDTH ,  TETRIS_FIELD_DRAW_y_POS+ (19 - y_pos) );


					}
					else
					{

						game_space[x_pos][y_pos]= PIXEL_OFF;
					}


				}
			}
		}
	}



}

int8_t CheckIfTetriminoFits(uart_channel_t  uart_channel,uint8_t someTetrimino,uint8_t direction, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	g_tetrimino_t  *set_ptr;/** pointer to tetrimino set of player 1 or player 2*/
	uint8_t tetri_X = 0 , tetri_Y = 0 ;

	if(UART_0 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P1;
	}
	else if(UART_4 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P2;
	}

	for(int8_t x_pos = originX ; x_pos< originX + 4 ; x_pos++ , tetri_X++)
	{
		tetri_Y=0;
		for (int8_t y_pos=originY ;y_pos<originY+4;y_pos++,tetri_Y++)
		{
			if (( ( PIXEL_ON== set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y]) &&  (x_pos>9) ) || ( (PIXEL_ON == set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y]) &&  (x_pos<0) )  )
			{
				return TETRIMINO_CANT_FIT;
			}
			if((PIXEL_ON == set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y]) && (y_pos<=-1))
			{
				return TETRIMINO_CANT_FIT;
			}

			if(tetri_X>=0 && tetri_X<=9 && tetri_Y>=0 && tetri_Y<=19 && x_pos>=0 && x_pos<=9 && y_pos>=0 && y_pos<=19 )//avoid checking on inexistent matrix ranges
				if((PIXEL_ON == set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y]) && (  PIXEL_ON == game_space[x_pos][y_pos]))//if there is a bit on means that...
					return TETRIMINO_CANT_FIT;
		}
	}
	return TETRIMINO_FIT;

}


void erraseTetriminoOnScreen( uart_channel_t uart_channel,uint8_t someTetrimino,uint8_t direction, int8_t originX,int8_t originY,uint8_t game_space[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	g_tetrimino_t  *set_ptr ; /** pointer to tetrimino set for player 1 and player 2*/
	if(UART_0 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P1;
	}
	else if(UART_4 == uart_channel)
	{
		set_ptr =  g_tetriminos_set_P2;
	}

	int8_t tetri_X = 0,tetri_Y = 0;
	for(int8_t x_pos = originX ; x_pos < originX + 4 ; x_pos++, tetri_X++)
	{
		tetri_Y=0;
		for (int8_t y_pos=originY ;y_pos<originY+4;y_pos++,tetri_Y++)
			if(tetri_X>=0 && tetri_X<=9 && tetri_Y>=0 && tetri_Y<=19 && x_pos>=0 && x_pos<=9 && y_pos>=0 && y_pos<=19 )//avoind checking on inexistent matrix ranges
				if(set_ptr[someTetrimino].tetrimino_Matrix[tetri_X][tetri_Y]==1)
				{
					/** clearing space in gameSpace matrix of tetrimino block, asumming tht each tetrimno is composed by four blocks*/
					game_space[x_pos][y_pos] = PIXEL_OFF ;
					/** clearing on uart terminal the tetrimino block,*/
					if(1 == direction)
					{
						draw_tetrimino_block( uart_channel, BLACK_PIXEL, TETRIS_FIELD_DRAW_X_POS +x_pos *PIXEL_WIDTH ,  TETRIS_FIELD_DRAW_y_POS+ (19 - y_pos) );
					}
				}
	}
}



int8_t tetrisScore(uart_channel_t  uart_channel , uint8_t tetrisScreen[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	/**   This function analyzes the  tetris screen
          and if finds a row full, increments the
          row score.

	 */

	uint8_t rowScore=0;/** will be usefull to calculate player score*/
	uint8_t errasable_rows_Y_positions[]={INVALID_Y_POS,INVALID_Y_POS,INVALID_Y_POS,INVALID_Y_POS,INVALID_Y_POS};
	uint8_t y_pos = 0 ;
	for (y_pos =0 ;GAME_SPACE_LEGNTH > y_pos; y_pos++)
	{

		uint8_t pixelsInrow=0; /** variable PointsInrow changed to pixelsInRow*/
		for (uint8_t x_pos=0;x_pos<GAME_SPACE_WIDTH;x_pos++)
		{

			if( PIXEL_OFF != tetrisScreen[x_pos][y_pos])
			{
				pixelsInrow++;//
			}
			if( ROW_FULL_OF_PIXELS <= pixelsInrow)
			{
				//saving the Y pos from row will be eliminated and other rows
				errasable_rows_Y_positions[rowScore] = y_pos ;
				rowScore ++;

			}

		}



	}
	/** screen will descend according to filled rows that player made*/
	/** and also the current score of the player will be calculated and displayed*/
	if(1 <= rowScore)
	{
		/** printing the score*/
		set_screen_cursor(  uart_channel ,54,7 );
		UART_put_string(UART_0,"\033[0;37;40m");
		/** the score of the player will be calculated here*/
		char score_str[10] ;
		if(UART_0 == uart_channel)
		{
			/** DISPLAYING SCORE TO THE OTHER PLAYER SCREEN*/
			g_score_p1 = g_score_p1 + rowScore*combo_score[rowScore-1] ;
			itoa(  g_score_p1, score_str, DECIMAL_BASE);
			UART_put_string( uart_channel,score_str);
			set_screen_cursor(  uart_channel ,51,9 );
			UART_put_string( uart_channel,g_combo_names[rowScore-1]);
			/** DISPLAYING SCORE TO THE OTHER PLAYER SCREEN*/
			set_screen_cursor(  UART_4 ,36,27 );
			UART_put_string( UART_4,score_str);
			set_screen_cursor(  UART_4 ,49,30 );
			UART_put_string( UART_4,g_combo_names[rowScore-1]);
		}
		else if(UART_4 == uart_channel)
		{
			/** DISPLAYING SCORE TO THE OTHER PLAYER SCREEN*/
			g_score_p2 = g_score_p2 + rowScore*combo_score[rowScore-1] ;
			itoa(g_score_p2, score_str , DECIMAL_BASE);
			UART_put_string( uart_channel,score_str);
			set_screen_cursor(  uart_channel ,51,9 );
			/** DISPLAYING SCORE TO THE OTHER PLAYER SCREEN*/
			UART_put_string( uart_channel,g_combo_names[rowScore-1]);
			set_screen_cursor(  UART_0 ,36,27 );
			UART_put_string( UART_0,score_str);
			set_screen_cursor(  UART_0 ,49,30 );
			UART_put_string( UART_0,g_combo_names[rowScore-1]);
		}

		/** screen will be decend and filled row errased*/
		for(uint8_t errased_rows = rowScore ; 0< errased_rows ; errased_rows--)
		{
			uint8_t row_Y = errasable_rows_Y_positions[errased_rows-1] ;
			if(INVALID_Y_POS != row_Y)
			{
				MakeScreenDescend(tetrisScreen,row_Y);

			}
		}
		/** change in the screen is noticeable after this sentence*/
		printScreen(uart_channel, tetrisScreen, 0);


	}


	return 0;
}




void MakeScreenDescend(uint8_t tetrisScreen[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH],int8_t fromLine)
{
	/** make screen descent from certain line or row*/
	int8_t x , y ;
	for (y = fromLine ; GAME_SPACE_LEGNTH>y ; y++)
		for (x = 0 ; x<GAME_SPACE_WIDTH; x++)
		{
			if (19>y)
			{
				tetrisScreen[x][y]=tetrisScreen[x][y+1];
			}
			else
			{
				tetrisScreen[x][y]=0;
			}
		}

}

void printTetris()
{
	//prints T E T R I S on screen
	ClearArray(g_Message,GAME_SPACE_WIDTH,GAME_SPACE_LEGNTH);
	//s
	g_Message[1][0]  = PIXEL_ON;
	g_Message[1][1]=1;
	g_Message[1][2]=1;
	g_Message[1][3]=1;
	g_Message[1][4]=1;
	//S
	g_Message[4][0]=1;
	g_Message[5][0]=1;
	g_Message[6][0]=1;
	g_Message[6][1]=1;
	g_Message[6][2]=1;
	g_Message[5][2]=1;
	g_Message[4][2]=1;
	g_Message[4][3]=1;
	g_Message[4][4]=1;
	g_Message[5][4]=1;
	//
	g_Message[6][4]=1;
	//T
	g_Message[6][0]=1;
	g_Message[1][6]=1;
	g_Message[1][7]=1;
	g_Message[1][8]=1;
	g_Message[1][9]=1;
	g_Message[0][9]=1;
	g_Message[2][9]=1;
	//R
	g_Message[4][6]=1;
	g_Message[4][7]=1;
	g_Message[4][8]=1;
	g_Message[4][9]=1;
	g_Message[5][7]=1;
	g_Message[5][9]=1;
	g_Message[6][8]=1;
	g_Message[6][9]=1;
	g_Message[6][6]=1;
	//T
	g_Message[1][11]=1;
	g_Message[1][12]=1;
	g_Message[1][13]=1;
	g_Message[1][14]=1;
	g_Message[1][15]=1;
	g_Message[0][15]=1;
	g_Message[2][15]=1;
	//E
	g_Message[4][11]=1;
	g_Message[5][11]=1;
	g_Message[6][11]=1;
	g_Message[4][12]=1;
	g_Message[4][13]=1;
	g_Message[4][14]=1;
	g_Message[4][15]=1;
	g_Message[5][15]=1;
	g_Message[6][15]=1;
	g_Message[5][13]=1;
	g_Message[6][13]=1;
	//printScreen(g_Message,1);
}




void printSadFace(uart_channel_t  uart_channel )
{

	ClearArray(g_Message,GAME_SPACE_WIDTH,GAME_SPACE_LEGNTH);
	g_Message[1][12]=1;
	g_Message[2][12]=1;
	g_Message[5][12]=1;
	g_Message[6][12]=1;
	g_Message[1][13]=1;
	g_Message[2][13]=1;
	g_Message[5][13]=1;
	g_Message[6][13]=1;

	g_Message[4][8]=1;
	g_Message[3][8]=1;

	g_Message[3][7]=1;
	g_Message[4][7]=1;
	g_Message[5][7]=1;
	g_Message[2][7]=1;

	g_Message[1][6]=1;

	g_Message[6][6]=1;
	g_Message[0][5]=1;
	g_Message[7][5]=1;

	printScreen(uart_channel,g_Message,1);
}



void read_tetri_movement_keys(uart_channel_t uart_channel , uint8_t * move)
{
	/*************************************** For player 	1 ****************************************/
	/*************************************************************************************************/
	if(TRUE == (g_mail_box_uart_0.flag) && UART_0== uart_channel )
	{
		if ( w_ASCII == g_mail_box_uart_0.mailBox      || W_ASCII == g_mail_box_uart_0.mailBox)
		{
			*(move) = ROTATE ;
			play_tetri_sound_effect(ROTATE_FREQ);

		}
		else if ( a_ASCII == g_mail_box_uart_0.mailBox || A_ASCII == g_mail_box_uart_0.mailBox)
		{
			*(move) = LEFT ;
			play_tetri_sound_effect(LEFT_FREQ);
		}
		else if ( s_ASCII == g_mail_box_uart_0.mailBox || S_ASCII == g_mail_box_uart_0.mailBox )
		{
			*(move) = FALL ;
			play_tetri_sound_effect(FALL_FREQ);
		}
		else if ( d_ASCII == g_mail_box_uart_0.mailBox || D_ASCII == g_mail_box_uart_0.mailBox )
		{
			*(move) = RIGHT ;
			play_tetri_sound_effect(RIGTH_GREQ );
		}
		/** When user press pause 'p' OR 'P' button */
		else if( p_ASCII == g_mail_box_uart_0.mailBox || P_ASCII == g_mail_box_uart_0.mailBox )
		{
			*(move) = PAUSE;
		}
		else
		{
			*(move) = NO_READ_KEY ;
		}


		g_mail_box_uart_0.flag = 0;/**/
	}

	/*************************************** For player 	2 ****************************************/
	/*************************************************************************************************/
	if(TRUE == (g_mail_box_uart_4.flag) && UART_4== uart_channel )
	{

		if ( i_ASCII == g_mail_box_uart_4.mailBox      || I_ASCII == g_mail_box_uart_4.mailBox)
		{
			*(move) = ROTATE ;
			play_tetri_sound_effect(ROTATE_FREQ);

		}
		else if ( j_ASCII == g_mail_box_uart_4.mailBox || J_ASCII == g_mail_box_uart_4.mailBox )
		{
			*(move) = LEFT ;
			play_tetri_sound_effect(LEFT_FREQ);
		}
		else if ( k_ASCII == g_mail_box_uart_4.mailBox || K_ASCII == g_mail_box_uart_4.mailBox)
		{
			*(move) = FALL ;
		}
		else if ( l_ASCII == g_mail_box_uart_4.mailBox || L_ASCII == g_mail_box_uart_4.mailBox)
		{
			*(move) = RIGHT ;
		}
		else if( p_ASCII == g_mail_box_uart_4.mailBox  || P_ASCII == g_mail_box_uart_4.mailBox )
		{
			*(move) = PAUSE;
		}
		else if ( s_ASCII == g_mail_box_uart_0.mailBox || S_ASCII == g_mail_box_uart_0.mailBox )
		{
			*(move) = FALL ;
		}
		else
		{
			*(move) = NO_READ_KEY ;
		}

		g_mail_box_uart_4.flag = 0;/**/
	}
}
void generate_new_tetri(uart_channel_t uart_channel ,uint8_t * current_tetri, int8_t * tetri_X, int8_t* tetri_Y, uint8_t * tetri_color ,uint8_t  gameSpace[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	*(current_tetri) = ramdomNumber(0,6); // numeros random provienen del timer
	/** eligiendo coordenadas para el tetrimino*/
	*( tetri_Y ) = 19;
	*( tetri_X ) =  3 ;
	switch(*(current_tetri))
	{
	case L_TETRIMINO_NUMBER	:

		*( tetri_color ) = RED_TETRIMINO ;  // L BLOCK COLOR: RED
		break;
	case J_TETRIMINO_NUMBER :

		*( tetri_color ) = YELLOW_TETRIMINO;
		break;
	case T_TETRIMINO_NUMBER	 :

		*( tetri_color ) =  PURPLE_TETRIMINO;
		break;

	case S_TETRIMINO_NUMBER	:

		*( tetri_color ) =  GREEN_TETRIMINO;
		break;

	case Z_TETRIMINO_NUMBER	:

		*( tetri_color ) = BLUE_TETRIMINO;
		break;
	case O_TETRIMINO_NUMBER	:

		*( tetri_color ) = CYAN_TETRIMINO;
		break;

	case I_TETRIMINO_NUMBER	:
		/** There's not enough colors, so this piece will have a random one */
		*( tetri_color ) = ramdomNumber(0,5);
		break;

	default:
		*( tetri_color ) =  PURPLE_TETRIMINO;
	}


	/** revisando si el tetrimino cabe en lamatriz de juego*/
	if( TRUE == CheckIfTetriminoFits(uart_channel, *(current_tetri),UNUSED_PAR, *( tetri_X )  , *( tetri_Y ),  gameSpace) )
	{
		/** como el tetrimino, cabe será dibujado*/
		drawTetrimino (uart_channel,  *(current_tetri),*( tetri_color ),*( tetri_X )  , *( tetri_Y ),  gameSpace);
	}
}


void tetri_rigth(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	uint8_t tetri_X_aux = *(tetri_X_ptr) + (ONE_POSITION);
	/** Temporally erased from matrix that stores position. Although is still visible to user */
	erraseTetriminoOnScreen(uart_channel,*(actual_tetri_ptr), ERRASE_ON_TERM_AND_MATRIX, *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	if( TRUE == CheckIfTetriminoFits(uart_channel,*(actual_tetri_ptr), UNUSED_PAR , tetri_X_aux, *(tetri_Y_ptr), game_area_of_display) )
	{
		*(tetri_X_ptr) = tetri_X_aux ;
		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);

	}
	else
	{
		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	}
}

void tetri_left(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	uint8_t tetri_X_aux = *(tetri_X_ptr) - (ONE_POSITION);
	/** Temporally erased from matrix that stores position. Although is still visible to user */
	erraseTetriminoOnScreen(uart_channel,*(actual_tetri_ptr), ERRASE_ON_TERM_AND_MATRIX, *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	if( TRUE == CheckIfTetriminoFits(uart_channel,*(actual_tetri_ptr), UNUSED_PAR , tetri_X_aux, *(tetri_Y_ptr), game_area_of_display) )
	{
		*(tetri_X_ptr) = tetri_X_aux ;
		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);

	}
	else
	{
		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	}
}



void rotate_tetri(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	erraseTetriminoOnScreen(uart_channel,*(actual_tetri_ptr), ERRASE_ON_TERM_AND_MATRIX, *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	counter_clock_tetri_rotation(uart_channel,actual_tetri_ptr, tetri_X_ptr, tetri_Y_ptr, game_area_of_display);
	drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
}

void welcome_screen(uart_channel_t uart_channel)
{
	set_screen_cursor(uart_channel , 15,5);
	UART_put_string( uart_channel, " ITESO EMBEDDED SYSTEMS 1  PRESENTS");
	set_screen_cursor(uart_channel , 20,10);
	UART_put_string( uart_channel, " TETRHANOS-K64");
	set_screen_cursor(uart_channel , 12,20);
	UART_put_string( uart_channel, " TYPE YOUR NAME");






}

game_status_t fall_tetri(uart_channel_t uart_channel,uint8_t* actual_tetri_ptr,int8_t * tetri_X_ptr, int8_t * tetri_Y_ptr,uint8_t *tetri_color_ptr,uint8_t game_area_of_display[GAME_SPACE_WIDTH][GAME_SPACE_LEGNTH])
{
	static  uint8_t counts_to_loose = 0;
	uint8_t tetri_Y_aux = (  *(tetri_Y_ptr) - ONE_POSITION) ;
	erraseTetriminoOnScreen(uart_channel,*(actual_tetri_ptr), ERRASE_ON_TERM_AND_MATRIX, *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
	if( TRUE == CheckIfTetriminoFits(uart_channel,*(actual_tetri_ptr), UNUSED_PAR , *(tetri_X_ptr), tetri_Y_aux, game_area_of_display) )
	{

		*(tetri_Y_ptr) = tetri_Y_aux ;
		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
		counts_to_loose = 0;
	}
	/** If the current tetrimino lands, it will no fit anymore and a new piece is created */
	else
	{

		drawTetrimino(uart_channel, *(actual_tetri_ptr), *(tetri_color_ptr) , *(tetri_X_ptr), *(tetri_Y_ptr), game_area_of_display);
		tetrisScore( uart_channel ,game_area_of_display);
		generate_new_tetri(uart_channel, actual_tetri_ptr,  tetri_X_ptr, tetri_Y_ptr ,tetri_color_ptr , game_area_of_display);
		counts_to_loose ++;

		/** If the screen of the player (1 or 2) fills with tetriminos*/
		if ( (TOP_ROW <= *(tetri_Y_ptr)  )  && 	 (3 <= counts_to_loose ))
		{

			if(UART_0 == uart_channel)
			{
				return (P1_LOST);
				g_score_p1= INITIAL_SCORE ;
				g_score_p1= INITIAL_SCORE ;
			}
			else
			{
				return (P2_LOST);
				g_score_p1= INITIAL_SCORE ;
				g_score_p1= INITIAL_SCORE ;
			}

		}



	}
	return (GAME_CONTINUES );
}


void print_game_data_text_insole( uart_channel_t uart_channel,uint8_t player2_name[])
{
	char player_text[]=" ";

	/** text in blue for player 1 or UART_0*/
	if( UART_0 == uart_channel)
	{

		clear_screen(uart_channel);
		UART_put_string(uart_channel,"\033[0;31;40m");

		set_screen_cursor(uart_channel , 2,27);
		UART_put_string( uart_channel,  "OPONENT DATA-> STATUS:      SCORE:         COMBOS:");

		UART_put_string(uart_channel,"\033[0;36;40m");
		set_screen_cursor(uart_channel , 25,25);
		UART_put_string( uart_channel, "PLAYER 1 :");
		UART_put_string( uart_channel, (char *)player2_name);
		set_screen_cursor(uart_channel , 1,8);
		UART_put_string( uart_channel, " KEY MAP");
		set_screen_cursor(uart_channel , 1,10);
		UART_put_string( uart_channel, " 'R' ROTATE");
		set_screen_cursor(uart_channel , 1,12);
		UART_put_string( uart_channel, " 'A' LEFT");
		set_screen_cursor(uart_channel , 1,14);
		UART_put_string( uart_channel, " 'D' RIGTH ");
		set_screen_cursor(uart_channel , 1,16);
		UART_put_string( uart_channel, " 'S' FALL");
		/** Draw the game area */
		draw_tetris_field(uart_channel);
	}
	/** text in red for player 2 or UART_4*/
	else if( UART_4 == uart_channel)
	{
		clear_screen(uart_channel);
		UART_put_string(UART_4,"\033[0;36;40m");
		set_screen_cursor(uart_channel , 2,27);
		UART_put_string( uart_channel,  "OPONENT DATA-> STATUS:      SCORE:         COMBOS:");
		set_screen_cursor(uart_channel , 3,37);


		UART_put_string(UART_4,"\033[0;31;40m");
		set_screen_cursor(uart_channel , 25,25);
		UART_put_string( uart_channel,  "PLAYER 2 :");
		UART_put_string( uart_channel,(char *) player2_name);
		set_screen_cursor(uart_channel , 1,8);
		UART_put_string( uart_channel, " KEY MAP");
		set_screen_cursor(uart_channel , 1,10);
		UART_put_string( uart_channel, " 'I' ROTATE");
		set_screen_cursor(uart_channel , 1,12);
		UART_put_string( uart_channel, " 'J' LEFT");
		set_screen_cursor(uart_channel , 1,14);
		UART_put_string( uart_channel, " 'L' RIGTH ");
		set_screen_cursor(uart_channel , 1,16);
		UART_put_string( uart_channel, " 'K' FALL");
		draw_tetris_field(uart_channel);


	}


	set_screen_cursor(uart_channel , 20,2);
	UART_put_string( uart_channel, " TETRIS-K64 ");
	set_screen_cursor(uart_channel , 43,4);
	UART_put_string( uart_channel, " PRESS 'P' TO PAUSE");
	set_screen_cursor(uart_channel , 43,7);
	UART_put_string( uart_channel, " SCORE :");
	set_screen_cursor(uart_channel , 43,9);
	UART_put_string( uart_channel, " COMBO :");
	set_screen_cursor(uart_channel , 43,11);
	UART_put_string( uart_channel, " LEVEL :");
	set_screen_cursor(uart_channel , 20,25);
	UART_put_string( uart_channel, player_text);

}

void get_players_name( 	uint8_t *p1_name , uint8_t * p2_name  )
{
	/** Here, each, process is blocked until the player types its name and both press 'ENTER'*/

	/** Get string from each player (NAME) */
	uint8_t p1_put_his_name_flag = FALSE ;
	uint8_t p2_put_his_name_flag = FALSE ;
	while (  (FALSE == p1_put_his_name_flag  ) ||  ( FALSE == p2_put_his_name_flag ))
	{
		if( (STRING_INGRESED  == get_player1_name(p1_name )) && ( FALSE == p1_put_his_name_flag ) )
		{

			set_screen_cursor(UART_0 , 2,27);
			UART_put_string(UART_0, " BIENVENIDO : ") ;
			UART_put_string(UART_0,(char *) p1_name ) ;
			set_screen_cursor(UART_0 , 2,28);
			UART_put_string(UART_0, " AWAITING FOR PLAYER 2: ") ;
			p1_put_his_name_flag = TRUE;
		}
		if( (STRING_INGRESED == get_player2_name(p2_name )) && ( FALSE == p2_put_his_name_flag ))
		{

			set_screen_cursor(UART_4 , 2,27);
			UART_put_string(UART_4, " BIENVENIDO : ") ;
			UART_put_string(UART_4,(char *) p2_name );
			set_screen_cursor(UART_4 , 2,28);
			UART_put_string(UART_4, " AWAITING FOR PLAYER 2: ") ;
			p2_put_his_name_flag = TRUE;
		}
	}

	return ;
}
uint8_t get_player1_name( uint8_t * rx_buffer)
{
	static uint8_t rx_bufer_index = 0;
	/** if a  key has been pressed the flag will be set*/
	if(TRUE == (g_mail_box_uart_0.flag)  )
	{
		/** if the key was ENTER , jump a line, and write string end*/
		if(ENTER == g_mail_box_uart_0.mailBox )
		{
			UART_put_string(UART_0, "( ENTER )\n ") ;
			UART_put_char (UART_0, ENTER) ;
			rx_buffer[rx_bufer_index] = NULL_CHAR ;/* end of string into buffer . Writing null character*/
			rx_bufer_index = 0 ;
			g_mail_box_uart_0.flag = 0;
			return ( STRING_INGRESED ) ;
		}
		/** else if the key is  escape, the user wants to quit*/
		else if ( ESCAPE == g_mail_box_uart_0.mailBox)
		{

			UART_put_char (UART_0, ENTER);
			rx_bufer_index = 0 ;/* clearing buffer and his indexer*/
			clear_rx_buffer( rx_buffer ) ;
			UART_put_string(UART_0, "( ESC uart0)\n");
			g_mail_box_uart_0.flag = 0;/**/
			return  (ESCAPE_INGRESED);

		}
		/** else the key is stored into rx buffer*/
		else
		{
			UART_put_char (UART_0, g_mail_box_uart_0.mailBox);
			rx_buffer[rx_bufer_index] = g_mail_box_uart_0.mailBox;/* write into buffer*/
			rx_bufer_index++;/* enables to  store a new character on buffer*/
			/** if the buffer has been filled*/
			if (RX_BUFFER_LENGTH <= rx_bufer_index )
			{
				/* clear buffer and reset his index*/
				rx_bufer_index = 0;
				clear_rx_buffer( rx_buffer ) ;
			}
		}
		g_mail_box_uart_0.flag = 0;/**/

	}
	return  (NO_STRING_INGRESED ) ;
}
uint8_t  get_player2_name( uint8_t * rx_buffer)
{
	static uint8_t rx_bufer_index = 0;
	/** if a  key has been pressed the flag will be set*/
	if(TRUE == (g_mail_box_uart_4.flag)  )
	{
		/** if the key was ENTER , jump a line, and write string end*/
		if(ENTER == g_mail_box_uart_4.mailBox )
		{
			UART_put_string(UART_4, "( ENTER )\n ") ;
			UART_put_char (UART_4, ENTER) ;
			rx_buffer[rx_bufer_index] = NULL_CHAR ;/* end of string into buffer . Writing null character*/
			rx_bufer_index = 0 ;
			g_mail_box_uart_4.flag = 0;
			return ( STRING_INGRESED ) ;
		}
		/** else if the key is  escape, the user wants to quit*/
		else if ( ESCAPE == g_mail_box_uart_4.mailBox)
		{

			UART_put_char (UART_4, ENTER);
			rx_bufer_index = 0 ;/* clearing buffer and his indexer*/
			clear_rx_buffer( rx_buffer ) ;
			UART_put_string(UART_4, "( ESC uart0)\n");
			g_mail_box_uart_4.flag = 0;/**/
			return  (ESCAPE_INGRESED);

		}
		/** else the key is stored into rx buffer*/
		else
		{
			UART_put_char (UART_4, g_mail_box_uart_4.mailBox);
			rx_buffer[rx_bufer_index] = g_mail_box_uart_4.mailBox;/* write into buffer*/
			rx_bufer_index++;/* enables to  store a new character on buffer*/
			/** if the buffer has been filled*/
			if (RX_BUFFER_LENGTH <= rx_bufer_index )
			{
				/* clear buffer and reset his index*/
				rx_bufer_index = 0;
				clear_rx_buffer( rx_buffer ) ;
			}
		}
		g_mail_box_uart_4.flag = 0;/**/

	}
	return  (NO_STRING_INGRESED ) ;
}


void clear_rx_buffer( uint8_t * rx_buffer )
{
	uint8_t iterator ;
	for(iterator = 0 ; ( RX_BUFFER_LENGTH -1 ) > iterator ;  iterator ++ )
	{
		rx_buffer[iterator] = 0;
	}
}

uint32_t get_score(uart_channel_t uart_channel)
{

	/** Determines which score is requested*/
	if( UART_0 == uart_channel)
	{
		return (g_score_p1);
	}
	else
	{
		return (g_score_p2);
	}
}

