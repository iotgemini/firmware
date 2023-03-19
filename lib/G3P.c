/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Last Update:			01 / 03 / 2023
	
	Descrizione:			Libreria per retro compatibilità
	
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//It init all variables and send the setting to the NRF
void 	Init_G3P(void){
	
	unsigned char eeprom_data[NUM_BYTE_EEPROM_RF];
	int cont_temp;
	int i;
	
	wdt_reset(); //it reset the WDT
	
	CMD_to_Run = 100; //no command to run
	
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	
	//getting settings from the eeprom
	if(return_array_data_EEPROM((unsigned char*)&eeprom_data[0],NUM_BYTE_EEPROM_RF, START_ADDRESS_WHERE_TO_SAVE_RF) > 0){
		wdt_reset(); //it reset the WDT
		//it has to init the EEPROM
		for(i=0;i<NUM_BYTE_EEPROM_RF;i++){ 
			eeprom_data[i]=eeprom_def_tab_G3P[i]; 
			wdt_reset(); //it reset the WDT
		}
		save_array_data_EEPROM((unsigned char*)&eeprom_data[0], NUM_BYTE_EEPROM_RF, START_ADDRESS_WHERE_TO_SAVE_RF);
	}
	
	
	
	//loading settings from eeprom
	current_settings_G3P->id_baud =  eeprom_data[0]; //setting baud rate
	current_settings_G3P->len_array_data = eeprom_data[1]; //it is the number of byte for the payload
	current_settings_G3P->radio_channel = eeprom_data[2]; //the radio channel to use
	current_settings_G3P->power_tx =  (char) eeprom_data[3]; //the radio power to transmit data
	current_settings_G3P->akn_yes = eeprom_data[4]; //At '0' Acknowledge is disabled, at '1' Acknowledge is enabled. If enabled, it make to return (via USART) OK if transmission is successful, else it return ?A 
	current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk=eeprom_data[5]; //this is used to check if send back the asterisk for a CMD executed
	//Loading addresses from EEPROM:
	//This device address and Address of the device that to transmit
	wdt_reset(); //it reset the WDT
	for(cont_temp=0;cont_temp<5;cont_temp++){
		//wdt_reset(); //it reset the WDT
		current_settings_G3P->rx_address[cont_temp] = (char) eeprom_data[cont_temp+6];
		current_settings_G3P->tx_address[cont_temp] = (char) eeprom_data[cont_temp+11];
	}	
	current_settings_G3P->radio_channel_hopping = eeprom_data[16];
	current_settings_G3P->enabled_hopping = eeprom_data[17];
	current_settings_G3P->enabled_time_out_akn = eeprom_data[18];
	current_settings_G3P->sem_send_OK_on_TX_good = eeprom_data[19];
	
	current_settings_G3P->sem_TX_running=0;		//this indicate if the transmission of the data loaded into array_data_to_be_TX is happening

	
	wdt_reset(); //it reset the WDT
	

	//		LSB								MSB
	//	{	0xE7,	0xE1,	0xE7,	0xE1,	0xAA	}; this are default value for tx_address and rx_address
	//		|___________|   |___________|   |__|
	//		Address TX/RX    Network Addr   ID my Transceiver
	//coping in ascii the addresses
	
	byte_to_hexascii((unsigned char*)&current_settings_G3P->rx_address[2], (unsigned char*)&current_settings_G3P->address_network[0]);	//network address
	byte_to_hexascii((unsigned char*)&current_settings_G3P->rx_address[3], (unsigned char*)&current_settings_G3P->address_network[2]);	//network address
	//current_settings_G3P->tx_address[2] = current_settings_G3P->rx_address[2];
	//current_settings_G3P->tx_address[3] = current_settings_G3P->rx_address[3];
	
	
	byte_to_hexascii((unsigned char*)&current_settings_G3P->rx_address[0], (unsigned char*)&current_settings_G3P->address_this_device[0]);	//address of this device
	byte_to_hexascii((unsigned char*)&current_settings_G3P->rx_address[1], (unsigned char*)&current_settings_G3P->address_this_device[2]);	//address of this device
	
	byte_to_hexascii((unsigned char*)&current_settings_G3P->tx_address[0], (unsigned char*)&current_settings_G3P->address_to_tx[0]);	//address that to send data
	byte_to_hexascii((unsigned char*)&current_settings_G3P->tx_address[1], (unsigned char*)&current_settings_G3P->address_to_tx[2]);	//address that to send data
	
	wdt_reset(); //it reset the WDT
	
	
	//This function enable the reply of '*' for any command without reply via UART
	//CMD_C15(current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk, 4);
	
	
	#ifndef UART_DEBUG_G3P 
	if( sem_Used_As_Peripheral == 0){
		//Init the USART
		//CMD_C55(current_settings_G3P->id_baud, 4);
		CMD_to_Run=55;
		function_cmd_to_run();		//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt 
		cont_buffer_CMD_to_Run=0;
		CMD_to_Run=100; //at 100 no command has to be run
		//USART_Init_with_baudrate(9600);
	}
	#else
		UART_DEBUG_G3P_send_STR(68, (unsigned char *)"It does not init the UART for G3P because the DEBUG mode is enabled!",1); //it send a string through the UART only if is in debug mode
	#endif
	//wdt_reset(); //it reset the WDT
	
	#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(22, (unsigned char *)"Initializing SPI......",1); //it send a string through the UART only if is in debug mode
	#endif
	//Inizializza la porta SPI come MASTER
	SPI_MasterInit(); //inizializzare prima di nrf24_init();
	
	//wdt_reset(); //it reset the WDT
	
	PIN_CE_SET_ON; //it enable the NRF24L01
	
	
	#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(24, (unsigned char *)"Initializing NRF24......",1); //it send a string through the UART only if is in debug mode
	#endif
	
	// init hardware pins
    nrf24_init();
	
	//Channel , payload length
	nrf24_config(current_settings_G3P->radio_channel, current_settings_G3P->len_array_data+2); //questa funzione ha al suo interno delle configurazioni fisse, quindi � necessario eseguire i prossimi comandi per cambiarle
	
	if(current_settings_G3P->akn_yes == '1') nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
	else nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
	
	//Set the RF addresses 
	nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address);
	nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 

	// Optionally, go back to RX mode ... 
	nrf24_powerUpRx();
	
	#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(16, (unsigned char *)"END OF G3P INIT!",1); //it send a string through the UART only if is in debug mode
	#endif
	
}				


