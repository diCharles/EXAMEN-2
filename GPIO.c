/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author Diego Charles Suarez
	\date	18/02/2019
	\todo
	    Interrupts are not implemented in this API implementation.
 */
#include "MK64F12.h"
#include "GPIO.h"



static gpio_interrupt_flags_t g_intr_status_flag = {0};

/* fptrs are init with a void fuction that does nothing (dummy_function)*/
void (*g_callback_PortA_fptr)(void)=0;
void (*g_callback_PortB_fptr)(void)=0;
void (*g_callback_PortC_fptr)(void)=0;
void (*g_callback_PortD_fptr)(void)=0;
void (*g_callback_PortE_fptr)(void)=0;

void PORTA_IRQ_Callback(void (*fptr)(void))
{
	g_callback_PortA_fptr=fptr;
}
void PORTB_IRQ_Callback(void (*fptr)(void))
{
	g_callback_PortB_fptr=fptr;
}
void PORTC_IRQ_Callback(void (*fptr)(void))
{
	g_callback_PortC_fptr=fptr;
}
void PORTD_IRQ_Callback(void (*fptr)(void))
{
	g_callback_PortD_fptr=fptr;
}
void PORTE_IRQ_Callback(void (*fptr)(void))
{
	g_callback_PortE_fptr=fptr;
}


void PORTA_IRQHandler(void)
{

	g_intr_status_flag.flag_port_a = TRUE;
	if(NULL_PTR != g_callback_PortA_fptr)
	{
		g_callback_PortA_fptr();
	}
	GPIO_clear_interrupt(GPIO_A);
}
void PORTB_IRQHandler(void)
{

	g_intr_status_flag.flag_port_b = TRUE;
	if(NULL_PTR != g_callback_PortB_fptr)
	{
		g_callback_PortB_fptr();
	}
	GPIO_clear_interrupt(GPIO_B);
}

void PORTC_IRQHandler(void)
{
	g_intr_status_flag.flag_port_c = TRUE;
	if(NULL_PTR != g_callback_PortC_fptr)
	{
		g_callback_PortC_fptr();
	}
	g_callback_PortC_fptr();
	GPIO_clear_interrupt(GPIO_C);
}
void PORTD_IRQHandler(void)
{
	g_intr_status_flag.flag_port_d = TRUE;
	if(NULL_PTR !=g_callback_PortD_fptr)
	{
		g_callback_PortD_fptr();
	}
	GPIO_clear_interrupt(GPIO_D);
}
void PORTE_IRQHandler(void)
{
	g_intr_status_flag.flag_port_e = TRUE;
	g_callback_PortE_fptr();
	GPIO_clear_interrupt(GPIO_E);
}



void GPIO_clear_irq_status(gpio_port_name_t gpio)
{
	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = FALSE;
	}
	else
	{
		g_intr_status_flag.flag_port_c = FALSE;
	}
}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio)
{
	uint8_t status = 0;

	if(GPIO_A == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	else
	{
		status = g_intr_status_flag.flag_port_c;
	}

	return(status);
}

void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}
bool_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

bool_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pin_control_register)

{

	switch(port_name)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pin_control_register;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pin_control_register;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pin_control_register;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pin_control_register;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pin_control_register;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_data_direction_port(gpio_port_name_t portName ,gpio_port_direction_t direction)
{
	switch(portName)/** */
						{
							case GPIO_A: /** */
								if(GPIO_INPUT)
								{
									GPIOA->PDDR= 0x0000;
								}
								else//direction equals GPIO_OUTPUT
								{
									GPIOA->PDDR= 0xFFFF;
								}
								break;
							case GPIO_B: /** */
								if(GPIO_INPUT)
								{
									GPIOB->PDDR= 0x0000;
								}
								else//direction equals GPIO_OUTPUT
								{
									GPIOB->PDDR= 0xFFFF;
								}
								break;
							case GPIO_C: /** */
								if(GPIO_INPUT)
								{
									GPIOC->PDDR= 0x0000;
								}
								else//direction equals GPIO_OUTPUT
								{
									GPIOC->PDDR= 0xFFFF;
								}
								break;
							case GPIO_D: /** */
								if(GPIO_INPUT)
								{
									GPIOD->PDDR= 0x0000;
								}
								else//direction equals GPIO_OUTPUT
								{
									GPIOD->PDDR= 0xFFFF;
								}
								break;
							case GPIO_E: /** */
								if(GPIO_INPUT)
								{
									GPIOE->PDDR= 0x0000;
								}
								else//direction equals GPIO_OUTPUT
								{
									GPIOE->PDDR= 0xFFFF;
									}
								break;
							default: /**configure nothing*/
								return ;
						}// end switch
}

void GPIO_data_direction_pin(gpio_port_name_t portName, gpio_pin_direction_t pin_direction, uint8_t pin)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
						{
							case GPIO_A: /** */
								if(GPIO_PIN_INPUT == pin_direction)
								{
									GPIOA->PDDR &=   ~(0x0001 << pin);
								}
								else//GPIO_PIN_OUTPUT == pin_direction
								{
									GPIOA->PDDR |= (  0x0001) << pin;
								}
								break;
							case GPIO_B: /** */
								if(GPIO_PIN_INPUT == pin_direction)
								{
									GPIOB->PDDR &=   ~(0x0001 << pin);
								}
								else//GPIO_PIN_OUTPUT == pin_direction
								{
									GPIOB->PDDR |= (  0x0001) << pin;
								}
								break;
							case GPIO_C: /** */
								if(GPIO_PIN_INPUT == pin_direction)
								{
									GPIOC->PDDR &=   ~(0x0001 << pin);
								}
								else//GPIO_PIN_OUTPUT == pin_direction
								{
									GPIOC->PDDR |= (  0x0001) << pin;
								}
								break;
							case GPIO_D: /** */
								if(GPIO_PIN_INPUT == pin_direction)
								{
									GPIOD->PDDR &=   ~(0x0001 << pin);
								}
								else//GPIO_PIN_OUTPUT == pin_direction
								{
									GPIOD->PDDR |= (  0x0001) << pin;
								}
								break;
							case GPIO_E: /** */
								if(GPIO_PIN_INPUT == pin_direction)
								{
									GPIOE->PDDR &=   ~(0x0001 << pin);
								}
								else//GPIO_PIN_OUTPUT == pin_direction
								{
									GPIOE->PDDR |= (  0x0001) << pin;
								}
								break;
							default: /**no pin is set */
								return ;
						}// end switch
}

