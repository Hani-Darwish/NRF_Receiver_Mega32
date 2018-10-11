/*
nrf24l01 lib 0x02

copyright (c) Davide Gironi, 2012

References:
  -  This library is based upon nRF24L01 avr lib by Stefan Engelke
     http://www.tinkerer.eu/AVRLib/nRF24L01
  -  and arduino library 2011 by J. Coliz
     http://maniacbug.github.com/RF24

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef _NRF24L01_H_
#define _NRF24L01_H_
#include <avr/io.h>
#include "../HAL/nrf24l01registers.h"



//CE and CSN port definitions
#define NRF24L01_DDR    DDRB
#define NRF24L01_PORT   PORTB
#define NRF24L01_CE     PB3
#define NRF24L01_CSN    PB4

/* define the spi path */
#define NRF24L01_SPIPATH "../spi/spi.h" //spi lib path
 /* enable print info function */
#define NRF24L01_PRINTENABLE 1
/* CE and CSN functions */
#define nrf24l01_CSNhi     NRF24L01_PORT |=  (1<<NRF24L01_CSN);
#define nrf24l01_CSNlow    NRF24L01_PORT &= ~(1<<NRF24L01_CSN);
#define nrf24l01_CEhi      NRF24L01_PORT |=  (1<<NRF24L01_CE);
#define nrf24l01_CElow     NRF24L01_PORT &= ~(1<<NRF24L01_CE);

/* power setup */
#define NRF24L01_RF24_PA_MIN             1
#define NRF24L01_RF24_PA_LOW             2
#define NRF24L01_RF24_PA_HIGH            3
#define NRF24L01_RF24_PA_MAX             4
#define NRF24L01_RF24_PA                 NRF24L01_RF24_PA_MAX

/* speed setup */
/* NOTE : When using Enhanced ShockBurs mode
 * the Data is transmitted at high speed (1Mbps or 2Mbps). */

#define NRF24L01_RF24_SPEED_250KBPS      1
#define NRF24L01_RF24_SPEED_1MBPS        2
#define NRF24L01_RF24_SPEED_2MBPS        3
#define NRF24L01_RF24_SPEED              NRF24L01_RF24_SPEED_1MBPS

/* crc setup */
#define NRF24L01_RF24_CRC_DISABLED       1
#define NRF24L01_RF24_CRC_8              2
#define NRF24L01_RF24_CRC_16             3
#define NRF24L01_RF24_CRC                NRF24L01_RF24_CRC_16

/* transmission channel */
#define NRF24L01_CH                      92
#define NRF24L01_DATA_PIPE               2
/* payload lenght */
#define NRF24L01_PAYLOAD    			 32
/* auto ack enabled */
#define NRF24L01_ACK        			 1
/* auto retransmit delay and count 1500uS, 15 times */
#define NRF24L01_RETR (0b1111 << ARD) | (0b1111 << ARC)

/* enable or disable pipe */
#define NRF24L01_ENABLEDP0  1     // pipe 0
#define NRF24L01_ENABLEDP1  0     // pipe 1
#define NRF24L01_ENABLEDP2  1     // pipe 2
#define NRF24L01_ENABLEDP3  0     // pipe 3
#define NRF24L01_ENABLEDP4  0     // pipe 4
#define NRF24L01_ENABLEDP5  0     // pipe 5
/* address size */
#define NRF24L01_ADDRSIZE 5
/* pipe address */
#define NRF24L01_ADDRP0 {0xE8, 0xE8, 0xF0, 0xF0, 0xE2}      /* pipe 0, 5 byte address */

#define NRF24L01_ADDRP1 {0xC1, 0xC2, 0xC2, 0xC2, 0xA1}      /* pipe 1, 5 byte address */
#define NRF24L01_ADDRP2 {0xC1, 0xC2, 0xC2, 0xC2, 0xB2}      /* pipe 2, 5 byte address */
#define NRF24L01_ADDRP3 {0xC1, 0xC2, 0xC2, 0xC2, 0xC3}      /* pipe 3, 5 byte address */
#define NRF24L01_ADDRP4 {0xC1, 0xC2, 0xC2, 0xC2, 0xD4}      /* pipe 4, 5 byte address */
#define NRF24L01_ADDRP5 {0xC1, 0xC2, 0xC2, 0xC2, 0xE5}      /* pipe 5, 5 byte address */

#define NRF24L01_ADDRTX {0xE8, 0xE8, 0xF0, 0xF0, 0xE2}      /* tx default address */

/* type for transmit status*/
typedef enum
{
	nrf_transmit_success,
	nrf_trnsmit_timeOut,
	nrf_max_autoTransmit,
	nrf_transmit_failed,
	nrf_not_connected,
}wrt_nrf_DataStatus;



/* type for init status */
typedef enum
{
	set_init_success,
	set_channel_failed,
	set_config_failed,
	set_status_failed,
	set_setup_failed,
	set_retries_failed,
}init_nrf_status;


/*APIs Prototypes*/
extern wrt_nrf_DataStatus   EF_nrf24l01_writeData(uint8_t *data, uint8_t data_length);
extern uint8_t   EF_nrf24l01_readready(uint8_t* pipe);
extern uint8_t   EF_nrf24l01_readData(uint8_t *data);
extern void      nrf24l01_settxaddr(uint8_t *addr);
uint8_t EF_nrf24l01_init(uint8_t rf_channel);
/* low level functions */
uint8_t EF_nrf24l01_readregister(uint8_t reg);
void    EF_nrf24l01_readregisters(uint8_t reg, uint8_t *value, uint8_t len);
void    EF_nrf24l01_writeregister(uint8_t reg, uint8_t value);
void    EF_nrf24l01_writeregisters(uint8_t reg, uint8_t *value, uint8_t len);

/* Atomic function used to init the nrf24l01 */
uint8_t nrf24l01_setPwrlevel(void);
uint8_t nrf24l01_setDataRate(void);
uint8_t nrf24l01_setcrcLength(void);
uint8_t EF_nrf24l01_getstatus(void);
void    nrf24l01_flushRXfifo(void);
void 	nrf24l01_flushTXfifo(void);
void	nrf24l01_setRXMode(void);
void 	nrf24l01_setTXMode(void);
void 	nrf24l01_revaddress(uint8_t *addr, uint8_t *addrrev);
void 	nrf24l01_setrxaddr(uint8_t pipe, uint8_t *addr);


#if NRF24L01_PRINTENABLE == 1
extern void nrf24l01_printinfo(void(*prints)( char *), void(*printc)(unsigned char data));
#endif


#endif
