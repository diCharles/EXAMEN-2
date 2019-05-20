/*
 * tertris_sounds.c
 *
 *  Created on: 13/05/2019
 *      Author: Diego Charles and Ruben Charles
 *
 *      api for sound effects of game and tetriminos
 */


#include "tetris_sounds.h"

/** this flag indicates when a flag is being generated
 * is set only when play_tetri_sound_effect is called
 * is cleared only when  effect_timer_ISR is called by PIT1 callback*/
volatile uint8_t g_effect_flag = STOP_EFFECT ;

void play_tetri_sound_effect(uint16_t effect_frequency)
{
	/** initializing  flex timer channel 0
	 * and the pit2 only once*/
	static uint8_t init_only_once = FALSE;
	if(FALSE == init_only_once)
	{
		/* this is pin is conected to red anode  of external rgb led*/
		gpio_pin_control_register_t pin_control_register_bit_c_1 = GPIO_MUX4;/* setting alternative 4 of pin*/
		GPIO_clock_gating(GPIO_C);
		GPIO_pin_control_register(GPIO_C, bit_1, &pin_control_register_bit_c_1);/* PTC1 is CH0 output pin*/
		FlexTimer_Init(CHANNEL_0);
		set_PIT_timer_with_interrupt(PIT_0,F_CPU , EFFECT_DURATION_MS,
				PIT_CH0_IRQ, PRIORITY_9);
		PIT0_callback(effect_timer_ISR);
		init_only_once = TRUE;
	}
	FlexTimer_update_channel_value(CHANNEL_0,effect_frequency);
	g_effect_flag = PLAY_EFFECT;
}

void stop_tetri_sound_effect()
{
	/** setting channel value to zero, will make the output of the channel become zero*/
	FlexTimer_update_channel_value(CHANNEL_0,0);
}

void effect_timer_ISR(void)
{
	static volatile uint8_t counter = 0;
	counter ++;
	stop_tetri_sound_effect();
	/** counter gaurantees that effect wont be stoped before can be appreciated*/
	if((MINIMAL_DURATION_OF_EFFECT < counter) && (PLAY_EFFECT ==g_effect_flag) )
	{
		g_effect_flag = STOP_EFFECT;

		counter = 0 ;
	}
}
