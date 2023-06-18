/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			05 / 03 / 2023


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

/*****************************************************************
 * 	DEFINE
*****************************************************************/

/*****************************************************************
 * 	PUBLIC VARIABLES
*****************************************************************/
volatile char var_pin_busy;
volatile unsigned char var_RESET_NOW;			//this variable is set to 0 when a function need the mcu to be restarted
volatile unsigned char u8_decont_emulated_wdt;	//into the main loop is updated with 0xFF (255mS) and when the main loop does not update the value then the iterrupt "ISR (TIMER1_COMPA_vect)" decrement the variable and at 0 it reset the MCU

/*****************************************************************
 * 	PRIVATE VARIABLES
*****************************************************************/

/*****************************************************************
 * 	PUBLIC FUCTIONS
*****************************************************************/
void set_Pin_BUSY(void){
	if(sem_Used_As_Peripheral==1)
		var_pin_busy=1;
	else
		PORTD|=0x40;
}
void clear_Pin_BUSY(void){
	if(sem_Used_As_Peripheral==1)
		var_pin_busy=0;
	else
		PORTD &= ~0x40;
}
unsigned char status_Pin_BUSY(void){
	if(bit_is_set(PIND,6)==0)
		return 0;
	else
		return 1;
}



unsigned char bolean_pin_is_set(unsigned char port, unsigned char pin){
	unsigned char var_return = 0;
	if(bit_is_set(port,pin)>0){
		var_return = 1;
	}

	return var_return;
}


//inizializza i registri del Timer0
void timer0_init(void){
	//Enable Timer0 overflow Interrupt
	TIMSK0|=(1<<TOIE0);
	//Set Initial Timer value
	TCNT0=130;
	//Set prescaller 64 and start timer
	TCCR0B|=(1<<CS01)|(1<<CS00);
}


// this code sets up timer1
void init_Timer1(void){
	//https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328

	//OCR1A = 0x3D08; //2sec a 8MHz prescaler at 1024
	//OCR1A = 7812;	//1sec a 8MHz prescaler at 1024
	//OCR1A = 7812;	//250mS a 8MHz prescaler at 256
	//OCR1A = 7812;	//7,8125mS a 8MHz prescaler at 8
	//OCR1A = 0x000A;	//32uS a 8MHz prescaler at 8
	//OCR1A = 0x0018;	//50uS a 8MHz prescaler at 8
	//OCR1A = 1;	//16uS a 8MHz no prescaler
	//OCR1A = 0x0060;	//200uS a 8MHz prescaler at 8

	OCR1A = 2000;	//1mS a 16MHz prescaler at 8

	TCCR1B |= (1 << WGM12);  // Mode 4, CTC on OCR1A


    TIMSK1 |= (1 << OCIE1A); //Set interrupt on compare match


    //TCCR1B |= (1 << CS12) | (1 << CS10);  // set prescaler to 1024 and start the timer
	//TCCR1B |= (1 << CS12);  // set prescaler to 256 and start the timer
	TCCR1B |= (1 << CS11);  // set prescaler to 8 and start the timer
	//TCCR1B |= (1 << CS10);  // no prescaler
}


//init the WDT to occur after 60mS
void wdt_init(void) {
	//http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
	//wdt_enable(WDTO_60MS);
	wdt_enable(WDTO_30MS);
}


// -------------------------------------------------------------------------- //
// In this function you should do the following things:
//    - Set MISO pin input
//    - Set MOSI pin output
//    - Set SCK pin output
//    - Set CSN pin output
//    - Set CE pin output
// -------------------------------------------------------------------------- //
void nrf24_setupPins(void){

}

// --------------------------------------------------------------------------///
// nrf24 CE pin control function
//    - state:1 => Pin HIGH
//    - state:0 => Pin LOW    ///
// --------------------------------------------------------------------------///
void nrf24_ce_digitalWrite(uint8_t state){
	if(state>0){
		PIN_CE_SET_ON;
	}else{
		PIN_CE_SET_OFF;
	}
}

// --------------------------------------------------------------------------///
// nrf24 CE pin control function
//    - state:1 => Pin HIGH
//    - state:0 => Pin LOW    ///
// --------------------------------------------------------------------------///
void nrf24_csn_digitalWrite(uint8_t state){
	if(state>0){
		PIN_CSN_SET_ON;
	}else{
		PIN_CSN_SET_OFF;
	}
}