//Giving the byte received, it will manage the array of data received	
//and execute the commands
void 	execute_USART_CMD(char byte_received){
	unsigned char sem_cmd_not_recogised;	//this is a semaphore to understand	if the command is into the list of command, in case the command received will be discarded

	PIN_BUSY_SET_ON; //tell to the external MCU "I am doing something, thus just wait!"

	cont_time_out_cmd = MAX_TIMEOUT_CMD;
	str_cmd[len_str_cmd] = byte_received;
	
	if(len_str_cmd<(MAX_LEN_STR_CMD-1)){
		len_str_cmd++;
		
		if(str_cmd[0]=='C'){ 
				
			if(len_str_cmd>2){	//command has been received, now check if the CMD 
								//has parameters otherwise it goes to run the CMD
				
				sem_cmd_not_recogised = 1; //for now the command is not recognised
				
				if(str_cmd[1]=='0'){
					if(str_cmd[2]=='0'){	//CMD = C00
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C00((char*)&str_cmd[0], len_str_cmd); //This function set the RF operating channel
					}else if(str_cmd[2]=='1'){	//CMD = C01
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C01((char*)&str_cmd[0], len_str_cmd); //Set the network address	
					}else if(str_cmd[2]=='2'){	//CMD = C02
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C02((char*)&str_cmd[0], len_str_cmd); //Set the address of this device		
					}else if(str_cmd[2]=='3'){	//CMD = C03
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C03((char*)&str_cmd[0], len_str_cmd); //Set the address to transmit
					}else if(str_cmd[2]=='4'){	//CMD = C04
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C04((char*)&str_cmd[0], len_str_cmd); //Set the number of bytes for each transmission
					}else if(str_cmd[2]=='5'){	//CMD = C05
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C05((char*)&str_cmd[0], len_str_cmd); //This function set the RF channel for frequency hopping
					}else if(str_cmd[2]=='7'){	//CMD = C07
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C07((char)str_cmd[3], len_str_cmd); //This function set TX power
					}else if(str_cmd[2]=='8'){	//CMD = C08
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C08(); //This function init the Radio
					}else if(str_cmd[2]=='9'){	//CMD = C09
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C09((char)str_cmd[3], len_str_cmd); //This function Enable/Disable the auto acknowledge
					}
				}else if(str_cmd[1]=='1'){
					if(str_cmd[2]=='0'){	//CMD = C10
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C10((char)str_cmd[3], len_str_cmd); //This function Enable/Disable the frequency hopping
					}else if(str_cmd[2]=='1'){	//CMD = C11
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C11((char)str_cmd[3], len_str_cmd); //Default='0' (off)	//This function Enable/Disable the time out for the acknowledge
					}else if(str_cmd[2]=='5'){	//CMD = C15
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C15(str_cmd[3],len_str_cmd); //This function enable the reply of '*' for any command without reply via UART
					}
				}else if(str_cmd[1]=='2'){
					if(str_cmd[2]=='0'){	//CMD = C20
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C20((char*)&str_cmd[0], len_str_cmd); //This function set the maximum waiting time to find the channel free. After this time it start the auto retry cycle.
					}else if(str_cmd[2]=='1'){	//CMD = C21
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C21((char*)&str_cmd[0], len_str_cmd); //This function set maximum waiting time for AKN
					}else if(str_cmd[2]=='2'){	//CMD = C22
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C22((char*)&str_cmd[0], len_str_cmd); //This function set the waiting time between two transmission when an error occur
					}else if(str_cmd[2]=='3'){	//CMD = C23
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C23((char*)&str_cmd[0], len_str_cmd); //This function set the max number of retry in case of unsuccessful transmission
					}else if(str_cmd[2]=='9'){	//CMD = C29
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C29((char)str_cmd[3], len_str_cmd); //This function set the RF module to answer OK when there is a successful transmission  
					}
				}else if(str_cmd[1]=='3'){
					if(str_cmd[2]=='0'){	//CMD = C30
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C30((char*)&str_cmd[0], len_str_cmd); //This function get the data from the UART to be sent
					}else if(str_cmd[2]=='1'){	//CMD = C31
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C31(); //Make the data loaded with C30 to be sent via Radio
					}
				}else if(str_cmd[1]=='4'){		
					if(str_cmd[2]=='5'){	//CMD = C45
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C45();			//save the current settings into the EEPROM
					}else	if(str_cmd[2]=='6'){	//CMD = C46
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C46();			//Restore and apply the default settings
					}
				}else if(str_cmd[1]=='5'){					
					if(str_cmd[2]=='4'){			//CMD = C54		
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C54(); //This function send via USART the identification of the transceiver
						
					}else if(str_cmd[2]=='5'){		//CMD = C55			
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C55(str_cmd[3],len_str_cmd); //This function set the speed of the UART
					}
				}else if(str_cmd[1]=='8'){					
					if(str_cmd[2]=='5'){			//CMD = C85		
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C85(); //This function send via USART the *
					}else if(str_cmd[2]=='9'){			//CMD = C89		
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C89(); //This function send via USART the TX address
					}
						
				}else if(str_cmd[1]=='9'){					
					if(str_cmd[2]=='9'){			//CMD = C99		
						sem_cmd_not_recogised = 0; //the command is recognised
						CMD_C99(); //This function send the current settings
					}
				}
				
				if(sem_cmd_not_recogised == 1){ //the command is not in list thus it is reset
					Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
					PIN_BUSY_SET_OFF;
				}//else{
					//UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
					//UART_DEBUG_G3P_send_STR(7, (unsigned char *)"CMD_C00",1); //it send a string through the UART only if is in debug mode
				//}
			}
			
		}else{ //if the first letter is not a command then discard the command
			Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
			PIN_BUSY_SET_OFF;
		}
	}else{
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		PIN_BUSY_SET_OFF;
	}
}		


//controller of the command received
//this has to be into the interrupt routine of 1mS
void 	Controller_USART_CMD_G3P(void){
	if(cont_time_out_cmd>1){
		cont_time_out_cmd--;
	}else if(cont_time_out_cmd==1){
		cont_time_out_cmd--;
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	}			
	
}


//NN	//This function set the RF operating channel //expected parameter: NN  (2 char. Example can be "02" that select channel 2)
void 	CMD_C00(char *str_cmd, int len_string_cmd){
	int channel_to_set = 1000;

	if(len_string_cmd>4){ //if the length of the command is right then it execute the command
		//PIN_BUSY_SET_ON
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		
		
		if(str_cmd[3] == '0'){
			channel_to_set = 0;
		}else if(str_cmd[3] == '1'){
			channel_to_set = 10;
		}else if(str_cmd[3] == '2'){
			channel_to_set = 20;
		}else if(str_cmd[3] == '3'){
			channel_to_set = 30;
		}else if(str_cmd[3] == '4'){
			channel_to_set = 40;
		}else if(str_cmd[3] == '5'){
			channel_to_set = 50;
		}else if(str_cmd[3] == '6'){
			channel_to_set = 60;
		}
		
		if(channel_to_set!=1000){
			if(str_cmd[4] == '0'){
				channel_to_set += 0;
			}else if(str_cmd[4] == '1'){
				channel_to_set += 1;
			}else if(str_cmd[4] == '2'){
				channel_to_set += 2;
			}else if(str_cmd[4] == '3'){
				channel_to_set += 3;
			}else if(str_cmd[4] == '4'){
				channel_to_set += 4;
			}else if(str_cmd[4] == '5'){
				channel_to_set += 5;
			}else if(str_cmd[4] == '6'){
				channel_to_set += 6;
			}else if(str_cmd[4] == '7'){
				channel_to_set += 7;
			}else if(str_cmd[4] == '8'){
				channel_to_set += 8;
			}else if(str_cmd[4] == '9'){
				channel_to_set += 9;
			}else{
				channel_to_set = 1000;
			}
		}
				
		if(channel_to_set!=1000){ //if the channel is ok then it set
			current_settings_G3P->radio_channel = (unsigned char) channel_to_set;
			
			//CMD_to_Run=0; //nrf24_configRegister(RF_CH,(uint8_t)current_settings_G3P->radio_channel);
			manager_CMD_to_Run(0);	//this function manage the sequence of command to run
			
			//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
			
			//Channel: channel_to_set , payload length: len_array_data
			//nrf24_config(current_settings_G3P->radio_channel, current_settings_G3P->len_array_data+2);
	
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		}else{
			PIN_BUSY_SET_OFF;
		}

	}
}		


