//v0.1 Esta version añade la funcion de calcular la autonomia_segundos_totales en tiempo restante de bateria

int tiempo_real;

/*PINES*/
const int PIN_AMP_METER = A0;            //PIN AMPERIMETRO
const int PIN_VOLT_METER = A7;           //PIN VOLTIMETRO

/*AMPERIMETRO SENSOR HALL*/
const float CORRECT_VOLT_HALL = -2.50;   //Para restar el voltage que registra el sensor hall cuando la corriente es = 0
const float SENSIBILIDAD = 65;           //sensibilidad en Voltios/Amperio para sensor de 5A

/*VOLTIMETRO*/
const float CORRECT_VOLT_BATT = 49.8;    //La bateria proporciona alrededor de 50V a la que fisicamente se le ha aplicado un divisor de tension de 10:1 asique para 50V de bateria le llegan 5

/*BATERIA*/
const int TIEMPO_DE_LECTURA = 100;       //sera el tiempo en milisegundos 
float bateriaRestante = 162000;          //valor asignado como capacidad total inicialmente, resultado de multiplicar la capacidad de la bateria "45Ah" x 3600s puesto que restaremos el valor medio de corriente medido cada segundo
int tiempo_medicion = 0;                 //Esta variable se usara para tomar los valores de medida cada cierto intervalo de tiempo definido en la siguiente constante
const int DELAY_MEDICION = 1;            //En este caso el valor es 1 porque queremos que se realice una medicion cada segundo
int porcentaje_bateria = 0;              //Almacenara el porcentaje de bateria que se calculara a partir de bateriaRestante posteriormente
int long autonomia_segundos_totales = 0; //Medira la autonomia instantanea expresada en segundos
int short autonomia_segundos = 0;        //Autonomia en segundos restantes 
int long autonomia_minutos_totales = 0;  
int short autonomia_minutos = 0;
int long autonomia_horas_totales = 0;

int valor_prueba = 0 ;

