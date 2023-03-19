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

// NOTE:
// I comandi per frequency hopping sono creati ma non abilitati a livello di NRF24.
// Comando C11 abilita time out akn � ailitato ma non implementato a livello di logica
// comandi da implementare la logica ma abilitati come comandi seriali: C20,C21,C22,C23, C07
/*	SerialCmdRFPi(handleUART, "C200100", answerRFPI, CMD_WAIT1); //set maximum waiting time 10mS
	SerialCmdRFPi(handleUART, "C210001", answerRFPI, CMD_WAIT1); //set maximum waiting time for AKN 100mS
	SerialCmdRFPi(handleUART, "C220050", answerRFPI, CMD_WAIT1); //set to 5mS the waiting time between two transmission when an error occur
	SerialCmdRFPi(handleUART, "C2302", answerRFPI, CMD_WAIT1); //max number of retry in case of unsuccessful transmission
	SerialCmdRFPi(handleUART, "C072", answerRFPI, CMD_WAIT1); //set TX power
	*/


#ifdef UART_DEBUG
	//#define UART_DEBUG_G3P //activate the debug onthe UART for this library
#endif	
	
//#define IDENTIFICATION_STRING					"G3P-RPPM-2400 R303" //"G3P-2.4GHz"
#define IDENTIFICATION_STRING					"IOT-RPPM-2400 R303" //"G3P-2.4GHz"
#ifndef ENABLE_SPECIAL_FUNCTIONS
#define STRING_TRANSCEIVER_BAD					"BAD-TRANSCEIVER"
#endif

#define MAX_LEN_STR_CMD				30			//this is the max lenght of the string that contain the command received from the USART
#define MAX_TIMEOUT_CMD				1000			//mS. after this time if the command is not complete or it has error then reset the array command

#define NUM_BYTE_EEPROM_RF			20

#define MAX_DELAY_CARRIER_LED		10			//mS. Every time a byte is received from the NRF24 the carrier led is ON for this time

#define MAX_BUFFER_CMD_TO_RUN		20	


volatile char CMD_to_Run; //at 100 no command has to be run
volatile char buffer_CMD_to_Run[MAX_BUFFER_CMD_TO_RUN]; //here there are all command that has to be runned, there is a manager that will set the sequence of command to run
volatile unsigned char cont_buffer_CMD_to_Run;


typedef struct settings_G3P{
	char id_baud; 						//it contain a char between 1 and 7
	unsigned char len_array_data;		//this is the amount of byte sent/received for each radio transmission (payload)
	
	unsigned char radio_channel;		//this is the radio channel used to TXRX
	unsigned char radio_channel_hopping;		//this is the radio channel used to TXRX  with the frequency hopping together at radio_channel 
	
	unsigned char enabled_hopping;		//it can be '1' or '0'. This enable the frequency hopping
	
	char power_tx;						//this is the power to apply to the transmission 0=lower (-10dBm), 3=higher (+10dBm)
	char akn_yes;						//At '0' Acknowledge is disabled, at '1' Acknowledge is enabled. If enabled, it make to return (via USART) OK if transmission is successful, else it return ?A 
	
	unsigned char enabled_time_out_akn;	//This Enable/Disable the time out for the acknowledge
	
	unsigned char sem_send_OK_on_TX_good; //at 1 make the RF module to answer OK when there is a successful transmission  
	
	//unsigned int address_network;		
	//unsigned int address_device;
	
	char address_to_tx[4];				//address that to send data
	char address_this_device[4];		//address of this device
	char address_network[4];			//address network, is the byte 2 and byte 3 into the tx_address and rx_address
	
	unsigned char tx_address[5];		//address that to send data
	unsigned char rx_address[5];		//address of this device
	//		LSB								MSB
	//	{	0xE7,	0xE1,	0xE7,	0xE1,	0xAA	}; this are default value for tx_address and rx_address
	//		|___________|   |___________|   |__|
	//		Address TX/RX    Network Addr   ID my Transceiver
	
	unsigned char array_data_to_be_TX[26];		//this is filled up by command C30
	
	unsigned char sem_TX_running;		//this indicate if the transmission of the data loaded into array_data_to_be_TX is happening (managed into manager_RX_NRF24)
	//unsigned char cont_TX_running;		//this count how many byte have been transmitted, when this is equal to len_array_data than TX stop (managed into manager_RX_NRF24)
	
	unsigned char Sem_Send_Back_Via_UART_the_Asterisk; //this is used to check if send back the asterisk for a CMD executed

	
	//char *Name; //name of the peripheral
	//char NetAddress[5]; //it keep the 4 characters (they are 4 hexadecimal) which represent the address of the network 

} struct_settings_G3P;

