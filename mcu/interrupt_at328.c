/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			05 / 03 / 2023


	List compatible MCU: 	ATmega328

	Description:			Interrupt functions for ATmega328


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

//Interrupt Service Routine for INT0
ISR(INT0_vect){
	//not used
	//here for future development
}

//Interrupt Service Routine for USART
ISR(USART_RX_vect){
	decont_check_and_parse_functions_manager = MAX_DELAY_RUN_MANAGER_FUNCTION; //in questo modo sono sicuro che le funzioni eseguite periodicamente saranno eseguite in ritardo e non quando ricetrasmette!
	while(!(UCSR0A & (1<<RXC0)));
	if( sem_Used_As_Peripheral == 1){
	}else{
		//the data received is contained into the register UDR
		execute_USART_CMD(UDR0); //Giving the byte received, it will manage the array of data received
								 //and execute the commands. This has to be placed into the USART iterrupt routine
	}
}


//Funzione eseguita quando si scatena l'interrupt del Timer0.
//L'interrupt viene generato quando TCNT0 va in overflows.
//in questo programma va in overflow con una cadenza di 1mS
//Interrupt Service Routine for Timer0
//this interrupt is generate when TCNT0 goes in overflows
/*ISR(TIMER0_OVF_vect){ //Timer0 Overflow ISR
	TCNT0 += 130; //permette la cadenza di 1mS
	//here some functions that needs to be run every 1mS
*/

ISR (TIMER1_COMPA_vect){
	if( (var_RESET_NOW == 0) && (u8_decont_emulated_wdt > 0) ){
		wdt_reset(); //it reset the WDT
		u8_decont_emulated_wdt--;
	}

	if( sem_Used_As_Peripheral == 1 || programming_mode_enabled==1){
		//here some functions that needs to be run every 1mS
		interrupt_timer_of_1mS_RFPI();
		check_and_parse_functions_manager_shield_every_1mS();	//here some functions that needs to be run every 1mS
#ifdef ENABLE_SPECIAL_FUNCTIONS
		if(sem_Used_As_Peripheral == 1){
			make_it_works_well();
		}
#endif
	}else{
		Controller_USART_CMD_G3P();	//controller of the command received
									//this has to be into the interrupt routine of 1mS
		controller_activity_manager_RX_NRF24();		//this will end the activity started into the manager_RX_NRF24()
	}

	#ifdef UART_DEBUG
		cont1_debug++;
	#endif
}
