/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			It is the manager to load the 
							configurations of the shields
	
	
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




//Function that read the settings into the eeprom and then load the shields configurations
void 	manager_load_shields(void){
	
	unsigned char i;
	
	//init ADC because ADC1 is always used to get MCU Volts
	init_10bit_adc(); //it init the ADC
	
	
	//resetting gloabal variables
	pointer_struct_DHT11 = 0;
	
	
	//timer
	cont_Timer_Manager_Shield_HH = 0;
	cont_Timer_Manager_Shield_MM = 0;
	cont_Timer_Manager_Shield_SS = 0;
	cont_Timer_Manager_Shield_ms = 0;
	cont_Timer_Manager_Shield_10ms = 0;

	//used to timer an output
	decont_semRelayTimer = 0;
	decont_RelayTimer_10ms = 0;
	decont_RelayTimer_SS = 0;
	decont_RelayTimer_MM = 0;
	decont_RelayTimer_HH = 0;
	last_cont_Timer_Manager_Shield_SS = 0;
	
	
	//pin functions
	root_pin_functions = (struct pin_functions*)malloc(sizeof(struct pin_functions));
	root_pin_functions->PIN_USED = 0;
	root_pin_functions->PIN_MASK = 0;
	//root_pin_functions->PIN_STATUS = (unsigned char)eeprom_read_byte(START_ADDRESS_OUT_PIN_VALUE);
	root_pin_functions->PIN_STATUS = (unsigned char)eeprom_read_byte((unsigned char *)START_ADDRESS_OUT_PIN_VALUE);
	root_pin_functions->SEM_ADC = 0;
	root_pin_functions->SEM_PWM = 0;
	root_pin_functions->SEM_UART = 0;
	root_pin_functions->SEM_I2C = 0;
	root_pin_functions->SEM_DHT11 = 0;
	
	//counter
	decont_check_and_parse_functions_manager = 0;
	
	//various
	ID_Function_Manager_to_Run = 0;
	
	
	//variables used into lib RFPIMCU
	var_NUM_DIGITAL_INPUT = 0;
	var_NUM_DIGITAL_OUTPUT = 0;
	var_NUM_ANALOGUE_INPUT = 1; //this 1 is for the virtual pin11 that is used to send the ADC1 that for calculating MCU Volts
	var_NUM_ANALOGUE_OUTPUT = 0;
	var_NUM_FUNCTIONS = 3; //+1 for fuction settings. +1 for timer fuction. +1 for input duty.
	sem_Led_TX_keep_OFF = (unsigned char)eeprom_read_byte((unsigned char *)START_ADDRESS_SETTINGS_PLATFORM);
	
	
	//variables used for the functions into RFPIMCU, function Timer
	semRelayTimer = (unsigned char)eeprom_read_byte((unsigned char *)START_ADDRESS_TIMER_RELAY_VALUE);
	RelayTimer_10ms = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE_10MS));
	RelayTimer_SS = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+1));
	RelayTimer_MM = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+2));
	RelayTimer_HH = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+3));
	
	

	for(i=0;i<10;i++){
		//variables used for the functions into RFPIMCU, function Inputs Duty
		fun_input_ctrl_output[i] = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_INPUTS_DUTY_VALUE+i));

		//variables used for the functions into RFPIMCU, function control output with THRESHOLD HIGH and LOW
		fun_threshold_ctrl_output[i] = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_THRESHOLD_VALUES+i));
	}


	//this variables make to update the eeprom into the function check_and_parse_functions_manager()
	id_data_eeprom_to_update = 0;

	
		
	//creating space in RAM and loading data from the EEPROM
	root_struct_shields = (struct manager_shield*)malloc(sizeof(struct manager_shield));
	root_struct_shields->root_settings_shield = 0;
	
//	root_struct_shields->num_shield_linked = eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD-1));
//	if( root_struct_shields->num_shield_linked == 0xFF || root_struct_shields->num_shield_linked == 0x00){
//		root_struct_shields->num_shield_linked = 0; //no shields linked
//	}else{
//		for (int i=0; i<root_struct_shields->num_shield_linked; i++ ){


	unsigned int tempID = 0;
	//unsigned char tempByteEEPROM = 0;
		for (i=0; i<MAX_NUM_SHIELDS_CONNECTABLE; i++ ){ //
		
			#ifdef UART_DEBUG_MANAGER_BOOT
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"******* LOADING SHIELD FROM POSITION ",1); //it send a string through the UART only if is in debug mode
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)i, 0);
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)" EEPROM *******",0);
			#endif
		
			tempID = 0;
			//for first it check if the ID is an valid ID otherwise skip
			tempID = eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
			
//			#ifdef UART_DEBUG_MANAGER_BOOT
//			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"     ADDRESS=",1);
//			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)), 0);
//			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"     DATA=",0);
//			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)tempID, 0);
//			#endif
			
			tempID = tempID << 8;
			tempID |= eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+1)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
			
