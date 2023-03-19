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





//inizializza i registri per la USART
void USART_Init_with_baudrate(unsigned int baudrate){
  
   unsigned int baud_prescale;
  
   #if F_CPU == 16000000UL

	    if(baudrate==19200){	baud_prescale=52-1;
	    }else if(baudrate==38400){	baud_prescale=26-1; 	//  with F_CPU=16MHz
	    }else if(baudrate==57600){
		   baud_prescale=35-1; 	//  with F_CPU=16MHz
		   UCSR0A = ((1<<U2X0)); //double the UART speed
	    }else if(baudrate==11520){
			baud_prescale=17-1; //115200baud	// 	with F_CPU=16MHz
			UCSR0A = ((1<<U2X0)); //double the UART speed
	    }else{
			baud_prescale=104-1;	//else 9600baud		// 	with F_CPU=16MHz	
		}
   
   //#elif F_CPU == 8000000UL
   #else
   
	    if(baudrate==19200){	baud_prescale=26-1;
	    }else if(baudrate==38400){	baud_prescale=13-1; 	//38461  with F_CPU=8MHz
	    }else if(baudrate==57600){
			//baud_prescale=9-1; 		//55555  with F_CPU=8MHz
			baud_prescale=17-1; 	// baud for 57600 with F_CPU=8MHz
			UCSR0A = ((1<<U2X0)); //double the UART speed
		//}else if(baudrate==11520){
			////baud_prescale=4-1; //115200baud	//125000 with F_CPU=8MHz
			//baud_prescale=9-1; 	//  with F_CPU=8MHz
			//UCSR0A = ((1<<U2X0)); //double the UART speed
			
		}else{
			baud_prescale=52-1;							//9615	 with F_CPU=8MHz
		}
   
   #endif

   //Imposta il baud rate
   UBRR0L = baud_prescale; 		 //carica i primi 8bit nel registro UBRR
   UBRR0H = (baud_prescale >> 8); //adesso carica i sucessivi 8bit nel registro UBRR
	 
  //per settare il numero di bit per trasmissione, la parit� e gli stop bit 
  //della seriale bisogna modificare
  //il registro UCSRC che per� di default ha i valori che servono a noi:
  //8bit dati, no parit�, 1 stop bit

   // Abilita la ricezione, trasmissione e l'interrupt della USART
   UCSR0B  = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));
  
   
}

//carica il byte nel relativo registro di invio e invia il dato
void USART_SendByte(unsigned char data){

  //Se l'ultimo dato caricato non � ancora stato trasmesso allora
  //aspetta che venga inviato
  while(!(UCSR0A & (1<<UDRE0)));

  //Carica e trasmette il dato
  UDR0 = data;
}

// Funzione non usata, ma qui per completezza
// Aspetta fino a quando un dato non viene ricevuto e poi restituisce il dato
unsigned char USART_ReceiveByte(void){
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}