//HHLL	//Default="E7E1" //Set the network address	
void 	CMD_C01(char * str_cmd, int len_string_cmd){
	//		LSB								MSB
	//	{	0xE7,	0xE1,	0xE7,	0xE1,	0xAA	}; this are default value for tx_address and rx_address
	//		|___________|   |___________|   |__|
	//		Address TX/RX    Network Addr   ID my Transceiver
	
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		//PIN_BUSY_SET_ON
		
		#ifdef UART_DEBUG_G3P 
			UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		
		
		for(int i=0;i<4;i++){
			current_settings_G3P->address_network[i] = str_cmd[3+i];
		}
		current_settings_G3P->tx_address[2] = hexascii_to_byte((unsigned char*)&str_cmd[3]);
		current_settings_G3P->tx_address[3] = hexascii_to_byte((unsigned char*)&str_cmd[5]);
		current_settings_G3P->rx_address[2] = current_settings_G3P->tx_address[2];
		current_settings_G3P->rx_address[3] = current_settings_G3P->tx_address[3];
		
		
		//Set the RF addresses 
		//CMD_to_Run=1;
		manager_CMD_to_Run(1);	//this function manage the sequence of command to run
		//nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address);
		//nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 

		//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
		
		//Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		
	}
}


//HHLL	//Default="E7E1" //Set the address of this device
void 	CMD_C02(char * str_cmd, int len_string_cmd){
	//		LSB								MSB
	//	{	0xE7,	0xE1,	0xE7,	0xE1,	0xAA	}; this are default value for tx_address and rx_address
	//		|___________|   |___________|   |__|
	//		Address TX/RX    Network Addr   ID my Transceiver
	
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		for(int i=0;i<4;i++){
			current_settings_G3P->address_this_device[i] = str_cmd[3+i];
		}
		current_settings_G3P->rx_address[0] = hexascii_to_byte((unsigned char*)&str_cmd[3]);
		current_settings_G3P->rx_address[1] = hexascii_to_byte((unsigned char*)&str_cmd[5]);
		
		//Set the RF addresses 
		//CMD_to_Run=2;
		manager_CMD_to_Run(2);	//this function manage the sequence of command to run
		//nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 
		//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
		
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
	}
}


//HHLL	//Default="E7E1" //Set the address to transmit
void 	CMD_C03(char * str_cmd, int len_string_cmd){
	//		LSB								MSB
	//	{	0xE7,	0xE1,	0xE7,	0xE1,	0xAA	}; this are default value for tx_address and rx_address
	//		|___________|   |___________|   |__|
	//		Address TX/RX    Network Addr   ID my Transceiver
	
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		for(int i=0;i<4;i++){
			current_settings_G3P->address_to_tx[i] = str_cmd[3+i];
		}
		current_settings_G3P->tx_address[0] = hexascii_to_byte((unsigned char*)&str_cmd[3]);
		current_settings_G3P->tx_address[1] = hexascii_to_byte((unsigned char*)&str_cmd[5]);
		
		//Set the RF addresses 
		//CMD_to_Run=3;
		manager_CMD_to_Run(3);	//this function manage the sequence of command to run
		
		//nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address);
		//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
		
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
	}
}


//NN 	//Set the number of bytes for each transmission
void 	CMD_C04(char *str_cmd, int len_string_cmd){
	int num_bytes_to_set = 1000;

	if(len_string_cmd>4){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		if(str_cmd[3] == '0'){
			num_bytes_to_set = 0;
		}else if(str_cmd[3] == '1'){
			num_bytes_to_set = 10;
		}else if(str_cmd[3] == '2'){
			num_bytes_to_set = 20;
		}else if(str_cmd[3] == '3'){
			num_bytes_to_set = 30;
		}else if(str_cmd[3] == '4'){
			num_bytes_to_set = 40;
		}else if(str_cmd[3] == '5'){
			num_bytes_to_set = 50;
		}else if(str_cmd[3] == '6'){
			num_bytes_to_set = 60;
		}else if(str_cmd[3] == '7'){
			num_bytes_to_set = 70;
		}else if(str_cmd[3] == '8'){
			num_bytes_to_set = 80;
		}else if(str_cmd[3] == '9'){
			num_bytes_to_set = 90;
		}
		
		if(num_bytes_to_set!=1000){
			if(str_cmd[4] == '0'){
				num_bytes_to_set += 0;
			}else if(str_cmd[4] == '1'){
				num_bytes_to_set += 1;
			}else if(str_cmd[4] == '2'){
				num_bytes_to_set += 2;
			}else if(str_cmd[4] == '3'){
				num_bytes_to_set += 3;
			}else if(str_cmd[4] == '4'){
				num_bytes_to_set += 4;
			}else if(str_cmd[4] == '5'){
				num_bytes_to_set += 5;
			}else if(str_cmd[4] == '6'){
				num_bytes_to_set += 6;
			}else if(str_cmd[4] == '7'){
				num_bytes_to_set += 7;
			}else if(str_cmd[4] == '8'){
				num_bytes_to_set += 8;
			}else if(str_cmd[4] == '9'){
				num_bytes_to_set += 9;
			}else{
				num_bytes_to_set = 1000;
			}
		}
				
		if(num_bytes_to_set!=1000 && num_bytes_to_set<27){ //if the channel is ok then it set
			current_settings_G3P->len_array_data = (unsigned char) num_bytes_to_set;
			
		//	CMD_to_Run=4;
			manager_CMD_to_Run(4);	//this function manage the sequence of command to run
			
			//nrf24_configRegister(RX_PW_P1, (uint8_t)current_settings_G3P->len_array_data+2); // Data payload pipe
			//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
		
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		}else{
			PIN_BUSY_SET_OFF;	
		}
		
		
		
	}
}		


//NN	//This function set the RF channel for frequency hopping
void 	CMD_C05(char *str_cmd, int len_string_cmd){
	int channel_to_set = 1000;

	if(len_string_cmd>4){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		if(str_cmd[3] == '0'){
			channel_to_set = 0;
		}else if(str_cmd[3] == '1'){
			channel_to_set = 10;
		}else if(str_cmd[3] == '2'){
			channel_to_set = 20;
		}else if(str_cmd[3] == '3'){
			channel_to_set = 30;
		}else if(str_cmd[3] == '4'){
			channel_to_set = 40;
		}else if(str_cmd[3] == '5'){
			channel_to_set = 50;
		}else if(str_cmd[3] == '6'){
			channel_to_set = 60;
		}
		
		if(channel_to_set!=1000){
			if(str_cmd[4] == '0'){
				channel_to_set += 0;
			}else if(str_cmd[4] == '1'){
				channel_to_set += 1;
			}else if(str_cmd[4] == '2'){
				channel_to_set += 2;
			}else if(str_cmd[4] == '3'){
				channel_to_set += 3;
			}else if(str_cmd[4] == '4'){
				channel_to_set += 4;
			}else if(str_cmd[4] == '5'){
				channel_to_set += 5;
			}else if(str_cmd[4] == '6'){
				channel_to_set += 6;
			}else if(str_cmd[4] == '7'){
				channel_to_set += 7;
			}else if(str_cmd[4] == '8'){
				channel_to_set += 8;
			}else if(str_cmd[4] == '9'){
				channel_to_set += 9;
			}else{
				channel_to_set = 1000;
			}
		}
				
		if(channel_to_set!=1000){ //if the channel is ok then it set
			current_settings_G3P->radio_channel_hopping = (unsigned char) channel_to_set;
			//current_settings_G3P->enabled_hopping = 1;
			
			//nrf24_configRegister(RF_CH,(uint8_t)current_settings_G3P->radio_channel);
			
			//Channel: channel_to_set , payload length: len_array_data
			//nrf24_config(current_settings_G3P->radio_channel, current_settings_G3P->len_array_data+2);
			
			//CMD_to_Run=5;//
			manager_CMD_to_Run(5);	//this function manage the sequence of command to run
	
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		}else{
			PIN_BUSY_SET_OFF;	
		}
		
		
		
	}
}	


