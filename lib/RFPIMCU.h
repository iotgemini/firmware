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

#ifdef UART_DEBUG
	//#define UART_DEBUG_RFPIMCU //activate thedebug onthe UART for this library
#endif

//Follows the name of the peripherals. See the architecture where is the "n" command.
#define NAME_PERIPHERAL 		"RF00182" //max name lenght has to be equal or lower than 12 characters



//Follows the parameters of the peripherals. See the architecture where is the "t" command.
#define TYPE_H					var_ID_TYPE_H	//0		//it is the most significant byte of the univocal identifier  
#define TYPE_L					var_ID_TYPE_L	//7		//it is the less significant byte of the univocal identifier  
#define NUM_DIGITAL_INPUT		var_NUM_DIGITAL_INPUT //1 		//how many digital input has
#define NUM_DIGITAL_OUTPUT		var_NUM_DIGITAL_OUTPUT //2 		//how many digital output has
#define NUM_ANALOGUE_INPUT		var_NUM_ANALOGUE_INPUT //3		//how many analogue input has
#define NUM_ANALOGUE_OUTPUT		var_NUM_ANALOGUE_OUTPUT //0		//how many analogue output has
#define NUM_FUNCTIONS			var_NUM_FUNCTIONS //3		//how many functions
#define FIRMWARE				2		//firmware release
#define RADIO_CHANNEL			1		//channel used to operate normally

//Follows the IO to be configured with the port of your microcontroller
#ifdef RFNANO 
	#define PIN_BTN_PROGRAMMING_MODE 	bolean_pin_is_set(PINB,1)	//it is the pin where the button is connected (this pin has to be configured has input)
#else
	#define PIN_BTN_PROGRAMMING_MODE 	bolean_pin_is_set(PINB,0)	//it is the pin where the button is connected (this pin has to be configured has input)
#endif

#define LOGIC_BTN_PROGRAMMING_MODE 	0		//it used to negate the logic, value 1 or 0
//#define LOGIC_BTN_PROGRAMMING_MODE 	1 //ONLYFORDEMOBOARD

#define PIN_LED_TX_ON				PORTD|=0x80		//LED TX
#define PIN_LED_TX_OFF				PORTD&=~0x80	//LED TX
#define STATUS_LED_TX				bolean_pin_is_set(PIND,7)
#define LOGIC_LED					1		//it used to negate the logic, value 1 or 0

//IO radio module
//#define PIN_BUSY_RF				bolean_pin_is_set(PIND,6)	//it is at 1 when is busy the Radio Transceiver (this pin has to be configured has input)
//#define PIN_RES_RF_HIGH			PORTD |= 0x20	//Reset Radio Transceiver (this pin has to be configured has output)
//#define PIN_RES_RF_LOW			PORTD &= 0xDF
//#define PIN_SLEEP_RF_HIGH			PORTD |= 0x08	//
//#define PIN_SLEEP_RF_LOW			PORTD &=~0x08

//#define PIN_RX_RF				PD0	//RX Radio Transceiver (this pin has to be configured has output and conected to TX of the MCU UART)
//#define PIN_TX_RF				PD1	//TX Radio Transceiver (this pin has to be configured has input and conected to RX of the MCU UART)


//Follows the parameters for the normal operating functions
#define MAX_TIME_LED_TX_OFF		50  		//used to turn off LED TX to show it is transmitting. 
											//if for X milliseconds does not transmit data through the uart
											
#define TIME_OUT_UART_DATA		20			//used to reset the buffer counter of the UART
											//if for X milliseconds does not receive data from uart
											//and the comand is not parsed, then will reset
											
#define MAX_TIME_PROGRAMMING_MODE_ENABLED 50000 //when the button is pressed it will stay into the programming mode for 50 seconds


#define MIN_TIME_BUTTON_PRESSED	100			//if the button is still pressed after this delay in ms
											//then it will be detected and it will start the programming mode
	
#define TIME_CHECK_AND_PARSE_FUNCTIONS_PERI 100 //it is in ms. This avoid to send too much data through radio
												//every 100ms the function check_and_parse_functions_peri(void); is parsed

#define TIME_BTN_TO_RESET_ADDRESSES		4000 //mS. //here if the button is pressed for long time will reset the addresses to 0x0000

#define SIZE_BUFFER_UART			40		//size of the buffer for UART used to receive data from RF transceiver

#define	BAUD_RATE_UART			9600		//baud rate UART used to communicate with RF transceiver


//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------

//they keeps the address of the current Gateway Network
volatile unsigned char net_add[5] 	= "0000";
volatile unsigned char peri_add[5] 	= "0000";
volatile unsigned char rfpi_add[5] 	= "0000";

