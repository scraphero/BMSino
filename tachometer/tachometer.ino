//Variables de tiempo
unsigned long tiempo_real = 0;             //tiempo real en milisegundos
float tiempo_medicion = 0;                 //tiempo que tarda en registrar el numero de pulsos que nosotros establecemos en LIMITE_PULSOS
float tiempo_medicion_sec = 0;             //el mismo valor pero expresado en segundos en vez de milisegundos
unsigned long tiempo_anterior = 0;         //en esta variable se almacenara el dato del tiempo en el que se termino la medicion anterior y por tanto comenzo la siguiente

//CONSTANTES y variables del contador
int short PIN_CONTADOR = 8;                //Establecer el pin input para los pulsos
int short LIMITE_PULSOS = 4;               //Establecer cada cuantos pulsos se haran los calculos, aumentar para medidas mas precisas, disminuir para datos mas dinamicos.
bool estado_contador_actual = 0;           //Almacenara el valor actual de input que puede ser 0 o 1
bool estado_contador_anterior = 0;         //Almacenara el estado anterior de la variable anterior
int short contador = 0;                    //Aqui ya se almacenara el dato de pulsos de manera decimal
float pulsos_x_segundo = 0;                //Se almacenara el dato de pulsos por segundo que se obtiene de la ecuacion en la linea 46

//CONSTANTES y variables de RPM
int short PULSOS_POR_VUELTA = 1;           //Introducir aqui el numero de dientes que tenga la corona para el sensor de pulsos
float rpm = 0;

//CONSTANTES y variables velocidad_kmh
float PERIMETRO_RUEDA = 1;                 //Introducir aqui el perimetro de la rueda en km
float velocidad_kmh = 0;                   //variable que almacenara la velocidad_kmh

void setup()
{
  Serial.begin(115200);                    //Inicia comunicacion Serial
  tiempo_real = millis();                  //definimos tiempo real en setup para que funcione bien la primera vez que se ejecute
  Serial.println("fin setup");             //Sabremos que ha terminado el setup
}

void loop()
{
  tiempo_anterior = tiempo_real;                               //Detenemos aqui el tiempo guardandolo en tiempo anterior
  while(contador < LIMITE_PULSOS)                              //Mientras el numero de pulsos sea inferior a LIMITE_PULSOS
  {
    //Serial.println("while");                                 //Prueba, borrar esta linea si todo funciona ok
    estado_contador_actual = digitalRead(PIN_CONTADOR);        //Leer estado del PIN CONTADOR y almacenarlo en la variable del estado actual
    if (estado_contador_actual != estado_contador_anterior)    //Si cambia el estado
    {
      //Serial.println("cambio de estado");                    //Prueba, borrar esta linea si todo funciona ok
      if (estado_contador_actual == 1)                         //A HIGH
      {
        contador = contador + 1;                               //sumar 1 a contador
        Serial.println(contador);                              //Prueba, borrar esta linea si todo funciona ok
      }
    }   
    estado_contador_anterior = estado_contador_actual;         //guardar en estado anterior el estado actual y volvemos a empezar el while si el valor de contador sigue siendo inferior al limite definido
  }

  tiempo_real = millis();                                      //actualizamos tiempo real
  tiempo_medicion = tiempo_real - tiempo_anterior;
  tiempo_medicion_sec = tiempo_medicion / 1000;
  pulsos_x_segundo = (contador-1) / tiempo_medicion_sec;       //ecuacion pulsos x segundo
  rpm = pulsos_x_segundo / PULSOS_POR_VUELTA * 60;             //ecuacion para conseguir las rpm
 	velocidad_kmh = rpm * PERIMETRO_RUEDA * 60;                      //velocidad_kmh en km/h

  //Serial.print("tm ");Serial.print(tiempo_medicion);Serial.print("   tr ");Serial.print(tiempo_real); Serial.print("   ta ");Serial.print(tiempo_anterior);
  //Serial.print("   cont ");Serial.print(contador-1);Serial.print("   tms ");Serial.print(tiempo_medicion_sec,3);
  Serial.print("   p/s "); Serial.println(pulsos_x_segundo,3); 
  contador = 0;
}
