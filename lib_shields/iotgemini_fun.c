/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			IOTGEMINI platform functions for ATmega328
	
	
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


//Function that return number of the pin (from pin3 to pin10) from the number of the ID
unsigned char num_pin_iotgemini_from_id_input(unsigned char id_io){
	//the next cycle determines which input has the id contained into id_io
	unsigned char num_pin_to_send_status=2;
	unsigned char cont_pin=0;
	unsigned char num_analogue_pin=0;
	unsigned char test_bit_x =1;
	unsigned char i;
	for(i=0; i<8;i++){
		if( ((root_pin_functions->PIN_MASK & test_bit_x) == 0) && ((root_pin_functions->PIN_USED & test_bit_x) != 0) ){ //check if it is an input and if it is used

			if(	((root_pin_functions->SEM_ADC & test_bit_x) != 0)
				|| ((root_pin_functions->SEM_DHT11 & test_bit_x) != 0)
			  ){ //check if it is an analogue input
				num_analogue_pin++;
			}else{
				cont_pin++;
				if( cont_pin > id_io ){ //exit
					num_pin_to_send_status=i+3;
					i=8;
				}
			}
		}
		
		test_bit_x = test_bit_x * 2;
	}

	if( ((root_pin_functions->SEM_DHT11 & 0b00000001) != 0) && (num_pin_to_send_status == 2) ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=3;
		}
	}
	
	if( ((root_pin_functions->SEM_DHT11 & 0b00000010) != 0) && (num_pin_to_send_status == 2) ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=4;
		}
	}
	
	if(  ( ((root_pin_functions->SEM_ADC & 0b00000100) != 0) || ((root_pin_functions->SEM_DHT11 & 0b00000100) != 0) )
		 && (num_pin_to_send_status==2)
	  ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=5;
		}
	}

	if( ((root_pin_functions->SEM_DHT11 & 0b00001000) != 0) && (num_pin_to_send_status == 2) ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=6;
		}
	}

	if( ( ((root_pin_functions->SEM_ADC & 0b00010000) != 0) || ((root_pin_functions->SEM_DHT11 & 0b00010000) != 0))
		&& (num_pin_to_send_status==2)
	  ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=7;
		}
	}

	if( ((root_pin_functions->SEM_DHT11 & 0b00100000) != 0) && (num_pin_to_send_status == 2) ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=8;
		}
	}

	if( ((root_pin_functions->SEM_DHT11 & 0b01000000) != 0) && (num_pin_to_send_status == 2) ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=9;
		}
	}

	if( ( ((root_pin_functions->SEM_ADC & 0b10000000) != 0) || ((root_pin_functions->SEM_DHT11 & 0b10000000) != 0) )
		&& (num_pin_to_send_status==2)
	  ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=10;
		}
	}

	if( num_pin_to_send_status == 2 ){ //check if it is an analogue input
		cont_pin++;
		if( cont_pin > id_io ){//cmd_rfpi[7] ){
			num_pin_to_send_status=11; //it send the MCU Volt (ADC1)
		}
	}
	
	return num_pin_to_send_status;
}



//Function that return number of the pin (from pin3 to pin10) from the number of the ID
unsigned char num_pin_iotgemini_from_id_output(unsigned char id_io){
	//the next cycle determines which outpu has the id contained into id_io
	unsigned char num_pin_to_send_status=2;
	unsigned char cont_pin=0;
	unsigned char test_bit_x =1;
	unsigned char num_analogue_pin=0;
	unsigned char i;
	for(i=0; i < 8 ;i++){
		if((root_pin_functions->PIN_MASK & test_bit_x) != 0){ //check if it is an output
			if((root_pin_functions->SEM_PWM & test_bit_x) != 0){ //check if it is an analogue output
				num_analogue_pin++;
			}else{
				cont_pin++;
				if(cont_pin>id_io){ //exit
					num_pin_to_send_status=i+3;
					i=8;
				}
			}
		
		}
		test_bit_x = test_bit_x * 2;
	}
	
	if(num_pin_to_send_status==2){ //that means no digital pin has been found, thus it is analogue
		for(i=0; i<num_analogue_pin;i++){
			cont_pin++;
			if(cont_pin>id_io){ //exit
				num_pin_to_send_status=(i*3)+3;
				i=num_analogue_pin;
			}
		}
	}
	
	return num_pin_to_send_status;
}