//N		//Default='3' (off)	//This function set TX power
void 	CMD_C07(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		//PIN_BUSY_SET_ON
		//da implementare
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//CMD_to_Run=7;//
		if(TYPE_L != BAD_TRANSCEIVER){
			manager_CMD_to_Run(7);	//this function manage the sequence of command to run
		}
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
	}
}


//This function init the Radio
void 	CMD_C08(void){
	#ifdef UART_DEBUG_G3P
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	//PIN_BUSY_SET_ON
	
	//CMD_C45();			//save the current settings into the EEPROM
	//Init_G3P();
	
	//Channel , payload length
	//CMD_to_Run=8;
	manager_CMD_to_Run(8);	//this function manage the sequence of command to run
	
	
	/*nrf24_config(current_settings_G3P->radio_channel, current_settings_G3P->len_array_data+2); //questa funzione ha al suo internet delle configurazioni fisse, quindi � necessario eseguire i prossimi comandi per cambiarle
	
	
	
	if(current_settings_G3P->akn_yes == '1') nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
	else nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
	
	//Set the RF addresses 
	nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address);
	nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 
		
		
	// Optionally, go back to RX mode ... 
	nrf24_powerUpRx();
	*/
	
	
	//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
}


//N		//Default='0' (off)	//This function Enable/Disable the auto acknowledge
void 	CMD_C09(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		if(parameter == '1'){
			current_settings_G3P->akn_yes = 49; //'1'
			//nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
		}else{
			current_settings_G3P->akn_yes = 48; //'0'
			//nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
			
		} 
		//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 
		
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		
		//CMD_to_Run=9;
		manager_CMD_to_Run(9);	//this function manage the sequence of command to run
	}
}


//N		//Default='0' (off)	//This function Enable/Disable the frequency hopping
void 	CMD_C10(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		
		//PIN_BUSY_SET_ON
		
		if(parameter == '1'){
			current_settings_G3P->enabled_hopping = 1;
			//nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
		}else{
			
			current_settings_G3P->enabled_hopping = 0;
			//current_settings_G3P->radio_channel_hopping = 0;
			//nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
		} 
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		//CMD_to_Run=10;
		manager_CMD_to_Run(10);	//this function manage the sequence of command to run
	}
}	


//N		//Default='0' (off)	//This function Enable/Disable the time out for the acknowledge
void 	CMD_C11(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		
		if(parameter == '0'){
			current_settings_G3P->enabled_time_out_akn = 0;
			//nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
		}else{
			current_settings_G3P->enabled_time_out_akn = 1;
			//nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
		} 
		
		//CMD_to_Run=11;
		manager_CMD_to_Run(11);	//this function manage the sequence of command to run
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
	}
}	


//This function enable the reply of '*' for any command without reply via UART
void 	CMD_C15(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		if(parameter == '0'){
			current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk = 0; //'0'
		}else{
			current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk = 1; //'1'
			
			manager_CMD_to_Run(15);	//this function manage the sequence of command to run
		}
		
		//PIN_BUSY_SET_OFF;	//tell to the external MCU "I am doing something, thus just wait!"
	}
}

//this function is used into the other CMD to check if send back the asterisk
void	check_if_send_asterisk(void){
	if(current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk == 1){ //'1'
		if( sem_Used_As_Peripheral == 1){
			#ifdef UART_DEBUG_G3P
				//UART_DEBUG_G3P_send_STR(1, (unsigned char*)42,1); //it send a string through the UART only if is in debug mode
			#endif
			interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
		}else{
			USART_SendByte((unsigned char)42); //42 is the *
		}
	}
		
	//Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
}	


//NNNN	//Default=20mS	//This function set the maximum waiting time to find the channel free. After this time it start the auto retry cycle.		
void 	CMD_C20(char *str_cmd, int len_string_cmd){
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		//PIN_BUSY_SET_ON
		//da implementare
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		//CMD_to_Run=20;//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		manager_CMD_to_Run(20);	//this function manage the sequence of command to run
	}
}


//NNNN	//Default=600mS	//This function set maximum waiting time for AKN
void 	CMD_C21(char *str_cmd, int len_string_cmd){
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		//PIN_BUSY_SET_ON
		//da implementare
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		//CMD_to_Run=21;//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		manager_CMD_to_Run(21);	//this function manage the sequence of command to run
	}
}


//NNNN	//Default=500mS	//This function set the waiting time between two transmission when an error occur
void 	CMD_C22(char *str_cmd, int len_string_cmd){	
	if(len_string_cmd>6){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		 #endif
		//PIN_BUSY_SET_ON
		//da implementare
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		//CMD_to_Run=22;//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		manager_CMD_to_Run(22);	//this function manage the sequence of command to run
	}
}


//NN	//Default=05	//This function set the max number of retry in case of unsuccessful transmission
void 	CMD_C23(char *str_cmd, int len_string_cmd){	
	if(len_string_cmd>4){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		 #endif
		//PIN_BUSY_SET_ON
		//da implementare
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		//CMD_to_Run=23;//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		manager_CMD_to_Run(23);	//this function manage the sequence of command to run
	}
}


//N		//Default='0' (off)	//This function set the RF module to answer OK when there is a successful transmission  
void 	CMD_C29(char parameter, int len_string_cmd){	
	if(len_string_cmd>3){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		//PIN_BUSY_SET_ON
		if(parameter == '0'){
			current_settings_G3P->sem_send_OK_on_TX_good = 0;
		}else{
			current_settings_G3P->sem_send_OK_on_TX_good = 1;
		} 
		
		check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		PIN_BUSY_SET_OFF;	//tell to the external MCU "I am doing something, thus just wait!"
	}
}


//Nx depend by C04		//This function get the data from the UART to be sent
void 	CMD_C30(char *str_cmd, int len_string_cmd){
	if(len_string_cmd>(current_settings_G3P->len_array_data+3-1)){ //if the length of the command is right then it execute the command
		#ifdef UART_DEBUG_G3P
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
		
		//PIN_BUSY_SET_ON
		
		for(int cont=0;cont<current_settings_G3P->len_array_data;cont++){
			current_settings_G3P->array_data_to_be_TX[cont]=str_cmd[cont+3];
		}
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		//PIN_BUSY_SET_OFF;	
		
		manager_CMD_to_Run(30);	//this function manage the sequence of command to run
		
	}
}


//		//Make the data loaded with C30 to be sent via Radio
void 	CMD_C31(void){
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); 			//It reset all variable that are used to check and execute the command received via USART
	current_settings_G3P->sem_TX_running = 1;
	manager_CMD_to_Run(31);	//this function manage the sequence of command to run
}			


//Store the current settings into the EEPROM
void CMD_C45(void){
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(45);	//this function manage the sequence of command to run
}


