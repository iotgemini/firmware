/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.1
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Common functions to use to build 
							a peripheral for the RFPI
	
	
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

unsigned char checksum (unsigned char *ptr, size_t sz) {
    //unsigned char chk = 0;
	unsigned char sum=0;
	unsigned char i;
    /*while (sz-- != 0)
        chk -= *ptr++;*/
	for(i=0;i<(sz-1);i++){ //last byte is the byte to compare for checksum
		sum += ptr[i];
	}
	if(sum != ptr[sz-1]){
		sum=1; //erro into data!
	}else{
		sum=0; //DATA ALL OK!
	}
    return sum;
}

unsigned char checksum2 (unsigned char *ptr, size_t sz) {
    //unsigned char chk = 0;
	unsigned char sum=0;
	unsigned char i;
    /*while (sz-- != 0)
        chk -= *ptr++;*/
	for(i=1;i<(sz);i++){ //first byte is the byte to compare for checksum
		sum += ptr[i];
	}
	if(sum != ptr[0]){
		sum=1; //error into data!
	}else{
		sum=0; //DATA ALL OK!
	}
    return sum;
}

unsigned char calc_checksum (unsigned char *ptr, size_t sz) {
	unsigned char sum=0;
	unsigned char i;
	for(i=0;i<sz;i++){ 
		sum += ptr[i];
	}
    return sum;
}



//***********************************************************************************
//	HERE START THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************


//here there are the functions which are executed autonomously by the peri	
//example if an input change it will send the status to the Gateway
void check_and_parse_functions_peri(void){
		
	//write here the functions you want execute into the main while
		
}


//this is used into command i and p
void fill_data_with_status_pin_and_send_back(unsigned char *data, unsigned char num_pin){
	unsigned char varErrorReadingPin=0;
		
	//this get the value and into data[4] there is a value on 8bit, into the data[5] there is the num of bit used, 
	//if the num bit used is over 8bit the data it is not kept into data[4], but into the consecutive data[6],data[7].....data[x]
	varErrorReadingPin = return_value_pin(num_pin, (unsigned char *)data);


	if(num_pin>2 && num_pin < 12 && varErrorReadingPin==0){
		//this make blink the led to notify the command has been received correctly
		Led_TX_OFF_and_after_delay_turn_ON();
		
		UART_send_and_fill_data_to_RF((unsigned char *)data, 16);
		
		#ifdef UART_DEBUG_RFPIMCU
		UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"SENT STATUS PIN",1); //it send a string through the UART only if is in debug mode
		UART_DEBUG_send_NUM_BYTE_HEX(num_pin, 0);
		UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)" = ",0);
		UART_DEBUG_send_NUM_BYTE_HEX(data[4], 0);
		#endif
	}
}


//Function to send the status of an input
void i_SendInStatus(void){
	//here you have to write the code to read the input and send back the status
	
	 unsigned char data[16];
//	 unsigned int adc_value;
	 
	data[0] = 'R';
	data[1] = 'B';
	data[2] = 'i';
	data[3] = cmd_rfpi[7];	//identifier input	
	
	//data[5] = 'D'; //is a digital input
	
	//IT IS VERY IMPORTANT THE FIRST INPUTS WITH A LOWER IDENTIFIER
	//ARE DIGITAL AND ONLY AFTER THERE ARE THE ANALOGUE INPUTS WITH AN HIGHER IDENTIFIER
	
	//pay attention to the type of variable:
	//if data[5] = 'I'  It means that byte data[4] is discarded and follows an integer with data [6] MSB and data [7] LSB
	//if data[5] = 'L'  It means that byte data[4] is discarded and follows a long with data [6] MSB and data [9] LSB
	//if data[5] = 'D'  Means that only the byte date[4] is received
	
	
	//fill_data_with_status_pin_and_send_back(data,3); //this is used into command i and p
	
	
	unsigned char num_pin_to_send_status=2;
	//Function that return number of the pin (from pin3 to pin10) from the number of the ID
	num_pin_to_send_status = num_pin_iotgemini_from_id_input(data[3]);
	
	
	#ifdef UART_DEBUG_RFPIMCU
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"CMD=i for PIN",1); //it send a string through the UART only if is in debug mode
	UART_DEBUG_send_NUM_BYTE_HEX(num_pin_to_send_status, 0);
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)" with ID= ",0);
	UART_DEBUG_send_NUM_BYTE_HEX(data[3], 0);
	#endif
		
	fill_data_with_status_pin_and_send_back(data, num_pin_to_send_status); //this is used into command i and p	


}


//Function to send the status of an output
void p_SendOutStatus(void){
	//here you have to write the code to read the output status and send back the status
	
	// unsigned char data[16];

	//IT IS VERY IMPORTANT THE FIRST OUTPUTS WITH A LOWER IDENTIFIER
	//ARE DIGITAL AND ONLY AFTER THERE ARE THE ANALOGUE OUTPUTS WITH AN HIGHER IDENTIFIER
	
	//pay attention to the type of variable:
	//if data[5] = 'I'  It means that byte data[4] is discarded and follows an integer with data [6] MSB and data [7] LSB
	//if data[5] = 'L'  It means that byte data[4] is discarded and follows a long with data [6] MSB and data [9] LSB
	//if data[5] = 'D'  Means that only the byte date[4] is received

	
	unsigned char data[16];
	data[0] = 'R';
	data[1] = 'B';
	data[2] = 'p';
	data[3] = cmd_rfpi[7];	//identifier input	
	
	
	unsigned char num_pin_to_send_status=2;
	//Function that return number of the pin (from pin3 to pin10) from the number of the ID
	num_pin_to_send_status = num_pin_iotgemini_from_id_output(data[3]);

	#ifdef UART_DEBUG_RFPIMCU
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"CMD=p for PIN",1); //it send a string through the UART only if is in debug mode
	UART_DEBUG_send_NUM_BYTE_HEX(num_pin_to_send_status, 0);
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)" with ID= ",0);
	UART_DEBUG_send_NUM_BYTE_HEX(data[3], 0);
	#endif
		
	fill_data_with_status_pin_and_send_back(data, num_pin_to_send_status); //this is used into command i and p	
	
}