//			#ifdef UART_DEBUG_MANAGER_BOOT
//			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"     ADDRESS=",1);
//			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+1)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)), 0);
//			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"     DATA=",0);
//			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)tempID, 0);
//			#endif
			
			if(tempID != 0 && tempID != 0xFFFF && tempID <= MAX_ID_EXISTING_SHIELDS){ //it means the ID is valid thus it goes to read all data from the eeprom
			
				if(root_struct_shields->root_settings_shield == 0){
					root_struct_shields->root_settings_shield = (struct settings_shield*)malloc(sizeof(struct settings_shield));
					current_settings_shield = root_struct_shields->root_settings_shield;
					current_settings_shield->next=0;
					
				}else{
					current_settings_shield->next = (struct settings_shield*)malloc(sizeof(struct settings_shield));
					current_settings_shield = current_settings_shield->next;
					current_settings_shield->next=0;
				}
				//current_settings_shield->ID = eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
				//current_settings_shield->ID = current_settings_shield->ID << 8;
				//current_settings_shield->ID |= eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+1)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
				current_settings_shield->ID = tempID;
				
				#ifdef UART_DEBUG_MANAGER_BOOT
				UART_DEBUG_MANAGER_send_STR2((unsigned char *)"      ID SHIELD LOADED: ",1);
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)current_settings_shield->ID, 0);
				#endif
				
				current_settings_shield->PIN_USED = (unsigned char)eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+2)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
				current_settings_shield->PIN_MASK = (unsigned char)eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+3)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
				current_settings_shield->PULL_UP = (unsigned char)eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+4)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
				
				
				#ifdef UART_DEBUG_MANAGER_BOOT
				UART_DEBUG_MANAGER_send_STR2((unsigned char *)"      PIN_USED=",1); //it send a string through the UART only if is in debug mode
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)current_settings_shield->PIN_USED, 0);
				UART_DEBUG_MANAGER_send_STR2((unsigned char *)"      PIN_MASK=",1); //it send a string through the UART only if is in debug mode
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)current_settings_shield->PIN_MASK, 0);
				UART_DEBUG_MANAGER_send_STR2((unsigned char *)"      PULL_UP=",1); //it send a string through the UART only if is in debug mode
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)current_settings_shield->PULL_UP, 0);
				#endif
				
				
				for(int j=0; j<7; j++){
					current_settings_shield->array_ID_Functions[j] = (unsigned char)eeprom_read_byte((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+5+j)+(i*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)));
					if(current_settings_shield->array_ID_Functions[j]==0xFF) current_settings_shield->array_ID_Functions[j] = 0;
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"      ID_FUNCTION",1); //it send a string through the UART only if is in debug mode
					UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)j, 0);
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"=",0);
					UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)current_settings_shield->array_ID_Functions[j], 0);
					#endif
				
				}				
			}
			
			#ifdef UART_DEBUG_MANAGER_BOOT
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*",1);
			UART_DEBUG_send_STARS(74);
			#endif
			
		}
	//}
	
	
	
	
	#ifdef UART_DEBUG_MANAGER_BOOT
	UART_DEBUG_MANAGER_send_STR2((unsigned char *)" Configuring pinout:\n",1); //it send a string through the UART only if is in debug mode
	#endif
	
	//configuring GPIO
	// PIN3 = PD3		PIN_USED=0b00000001
	// PIN4 = PD0		PIN_USED=0b00000010
	// PIN5 = PC0		PIN_USED=0b00000100
	// PIN6 = PD6		PIN_USED=0b00001000
	// PIN7 = PC4		PIN_USED=0b00010000
	// PIN8 = PD1		PIN_USED=0b00100000
	// PIN9 = PD5		PIN_USED=0b01000000
	// PIN10= PC5		PIN_USED=0b10000000
	
	current_settings_shield = root_struct_shields->root_settings_shield;
	while(current_settings_shield != 0){
		
		if( (current_settings_shield->PIN_USED & 0b00000001) != 0 ){//this pin is used: PIN3 = PD3		PIN_USED=0b00000001
			root_pin_functions->PIN_USED |= 0b00000001;
			if( (current_settings_shield->PIN_MASK & 0b00000001) != 0 ){
					PIN3_SET_AS_OUTPUT; // PD3 as output
					root_pin_functions->PIN_MASK |= 0b00000001;
					
					if( (root_pin_functions->PIN_STATUS & 0b00000001) != 0 ) PIN3_SET_ON; //turn ON the pin

					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN3 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b00000001) != 0 ){
					PIN3_ENABLE_PULLUP; // pull-up resistor enabled on PD3
				}else{
					PIN3_DISABLE_PULLUP; // pull-up resistor disabled on PD3
				}
			}
		}
		
		#ifndef UART_DEBUG_MANAGER_BOOT
		if( (current_settings_shield->PIN_USED & 0b00000010) != 0 ){//this pin is used: PIN4 = PD0		PIN_USED=0b00000010
			root_pin_functions->PIN_USED |= 0b00000010;
			if( (current_settings_shield->PIN_MASK & 0b00000010) != 0 ){
					PIN4_SET_AS_OUTPUT; // PD0 as output
					root_pin_functions->PIN_MASK |= 0b00000010;
					
					if( (root_pin_functions->PIN_STATUS & 0b00000010) != 0 ) PIN4_SET_ON; //turn ON the pin

			}else{
				if( (current_settings_shield->PULL_UP & 0b00000010) != 0 ){
					PIN4_ENABLE_PULLUP; // pull-up resistor enabled on PD0
				}else{
					PIN4_DISABLE_PULLUP; // pull-up resistor disabled on PD0
				}
			}
		}
		#endif
		
		if( (current_settings_shield->PIN_USED & 0b00000100) != 0 ){//this pin is used: PIN5 = PC0		PIN_USED=0b00000100
			root_pin_functions->PIN_USED |= 0b00000100;
			if( (current_settings_shield->PIN_MASK & 0b00000100) != 0 ){
					PIN5_SET_AS_OUTPUT; // PC0 as output
					root_pin_functions->PIN_MASK |= 0b00000100;
					
					if( (root_pin_functions->PIN_STATUS & 0b00000100) != 0 ) PIN5_SET_ON; //turn ON the pin
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN5 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b00000100) != 0 ){
					PIN5_ENABLE_PULLUP; // pull-up resistor enabled on PC0
				}else{
					PIN5_DISABLE_PULLUP; // pull-up resistor disabled on PC0
				}
			}
		}
		
		if( (current_settings_shield->PIN_USED & 0b00001000) != 0 ){//this pin is used: PIN6 = PD6		PIN_USED=0b00001000
			root_pin_functions->PIN_USED |= 0b00001000;
			if( (current_settings_shield->PIN_MASK & 0b00001000) != 0 ){
					PIN6_SET_AS_OUTPUT; // PD6 as output
					root_pin_functions->PIN_MASK |= 0b00001000;
					
					if( (root_pin_functions->PIN_STATUS & 0b00001000) != 0 ) PIN6_SET_ON; //turn ON the pin
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN6 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b00001000) != 0 ){
					PIN6_ENABLE_PULLUP; // pull-up resistor enabled on PD6
				}else{
					PIN6_DISABLE_PULLUP; // pull-up resistor disabled on PD6
				}
			}
		}
		
		if( (current_settings_shield->PIN_USED & 0b00010000) != 0 ){//this pin is used: PIN7 = PC4		PIN_USED=0b00010000
			root_pin_functions->PIN_USED |= 0b00010000;
			if( (current_settings_shield->PIN_MASK & 0b00010000) != 0 ){
					PIN7_SET_AS_OUTPUT; // PC4 as output
					root_pin_functions->PIN_MASK |= 0b00010000;
					
					if( (root_pin_functions->PIN_STATUS & 0b00010000) != 0 ) PIN7_SET_ON; //turn ON the pin
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN7 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b00010000) != 0 ){
					PIN7_ENABLE_PULLUP; // pull-up resistor enabled on PC4
				}else{
					PIN7_DISABLE_PULLUP; // pull-up resistor disabled on PC4
				}
			}
		}
		
		#ifndef UART_DEBUG_MANAGER_BOOT
		if( (current_settings_shield->PIN_USED & 0b00100000) != 0 ){//this pin is used: PIN8 = PD1		PIN_USED=0b00100000
			root_pin_functions->PIN_USED |= 0b00100000;
			if( (current_settings_shield->PIN_MASK & 0b00100000) != 0 ){
					PIN8_SET_AS_OUTPUT; // PD1 as output
					root_pin_functions->PIN_MASK |= 0b00100000;
					
					if( (root_pin_functions->PIN_STATUS & 0b00100000) != 0 ) PIN8_SET_ON; //turn ON the pin
					
			}else{
				if( (current_settings_shield->PULL_UP & 0b00100000) != 0 ){
					PIN8_ENABLE_PULLUP; // pull-up resistor enabled on PD1
				}else{
					PIN8_DISABLE_PULLUP; // pull-up resistor disabled on PD1
				}
			}
		}
		#endif
		
		if( (current_settings_shield->PIN_USED & 0b01000000) != 0 ){//this pin is used: PIN9 = PD5		PIN_USED=0b01000000
			root_pin_functions->PIN_USED |= 0b01000000;
			if( (current_settings_shield->PIN_MASK & 0b01000000) != 0 ){
					PIN9_SET_AS_OUTPUT; // PD5 as output
					root_pin_functions->PIN_MASK |= 0b01000000;
					
					if( (root_pin_functions->PIN_STATUS & 0b01000000) != 0 ) PIN9_SET_ON; //turn ON the pin
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN9 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b01000000) != 0 ){
					PIN9_ENABLE_PULLUP; // pull-up resistor enabled on PD5
				}else{
					PIN9_DISABLE_PULLUP; // pull-up resistor disabled on PD5
				}
			}
		}
		
		if( (current_settings_shield->PIN_USED & 0b10000000) != 0 ){//this pin is used: PIN10= PC5		PIN_USED=0b10000000
			root_pin_functions->PIN_USED |= 0b10000000;
			if( (current_settings_shield->PIN_MASK & 0b10000000) != 0 ){
					PIN10_SET_AS_OUTPUT; // PC5 as output
					root_pin_functions->PIN_MASK |= 0b10000000;
					
					if( (root_pin_functions->PIN_STATUS & 0b10000000) != 0 ) PIN10_SET_ON; //turn ON the pin
					
					#ifdef UART_DEBUG_MANAGER_BOOT
					UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN10 OUT",1); //it send a string through the UART only if is in debug mode
					#endif
			}else{
				if( (current_settings_shield->PULL_UP & 0b10000000) != 0 ){
					PIN10_ENABLE_PULLUP; // pull-up resistor enabled on PC5
				}else{
					PIN10_DISABLE_PULLUP; // pull-up resistor disabled on PC5
				}
			}
		}
		

		//here it init the variables for the shileds
		//unsigned char temp_char = 0;
		//lib_shields((struct settings_shield *)&current_settings_shield, (unsigned char *)&temp_char);//the function 0 is the init function
		//lib_shields(current_settings_shield, temp_char);//the function 0 is the init function
		lib_shields((struct settings_shield *)current_settings_shield, 0);//the function 0 is the init function
		
		
		current_settings_shield = current_settings_shield->next;
	}
	
	#ifdef UART_DEBUG_MANAGER_BOOT
	//UART_DEBUG_MANAGER_send_STR2((unsigned char *)"PIN_MASK=",1); //it send a string through the UART only if is in debug mode
	//UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)root_pin_functions->PIN_MASK, 0);
	#endif
	
	
	
	//managing the shields
	
	
	
}

