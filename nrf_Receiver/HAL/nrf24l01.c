/*
nrf24l01 lib 0x02

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <string.h>
#include <stdio.h>
#include "nrf24l01.h"
#include "nrf24l01registers.h"
#include "../MCAL/EF_SPI.h"
#include  "../Utilities/EF_SpechialTIMER.h"
#include "../MCAL/EF_UART.h"

#define DEBUG_RF


/* address variables */
static uint8_t nrf24l01_addr0[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP0;
static uint8_t nrf24l01_addr1[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP1;
static uint8_t nrf24l01_addr2[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP2;
static uint8_t nrf24l01_addr3[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP3;
static uint8_t nrf24l01_addr4[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP4;
static uint8_t nrf24l01_addr5[NRF24L01_ADDRSIZE]  = NRF24L01_ADDRP5;

static uint8_t nrf24l01_addrtx[NRF24L01_ADDRSIZE] = NRF24L01_ADDRTX;


/*************************************************************************************
* Function    : uint8_t EF_nrf24l01_readregister(uint8_t reg)
*
* DESCRIPTION : This function used to read one byte from
*                a given register .
*
* PARAMETERS  : U8_t reg --> register
*
* Return Value: uint8_t : the reading value.
**************************************************************************************/
uint8_t EF_nrf24l01_readregister(uint8_t reg)
{
	uint8_t result=0;
	nrf24l01_CSNlow;                     //low CSN
	EF_SpiTransfer(R_REGISTER | (REGISTER_MASK & reg), NULL);
	EF_SpiTransfer(NOP,&result);        //read write register
    nrf24l01_CSNhi;                     //high CSN
    return result;
}

/*****************************************************************************************
* Function    : void EF_nrf24l01_readregisters(uint8_t reg, uint8_t *value, uint8_t len)
*
* DESCRIPTION : This function used to read more bytes from
*                a given register with length.
*
* PARAMETERS  : uint8_t reg    --> register
* 				uint8_t *value --> pointer to value that will be updated.
* 				uint8_t len    --> the length of the data.
*
* Return Value: void.
*****************************************************************************************/

void EF_nrf24l01_readregisters(uint8_t reg, uint8_t *value, uint8_t len)
{
	uint8_t i = 0;
	nrf24l01_CSNlow;    //low CSN
	EF_SpiTransfer(R_REGISTER | (REGISTER_MASK & reg),NULL);
	for(i=0; i<len; i++)
		EF_SpiTransfer(NOP,value+i);       //read write register
	nrf24l01_CSNhi;    //high CSN
}


/*********************************************************************************
* Function    : void EF_nrf24l01_writeregister(uint8_t reg, uint8_t value)
*
* DESCRIPTION : This function used to write or configure one byte into
*                a given register .
*
* PARAMETERS  : uint8_t reg   --> register
* 				uint8_t value --> the value to configure with it.
*
* Return Value: uint8_t : the reading value.
**********************************************************************************/

void EF_nrf24l01_writeregister(uint8_t reg, uint8_t value)
{
	nrf24l01_CSNlow;    //low CSN
	EF_SpiTransfer(W_REGISTER | (REGISTER_MASK & reg),NULL);
	EF_SpiTransfer(value,NULL); //write register
	nrf24l01_CSNhi;   //high CSN
}


/*****************************************************************************************
* Function    : void EF_nrf24l01_writeregisters(uint8_t reg, uint8_t *value, uint8_t len)
*
* DESCRIPTION : This function used to write or configure one byte into
*                a given register .
*
* PARAMETERS  : uint8_t reg    --> register
* 				uint8_t *value --> pointer to value that will be updated.
* 				uint8_t len    --> the length of the data.
*
* Return Value: uint8_t : void.
******************************************************************************************/
void EF_nrf24l01_writeregisters(uint8_t reg, uint8_t *value, uint8_t len)
{
	uint8_t i = 0;
	nrf24l01_CSNlow; //low CSN
    EF_SpiTransfer(W_REGISTER | (REGISTER_MASK & reg),NULL);
	for(i=0; i<len; i++)
		 EF_SpiTransfer(value[i],NULL); //write register
	nrf24l01_CSNhi; //high CSN
}

/*************************************************************
 *  	  reverse an array, NRF24L01 expects LSB first
 ************************************************************/
void nrf24l01_revaddress(uint8_t *addr, uint8_t *addrrev)
{
	//reverse address
	uint8_t i = 0;
	for(i=0; i<NRF24L01_ADDRSIZE; i++)
		memcpy(&addrrev[i], &addr[NRF24L01_ADDRSIZE-1-i], 1);
}


/*****************************************************************************************
* Function    : void nrf24l01_setrxaddr(uint8_t pipe, uint8_t *addr)
*
* DESCRIPTION : This function used to set the address of receiver for a given data pipe .
*
* PARAMETERS  : uint8_t pipe    --> data pipe
* 				uint8_t *addr --> pointer to address.
*
* Return Value: uint8_t : void.
*
* NOTE :   the register EN_RXADDR MUST be enable for the given pipe first.
******************************************************************************************/

void nrf24l01_setrxaddr(uint8_t pipe, uint8_t *addr)
{
	if(pipe == 0)
	{
		memcpy(&nrf24l01_addr0, addr, NRF24L01_ADDRSIZE);     //cache address
		uint8_t addrrev[NRF24L01_ADDRSIZE];
		nrf24l01_revaddress(addr, (uint8_t *)addrrev);
		/* write only LSB MSBytes are equal to RX_ADDR_P */
    	EF_nrf24l01_writeregisters(RX_ADDR_P0, addrrev, NRF24L01_ADDRSIZE);
	}
	else if(pipe == 1)
	{
		memcpy(&nrf24l01_addr1, addr, NRF24L01_ADDRSIZE);
		uint8_t addrrev[NRF24L01_ADDRSIZE];
		nrf24l01_revaddress(addr, (uint8_t *)addrrev);
		/* write only LSB MSBytes are equal to RX_ADDR_P */
    	EF_nrf24l01_writeregisters(RX_ADDR_P1, addrrev, NRF24L01_ADDRSIZE);
	}
	else if(pipe == 2)
	{
		memcpy(&nrf24l01_addr2, addr, NRF24L01_ADDRSIZE);
		EF_nrf24l01_writeregister(RX_ADDR_P2, addr[NRF24L01_ADDRSIZE-1]);
	}
	else if(pipe == 3)
	{
		memcpy(&nrf24l01_addr3, addr, NRF24L01_ADDRSIZE);
		EF_nrf24l01_writeregister(RX_ADDR_P3, addr[NRF24L01_ADDRSIZE-1]);
	}
	else if(pipe == 4)
	{
		memcpy(&nrf24l01_addr4, addr, NRF24L01_ADDRSIZE);
		EF_nrf24l01_writeregister(RX_ADDR_P4, addr[NRF24L01_ADDRSIZE-1]);
	}
	else if(pipe == 5)
	{
		memcpy(&nrf24l01_addr5, addr, NRF24L01_ADDRSIZE);
		EF_nrf24l01_writeregister(RX_ADDR_P5, addr[NRF24L01_ADDRSIZE-1]);
	}
}

/*********************************************************************************************
* Function    : void nrf24l01_settxaddr(uint8_t *addr)
*
* DESCRIPTION : This function used to set the address of transmitter.
*
* PARAMETERS  : uint8_t *addr --> pointer to address.
*
* Return Value: uint8_t : void.
**********************************************************************************************/

void nrf24l01_settxaddr(uint8_t *addr)
{
	memcpy(&nrf24l01_addrtx, addr, NRF24L01_ADDRSIZE); //cache address
	uint8_t addrrev[NRF24L01_ADDRSIZE];
	nrf24l01_revaddress(addr, (uint8_t *)addrrev);
	EF_nrf24l01_writeregisters(RX_ADDR_P0, addrrev, NRF24L01_ADDRSIZE); //set rx address for ack on pipe 0
	EF_nrf24l01_writeregisters(TX_ADDR, addrrev, NRF24L01_ADDRSIZE);    //set tx address
}

/********************************************************************************************
 *     			 flush RX fifo, the whole fifo
 ********************************************************************************************/
void nrf24l01_flushRXfifo()
{
	nrf24l01_CSNlow; //low CSN
	EF_SpiTransfer(FLUSH_RX,NULL);
	EF_SpiTransfer(NOP,NULL);

//	_delay_us(1000);
	nrf24l01_CSNhi; //high CSN
}

/*********************************************************************************************
 *     			 flush TX fifo, the whole fifo.
 *********************************************************************************************/
void nrf24l01_flushTXfifo()
{
	nrf24l01_CSNlow; //low CSN
	EF_SpiTransfer(FLUSH_TX,NULL);
	EF_SpiTransfer(NOP,NULL);

//	_delay_us(1000);
	nrf24l01_CSNhi; //high CSN
}

/*****************************************************************************************
* Function    : nrf24l01_setRX()
*
* DESCRIPTION : This function used to set the module in receiving state.
*
* PARAMETERS  : void.
*
* Return Value: void.
******************************************************************************************/
void nrf24l01_setRXMode()
{
	nrf24l01_setrxaddr(0, nrf24l01_addr0);       /* restore pipe 0 address */
	EF_nrf24l01_writeregister(CONFIG, EF_nrf24l01_readregister(CONFIG) | (1<<PRIM_RX));         //prx mode
	EF_nrf24l01_writeregister(CONFIG, EF_nrf24l01_readregister(CONFIG) | (1<<PWR_UP));          //power up
	EF_nrf24l01_writeregister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT));   //reset status
	nrf24l01_flushRXfifo();   //flush rx fifo
	nrf24l01_flushTXfifo();   //flush tx fifo
	nrf24l01_CEhi;            //start listening
	_delay_us(150);           //wait for the radio to power up
}

/*****************************************************************************************
* Function    : nrf24l01_setTX()
*
* DESCRIPTION : This function used to set the module in transmit state.
*
* PARAMETERS  : void.
*
* Return Value: void.
******************************************************************************************/
void nrf24l01_setTXMode()
{
	nrf24l01_CElow; //stop listening
	EF_nrf24l01_writeregister(CONFIG, EF_nrf24l01_readregister(CONFIG) & ~(1<<PRIM_RX));  //ptx mode
	EF_nrf24l01_writeregister(CONFIG, EF_nrf24l01_readregister(CONFIG) | (1<<PWR_UP));     //power up
	EF_nrf24l01_writeregister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT)); //reset status
	nrf24l01_flushTXfifo(); //flush tx fifo
	_delay_us(150); //wait for the radio to power up
}



/*****************************************************************************************
* Function    : void nrf24l01_printinfo(void(*prints)(const char *),
* 													void(*printc)(unsigned char data))
*
* DESCRIPTION : This function used to print the some information related
* 									to configuration of the registers.
*
* PARAMETERS  : void(*prints)(const char *). --> pointer to function
* 												to print a string over UART.
*				void(*printc)(unsigned char data))--> pointer to function to print
*													  a char.
* Return Value: void.
*
* NOTE: NRF24L01_PRINTENABLE MUST be enable to use it.
******************************************************************************************/

void nrf24l01_printinfo(void(*prints)( char *), void(*printc)(unsigned char data))
{
	char buff[100];
	prints("info\r\n");
	sprintf(buff,"STATUS: %02X\r\n", EF_nrf24l01_getstatus()); prints(buff);
	sprintf(buff,"CONFIG: %02X\r\n", EF_nrf24l01_readregister(CONFIG)); prints(buff);
	sprintf(buff,"RF_CH: %02X\r\n", EF_nrf24l01_readregister(RF_CH)); prints(buff);
	sprintf(buff,"RF_SETUP: %02X\r\n", EF_nrf24l01_readregister(RF_SETUP)); prints(buff);
	sprintf(buff,"EN_AA: %02X\r\n", EF_nrf24l01_readregister(EN_AA)); prints(buff);
	sprintf(buff,"EN_RXADDR: %02X\r\n", EF_nrf24l01_readregister(EN_RXADDR)); prints(buff);
//	sprintf(buff,"OBSERVE_TX: %02X\r\n", EF_nrf24l01_readregister(OBSERVE_TX)); prints(buff);
	prints("\r\n");
}



/*****************************************
 *  read status register
 ********************************************/
uint8_t EF_nrf24l01_getstatus()
{
	uint8_t status_reg = 0;
	nrf24l01_CSNlow; //low CSN
	EF_SpiTransfer(NOP,&status_reg); //get status, send NOP request
	nrf24l01_CSNhi; //high CSN
	return status_reg;
}


/*****************************************************************************************
* Function    : uint8_t nrf24l01_readready(uint8_t* pipe)
*
* DESCRIPTION : This function used to check if there any data into
* 					any data pipe ready to read .
*
* PARAMETERS  : uint8_t* pipe. >> pointer to the data pipe.
*
* Return Value: uint8_t.
******************************************************************************************/

uint8_t EF_nrf24l01_readready(uint8_t* pipe)
{
	uint8_t status = 0;
	status = EF_nrf24l01_getstatus();
    uint8_t ret = status & (1<<RX_DR);
    if(ret) {
		//get the pipe number
		if(pipe)
			*pipe = (status >> RX_P_NO) & 0b111;
    }
    return ret;
}

/**********************************************************************************************
* Function    : void nrf24l01_readData(uint8_t *data)
*
* DESCRIPTION : This function used to readData transmitted.
*
* PARAMETERS  : uint8_t* data. >> pointer to the data.
*
* Return Value: uint8_t.  TRUE if the read data complete.
******************************************************************************************/

uint8_t EF_nrf24l01_readData(uint8_t *data)
{
	uint8_t index = 0;
	uint8_t ret_read = 0;

	//read rx register
	nrf24l01_CSNlow;             //low CSN
    EF_SpiTransfer(R_RX_PAYLOAD,NULL);
    for(index=0; index<NRF24L01_PAYLOAD; index++)
		EF_SpiTransfer(NOP,(data+index));
    nrf24l01_CSNhi;            //high CSN
    /* reset register */
    EF_nrf24l01_writeregister(STATUS, (1<<RX_DR));
    /* handle ack payload receipt */
	if (EF_nrf24l01_getstatus() & (1<<TX_DS))
		EF_nrf24l01_writeregister(STATUS, (1<<TX_DS));
    /* return 1 if receive complete  */
    if(EF_nrf24l01_getstatus() & 1<<RX_DR)
    	ret_read = TRUE;

return ret_read;
}

/*****************************************************************************************
* Function    : uint8_t nrf24l01_writeData(uint8_t *data)
*
* DESCRIPTION : This function used to write data over nRF module.
*
* PARAMETERS  : uint8_t* data. >> pointer to the data.
*
* Return Value: uint8_t.
******************************************************************************************/

wrt_nrf_DataStatus EF_nrf24l01_writeData(uint8_t *data, uint8_t data_length)
{
	uint8_t index = 0;
	wrt_nrf_DataStatus ret_writData_status = nrf_transmit_failed;

	EF_void_TimerStart(NRF_SEND_ID);
	//set tx mode
	nrf24l01_setTXMode();
	//write data
	nrf24l01_CSNlow; //low CSN
	EF_SpiTransfer(W_TX_PAYLOAD,NULL);
	for(index=0; index < data_length; index++)
	{
		EF_SpiTransfer(data[index],NULL);
//		EF_void_UART_SendString("spi data ");
//		EF_void_UART_PutChar(data[index]);
//		EF_void_UART_SendString("\n");

	}
	nrf24l01_CSNhi; //high CSN
	//start transmission
	nrf24l01_CEhi; //high CE
	_delay_us(150);
	nrf24l01_CElow; //low CE

	//stop if max_retries reached or send is ok
	do {
		_delay_us(10);
	}
	while((!(EF_nrf24l01_getstatus() & (1<<MAX_RT | 1<<TX_DS))) && (!(EF_BOOLEAN_Timer_IsTimedOut(NRF_SEND_ID))));


	   if ((EF_nrf24l01_getstatus() == 0xFF))
		{
			ret_writData_status = nrf_not_connected;
#ifdef DEBUG_RF
			EF_void_UART_SendString(" RF NOT Connected \n");
#endif
		}
		else
		{
			if(EF_BOOLEAN_Timer_IsTimedOut(NRF_SEND_ID) == TRUE)
			{
				ret_writData_status = nrf_trnsmit_timeOut;
#ifdef DEBUG_RF
				EF_void_UART_SendString("wait ACK Time Out");
#endif
			}
			/* return 1 if transmit complete  */
			else if(EF_nrf24l01_getstatus() & (1<<TX_DS))
			{
				ret_writData_status = nrf_transmit_success;
#ifdef DEBUG_RF
				EF_void_UART_SendString("send date success\n");
#endif
			}
			/* return 0 if max auto transmition occurs or time out   */
			else if((EF_nrf24l01_getstatus() & (1<<MAX_RT)))
			{
				ret_writData_status = nrf_max_autoTransmit;
#ifdef DEBUG_RF
				EF_void_UART_SendString("max auto transmit \n");
#endif

			}

		}

    /* reset timer*/
	EF_void_TimerStop(NRF_SEND_ID);
	EF_void_TimerReset(NRF_SEND_ID);
	/* reset PLOS_CNT */
	EF_nrf24l01_writeregister(RF_CH, NRF24L01_CH);
	/* power down */
//	EF_nrf24l01_writeregister(CONFIG, EF_nrf24l01_readregister(CONFIG) & ~(1<<PWR_UP));
	/* set rx mode */
//	nrf24l01_setRXMode();

return ret_writData_status;
}

/*******************************************************************************************
 *     					  	set power level
 ********************************************************************************************/
uint8_t nrf24l01_setPwrlevel()
{
	uint8_t ret_value = 0;
	uint8_t setup = EF_nrf24l01_readregister(RF_SETUP);
	setup &= ~((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH));
	if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_MAX) {
		setup |= (1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH);
	} else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_HIGH) {
		setup |= (1<<RF_PWR_HIGH) ;
	} else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_LOW) {
		setup |= (1<<RF_PWR_LOW);
	} else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_MIN) {
	} else {
		//default is max power
		setup |= (1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH);
	}
	ret_value =  setup;
	EF_nrf24l01_writeregister(RF_SETUP, setup);
