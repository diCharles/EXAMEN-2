/*
 * hall_of_fame.c
 *
 * brief This module reads the EEPROM memory to mantain a table with high scores and its authors
 *  Created on: 13/05/2019
 *      Author: Ruben Charles & Diego Charles
 */


#include "hall_of_fame.h"

/** Struct to initial configuration of EEPROM */
EEPROM_24LC256_t g_EEPROM = {I2C_0,I2C_BAUD_RATE,BUS_CLOCK,EEPROM_ADDRESS};
/** Pointer of previous struct for API of EEPROM */
EEPROM_24LC256_t * g_EEPROM_p = &g_EEPROM;


void hall_of_fame_peripherals_init(void)
{
	EEPROM_24LC256_init(g_EEPROM_p, CHIP_SELECT);
}

/**
 * brief Swap the values of two variables
 *
 * @param[in] winner_score
 * @param[in] score_to_be_replaced
 */
void swap_scores(uint32_t winner_score, uint32_t score_to_be_reaplaced, uint32_t * temp_score_arr , uint8_t index_of_data_swapped);

/**  brief  This function reads scores and names allocated in EEPROM and checks if the score of the player 
 *          deserves access to the hall. In this case, the hall is updated with player's name and score
 * 
 *  @param[in]   score              score of the player that won the match
 *  @param[in]   player_name        depending of this number (1 or 2), the function will read the name of the player from EEPROM
 */
submission_result_t hall_of_fame_submission( uint32_t score , uint8_t * player_name)
{



    static struct_EEPROM_READ_status_t EEPROM_read_return;
    static struct_EEPROM_READ_status_t * EEPROM_read_return_p = &EEPROM_read_return;


    static eeprom_init_status_t is_EEPROM_init_flag =  EEPROM_NOT_INIT;
    /** Configures EEPROM address and I2C if not configured yet*/
    if (EEPROM_NOT_INIT == is_EEPROM_init_flag )
    {
        EEPROM_24LC256_init(g_EEPROM_p, CHIP_SELECT);
        is_EEPROM_init_flag = EEPROM_ALREADY_INIT;
    }
    
   // uint8_t format[20]={};
   // EEPROM_24LC256_page_WRITE_with_array(g_EEPROM_p , 0, format, 20);



    /******************************* READING  EEPROM's SCORES **************************************************/



    submission_result_t submission_result_flag = NOT_ADMITTED;
    /** READ scores stored from the EEPROM and store them in a temporary array. */
    uint32_t temp_score_array[SIZE_OF_TEMP_SCRE_ARRAY]={};
    /** The EEPROM stores data in 8 blocks. In order to read a complete score (32 bits), four iterations are needed*/
      for(uint32_t score_address = ADDRESS_OF_SCORE_SECTION;   score_address  < SIZE_OF_SCORE_SECTION ; score_address+= SCORE_OFFSET  )
    {
        /** This temporary variable will store the 4 bytes of each score*/
        uint32_t temp_byte_read = TEMP_BYTE_READ_START_VALUE; // 0
        /** Read each byte of score */
        for(uint8_t score_byte = 0 ; score_byte < SIZE_OF_SCORE; score_byte ++ )
        {

        	EEPROM_read_return_p = EEPROM_24LC256_random_READ ( g_EEPROM_p,score_address + score_byte);
        	/** If EEPROM is read succesfully */
        	//if( READ_SUCCESS == EEPROM_read_return_p->EEPROM_READ_return_status)
        	//{
        		/** Add each byte read  to the score of each player in hall of fame  by using an array of masks*/
        		temp_byte_read |= EEPROM_read_return_p->EEPROM_byte_read <<  (score_byte * LEFT_SHIFT) ;
        	//}
        	//else
        	//{
        		/** If EEPROM was not found or I2C is busy for some reason */
        	//	return (ERROR);
        	//}

        }

        /** Store each score read*/
        temp_score_array[score_address / SCORE_OFFSET] = temp_byte_read;

    }


      /** If the score earned by the winner is high enough, the temporary array will be updated
       *  The lowest position corresponds to the lowest score in the ranking and vice-versa */
      uint8_t position_of_hall = 0
    		  ;
      for(uint8_t index_score = 0 ; index_score < SIZE_OF_TEMP_SCRE_ARRAY ; index_score ++ )
      {
    	  if( temp_score_array[ index_score ] < score )
    	  {
    		  swap_scores(score, temp_score_array[ index_score ], temp_score_array , index_score);
    		  submission_result_flag = ADMITTED;
    		  /** Place in hall for the new score*/
    		  position_of_hall = index_score;
    	  }
      }


        /************** Now, update the hall of fame**************************************************************************/


      /** If the score earned by the winner is high enough, the temporary array will be updated
       *  The lowest position corresponds to the lowest score in the ranking and vice-versa */



      /** Write the temporary array in EEPROM if any changes are needed in the hall of fame*/
      if(ADMITTED == submission_result_flag )
      {

    	  for(uint32_t score_address = ADDRESS_OF_SCORE_SECTION;   score_address  < SIZE_OF_SCORE_SECTION ; score_address+= SCORE_OFFSET  )
    	  {
    		  /** This temporary variable will store the 4 bytes of each score*/
    		  uint32_t temp_byte_read = temp_score_array[score_address/ SCORE_OFFSET];
    		  /** Write each byte of score */

    			  /** Store the LSB in lowest segment of memory*/
    		   EEPROM_24LC256_byte_WRITE(g_EEPROM_p ,  score_address + 0  ,(uint8_t)(temp_byte_read & 0x000000FF) );
    		   EEPROM_24LC256_byte_WRITE(g_EEPROM_p ,  score_address + 1,(uint8_t)(temp_byte_read & 0x0000FF00)  >> 8);
    		   EEPROM_24LC256_byte_WRITE(g_EEPROM_p ,  score_address + 2 ,(uint8_t)(temp_byte_read & 0x00FF0000) >> 16);
    		   EEPROM_24LC256_byte_WRITE(g_EEPROM_p ,  score_address + 3 ,(uint8_t)(temp_byte_read & 0xFF000000) >> 24);

    		  // LEFT_SHIFT
    	  }
    	  /** Store the name of the winner */

    	  EEPROM_24LC256_page_WRITE_with_array(g_EEPROM_p, NAME_PAGE_ADDRESS + OFFSET_OF_NAME_ADDRESS * position_of_hall , player_name, SIZE_OF_PLAYER_NAME);

    	  /** Our work is finished here*/
    	  return ( ADMITTED);

      }
        /** When the player was  not ELEGIBLE to be in the HALL of Fame*/
        return( NOT_ADMITTED);
    }
    


