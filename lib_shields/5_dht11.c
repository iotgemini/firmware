/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Common functions to use umidity sensor DHT11
	
	
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


//***********************************************************************************
//	HERE START THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************

//Function that allocate the variables and init the shield
void 	init_DHT11_5(struct settings_shield *currentSettingsShield){
	//the manager init the pin as input thus I have just to add +1 to the variables var_NUM_ANALOGUE_INPUT
	var_NUM_ANALOGUE_INPUT++;


	root_pin_functions->SEM_DHT11 |= currentSettingsShield->PIN_USED;
	
	enablePullUp(currentSettingsShield->PIN_USED);
	
	if(pointer_struct_DHT11==0){
		pointer_struct_DHT11 = (struct DHT11_5*)malloc(sizeof(struct DHT11_5));
		pointer_struct_DHT11->sem_get_new_value_with_this_pin = currentSettingsShield->PIN_USED;
	}else{
		pointer_struct_DHT11->sem_get_new_value_with_this_pin |= currentSettingsShield->PIN_USED;
	}
	//pointer_struct_DHT11->sem_get_new_value_with_this_pin =0;
	
	/*for(int i=0;i<8;i++){
		pointer_struct_DHT11->I_umidity[i] = 0;
		//pointer_struct_DHT11->D_umidity[i] = 0;
		pointer_struct_DHT11->temperature[i] = 0; 
	}*/
	

	//pointer_struct_DHT11->value_DHT11 = ht11_read(unsigned char *PORTUSED, unsigned char pin);
	
	
	//pointer_struct_DHT11->decont_delay_ms_before_read_again = DHT11_DELAY_MS_BEFORE_READ_AGAIN; //determine the frequency of reading
	
	
	#ifdef UART_DEBUG_DHT11_5
	UART_DEBUG_DHT11_5_send_STR2((unsigned char *)"INIT DHT11",1); //it send a string through the UART only if is in debug mode
	//UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)root_pin_functions->SEM_ADC, 0);
	#endif
	
	
}

//void decont_vars_DHT11(void){ //this function has to be called every 1mS, so has to be written into the interrupt routine of 1mS
//	if(pointer_struct_DHT11->decont_delay_ms_before_read_again>0){ pointer_struct_DHT11->decont_delay_ms_before_read_again--; }
//}



 



void Request_DHT11(unsigned char pin)				// Microcontroller send start pulse/request 
{
	pinMode(pin, OUTPUT); //DDRD |= (1<<DHT11_PIN);
	digitalWrite(pin, LOW); //PORTD &= ~(1<<DHT11_PIN);	// set to low pin 
	delay_ms_at328(20);			// wait for 20ms
	digitalWrite(pin, HIGH); //PORTD |= (1<<DHT11_PIN);	// set to high pin 
}

unsigned char Response_DHT11(unsigned char pin)				// receive response from DHT11 
{
	unsigned int timeout = 100;
	enablePullUp(pin);
	pinMode(pin, INPUT); //DDRD &= ~(1<<DHT11_PIN);
	enablePullUp(pin);
	while(digitalRead(pin) == HIGH && timeout>0){timeout--; _delay_us(1);} //while(PIND & (1<<DHT11_PIN));
	if(timeout>0) timeout = 100; else return 1;
	while(digitalRead(pin) == LOW && timeout>0){timeout--; _delay_us(1);} //while((PIND & (1<<DHT11_PIN))==0);
	if(timeout>0) timeout = 100; else return 2;
	while(digitalRead(pin) == HIGH && timeout>0){timeout--; _delay_us(1);} //while(PIND & (1<<DHT11_PIN));
	if(timeout>0) return 0; else return 3;
}

