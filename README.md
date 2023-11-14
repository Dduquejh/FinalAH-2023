# **EXAMEN FINAL**

## **Explicación del proyecto:**
Realizar la medición de la gravedad creando una fotopuerta y usando una kl25z para realizar los calculos del tiempo y las demás operaciones.

## **Explicación del código**
```c++
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
```
En este fragmento se realiza el llamado a las librerias y la creación de objetos con los cuales se va a trabajar

### **Dentro del main**
```c++
cout << "Inicio del programa" << endl;
const float height = 11.8/100;
float time = 0, gravity = 0;
bool endFlag = false, startFlag = true;
led = 1;
```
Se crean las variables que se necesitan y se definen unas banderas y se apaga el led de la tarjeta que funciona como indicativo de cuando está midiento el tiempo

### **Dentro del ciclo while**
```c++
if(startFlag && inicial.read() < 0.3f){
   cout << "Se empieza a medir el tiempo" << endl; 
   t.start();
   endFlag = true;
   startFlag = false;
   led = 0;
}
```
En esta primera parte del condicional se evalua si está la compuerta de inicio habilitada para empezar a medir y se valida que la cantidad de luz que recibe el sensor sea inferior al dato usado. Si se cumple las condiciones se inicia el contador, se cambia el estado de la bandera de fin y de inicio, para habilitar la medición de la compuerta que da por terminado el contador y bloquear la medición de la compuerta de inicio. Además de que se enciende el led de la tarjeta como un indicativo de cuando se está tomando el tiempo

```c++
else if(endFlag && final.read() < 0.4f){
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
```
En la segunda parte del condicional, se evalua que la compuerta de fin esté habilitada para detener el temporizador y que la cantidad de luz que recibe el fotoresistor sea inferior al valor. Si estas condiciones se cumplen, se detiene el temporizador, y se hace la conversión de este tiempo de microsegundos a segundos. Una vez se hace esto, se calcula la gravedad usando la fórmula $g(x) = 2*h/t^2$, con $h$ como altura y $t$ como el tiempo. Luego de realizar este cálculo, se encarga de mostrar tanto en terminal como en un lcd este dato. Y para finalizar cambia el estado del led indicativo a apagado, se cambia el estado de las banderas para volver habilitar la compuerta de inicio y reinicia el temporizador