//Restore and apply the default settings
void 	CMD_C46(void){
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(46);	//this function manage the sequence of command to run
}							


//This function send via USART the identification of the transceiver
void 	CMD_C54(void){
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(54);	//this function manage the sequence of command to run
}							


//Init the USART with a baud rate
void 	CMD_C55(char parameter, int len_string_cmd){
	if(len_string_cmd>3){ //if the lenght of the command is righrt then it execute the command
		#ifdef UART_DEBUG_G3P 
		UART_DEBUG_G3P_send_STR(len_string_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
		#endif
		Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
		
		if(parameter == 51){ //'3' //Init the USART with a baud rate of 9600bps
			//PIN_BUSY_SET_ON;	//tell to the external MCU "I am doing something, thus just wait!"
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			current_settings_G3P->id_baud = 51;
			//CMD_to_Run=55;//USART_Init_with_baudrate((unsigned int)9600);
			manager_CMD_to_Run(55);	//this function manage the sequence of command to run
		}else if(parameter == 52){ //'4' //Init the USART with a baud rate of 19200bps
			//PIN_BUSY_SET_ON;	//tell to the external MCU "I am doing something, thus just wait!"
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			current_settings_G3P->id_baud = 52;
			//CMD_to_Run=55;//USART_Init_with_baudrate((unsigned int)19200);
			manager_CMD_to_Run(55);	//this function manage the sequence of command to run
		}else if(parameter == 53){ //'5' //Init the USART with a baud rate of 38400bps
			//PIN_BUSY_SET_ON;	//tell to the external MCU "I am doing something, thus just wait!"
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			current_settings_G3P->id_baud = 53;
			//CMD_to_Run=55;//USART_Init_with_baudrate((unsigned int)38400);
			manager_CMD_to_Run(55);	//this function manage the sequence of command to run
		}else if(parameter == 54){ //'6' //Init the USART with a baud rate of 57600bps
			//PIN_BUSY_SET_ON;	//tell to the external MCU "I am doing something, thus just wait!"
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			current_settings_G3P->id_baud = 54;
			//CMD_to_Run=55;//USART_Init_with_baudrate((unsigned int)57600);
			manager_CMD_to_Run(55);	//this function manage the sequence of command to run
		}else if(parameter == 55){ //'7' //Init the USART with a baud rate of 115200bps
			//PIN_BUSY_SET_ON;	//tell to the external MCU "I am doing something, thus just wait!"
			//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			current_settings_G3P->id_baud = 55;
			//CMD_to_Run=55;//USART_Init_with_baudrate((unsigned int)115200);
			manager_CMD_to_Run(55);	//this function manage the sequence of command to run
		}
		
	}
}

//This function send via USART the *
void	CMD_C85(void){ 
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(85);	//this function manage the sequence of command to run
}

//This function send via USART the TX Address
void	CMD_C89(void){ 
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(89);	//this function manage the sequence of command to run
}


//This function send back the current settings
void	CMD_C99(void){
	#ifdef UART_DEBUG_G3P 
	UART_DEBUG_G3P_send_STR(len_str_cmd, (unsigned char*)&str_cmd[0],1); //it send a string through the UART only if is in debug mode
	#endif
	Reset_USART_CMD(); //It reset all variable that are used to check and execute the command received via USART
	manager_CMD_to_Run(99);	//this function manage the sequence of command to run
}


//It reset all variable that are used to check and execute the command received via USART
void 	Reset_USART_CMD(void){
	len_str_cmd = 0;		//the command string is empty
	cont_time_out_cmd = 0;	//counter just start when it receive a byte						
}


//this manage the data comming from the NRF24 via SPI
void	manager_RX_NRF24(void){
	unsigned char address_trasmitter[4];
		
	if(nrf24_dataReady()){
		PIN_BUSY_SET_ON;
		#ifdef UART_DEBUG_G3P 
			//USART_SendByte((unsigned char)byte);
			UART_DEBUG_G3P_send_STR(17, (unsigned char *)"DATA FROM RADIO: ",1); //it send a string through the UART only if is in debug mode
		#endif
		
		decont_delay_carrier_led = MAX_DELAY_CARRIER_LED;	//the carrier led is ON until the interrupt of the timer0 bring this variable to 0
		if(sem_Used_As_Peripheral==1){
			var_carrier_led=1;
		}else{
			PIN_CD_SET_ON;
		}
		
		
        nrf24_getData((uint8_t*)data_array_from_nrf24);      
		//PIN_LEDOK_SET_ON;
		
        
		//get the address of who transmitted the message		
		byte_to_hexascii((unsigned char*)&data_array_from_nrf24[0], (unsigned char*)&address_trasmitter[0]);	//address of this device
		byte_to_hexascii((unsigned char*)&data_array_from_nrf24[1], (unsigned char*)&address_trasmitter[2]);	//address of this device
	
		//transmitting via UART the address of the transmitter
		for(int i=0;i<4;i++){	//giving the data via USART
			if( sem_Used_As_Peripheral == 1){
				interrupt_uart0_data_from_RF((unsigned char) address_trasmitter[i]);
				#ifdef UART_DEBUG_G3P 
					USART_SendByte((unsigned char) address_trasmitter[i]);
				#endif
			}else{
				USART_SendByte((unsigned char) address_trasmitter[i]);
			}
		}
		
		for(int i=2;i<current_settings_G3P->len_array_data+2;i++){	//giving the data via USART
			if( sem_Used_As_Peripheral == 1){
				interrupt_uart0_data_from_RF((unsigned char) data_array_from_nrf24[i]);
				#ifdef UART_DEBUG_G3P 
					USART_SendByte((unsigned char) data_array_from_nrf24[i]);
				#endif
			}else{
				USART_SendByte((unsigned char) data_array_from_nrf24[i]);
			}
		}
		
		//nrf24_powerUpRx(); // Optionally, go back to RX mode ... 

		PIN_BUSY_SET_OFF;
    }
		
	//if(decont_delay_carrier_led==0)
	//	PIN_LEDOK_SET_OFF;	

		/*if(data_array_from_nrf24[0]==0x0F && decont_delay_carrier_led==0){
			decont_delay_carrier_led = 100;
			//bad: if(STATUS_CD>0){
				if(sem_Used_As_Peripheral==1){
					var_carrier_led=0;
				}else{
					PIN_CD_SET_OFF;
				}
			}else{
				if(sem_Used_As_Peripheral==1){
					var_carrier_led=1;
				}else{
					PIN_CD_SET_ON;
				}
			}
		}*/
		
		/*if(data_array_from_nrf24[1]==0xAA){
			data_array_from_nrf24[1]=0x00;
			PIN_CD_SET_OFF;
		}else if(data_array_from_nrf24[1]==0xBB){
			data_array_from_nrf24[1]=0x00;
			if(sem_Used_As_Peripheral==1){
				var_carrier_led=1;
			}else{
				PIN_CD_SET_ON;
			}
		}*/
}					

//this will end the activity started into the manager_RX_NRF24(), 
//this function run into the interrupt of the timer0
void	controller_activity_manager_RX_NRF24(void){
	if(decont_delay_carrier_led>0) decont_delay_carrier_led--;
	else{
		if(sem_Used_As_Peripheral==1){
			var_carrier_led=0;
		}else{
			PIN_CD_SET_OFF;
		}
	} 
}




//this manage the data to TX to the NRF24 via SPI
void	manager_TX_NRF24(void){
	unsigned char status;
	unsigned char data_to_TX_via_RF[30];
	
	
	//if(current_settings_G3P->sem_TX_running == 1){ //data is loaded and C31 has been received thus has to TX the data
	
		current_settings_G3P->sem_TX_running = 1;
		
		#ifdef UART_DEBUG_G3P 
			//USART_SendByte((unsigned char)byte);
			UART_DEBUG_G3P_send_STR(24, (unsigned char *)"SENDING DATA VIA RADIO! ",1); //it send a string through the UART only if is in debug mode
		#endif
		
		
		PIN_BUSY_SET_ON;
		
		//cli(); //Disabilita gli interrupts globali
		decont_delay_carrier_led = MAX_DELAY_CARRIER_LED;	//the carrier led is ON until the interrupt of the timer0 bring this variable to 0
		if(sem_Used_As_Peripheral==1){
			var_carrier_led=1;
		}else{
			PIN_CD_SET_ON;
		}
		
		
		//USART_SendByte((unsigned char) 'q'); USART_SendByte((unsigned char) 'p'); 
		
		data_to_TX_via_RF[0]=current_settings_G3P->rx_address[0];
		data_to_TX_via_RF[1]=current_settings_G3P->rx_address[1];
		
		for(int cont=0;cont<current_settings_G3P->len_array_data;cont++){
			data_to_TX_via_RF[cont+2]=current_settings_G3P->array_data_to_be_TX[cont];
		}
		
		
		// Automatically goes to TX mode 
		//nrf24_send(current_settings_G3P->array_data_to_be_TX);  
		nrf24_send(data_to_TX_via_RF);  
			
			
		// Wait for transmission to end 
		_delay_us(1000);
		while(nrf24_isSending()){_delay_us(100);}
		_delay_us(100);
		
		
		if(current_settings_G3P->sem_send_OK_on_TX_good == 1){
			//has to send OK if transmission is OK otherwise send
			status=0xFF;
			//while(status==0xFF){ //if status is always 0xFF something does not work properly
			status=nrf24_lastMessageStatus();
			//}
			if(status==NRF24_TRANSMISSON_OK){
				if( sem_Used_As_Peripheral == 1){
					interrupt_uart0_data_from_RF((unsigned char) 'O');
					interrupt_uart0_data_from_RF((unsigned char) 'K');
				}else{
					USART_SendByte((unsigned char) 'O'); USART_SendByte((unsigned char) 'K'); //OK
				}
			}else if(status==NRF24_MESSAGE_LOST){
				if( sem_Used_As_Peripheral == 1){
					interrupt_uart0_data_from_RF((unsigned char) 'X');
					interrupt_uart0_data_from_RF((unsigned char) 'A');
				}else{
					USART_SendByte((unsigned char) 'X'); USART_SendByte((unsigned char) 'A');
				}
			}else{
				if( sem_Used_As_Peripheral == 1){
					interrupt_uart0_data_from_RF((unsigned char) 'F');
					interrupt_uart0_data_from_RF((unsigned char) 'F');
				}else{
					USART_SendByte((unsigned char) 'F'); USART_SendByte((unsigned char) 'F');
				}
			}
		}
		

		
		
		//sei(); //Abilita gli interrupts globali
		
		//nrf24_powerDown();
		//_delay_us(1);
		// Optionally, go back to RX mode ... 
		nrf24_powerUpRx();
		
		//check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
		
		current_settings_G3P->sem_TX_running = 0;
		//PIN_BUSY_SET_OFF;
		
	//}
}


//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt 
void	function_cmd_to_run(void){
		
		if(CMD_to_Run<100){
			
			/*#ifdef UART_DEBUG_G3P 
				char dec_value[10];
				UART_DEBUG_G3P_send_STR(20, (unsigned char *)"COMMAND TO EXECUTE: ",1); //it send a string through the UART only if is in debug mode
				itoa((unsigned int)CMD_to_Run,dec_value,10);
				for( int i=0;i<3 && dec_value[i]!='\0';i++){
					USART_SendByte((unsigned char)dec_value[i]);
				}
			#endif*/
			
			if(CMD_to_Run==0){ 		
				/*#ifdef UART_DEBUG_G3P 
					char dec_value[10];
					UART_DEBUG_G3P_send_STR(20, (unsigned char *)"The channel set is: ",1); //it send a string through the UART only if is in debug mode
					itoa(current_settings_G3P->radio_channel,dec_value,10);
					for( int i=0;i<3 && dec_value[i]!='\0';i++){
						USART_SendByte((unsigned char)dec_value[i]);
					}
				#endif*/
				nrf24_configRegister(RF_CH,(uint8_t)current_settings_G3P->radio_channel); 
				nrf24_powerUpRx(); 
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();	
			}else if(CMD_to_Run==1){	
				nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address); 
				nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 
				nrf24_powerUpRx(); 	
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==2){	
				nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 
				nrf24_powerUpRx(); 	
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==3){	
				nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address); 
				nrf24_powerUpRx(); 
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==4){	
				nrf24_configRegister(RX_PW_P1, (uint8_t)current_settings_G3P->len_array_data);  
				nrf24_powerUpRx(); 
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();	
			}else if(CMD_to_Run==5){	 
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==7){	 
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==8){	
				if(current_settings_G3P->akn_yes == '1') nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
				else nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
				
				//Set the RF addresses 
				nrf24_tx_address((uint8_t*)current_settings_G3P->tx_address);
				nrf24_rx_address((uint8_t*)current_settings_G3P->rx_address); 
				
				nrf24_config(current_settings_G3P->radio_channel, current_settings_G3P->len_array_data+2); //questa funzione ha al suo internet delle configurazioni fisse, quindi � necessario eseguire i prossimi comandi per cambiarle
				
					
				// Optionally, go back to RX mode ... 
				nrf24_powerUpRx();
								
				CMD_to_Run=100; //at 100 no command has to be run
				
				check_if_send_asterisk();
				
			}else if(CMD_to_Run==9){
				if(current_settings_G3P->akn_yes == 48){
					nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe disabled
				}else{
					nrf24_configRegister(RX_PW_P0, 0x01); // Auto-ACK enabled
				} 
				nrf24_powerUpRx();
				
				CMD_to_Run=100; //at 100 no command has to be run
				
				check_if_send_asterisk();	

			}else if(CMD_to_Run==10){	
				if(current_settings_G3P->enabled_hopping == 0){
					//da implementare
				}else{
					//da implementare
				} 
				nrf24_powerUpRx();
								
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
				
			}else if(CMD_to_Run==11){		
				if(current_settings_G3P->enabled_time_out_akn == 0){
					//da implementare
				}else{
					//da implementare
				} 
				nrf24_powerUpRx();
								
				CMD_to_Run=100; //at 100 no command has to be run
				
				check_if_send_asterisk();
			}else if(CMD_to_Run==15){		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==20){		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==21){		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==22){		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==23){		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
				
			}else if(CMD_to_Run==30){ //data to send via RF has been loaded		
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
			}else if(CMD_to_Run==31){ //send data via RF
				manager_TX_NRF24();					//this manage the data to TX to the NRF24 via SPI
				
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();
				
			}else if(CMD_to_Run==45){		
				unsigned char eeprom_data[NUM_BYTE_EEPROM_RF];

				eeprom_data[0] = (unsigned char)current_settings_G3P->id_baud; // baud rate
				eeprom_data[1] = (unsigned char)current_settings_G3P->len_array_data; //it is the number of byte for the payload
				eeprom_data[2] = (unsigned char)current_settings_G3P->radio_channel; //the radio channel to use
				eeprom_data[3] = (unsigned char)current_settings_G3P->power_tx; //the radio power to transmit data
				eeprom_data[4] = (unsigned char)current_settings_G3P->akn_yes; //
				eeprom_data[5] = (unsigned char)current_settings_G3P->Sem_Send_Back_Via_UART_the_Asterisk; //At '0' Acknowledge is disabled, at '1' Acknowledge is enabled. If enabled, it make to return (via USART) OK if transmission is successful, else it return ?A 
				for(int cont_temp=0;cont_temp<5;cont_temp++){
					wdt_reset(); //it reset the WDT
					eeprom_data[cont_temp+6] = (unsigned char)current_settings_G3P->rx_address[cont_temp];
					eeprom_data[cont_temp+11] = (unsigned char)current_settings_G3P->tx_address[cont_temp];
				}
				eeprom_data[16] = (unsigned char)current_settings_G3P->radio_channel_hopping;
				eeprom_data[17] = (unsigned char)current_settings_G3P->enabled_hopping;
				eeprom_data[18] = (unsigned char)current_settings_G3P->enabled_time_out_akn;
				eeprom_data[19] = (unsigned char)current_settings_G3P->sem_send_OK_on_TX_good;

				save_array_data_EEPROM((unsigned char*)&eeprom_data[0], NUM_BYTE_EEPROM_RF, START_ADDRESS_WHERE_TO_SAVE_RF);
				
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
			
			}else if(CMD_to_Run==46){
				unsigned char eeprom_data[NUM_BYTE_EEPROM_RF];
	
				//it has to write the default value into the EEPROM
				for(int i=0;i<NUM_BYTE_EEPROM_RF;i++){ 
					eeprom_data[i]=eeprom_def_tab_G3P[i]; 
				}
				save_array_data_EEPROM((unsigned char*)&eeprom_data[0], NUM_BYTE_EEPROM_RF, START_ADDRESS_WHERE_TO_SAVE_RF);
							
				//It init all variables and send the setting to the RF
				Init_G3P();

				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
				
			}else if(CMD_to_Run==54){	
				char str_temp[] = IDENTIFICATION_STRING;
				if(TYPE_L == BAD_TRANSCEIVER){ //send a message
					strcpy(str_temp,STRING_TRANSCEIVER_BAD);
				}
				int len_str = strlen(str_temp);
				for(int i=0; i<len_str; i++){
					if( sem_Used_As_Peripheral == 1){
						interrupt_uart0_data_from_RF(str_temp[i]);
					}else{
						USART_SendByte(str_temp[i]);
					}
				}
				
				CMD_to_Run=100; //at 100 no command has to be run
				
			}else if(CMD_to_Run==55){	
			
				if(current_settings_G3P->id_baud == 51){ //'3' //Init the USART with a baud rate of 9600bps
					check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
					_delay_us(1000);
					if( sem_Used_As_Peripheral == 0) //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						USART_Init_with_baudrate((unsigned int)9600);
					//CMD_to_Run=45; //save into eeprom all current settings
				}else if(current_settings_G3P->id_baud == 52){ //'4' //Init the USART with a baud rate of 19200bps
					check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
					_delay_us(1000);
					if( sem_Used_As_Peripheral == 0) //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						USART_Init_with_baudrate((unsigned int)19200);
					//CMD_to_Run=45; //save into eeprom all current settings
				}else if(current_settings_G3P->id_baud == 53){ //'5' //Init the USART with a baud rate of 38400bps
					check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
					_delay_us(1000);
					if( sem_Used_As_Peripheral == 0) //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						USART_Init_with_baudrate((unsigned int)38400);
					//CMD_to_Run=45; //save into eeprom all current settings
				}else if(current_settings_G3P->id_baud == 54){ //'6' //Init the USART with a baud rate of 57600bps
					check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
					_delay_us(1000);
					if( sem_Used_As_Peripheral == 0) //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						USART_Init_with_baudrate((unsigned int)57600);
					//CMD_to_Run=45; //save into eeprom all current settings
				}else if(current_settings_G3P->id_baud == 55){ //'7' //Init the USART with a baud rate of 115200bps
					check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set
					//USART_SendByte((unsigned char) 79);USART_SendByte((unsigned char) 75); //OK
					_delay_us(1000);
					//cli(); //Disabilita gli interrupts globali
					if( sem_Used_As_Peripheral == 0) //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						USART_Init_with_baudrate((unsigned int)11520);
					//sei(); //Abilita gli interrupts globali
					
					//CMD_to_Run=45; //save into eeprom all current settings
				}//else
					CMD_to_Run=100; //at 100 no command has to be run	

			}else if(CMD_to_Run==85){
				if( sem_Used_As_Peripheral == 1) {
					interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
				}else{
					USART_SendByte((unsigned char)42); //42 is the *
				}
				CMD_to_Run=100; //at 100 no command has to be run	
				
			}else if(CMD_to_Run==89){	
			
				//USART_SendByte((unsigned char)66); //B
				
				//nrf24_writeRegister(TX_ADDR,adr,nrf24_ADDR_LEN);
				uint8_t value_register;//[nrf24_ADDR_LEN]; 
				//nrf24_readRegister(RF_CH, &value_register, 1);// nrf24_ADDR_LEN);
				//nrf24_readRegister(FIFO_STATUS,&value_register,1);
				nrf24_readRegister(RF_CH,&value_register,1);
				value_register=(value_register & (1 << RX_EMPTY));
				
				/*char str_temp[] = "Register=";
				int len_str = strlen(str_temp);
				for(int i=0; i<len_str; i++){
					if( sem_Used_As_Peripheral == 1){
						interrupt_uart0_data_from_RF(str_temp[i]);
					}else{
						USART_SendByte(str_temp[i]);
					}
				}*/
				
				//USART_SendByte((unsigned char)67); //C
				
				//value_register = var_ID_TYPE_L;
				
				//value_register = current_settings_G3P->radio_channel;
				
				unsigned char str_value_register[2];
				
				byte_to_hexascii((unsigned char*)&value_register, (unsigned char*)&str_value_register[0]);	//
				
				//USART_SendByte((unsigned char)value_register);
				
				for(int i=0; i<2; i++){
					if( sem_Used_As_Peripheral == 1){
						interrupt_uart0_data_from_RF(str_value_register[i]);
					}else{
						USART_SendByte(str_value_register[i]);
					//	USART_SendByte(value_register);
						//USART_SendByte(" ");
					}
				}
				
				
				value_register = current_settings_G3P->radio_channel;

				
				byte_to_hexascii((unsigned char*)&value_register, (unsigned char*)&str_value_register[0]);	//
				
				//USART_SendByte((unsigned char)value_register);
				
				for(int i=0; i<2; i++){
					if( sem_Used_As_Peripheral == 1){
						interrupt_uart0_data_from_RF(str_value_register[i]);
					}else{
						USART_SendByte(str_value_register[i]);
					//	USART_SendByte(value_register);
						//USART_SendByte(" ");
					}
				}
				
				var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
				while(1);
			
			
				CMD_to_Run=100; //at 100 no command has to be run	
				
			}else if(CMD_to_Run==99){
				/*Addr NET E7E1
				Addr DEV E7E1
				Ch 0100
				Power 3
				#Bytes 24
				AKN 1
				*
				*/
				
				char str_addr[]="Addr ";
				char str_net[]="NET ";
				char str_dev[]="DEV ";
				char str_ch[]="Ch ";
				char str_power[]="Power ";
				char str_bytes[]="#Bytes ";
				char str_akn[]="AKN ";
				char str_temp[5];
				int i;
					
					
				if( sem_Used_As_Peripheral == 0){ //interrupt_uart0_data_from_RF((unsigned char) 42); //42 is the *
						
					
					
					for(i=0; i< strlen(str_addr);i++) USART_SendByte((unsigned char)str_addr[i]); //Addr
					for(i=0; i< strlen(str_net);i++) USART_SendByte((unsigned char)str_net[i]); //NET
					for(i=0; i< 4;i++) USART_SendByte((unsigned char)current_settings_G3P->address_network[i]); //network address
					
					USART_SendByte((unsigned char)13); //Carriage Return
					USART_SendByte((unsigned char)10); //Line feed (new line)
					
					for(i=0; i< strlen(str_addr);i++) USART_SendByte((unsigned char)str_addr[i]); //Addr
					for(i=0; i< strlen(str_dev);i++) USART_SendByte((unsigned char)str_dev[i]); //DEV
					for(i=0; i< 4;i++) USART_SendByte((unsigned char)current_settings_G3P->address_this_device[i]); //device address
					
					USART_SendByte((unsigned char)13); //Carriage Return
					USART_SendByte((unsigned char)10); //Line feed (new line)
					
					for(i=0; i< strlen(str_ch);i++) USART_SendByte((unsigned char)str_ch[i]); //Ch
					//byte_to_hexascii((unsigned char *)&current_settings_G3P->radio_channel, (unsigned char *)str_temp);	
					if(current_settings_G3P->radio_channel < 10){
						str_temp[0]=48; //'0'
						itoa((int)current_settings_G3P->radio_channel, &str_temp[1],10);
					}else itoa((int)current_settings_G3P->radio_channel, str_temp,10);
					for(i=0; i< 2;i++) USART_SendByte((unsigned char)str_temp[i]); //ch
					//byte_to_hexascii((unsigned char *)&current_settings_G3P->radio_channel_hopping, (unsigned char *)str_temp);	
					if(current_settings_G3P->radio_channel_hopping < 10){
						str_temp[0]=48; //'0'
						itoa((int)current_settings_G3P->radio_channel_hopping, &str_temp[1],10);
					}else itoa((int)current_settings_G3P->radio_channel_hopping, str_temp,10);
					for(i=0; i< 2;i++) USART_SendByte((unsigned char)str_temp[i]); //ch
					
					USART_SendByte((unsigned char)13); //Carriage Return
					
					for(i=0; i< strlen(str_power);i++) USART_SendByte((unsigned char)str_power[i]); //Power
					USART_SendByte((unsigned char)current_settings_G3P->power_tx); //
					
					USART_SendByte((unsigned char)13); //Carriage Return
					USART_SendByte((unsigned char)10); //Line feed (new line)
					
					for(i=0; i< strlen(str_bytes);i++) USART_SendByte((unsigned char)str_bytes[i]); //Bytes
					//intToStr((int)current_settings_G3P->len_array_data, str_bytes); //convert a number in string
					if(current_settings_G3P->len_array_data < 10){
						str_temp[0]=48; //'0'
						itoa((int)current_settings_G3P->len_array_data, &str_temp[1],10);
					}else itoa((int)current_settings_G3P->len_array_data, str_temp,10);
					for(i=0; i< 2;i++) USART_SendByte((unsigned char)str_temp[i]); //len_array_data
					
					USART_SendByte((unsigned char)13); //Carriage Return
					USART_SendByte((unsigned char)10); //Line feed (new line)
					
					for(i=0; i< strlen(str_akn);i++) USART_SendByte((unsigned char)str_akn[i]); //AKN
					USART_SendByte((char)current_settings_G3P->akn_yes); //akn

					USART_SendByte((unsigned char)13); //Carriage Return
					USART_SendByte((unsigned char)10); //Line feed (new line)
					
				}
				
				CMD_to_Run=100; //at 100 no command has to be run
				check_if_send_asterisk();	//this function send back the asterisk if the semaphore is set

			}else{ //no command found to run
				CMD_to_Run=100; //at 100 no command has to be run	
			}

			//if(CMD_to_Run>=100){
			//	PIN_BUSY_SET_OFF;
			//}
			
			
			//CMD_to_Run=100; //at 100 no command has to be run
		}else{
			//cli(); //Disabilita gli interrupts globali
			//manager_CMD_to_Run(100);	//this function manage the sequence of command to run
			//sei(); //Abilita gli interrupts globali
			
			if(len_str_cmd == 0 && cont_buffer_CMD_to_Run>0){		//the command string is empty
				cli(); //Disabilita gli interrupts globali
				if(manager_CMD_to_Run(100)==0){	//this function manage the sequence of command to run
					//non ci sono pi� comandi da eseguire quindi spegne il pin di busy
				}
				sei(); //Abilita gli interrupts globali
			}else if(CMD_to_Run==100 && cont_buffer_CMD_to_Run==0){
				PIN_BUSY_SET_OFF;
			}
				
			
			manager_RX_NRF24();	//this manage the data coming from the NRF24 via SPI
			
			Controller_USART_CMD_G3P();	//controller of the command received
			
			controller_activity_manager_RX_NRF24();		//this will end the activity started into the manager_RX_NRF24()
			
			
		}
		
}