//this get the value and into data[4] there is a value on 8bit, into the data[5] there is the num of bit used, 
//if the num bit used is over 8bit the data it is not kept into data[4], but into the consecutive data[6],data[7].....data[x]
unsigned char return_value_pin(unsigned char num_pin_to_return_status, unsigned char *data){
	unsigned char varExit = 0;
	unsigned char varReturn = 0; //if stay at 0 then no error occured
	
	data[5] = 1; //it means there is 1 bit
	
	unsigned char wheight_num_pin = 0b00000001;
	wheight_num_pin = wheight_num_pin << (num_pin_to_return_status - 3);
	if(root_pin_functions->SEM_DHT11 != 0 && wheight_num_pin == (root_pin_functions->SEM_DHT11 & wheight_num_pin) ){
		
		data[4] = (unsigned char)pointer_struct_DHT11->I_umidity[(num_pin_to_return_status - 3)];
		
		data[5] = 32; //it means there are 32bits for the float value
		//data[5] = 32+8; //it means there are 32bits + 8bit for check sum
		
		data[6] = (unsigned char)(pointer_struct_DHT11->I_umidity[(num_pin_to_return_status - 3)] >> 8); 
		data[7] = (unsigned char)(pointer_struct_DHT11->I_umidity[(num_pin_to_return_status - 3)] ); 
		//data[8] = (unsigned char)(pointer_struct_DHT11->D_umidity[(num_pin_to_return_status - 3)] >> 8); 
		//data[9] = (unsigned char)pointer_struct_DHT11->D_umidity[(num_pin_to_return_status - 3)]; 
		
		data[8] = (unsigned char)(pointer_struct_DHT11->temperature[(num_pin_to_return_status - 3)] >> 8); 
		data[9] = (unsigned char)pointer_struct_DHT11->temperature[(num_pin_to_return_status - 3)]; 
		
		//if(data[6]==0xFF && data[7]==0xFF && data[8]==0xFF && data[9]==0xFF) varReturn = 1; //impossibile to read dht11
		
		//data[10] = (unsigned char)pointer_struct_DHT11->checkSum[(num_pin_to_return_status - 3)]; 
		
		pointer_struct_DHT11->sem_get_new_value_with_this_pin |= wheight_num_pin;
		
		//data[15] = 5; //DHT11 //this says to the gateway the ID of the shield is connected
		
	//ID=0 //PD3 = PIN3
	}else if(num_pin_to_return_status == 3){ 
		
		if((root_pin_functions->SEM_PWM & 0b00000001) != 0){ //check if it is used as PWM
			data[4] = get_duty_cycle_RED_LED();
			data[5] = 8; //it means that is an analogue output
			//data[15] = 4; //RGB //this says to the gateway the ID of the shield is connected
		//}else if(root_pin_functions->SEM_DHT11 != 0){
		//	data[4] = pointer_struct_DHT11->value_DHT11;
		//	pointer_struct_DHT11->sem_get_new_value = 1;
			
		}else{
			data[4] = bolean_pin_is_set(PIND,3); 
			/*if((root_pin_functions->PIN_MASK & 0b00000001) != 0){ //check if it is an output
				//it means that is an output
				data[15] = 1; //LED - RELAY //this says to the gateway the ID of the shield is connected
			}else{
				//it means this is an input 
				data[15] = 2; //SW //this says to the gateway the ID of the shield is connected
			}*/
			
		}
	}else 
	
	//ID=1 //PD0 = PIN4
	if(num_pin_to_return_status == 4){
		
		data[4] = bolean_pin_is_set(PIND,0); 
		/*if((root_pin_functions->PIN_MASK & 0b00000010) != 0) //check if it is an output
			data[5] = 0; //it means that is an output
		else 
			data[5] = 1; //it means there are 1 bit
		*/
	}else
	
	//ID=2 //PC0 = PIN5  
	if(num_pin_to_return_status == 5){
		if((root_pin_functions->PIN_MASK & 0b00000100) != 0){ //check if it is an output
			data[4] = bolean_pin_is_set(PINC,0);
		}else{
			
			if((root_pin_functions->SEM_ADC & 0b00000100) != 0){ //it is analogue input
				data[4] = (unsigned char)(ADC0_Average / 4);
				data[5] = 10; //it means there are 10 bit
				data[6] = (unsigned char)(ADC0_Average >> 8); 
				data[7] = (unsigned char)ADC0_Average; 
			}else{ //it is digital input
				data[4] = bolean_pin_is_set(PINC,0); //PC0 = PIN5
				//data[5] = 1; //it means there are 1 bit
			}
		}
		
	}else
		
	//ID=3 //PD6 = PIN6
	if(num_pin_to_return_status == 6){
		if((root_pin_functions->SEM_PWM & 0b00001000) != 0){ //check if it is used as PWM
			data[4] = get_duty_cycle_BLUE_LED();
			data[5] = 8; //it means that is an nalogue output
		}else{
			data[4] = bolean_pin_is_set(PIND,6); 
		}
	}else
		
	//ID=4 //PC4 = PIN7
	if(num_pin_to_return_status == 7){
		if((root_pin_functions->PIN_MASK & 0b00010000) != 0){ //check if it is an output
			data[4] = bolean_pin_is_set(PINC,4);
			//data[5] = 0; //it means that is an output
		}else{
			if((root_pin_functions->SEM_ADC & 0b00010000) != 0){ //it is analogue input
				//ADC4_Average =	get_10bit_adc(NUM_ADC4);//temp to delete
			
				data[4] = (unsigned char)(ADC4_Average / 4);
				data[5] = 10; //it means there are 10 bit
				data[6] = (unsigned char)(ADC4_Average >> 8); 
				data[7] = (unsigned char)ADC4_Average; 
				
				#ifdef UART_DEBUG_RFPIMCU
				UART_DEBUG_RFPIMCU_send_STR2((unsigned char *)"ADC4=",1); //it send a string through the UART only if is in debug mode
				UART_DEBUG_send_NUM_BYTE_HEX(data[4], 0);
				#endif
			}else{ //it is digital input
				data[4] = bolean_pin_is_set(PINC,4); 
				//data[5] = 1; //it means there are 1 bit
			}
		}
	}else
	
	//ID=5 //PD1 = PIN8
	if(num_pin_to_return_status == 8){
		data[4] = bolean_pin_is_set(PIND,1); 
		/*if((root_pin_functions->PIN_MASK & 0b00100000) != 0) //check if it is an output
			data[5] = 0; //it means that is an output
		else 
			data[5] = 1; //it means there are 1 bit
		*/
	}else
		
	//ID=6 //PD5 = PIN9
	if(num_pin_to_return_status == 9){
		if((root_pin_functions->SEM_PWM & 0b01000000) != 0){ //check if it is used as PWM
			data[4] = get_duty_cycle_GREEN_LED();
			data[5] = 8; //it means that is an nalogue output
		}else{
			data[4] = bolean_pin_is_set(PIND,5); 
		}
	}else

	//ID=7 //PC5 = PIN10
	if(num_pin_to_return_status == 10){
		if((root_pin_functions->PIN_MASK & 0b10000000) != 0){ //check if it is an output
			data[4] = bolean_pin_is_set(PINC,5);
			//data[5] = 0; //it means that is an output
		}else{
			if((root_pin_functions->SEM_ADC & 0b10000000) != 0){ //it is analogue input
				data[4] = (unsigned char)(ADC5_Average / 4);
				data[5] = 10; //it means there are 10 bit
				data[6] = (unsigned char)(ADC5_Average >> 8); 
				data[7] = (unsigned char)ADC5_Average; 
			}else{ //it is digital input
				data[4] = bolean_pin_is_set(PINC,5); //PC5 = PIN10
				//data[5] = 1; //it means there are 1 bit
			}
		}
	}
	
	//ID=8 //Virtual = PIN11
	if(num_pin_to_return_status == 11){
				data[4] = (unsigned char)(ADC1_Average / 4);
				data[5] = 10; //it means there are 10 bit
				data[6] = (unsigned char)(ADC1_Average >> 8); 
				data[7] = (unsigned char)ADC1_Average; 
				
				data[13] = 255; //this says to the gateway the ID of the shield is connected
	}else{
		//this cycle is used to find which shield is connected to the pin 
		current_settings_shield = root_struct_shields->root_settings_shield;
		while(current_settings_shield != 0 && varExit==0){
			if( (current_settings_shield->PIN_USED & wheight_num_pin) == wheight_num_pin ){//this pin is used: PIN3 = PD3
				
				data[13] = current_settings_shield->ID; //this says to the gateway the ID of the shield is connected
				varExit=1;
			}
			current_settings_shield = current_settings_shield->next;
		}
	}
	data[14] = num_pin_to_return_status; //this says to the gateway the number of the pin
	
	//the last byte is used for the checksum error
	//data[15] = calc_checksum (data, 15); //it calc for the first 15 bytes
	
	return varReturn;
}


