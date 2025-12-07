/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			14 / 04 / 2025


	List compatible MCU: 	ATmega328

	Description:			Common functions for ATmega328


	License:				This firmware is provided in a way
							free and without any warranty
							The firmware can be modified and
							unlimited copied.
							The author does not assume any
							liability for damage brought or
							caused by this firmware.

	Licenza:				Questo firmware viene fornito in modo
							gratuito e senza alcuna garanzia
							Il firmware può essere modificato e
							copiato senza limiti.
							L'autore non si assume nessuna
							responsabilità per danni portati o
							causati da questo firmware.

*****************************************************************/

#ifndef LIB_MCU_AT328_H_
#define LIB_MCU_AT328_H_

/*****************************************************************
 * 	DEFINE
*****************************************************************/
#define 	VALUE_EMULATE_WDT		255	//[mS] Max value = 255. The variable u8_decont_emulated_wdt is updated with this value. The interrupt decrement the variable u8_decont_emulated_wdt and when it reach 0 then it reset the MCU


//IO
#define 	STATUS_BTN0	 			bolean_pin_is_set(PINB,0) //

#define 	STATUS_SLEEP			bolean_pin_is_set(PIND,3)

#ifdef RFNANO
	#define 	PIN_CE_SET_ON			PORTB |= 0x01 //PB0 is the  Chip Enable (enable the NRF24L01)
	#define 	PIN_CE_SET_OFF			PORTB &= ~0x01
	#define 	STATUS_CE				bolean_pin_is_set(PINB,0)
#else
	#define 	PIN_CE_SET_ON			PORTB |= 0x02 //PB1 is the  Chip Enable (enable the NRF24L01)
	#define 	PIN_CE_SET_OFF			PORTB &= ~0x02
	#define 	STATUS_CE				bolean_pin_is_set(PINB,1)
#endif


#define 	PIN_CSN_SET_ON			PORTB |= 0x04 //PB2 is the  SPI Enable (enable the SPI on NRF24L01)
#define 	PIN_CSN_SET_OFF			PORTB &= ~0x04
#define 	STATUS_CSN				bolean_pin_is_set(PINB,2)

#define 	PIN_SCK_SET_ON			PORTB |= 0x20 //PB5 is the  SPI Clock
#define 	PIN_SCK_SET_OFF			PORTB &= ~0x20
#define 	STATUS_SCK				bolean_pin_is_set(PINB,5)

#define 	PIN_MOSI_SET_ON			PORTB |= 0x08 //PB3 is the  SPI MOSI
#define 	PIN_MOSI_SET_OFF		PORTB &= ~0x08
#define 	STATUS_MOSI				bolean_pin_is_set(PINB,3)

#define 	PIN_MISO_SET_ON			PORTB |= 0x10 //PB4 is the  SPI MISO
#define 	PIN_MISO_SET_OFF		PORTB &= ~0x10
#define 	STATUS_MISO				bolean_pin_is_set(PINB,4)


#define 	PIN_CD_SET_ON			PORTC |= 0x01 //PC0 is the CD where is connected the Led
#define 	PIN_CD_SET_OFF			PORTC &= ~0x01
#define 	STATUS_CD				bolean_pin_is_set(PINC,0)

//RESERVED
#define 	PD4_SET_ON				PORTD |= 0x10 //PD4
#define 	PD4_SET_OFF				PORTD &= ~0x10
#define 	STATUS_PD4				bolean_pin_is_set(PIND,4)
#define 	PD4_SET_AS_OUTPUT		DDRD |= 0x10
#define 	PD4_SET_AS_INPUT		DDRD &= ~0x10
#define 	PD4_ENABLE_PULLUP		PD4_SET_ON
#define 	PD4_DISABLE_PULLUP		PD4_SET_OFF

