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



//Inizializza la porta SPI come MASTER
void SPI_MasterInit(void)
{
	DDRB |= (1<<PIN_SS)|(1<<PIN_MOSI)|(1<<PIN_SCK);    // SCK, MOSI and SS as outputs
    //DDRB &= ~(1<<PIN_MISO);                 // MISO as input

    SPCR |= (1<<MSTR);               // Set as Master
    SPCR |= (1<<SPR0)|(1<<SPR1);     // divided clock by 128
    SPCR |= (1<<SPE);                // Enable SPI
}

//Invia un byte tramite port SPI
uint8_t SPI_SendByte (uint8_t databyte)
{
    // Copy data into the SPI data register
    SPDR = databyte;
    // Wait until transfer is complete
    while (!(SPSR & (1 << SPIF)));
	
	return SPDR;
}
