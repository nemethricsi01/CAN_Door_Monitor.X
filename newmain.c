
// PIC18F25K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = HS1       // Oscillator (HS oscillator (Medium power, 4 MHz - 16 MHz))
#pragma config PLLCFG = ON      // PLL x4 Enable bit (Enabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = ON     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-01FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 02000-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-01FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 02000-03FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 16000000
struct CAN_RXBUFF
   {
    uint8_t idh;
    uint8_t idl;
    uint16_t fullid;
    uint8_t dl;
    uint8_t d0;
    uint8_t d1;
    uint8_t d2;
    uint8_t d3;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
   };
struct CAN_RXBUFF can_rxbuff;
uint16_t ledtimer = 0;
uint8_t  canMessageFlag;
uint16_t ledHighTimer,ledLowTimer;
void can_init(void)
{

    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 1;
    
    CANCONbits.REQOP = 0b100;//request config mode
    while (0x80 != (CANSTAT & 0xE0)); // wait until ECAN is in config mode
    ECANCON = 0x00;
    RXM0EIDH = 0x00;
    RXM0EIDL = 0x00;
    RXM0SIDH = 0x00;
    RXM0SIDL = 0x00;
    RXM1EIDH = 0x00;
    RXM1EIDL = 0x00;
    RXM1SIDH = 0x00;
    RXM1SIDL = 0x00;  
    RXF0EIDH = 0x00;
    RXF0EIDL = 0x00;
    RXF0SIDH = 0x00;
    RXF0SIDL = 0x00;
    RXF1EIDH = 0x00;
    RXF1EIDL = 0x00;
    RXF1SIDH = 0x00;
    RXF1SIDL = 0x00;


    BRGCON1 = 0x01;
    BRGCON2 = 0x98;
    BRGCON3 = 0x01;

    ECANCONbits.MDSEL = 0b00;
    CANCONbits.REQOP = 0b011;//request normal mode
    while (CANSTATbits.OPMODE != 0b011); // wait until ECAN is in Normal mode
}

