/*
 * FlexTimer.c
 *
 *  Created on: 25/03/2019
 *      Author: Diego Charles and Ruben Charles
 *     brief: implementation of functions in FlexTimer.h
 */
#include "FlexTimer.h"

void FlexTimer_update_channel_value(flexTimer_channels_t channel,int16_t channel_value)
{
	/**Assigns a new value for the duty cycle*/
	if(CHANNEL_0 == channel)
	{
		FTM0->CONTROLS[0].CnV = channel_value;
	}
	else if(CHANNEL_1 == channel)
	{
		FTM0->CONTROLS[1].CnV = channel_value;
	}
	else if(CHANNEL_2 == channel)
	{
		FTM0->CONTROLS[2].CnV = channel_value;
	}
	else if(CHANNEL_3 == channel)
	{
		FTM0->CONTROLS[3].CnV = channel_value;
	}
}


void FlexTimer_Init(flexTimer_channels_t channel)
{

	/**Clock gating for FlexTimers*/
	SIM->SCGC6 |= SIM_SCGC6_FTM0(1);

	/**Selects the FTM behavior in BDM mode.In this case in functional mode pag 1076*/
	FTM0->CONF |= FTM_CONF_BDMMODE(3);

	/*select flexTimer channel mode and configuration*/
	Fleximer_mode(CHANNEL_0,PWM ,SIMPLE_PWM);
	Fleximer_mode(CHANNEL_1,PWM ,SIMPLE_PWM);
	Fleximer_mode(CHANNEL_2,PWM ,SIMPLE_PWM);
	FlexTimer_mod(CHANNEL_0, 255);

	/* by default the flextimer channel is runnig with output equal to zero*/
	FlexTimer_update_channel_value(CHANNEL_0, 0);
	FlexTimer_update_channel_value(CHANNEL_1, 0);
	FlexTimer_update_channel_value(CHANNEL_2, 0);
	/*setting finally clock source*/
	FlexTimer_clockSource_and_prescaler(CHANNEL_0,FLEX_TIMER_CLKS_1,FLEX_TIMER_PS_128);


}
void Fleximer_mode(flexTimer_channels_t channel, flexTimer_modes_t mode, flexTimer_mode_configurations_t config)
{

	switch(mode)
	{
	case OUTPUT_COMPARE:
		/**It enable the FTM*/
		FTM0->MODE |= FTM_MODE_FTMEN_MASK;


		/* setting  output compare configuration*/
		if( TOGGLE_OUTPUT_ON_MATCH== config)
			/**Configure FlexTimer in output compare in toggle mode*/
			FTM0->CONTROLS[0].CnSC |= FTM0->CONTROLS[0].CnSC |= FTM_CnSC_MSA(1)| FTM_CnSC_ELSA(1);
		else if(CLEAR_OUTPUT_ON_MATCH== config)
		{
			/**Configure FlexTimer in output compare in clear output on match*/
			FTM0->CONTROLS[0].CnSC |=   FTM_CnSC_MSA(1) |FTM_CnSC_ELSB(1);
			FTM0->CONTROLS[0].CnSC &=~(FTM_CnSC_ELSA(1));
		}
		else if(SET_OUTPUT_ON_MATCH== config)
			/**Configure FlexTimer in output compare  in set output on match*/
			FTM0->CONTROLS[0].CnSC |=   FTM_CnSC_MSA(1) |FTM_CnSC_ELSB(1) | FTM_CnSC_ELSA(1);
		break;

	case TIMER_OVERFLOW:
		FTM0->MODE |= FTM_MODE_FTMEN_MASK;
		/**Selects the FTM behavior in BDM mode.In this case in functional mode*/
		FTM0->CONF |= FTM_CONF_BDMMODE(3);
		/**Enabling the interrupt*/
		FTM0->SC |=  FTM_SC_TOIE(1);
		break;
	case PWM		   :
		if( SIMPLE_PWM== config)
		{
			/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
			 * When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
			if( CHANNEL_0== channel)
			{
				FTM0->MODE |= FTM_MODE_WPDIS_MASK;/**Enables the writing over all registers*/
				FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
				FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);/**Selects the Edge-Aligned PWM mode mode*/
			}
			else if(CHANNEL_1== channel)
			{
				FTM0->MODE |= FTM_MODE_WPDIS_MASK;/**Enables the writing over all registers*/
				FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
				FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);/**Selects the Edge-Aligned PWM mode mode*/
			}
			else if(CHANNEL_2== channel)
			{
				FTM0->MODE |= FTM_MODE_WPDIS_MASK;/**Enables the writing over all registers*/
				FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
				FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);/**Selects the Edge-Aligned PWM mode mode*/
			}
			else if(CHANNEL_3== channel)
			{
				FTM0->MODE |= FTM_MODE_WPDIS_MASK;/**Enables the writing over all registers*/
				FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
				FTM0->CONTROLS[3].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);/**Selects the Edge-Aligned PWM mode mode*/
			}

		}
		else if( CENTER_ALIGNED_PWM == config)
		{
			SIM->SCGC6|=0x03000000; //enable FTM0 and FTM0 module clock
			SIM->SCGC5 = SIM->SCGC5|0x3E00; //enable port A/B/C/D/E clock
			FTM0->CONF = 0xC0; //set up BDM in 11
			FTM0->FMS = 0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE reg
			FTM0->MODE|=0x05; //enable write the FTM CnV register
			FTM0->MOD = 1000;
			FTM0->CONTROLS[0].CnSC = 0x28;//         --------------------------------------->FTM0_C0SC=0x28; ////center-alignment, PWM begins with High
			FTM0->CONTROLS[1].CnSC = 0x28;//          --------------------------------------->FTM0->C1SC=0x28; //PWM waveform is high-low-high
			FTM0->COMBINE = 0x02; //complementary mode for CH0&CH1 of FTM0
			FTM0->COMBINE|= 0x10; // dead timer insertion enabled in complementary mode for //CH0&CH1 of FTM0
			FTM0->DEADTIME = 0x1F; //dead time is 16 system clock cycles
			FTM0->CONTROLS[1].CnV = 500;//               ------------------------------------>FTM0->C1V=500;
			FTM0->CONTROLS[0].CnV = 500;//           --------------------------------------->FTM0->C0V=500;
			FTM0->CNTIN = 0x00;
			FTM0->CONTROLS[2].CnSC=0x28;//         --------------------------------------->FTM0->C2SC=0x28;
			FTM0->CONTROLS[3].CnSC=0x28;//               --------------------------------------->FTM0->C3SC=0x28;
			FTM0->COMBINE|=0x0200;
			FTM0->COMBINE|=0x1000;
			FTM0->CONTROLS[3].CnV=250;// ------------------------------------------------->FTM0->C3V=250;
			FTM0->CONTROLS[2].CnV=250;//--------------------------------------------------->FTM0->C2V=250;
			FTM0->SC=0x68;
		}
		else if( COMPLEMENTARY_PWM == config)
		{

		}
		break;
	case INPUT_CAPTURE:
		if( SINGLE_EDGE_CAPTURE == config)
		{
			//enable FTM0 clock
			SIM->SCGC6|=0x03000000; //enable FTM0 and FTM0 module clock
			SIM->SCGC5=SIM->SCGC5|0x3E00; //enable port A/B/C/D/E clock
			FTM0->SC=0x00;
			/* capture on rising edge onl bit ElSA in CnSCy*/
			FTM0->CONTROLS[0].CnSC=0x04;/*old line from app note FTM0->C0SC|=0x04;*/
			FTM0->COMBINE=0x00; //clear
			//enable capture interrupt bit CHIE in CnsC pag 965
			FTM0->CONTROLS[0].CnSC=0x40;//  old line for app note----->>FTM0->SC[0]|=0x40; //enable CH0 interrupt
			FTM0->SC|=0x08;
			//in ISR of capture interrupt, read the FTM_c0V register to get the capture value
		}


		break;

	}

}
void FlexTimer_clockSource_and_prescaler(flexTimer_channels_t channel, uint8_t clockSource, uint8_t prescaler )
{
	/* for  SC register descrption view page 961 */
	/* clock source selction CLKS
	 * 00 No clock selected. This in effect disables the FTM counter.
	  01 System clock
	  10 Fixed frequency clock
	  11 External clock*/
	/* prescaler selection PS
	 * 000 Divide by 1
		001 Divide by 2
		010 Divide by 4
		011 Divide by 8
		100 Divide by 16
		101 Divide by 32
		110 Divide by 64
		111 Divide by 128*/


	switch(channel)
	{
	case CHANNEL_0:
		FTM0->SC |= FTM_SC_CLKS(clockSource)| FTM_SC_PS(prescaler);
		break;
	case CHANNEL_1:
		FTM1->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
		break;
	case CHANNEL_2:
		FTM2->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
		break;
	case CHANNEL_3:
		FTM3->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
		break;
	default:
		break;
	}


}
void FlexTimer_mod(flexTimer_channels_t channel, uint16_t modulo)
{
	switch(channel)
	{
	case CHANNEL_0:
		FTM0->MOD=modulo;
		break;
	case CHANNEL_1:
		FTM1->MOD=modulo;
		break;
	case CHANNEL_2:
		FTM2->MOD=modulo;
		break;
	default:
		break;
	}
}


void FTM0_ISR(void)
{
	if(FTM0->STATUS&0x01)
	{
		if(FTM0->STATUS&0x01)
		{

			//tempPrev=temp;
			FTM0->CONTROLS[0].CnV;//----------------------------------->temp=FTM0_C0V; //read FTM0 counter register
			//diff=temp-tempPrev;
			//FTM0_STATUS&=0xFE;
			//read Hall sensor logic
			//The FTM0_CH0 channel is multiplexed with GPIOC1, read GPIOC1 logic

			//var0=GPIOC_PDIR;
			//GPIOC_PTOR=0x80;
			asm("nop"); //set a break point here

		}
	}


}