//here some functions that needs to be run every 1mS
void check_and_parse_functions_manager_shield_every_1mS(void){
	//unsigned char array_cmd[16];
	
	//this is a timer used in each function of a shield that needs a timer
	if(cont_Timer_Manager_Shield_ms < 10){
		cont_Timer_Manager_Shield_ms++;
	}else{
		if(cont_Timer_Manager_Shield_10ms < 100){
			cont_Timer_Manager_Shield_10ms++;
			cont_Timer_Manager_Shield_ms = 0;
		}else{			
			if(cont_Timer_Manager_Shield_SS < 60){
				cont_Timer_Manager_Shield_SS++;
				cont_Timer_Manager_Shield_ms = 0;
				cont_Timer_Manager_Shield_10ms = 0;
			}else{
				if(cont_Timer_Manager_Shield_MM < 60){
					cont_Timer_Manager_Shield_MM++;
					cont_Timer_Manager_Shield_SS = 0;
					cont_Timer_Manager_Shield_ms = 0;
					cont_Timer_Manager_Shield_10ms = 0;
				}else{
					if(cont_Timer_Manager_Shield_HH < 24){
						cont_Timer_Manager_Shield_HH++;
						cont_Timer_Manager_Shield_MM = 0;
						cont_Timer_Manager_Shield_SS = 0;
						cont_Timer_Manager_Shield_ms = 0;
						cont_Timer_Manager_Shield_10ms = 0;
					}else{
						//restart timer from 0
						cont_Timer_Manager_Shield_HH = 0;
						cont_Timer_Manager_Shield_MM = 0;
						cont_Timer_Manager_Shield_SS = 0;
						cont_Timer_Manager_Shield_ms = 0;
						cont_Timer_Manager_Shield_10ms = 0;
					}
				}	
			}
		}
	}
	
	//decont_vars_DHT11(); //this decrements the variables for the sensor 5 (DHT11 and DHT22)
	
}


