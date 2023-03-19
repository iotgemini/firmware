/*****************************************************************

	MCU: 					ATmega328p
	Autore: 				Emanuele Aimone
	Last Update:			01 / 03 / 2023
	
	Descrizione:			Libreria SPI per ATmega
	
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



//Funzioni:
void 	SPI_MasterInit(void);					//Inizializza la porta SPI come MASTER
uint8_t SPI_SendByte(uint8_t databyte);			//Invia un byte tramite port SPI
