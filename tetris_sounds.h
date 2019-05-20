/*
 * tetris_sounds.h
 *
 *  Created on: 13/05/2019
 *      Author: José Rodrigo
 */

#ifndef TETRIS_SOUNDS_H_
#define TETRIS_SOUNDS_H_


#include "Bits.h"
#include "FlexTimer.h"
#include "PIT.h"
#include "GPIO.h"

#define STOP_EFFECT     0U
#define PLAY_EFFECT     1U

#define F_CPU 210000000UL// K64 processor clock
#define EFFECT_DURATION_MS (0.010F)
#define MINIMAL_DURATION_OF_EFFECT   1U

/**
  brief : the called this function will generate an sound effect
  with a periodic square signal, with certain duration
  its recommend to add a capactor at output pin
 * */
void play_tetri_sound_effect(uint16_t effect_frequency);
/*
 * brief: this function will stop the current sound effect
 * */
void stop_tetri_sound_effect();

void effect_timer_ISR(void);
#endif /* TETRIS_SOUNDS_H_ */
