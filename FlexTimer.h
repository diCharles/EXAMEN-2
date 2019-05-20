/*
 * FlexTimer.h
 *
 *  Created on: 25/03/2019
 *      Author: Diego Charles Suárez
 *      \brief
		This is the header file for the FlexTimer divice driver.
		Add configuration structures.
 */


#ifndef FLEXTIMER_H_
#define FLEXTIMER_H_


#include "MK64F12.h"

#define FLEX_TIMER_CLKS_0  (0U)
#define FLEX_TIMER_CLKS_1  (1U)
#define FLEX_TIMER_CLKS_2  (2U)
#define FLEX_TIMER_CLKS_3  (3U)


#define FLEX_TIMER_PS_1    (0U)
#define FLEX_TIMER_PS_2    (1U)
#define FLEX_TIMER_PS_4    (2U)
#define FLEX_TIMER_PS_8    (3U)
#define FLEX_TIMER_PS_16    (4U)
#define FLEX_TIMER_PS_32    (5U)
#define FLEX_TIMER_PS_64    (6U)
#define FLEX_TIMER_PS_128    (7U)


#define  FLEX_TIMER_DMA   0x01
#define  FLEX_TIMER_ELSA  0x04
#define  FLEX_TIMER_ELSB  0x08
#define  FLEX_TIMER_MSA   0x10
#define  FLEX_TIMER_MSB   0x20
#define  FLEX_TIMER_CHIE  0x40
#define  FLEX_TIMER_CHF   0x80

/*FLEX TIMER OPERATION MODES*/
typedef enum{
			OUTPUT_COMPARE,
			TIMER_OVERFLOW,
			PWM,
			INPUT_CAPTURE
			}flexTimer_modes_t;
/* FLEX TIMER CONFIGURATION OF OPERATION MODES*/
typedef enum{
			TOGGLE_OUTPUT_ON_MATCH,
			CLEAR_OUTPUT_ON_MATCH,
			SET_OUTPUT_ON_MATCH,
			SIMPLE_PWM,
			EGDE_ALIGENED_PWM,
			CENTER_ALIGNED_PWM,
			COMPLEMENTARY_PWM,
			SINGLE_EDGE_CAPTURE
			}flexTimer_mode_configurations_t;
/*FLEX TIMER CHANNELS*/
typedef enum{CHANNEL_0,
			CHANNEL_1,
			CHANNEL_2,
			CHANNEL_3
			}flexTimer_channels_t;

/* write directly Counter Value (CnV) register*/
void FlexTimer_update_channel_value(flexTimer_channels_t channel, int16_t channel_value);

/*brief for flex timer initialization in a specific mode from flexTimer_mode_t*/
/*channel argument still not implemented*/
void FlexTimer_Init(flexTimer_channels_t channel);

/*configures a flex timer channel  mode and configuration of the mode*/
void Fleximer_mode(flexTimer_channels_t channel, flexTimer_modes_t mode, flexTimer_mode_configurations_t);

/*selects clock source to given Flextimer channel*/
void FlexTimer_clockSource_and_prescaler(flexTimer_channels_t channel, uint8_t clockSource, uint8_t prescaler);

/*selects flexTimer channel  count modulo modifying FTMx->MOD */
void FlexTimer_mod(flexTimer_channels_t channel, uint16_t modulo);

/* helps programmer to select frequency of flextimer channel*/
/* The fixed frequency clock for each FTM is MCGFFCLK.*/
void FlexTimer_frecuency(flexTimer_channels_t channel, uint32_t freq);

/* sets PWM duty cycle for the desired channel*/
void FlexTimer_PWM_duty_cycle(flexTimer_channels_t channel,uint8_t dutyCycle);

#endif /* FLEXTIMER_H_ */