//translate the weight of the pin IOTGEMINI to the address of the register of the real pin on the AT328
void return_AT328_io_register_from_IOTG_pin( unsigned char wheight_pin, volatile uint8_t *port_register, volatile uint8_t *direction_register, volatile uint8_t *portin_register, volatile uint8_t *weight_pin_AT328 ){
	
	if( (wheight_pin & 0b00000001) != 0 ){//PIN3
		port_register = &PORTD; 
		direction_register = &DDRD; 
		portin_register = &PIND;
		*weight_pin_AT328 = ( 1 << 3 );
	}else if( (wheight_pin & 0b00000010) != 0 ){//PIN4
		port_register = &PORTD; 
		direction_register = &DDRD; 
		portin_register = &PIND;
		*weight_pin_AT328 = ( 1 << 0 ); 
	}else if( (wheight_pin & 0b00000100) != 0 ){//PIN5
		port_register = &PORTC; 
		direction_register = &DDRC; 
		portin_register = &PINC;
		*weight_pin_AT328 = ( 1 << 0 ); 
	}else if( (wheight_pin & 0b00001000) != 0 ){//PIN6
		port_register = &PORTD; 
		direction_register = &DDRD; 
		portin_register = &PIND;
		*weight_pin_AT328 = ( 1 << 6 );
	}else if( (wheight_pin & 0b00010000) != 0 ){//PIN7
		port_register = &PORTC; 
		direction_register = &DDRC; 
		portin_register = &PINC;
		*weight_pin_AT328 = ( 1 << 4 ); 
	}else if( (wheight_pin & 0b00100000) != 0 ){//PIN8
		port_register = &PORTD; 
		direction_register = &DDRD; 
		portin_register = &PIND;
		*weight_pin_AT328 = ( 1 << 1 ); 
	}else if( (wheight_pin & 0b01000000) != 0 ){//PIN9
		port_register = &PORTD; 
		direction_register = &DDRD; 
		portin_register = &PIND;
		*weight_pin_AT328 = ( 1 << 5 ); 
	}else if( (wheight_pin & 0b10000000) != 0 ){//PIN10
		port_register = &PORTC; 
		direction_register = &DDRC; 
		portin_register = &PINC;
		*weight_pin_AT328 = ( 1 << 5 ); 
	}

}
