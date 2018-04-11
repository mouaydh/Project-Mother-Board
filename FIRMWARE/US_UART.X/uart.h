#ifndef _UART_H_
#define _UART_H_

#define UxRx_length 8
#define UxTx_length 25

#define FLAG_Tx_MASK 0x0001 // indique la fin de la transmission de Tx_string
#define FLAG_Tx_MASKB 0xFFFE
#define FLAG_Tx_wait_MASK 0x0002// demande un attente avant la transmission d'un
#define FLAG_Tx_wait_MASKB 0xFFFD// nouveau char
struct Struct_Uart1{
	unsigned int Flag;
    char Tx_string[UxTx_length];
    unsigned char Tx_size;
    char Rx_string[UxRx_length];
};

extern void initUART1(unsigned long);
extern void startU1TX(void);
void  fctU1Tx_string(char *, char);
void ftcU1Tx_int(int );

#endif
