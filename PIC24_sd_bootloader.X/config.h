/********************************************************************
*
* PIC24F Serial Bootloader
*
*********************************************************************
* FileName:		config.h
* Dependencies:
* Processor:	PIC24F Family
* Compiler:		C30 v3.00 or later
*SD CARD BOOTLOADER 
********************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

//Includes *************************************************************************
#include <xc.h>
#include "GenericTypeDefs.h"
//**********************************************************************************


//Configuration Defines ************************************************************

//Bootloader device configuration
#define DEV_HAS_WORD_WRITE	//Device has word write capability (24FJxxx devices)
//#define DEV_HAS_PPS			//Device has Peripheral Pin Select (PPS) feature
//#define DEV_HAS_CONFIG_BITS	//Device has Configuration Bits
//#define DEV_HAS_EEPROM		//Device has internal data EEPROM
//#define DEV_HAS_USB			//Device is a USB capable device with a 96MHz PLL


//Bootloader feature configuration
#define USE_BOOT_PROTECT	//Use BL block protection 
//#define USE_RUNAWAY_PROTECT	//Provide runaway code protection using program flow
//#define USE_CONFIGWORD_PROTECT	//Protect last page from writes/erases
//#define USE_VECTOR_PROTECT  //Use Reset and IVT protection
//#define USE_HI_SPEED_BRG	//Use BRGH=1, UART high speed mode
//#define USE_WORKAROUNDS	//UART workarounds for device errata
//#define USE_AUTOBAUD      //Use hardware autobaud feature
//#define USE_AES //Use encryption
//#define USE_RESET_SAVE    //Restores the reset vector without using USE_BOOT_PROTECT


//HEX file name.
#define HEX_FILE_NAME "TEST.HEX"

//Bootloader Operation Configuration
#define MAJOR_VERSION		0x01	//Bootloader FW version
#define MINOR_VERSION		0x02

#define FCY     16000000	//Instruction clock speed (Fosc/2)

#define UARTNUM     1       //Which device UART to use

#ifndef USE_AUTOBAUD
    //#define BAUDRATE    38400
    #define BAUDRATE    115200
#endif

#define MAX_PACKET_SIZE		261	//Max packet size
//#define USER_PROG_RESET     0x100	//User app reset vector location

//USER_PROG_RESET should be the location of a pointer to the start of user code, 
//not the location of the first instruction of the user application.
//#define USER_PROG_RESET     0x100	//User app reset vector location
#define USER_PROG_RESET     0x100
#define USER_PROG_RESET_ADDR 0x80C0
#define DELAY_TIME_ADDR 	0x102	//BL entry delay location

#define BOOT_ADDR_LOW 		0x400	//start of BL protection area
#ifndef USE_AES
	#define BOOT_ADDR_HI  		0x8000	//end of BL protection area 
#else
 	#define BOOT_ADDR_HI  		0x13FF	//end of BL protection area ** USE 0x13FF for AES support
#endif

//If using encryption, set the AES encryption key
#ifdef USE_AES
	#define AES_KEY {0x0100,0x0302,0x0504,0x0706,0x0908,0x0B0A,0x0D0C,0x0F0E}
#endif

//If using RPx pins multiplexed with ANx functions, uncomment these lines to configure AD1PCFG
//#define UTX_ANA		AD1PCFGbits.PCFG2
//#define URX_ANA		AD1PCFGbits.PCFG4

//Define UART pins for PPS devices, by default Explorer 16 settings are used
#define DEV_HAS_PPS
#ifdef DEV_HAS_PPS	
	#if defined(__PIC24FJ64GA004__)
		#define PPS_UTX_PIN		RPOR12bits.RP25R //UART TX pin
		#define PPS_URX_PIN		19				 //UART RX pin
        #define PPS_URX_REG    	RPINR19bits.U2RXR

    
    #elif defined __PIC24FJ64GB002__
        #define PPS_UTX_PIN		RPOR1bits.RP3R	//UART TX pin
		#define PPS_URX_PIN 	2				//UART RX pin
        #define PPS_URX_REG    	RPINR18bits.U1RXR

	#elif (defined(__PIC24FJ256GB110__) || \
		   defined(__PIC24FJ256GA110__))
		#define PPS_UTX_PIN		RPOR8bits.RP17R	//UART TX pin
		#define PPS_URX_PIN 	10				//UART RX pin
        #define PPS_URX_REG    	RPINR19bits.U2RXR
	#endif
#endif

	
//Device Flash Memory Configuration
//Size of flash memory sections in bytes
//includes upper phantom byte (32-bit wide memory)
#if defined(__PIC24F__)     //PIC24F "J" type devices
    #define PM_INSTR_SIZE 		4		//bytes per instruction 
    #define PM_ROW_SIZE 		256  	//user flash row size in bytes 
    #define PM_PAGE_SIZE 		2048 	//user flash page size in bytes

#elif defined(__PIC24FK__)  //PIC24F "K" type devices
    #define PM_INSTR_SIZE 		4		//bytes per instruction 
    #define PM_ROW_SIZE 		128  	//user flash row size in bytes 
    #define PM_PAGE_SIZE 		128 	//user flash page size in bytes
#endif

//Vector section is either 0 to 0x200 or 0 to end of first page, whichever is larger
#define VECTOR_SECTION      ((0x200>(PM_PAGE_SIZE/2))?0x200:(PM_PAGE_SIZE/2)) 

#ifdef DEV_HAS_CONFIG_BITS
	#define CM_ROW_SIZE 		1		//configuration row size in bytes
#endif

#if		defined(__PIC24FJ128GA010__)
	#define CONFIG_START 		0x157FC	//Flash config word locations for devices
	#define CONFIG_END   		0x157FE	//w/o config bits

#elif  (defined(__PIC24FJ256GB110__)|| \
       	defined(__PIC24FJ256GA110__))
	#define CONFIG_START 		0x2ABFA	
	#define CONFIG_END   		0x2ABFE

#elif 	defined(__PIC24FJ64GA004__)
	#define CONFIG_START 		0xABFC	
	#define CONFIG_END 		    0xABFE	

#elif 	defined(__PIC24FJ64GB002__)
	#define CONFIG_START 		0xABF8 //TODO : set config
	#define CONFIG_END 		    0xABFE //TODO : set config

#elif   defined(__PIC24F16KA102__)
	#define CONFIG_START 		0xF80000	
    #define CONFIG_END 		    0xF80010

#else
    #warning "No config location defined... using default config locations"
	#define CONFIG_START 		0x157FC	
	#define CONFIG_END   		0x157FE

#endif


//Self-write NVMCON opcodes	
#if defined(__PIC24F__)     //PIC24F "J" type devices
    #define PM_PAGE_ERASE 		0x4042	//NVM page erase opcode
    #define PM_ROW_WRITE 		0x4001	//NVM row write opcode

#elif defined(__PIC24FK__)  //PIC24F "K" type devices
    #define PM_PAGE_ERASE 		0x4058	//NVM page erase opcode
    #define PM_ROW_WRITE 		0x4004	//NVM row write opcode
#endif

#ifdef DEV_HAS_WORD_WRITE
	#define PM_WORD_WRITE		0x4003	//NVM word write opcode
#endif

#ifdef DEV_HAS_EEPROM
	#define EE_WORD_WRITE		0x4004	//EE word write opcode
	#define EE_ROW_WRITE		0x4006	//EE row write opcode
#endif

#ifdef DEV_HAS_CONFIG_BITS
	#define CONFIG_WORD_WRITE	0x4004	//Config memory write opcode
#endif	

//**********************************************************************************

//UART Baud Rate Calculation *******************************************************
#ifndef USE_AUTOBAUD

//#define USE_HI_SPEED_BRG
#ifdef USE_HI_SPEED_BRG
    #define BRG_DIV 4
#else
    #define BRG_DIV 16
#endif

/*
#define BAUDRATEREG    ((FCY + (BRG_DIV/2*BAUDRATE))/BRG_DIV/BAUDRATE-1)
#define BAUD_ACTUAL    (FCY/BRG_DIV/(BAUDRATEREG+1))

#define BAUD_ERROR          ((BAUD_ACTUAL > BAUDRATE) ? BAUD_ACTUAL-BAUDRATE : BAUDRATE-BAUD_ACTUAL)
#define BAUD_ERROR_PRECENT  ((BAUD_ERROR*100+BAUDRATE/2)/BAUDRATE)

#if (BAUD_ERROR_PRECENT > 3)
//    #error "UART frequency error is worse than 3%"
#elif (BAUD_ERROR_PRECENT > 2)
    #warning "UART frequency error is worse than 2%"
#endif
*/