unsigned int Receive_data_DHT11(unsigned char pin)			// receive data 
{	
	unsigned int c=0;
	unsigned int timeout = 100;
	int q;
	for (q=0; q<8 /*&& timeout>0*/; q++)
	{
		while(digitalRead(pin) == LOW && timeout>0){timeout--; _delay_us(1);} //while((PIND & (1<<DHT11_PIN)) == 0);  // check received bit 0 or 1 
		if(timeout>0) timeout = 70;
		_delay_us(30);
		if(digitalRead(pin) == HIGH)//if(PIND & (1<<DHT11_PIN))// if high pulse is greater than 30ms 
			c = (c<<1)|(0x01);	// then its logic HIGH 
		else			// otherwise its logic LOW 
			c = (c<<1);
		while(digitalRead(pin) == HIGH && timeout>0){timeout--; _delay_us(1);} //while(PIND & (1<<DHT11_PIN));
		if(timeout>0) timeout = 100;
	}
	//if(timeout>0)
		return c;
	//else
	//	return 0;
}



//int dht11_read(unsigned char *PORTUSED, unsigned char wheight_pin)
unsigned char dht11_read(unsigned char wheight_pin)
{
	unsigned char varReturn = 0;
	unsigned char error = 0;
	unsigned char index=0;
	unsigned char count_wheight_pin = 1;
	
	
	unsigned int I_RH,D_RH,I_Temp,D_Temp,CheckSum;
	
	
	
	//if(pointer_struct_DHT11->decont_delay_ms_before_read_again==0){
	//	pointer_struct_DHT11->decont_delay_ms_before_read_again = DHT11_DELAY_MS_BEFORE_READ_AGAIN; //determine the frequency of reading
		
		while(count_wheight_pin<wheight_pin){
			count_wheight_pin = count_wheight_pin * 2;
			index++;
		}

		
		I_RH=D_RH=I_Temp=D_Temp=CheckSum=0;
		
		//cli();//disable interrupt
			Request_DHT11(wheight_pin);		// send start pulse 
			//unsigned char var_Response_OK=Response_DHT11(wheight_pin);		// receive response 
			error = Response_DHT11(wheight_pin);		// receive response 
			if(error==0){
				I_RH=Receive_data_DHT11(wheight_pin);	// store first eight bit in I_RH 
				D_RH=Receive_data_DHT11(wheight_pin);	// store next eight bit in D_RH 
				I_Temp=Receive_data_DHT11(wheight_pin);	// store next eight bit in I_Temp 
				D_Temp=Receive_data_DHT11(wheight_pin);	// store next eight bit in D_Temp 
				CheckSum=Receive_data_DHT11(wheight_pin);// store next eight bit in CheckSum 
				//Led_TX_OFF_and_after_delay_turn_ON();
			}
		//sei();//enable interrupt
		//this make blink the led to notify the command has been received correctly
		Led_TX_OFF_and_after_delay_turn_ON();
		
			//pointer_struct_DHT11->checkSum[index] = CheckSum;
			
			if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum || error!=0)
			//if(var_Response_OK>0)
			{
				//lcd_gotoxy(0,0);
				//lcd_print("Error");
				
				//the last bit will identify the error
			//	pointer_struct_DHT11->I_umidity[index] |= 0x8000;
			//	pointer_struct_DHT11->temperature[index] |= 0x8000; 
				
				//this values means there is error in reading
			//	pointer_struct_DHT11->I_umidity[index] |= 0x4000;
			//	pointer_struct_DHT11->temperature[index] |= 0x4000; 
							
				pointer_struct_DHT11->I_umidity[index] =  0xFFFF; //it says there is an error
				pointer_struct_DHT11->temperature[index] =  0xFFFF; //it says there is an error
				
				varReturn = 1; //errore in lettura dati
			}else{	
				pointer_struct_DHT11->I_umidity[index] =  I_RH << 8; //integer part
				
				pointer_struct_DHT11->I_umidity[index] |=  (D_RH & 0x00FF); //decimal part
				
				//pointer_struct_DHT11->D_umidity[index] =  D_RH; //decimal part
				
				pointer_struct_DHT11->temperature[index] = I_Temp << 8; //integer part 
				pointer_struct_DHT11->temperature[index] |= (D_Temp & 0x00FF); //decimal part 
				
				//pointer_struct_DHT11->temperature[index] = I_Temp+20; //to delete
				
				
				
				//itoa(I_RH,data,10);
				//lcd_gotoxy(11,0);
				//lcd_print(data);
				//lcd_print(".");
				
				//itoa(D_RH,data,10);
				//lcd_print(data);
				//lcd_print("%");

				//itoa(I_Temp,data,10);
				//lcd_print(data);

				
				//itoa(D_Temp,data,10);
				//lcd_print(data);

				
				//itoa(CheckSum,data,10);
				//lcd_print(data);

			}
			
			
			
			
			
			// BUFFER TO RECEIVE