void end_send_nextion()
{
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void medicion_periodica()
{

    float voltajeSensor= analogRead(PIN_AMP_METER)*(5.0 / 1023.0);                //lectura del sensor hall, medida transformada en valores de 0 a 5 para conocer el valor de tension real del output del sensor HALL
    float corriente_hall = ( voltajeSensor + CORRECT_VOLT_HALL ) * SENSIBILIDAD;  //Ecuación  para obtener la corriente a partir del valor anterior, ajuste de offset y sensibilidad, convertir la señal de voltaje output del sensor HALL en el valor de corriente real
    float voltajeBatt= analogRead(PIN_VOLT_METER)*(CORRECT_VOLT_BATT / 1023.0);   //lectura de volaje 
    float Watt = voltajeBatt * corriente_hall ;                                   //Potencia = V * I
    bateriaRestante = bateriaRestante - corriente_hall;                           //Cada vez que se ejecute esta linea se restara el valor medido de corriente a las unidades totales de bateria
    porcentaje_bateria = (bateriaRestante / 162000)*100;                          //Sencilla operacion para calcular en forma de porcentaje la bateria restante a partir de esas unidades sin magnitud que nos hemos inventado
    
    autonomia_segundos_totales = abs( bateriaRestante / corriente_hall ) ;        //autonomia_segundos_totales sera tiempo restante de bateria en segundos, sera un valor instantaneo
    autonomia_segundos = autonomia_segundos_totales % 60;                         //autonomia en segundos como dato complementario de horas y minutos es el resultado de calcular el resto de la division de los segundos totales entre 60
    autonomia_minutos_totales = autonomia_segundos_totales / 60;                  //minutos restantes totales es el valor resultante de dividir los segundos totales entre 60
    autonomia_minutos = autonomia_minutos_totales % 60;                           //autonomia en minutos como dato complementario de horas y segundos es el resultado de calcular el resto de la division de los minutos totales entre 60
    autonomia_horas_totales = autonomia_minutos_totales / 60;                     //horas de autonomia es el resultado de dividir los minutos totales entre 60

    valor_prueba = valor_prueba + 1 ;
    if ( valor_prueba > 100 ){ valor_prueba = 0 ; }


  //{ Prints DEBUG

    //Serial.print("tiempo real:");
    //Serial.print(tiempo_real);
    //Serial.print("    tiempo medicion");
    //Serial.print(tiempo_medicion);
    //Serial.print("    bateria restante:");
    //Serial.print(bateriaRestante);
    //Serial.print("    ");

    //Serial.print("Au: ");
    //Serial.print(autonomia_segundos_totales);        //Serial print autonomia en segundos totales
    //Serial.print("s left    ");
    
    //Serial.print("    Voltaje sensor: ");
    //Serial.print(voltajeSensor);                     //Voltaje del sensor Hall
  //}

  //{ Print para pantalla NEXTION
    Serial.print( "j0.val=" );
    Serial.print( valor_prueba );
    end_send_nextion();
  //}


  /*//{ Print STRING PORCENTAJE BATERIA
    if (porcentaje_bateria < 100) { Serial.print(0); }  //para mostrar 1 cero a la izquierda en caso de que el valor sea de 2 cifras esto es necesario para mantener el string estable
    if (porcentaje_bateria < 10) { Serial.print(00); }    //para mostrar 2 ceros a la izquierda en caso de que el valor sea de 1 cifras
    Serial.print(porcentaje_bateria);                     //Serial print porcentaje bateria
    Serial.print("% ");                                //Mostrar el simbolo de porcentaje tras el valor
  //}*/

  /*//{ Print STRING AUTONOMIA
    //autonomia_horas_totales = 0 ; //DEBUG
    if ( autonomia_horas_totales < 100 ) { Serial.print(0); }   //otra vez ceros a la izquierda
    if ( autonomia_horas_totales < 10 ) { Serial.print(00); }   //
    Serial.print( autonomia_horas_totales ) ;  //Horas restantes de autonomia   
    Serial.print("h");
    
    if ( autonomia_minutos < 10 ) { Serial.print(0); }   //para mostrar 1 ceros a la izquierda en caso de que el valor sea de 1 cifras
    Serial.print ( abs ( autonomia_minutos ) );                   //Minutos restantes de autonomia
    Serial.print("m");
    
    if (autonomia_segundos < 10) { Serial.print(0); }  //para mostrar 1 ceros a la izquierda en caso de que el valor sea de 1 cifras
    Serial.print(autonomia_segundos);                  //Segundos restantes de autonomia
    Serial.print("s");
  //}*/

  /*//{ Print STRING POTENCIA
    //Watt = 11111.1;                                                    //DEBUG
    Serial.print(" W:");                                             //imprime el indicador de watios para hacer mas comprensible el string
    if ( Watt >= 0 ) { Serial.print( "+" ); }                        //imprime signo positivo
    if ( Watt <  0 ) { Serial.print( "-" ); }                        //o negativo segun corresponda
    Watt = abs(Watt);                                                //ahora Watt se convierte en un valor absoluto pues no es deseable que se imprima mas vezes el signo
    if ( Watt >= 9999.99 ) { Watt = 9999.99 ; }                      //impide que se supere este valor pues que aumente el numero de cifras estropearia el string
    if ( (Watt < 1000 ) && ( Watt >= 100 ) ) { Serial.print("0"); }  //para mostrar 1 ceros a la izquierda en caso de que el valor sea de 1 cifras
    if ( (Watt < 100) && (Watt >= 10) ) { Serial.print("00"); }      //para mostrar 2 ceros a la izquierda en caso de que el valor sea de 1 cifras
    if ( Watt < 10 ) { Serial.print("000"); }                        //para mostrar 3 ceros a la izquierda en caso de que el valor sea de 1 cifras
    Serial.print( Watt );                                            //Print Potencia
  //}*/

  /*//{ Serial Print TENSION BATERIA
    Serial.print(" V:");
    Serial.print(voltajeBatt);                         //Print Tension
  //}*/

  /*//{ Serial Print CORRIENTE BATERIA
    if ( corriente_hall > 0 ) { Serial.print( "+" ) ; }
    Serial.print(" A:");
    Serial.println ( corriente_hall, 2 ) ;    //Print Corriente
  //}*/

    tiempo_medicion = tiempo_medicion + DELAY_MEDICION ;    //Como hemos comentado antes esta linea le añade el tiempo que queremos que retarde entre lecturas
}


void setup() {                            //SETUP
  Serial.begin(9600);                   //Activar comunicacion serie
}

void loop()                               //LOOP
{
  tiempo_real = (millis()/1000);          //tiempo_real almacenara el tiempo en segundos para ello hemos dividido millis entre 1000

  if ( tiempo_medicion <= tiempo_real )   //Al final de la funcion medicion_periodica le sumamos a tiempo_medicion el tiempo en segundos DELAY_MEDICION asi no se volvera a ejecutar la funcion hasta que tiempo_real vuelva a igualar tiempo_medicion
  {
    medicion_periodica();                 //Llamada a la funcion que engloba las mediciones sobre la bateria y sus calculos V, corriente_hall, W, bateria restante etc.
  }
}