volatile unsigned char sender_add[5] = "0000"; //when receive a command


volatile unsigned char cont_cmd_rfpi;
volatile unsigned char cmd_rfpi[21]; //into this string will be kept the command comming from Gateway
							//is long 21 because the first 4 bytes is the address of the sender (Gateway)
							//and for each transmission from Gateway contain 16bytes of data, 
							//and there is one more for the '\0'

//Variables used by the EEPROM:
const unsigned char eeprom_def_tab_addresses[]={
	(unsigned char)(0), //0: net_add H
	(unsigned char)(0), //1: net_add L
	(unsigned char)(0), //2: peri_add H
	(unsigned char)(0), //3: peri_add L
	(unsigned char)(0), //4: rfpi_add H
	(unsigned char)(0), //5: rfpi_add L
	(unsigned char)(0) 	//6: bit0=sem_address_assigned
};

//const unsigned char eeprom_def_tab[]={
//	(unsigned char)(0)	, 	//0: 	bit0=PIN_RELE, bit1=PIN_GPIO1,
//	(unsigned char)(0)	, 	//1: 	sem_input_control_output
//
//	(unsigned char)(0)	, 	//2: 	semRelayTimer
//	(unsigned char)(0)	, 	//3: 	(RelayTimer_ms >> 8)
//	(unsigned char)(0)	, 	//4: 	(RelayTimer_ms)
//	(unsigned char)(5)	, 	//5: 	RelayTimer_SS
//	(unsigned char)(0)	, 	//6: 	RelayTimer_MM
//	(unsigned char)(0)	, 	//7: 	RelayTimer_HH
//
//	(unsigned char)(0)	, 	//8:	sem_trigger_temperature
//	(unsigned char)(40)	, 	//9:	trigger_temperature_low_INT
//	(unsigned char)(50)	, 	//10:	trigger_temperature_high_INT
//	(unsigned char)(0)	, 	//11:	temperature_offset_to_send_data_temperature
//
//	(unsigned char)(0)	,	//12:	semaphore trigger IN0 (0-10V)
//	(unsigned char)(100),	//13:	soglia bassa trigger IN0 (0-10V)
//	(unsigned char)(200),	//14:	soglia alta trigger IN0 (0-10V)
//	(unsigned char)(0)	,	//15:	semaphore trigger IN1 (0-10V)
//	(unsigned char)(100),	//16:	soglia bassa trigger IN1 (0-10V)
//	(unsigned char)(200)	//17:	soglia alta trigger IN1 (0-10V)
//
//};

//this constant are the comand used to drive the RF transceiver
const unsigned char C30[] = "C30"; //command to load the data (after this command has to follow the 16bytes)
const unsigned char C31[] = "C31"; //command to make the radio to send the data
const unsigned char C553[] ="C553"; //set baud rate 9600bps
const unsigned char C554[] ="C554"; //set baud rate 19200bps
const unsigned char C555[] ="C555"; //set baud rate 38400bps
const unsigned char C556[] ="C556"; //set baud rate 57600bps
const unsigned char C557[] ="C557"; //set baud rate 115200bps

//the following variables are just used by the UART
volatile unsigned char uart_receive_cnt;
volatile unsigned char uart_receive_buffer[SIZE_BUFFER_UART];
				
							
volatile unsigned char programming_mode_enabled; //used to keep under control when the button is pressed
										//thus will enter in programming mode or exit

volatile unsigned int	decontLedTX;	//used to turn off LED TX to show it is transmitting

volatile unsigned char	sem_Led_TX_keep_OFF;	//b0 LSB is used to turn off LED TX to show it is transmitting, 
												//b1 is used to enable/disable ADC compensation if MCU is not powerd with 5V
									
volatile unsigned int	decontDelayRFPI;	//used to create a delay function

volatile unsigned char	decontTimeOutSerialData;	//used to reset the data buffer from serial port

volatile unsigned int	decont_programming_mode_enabled; //used to go out when the time is elapsed

volatile unsigned char	decont_check_and_parse_functions_peri; //used for the function check_and_parse_functions_peri(void);

volatile unsigned char	sem_address_assigned;		//used for the radio control

volatile unsigned int	decont_reset_addresses;		//here if the button is pressed for long time will reset the addresses to 0x0000

volatile unsigned char 	sem_radio_module_connected;	//this is at 1 when F is connected to the board