//this function manage the sequence of command to run
unsigned char manager_CMD_to_Run(char New_CMD_to_Run){
	int i;
	//CMD_to_Run = New_CMD_to_Run;
	if(cont_buffer_CMD_to_Run>0){
		//USART_SendByte('a');
			
			//now check if is ready to run a new command
			if(CMD_to_Run>=100){ 

				PIN_BUSY_SET_ON; //tell to the external MCU "I am doing something, thus just wait!"
				
				//it can run a new command and take off this command from the buffer
				CMD_to_Run = buffer_CMD_to_Run[0];
				
				//now shift by one the buffer
				for(i=0; i<(cont_buffer_CMD_to_Run-1); i++){
					 buffer_CMD_to_Run[i] =  buffer_CMD_to_Run[i+1];
				}
				cont_buffer_CMD_to_Run--;
				buffer_CMD_to_Run[cont_buffer_CMD_to_Run] = 100; //last byte become empty
				
				//USART_SendByte('b');
			}
	}

	if(New_CMD_to_Run<100){ //it just check if is a new command to save
		if(CMD_to_Run==100 && cont_buffer_CMD_to_Run==0){
			PIN_BUSY_SET_ON; //tell to the external MCU "I am doing something, thus just wait!"
			
			//it can run a new command immediately
			CMD_to_Run = New_CMD_to_Run;
			
			//USART_SendByte('c');
		}else if(cont_buffer_CMD_to_Run<MAX_BUFFER_CMD_TO_RUN){
			//if there is space into the buffer it save the CMD into the buffer
			buffer_CMD_to_Run[cont_buffer_CMD_to_Run] = New_CMD_to_Run;
			cont_buffer_CMD_to_Run++;
			
			//USART_SendByte('d');
		}
	}
	
	return cont_buffer_CMD_to_Run;
}

	
//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_G3P
void 	UART_DEBUG_G3P_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('G');USART_SendByte('3');USART_SendByte('P');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
#endif