volatile struct_settings_G3P *current_settings_G3P;	//is a variable that contain all settings of the G3P


volatile char str_cmd[MAX_LEN_STR_CMD];
volatile int len_str_cmd;
volatile int cont_time_out_cmd;					//if at 0 then reset the array command 

volatile int decont_delay_carrier_led;			//the carrier led is ON until the interrupt of the timer0 bring this variable to 0
												//this variable is managed into controller_activity_manager_RX_NRF24()

volatile char data_array_from_nrf24[30];
//volatile char tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
//volatile char rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};



const unsigned char eeprom_def_tab_G3P[]={					
	(unsigned char)('3')	, 	//0: 	id_baud: 			is the baud rate to init the USART. '3' that is 9600baud
	(unsigned char)(4)		,	//(24)		, 	//1: 	len_array_data: 	this is the ammount of byte sent/received for each radio transmission
	(unsigned char)(1)		, 	//2: 	radio_channel: 		this is the radio channel used to TXRX
	(unsigned char)('3')	, 	//3: 	power_tx: 			this is the power to apply to the transmission 0=lower (-10dBm), 3=higher (+10dBm)
	(unsigned char)(0)		, 	//4: 	akn_yes: 			At '0' Acknowledge is disabled, at '1' Acknowledge is enabled. If enabled, it make to return (via USART) OK if transmission is successful, else it return ?A
	(unsigned char)(1),//(0)		,	//5:	Sem_Send_Back_Via_UART_the_Asterisk

	(unsigned char)(0xE7)	, 	//6: 	address_this_device:	it is the MSB of the address (address of this device)
	(unsigned char)(0xE1)	, 	//7: 	address_this_device:	this is the MSB for the Network Address
	(unsigned char)(0xE7)	, 	//8: 	address_this_device		this is the LSB for the Network Address
	(unsigned char)(0xE1)	, 	//9: 	address_this_device		this is the MSB for the Device Address
	(unsigned char)(0xAA)	, 	//10: 	address_this_device:	this is the LSB for the Device Address

	(unsigned char)(0xE7)	, 	//11: 	address_to_tx:		it is the MSB of the device address to transmit data
	(unsigned char)(0xE1)	, 	//12: 	address_to_tx		this is the MSB for the Network Address
	(unsigned char)(0xE7)	, 	//13: 	address_to_tx		this is the LSB for the Network Address
	(unsigned char)(0xE1)	, 	//14: 	address_to_tx		this is the MSB for the Device Address
	(unsigned char)(0xAA)	, 	//15: 	address_to_tx:		this is the LSB for the Device Address

	(unsigned char)(0x00)	, 	//16:	radio_channel_hopping:	this is the radio channel used to TXRX  with the frequency hopping together at radio_channel
	(unsigned char)(0x00)	, 	//17:	enabled_hopping
	(unsigned char)(0x00)	, 	//18:	enabled_time_out_akn
	(unsigned char)(1)//(0x00)	 	//19:	sem_send_OK_on_TX_good

};



//Funzioni:

void 	Init_G3P(void);							//It init all variables and send the setting to the NRF

void 	Controller_USART_CMD_G3P(void);			//controller of the command received
												//this has to be into the interrupt routine of 1mS

void 	execute_USART_CMD(char byte_received);	//Giving the byte received, it will manage the array of data received
												//and execute the comands
												//this has to be into the USART iterrupt routine

				

				
void 	CMD_C00(char *str_cmd, int len_string_cmd);		//NN	//This function set the RF operating channel //expected parameter: NN  (2 char. Example can be "02" that select channel 2)		
			