//Function to set the status of an output
void o_SetOut(unsigned char *array_cmd, unsigned char enable_functions){
	//here you have to write the code to set the output status
	
	//IT IS VERY IMPORTANT THE FIRST OUTPUTS WITH A LOWER IDENTIFIER
	//ARE DIGITAL AND ONLY AFTER THERE ARE THE ANALOGUE OUTPUTS WITH AN HIGHER IDENTIFIER
	
	//first if, to set the first output status (identifier = 0)
	
	unsigned char num_pin_to_send_status=2;
	//Function that return number of the pin (from pin3 to pin10) from the number of the ID
	num_pin_to_send_status = num_pin_iotgemini_from_id_output(array_cmd[7]);

	if(enable_functions>0){
		if(array_cmd[7]==((semRelayTimer&0b00111111)-1)){
			decont_RelayTimer_10ms = RelayTimer_10ms; //byte milli seconds divided by 10
			decont_RelayTimer_SS = RelayTimer_SS; //byte seconds
			decont_RelayTimer_MM = RelayTimer_MM; //byte minutes
			decont_RelayTimer_HH = RelayTimer_HH; //byte hours
		}
	}
	
	//ID=0 //PD3 = PIN3
	if(num_pin_to_send_status == 3){ 
		if((root_pin_functions->PIN_MASK & 0b00000001) != 0){ //check if it is an output
			if((root_pin_functions->SEM_PWM & 0b00000001) != 0){ //check if it is used as PWM
				if(array_cmd[9]==24){
					set_duty_cycle_RGB_LED((int)array_cmd[10], (int)array_cmd[11], (int)array_cmd[12]);	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
					//set_duty_cycle_RGB_LED((int)array_cmd[10], (int)0xFF, (int)0xFF);
					id_data_eeprom_to_update |= 0b0000000000111000; //make to update the data into the EEPROM
				}else{
					set_duty_cycle_RGB_LED((int)array_cmd[8], (int)-1, (int)-1);	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
					id_data_eeprom_to_update |= 0b0000000000001000; //make to update the data into the EEPROM
				}
			}else{
				if(array_cmd[8] > 0){ //check value to set
					PORTD |= 0x08; //ON
					root_pin_functions->PIN_STATUS |= 0b00000001;
				}else{ 
					PORTD &= ~0x08; //OFF
					root_pin_functions->PIN_STATUS &= ~0b00000001;
				}
				id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
			}
		}
	}else 
	
	//ID=1 //PD0 = PIN4
	if(num_pin_to_send_status == 4){ 
		if((root_pin_functions->PIN_MASK & 0b00000010) != 0){ //check if it is an output
			if(array_cmd[8] > 0){ //check value to set
				PORTD |= 0x01; //ON
				root_pin_functions->PIN_STATUS |= 0b00000010;
			}else{ 
				PORTD &= ~0x01; //OFF
				root_pin_functions->PIN_STATUS &= ~0b00000010;
			}
			id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
		}
	}else
	
	//ID=2 //PC0 = PIN5
	if(num_pin_to_send_status == 5){ 
		if((root_pin_functions->PIN_MASK & 0b00000100) != 0){ //check if it is an output
			if(array_cmd[8] > 0){ //check value to set
				PORTC |= 0x01; //ON
				root_pin_functions->PIN_STATUS |= 0b00000100;
				#ifdef UART_DEBUG_RFPIMCU
				UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"Turning ON PIN5....",1); //it send a string through the UART only if is in debug mode
				#endif
			}else{ 
				PORTC &= ~0x01; //OFF
				root_pin_functions->PIN_STATUS &= ~0b00000100;
				#ifdef UART_DEBUG_RFPIMCU
				//UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"Turning OFF PIN5....",1); //it send a string through the UART only if is in debug mode
				#endif
			}
			id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
		}
	}else
		
	//ID=3 //PD6 = PIN6
	if(num_pin_to_send_status == 6){ 
		if((root_pin_functions->PIN_MASK & 0b00001000) != 0){ //check if it is an output
			if((root_pin_functions->SEM_PWM & 0b00001000) != 0){ //check if it is used as PWM
				set_duty_cycle_RGB_LED((int)-1, (int)-1, (int)array_cmd[8]);	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
				id_data_eeprom_to_update |= 0b0000000000100000;
			}else{
				if(array_cmd[8] > 0){ //check value to set
					PORTD |= 0x40; //ON
					root_pin_functions->PIN_STATUS |= 0b00001000;
				}else{ 
					PORTD &= ~0x40; //OFF
					root_pin_functions->PIN_STATUS &= ~0b00001000;
				}
				id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
			}
		}
	}else
		
	//ID=4 //PC4 = PIN7
	if(num_pin_to_send_status == 7){ 
		if((root_pin_functions->PIN_MASK & 0b00010000) != 0){ //check if it is an output
			if(array_cmd[8] > 0){ //check value to set
				PORTC |= 0x10; //ON
				root_pin_functions->PIN_STATUS |= 0b00010000;
			}else{ 
				PORTC &= ~0x10; //OFF
				root_pin_functions->PIN_STATUS &= ~0b00010000;
			}
			id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
		}
	}else
	
	//ID=5 //PD1 = PIN8
	if(num_pin_to_send_status == 8){ 
		if((root_pin_functions->PIN_MASK & 0b00100000) != 0){ //check if it is an output
			if(array_cmd[8] > 0){ //check value to set
				PORTD |= 0x02; //ON
				root_pin_functions->PIN_STATUS |= 0b00100000;
			}else{ 
				PORTD &= ~0x02; //OFF
				root_pin_functions->PIN_STATUS &= ~0b00100000;
			}
			id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
		}
	}else
		
	//ID=6 //PD5 = PIN9
	if(num_pin_to_send_status == 9){ 
		if((root_pin_functions->PIN_MASK & 0b01000000) != 0){ //check if it is an output
			if((root_pin_functions->SEM_PWM & 0b01000000) != 0){ //check if it is used as PWM
				set_duty_cycle_RGB_LED((int)-1, (int)array_cmd[8], (int)-1);	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
				id_data_eeprom_to_update |= 0b0000000000010000;
			}else{
				if(array_cmd[8] > 0){ //check value to set
					PORTD |= 0x20; //ON
					root_pin_functions->PIN_STATUS |= 0b01000000;
				}else{ 
					PORTD &= ~0x20; //OFF
					root_pin_functions->PIN_STATUS &= ~0b01000000;
				}
				id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
			}
		}
	}else

	//ID=7 //PC5 = PIN10
	if(num_pin_to_send_status == 10){ 
		if((root_pin_functions->PIN_MASK & 0b10000000) != 0){ //check if it is an output
			if(array_cmd[8] > 0){ //check value to set
				PORTC |= 0x20; //ON
				root_pin_functions->PIN_STATUS |= 0b10000000;
			}else{ 
				PORTC &= ~0x20; //OFF
				root_pin_functions->PIN_STATUS &= ~0b10000000;
			}
			id_data_eeprom_to_update |= 0b0000000000000001; //make to update the data into the EEPROM
		}
	}

	
	if(array_cmd[7] < 8 ){
		//this make blink the led to notify the command has been received correctly
		Led_TX_OFF_and_after_delay_turn_ON();
	}
	
}