// --------------------------------------------------------------------------///
// nrf24 SCK pin control function
//    - state:1 => Pin HIGH
//    - state:0 => Pin LOW    ///
// --------------------------------------------------------------------------///
void nrf24_sck_digitalWrite(uint8_t state){
	if(state>0){
		PIN_SCK_SET_ON;
	}else{
		PIN_SCK_SET_OFF;
	}
}

// --------------------------------------------------------------------------///
// nrf24 MOSI pin control function
//    - state:1 => Pin HIGH
//    - state:0 => Pin LOW    ///
// --------------------------------------------------------------------------///
void nrf24_mosi_digitalWrite(uint8_t state){
	if(state>0){
		PIN_MOSI_SET_ON;
	}else{
		PIN_MOSI_SET_OFF;
	}
}

// --------------------------------------------------------------------------///
// nrf24 MISO pin read function
// - returns: Non-zero if the pin is high///
// --------------------------------------------------------------------------///
uint8_t nrf24_miso_digitalRead(void){

	return ((uint8_t)STATUS_MISO);
	//return 1;
}



//it reset the EEPROM, but not the code to control if it is original!
void reset_eeprom(void){
	unsigned char var_temp_byte;
	unsigned int eeprom_fw_version_rf_iotgemini;
	unsigned int i;

	#ifdef UART_DEBUG
		UART_DEBUG_send_STR(26, (unsigned char *)"BEGIN TO RESET EEPROM.....",1); //it send a string through the UART only if it is in debug mode
		#endif
		i=0;
		while(i<1023){
			wdt_reset(); //it reset the WDT
			if(i<START_ADDRESS_FREE || i>(START_ADDRESS_FREE+15)){
				wdt_reset(); //it reset the WDT
				if(		(i==START_ADDRESS_OUT_PIN_VALUE) //EEPROM valori a 0 per  l'ultimo valore che ha registrato per un output
					||	(i>=START_ADDRESS_TIMER_RELAY_VALUE && i<(START_ADDRESS_TIMER_RELAY_VALUE+4)) //EEPROM valori a 0 per funzione timer
					||	(i>=START_ADDRESS_INPUTS_DUTY_VALUE && i<(START_ADDRESS_INPUTS_DUTY_VALUE+8)) //EEPROM valori a 0 per funzione Input Duty
					){
					eeprom_write_byte ((unsigned char *)(i), 0x00);
				}else{
					eeprom_write_byte ((unsigned char *)(i), 0xFF);
				}
			}
			i++;
		}
		wdt_reset(); //it reset the WDT
		eeprom_fw_version_rf_iotgemini = FW_VERSION_RF_IOTGEMINI;
		var_temp_byte = eeprom_fw_version_rf_iotgemini >> 8;
		eeprom_write_byte ((unsigned char *)(START_ADDRESS_FW_VERSION), var_temp_byte);
		wdt_reset(); //it reset the WDT
		var_temp_byte = eeprom_fw_version_rf_iotgemini;
		eeprom_write_byte ((unsigned char *)(START_ADDRESS_FW_VERSION+1), var_temp_byte);
		#ifdef UART_DEBUG
		UART_DEBUG_send_STR(22, (unsigned char *)"EEPROM HAS BEEN RESET!",1); //it send a string through the UART only if is in debug mode
		#endif
}


