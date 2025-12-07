/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Last Update:			01 / 03 / 2023
	
	Descrizione:			Libreria per scrivere/leggere i dati 
							nella/dalla EEPROM. I dati provenienti 
							da un array vengono scritti sulle 
							celle della EEPROM.
	
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


void save_array_data_EEPROM(unsigned char *array_data, unsigned int lenght_array_data, unsigned int var_START_ADDRESS_WHERE_TO_SAVE){ 
	unsigned int i=0;
	while(i<lenght_array_data){
		CLR_WDT(); //it reset the WDT
		eeprom_write_byte ((unsigned char *)(i+var_START_ADDRESS_WHERE_TO_SAVE), array_data[i]);
		i++;
	}
}

unsigned char return_array_data_EEPROM(unsigned char *array_data, unsigned int lenght_array_data, unsigned int var_START_ADDRESS_WHERE_TO_SAVE){
	unsigned char varReturn=0;
	unsigned int i; 
	unsigned int contVirginByte;
	
	//check if is virgin
	i=0; contVirginByte=0;
	while(i<lenght_array_data){ //it check when it finds the 0xFF
		CLR_WDT(); //it reset the WDT
		if(eeprom_read_byte((unsigned char *)(i+var_START_ADDRESS_WHERE_TO_SAVE)) == VALUE_VIRGIN_BYTE){
			contVirginByte++;
		}
		i++;
	}
	if(contVirginByte==lenght_array_data){ 
		varReturn = 1; //an array of data has never been saved
	}else{
		i=0;
		while(i<lenght_array_data){
			CLR_WDT(); //it reset the WDT
			array_data[i] = (unsigned char)eeprom_read_byte((unsigned char *)(i+var_START_ADDRESS_WHERE_TO_SAVE));
			i++;
		}
		
	}
	
	return varReturn;
}