//Function to set the parameters of a special function
void f_SetFunction(void){
	
	unsigned char i,l;
	unsigned int u16_Temp;

	//1st function: set parameters for the timer that control one of the output
	if(cmd_rfpi[7] == 1){
		
		semRelayTimer = cmd_rfpi[8]; 	//function status. at 1 is enabled out 0, at 2 is enabled out 1 and so on
		u16_Temp = (unsigned int)cmd_rfpi[9];
		u16_Temp = u16_Temp << 8;
		u16_Temp |= (unsigned int)cmd_rfpi[10];
		RelayTimer_10ms = (unsigned char)(u16_Temp / 10);	//if( RelayTimer_10ms > 100 ){	RelayTimer_10ms = 0; }
		RelayTimer_SS = (signed char)cmd_rfpi[11];			//if( RelayTimer_SS > 60 ){	RelayTimer_SS = 60; }
		RelayTimer_MM = cmd_rfpi[12];						//if( RelayTimer_MM > 60 ){	RelayTimer_MM = 60; }
		RelayTimer_HH = cmd_rfpi[13];

		//update_eeprom_peri();
		id_data_eeprom_to_update |= 0b0000000001000000;
		
	}else

	//3rd function: input duty
	if(cmd_rfpi[7] == 2){
		//digital input control a digital output
		//duty data for input ID=0 to ID=7
		for(i=0;i<10;i++){
			fun_input_ctrl_output[i] = cmd_rfpi[i+8];		
		}
		//meaning bits inside fun_input_ctrl_output from byte 0 to byte 7
		//	bit0 to bit2	=	ID of the input that control the output
		//	bit3 to bit5 	= 	ID of the output to control
		//	bit6 to	bit7	=	Status to set to the output: 0 = OFF, 1 = ON, 2 = TOGGLES, 
		//every 2bit into byte 8 and byte 9 there is the status for each of the 8 function "Input Duty"
		//example: byte 8 of the array fun_input_ctrl_output (so the array point to fun_input_ctrl_output[8])
		// bit0 to bit1		=	Input Trigger: 0 = Disabled, 1 = Rising edge, 2 = Falling edge, 3 = Both. This are relate to the fun_input_ctrl_output[0]
		// bit2 to bit3		=	Input Trigger: 0 = Disabled, 1 = Rising edge, 2 = Falling edge, 3 = Both. This are relate to the fun_input_ctrl_output[1]
		//	........ and so on
		
		
		//update_eeprom_peri();
		id_data_eeprom_to_update |= 0b0000000010000000;
		
	}else
		
	//3th function: this enable/disable some features of the platform
	if(cmd_rfpi[7] == 3){
		//getting the settings of the platform
		sem_Led_TX_keep_OFF = cmd_rfpi[8]; //get the setting for the status LED
		PIN_LED_TX_OFF;

		//update_eeprom_peri();
		id_data_eeprom_to_update |= 0b0000000100000000;
		
	}
	
	//4th function: this set Values on 16bits that are the threshold to control an output
	if(cmd_rfpi[7] == 4){
		for(i=0;i<10;i++){
			fun_threshold_ctrl_output[i] = cmd_rfpi[i+8];
		}

		#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
		for(i=0;i<2;i++){
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*",1);UART_DEBUG_send_STARS(60);
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*   RECEIVED FUNCTION THRESHOLD ",1); //it send a string through the UART only if it is in debug mode
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)i, 0); //status to set
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*   STATUS FUNCTION = ",1); //it send a string through the UART only if is in debug mode
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)fun_threshold_ctrl_output[i*5], 0); //status to set
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"             *",0);
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*   THRESHOLD_0 LOW = ",1); //it send a string through the UART only if is in debug mode
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)fun_threshold_ctrl_output[(i*5)+2], 0); //MSB
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ",0);
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)fun_threshold_ctrl_output[(i*5)+1], 0); //LSB
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*   THRESHOLD HIGH_0 = ",1); //it send a string through the UART only if is in debug mode
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)fun_threshold_ctrl_output[(i*5)+4], 0); //MSB
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ",0);
			UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)fun_threshold_ctrl_output[(i*5)+3], 0); //LSB
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)"*",1);UART_DEBUG_send_STARS(60);
		}
		#endif

		i = (unsigned char)((fun_threshold_ctrl_output[0] >> 3) & 0x07); //ID output to control of the first function
		l = (unsigned char)((fun_threshold_ctrl_output[5] >> 3) & 0x07); //ID output to control of the second function
		if(i == l){ //for both fuctions the output to control is same thus I am going to disable the second threshold function
			fun_threshold_ctrl_output[5] = fun_threshold_ctrl_output[5] | 0b11000000; //disabling the second threshold fuction because both fuctions controls the same output
			#ifdef UART_DEBUG_MANAGER_FUNCTIONS_THRESHOLDS
			UART_DEBUG_MANAGER_send_STR2((unsigned char *)" ERROR: the 2 Thresholds Functions that controls the same output! Thus I am going to disable the second Threshold Function!",1);
			#endif
		}


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

		id_data_eeprom_to_update |= 0b0000001000000000; //into the manager.c it will allow to update the eeprom with the value just gotten

	}


//	//5th function: free
//	if(cmd_rfpi[7] == 5){
//
//		//EXAMPLE:
//		//trigger_temperature_low_8bit = cmd_rfpi[9]; //
//		//trigger_temperature_high_8bit = cmd_rfpi[11]; //
//		//temperature_offset_to_send_data_temperature = cmd_rfpi[13]; //
//
//		id_data_eeprom_to_update |= 0b0000010000000000;
//
//	}


	if(cmd_rfpi[7] == 254){ //ID function 254 (radio control)
	
		sem_address_assigned=1;
		
		//Updating EEPROM
		update_eeprom_addresses();
		
		set_current_network_Gateway(); //set the addresses	
	
		programming_mode_enabled = 0; //exit from program mode
	}

	
}		



//Function to send the status of a special function
void u_SendFunctionStatus(void){
	//here you have to write the code to read the input and send back the status
	
	unsigned char data[16],i;
	unsigned int u16_Temp;
	
	data[0] = 'R';
	data[1] = 'B';
	data[2] = 'u';
	data[3] = cmd_rfpi[7];		//identifier function

	//1st function: timer parameters
	if(cmd_rfpi[7] == 1){
		//this make blink the led to notify the command has been received correctly
		Led_TX_OFF_and_after_delay_turn_ON();

		data[4] = semRelayTimer; 	//function status. first bit is enabled for output0, the second is enable for output 1
		u16_Temp = (unsigned int)(RelayTimer_10ms * 10);
		data[5] = (unsigned char)(u16_Temp >> 8);
		data[6] = (unsigned char)u16_Temp;
		data[7] = (unsigned char)RelayTimer_SS;
		data[8] = RelayTimer_MM;
		data[9] = RelayTimer_HH;

		UART_send_and_fill_data_to_RF((unsigned char *)data, 10);
		
	}else
	
	//2nd function: Input duty
	if(cmd_rfpi[7] == 2){
		for(i=0;i<10;i++){
			 data[i+4] = fun_input_ctrl_output[i];
		}
		UART_send_and_fill_data_to_RF((unsigned char *)data, 14);
		
	}else
		
	//3th function: this enable/disable some features of the platform
	if(cmd_rfpi[7] == 3){
		data[4] = sem_Led_TX_keep_OFF;//status function

		data[13] = (unsigned char)(ADC1_Average>>8);
		data[14] = (unsigned char)(ADC1_Average); //MSB

		UART_send_and_fill_data_to_RF((unsigned char *)data, 15);
		
	}

	//4th function: this Values on 16bits that are the threshold to control an output
	if(cmd_rfpi[7] == 4){
		for(i=0;i<10;i++){
			 data[i+4] = fun_threshold_ctrl_output[i];
		}
		UART_send_and_fill_data_to_RF((unsigned char *)data, 14);

	}else


	if(cmd_rfpi[7] == 254){//ID function 254 (radio control)
		data[0] = 'R';
		data[1] = 'B';
		data[2] = 'u';
		data[3] = 254;		//identifier function
		
		data[4] = sem_address_assigned;		//status function

		data[5] = (unsigned char)hexascii_to_byte((unsigned char *)net_add); 		//H
		data[6] = (unsigned char)hexascii_to_byte((unsigned char *)(net_add+2));	//L
		data[7] = (unsigned char)hexascii_to_byte((unsigned char *)peri_add); 		//H
		data[8] = (unsigned char)hexascii_to_byte((unsigned char *)(peri_add+2));	//L
		data[9] = (unsigned char)hexascii_to_byte((unsigned char *)rfpi_add); 		//H
		data[10] = (unsigned char)hexascii_to_byte((unsigned char *)(rfpi_add+2));	//L
		
		UART_send_and_fill_data_to_RF((unsigned char *)data, 11);
	}
	
}