//this init the IO, init external modules, check protections
void init_mcu(void){
	unsigned int eeprom_fw_version_rf_iotgemini;
	unsigned char var_temp_byte;
	unsigned char sem_init_as_peri_for_get_addresses=0;

	PORTB = 0x01; //pull-up resistor on PB0 for the button
	PORTD = 0x00; //no pull-up resistor
	PORTC = 0x00; //no pull-up resistor

	DDRB = 2+4+8+32; //PB1,PB2,PB3,PB5 as output //are for SPI communication
	DDRD = 128; //PD7 as output for the status led
	DDRC = 4; //PC2 output because it used for the protection, it send a signal and it would be received by PC2

	u8_decont_emulated_wdt = VALUE_EMULATE_WDT; //this variable has to be updated each time because at 0 the mcu would be rebooted

	#ifdef UART_DEBUG
	DDRD |= 2; //PD1 as output
	USART_Init_with_baudrate(USART_BAUDRATE); //used for debug. USART_BAUDRATE into myUsart328.h
	UART_DEBUG_send_STR2((unsigned char *)"STARTING.....",1); //it send a string through the UART only if is in debug mode
	#endif
	var_ID_TYPE_L = 100;
	var_ID_TYPE_H=0;
#ifdef ENABLE_SPECIAL_FUNCTIONS
	wdt_reset(); //it reset the WDT
	PD4_DISABLE_PULLUP;
	PD4_SET_AS_OUTPUT; //used into the function make_it_works_well()
	make_it_works_well();
	make_it_wonderful();
	wdt_reset(); //it reset the WDT
	check_if_that_is_good();
	wdt_reset(); //it reset the WDT
#endif

	//here reset the EEPROM if it find on the EEPROM another version of the firmware that is not this
	var_temp_byte = eeprom_read_byte((unsigned char *)(START_ADDRESS_FW_VERSION));
	eeprom_fw_version_rf_iotgemini = var_temp_byte << 8;
	var_temp_byte = eeprom_read_byte((unsigned char *)(START_ADDRESS_FW_VERSION+1));
	eeprom_fw_version_rf_iotgemini |= var_temp_byte;
	if(eeprom_fw_version_rf_iotgemini != FW_VERSION_RF_IOTGEMINI){
		//it reset the EEPROM, but not the code to control if it is original!
		reset_eeprom();
	}

	wdt_reset(); //it reset the WDT

	//here check if the network address is set. if this network address is set then it has been installed as peripheral
	if( (eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK)) != 0x00 && eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK)) != 0xFF)  	 ||
		(eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+1)) != 0x00 && eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+1)) != 0xFF)
		){
		sem_Used_As_Peripheral = 1;
		#ifdef UART_DEBUG
		UART_DEBUG_send_STR(34, (unsigned char *)"This device is set as pheripheral!",1); //it send a string through the UART only if is in debug mode
		#endif
	}else{
		sem_Used_As_Peripheral = 0;
		#ifdef UART_DEBUG
		UART_DEBUG_send_STR(34, (unsigned char *)"This device is set as transceiver!",1); //it send a string through the UART only if is in debug mode
		#endif
	}

	var_EEPROM_Semaphore = eeprom_read_byte((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES));

	//this function check if the reset is caused by the button,
	//if the it is caused by the button then check if the button is still pressed
	//if the button is not pressed then it start the installation into the IoT network
	//if the button is still pressed then wait until is release and check if the minimum time has been passed to reset addresses:
	//if the minimum time is passed then reset the all addresses to 0x00 and blink the led otherwise it start the installation into the IoT network
	sem_init_as_peri_for_get_addresses = check_btn_to_enter_in_programming(var_EEPROM_Semaphore);

	wdt_reset(); //it reset the WDT

	//Reserving memory for the struct
	current_settings_G3P = (struct_settings_G3P*) malloc(sizeof(struct_settings_G3P));

	wdt_reset(); //it reset the WDT

	var_RESET_NOW = 0; //at 1 it make the MCU to reset itself through the WDT

	#ifdef UART_DEBUG
	UART_DEBUG_send_STR(22, (unsigned char *)"Initializing G3P......",1); //it send a string through the UART only if is in debug mode
	#endif
	Init_G3P(); //It init all variables and send the setting to the NRF
	#ifdef UART_DEBUG
	//UART_DEBUG_send_STR(25, (unsigned char *)"Initializing timer0......",1); //it send a string through the UART only if is in debug mode
	#endif
	//timer0_init(); //inizializza i registri del Timer0
	#ifdef UART_DEBUG
	UART_DEBUG_send_STR(25, (unsigned char *)"Initializing timer1......",1); //it send a string through the UART only if is in debug mode
	#endif
	init_Timer1(); // this code sets up timer1
	#ifdef UART_DEBUG
	UART_DEBUG_send_STR(13, (unsigned char *)"WDT DISABLED!",1); //it send a string through the UART only if is in debug mode
	#else
	wdt_init(); //init the WDT to occur after 30mS
	#endif

	sei(); //Abilita gli interrupts globali

	#ifdef UART_DEBUG
	UART_DEBUG_send_STR(18, (unsigned char *)"Interrupt enabled!",1); //it send a string through the UART only if is in debug mode
	#endif

	if( sem_Used_As_Peripheral == 1){
		//used as peripheral then init the pinout in another way

		// ****************************************************************
		//				BEGIN: CONFIG FOR SHIELDS
		// ****************************************************************
			//Function that read the settings into the eeprom and then load the shields configurations
			manager_load_shields();

		// ****************************************************************
		//				BEGIN: CONFIG FOR SHIELDS
		// ****************************************************************

		//init the radio module
		init_radio_RF();	//function from rfpimcu.c library

	}else{
		//here init only as transceiver

		PORTD |= 0x08; //pull-up resistors on PD3 because is the SLEEP
		PORTD |= 0x20; //pull-up resistors on PD5 is the virtual RESET

		DDRD |= 2; //PD1 is an output because UART TX
		DDRD |= 0x40; //PD6 is output because is the BUSY signal
		DDRC |= 1+16+32; //PC0,PC4,PC5 are output because are CD,ACTIVE,RESERVED


		PIN_CD_SET_OFF;
		PIN_ACTIVE_SET_OFF;
		PIN_RESERVED_SET_OFF;

		//init INTERRUPT0 occur
		////EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
		//	EICRA |= (1 << ISC01);    // set INT0 to trigger on the falling edge
		//	EIMSK |= (1 << INT0);     // Turns on INT0

		PIN_BUSY_SET_OFF;
	}

	if(sem_init_as_peri_for_get_addresses==1){
		#ifdef UART_DEBUG
		UART_DEBUG_send_STR(33, (unsigned char *)"BEGIN TO SET PROGRAMMING NETWORK!",1); //it send a string through the UART only if is in debug mode
		#endif
		set_programming_network_Gateway();//set the programming network addresess
		decont_programming_mode_enabled = MAX_TIME_PROGRAMMING_MODE_ENABLED;
		programming_mode_enabled = 1;
		#ifdef UART_DEBUG
		UART_DEBUG_send_STR(24, (unsigned char *)"PROGRAMMING NETWORK SET!",1); //it send a string through the UART only if is in debug mode
		#endif
	}

	#ifdef UART_DEBUG
	//USART_Init_with_baudrate(USART_BAUDRATE); //used for debug. USART_BAUDRATE into myUsart328.h
	UART_DEBUG_send_STR2((unsigned char *)"BEGIN INFINITE LOOP!",1); //it send a string through the UART only if is in debug mode
	#endif
}