//functions for the PIN to put into the infinite loop
void check_and_parse_functions_manager(void){
	
	unsigned char array_cmd2[16];
	unsigned char ii;
	unsigned char ll;

	//variabili usate per Input Duty
	unsigned char temp_input_trigger;
	unsigned char temp_id_input;
	unsigned char temp_num_pin_input;
	unsigned char temp_num_pin_output;
	unsigned char temp_status_pin_input;
	unsigned char temp_last_status_pin;
	unsigned char temp_status_output_to_set;
	unsigned char temp_trigger_occured;
	unsigned char temp_pin_output;
	
	unsigned char data[16];
	
	unsigned int u16_temp_value_threshold_low;
	unsigned int u16_temp_value_threshold_high;
	unsigned int u16_temp_value_pin;

	unsigned long u32TempVar;

	unsigned char varErrorReadingPin=0;


	
	if(decont_check_and_parse_functions_manager == 0){
		decont_check_and_parse_functions_manager = MAX_DELAY_RUN_MANAGER_FUNCTION; //ogni circa 50mS esegue una funzione
		
		//Input Duty: eseguite e controllate sempre ogni 50ms
		//meaning bits inside fun_input_ctrl_output from byte 0 to byte 7
		//	bit0 to bit2	=	ID of the input that control the output
		//	bit3 to bit5 	= 	ID of the output to control
		//	bit6 to	bit7	=	Status to set to the output: 0 = OFF, 1 = ON, 2 = TOGGLES, 
		//every 2bit into byte 8 and byte 9 there is the status for each of the 8 function "Input Duty"
		//example: byte 8 of the array fun_input_ctrl_output (so the array point to fun_input_ctrl_output[8])
		// bit0 to bit1		=	Input Trigger: 0 = Disabled, 1 = Rising edge, 2 = Falling edge, 3 = Both. This are relate to the fun_input_ctrl_output[0]
		// bit2 to bit3		=	Input Trigger: 0 = Disabled, 1 = Rising edge, 2 = Falling edge, 3 = Both. This are relate to the fun_input_ctrl_output[1]
		//	........ and so on
		if(var_NUM_DIGITAL_INPUT>0){
			//if(var_NUM_DIGITAL_OUTPUT>0)
			for(ii=0;ii<8;ii++){//inizio ciclo for per gli 8 input previsti
				temp_trigger_occured=0;
				//temp_input_trigger = (fun_input_ctrl_output[ii]&0b00011000)>>3;
				if(ii<4){
					temp_input_trigger = (fun_input_ctrl_output[8]>>(ii*2))&0x03; //fa uno shift di 0 posizioni, poi se controlla il secondo trigger allora lo shift lo fa di 2 posizioni e poi di 4 per il terzo .....
				}else{
					temp_input_trigger = (fun_input_ctrl_output[9]>>((ii-4)*2))&0x03;  //questo e per il byte 9 e fa uno shift di 0 posizioni, poi se controlla il secondo trigger allora lo shift lo fa di 2 posizioni e poi di 4 per il terzo .....
				}
				if(temp_input_trigger>0){ //significa che il trigger per questo input è abilitato
					//ora estraggo l'ID dell'input e leggo il valore
					temp_id_input = (fun_input_ctrl_output[ii]>>3) & 0x07; //mantiene i primi 3 bit che identifica il numero di ID dell'input
					//per sapere il valore fisico del pin corrispondente, estraggo il numero di pin relativo all'ID e poi ne leggo il valore
					temp_num_pin_input = num_pin_iotgemini_from_id_input(temp_id_input);
					varErrorReadingPin = return_value_pin(temp_num_pin_input, data);
					temp_status_pin_input = data[4]; //dentro data[4] ho il valore dell'input scritto dalla funzione return_value_pin
					if(temp_status_pin_input>0){ //questo per assicurarmi di poterlo poi confrontare sucessivamente con il bit relativo memorizzato dentro il byte last_status_input
						temp_status_pin_input = 1;
					}else{
						temp_status_pin_input = 0;
					}
					
					//temp_last_status_pin = ((last_status_input>>(temp_num_pin_input-2))&0x01); //ora estraggo l'ultimo stato di questo input
					temp_last_status_pin = ((last_status_input>>(ii))&0x01); //ora estraggo l'ultimo stato di questo input
					if( temp_status_pin_input != temp_last_status_pin ){ 
						if(temp_input_trigger==1){ //rising edge
							if(temp_status_pin_input==1 && temp_last_status_pin==0){ //se prima era basso e ora è alto allora è rising edge
								temp_trigger_occured=1;
							}
						}else if(temp_input_trigger==2){ //falling edge
							if(temp_status_pin_input==0 && temp_last_status_pin==1){ //se prima era alto e ora è basso allora è falling edge
								temp_trigger_occured=1;
							}
						}
						if(temp_status_pin_input==0){
							temp_status_pin_input = 1;
							//last_status_input &=  ~(temp_status_pin_input << (temp_num_pin_input-2));
							last_status_input &=  ~(temp_status_pin_input << (ii));
						}else{
							//last_status_input |= (temp_status_pin_input << (temp_num_pin_input-2));
							last_status_input |= (temp_status_pin_input << (ii));
						}
						
						
						if(temp_trigger_occured==1){ //se il triggrer è scattato allora cambia lo stato dell'output
							array_cmd2[7]=(fun_input_ctrl_output[ii]&0x07); //set id output
							temp_status_output_to_set = (fun_input_ctrl_output[ii]>>6);
							if(temp_status_output_to_set==0){ //OFF
								array_cmd2[8]=0; //set off
							}else if(temp_status_output_to_set==1){ //ON
								array_cmd2[8]=1; //set on
							}else if(temp_status_output_to_set==2){ //TOGGLES
								//devo sapere lo stato e poi invertirlo........
								
								
								//Function that return number of the pin (from pin3 to pin10) from the number of the ID
								temp_pin_output = num_pin_iotgemini_from_id_output(array_cmd2[7]);
			
								//this get the value and into data[4] there is a value on 8bit, into the data[5] there is the num of bit used, 
								//if the num bit used is over 8bit the data it is not kept into data[4], but into the consecutive data[6],data[7].....data[x]
								varErrorReadingPin = return_value_pin(temp_pin_output, data);

								if(data[4]==0){
									array_cmd2[8]=255; //set on
									
									array_cmd2[9]=24;//this for the RGB
									array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=255;
									
								}else{
									array_cmd2[8]=0; //set off
									
									array_cmd2[9]=24;//this for the RGB
									array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=0;
								}
								
							}
							o_SetOut(array_cmd2,1);		//Function to set the status of an output
						}
						
						//last_status_input = last_status_input & (0xFE << ii);
						//last_status_input = last_status_input | (temp_status_pin_input << ii);
					}
				}
			}//fine ciclo for per gli 8 input previsti
		}
		
		if(ID_Function_Manager_to_Run==1){	
			if( (id_data_eeprom_to_update & 0b000000000000001) != 0){	//it update the status of the Outputs PIN
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_OUT_PIN_VALUE), root_pin_functions->PIN_STATUS);
			}
			if( (id_data_eeprom_to_update & 0b0000000000001000) != 0){	//it update the PWM of PIN3 (led RED)
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_PWM_VALUE), get_duty_cycle_RED_LED());
			}
			if( (id_data_eeprom_to_update & 0b0000000000010000) != 0){	//it update the PWM of PIN3 (led GREEN)
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_PWM_VALUE+1), get_duty_cycle_GREEN_LED());
			}
			if( (id_data_eeprom_to_update & 0b0000000000100000) != 0){	//it update the PWM of PIN3 (led BLUE)
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_PWM_VALUE+2), get_duty_cycle_BLUE_LED());
			}
			if( (id_data_eeprom_to_update & 0b0000000001000000) != 0){	//it update the value for the timer
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE), semRelayTimer);
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE_10MS), RelayTimer_10ms);
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+1), RelayTimer_SS);
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+2), RelayTimer_MM);
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_TIMER_RELAY_VALUE+3), RelayTimer_HH);
			}
			if( (id_data_eeprom_to_update & 0b0000000010000000) != 0){	//it update the value for the inputs duty
				for(ii=0;ii<10;ii++)
					eeprom_write_byte ((unsigned char *)(START_ADDRESS_INPUTS_DUTY_VALUE+ii), fun_input_ctrl_output[ii]);
			}
			if( (id_data_eeprom_to_update & 0b0000000100000000) != 0){	//it update the PWM of PIN3 (led BLUE)
				eeprom_write_byte ((unsigned char *)(START_ADDRESS_SETTINGS_PLATFORM), sem_Led_TX_keep_OFF);
			}
			if( (id_data_eeprom_to_update & 0b0000001000000000) != 0){	//it update into the EEPROM the values for THRESHOLD0 and THRESHOLD1
				for(ii=0;ii<10;ii++)
					eeprom_write_byte ((unsigned char *)(START_ADDRESS_THRESHOLD_VALUES+ii), fun_threshold_ctrl_output[ii]);
			}
		
			id_data_eeprom_to_update = 0;
		
		
		}else if(ID_Function_Manager_to_Run==2){
			//ho abilitato da init dentro basic_function.c la conversione automatica sul fronte di salita del trigger di Timer1 COMPA
			if((root_pin_functions->SEM_ADC&0b00000100) != 0){
				//ADC0_Average = get_average_ADC((unsigned int *)&ADC0_Average_Array[0], (unsigned char *)&contSampleADC0, (unsigned char *)&NUM_MAX_SAMPLE_ADC0, (unsigned char *)&NUM_ADC0);
				ADC0_Average = get_10bit_adc(NUM_ADC0);
			}
		}else if(ID_Function_Manager_to_Run==3){
			if((root_pin_functions->SEM_ADC&0b00010000) != 0){
				//ADC4_Average = get_average_ADC((unsigned int *)&ADC4_Average_Array[0], (unsigned char *)&contSampleADC4, (unsigned char *)&NUM_MAX_SAMPLE_ADC4, (unsigned char *)&NUM_ADC4);
				ADC4_Average = get_10bit_adc(NUM_ADC4);
				
				#ifdef UART_DEBUG_MANAGER_PARSE_FUNCTIONS
				//UART_DEBUG_MANAGER_send_STR2((unsigned char *)"get_average_ADC4.....",1); //it send a string through the UART only if is in debug mode
				#endif
			}
			#ifdef UART_DEBUG_MANAGER_PARSE_FUNCTIONS
			//UART_DEBUG_MANAGER_send_STR2((unsigned char *)"SEM_ADC=",1); //it send a string through the UART only if is in debug mode
			//UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)root_pin_functions->SEM_ADC, 0);
			#endif
		}else if(ID_Function_Manager_to_Run==4){
			if((root_pin_functions->SEM_ADC&0b10000000) != 0){
				//ADC5_Average = get_average_ADC((unsigned int *)&ADC5_Average_Array[0], (unsigned char *)&contSampleADC5, (unsigned char *)&NUM_MAX_SAMPLE_ADC5, (unsigned char *)&NUM_ADC5);
				ADC5_Average = get_10bit_adc(NUM_ADC5);
			}
			
		}else if(ID_Function_Manager_to_Run==5){
			if(root_pin_functions->SEM_DHT11 != 0){
				//if(pointer_struct_DHT11->sem_get_new_value_with_this_pin != 0){
					for(ii=1;ii<128 && pointer_struct_DHT11->sem_get_new_value_with_this_pin != 0; ii*=2){
						if( (pointer_struct_DHT11->sem_get_new_value_with_this_pin & ii) != 0){ //if the wheight is set then get the data
							if(dht11_read( (unsigned char)(ii) ) == 0){
							//dht11_read( (unsigned char)(ii) );
								//no errors in reading sensor data
								pointer_struct_DHT11->sem_get_new_value_with_this_pin &= ~ii ; //taking off the wheigt of the pin where is connected this sensor	
							}
						}
					}
					//pointer_struct_DHT11->sem_get_new_value_with_this_pin=0;
				//}
			}
			
			
		}else if(ID_Function_Manager_to_Run==6){
			ADC1_Average = get_10bit_adc(NUM_ADC1);
			//676 is the value for 3V3 if the powers supply of the MCU is perfectly a 5V
			//offset_ADC_compensation = ADC1_Average - 676;
			//Led_TX_OFF_and_after_delay_turn_ON();
			
			
	}else if( 	(ID_Function_Manager_to_Run == ID_FUNCTION_MANAGER_THRESHOLD0)
				|| (ID_Function_Manager_to_Run == ID_FUNCTION_MANAGER_THRESHOLD1)
			){	//qui intendo eseguire la funzione threshold0-1 su di un output esistente con un input analogico

			//fun_threshold_ctrl_output[10];	//this has the Values on 16bits that are the threshold to control an output

			// Byte and Bits meaning inside fun_threshold_ctrl_output[] from byte 0 to byte 9
			// ------------------- BYTE0	=	ID INPUT-OUTPUT FOR THRESHOLD0 -----------------------
			//		B0:bit0 (LSB) 	to	 B0:bit2 (MSB)	=	ID of the input that control the output
			//		B0:bit3 (LSB) 	to	 B0:bit5 (MSB)	=	ID of the output to control
			//		B0:bit6 (LSB) 	to	 B0:bit7 (MSB)	=	Way to control output:
			//											___________________________________________________________________
			//											| VALUE bit6-bit7		< LOW THRESHOLD0		> HIGH THRESHOLD0 |
			//											|_________________________________________________________________|
			//											|		0x0 			SET OUT LOW 			SET OUT HIGH	  |
			//											|		0x1 			SET OUT HIGH 			SET OUT LOW		  |
			//											|		0x2				FUNCTION DISABLED		FUNCTION DISABLED |
			//											|_________________________________________________________________|
			// ---------------------------------------------------------------------------------------
			// ------------------- BYTE1+BYTE2	=	HIGH THRESHOLD0 ----------------------------------
			//		B1:bit0 (LSB),	B1:bit1,	......	B2:bit6,	B2:bit7 (MSB)
			// ---------------------------------------------------------------------------------------
			// ------------------- BYTE3+BYTE4	=	LOW THRESHOLD0 ----------------------------------
			//		B3:bit0 (LSB),	B3:bit1,	......	B4:bit6,	B4:bit7 (MSB)
			// ---------------------------------------------------------------------------------------

			// ------------------- BYTE5	=	ID INPUT-OUTPUT FOR THRESHOLD1 -----------------------
			//		B5:bit0 (LSB) 	to	 B5:bit2 (MSB)	=	ID of the input that control the output
			//		B5:bit3 (LSB) 	to	 B5:bit5 (MSB)	=	ID of the output to control
			//		B5:bit6 (LSB) 	to	 B5:bit7 (MSB)	=	Way to control output:
			//											___________________________________________________________________
			//											| VALUE bit6-bit7		< LOW THRESHOLD1		> HIGH THRESHOLD1 |
			//											|_________________________________________________________________|
			//											|		0x0 			SET OUT LOW 			SET OUT HIGH	  |
			//											|		0x1 			SET OUT HIGH 			SET OUT LOW		  |
			//											|		0x2				FUNCTION DISABLED		FUNCTION DISABLED |
			//											|_________________________________________________________________|
			// ---------------------------------------------------------------------------------------
			// ------------------- BYTE6+BYTE7	=	HIGH THRESHOLD1 ----------------------------------
			//		B6:bit0 (LSB),	B6:bit1,	......	B7:bit6,	B7:bit7 (MSB)
			// ---------------------------------------------------------------------------------------
			// ------------------- BYTE8+BYTE9	=	LOW THRESHOLD1 -----------------------------------
			//		B8:bit0 (LSB),	B8:bit1,	......	B9:bit6,	B9:bit7 (MSB)
			// ---------------------------------------------------------------------------------------

			if(var_NUM_DIGITAL_OUTPUT>0){
				//for(ii=0;ii<2 && ii<var_NUM_DIGITAL_OUTPUT;ii++){//I have 2 function to check that works with threshold
				ii = ID_Function_Manager_to_Run - ID_FUNCTION_MANAGER_THRESHOLD0; // selects the bytes to do the controls
					ll = (unsigned char)(ii*5); //index byte0
					temp_status_output_to_set = (unsigned char)((fun_threshold_ctrl_output[ll] >> 6) & 0x03); // I get the way to control the output or if it is disabled with value 0x3
					if(	temp_status_output_to_set != 0x03 ){ // if the function is NOT DISABLED then I check LOW and HIGH THRESHOLDS
						temp_id_input = (unsigned char)(fun_threshold_ctrl_output[ll] & 0b00000111); // I get the input ID that control the output
						//per sapere il valore fisico del pin corrispondente, estraggo il numero di pin relativo all'ID e poi ne leggo il valore
						temp_num_pin_input = num_pin_iotgemini_from_id_input(temp_id_input);
						varErrorReadingPin = return_value_pin(temp_num_pin_input, data);

						#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)"***",1);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" THRESHOLDS: ",1);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ID INPUT = ",0);
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)temp_id_input & 0xFF, 0);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)"  PIN INPUT = ",0);
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)temp_num_pin_input & 0xFF, 0);
						#endif

						//because I always check on 16bits I compare the 16bits stored into the EEPROM (fun_threshold_ctrl_output[]) with the data[6] and data[7]
						ll++; //index byte1 = LSB HIGH THRESHOLD x
						u16_temp_value_threshold_low = (unsigned int)fun_threshold_ctrl_output[ll];
						u16_temp_value_threshold_low &= 0x00FF;
						ll++; //index byte2 = MSB HIGH THRESHOLD x
						u16_temp_value_threshold_low |= (unsigned int)(fun_threshold_ctrl_output[ll] << 8);

						ll++; //index byte3 = LSB LOW THRESHOLD x
						u16_temp_value_threshold_high = (unsigned int)fun_threshold_ctrl_output[ll];
						u16_temp_value_threshold_high &= 0x00FF;
						ll++; //index byte4 = MSB LOW THRESHOLD x
						u16_temp_value_threshold_high |= (unsigned int)(fun_threshold_ctrl_output[ll] << 8);

						//data[5] == 32 then is the DHT11 // inside data[5] there is written how many bits take the value of the input
						if( (data[5] == 32) && (ii == 1) && ((fun_threshold_ctrl_output[0] & 0x3) == (fun_threshold_ctrl_output[5] & 0x3)) ){
							//it is the DHT11 where data[6] and data[7] is the umidity and data[8] and data[9] is the temperature
							//thus if ID input is same for bot THRESHOLD0 and THRESHOLD1 then THRESHOLD0 is for umidity and THRESHOLD1 for temperature
							u16_temp_value_pin = (unsigned int)data[9];
							u16_temp_value_pin &= 0x00FF;
							u16_temp_value_pin |= (unsigned int)(data[8] << 8);
						}else{
							u16_temp_value_pin = (unsigned int)data[7];
							u16_temp_value_pin &= 0x00FF;
							u16_temp_value_pin |= (unsigned int)(data[6] << 8);
						}
						if(cont_input_values_threshold_fun[ii] > 2){
							cont_input_values_threshold_fun[ii] = 0;
						}else{
							cont_input_values_threshold_fun[ii]++;
						}
						input_values_threshold_fun[ii][cont_input_values_threshold_fun[ii]] = u16_temp_value_pin; //recording the values
						//going to do average
						u32TempVar = 0;
						for(ll=0;ll<4;ll++){
							u32TempVar += input_values_threshold_fun[ii][ll];
						}
						u32TempVar /= 4;
						u16_temp_value_pin = (unsigned int) u32TempVar;


						#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" THRESHOLDS: ",1);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" THRESHOLD LOW = ",0); //it send a string through the UART only if is in debug mode
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)(u16_temp_value_threshold_low >> 8) & 0xFF, 0);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ",0);
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)u16_temp_value_threshold_low & 0xFF, 0);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" THRESHOLD HIGH = ",0); //it send a string through the UART only if is in debug mode
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)(u16_temp_value_threshold_high >> 8) & 0xFF, 0);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ",0);
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)u16_temp_value_threshold_high & 0xFF, 0);
						//UART_DEBUG_send_STARS(3);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" VALUE PIN = ",0); //it send a string through the UART only if is in debug mode
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)(u16_temp_value_pin >> 8) & 0xFF, 0);
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ",0);
						UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)u16_temp_value_pin & 0xFF, 0);
						#endif

						ll = (unsigned char)(ii*5); //index byte0
						array_cmd2[7] = (unsigned char)((fun_threshold_ctrl_output[ll] >> 3) & 0x07); //set ID output to control
						array_cmd2[9] = (unsigned char)24;//this for the RGB

						if(u16_temp_value_pin < u16_temp_value_threshold_low){ //check if value input is under LOW THRESHOLD
							if(temp_status_output_to_set==1){ //SET OUTPUT TO LOW
								array_cmd2[8]=0; //set off
								array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=0; //for RGB
								#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
								UART_DEBUG_MANAGER_send_STR2((unsigned char *)" -> BELOW LOW THEN SET OFF ",0);
								#endif
							}else if(temp_status_output_to_set==2){ //SET OUTPUT TO HIGH
								array_cmd2[8]=1; //set on
								array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=255; //for RGB
								#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
								UART_DEBUG_MANAGER_send_STR2((unsigned char *)" -> BELOW LOW THEN SET ON ",0);
								#endif
							}
						}else if(u16_temp_value_pin > u16_temp_value_threshold_high){ //check if value input is above HIGH THRESHOLD
							if(temp_status_output_to_set==1){ //SET OUTPUT TO HIGH
								array_cmd2[8]=1; //set on
								array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=255; //for RGB
								#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
								UART_DEBUG_MANAGER_send_STR2((unsigned char *)" -> ABOVE HIGH THEN SET ON ",0);
								#endif
							}else if(temp_status_output_to_set==2){ //SET OUTPUT TO LOW
								array_cmd2[8]=0; //set off
								array_cmd2[10]=array_cmd2[11]=array_cmd2[12]=0; //for RGB
								#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
								UART_DEBUG_MANAGER_send_STR2((unsigned char *)" -> ABOVE HIGH THEN SET OFF ",0);
								#endif
							}
						}
						ll = (unsigned char)(ii+10); //index byte10 for THRESHOLD0 and byte11 for THRESHOLD1
						temp_num_pin_output = num_pin_iotgemini_from_id_output(array_cmd2[7]); // dall'ID  (array_cmd2[7]) dell'output da controllare ottengo il numero di pin fisico che è nello schema
						varErrorReadingPin = return_value_pin(temp_num_pin_output, (unsigned char *)data);
						if(varErrorReadingPin==0){ //no error in reading status of the output
							//data[4] //actual value of the output
							//data[5] //size of the variable that contain the output status/value. If it is 1 the value is bolean, otherwise can be 8 thus is a byte which is used for PWM (RGB control)
							if(	(array_cmd2[8] == 0 && data[4] > 0) 		//if the status to set (array_cmd2[8]) is different from the actual value of the status of the output (data[4]) then has to set the output with the new status (array_cmd2[8])
								|| (array_cmd2[8] > 0 && data[4] == 0)
								|| (fun_threshold_ctrl_output[ll] != data[4]) 	//the last 2 bytes of the vector fun_threshold_ctrl_output[] are used as flag to remember the last status of the output
							){
								fun_threshold_ctrl_output[ll] = data[4]; //saving last status thus it will not keep calling the function o_SetOut()
								o_SetOut(array_cmd2,0);		//Function to set the status of an output
							}
						}


						#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
						UART_DEBUG_MANAGER_send_STR2((unsigned char *)"***",1);
						#endif
					}
				//}
			}


		//}else if(ID_Function_Manager_to_Run==9){	//free

			
		}else{
			ID_Function_Manager_to_Run = 0;
		}
		
		ID_Function_Manager_to_Run++;
		
		
	}else{
		if(last_cont_10ms_function_manager != cont_Timer_Manager_Shield_10ms){
			last_cont_10ms_function_manager = cont_Timer_Manager_Shield_10ms;
			decont_check_and_parse_functions_manager--;
		}
		
		
		
		//-----------------------------------------------------------------
		//here there are the functions applied to the shields as the timer
		//-----------------------------------------------------------------
		
		//the timer will change an output status after the time given by the user
		if( (semRelayTimer & 0b00111111) > 0 ){ //BEGIN TIMER
			//if( last_cont_Timer_Manager_Shield_SS != cont_Timer_Manager_Shield_SS ){ //if the timer is enabled  then decrement all variables
			//	last_cont_Timer_Manager_Shield_SS = cont_Timer_Manager_Shield_SS;
			if( last_cont_Timer_Manager_Shield_10ms != cont_Timer_Manager_Shield_10ms ){ //if the timer is enabled  then decrement all variables
				last_cont_Timer_Manager_Shield_10ms = cont_Timer_Manager_Shield_10ms;
				if(decont_RelayTimer_10ms > 0){
					decont_RelayTimer_10ms--;
				}else{
					if(decont_RelayTimer_SS > 0){
						decont_RelayTimer_SS--;
						decont_RelayTimer_10ms = 99; //99 because it pass from here every 10mS
					}else{
						if(decont_RelayTimer_MM > 0){
							decont_RelayTimer_MM--;
							decont_RelayTimer_SS = 59;
							decont_RelayTimer_10ms = 99;
						}else{
							if(decont_RelayTimer_HH > 0){
								decont_RelayTimer_HH--;
								decont_RelayTimer_MM = 59;
								decont_RelayTimer_SS = 59;
								decont_RelayTimer_10ms = 99;
							}else{
								if(decont_RelayTimer_SS==0){
									if((semRelayTimer&0b00111111)>0){
										array_cmd2[7]=(semRelayTimer&0b00111111)-1; //set id output
									}else{
										array_cmd2[7]=0; //set id output = 0
									}
									if((semRelayTimer&0b10000000)==0b10000000){
										array_cmd2[8]=1; //set on
									}else{
										array_cmd2[8]=0; //set off
									}
									o_SetOut(array_cmd2,0);		//Function to set the status of an output
									decont_RelayTimer_SS = -1; //it avoid to keep to pass from here
								}
							}
						}	
					}
				}
			}
		}//END TIMER
		
	}
	
}