//			uint8_t bits[5];
//			uint8_t cnt = 7;
//			uint8_t idx = 0;
//			uint8_t varExit = 0;
//			int i;
//
//			// EMPTY BUFFER
//			for (i=0; i< 5; i++) bits[i] = 0;
//
//			// REQUEST SAMPLE
//			pinMode(wheight_pin, OUTPUT);
//			digitalWrite(wheight_pin, LOW);
//			delay_ms_at328(18);
//			digitalWrite(wheight_pin, HIGH);
//			delayMicroseconds(40);
//			enablePullUp(wheight_pin);
//			pinMode(wheight_pin, INPUT);
//			enablePullUp(wheight_pin);
//
//			// ACKNOWLEDGE or TIMEOUT
//			unsigned int loopCnt = 100;
//			unsigned int tt,t;
//			while(digitalRead(wheight_pin) == LOW && varExit==0){
//					if (loopCnt-- == 0) varExit=1;//return DHTLIB_ERROR_TIMEOUT;
//			}
//			loopCnt = 100;
//			while(digitalRead(wheight_pin) == HIGH && varExit==0){
//					if (loopCnt-- == 0) varExit=2;//return DHTLIB_ERROR_TIMEOUT;
//					delayMicroseconds(1);
//			}
//
//			// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
//			for (i=0; i<40 && varExit==0; i++)
//			{
//					loopCnt = 100;
//					t = 0;
//					while(digitalRead(wheight_pin) == LOW && varExit==0){
//						if (loopCnt-- == 0) varExit=3;//return DHTLIB_ERROR_TIMEOUT;
//						t++;
//						delayMicroseconds(1);
//					}
//
//					//unsigned long t = micros();
//
//					_delay_us(10);
//
//					loopCnt = 100;
//					tt = 0;
//					while(digitalRead(wheight_pin) == HIGH && varExit==0){
//						if (loopCnt-- == 0) varExit=4;//return DHTLIB_ERROR_TIMEOUT;
//						tt++;
//						delayMicroseconds(1);
//					}
//
//					//if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
//					if ((tt - t) > 40) bits[idx] |= (1 << cnt);
//					if (cnt == 0)   // next byte?
//					{
//							cnt = 7;    // restart at MSB
//							idx++;      // next byte!
//					}
//					else cnt--;
//			}
//
//			// WRITE TO RIGHT VARS
//			// as bits[1] and bits[3] are allways zero they are omitted in formulas.
//		   // humidity    = bits[0];
//		   // temperature = bits[2];
//
//
//			uint8_t sum = bits[0] + bits[2];
//
//			if (bits[4] != sum){// || varExit>0){
//				//return DHTLIB_ERROR_CHECKSUM;
//				pointer_struct_DHT11->I_umidity[index] = wheight_pin;//varExit;
//				pointer_struct_DHT11->D_umidity[index] = 0;
//				pointer_struct_DHT11->temperature[index] = varExit;
//			}else{
//				//return DHTLIB_OK;
//				pointer_struct_DHT11->I_umidity[index] = bits[0];
//				pointer_struct_DHT11->D_umidity[index] = bits[0];
//				pointer_struct_DHT11->temperature[index] = bits[2];
//			}
//
		   
		
	//}
	
	return varReturn;
		
}




//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_DHT11_5
void 	UART_DEBUG_DHT11_5_send_STR2(unsigned char *data, unsigned char line_feed){
	
	unsigned char i;
	unsigned char num_data=strlen(data);
	
	
	//while(PIN_BUSY_RF){} //wait until the last command is processed by the radio module
	
	//function_cmd_to_run();						//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('D');USART_SendByte('T');USART_SendByte('H');USART_SendByte('1');USART_SendByte('1');USART_SendByte('_');USART_SendByte('5');USART_SendByte(')');
	
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
	
	
	
}
#endif