//PIN3
#define 	PIN3_SET_ON				PORTD |= 0x08 //PD3
#define 	PIN3_SET_OFF			PORTD &= ~0x08
#define 	STATUS_PIN3				bolean_pin_is_set(PIND,3)
#define 	PIN3_SET_AS_OUTPUT		DDRD |= 0x08
#define 	PIN3_SET_AS_INPUT		DDRD &= ~0x08
#define 	PIN3_ENABLE_PULLUP		PIN3_SET_ON
#define 	PIN3_DISABLE_PULLUP		PIN3_SET_OFF

//PIN4
#define 	PIN4_SET_ON				PORTD |= 0x01 //PD0
#define 	PIN4_SET_OFF			PORTD &= ~0x01
#define 	STATUS_PIN4				bolean_pin_is_set(PIND,0)
#define 	PIN4_SET_AS_OUTPUT		DDRD |= 0x01
#define 	PIN4_SET_AS_INPUT		DDRD &= ~0x01
#define 	PIN4_ENABLE_PULLUP		PIN4_SET_ON
#define 	PIN4_DISABLE_PULLUP		PIN4_SET_OFF

//PIN5
#define 	PIN5_SET_ON				PORTC |= 0x01 //PC0
#define 	PIN5_SET_OFF			PORTC &= ~0x01
#define 	STATUS_PIN5				bolean_pin_is_set(PINC,0)
#define 	PIN5_SET_AS_OUTPUT		DDRC |= 0x01
#define 	PIN5_SET_AS_INPUT		DDRC &= ~0x01
#define 	PIN5_ENABLE_PULLUP		PIN5_SET_ON
#define 	PIN5_DISABLE_PULLUP		PIN5_SET_OFF

//PIN6
#define 	PIN6_SET_ON				PORTD |= 0x40 //PD6
#define 	PIN6_SET_OFF			PORTD &= ~0x40
#define 	STATUS_PIN6				bolean_pin_is_set(PIND,6)
#define 	PIN6_SET_AS_OUTPUT		DDRD |= 0x40
#define 	PIN6_SET_AS_INPUT		DDRD &= ~0x40
#define 	PIN6_ENABLE_PULLUP		PIN6_SET_ON
#define 	PIN6_DISABLE_PULLUP		PIN6_SET_OFF

//PIN7
#define 	PIN7_SET_ON				PORTC |= 0x10 //PC4
#define 	PIN7_SET_OFF			PORTC &= ~0x10
#define 	STATUS_PIN7				bolean_pin_is_set(PINC,4)
#define 	PIN7_SET_AS_OUTPUT		DDRC |= 0x10
#define 	PIN7_SET_AS_INPUT		DDRC &= ~0x10
#define 	PIN7_ENABLE_PULLUP		PIN7_SET_ON
#define 	PIN7_DISABLE_PULLUP		PIN7_SET_OFF

//PIN8
#define 	PIN8_SET_ON				PORTD |= 0x02 //PD1
#define 	PIN8_SET_OFF			PORTD &= ~0x02
#define 	STATUS_PIN8				bolean_pin_is_set(PIND,1)
#define 	PIN8_SET_AS_OUTPUT		DDRD |= 0x02
#define 	PIN8_SET_AS_INPUT		DDRD &= ~0x02
#define 	PIN8_ENABLE_PULLUP		PIN8_SET_ON
#define 	PIN8_DISABLE_PULLUP		PIN8_SET_OFF

//PIN9
#define 	PIN9_SET_ON				PORTD |= 0x20 //PD5
#define 	PIN9_SET_OFF			PORTD &= ~0x20
#define 	STATUS_PIN9				bolean_pin_is_set(PIND,5)
#define 	PIN9_SET_AS_OUTPUT		DDRD |= 0x20
#define 	PIN9_SET_AS_INPUT		DDRD &= ~0x20
#define 	PIN9_ENABLE_PULLUP		PIN9_SET_ON
#define 	PIN9_DISABLE_PULLUP		PIN9_SET_OFF

