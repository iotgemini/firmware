/*****************************************************************

	MCU: 					ATmega328p
	Autore: 				Emanuele Aimone
	Last Update:			01 / 03 / 2023
	
	Descrizione:			Libreria USART per ATmega
	
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



//Importante: bisogna cambiare la frequenza F_CPU definita qui sotto 
//in modo che sia uguale a quella usata sul microcontrollore
//Se i fuse sono impostati per una frequenza interna da 8MHz allora
//F_CPU deve essere 8000000UL
#ifndef F_CPU
	//#define F_CPU 8000000UL  // 8 MHz
	#define F_CPU 16000000UL  // 16 MHz
#endif 

	
//Funzioni:
void USART_Init_with_baudrate(unsigned int baudrate);
void USART_SendByte(unsigned char data); 	//carica il byte nel relativo 
											//registro di invio e invia il dato

// Aspetta fino a quando un dato non viene ricevuto e poi restituisce il dato
unsigned char USART_ReceiveByte(void);
