/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *   
 *	El numero 15 (0000001111) se representaria encendiendo los 
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos 
 *	los led anteriores y encenderse uno de los leds VERDES. 
 *	Notese, en el ejemplo los 0 representan los led apagados 
 *	y los 1 los led encendidos). 
 *   
 *	-------------------------------------------------------
 *   
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga. 
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los 
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los 
 *	leds encendidos sea perceptible para el ojo humano y 
 *	documentar cada función creada en el código. 
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/

//##### Una Ayudita #####
//--- Defines ---//
#define ZERO_TO_HERO 0          //? To start the counter from 0
#define BUTTON_START 2
#define BUTTON_RESET 3
#define FIRST_LED 4             //? First pin of the leds
#define LAST_LED 13             //? Last pin of the leds
#define BASE_MILLI_SECONDS 1000 //? Secods expresed in miliseconds
#define MAX_SECONDS 1023        //! Max amount of secods to show
#define TAM 10
//--- End Defines ---//

void Setear_Leds(int pinFirstLed, int pinLastLed)
{
  for (int i = pinFirstLed; i <= pinLastLed; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void setup(){
  Setear_Leds(FIRST_LED, LAST_LED);

  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_RESET, INPUT);

  Serial.begin(9600);
}

//VARIABLES

unsigned long millisAntes = 0;
int segundos = 0;
int arrayEnteros[TAM];

int botonStartAntes = 0;
int botonResetAntes = 0;

int presionoStart = 0;
int presionoReset = 0;

//FUNCIONES

void ApagarLeds(int pinFirstLed, int pinLastLed)
{
  for(int i = pinFirstLed; i <= pinLastLed; i++)
  {
    digitalWrite(i, LOW);
  }
}

void DecimalABinario(int tamanio, int numero, int array[])
{
  short numeroBin = 0;
  int ultimoIndice = tamanio - 1;

  ResetearBinario(tamanio, array);

  // Arma cadena binaria en el array enteros.
  while (numero > 0) {
    numeroBin = numero % 2;
    numero /= 2;
    array[ultimoIndice] = numeroBin;
    ultimoIndice--;
  }
}

void ResetearBinario(int tamanio, int array[])
{
  for (int i = 0; i < tamanio; i++)
  {
    array[i] = 0;
  }
}

void PrenderLedsBinario(int tamanio, int array[])
{
  /*Relaciona el indice de cada bit en el array con el pinLed 
  correspondiente, encendiendo o no el led si guarda un 1 o 0*/ 
  for (int i = 0; i < tamanio; i++)
  {
    digitalWrite(LAST_LED - i, array[i]);
  }
}

//Imprime el arrayEnteros/Binario actual.
void ImprimirCadenaBinaria(int tamanio, int array[])
{
  Serial.print("Binario: [");

  for (int i = 0; i < tamanio; i++)
  {
    Serial.print(array[i]);
  }

  Serial.println("]");
}

void presionar_start(int pinBotonStart)
{
  unsigned long millisAhora = millis();

  short botonStartAhora = digitalRead(pinBotonStart);

  if (botonStartAhora == HIGH && botonStartAntes == LOW) {
    presionoStart = !presionoStart;
  }

  /*Los segundos funcionan como un contador , muestra su binario
    correspondiente y prende los leds correctos bit a bit*/
  if (presionoStart && millisAhora - millisAntes >= BASE_MILLI_SECONDS)
  {
    segundos++;

    secuenciaCronometro();

    millisAntes = millisAhora;
  }

  botonStartAntes = botonStartAhora;
}

void presionar_reset(int pinBotonReset)
{
  short botonResetAhora = digitalRead(pinBotonReset);

  if (botonResetAhora == HIGH && botonResetAntes == LOW) {
    segundos = 0;
  }

  botonResetAntes = botonResetAhora;
}

void secuenciaCronometro()
{
  /*Los segundos llegan a 1024 para que las ultimas
  luces no se apaguen bruscamente pero este no se escribe
  en consola*/
  if(segundos <= MAX_SECONDS) {
    DecimalABinario(TAM, segundos, arrayEnteros);
    PrenderLedsBinario(TAM, arrayEnteros);

    Serial.print("Segundos: ");
    Serial.print(segundos);
    Serial.print(" | ");
    ImprimirCadenaBinaria(TAM, arrayEnteros);
  }else{
    ApagarLeds(FIRST_LED, LAST_LED);
    presionoStart = 0;
    segundos = 0;
  }
}


/*********************LOOP*******************/
void loop()
{
  presionar_start(BUTTON_START);
  presionar_reset(BUTTON_RESET);

  delay(50);
}