//functions of the shields libraries
void lib_shields(struct settings_shield *current_shield_settings, unsigned char function_to_run){
	if(current_shield_settings->ID == 1){ //LED digital output
		if(function_to_run == 0){ //setup that is executed on boot
			#ifdef UART_DEBUG_MANAGER_LIB_SHIELD
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"Init variables for shield LED ID=1",1); //it send a string through the UART only if is in debug mode
			#endif
			init_led_1();					//Function that allocate the variables and init the shield
			
		}else if(function_to_run == 1){ //
			//here I could set a timer
		}
	}else if(current_shield_settings->ID == 2 || current_shield_settings->ID == 7){ //BUTTON digital input
		if(function_to_run == 0){ //setup that is executed on boot
			#ifdef UART_DEBUG_MANAGER_LIB_SHIELD
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"Init variables for shield BUTTON ID=2/7",1); //it send a string through the UART only if is in debug mode
			#endif
			init_button_2(current_shield_settings);					//Function that allocate the variables and init the shield
			
		}else if(function_to_run == 1){ //
			//here I could set a timer
		}
	}else if(	current_shield_settings->ID == 3 
				|| current_shield_settings->ID == 6 
				|| current_shield_settings->ID == 9
				){ //SENSOR analogue input
		if(function_to_run == 0){ //setup that is executed on boot
			#ifdef UART_DEBUG_MANAGER_LIB_SHIELD
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"Init variables for shield SENSOR ID=3/6/9",1); //it send a string through the UART only if is in debug mode
			#endif
			init_SENSOR_3(current_shield_settings);					//Function that allocate the variables and init the shield
			
		}else if(function_to_run == 1){ //
			//here I could set a timer
		}
	}else if(current_shield_settings->ID == 4){ //RGB LED
		if(function_to_run == 0){ //setup that is executed on boot
			#ifdef UART_DEBUG_MANAGER_LIB_SHIELD
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"Init variables for shield RGB ID=4",1); //it send a string through the UART only if is in debug mode
			#endif
			init_RGB_4(current_shield_settings);					//Function that allocate the variables and init the shield
			
		}else if(function_to_run == 1){ //
			//here I could set a timer
		}
	}else if(current_shield_settings->ID == 5 || current_shield_settings->ID == 8){ //DHT11 and DHT22
		if(function_to_run == 0){ //setup that is executed on boot
			#ifdef UART_DEBUG_MANAGER_LIB_SHIELD
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"Init variables for shield DHT11-DHT22 ID=5",1); //it send a string through the UART only if is in debug mode
			#endif
			init_DHT11_5(current_shield_settings);					//Function that allocate the variables and init the shield
			
		}else if(function_to_run == 1){ //
			//here I could set a timer
		}
	}
	
}

