//**************************************************************************************************************
/**
 * main.c
 * Por: Rodrigo Cruz y Lourdes Ruiz
 */
//**************************************************************************************************************
// Librerias
//**************************************************************************************************************
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"inc/tm4c123gh6pm.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"
#include"driverlib/timer.h"
#include"driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

#define XTAL 16000000

//**************************************************************************************************************
// Prototipos de Función
//**************************************************************************************************************
void InitUART(void);


//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************
//botones
uint8_t count_four = 0;
uint8_t count_one = 0;
uint8_t count_two = 0;
uint8_t count_three = 0;
uint8_t parking;
uint8_t parking_lot_1;
uint8_t parking_lot_2;
uint8_t parking_lot_3;
uint8_t parking_lot_4;
uint8_t pressed_1 = 1;
uint8_t pressed_2 = 1;
uint8_t pressed_3 = 1;
uint8_t pressed_4 = 1;

int main(void)
{
    // Configuración del oscilador externo, usando PLL y teniendo una frecuencia de 40MHz
    SysCtlClockSet(
            SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                    | SYSCTL_OSC_MAIN);



    // Se inicializa la comunicación UART
    InitUART();

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);

    //Se configuran las LEDs de parqueo (verdes)
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2);//P1
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_4);//P2
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7);//P3
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7);//P4

    //Se configuran las LEDs de parqueo (rojos)
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);

    //Se configuran los push-buttons como entradas
     GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);
     GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);
     GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);
     GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);

     //Weak pull-ups
     GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
     GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
     GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
     GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Se habilitan las interrupciones Globales
    IntMasterEnable();

    //**********************************************************************************************************
    // Loop Principal
    //**********************************************************************************************************
    while (1)
    {
        /*
        if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5) == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
            parking_lot_1 = 1;
            UARTCharPut(UART1_BASE, '5');

               }
               */

        //ROJO - OCUPADO
        if(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5) == 0 && pressed_1 == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
            parking_lot_1 = 1;
            UARTCharPut(UART1_BASE, '5');
            pressed_1 = 1;
        }


        //VERDE - HABILITADO
        if ((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5) != 0 && pressed_1 == 1)){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
            parking_lot_1 = 0;
            UARTCharPut(UART1_BASE, '1');
            pressed_1 = 0;

        }

        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) == 0 && pressed_2 == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
            parking_lot_2 = 1;
            UARTCharPut(UART1_BASE, '6');
            pressed_2 = 1;
               }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) != 0 && pressed_2 == 1)){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
            parking_lot_2 = 0;
            UARTCharPut(UART1_BASE, '2');
            pressed_2 = 0;

        }

        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) == 0 && pressed_3 == 0){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
            parking_lot_3 = 1;
            UARTCharPut(UART1_BASE, '7');
            pressed_3 = 1;
               }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) != 0 && pressed_3 == 1)){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);
            parking_lot_3 = 0;
            UARTCharPut(UART1_BASE, '3');
            pressed_3 = 0;

        }

        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) == 0 && pressed_4 == 0){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
            parking_lot_4 = 1;
            UARTCharPut(UART1_BASE, '8');
            pressed_4 = 1;
               }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) != 0 && pressed_4 == 1)){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
            parking_lot_4 = 0;
            UARTCharPut(UART1_BASE, '4');
            pressed_4 = 0;

        }
        parking = 4 - (parking_lot_1+parking_lot_2+parking_lot_3+parking_lot_4);


        //Configuracion del display
        if(parking == 0){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); //A
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3); //B
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4); //C
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5); //D
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6); //E
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7); //F
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0); //G


        }
        if(parking == 1){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0); //A
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3); //B
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4); //C
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0); //D
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0); //E
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0); //F
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0); //G


        }
        if(parking == 2){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); //A
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3); //B
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0); //C
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5); //D
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6); //E
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0); //F
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4); //G


        }
        if(parking == 3){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); //A
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3); //B
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4); //C
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5); //D
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0); //E
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0); //F
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4); //G


        }
        if(parking == 4){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0); //A
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3); //B
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4); //C
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0); //D
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0); //E
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7); //F
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4); //G


        }

    }
}

void InitUART(void)
{
    /*Enable the GPIO Port C*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    /*Enable the peripheral UART Module 1*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART1_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //Habilita las interrupciones
   // UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //Registra el handler de la interrupcion
    //UARTIntRegister(UART0_BASE, UART0IntHandler);

    UARTEnable(UART1_BASE);

    //IntEnable(INT_UART0);
}