//it init the ADC
void init_10bit_adc(void){
	ADMUX = 0b00001111;//seleziona AREF, Right Adjust Result, and adc channel connected to 0V (GND)
	//ADMUX = 0b01001111;//seleziona AVcc con AREF collegato a massa con condensatore 100nF, Right Adjust Result, and adc channel connected to 0V (GND)

	//ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0); //prescaler adc is 32
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //prescaler adc is 128

	//ADCSRA = ADCSRA | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADATE); //prescaler adc is 32, ADATE enable auto trigger, trigger selected by bits ADTS in ADCSRB.
	//ADCSRB = ADCSRB | (1<<ADTS2) | (1<<ADTS0); //selectr Timer/Counter1 Compare Match

	// ADEN: Set to turn on ADC , by default it is turned off
	//ADPS2: ADPS2 and ADPS0 set to make division factor 32
	//ADMUX=0x00; //Set Vref to AREF, Left Adjust Result, and adc channel to PC0
}


//it return the value of the ADC
unsigned int get_10bit_adc(unsigned char channel_adc){
	volatile unsigned int value_adc=0;

	// Configure ADMUX
	//	b3:b0 - Set channel from 0 to 7
	if(channel_adc < 8){
		ADMUX &= 0xF0;
		ADMUX |= channel_adc;
	}

	_delay_us(25);

	ADCSRA |= (1<<ADSC); // Start conversion
	while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
	value_adc = ADCW; //Store ADC value

	return value_adc;
}


//https://gist.github.com/JChristensen/5616922
void goToSleep(void)//under costruction
{
    unsigned char adcsra = ADCSRA;          //save the ADC Control and Status Register A

    ADCSRA = 0;                    //disable the ADC
    EICRA = _BV(ISC01);            //configure INT0 to trigger on falling edge
    EIMSK = _BV(INT0);             //enable INT0
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();                         //stop interrupts to ensure the BOD timed sequence executes as required
    sleep_enable();
    //disable brown-out detection while sleeping (20-25�A)
    uint8_t mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);
    uint8_t mcucr2 = mcucr1 & ~_BV(BODSE);
    MCUCR = mcucr1;
    MCUCR = mcucr2;
    //sleep_bod_disable();           //for AVR-GCC 4.3.3 and later, this is equivalent to the previous 4 lines of code
    sei();                         //ensure interrupts enabled so we can wake up again
    sleep_cpu();                   //go to sleep
    sleep_disable();               //wake up here
    ADCSRA = adcsra;               //restore ADCSRA

	//this init the IO, init external modules, check protections
	init_mcu();
}



/*****************************************************************
 * 	PRIVATE FUCTIONS
*****************************************************************/