//Function to load Shields settings that are connected to the transceiver
void s_Shields(void){
	//here you have to write the code to save data for the shields
	
	unsigned char eeprom_position;
	
	#ifdef UART_DEBUG_RFPIMCU
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"******* SAVING SHIELD INTO POSITION ",1); //it send a string through the UART only if is in debug mode
	UART_DEBUG_send_NUM_BYTE_HEX(cmd_rfpi[7], 0);
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)" EEPROM *******",0);
	#endif
		
	if(cmd_rfpi[7] < 8){  
	
		//if(TYPE_L != BAD_TRANSCEIVER){

			eeprom_position = cmd_rfpi[7];		
			
			cli(); //disabilita gli interrupt
			for(int i=0;i<NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD;i++){
				wdt_reset(); //it reset the WDT
				eeprom_write_byte ((unsigned char *)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+i)+(eeprom_position*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)), (unsigned char)cmd_rfpi[8+i]); //
			
				#ifdef UART_DEBUG_RFPIMCU
				UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"     ADDRESS=",1); //it send a string through the UART only if is in debug mode
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)((START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD+i)+(eeprom_position*NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD)), 0);
				UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)",  DATA=",0);
				UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)cmd_rfpi[8+i], 0);
				#endif
			}
			sei(); //Abilita gli interrupts globali

		//}
	
		//this make blink the led to notify the command has been received correctly
		Led_TX_OFF_and_after_delay_turn_ON();
	}
	
	#ifdef UART_DEBUG_RFPIMCU
	UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"*",1);
	UART_DEBUG_send_STARS(70);
	#endif

	
}


//Function make the transceiver to reboot itself
void b_Reboot(void){
	//here you have to write the code to reboot the mcu

	//this make blink the led to notify the command has been received correctly
	Led_TX_OFF_and_after_delay_turn_ON();
	
	if( (cmd_rfpi[7] & 0b00000001) == 0){  
		unsigned char var_temp_byte = eeprom_read_byte((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES));
		var_temp_byte &= 0b11111110; //here enable the programming IoT network
		#ifdef UART_DEBUG_RFPIMCU
			UART_DEBUG_RFPIMCU_send_STR(36, (unsigned char *)"Enabled the programming IoT network!",1); //it send a string through the UART only if is in debug mode
		#endif
		eeprom_write_byte ((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES), (unsigned char )var_temp_byte);
	}
	#ifdef UART_DEBUG_RFPIMCU
		UART_DEBUG_RFPIMCU_send_STR(23, (unsigned char *)"Going to reset ........",1); //it send a string through the UART only if is in debug mode
	#endif			
	var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
	while(1);
	
}


void load_eeprom_peri(void){
	
	//here you have to write the code to read the
	//read eeprom of your MCU and restore the last addresses 
	//and status of the outputs
}


void update_eeprom_peri(void){
	
	 //here you have to write the code to write the
	 //write the eeprom of your MCU with the current addresses 
	 //and status of the outputs

}


//***********************************************************************************
//	HERE END THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************






//***********************************************************************************
//	THE FOLLOWING FUNCTIONS NEEDS TO BE MODIFIED IN ORDER TO USE ANOTHER MCU
//***********************************************************************************

//function to execute when occur the interrupt of data comming from the UART
void interrupt_uart0_data_from_RF(unsigned char byte){
	//here write the code to fill up the 20byte buffer
	
	//description variables to assign the value:
	//	byte: 				assign the 8bits register where is kept the data of your UART
	//	uart_receive_cnt: 	is the counter, it is incremented each time when a byte is received
	//						this counter is erased after the decontTimeOutSerialData is equal to 0 
	//						or when the command will be parsed
	//  uart_receive_buffer: it contain all bytes received up to 20, because the data are 16bytes 
	//						 and the address of the transmitter are 4bytes
	

		//the follwing code should be valid for all MCU
		decontTimeOutSerialData = TIME_OUT_UART_DATA;
		uart_receive_buffer[uart_receive_cnt] = byte;
		
		#ifdef UART_DEBUG_RFPIMCU
			//USART_SendByte((unsigned char)byte);
			//UART_DEBUG_RFPIMCU_send_STR(33, (unsigned char *)"BEGIN TO SET PROGRAMMING NETWORK!",1); //it send a string through the UART only if is in debug mode
		#endif
		
		
		if (uart_receive_cnt < SIZE_BUFFER_UART){
			uart_receive_cnt++; 
		}

}


//it send a byte through the UART to the radio module 
//(you have to write into this function the command for your MCU)
void 	UART_send_to_RF(unsigned char data){
	USART_SendByte(data);

	//code compatible with all MCU 
	Led_TX_OFF_and_after_delay_turn_ON();
		
}




void load_eeprom_addresses(void){
	unsigned char temp_dato_eeprom[7];
	unsigned char i;
	unsigned char cont_if_virgin=0;
	
	for(i=0; i<7; i++){
		temp_dato_eeprom[i] = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+i));
		if(temp_dato_eeprom[i]==0xFF){
			cont_if_virgin++;
		}
	}
	if(cont_if_virgin==7){ //then the EEPROM is virgin thus update with the default addresses (normally are all 0)
		for(i=0; i<7; i++){
			eeprom_write_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+i), eeprom_def_tab_addresses[i]);
			temp_dato_eeprom[i] = eeprom_def_tab_addresses[i];
		}
	}
	
	sem_address_assigned = temp_dato_eeprom[6] & 0x01;
	
	//load the addresses to RAM
	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[0], (unsigned char *)net_add); 
 	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[1], (unsigned char *)(net_add+2)); 
	
	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[2], (unsigned char *)peri_add); 
 	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[3], (unsigned char *)(peri_add+2)); 
	
	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[4], (unsigned char *)rfpi_add); 
 	byte_to_hexascii((unsigned char *)&temp_dato_eeprom[5], (unsigned char *)(rfpi_add+2)); 

}