#endif
//**********************************************************************************

//Constant Defines *****************************************************************
//Protocol Commands
/*
#define RD_VER 		0x00
#define RD_FLASH 	0x01
#define WT_FLASH	0x02
#define ER_FLASH	0x03
#define	RD_EEDATA	0x04
#define WT_EEDATA	0x05
#define RD_CONFIG	0x06
#define WT_CONFIG	0x07
#define VERIFY_OK	0x08

//Communications Control bytes
#define STX 0x55
#define ETX 0x04
#define DLE 0x05
 * */
//**********************************************************************************


//Support for PIC24F PPS ***********************************************************
#ifdef DEV_HAS_PPS
	//PPS Outputs
	#define NULL_IO		0
	#define U1TX_IO		3
	#define U1RTS_IO	4
	#define U2TX_IO		5
	#define U2RTS_IO	6

	//extern void ioMap();
#endif
//**********************************************************************************

//UART Configuration ***************************************************************
/*
#define UARTREG2(a,b)     U##a##b
#define UARTREG(a,b)    UARTREG2(a,b)

#define UxMODE      UARTREG(UARTNUM,MODE)
#define UxBRG       UARTREG(UARTNUM,BRG)
#define UxSTA       UARTREG(UARTNUM,STA)
#define UxRXREG     UARTREG(UARTNUM,RXREG)
#define UxTXREG     UARTREG(UARTNUM,TXREG)
#define UxMODEbits  UARTREG(UARTNUM,MODEbits)
#define UxSTAbits   UARTREG(UARTNUM,STAbits)

#ifdef DEV_HAS_PPS
    #define UxTX_IO UARTREG(UARTNUM,TX_IO)
#endif
 * */
//**********************************************************************************



//Function Prototypes **************************************************************
/*
    extern DWORD ReadLatch(WORD, WORD);
extern void Erase(WORD, WORD, WORD);
extern void WriteLatch(WORD, WORD, WORD, WORD);
extern void WriteMem(WORD);
extern void ResetDevice(WORD);
*/
//void PutChar(BYTE);
//void GetChar(BYTE *);
//void ReadPM(WORD, DWORD_VAL);
//void WritePM(WORD, DWORD_VAL);
//void ErasePM(WORD, DWORD_VAL);
//void WriteTimeout();
//void GetCommand();
//void HandleCommand();
//void PutResponse(WORD);
//void AutoBaud();
#if defined(USE_BOOT_PROTECT) || defined(USE_RESET_SAVE)
//void replaceBLReset(DWORD_VAL);
#endif
//**********************************************************************************


//Configuration Check **************************************************************
#if ((defined(DEV_HAS_WORD_WRITE) && defined(DEV_HAS_CONFIG_BITS)) || \
	 (defined(DEV_HAS_WORD_WRITE) && defined(DEV_HAS_EEPROM)))
	#warning "No devices support configured feature set."
#endif
//**********************************************************************************

#endif //ifdef CONFIG_H