void 	CMD_C01(char * str_cmd, int len_string_cmd);	//HHLL	//Default="E7E1" //Set the network address		

void 	CMD_C02(char * str_cmd, int len_string_cmd);	//HHLL	//Default="E7E1" //Set the address of this device		

void 	CMD_C03(char * str_cmd, int len_string_cmd);	//HHLL	//Default="E7E1" //Set the address to which it will transmit the data

void 	CMD_C04(char * str_cmd, int len_string_cmd);	//NN 	//Defaults="26"	//Set the number of bytes for each transmission

void 	CMD_C05(char *str_cmd, int len_string_cmd);		//NN	//This function set the RF channel for frequency hopping

void 	CMD_C07(char parameter, int len_string_cmd);	//N		//Default='3' (off)	//This function set TX power (3=0dBm, 2=-6dBm, 1=-12dBm, 0=-18dBm)  

void 	CMD_C08(void);									//		//This function init the Radio

void 	CMD_C09(char parameter, int len_string_cmd);	//N		//Default='0' (off)	//This function Enable/Disable the auto acknowledge

void 	CMD_C10(char parameter, int len_string_cmd);	//N		//Default='0' (off)	//This function Enable/Disable the frequency hopping

void 	CMD_C11(char parameter, int len_string_cmd);	//N		//Default='0' (off)	//This function Enable/Disable the time out for the acknowledge

void 	CMD_C15(char parameter, int len_string_cmd);	//N		//This function enable the reply of '*' for any command received via UART
void	check_if_send_asterisk(void);					//this function is used into the other CMD to check if send back the asterisk

void 	CMD_C20(char *str_cmd, int len_string_cmd);		//NNNN	//Default=20mS	//This function set the maximum waiting time to find the channel free. After this time it start the auto retry cycle.

void 	CMD_C21(char *str_cmd, int len_string_cmd);		//NNNN	//Default=600mS	//This function set maximum waiting time for AKN

void 	CMD_C22(char *str_cmd, int len_string_cmd);		//NNNN	//Default=500mS	//This function set the waiting time between two transmission when an error occur

void 	CMD_C23(char *str_cmd, int len_string_cmd);		//NN	//Default=05	//This function set the max number of retry in case of unsuccessful transmission

void 	CMD_C29(char parameter, int len_string_cmd);	//N		//Default='0' (off)	//This function set the RF module to answer OK when there is a successful transmission  

void 	CMD_C30(char *str_cmd, int len_string_cmd);		//Nx depend by C04		//This function get the data from the UART to be sent
		
void 	CMD_C31(void);									//		//Make the data loaded with C30 to be sent via Radio

void 	CMD_C45(void);							//Store the current settings into the EEPROM
	
void 	CMD_C46(void);							//Restore and apply the default settings

void 	CMD_C54(void);							//This function send via USART the identification of the transceiver

void 	CMD_C55(char parameter, int len_string_cmd);//Init the USART with a baud rate between 9600bps and 115200bps
												// parameter = 3 -> 9600bps
												// parameter = 4 -> 19200bps
												// parameter = 5 -> 38400bps
												// parameter = 6 -> 57600bps
												// parameter = 7 -> 115200bps
												
void	CMD_C85(void);							//This function send via USART the *

void	CMD_C89(void);							//This function send via USART the RX Address

void	CMD_C99(void);							//This function send back the current settings




void 	Reset_USART_CMD(void);					//It reset all variable that are used to check and execute the command received via USART

void	manager_RX_NRF24(void);					//this manage the data comming from the NRF24 via SPI

void	controller_activity_manager_RX_NRF24(void);		//this will end the activity started into the manager_RX_NRF24(), 	
														//this function run into the interrupt of the timer0
														
void	manager_TX_NRF24(void);					//this manage the data to TX to the NRF24 via SPI

void	function_cmd_to_run(void);						//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt 

unsigned char 	manager_CMD_to_Run(char New_CMD_to_Run);	//this function manage the sequence of command to run


#ifdef UART_DEBUG_G3P
	void 	UART_DEBUG_G3P_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
#endif