void swap_scores(uint32_t winner_score, uint32_t score_to_be_reaplaced, uint32_t * temp_score_arr , uint8_t index_of_data_swapped)
{
    uint32_t data_swap_aux = winner_score;
    /** The data to be swapped  if it's lower than the score is one position forward than the winner score*/
    uint32_t index_of_winner = index_of_data_swapped - 1 ;

    if(WINNER_STILL_OUT == index_of_winner)
    {
        temp_score_arr[index_of_data_swapped] = winner_score;

    }
    else
    {

          temp_score_arr[index_of_winner] =  temp_score_arr[index_of_data_swapped];
          temp_score_arr[index_of_data_swapped] = data_swap_aux;
    }
    
}

/**
 *  brief Stores data from EEPROM in the received arrray
 *
 * @param array_for_scores
 * @return
 */
struct_EEPROM_READ_status_t * scores_of_hall(uint32_t * array_for_scores,uint32_t size_of_array_of_scores)
{
	 static struct_EEPROM_READ_status_t EEPROM_read_return;
	 static struct_EEPROM_READ_status_t * EEPROM_read_return_p = &EEPROM_read_return;

	     uint32_t temp_scores[5]={};
	    /** The EEPROM stores data in 8 blocks. In order to read a complete score (32 bits), four iterations are needed*/
	      for(uint32_t score_address = ADDRESS_OF_SCORE_SECTION;   score_address  < SIZE_OF_SCORE_SECTION ; score_address+= SCORE_OFFSET  )
	    {
	        /** This temporary variable will store the 4 bytes of each score*/
	        uint32_t temp_byte_read = TEMP_BYTE_READ_START_VALUE; // 0
	        /** Read each byte of score */
	        for(uint8_t score_byte = 0 ; score_byte < SIZE_OF_SCORE; score_byte ++ )
	        {

	        	EEPROM_read_return_p = EEPROM_24LC256_random_READ ( g_EEPROM_p,score_address + score_byte);
	        	/** If EEPROM is read successfully */
	        	if( READ_SUCCESS == EEPROM_read_return_p->EEPROM_READ_return_status)
	        	{
	        		/** Add each byte read  to the score of each player in hall of fame  by using an array of masks*/
	        		temp_byte_read |= (uint32_t) EEPROM_read_return_p->EEPROM_byte_read <<  (score_byte * LEFT_SHIFT) ;
	        	}
	        	else
	        	{
	        		/** If EEPROM was not found or I2C is busy for some reason */
	        		EEPROM_read_return_p->EEPROM_READ_return_status = READ_ERROR;
	        		return (EEPROM_read_return_p);
	        	}

	        }

	        /** Store each score read*/
	        temp_scores[score_address / SCORE_OFFSET] = temp_byte_read;

	    }
	     EEPROM_read_return_p->EEPROM_READ_return_status = READ_SUCCESS;
	     array_for_scores = temp_scores;
	    return (EEPROM_read_return_p);



}

/**
 * brief Stores data from EEPROM in the received array
 * @param array_for_names
 * @return
 */
EEPROM_READ_status_t names_of_hall(uint8_t *array_for_names, uint8_t size_of_array_of_names)
{

	EEPROM_READ_status_t read_status;

	 /** Read EEPROM to get names*/
	 for (uint8_t index_for_names = 0; index_for_names < size_of_array_of_names; index_for_names++)
	 {

		 read_status = EEPROM_24LC256_sequential_READ_with_array ( g_EEPROM_p ,  NAME_PAGE_ADDRESS + OFFSET_OF_NAME_ADDRESS * index_for_names , array_for_names, SIZE_OF_PLAYER_NAME );

		 if(READ_ERROR == read_status)
		 {

			 return ( read_status);
		 }
	 }

	 return (read_status);

}