void update_eeprom_addresses(void){
	unsigned char tempDato;
	
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)net_add);
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x00), tempDato);
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)(net_add+2));
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x01), tempDato);
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)peri_add);
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x02), tempDato);
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)(peri_add+2));
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x03), tempDato);
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)rfpi_add);
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x04), tempDato);
	tempDato=(unsigned char)hexascii_to_byte((unsigned char *)(rfpi_add+2));
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x05), tempDato);
	
	tempDato = (unsigned char)eeprom_read_byte((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x06)); //on byte 7th there are saved the flags for the RFPI network 
	eeprom_write_byte ((unsigned char *)(START_ADDRESS_WHERE_TO_SAVE_NETWORK+0x06), (tempDato|sem_address_assigned));
				
}				


//***********************************************************************************
//	HERE END THE FOLLOWING FUNCTIONS NEEDS TO BE MODIFIED IN ORDER TO USE ANOTHER MCU
//***********************************************************************************






//***********************************************************************************
//	THE FOLLOWING FUNCTION SHOULD BE COMPATIBLE WITH ALL MCU
//***********************************************************************************

//it init the radio
void init_radio_RF(void){
	char *str;
	char CMD_Str[6];
	char Str_Channel[3];
	CMD_Str[5]='\0';
	Str_Channel[2]='\0';

	//init global variables
	programming_mode_enabled = 0;
	decontTimeOutSerialData = 0;
	uart_receive_cnt = 0;
	cont_cmd_rfpi = 0;

	
	//this semaphore is used into the infinite loop
	sem_radio_module_connected = 1;
	
	//Set the Radio Transceiver to reply '*' when a command sent has been executed correctly
	//UART_send_CMD_to_RF(4, (unsigned char *)"C151");

	//Set number of byte to send each transmission
	UART_send_CMD_to_RF(5, (unsigned char *)"C0416");
	
	//Set TX power
	UART_send_CMD_to_RF(4, (unsigned char *)"C073");
	
	//Set the channel to use. The CH=1 is the default to operate
	//UART_send_CMD_to_RF(5, (unsigned char *)"C0001");
	if(RADIO_CHANNEL<10){
		strcpy((char *)CMD_Str,(const char *)"C000");
		itoa(RADIO_CHANNEL, (char *)Str_Channel,10);
		str=strcat((char *)CMD_Str,(char *)Str_Channel);
	}else{
		if(RADIO_CHANNEL<18){
			strcpy((char *)CMD_Str,(const char *)"C00");
			itoa(RADIO_CHANNEL, (char *)Str_Channel,10);
			str=strcat((char *)CMD_Str,(char *)Str_Channel); 
		}else{
			strcpy((char *)CMD_Str,(const char *)"C0001"); //default channel
		}
	}
	UART_send_CMD_to_RF(5, (unsigned char *)CMD_Str);


	
	//Init the radio on the Radio Transceiver module
	//UART_send_CMD_to_RF(3, (unsigned char *)"C08");

	//Set the Radio Transceiver to reply '*' when a command sent has been executed correctly
	UART_send_CMD_to_RF(4, (unsigned char *)"C151");

	//Set auto AKN to on
	UART_send_CMD_to_RF(4, (unsigned char *)"C091");

	//Set frequency hopping off
	UART_send_CMD_to_RF(4, (unsigned char *)"C100");

	//Activating waiting for AKN
	UART_send_CMD_to_RF(4, (unsigned char *)"C111");

	//Set maximum waiting time to 10mS
	UART_send_CMD_to_RF(7, (unsigned char *)"C200100");

	//Set maximum waiting time for AKN to 100mS
	UART_send_CMD_to_RF(7, (unsigned char *)"C210001");

	//Set to 5mS the waiting time between two transmission when an error occur
	UART_send_CMD_to_RF(7, (unsigned char *)"C220050");

	//Set to 2 the maximum number of retry
	UART_send_CMD_to_RF(5, (unsigned char *)"C2302");

	//Set the module Radio Transceiver to answer OK when there is a successful transmission
	UART_send_CMD_to_RF(4, (unsigned char *)"C291");

	//loading the addresses from the eeprom
	load_eeprom_addresses(); //load network addresses

	//after have loaded the addresses from the eeprom it is possible to setup the network
	set_current_network_Gateway();

	//Init the radio on the Radio Transceiver module
	//UART_send_CMD_to_RF(3, (unsigned char *)"C08");

	//save settings into memory
	UART_send_CMD_to_RF(3, (unsigned char *)"C45"); //store permanently into eeprom the settings

	//this make blink the led to notify the radio has been initialised
	Led_TX_OFF_and_after_delay_turn_ON();

}

//set the current network addresess
void set_current_network_Gateway(void){
	
	//Set the network address. net_add are 4 characters representing hexadecimal format.
	UART_send_CMD_to_RF(3, (unsigned char *)"C01"); UART_send_STR_to_RF(4, (unsigned char *)net_add);
	
	//Set the peripheral address. peri_add are 4 characters representing hexadecimal format.
	UART_send_CMD_to_RF(3, (unsigned char *)"C02"); UART_send_STR_to_RF(4, (unsigned char *)peri_add);
	
	//Set the destination address (Gateway address). rfpi_add are 4 characters representing hexadecimal format.
	UART_send_CMD_to_RF(3, (unsigned char *)"C03"); UART_send_STR_to_RF(4, (unsigned char *)rfpi_add);
	
	//set lenght of the payload transmission
	UART_send_CMD_to_RF(5, (unsigned char *)"C0416"); 
	
	//set the TX power
	UART_send_CMD_to_RF(5, (unsigned char *)"C0073"); 
	
	//Set the channel to communicate with the RFberry Pi (in default network is 5 in normal mode is 1)
	UART_send_CMD_to_RF(5, (unsigned char *)"C0001");
	
	//Init the radio on the Radio Transceiver module
	UART_send_CMD_to_RF(3, (unsigned char *)"C08");
	
}

//set the programming network addresess
void set_programming_network_Gateway(void){
	//Set the network address. 'FFFC' are 4 characters representing hexadecimal format. 
	UART_send_CMD_to_RF(7, (unsigned char *)"C01FFFC");
	
	//Set the peripheral address. 'FFFC' are 4 characters representing hexadecimal format.
	UART_send_CMD_to_RF(7, (unsigned char *)"C02FFFC");
	
	//Set the destination address (Gateway address). 'FFFC' are 4 characters representing hexadecimal format.
	UART_send_CMD_to_RF(7, (unsigned char *)"C03FFFC"); 
	
	//set lenght of the payload transmission
	UART_send_CMD_to_RF(5, (unsigned char *)"C0416"); 
	
	//set the TX power
	UART_send_CMD_to_RF(5, (unsigned char *)"C0072"); 
	
	//Set the channel to communicate with the RFberry Pi, because this is the programming network (to add a new peripheral) the channel to use is 5
	UART_send_CMD_to_RF(5, (unsigned char *)"C0005");
	
	//Init the radio on the Radio Transceiver module
	UART_send_CMD_to_RF(3, (unsigned char *)"C08");
	
}


