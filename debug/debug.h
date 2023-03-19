/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023


	List compatible MCU: 	ATmega328

	Description:			Common functions for ATmega328


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

#ifndef DEBUG_DEBUG_H_
#define DEBUG_DEBUG_H_

/*****************************************************************
 * 	DEFINE
*****************************************************************/

/*****************************************************************
 * 	PUBLIC VARIABLES
*****************************************************************/
extern int cont1_debug;

/*****************************************************************
 * 	PRIVATE VARIABLES
*****************************************************************/

/*****************************************************************
 * 	PUBLIC FUCTIONS
*****************************************************************/
void 	UART_DEBUG_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed); //it send a string through the UART only if is in debug mode
void 	UART_DEBUG_send_STR2(unsigned char *data, unsigned char line_feed);
void 	UART_DEBUG_send_NUM_BYTE_HEX(unsigned char byte_to_convert, unsigned char line_feed);
void 	UART_DEBUG_send_STARS(unsigned char num_star_to_send);

/*****************************************************************
 * 	PRIVATE FUCTIONS
*****************************************************************/



#endif /* DEBUG_DEBUG_H_ */