void main(void) {

    PIE5bits.RXB0IE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    TRISCbits.TRISC0 = 0;//LED2
    TRISCbits.TRISC1 = 0;//LED1
    TRISCbits.TRISC2 = 1;//STBY
    TRISBbits.TRISB6 = 0;//LED3
    TRISBbits.TRISB7 = 0;//LED5
    LATCbits.LATC0 = 1;
    LATCbits.LATC1 = 1;
    LATBbits.LATB6 = 1;
    LATBbits.LATB7 = 1;
    T0CONbits.TMR0ON = 0;//disable timer0
    T0CONbits.T08BIT = 0;//Timer0 is 16 bits wide
    T0CONbits.T0CS = 0;//internal clock
    T0CONbits.TMR0ON = 1;//timer0 ON
    INTCONbits.T0IE = 1;//timer 0 interrupt enable
    can_init();
    
    while(1)
    {
        can_rxbuff.fullid = can_rxbuff.idh<<3;
        can_rxbuff.fullid = can_rxbuff.fullid | can_rxbuff.idl>>5;
        
        if(canMessageFlag)
        {
            if( ( ledHighTimer + ledLowTimer ) == 0 )
            {
                LATCbits.LATC0 = 0;
                ledHighTimer = 500;
            }
            canMessageFlag = 0;
        }
        
        
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////second program option///////////////////////////////////////////////         
//         if( can_rxbuff.fullid == 0x12f )
//        {
//            if(     ( can_rxbuff.d3 &0xd0)       )
//                        {
//                            LATCbits.LATC1 = 0;
//                        }
//        }
//        if( can_rxbuff.fullid == 0x12f )
//        {
//            if(     ( can_rxbuff.d6 & 0x03 )       )
//                        {
//                            LATCbits.LATC0 = 0;
//                        }
//        }
        
        
        
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////first program option///////////////////////////////////////////////        
//        if( can_rxbuff.fullid == 0xfe )//LED1 IGN ON 0xFE A5 FC D0 DD FC FD BD 00
//        {
//            if(     ( can_rxbuff.d0 == 0xa5 ) &&
//                    ( can_rxbuff.d1 == 0xFC ) &&
//                    ( can_rxbuff.d2 == 0xD0 ) &&
//                    ( can_rxbuff.d3 == 0xDD ) &&
//                    ( can_rxbuff.d4 == 0xFC ) &&
//                    ( can_rxbuff.d5 == 0xFD ) &&
//                    ( can_rxbuff.d6 == 0xBD ) &&
//                    ( can_rxbuff.d7 == 0x00 )       )
//                        {
//                            LATCbits.LATC1 = 0;
//                        }
//        }
//        if( can_rxbuff.fullid == 0x130 )//LED2 Second IGN ON 0x130 45 F1 FC FF FF
//        {
//            if(     ( can_rxbuff.d0 == 0x45 ) &&
//                    ( can_rxbuff.d1 == 0xf1 ) &&
//                    ( can_rxbuff.d2 == 0xfc ) &&
//                    ( can_rxbuff.d3 == 0xff ) &&
//                    ( can_rxbuff.d4 == 0xff )     )
//                        {
//                            LATCbits.LATC0 = 0;
//                        }
//        }
//        if( can_rxbuff.fullid == 0x42 )//LED3 Door Open 0x42 00 00 08 00 03 6E 85 F4
//        {
//            if(     ( can_rxbuff.d0 == 0x00 ) &&
//                    ( can_rxbuff.d1 == 0x00 ) &&
//                    ( can_rxbuff.d2 == 0x08 ) &&
//                    ( can_rxbuff.d3 == 0x00 ) &&
//                    ( can_rxbuff.d4 == 0x03 ) &&
//                    ( can_rxbuff.d5 == 0x6e ) &&
//                    ( can_rxbuff.d6 == 0x85 ) &&
//                    ( can_rxbuff.d7 == 0xf4 )    )
//                        {
//                            LATBbits.LATB6 = 0;
//                        }
//        }
    }
    return;
}
void __interrupt() myISR(void)
{
// *** CAN ***
	if (PIR5bits.RXB0IF && PIE5bits.RXB0IE)		// CAN controller interrupt
		{
		PIR5bits.RXB0IF = 0;

		can_rxbuff.d0	= RXB0D0;		// read data
		can_rxbuff.d1	= RXB0D1;		// read data
		can_rxbuff.d2	= RXB0D2;		// read data
		can_rxbuff.d3	= RXB0D3;		// read data
        can_rxbuff.d4	= RXB0D4;		// read data
        can_rxbuff.d5	= RXB0D5;		// read data
        can_rxbuff.d6	= RXB0D6;		// read data
        can_rxbuff.d7	= RXB0D7;		// read data
		can_rxbuff.dl	= RXB0DLC;
        can_rxbuff.idh  = RXB0SIDH;
        can_rxbuff.idl  = (RXB0SIDL&0xe0);
        canMessageFlag = 1;
        
        RXB0CONbits.RXFUL = 0;
		}
    if(INTCONbits.T0IE && INTCONbits.TMR0IF)
    {
        ledtimer++;
        if(ledtimer > 500)
        {
            LATBbits.LATB7 ^= 1;
            
            
            ledtimer = 0;
        }
        if(ledHighTimer > 0)
        {
            ledHighTimer--;
            if(ledHighTimer == 0)
            {
                ledLowTimer = 500;
                LATCbits.LATC0 = 1;
            }
        }
        if(ledLowTimer > 0)
        {
            ledLowTimer--;
            
        }
        TMR0H = 0xf8;//preload timer so the timer period is about 1ms roughly
        TMR0L = 0x2F;
        INTCONbits.TMR0IF = 0;
        
    }
}