//it send a string through the UART to the radio module 
void 	UART_send_STR_to_RF(unsigned char num_data, unsigned char *data){
	unsigned char i;
	for(i=0;i<num_data;i++){
		execute_USART_CMD(*(data+i));
	}	
	function_cmd_to_run();						//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt
}


//it send a string command through the UART to the radio module 
void 	UART_send_CMD_to_RF(unsigned char num_data, unsigned char *data){
	unsigned char i;
	for(i=0;i<num_data;i++){
		//UART_send_to_RF(*(data+i));
		execute_USART_CMD(*(data+i));
	}	
	function_cmd_to_run();						//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt
}


//it check the UART buffer if there is data, return 0 if there is no data, and 1 if there is data
//(you have to write into this function the command for your MCU)
unsigned char	UART_check_data_RF(void){
	if(uart_receive_cnt>0){
		return 1;
	}else{
		return 0;	
	}
}


//get the last byte from the UART buffer
//(you have to write into this function the command for your MCU)
unsigned char	UART_get_last_byte_RF(void){
	return uart_receive_buffer[uart_receive_cnt-1];
}


//convert a number written with ASCII character in a unsigned int number
unsigned char hexascii_to_byte(unsigned char*strHextoConvert){
  unsigned char byteConvertedFromHex;
  unsigned char i,j,fourBit;
  fourBit = 0;

	//converting the 4 most significant bits
	j=48;
	i=0;
	while(i<16){
		if(j == strHextoConvert[0]){ 
			fourBit = i; 
		}
		i=i+1;
		j=j+1;
		if( j == 58)
			j=65;
	}
	
	byteConvertedFromHex = fourBit;
	byteConvertedFromHex = byteConvertedFromHex << 4;
	
	//converting the 4 less significant bits
	j=48;
	i=0;
	while(i<16){
		if(j == strHextoConvert[1]){ 
			fourBit = i ;
		}
		i=i+1;
		j=j+1;
		if (j == 58)
			j=65;
	}
	
	byteConvertedFromHex = byteConvertedFromHex | fourBit;
		
 	return (unsigned char)byteConvertedFromHex;
}


//convert a number in HExadecimal format but written with ASCII characcters, the strHexConverted is an array long at least 2
//if the number is 0x3F then strHexConverted[0] = '3' and strHexConverted[1] = 'F'
void byte_to_hexascii(unsigned char *byteToConvert, unsigned char *strHexConverted){
	unsigned char k;
	
	k=*byteToConvert;
				
	if(k == 0){
		strHexConverted[0] = 48;
		strHexConverted[1] = 48;
	}else{
		//the 4bit most significant
		k = k & 0b11110000;
		k = k >> 4;
		
		if(k>9){
			if(k==10)
				strHexConverted[0] = 'A';
			else if(k==11)
				strHexConverted[0] = 'B';
			else if(k==12)
				strHexConverted[0] = 'C';
			else if(k==13)
				strHexConverted[0] = 'D';
			else if(k==14)
				strHexConverted[0] = 'E';
			else if(k==15)
				strHexConverted[0] = 'F';
			
		}else{
			strHexConverted[0] = 0x30 + k;
		}
		
		//the 4bit less significant
		k=*byteToConvert;
		k = k & 0b00001111;
		
		if(k>9){
			if(k==10)
				strHexConverted[1] = 'A';
			else if(k==11)
				strHexConverted[1] = 'B';
			else if(k==12)
				strHexConverted[1] = 'C';
			else if(k==13)
				strHexConverted[1] = 'D';
			else if(k==14)
				strHexConverted[1] = 'E';
			else if(k==15)
				strHexConverted[1] = 'F';
			
		}else{
			strHexConverted[1] = 0x30 + k;
		}
		
	}
}



void decnum_to_hexascii(unsigned char*byteToConvert, unsigned char *strHexConverted){
	unsigned char k,j,i,t,l;
	
	k=*byteToConvert;
				
	if(k == 0){
		strHexConverted[0] = 48;
		strHexConverted[1] = 48;
	}else{
		j = 1;
		i = k;
		while(i > 0){
			i = i / 10;
			j = j * 10;
		}
		t = 0;
		j = j / 10;
		l = 0;
		while(j > 0){
			i = k;
			i = i / j;
			i = i - t;
			t = (t + i) * 10;
			j = j / 10;
			strHexConverted[l] = i+48;
			l++;
		}
	}
}

unsigned char check_bad_RF(void){
 	 unsigned char a,b,i;
 
 	a = 1;
	
	uart_receive_cnt = 0; //resetting the counter of the bytes received
	
 	UART_send_CMD_to_RF(3, (unsigned char *)"C85"); 
	i=100;
	while(i>0 && a==1){
		if(UART_check_data_RF()==1){ //function from rfberrypi_mcu.c library
			b = UART_get_last_byte_RF();
			if(b==0x2A){ //0x2A = '*'
				//if get * then the module is NOT BAD!
				a = 0;	
			}
		}
		delay_ms_RFPI(1);
		i--;
	}
 	
 	return a;
}


//it will get the addresses of the network (when the peripheral has been set in programming mode)
void w_GetGatewayNetwork(void){
	unsigned char var_temp_byte;
	
	byte_to_hexascii((unsigned char *)&cmd_rfpi[7], (unsigned char *)net_add);
	byte_to_hexascii((unsigned char *)&cmd_rfpi[8], (unsigned char *)(net_add+2));
	byte_to_hexascii((unsigned char *)&cmd_rfpi[9], (unsigned char *)peri_add);
	byte_to_hexascii((unsigned char *)&cmd_rfpi[10], (unsigned char *)(peri_add+2));
	byte_to_hexascii((unsigned char *)&cmd_rfpi[11], (unsigned char *)rfpi_add);
	byte_to_hexascii((unsigned char *)&cmd_rfpi[12], (unsigned char *)(rfpi_add+2));
	
	sem_address_assigned = 1; //for the radio control
	
	//save into EEPROM
    update_eeprom_addresses();
	
	set_current_network_Gateway(); //set the new addresses
		
	programming_mode_enabled = 0; //exit from programming mode
	decont_programming_mode_enabled = 0;
	var_temp_byte = var_EEPROM_Semaphore | 0b00000001; //here disable the programming IoT network
	eeprom_write_byte ((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES), (unsigned char )var_temp_byte);
	
	//decontLedTX = LOGIC_LED; //make the led to turn on
	Led_TX_OFF_and_after_delay_turn_ON();
}			


