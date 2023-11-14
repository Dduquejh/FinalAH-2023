/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

// Importación de librerias
#include "mbed.h"
#include <iostream>
#include "TextLCD.h"
using namespace std;
UnbufferedSerial serial(USBTX, USBRX, 9600);
TextLCD lcd(D2, D3, D4, D5, D6, D7, TextLCD::LCD16x2); 
AnalogIn inicial(A0);
AnalogIn final(A1);
DigitalOut led(LED1);
Timer t;

int main()
{
    // Definir variables a usar
    cout << "Inicio del programa" << endl;
    const float height = 11.8/100;
    float time = 0, gravity = 0;
    bool endFlag = false, startFlag = true;
    led = 1;

    // Ciclo principal
    while(true){
        if(startFlag && inicial.read() < 0.3f){
            cout << "Se empieza a medir el tiempo" << endl; 
            t.start();
            endFlag = true;
            startFlag = false;
            led = 0;
        }else if(endFlag && final.read() < 0.4f){
            cout << "Se deja de medir el tiempo" << endl;
            t.stop();
            chrono::microseconds tiempoTranscurrido = t.elapsed_time();     // Devuelve el tiempo en microsegundos
            time = tiempoTranscurrido.count();
            time /= 1000000;    // Se divide entre un millon para pasar de microsegundos a segundos
            printf("Tiempo: %.4f \n", time);
            gravity = 2*height/(time*time);
            printf("La gravedad es: %.4f \n", gravity);
             lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("La gravedad es: ");
            lcd.locate(0, 1);
            lcd.printf("%f m/s^2", gravity);
            endFlag = false;
            startFlag = true;
            led = 1;
            t.reset();
        }
    }

}