return ret_value;
}

/****************************************************************************************
 * 							set datarate
 ***************************************************************************************/
uint8_t nrf24l01_setDataRate()
{
  uint8_t ret_value = 0;
  uint8_t setup = EF_nrf24l01_readregister(RF_SETUP) ;
  setup &= ~((1<<RF_DR_LOW) | (1<<RF_DR_HIGH));
  if(NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_250KBPS) {
    setup |= (1<<RF_DR_LOW);
  } else {
    if (NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_2MBPS) {
    	setup |= (1<<RF_DR_HIGH);
    } else if (NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_2MBPS) {
    } else {
    	//default is 1Mbps
    }
  }
  ret_value = setup;
  EF_nrf24l01_writeregister(RF_SETUP, setup);
return ret_value;
}

/*************************************************************************************
 *        						set crc length
 *************************************************************************************/
uint8_t nrf24l01_setcrcLength()
{

  uint8_t ret_value = 0;
  uint8_t config = EF_nrf24l01_readregister(CONFIG) & ~((1<<CRCO) | (1<<EN_CRC));

  if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_DISABLED) {
	  //nothing
  } else if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_8) {
	  config |= (1<<EN_CRC);
  } else if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_16) {
	  config |= (1<<EN_CRC);
	  config |= (1<<CRCO);
  } else {
	  //default is disabled
  }
  ret_value = config;
  EF_nrf24l01_writeregister(CONFIG, config);