//Function to reply with the identification parameters of our periphera
void t_PeriParameters(void){
	 unsigned char data[16];
	
	//this make blink the led to notify the command has been received correctly
	Led_TX_OFF_and_after_delay_turn_ON();
		
	//16bytes to send
	data[0] = 'R';
	data[1] = 'B';
	data[2] = 't';
	data[3] = TYPE_H;				//it is the most significant byte of the univocal identifier  
	data[4] = TYPE_L;				//it is the less significant byte of the univocal identifier 
	data[5] = NUM_DIGITAL_INPUT;	//how many digital input has
	data[6] = NUM_DIGITAL_OUTPUT;	//how many digital output has
	data[7] = NUM_ANALOGUE_INPUT;	//how many digital input has
	data[8] = NUM_ANALOGUE_OUTPUT;	//how many digital output has
	data[9] = NUM_FUNCTIONS;		//how many functions
	data[10] = FIRMWARE;			//firmware release
	data[11] = RADIO_CHANNEL;		//channel used to operate normally
	
	UART_send_and_fill_data_to_RF((unsigned char *)data, 12);
}


//Function to reply with the name of this peripheral
void n_SendPeriName(void){
	 unsigned char data[17],i;
	 char *str;

	//this make blink the led to notify the command has been received correctly
	Led_TX_OFF_and_after_delay_turn_ON();


	data[0] = 'R';
	data[1] = 'B';
	data[2] = 'n';
		
	for(i=3;i<17;i++)	//filling the data not assigned
		data[i] = '\0';

	//here add name
	str=strcat((char *)data,(const char *)NAME_PERIPHERAL); //max name lenght has to be equal or lower than 12 characters

	UART_send_and_fill_data_to_RF((unsigned char *)data, 16);
}

	
//here check if has been received 20byte from the UART, 
//because 16bytes are the data and the first 4 bytes are the address of the transmitter (Gateway)
//then it will parse the data received, the data obviously contain the command
//and consecutively will execute the command			
void check_and_parse_cmd_from_Gateway(void){
	unsigned char array_cmd_rfpi[16];
	unsigned char j;

	//it reads uart_receive_buffer[] buffer and if there is a command it store the command into cmd_rfpi[] and set cont_cmd_rfpi=20
	//when cont_cmd_rfpi it is equal to 0 then will search again for a command into uart_receive_buffer[]
	get_cmd_from_uart_buffer();

	//parsing the command received from Gateway
	if(/*cmd_rfpi[4]=='R' && cmd_rfpi[5]=='B' &&*/ cont_cmd_rfpi == 20){
		//cmd_rfpi[4] it is used for the checksum since the modification done on 27/11/2020
		for(j=0;j<16;j++) array_cmd_rfpi[j]=cmd_rfpi[j+4];
		if(checksum2(array_cmd_rfpi, 16) != 0){
			//error into data
			cont_cmd_rfpi = 0;
		}else{

			//enable the 2 following line if you want answer to the tranmitter, that usually is the Gateway
			//but pay attention to give back the address of the Gateway, thus when your peripheral
			//has to communicate for a change will communicate to the Gateway and the data will be not lost
			//UART_send_CMD_to_RF(3, "C03"); //command to set the address where to send the data
			//UART_send_STR_to_RF(4, cmd_rfpi);//giving the address to the radio after the C03

			#ifdef UART_DEBUG_RFPIMCU
				unsigned char hex_value[2];
				UART_DEBUG_RFPIMCU_send_STR(26, (unsigned char *)"RECEIVED DATA FROM RADIO: ",1); //it send a string through the UART only if is in debug mode
				for(int i=0; i<cont_cmd_rfpi; i++){
					if(i<4){
						USART_SendByte((unsigned char)cmd_rfpi[i]);
					}else if(i==4){
						USART_SendByte((unsigned char)' ');
						USART_SendByte((unsigned char)cmd_rfpi[i]);
					}else if(i==5){
						USART_SendByte((unsigned char)cmd_rfpi[i]);
					}else if(i==6){
						USART_SendByte((unsigned char)' ');
						USART_SendByte((unsigned char)cmd_rfpi[i]);
					}else{
						byte_to_hexascii((unsigned char *)&cmd_rfpi[i], hex_value);
						USART_SendByte((unsigned char)' ');
						USART_SendByte((unsigned char)'0');USART_SendByte((unsigned char)'x');
						USART_SendByte((unsigned char)(hex_value[0]));
						USART_SendByte((unsigned char)(hex_value[1]));
					}
				}
				USART_SendByte(0x0D); //carriage return
				USART_SendByte(0x0A); //line feed
			#endif
		
		
			if(programming_mode_enabled == 1){
				decont_programming_mode_enabled = MAX_TIME_PROGRAMMING_MODE_ENABLED;
			}else{
				//load the address of the sender
				//UART_send_CMD_to_RF(3, "C03"); UART_send_STR_to_RF(4, sender_add);
			}

			if(cmd_rfpi[6]=='o'){
				#ifdef UART_DEBUG_RFPIMCU
					//UART_DEBUG_RFPIMCU_send_STR(33, (unsigned char *)"CALLING FUNCTION: o_SetOut(void);",1);
					UART_DEBUG_RFPIMCU_send_STR(33, (unsigned char *)"CALLING FUNCTION: o_SetOut(cmd_rfpi);",1);
				#endif
				o_SetOut((unsigned char *)cmd_rfpi,1); //Function to set the status of an output
			}else if(cmd_rfpi[6]=='i'){
				i_SendInStatus(); //Function to send the status of an input
			}else if(cmd_rfpi[6]=='p'){
				p_SendOutStatus(); //Function to send the status of an output
			}else if(cmd_rfpi[6]=='w'){
				w_GetGatewayNetwork(); //save the addresses of the network assigned by the Gateway
			}else if(cmd_rfpi[6]=='t'){
				t_PeriParameters();	//Function to reply with the identification parameters of our peripheral
			}else if(cmd_rfpi[6]=='n'){
				n_SendPeriName();	//Function to reply with the name of this peripheral
			}else if(cmd_rfpi[6]=='f'){
				f_SetFunction();	//Function to set the parameters of a special function
			}else if(cmd_rfpi[6]=='u'){
				u_SendFunctionStatus();	//Function to send the status of a special function
			}else if(cmd_rfpi[6]=='s'){
				s_Shields();	//Function to load Shields settings that are connected to the transceiver
			}else if(cmd_rfpi[6]=='b'){
				b_Reboot();	//Function make the transceiver to reboot itself
			}
			
			//after have parsed the command delete it
			cont_cmd_rfpi = 0;
			
			//enable the 2 line command below to set back the address of the Gateway (master)
			//UART_send_CMD_to_RF(3, "C03"); //command to set the address where to send the data
			//UART_send_STR_to_RF(4, rfpi_add);//giving the address to the radio after the C03

		}//END: if per checksum2
	}

	//turn on the TX if the time has expired
	if(decontLedTX==0 && programming_mode_enabled==0){
		if((sem_Led_TX_keep_OFF&0x01)!=0)
			PIN_LED_TX_ON; //led on
	}
}	

	
//this function check if the reset is caused by the button, 
//if the it is caused by the button then check if the button is still pressed
//if the button is not pressed then it start the installation into the IoT network
//if the button is still pressed then wait until is release and check if the minimum time has been passed to reset addresses:
//if the minimum time is passed then reset the all addresses to 0x00 and blink the led otherwise it start the installation into the IoT network
unsigned char check_btn_to_enter_in_programming(unsigned char var_Semaphore){
	unsigned char var_return = 0;
	unsigned char var_temp_byte;
	unsigned int cont_ms_elapsed=0;

	if( (var_Semaphore&0b00000001) == 0 ){
		//check if the button has been pressed
		while( (PIN_BTN_PROGRAMMING_MODE == LOGIC_BTN_PROGRAMMING_MODE) && (cont_ms_elapsed < TIME_BTN_TO_RESET_ADDRESSES) ){
			wdt_reset(); //it reset the WDT
			_delay_ms(1);
			cont_ms_elapsed++;
		}
		wdt_reset(); //it reset the WDT
		if(cont_ms_elapsed < TIME_BTN_TO_RESET_ADDRESSES){
			#ifdef UART_DEBUG_RFPIMCU
			UART_DEBUG_RFPIMCU_send_STR(56, (unsigned char *)"It will init and then set the programming network.......",1); //it send a string through the UART only if is in debug mode
			#endif
			var_return = 1;
			PIN_LED_TX_OFF; //led off
			sem_Used_As_Peripheral = 1;
		}else{
			//it reset the EEPROM
			reset_eeprom();

			var_temp_byte=0;
			while( PIN_BTN_PROGRAMMING_MODE == LOGIC_BTN_PROGRAMMING_MODE ){
				if( var_temp_byte > 50 ){
					var_temp_byte=0;
					if(STATUS_LED_TX){
						PIN_LED_TX_OFF; //led off
					}else{
						PIN_LED_TX_ON; //led on
					}
				}else var_temp_byte++;
				wdt_reset(); //it reset the WDT
				_delay_ms(1);
			}

			wdt_reset(); //it reset the WDT
			var_temp_byte = var_Semaphore | 0b00000001; //here disable the programming IoT network
			eeprom_write_byte ((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES), (unsigned char )var_temp_byte);
			var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
			while(1);
		}

	}else{
		PIN_LED_TX_ON; //led on
	}//end if: Semaphore
	
	return var_return;
}			

		
//this interrupt has to be called into an interrupt of a timer that occur around every 1mS
void interrupt_timer_of_1mS_RFPI(void){
	if( decontLedTX  >0 ) decontLedTX--;
	
	if( decontDelayRFPI >0 ) decontDelayRFPI--; //used by the function: delay_ms_RFPI(unsigned int time_ms_to_wait)
	
	if( decont_programming_mode_enabled > 0 ) decont_programming_mode_enabled--;
	
	if( decont_check_and_parse_functions_peri > 0 ) decont_check_and_parse_functions_peri--; //used for the function check_and_parse_functions_peri(void);
}

