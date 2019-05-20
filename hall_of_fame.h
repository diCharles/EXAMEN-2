/*
 * hall_of_fame.h
 *
 *  Created on: 13/05/2019
 *      Author: José Rodrigo
 */

#ifndef HALL_OF_FAME_H_
#define HALL_OF_FAME_H_

#include "MK64F12.h"
#include "EEPROM_24LC256.h"

/** For handling EEPROM memory: READ/WRITE operations*/
#define SIZE_OF_SCORE 4
#define SCORE_OFFSET 0x04
#define ADDRESS_OF_SCORE_SECTION 0x0000
#define SIZE_OF_SCORE_SECTION 20u
#define SIZE_OF_TEMP_SCRE_ARRAY 5u
#define TEMP_BYTE_READ_START_VALUE 0u
#define LEFT_SHIFT 8u
#define WINNER_STILL_OUT -1
#define SIZE_OF_HALL 5u

/** For memory addressing */
#define SCORE_PAGE_ADDRESS      0x0000
#define NAME_PAGE_ADDRESS       0x00C8
#define OFFSET_OF_SCORE_ADDRESS 0x0008
#define OFFSET_OF_NAME_ADDRESS  0x003C

#define SIZE_OF_PLAYER_NAME     50u
/** For EEPROM configuration ****/
#define  BUS_CLOCK 10500000U
#define  I2C_BAUD_RATE 100000U
#define CHIP_SELECT 0x0000U
#define SIZE_OF_MESSAGE 50U
/** For memory formatting */
#define FORMAT_START_ADDRESS 0x0000
#define FORMAT_END_ADDRESS  0x02BC
/** For EEPROM configuration *****************************************************************/
#define  BUS_CLOCK 10500000U
#define  I2C_BAUD_RATE 100000U
#define CHIP_SELECT 0x0000U
#define SIZE_OF_MESSAGE 50U




/** MEMORY MAP OF DATA
 *  ADDRESS 0h     C8h     1F4h               258h
 *  DATA   [SCORES][NAMES][NAME OF PLAYER 1] [NAME OF PLAYER 2]
 * 
 * SCORES COMPOSITON
 *  0h                                                 0h +  OFFSET_OF_NAME_ADDRESS * 5
 *  [SCORE 4 (LOWEST)] [SCORE 3] [SCORE 2] [SCORE 1] [SCORE 0 (HIGHEST RANKED)]
 * 
 * NAME COMPOSITION
 *  C8h                                                C8 + OFFSET_OF_NAME_ADDRESS * 5
 *  [NAME 4 (LOWEST)] [NAME 3 ] [ NAME 2] [ NAME 1 ] [NAME O (HIGHEST RANKED)]
 */

/** For EEPROM use */
typedef enum{EEPROM_NOT_INIT, EEPROM_ALREADY_INIT}eeprom_init_status_t;

/** Struct that holds information of each player of the hall */
typedef struct 
{
    uint32_t score;
    uint8_t * player_name;
    uint8_t index;
}hall_of_fame_players_t;



/*Return value of function that determines if the player is admitted to the hall of fame*/
typedef enum{ERROR = -1,NOT_ADMITTED , ADMITTED}submission_result_t;


/** For EEPROM formatting function, this allows a RESET (all scores at zero) and names section in blank.
 * Also, this prevents a misread of data
 */
typedef enum{ FORMAT_IMPOSSIBLE = -1, FORMAT_SUCCESS}memory_format_status_t;

/** brief this function initializes the EEPROM memory and  the I2C module
 */
void hall_of_fame_peripherals_init(void);

/**  brief  This function reads scores and names allocated in EEPROM and checks if the score of the player 
 *          deserves access to the hall. In this case, the hall is updated with player's name and score
 * 
 *  @param[in]   score              score of the player that won the match
 *  @param[in]   player_name        depending of this number (1 or 2), the function will read the name of the player from EEPROM
 */
submission_result_t hall_of_fame_submission( uint32_t score , uint8_t * player_name);


/** brief   For EEPROM formatting function, this allows a RESET (all scores at zero) and names section in blank.
 *          Also, this prevents a misread of data.
 */
void EEPROM_format(void);

/**
 *  brief Stores data from EEPROM in the received arrray
 *
 * @param array_for_scores
 * @param size_of_array_of_scores
 * @return
 */
struct_EEPROM_READ_status_t * scores_of_hall(uint32_t * array_for_scores, uint32_t size_of_array_of_scores);

/**
 * brief Stores data from EEPROM in the received array
 * @param array_for_names
 * @param size_of_array_of_names
 * @return
 */
EEPROM_READ_status_t names_of_hall(uint8_t *array_for_names, uint8_t size_of_array_of_names);

#endif /* HALL_OF_FAME_H_ */
