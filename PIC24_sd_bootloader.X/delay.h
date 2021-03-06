//
//  Copyright(c) 2017, SenSprout Inc.All rights reserved.
//

#ifndef DELAY_H_
#define DELAY_H_



#ifndef _FCY
#ifdef __XC16
//#define FCY 32000000UL
#define _FCY 16000000UL
#else
#define _FCY 40000000UL
#endif
#endif


//#define I_USE_TIMER
#define I_USE_TRIS


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __XC16
 extern void __delay32(unsigned long cycles);
#define delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(_FCY)/1000ULL)); }
#define delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(_FCY)/1000000ULL)); }
#else
 
 

void delay_ms (unsigned int n);
void delay_us (unsigned int n);


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