//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
	//here the list of the functions to be edited to implement the features wanted
	void 			i_SendInStatus(void);					//Function to send the status of an input
	void 			p_SendOutStatus(void);					//Function to send the status of an output
	void 			o_SetOut(unsigned char *array_cmd, unsigned char enable_functions);		//Function to set the status of an output

	void 			f_SetFunction(void);					//Function to set the parameters of a special function
	void 			u_SendFunctionStatus(void);				//Function to send the status of a special function
	
	void 			s_Shields(void); 						//Function to load Shields settings that are connected to the transceiver
	
	void 			b_Reboot(void);							//Function make the transceiver to reboot itself
	
	void 			update_eeprom_peri(void);				//update status IO
	void			load_eeprom_peri(void);					//load status IO
	
	
	
	//here the list of the functions to be edited in order to use another MCU
	void 			interrupt_uart0_data_from_RF(unsigned char byte); 	//function to execute when occur the interrupt of data comming from the UART
	unsigned char	UART_check_data_RF(void);			//it check the UART buffer if there is data, return 0 if there is no data and 1 if there is data
														//(you have to write into this function the command for your MCU)
	void			load_eeprom_addresses(void);		//load network addresses
	void			update_eeprom_addresses(void);		//update network addresses
	
	
	
	//the following functions should be compatible with all MCU
	void			w_GetGatewayNetwork(void);		//it will get the addresses of the network (when the peripheral has been set in programming mode)
	void			t_PeriParameters(void);				//Function to reply with the identification parameters of our periphera
	void			n_SendPeriName(void);				//Function to reply with the name of this peripheral
	
	void			init_radio_RF(void);													//it init the radio
	
	void 			UART_send_to_RF(unsigned char data);									//it send a byte through the UART to the radio module 
																							//(you have to write into this function the command for your MCU)


	unsigned char	UART_get_last_byte_RF(void);												//get the last byte from the UART buffer
																							//(you have to write into this function the command for your MCU)
	
	void 			UART_send_STR_to_RF(unsigned char num_data, unsigned char *data);	//it send a string through the UART to the radio module 
	
	void 			UART_send_CMD_to_RF(unsigned char num_data, unsigned char *data); 	//it send a string command through the UART to the radio module 
	
	void			set_current_network_Gateway(void);									//set the current network addresess
		
	void			set_programming_network_Gateway(void); 								//set the programming network addresess
	

	
	unsigned char	hexascii_to_byte(unsigned char*strHextoConvert);						//convert a number written with ASCII character in a unsigned int number


	void		 	byte_to_hexascii(unsigned char *byteToConvert, unsigned char *strHexConverted);	
							//convert a number in HExadecimal format but written with ASCII characcters, the strHexConverted is an array long at least 2
							//if the number is 0x3F then strHexConverted[0] = '3' and strHexConverted[1] = 'F'

	unsigned char	check_bad_RF(void);		//check if there is communication with the radio module (good=0, bad=1)

	void 			check_and_parse_cmd_from_Gateway(void);	//here check if has been received 20byte from the UART, 
															//because 16bytes are the data and the first 4 bytes are the address of the transmitter (Gateway)
															//then it will parse the data received, the data obviously contain the command
															//and consecutively will execute the command
	
	unsigned char 	check_btn_to_enter_in_programming(unsigned char var_Semaphore);	//this function check if the reset is caused by the button,
															//if the button is not pressed then it start the installation into the IoT network
															//if the butto is still pressed then wait until is release and check if the minimum time has been passed to reset addresses:
															//if the minimum time is passed then reset the all addresses to 0x00 and blink the led otherwise it start the installation into the IoT network

	void 			interrupt_timer_of_1mS_RFPI(void);				//this interrupt has to be called into an interrupt of a timer that occur around every 1mS

	void 			delay_ms_RFPI(unsigned int time_ms_to_wait);//delay in milliseconds
	

	void 			get_cmd_from_uart_buffer(void);	//:
							//it reads uart_receive_buffer[] buffer and if there is a command it store the command into cmd_rfpi[] and set cont_cmd_rfpi=20
							//when cont_cmd_rfpi it is equal to 0 then will search again for a command into uart_receive_buffer[]
	
	void 			RF_wait_BUSY(void);		//check if the radio module is busy, if it is busy then stop untill its free

	unsigned char 	instring(unsigned char* k, unsigned char* s);	//useful function to know if inside a string is contained a given string
	
	void 			UART_send_and_fill_data_to_RF(unsigned char *data, unsigned char num_data);
	
	void 			Led_TX_OFF_and_after_delay_turn_ON(void);

	
	#ifdef UART_DEBUG_RFPIMCU
	void 	UART_DEBUG_RFPIMCU_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
	void 	UART_DEBUG_RFPIMCU_send_STR2(unsigned char *data, unsigned char line_feed);
	#endif
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