return ret_value;
}

/***************************************************************************************
 *								 init nrf24l01
 **************************************************************************************/
uint8_t EF_nrf24l01_init(uint8_t rf_channel)
{

	init_nrf_status ret_init_status = set_init_success;
	U8_t setup_value = 0;
	/* setup port */
	NRF24L01_DDR |= (1<<NRF24L01_CSN);   //output
	NRF24L01_DDR |= (1<<NRF24L01_CE);    //output
	EF_SpiInit(1);
	nrf24l01_CElow;         /* low CE */
	nrf24l01_CSNhi;         /* high CSN */
	/* wait for the radio to init */
	_delay_ms(5);
	 /* set power level */
	nrf24l01_setPwrlevel();
	/* set data rate */
	setup_value = nrf24l01_setDataRate();
	/* check if rf setup set correctly or not */
	if( EF_nrf24l01_readregister(RF_SETUP) != setup_value)
	{
		ret_init_status = set_setup_failed;
#ifdef DEBUG_RF
		EF_void_UART_SendString(" setup_failed \n");
		EF_void_UART_PutChar( EF_nrf24l01_readregister(RF_SETUP));
#endif
	}
	/* set crc length */
	nrf24l01_setcrcLength();
	/* set retries */
	EF_nrf24l01_writeregister(SETUP_RETR, NRF24L01_RETR);
	if(EF_nrf24l01_readregister(SETUP_RETR) != (NRF24L01_RETR) )
	{
		ret_init_status = set_retries_failed;
#ifdef DEBUG_RF
		EF_void_UART_SendString("set retries_failed \n");
		EF_void_UART_PutChar(EF_nrf24l01_readregister(SETUP_RETR));
#endif
	}
	 /* set RF channel */
	EF_nrf24l01_writeregister(RF_CH, rf_channel);
	/* check if rf channel set correctly or not */
	if(EF_nrf24l01_readregister(RF_CH) != rf_channel)
	{
		ret_init_status = set_channel_failed;
#ifdef DEBUG_RF
		EF_void_UART_SendString( " set channel_failed \n");
		EF_void_UART_PutChar(EF_nrf24l01_readregister(RF_CH));
#endif
	}

	/* enable data pipe */
	EF_nrf24l01_writeregister(EN_RXADDR, 0);
#if NRF24L01_ENABLEDP0 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P0));
#endif
#if NRF24L01_ENABLEDP1 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P1));
#endif
#if NRF24L01_ENABLEDP2 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P2));
#endif
#if NRF24L01_ENABLEDP3 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P3));
#endif
#if NRF24L01_ENABLEDP4 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P4));
#endif
#if NRF24L01_ENABLEDP5 == 1
	EF_nrf24l01_writeregister(EN_RXADDR, EF_nrf24l01_readregister(EN_RXADDR) | (1<<ERX_P5));
#endif
	/* auto ack */
#if NRF24L01_ACK == 1
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P0));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P1));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P2));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P3));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P4));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) | (1<<ENAA_P5));
#else
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P0));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P1));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P2));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P3));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P4));
	EF_nrf24l01_writeregister(EN_AA, EF_nrf24l01_readregister(EN_AA) & ~(1<<ENAA_P5));
#endif

	/* enable dynamic ACK with the payload and enable payload with ACK */
	EF_nrf24l01_writeregister(FEATURE,(1 << EN_DPL) | (1 << EN_ACK_PAY));
	/* enable dynamic ACK for pipe 0 */   /* for all pipes  */
	EF_nrf24l01_writeregister(DYNPD,(1<<DPL_P0)|(1<<DPL_P1)|(1<<DPL_P2)|(1<<DPL_P3)|(1<<DPL_P4)|(1<< DPL_P5));

	/* rx address */
	nrf24l01_setrxaddr(0, nrf24l01_addr0);
	nrf24l01_setrxaddr(1, nrf24l01_addr1);
	nrf24l01_setrxaddr(2, nrf24l01_addr2);
	nrf24l01_setrxaddr(3, nrf24l01_addr3);
	nrf24l01_setrxaddr(4, nrf24l01_addr4);
	nrf24l01_setrxaddr(5, nrf24l01_addr5);
	/* tx address */
	nrf24l01_settxaddr(nrf24l01_addrtx);
	/* set rx mode */
	nrf24l01_setRXMode();


return ret_init_status;
}