uint32_t GPIO_read_port(gpio_port_name_t portName)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
					{
						case GPIO_A: /** GPIO A is written*/
							return GPIOA->PDIR;
							break;
						case GPIO_B: /** GPIO B is  written*/
							return GPIOB->PDIR;
							break;
						case GPIO_C: /** GPIO C is written*/
							return GPIOC->PDIR;
							break;
						case GPIO_D: /** GPIO D is  written*/
							return GPIOD->PDIR;
							break;
						case GPIO_E: /** GPIO E is written*/
							return GPIOA->PDIR;
							break;
						default: /**If doesn't exist the option*/
							return 0;
					}// end switch

	return 0;
}

bool_t GPIO_read_pin(gpio_port_name_t portName, uint8_t pin)
{
	uint32_t input_value=0x0000;
	switch(portName)/** Selecting the GPIO for write him a value*/
					{
						case GPIO_A: /** GPIO A is written*/
							input_value= GPIOA->PDIR;
							input_value= input_value & (0x0001<<pin);
							if(0 != input_value)
							{
							return TRUE;
							}
							else
							{
							return FALSE;
							}
							break;
						case GPIO_B: /** GPIO B is  written*/
							input_value= GPIOB->PDIR;
							input_value= input_value & (0x0001<<pin);
							if(0 != input_value)
							{
							return TRUE;
							}
							else
							{
								return FALSE;
							}
							break;
						case GPIO_C: /** GPIO C is written*/
							input_value= GPIOC->PDIR;
							input_value= input_value & (0x00000001<<pin);
							if(0 != input_value)
							{
							return TRUE;
							}
							else
							{
								return FALSE;
							}
							break;
						case GPIO_D: /** GPIO D is  written*/
							input_value= GPIOD->PDIR;
							input_value= input_value & (0x0001<<pin);
							if(0 != input_value)
							{
								return TRUE;
							}
							else
							{
								return FALSE;
							}
							break;
						case GPIO_E: /** GPIO E is written*/
							input_value= GPIOC->PDIR;
							input_value= input_value & (0x0001<<pin);
							if(0 != input_value)
							{
								return TRUE;
							}
							else
							{
								return FALSE;
							}
							break;
						default: /**If doesn't exist the option*/
							return FALSE;
					}// end switch
}

void GPIO_write_port(gpio_port_name_t portName, uint32_t data)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
				{
					case GPIO_A: /** GPIO A is written*/
						GPIOA->PDOR =data;
						break;
					case GPIO_B: /** GPIO B is  written*/
						GPIOB->PDOR =data;
						break;
					case GPIO_C: /** GPIO C is written*/
						GPIOC->PDOR =data;
						break;
					case GPIO_D: /** GPIO D is  written*/
						GPIOD->PDOR =data;
						break;
					case GPIO_E: /** GPIO E is written*/
						GPIOE->PDOR =data;
						break;
					default: /**If doesn't exist the option*/
						return;
				}// end switch
}


void GPIO_set_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
					{
						case GPIO_A: /** GPIO A is written*/
							GPIOA->PSOR= 0x0001<<pin;
							break;
						case GPIO_B: /** GPIO B is  written*/
							GPIOB->PSOR= 0x0001<<pin;
							break;
						case GPIO_C: /** GPIO C is written*/
							GPIOC->PSOR= 0x0001<<pin;
							break;
						case GPIO_D: /** GPIO D is  written*/
							GPIOD->PSOR= 0x0001<<pin;
							break;
						case GPIO_E: /** GPIO E is written*/
							GPIOE->PSOR= 0x0001<<pin;
							break;
						default: /**If doesn't exist the option set nothing*/
							return ;
					}// end switch
}

void GPIO_clear_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
						{
							case GPIO_A: /** */
								GPIOA->PCOR= 0x0001<<pin;
								break;
							case GPIO_B: /** */
								GPIOB->PCOR= 0x0001<<pin;
								break;
							case GPIO_C: /** */
								GPIOC->PCOR= 0x0001<<pin;
								break;
							case GPIO_D: /** */
								GPIOD->PCOR= 0x0001<<pin;
								break;
							case GPIO_E: /** */
								GPIOE->PCOR= 0x0001<<pin;
								break;
							default: /**If doesn't exist the option: clear nothing*/
								return ;
						}// end switch
}

void GPIO_toogle_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)/** Selecting the GPIO for write him a value*/
						{
							case GPIO_A: /** */
								GPIOA->PTOR= 0x0001<<pin;
								break;
							case GPIO_B: /** */
								GPIOB->PTOR= 0x0001<<pin;
								break;
							case GPIO_C: /** */
								GPIOC->PTOR= 0x0001<<pin;
								break;
							case GPIO_D: /** */
								GPIOD->PTOR= 0x0001<<pin;
								break;
							case GPIO_E: /** */
								GPIOE->PTOR= 0x0001<<pin;
								break;
							default: /**If doesn't exist the option: clear nothing*/
								return ;
						}// end switch
}