//PIN10
#define 	PIN10_SET_ON			PORTC |= 0x20 //PC5
#define 	PIN10_SET_OFF			PORTC &= ~0x20
#define 	STATUS_PIN10			bolean_pin_is_set(PINC,5)
#define 	PIN10_SET_AS_OUTPUT		DDRC |= 0x20
#define 	PIN10_SET_AS_INPUT		DDRC &= ~0x20
#define 	PIN10_ENABLE_PULLUP		PIN10_SET_ON
#define 	PIN10_DISABLE_PULLUP	PIN10_SET_OFF



#define 	PIN_BUSY_SET_ON			set_Pin_BUSY() //PD6 is the pin that tell to the external MCU "I am doing something, thus just wait!"
#define 	PIN_BUSY_SET_OFF		clear_Pin_BUSY()
#define 	STATUS_BUSY				status_Pin_BUSY()


#define 	STATUS_RESET				bolean_pin_is_set(PIND,5) //PD5 is the virtual reset


#define 	PIN_ACTIVE_SET_ON		PORTC |= 0x10 //PC4 is a pin that state something
#define 	PIN_ACTIVE_SET_OFF		PORTC &= ~0x10
#define 	STATUS_ACTIVE			bolean_pin_is_set(PINC,4)

#define 	PIN_RESERVED_SET_ON		PORTC |= 0x20 //PC5 is a pin reserved
#define 	PIN_RESERVED_SET_OFF	PORTC &= ~0x20
#define 	STATUS_RESERVED			bolean_pin_is_set(PINC,5)

//DEFINIZIONI DEI PIN USATI PER LA PORTA SPI
#define PIN_SCK                   	PORTB5
#define PIN_MOSI                  	PORTB3
#define PIN_MISO                 	PORTB4	//non usato
#define PIN_SS                    	PORTB2

/*****************************************************************
 * 	PUBLIC MACRO
*****************************************************************/
#define REBOOT_MCU                  var_RESET_NOW=1

/*****************************************************************
 * 	PUBLIC VARIABLES
*****************************************************************/
extern volatile char var_pin_busy;
extern volatile unsigned char var_RESET_NOW;			//this variable is set to 1 when a function need the mcu to be restarted
extern volatile unsigned char u8_decont_emulated_wdt;	//into the main loop is updated with 0xFF (255mS) and when the main loop does not update the value then the iterrupt "ISR (TIMER1_COMPA_vect)" decrement the variable and at 0 it reset the MCU

/*****************************************************************
 * 	PRIVATE VARIABLES
*****************************************************************/

/*****************************************************************
 * 	PUBLIC FUCTIONS
*****************************************************************/
void		 	set_Pin_BUSY(void);
void 			clear_Pin_BUSY(void);
unsigned char 	status_Pin_BUSY(void);

//Funzioni ATMEGA328:
void 			init_mcu(void); 											//this init the IO, init external modules, check protections
unsigned char 	bolean_pin_is_set(unsigned char port, unsigned char pin);
void 			timer0_init(void); 											//inizializza i registri del Timer0
void 			init_Timer1(void); 											//this code sets up timer1
void 			wdt_init(void); 											//init the WDT to occur after 30mS
void 			CLR_WDT(void); 												//reset the Watch Dog Timer and allow to reboot the MCU when the public variable var_RESET_NOW=1
void			delay_ms_at328(unsigned int ms);							//delay in milli second
void 			reset_eeprom(void); 										//it reset the EEPROM, but not the code to control if it is original!

void 			init_10bit_adc(void);							//it init the ADC
unsigned int 	get_10bit_adc(unsigned char channel_adc);		//it return the value of the ADC

void 			goToSleep(void);	//under costruction 	//https://gist.github.com/JChristensen/5616922
/*****************************************************************
 * 	PRIVATE FUCTIONS
*****************************************************************/


#endif /* LIB_MCU_AT328_H_ */