//delay in milliseconds
void delay_ms_RFPI(unsigned int time_ms_to_wait){	
	decontDelayRFPI = time_ms_to_wait;
	while(decontDelayRFPI); 
}



//it reads uart_receive_buffer[] buffer and if there is a command it store the command into cmd_rfpi[] and set cont_cmd_rfpi=20
//when cont_cmd_rfpi it is equal to 0 then will search again for a command into uart_receive_buffer[]
void get_cmd_from_uart_buffer(void){
	 unsigned char i;
	
		//check if a command has been received
		if(uart_receive_cnt >= 20){ //uart_receive_cnt is the counter of the bytes received from the UART
			i=0;
			while( ((i+5) < SIZE_BUFFER_UART) && !(/*uart_receive_buffer[i+4] == 'R' &&*/ uart_receive_buffer[i+5] == 'B') ){
				i++;	
			}
			cont_cmd_rfpi = 0;
			while( (cont_cmd_rfpi < 20) && (i < SIZE_BUFFER_UART) ){
				cmd_rfpi[cont_cmd_rfpi] = uart_receive_buffer[i];
				cont_cmd_rfpi++;
				i++;
				if( cont_cmd_rfpi < 4 ){
					sender_add[cont_cmd_rfpi] = cmd_rfpi[cont_cmd_rfpi]; //saving address sender	
				}
			}	
			decontTimeOutSerialData = 0;
		}
		
		//resetting the command if the timeout is expired
		if( decontTimeOutSerialData == 0 ){
			uart_receive_cnt = 0;	//resetting the counter used into the uart interrupt 
		}

}


//useful function to know if inside a string is contained a given string
unsigned char instring(unsigned char* k, unsigned char* s)
{
  unsigned char a,b,c,x,l1,l2,er;
  unsigned char* dato;
  unsigned char* stringa;
 
 er=0;
 l1=0;
 dato=k;
 while(*dato++)l1++;
 l2=0;
 stringa=s;
 while(*stringa++)l2++;
 
 stringa=s;
 c=1;
 for(a=0;a<(l2-l1+1);a++)
   {
	dato=k;
	x=0;
	for(b=0;b<l1;b++)
	  {
	   if(*stringa++==*dato++)x++;
	  }//end for
	stringa-=l1;
	stringa++;
	if(x==l1)
	  {
	   er=c;
	   break;
	  }
	else
	  {
	   c++;
	  }
   }//end for
 
 return er;
}



//check if the radio module is busy, if it is busy then stop untill its free
void RF_wait_BUSY(void){ 
// while(PIN_BUSY_RF);
 //delay_ms_RFPI(1); 
}


void UART_send_and_fill_data_to_RF(unsigned char *data, unsigned char num_data){
	unsigned char i;
	for(i=num_data;i<16;i++)	//filling the data not assigned
		*(data+i) = '.';
		
	//the last byte is used for the checksum error
	//data[15] = calc_checksum (data, 15); //it calc for the first 15 bytes
	data[0] = calc_checksum (&data[1], 15); //it calc for the first 15 bytes
	
	UART_send_CMD_to_RF(3, (unsigned char *)"C30"); //command to load the data (after this command has to follow the 16bytes)
	UART_send_STR_to_RF(16, (unsigned char *)data); //giving the 16bytes of data
	UART_send_CMD_to_RF(3, (unsigned char *)"C31"); //command to make the radio to send the data
}


void Led_TX_OFF_and_after_delay_turn_ON(void){
	decontLedTX = MAX_TIME_LED_TX_OFF;
	PIN_LED_TX_OFF; //led off
}


//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_RFPIMCU
void 	UART_DEBUG_RFPIMCU_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;

	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('R');USART_SendByte('F');USART_SendByte('P');USART_SendByte('I');USART_SendByte('M');USART_SendByte('C');USART_SendByte('U');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}

void 	UART_DEBUG_RFPIMCU_send_STR2(unsigned char *data, unsigned char line_feed){
	unsigned char i;
	unsigned char num_data=strlen(data);
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('R');USART_SendByte('F');USART_SendByte('P');USART_SendByte('I');USART_SendByte('M');USART_SendByte('C');USART_SendByte('U');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
#endif
