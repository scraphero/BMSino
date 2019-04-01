//v0.1 Esta version añade la funcion de calcular la autonomia_segundos_totales en tiempo restante de bateria

int tiempo_real;

// PINES
const int PIN_AMP_METER = A0;            //PIN AMPERIMETRO
const int PIN_VOLT_METER = A7;           //PIN VOLTIMETRO

// AMPERIMETRO SENSOR HALL
const float CORRECT_VOLT_HALL = -2.50;   //Para restar el voltage que registra el sensor hall cuando la corriente es = 0
const float SENSIBILIDAD = 65;           //sensibilidad en Voltios/Amperio para sensor de 50A
float corriente_hall = 0 ;

// VOLTIMETRO
const float CORRECT_VOLT_BATT = 49.8;    //La bateria proporciona alrededor de 50V a la que fisicamente se le ha aplicado un divisor de tension de 10:1 asique para 50V de bateria le llegan 5

// BATERIA
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

// Battery average
int get_battery_values_loop = 0 ;
float pile_current = 0 ;
float average_current = 0 ;

// NEXTION
int corriente_nextion = 0;

// get_battery_values
float Watt = 0 ;
float voltajeBatt = 0 ;
float voltajeSensor = 0 ;

// DEMO_VALUES
int demo_50_50 = 0 ;
int demo_0_50 = 0 ;

//{  FUNCTIONS
  void get_battery_values()
  {
    voltajeSensor= analogRead(PIN_AMP_METER)*(5.0 / 1023.0);                //lectura del sensor hall, medida transformada en valores de 0 a 5 para conocer el valor de tension real del output del sensor HALL
    corriente_hall = ( voltajeSensor + CORRECT_VOLT_HALL ) * SENSIBILIDAD;  //Ecuación  para obtener la corriente a partir del valor anterior, ajuste de offset y sensibilidad, convertir la señal de voltaje output del sensor HALL en el valor de corriente real
    voltajeBatt= analogRead(PIN_VOLT_METER)*(CORRECT_VOLT_BATT / 1023.0);   //lectura de volaje 
    Watt = voltajeBatt * corriente_hall ;                                   //Potencia = V * I
    porcentaje_bateria = (bateriaRestante / 162000)*100;                    //Sencilla operacion para calcular en forma de porcentaje la bateria restante a partir de esas unidades sin magnitud que nos hemos inventado


    //corriente_hall = 45 ;


    autonomia_segundos_totales = abs( bateriaRestante / corriente_hall ) ;  //autonomia_segundos_totales sera tiempo restante de bateria en segundos, sera un valor instantaneo
    autonomia_segundos = autonomia_segundos_totales % 60;                   //autonomia en segundos como dato complementario de horas y minutos es el resultado de calcular el resto de la division de los segundos totales entre 60
    autonomia_minutos_totales = autonomia_segundos_totales / 60;            //minutos restantes totales es el valor resultante de dividir los segundos totales entre 60
    autonomia_minutos = autonomia_minutos_totales % 60;                     //autonomia en minutos como dato complementario de horas y segundos es el resultado de calcular el resto de la division de los minutos totales entre 60
    autonomia_horas_totales = autonomia_minutos_totales / 60;               //horas de autonomia es el resultado de dividir los minutos totales entre 60

    if ( abs(autonomia_horas_totales) > 999 )                        // En caso de que el tiempo de autonomia sea excesivo le ponemos este limite ya desfiguraria los datos en la pantalla nextion
    {
      autonomia_horas_totales = 999 ;
      autonomia_minutos = 59 ;
      autonomia_segundos = 59 ;
    }

    pile_current = pile_current + corriente_hall ;    

    get_battery_values_loop = get_battery_values_loop + 1 ;     //cuenta el numero de vueltas que se da a esta funcion, sera reseteado desde la funcion average_battery_values 

    if ( tiempo_medicion <= tiempo_real )                       //Al final de la funcion get_battery_values le sumamos a tiempo_medicion el tiempo en segundos DELAY_MEDICION asi no se volvera a ejecutar la funcion hasta que tiempo_real vuelva a igualar tiempo_medicion
    {
      average_battery_values();
      tiempo_medicion = tiempo_medicion + DELAY_MEDICION ;      //Como hemos comentado antes esta linea le añade el tiempo que queremos que retarde entre lecturas
    }
  }

  void average_battery_values()
  {
    average_current = pile_current / get_battery_values_loop ;  //Calcula la corriente media 
    bateriaRestante = bateriaRestante - average_current;        //Cada vez que se ejecute esta linea se restara el valor medido de corriente a las unidades totales de bateria

    pile_current = 0 ;                                          //Resetea la variable donde se acumulan los valores de corriente
    get_battery_values_loop = 0 ;
  }

  void nextion_prints()
  {
    //{ CORRIENTE (consumo y recarga) | Listo
      if (corriente_hall <= 0)
      {
        Serial.print( "j2.val=" );
        Serial.print( 0 );
        end_send_nextion();

        corriente_nextion = abs(corriente_hall) * 2 ;
        corriente_nextion = map(corriente_nextion, 0, 100, 100, 0);
        Serial.print( "j1.val=" );
        Serial.print( corriente_nextion );
        end_send_nextion();
      }

      if (corriente_hall >= 0)
      {
        Serial.print( "j1.val=" );
        Serial.print( 100 );
        end_send_nextion();

        corriente_nextion = corriente_hall * 2 ;
        Serial.print( "j2.val=" );
        Serial.print( corriente_nextion );
        end_send_nextion();
      }
    //}

    //{ PORCENTAJE BATERIA | Listo
      if ( porcentaje_bateria > 100 ){ porcentaje_bateria = 0 ; }
      if ( porcentaje_bateria < 10 ){
        Serial.print( "j0.bco=" );
        Serial.print( 63488 );
        end_send_nextion();
      }
      else {
        Serial.print( "j0.bco=" );
        Serial.print( 48631 );
        end_send_nextion();
      }
      Serial.print( "j0.val=" );
      Serial.print( porcentaje_bateria );
      end_send_nextion();
    //}

    //{ AUTONOMIA h/m/s
      if (corriente_hall < 0){
        Serial.print( "t0.txt=" );
        Serial.print("\"");
        Serial.print( "-" ) ;  //Horas restantes de autonomia
        Serial.print( "h" );
        Serial.print("\"");
        end_send_nextion();

        Serial.print( "t1.txt=" );
        Serial.print("\"");
        Serial.print( "-" );        //Minutos restantes de autonomia
        Serial.print( "m" );
        Serial.print("\"");
        end_send_nextion();

        Serial.print( "t2.txt=" );
        Serial.print("\"");
        Serial.print( "-" );        //Segundos restantes de autonomia
        Serial.print( "s" );
        Serial.print("\"");
        end_send_nextion();
      }
      else{
        Serial.print( "t0.txt=" );
        Serial.print("\"");
        Serial.print( abs( autonomia_horas_totales ) ) ;  //Horas restantes de autonomia
        Serial.print( "h" );
        Serial.print("\"");
        end_send_nextion();

        Serial.print( "t1.txt=" );
        Serial.print("\"");
        Serial.print( abs( autonomia_minutos ) );        //Minutos restantes de autonomia
        Serial.print( "m" );
        Serial.print("\"");
        end_send_nextion();

        Serial.print( "t2.txt=" );
        Serial.print("\"");
        Serial.print( abs( autonomia_segundos ) );        //Segundos restantes de autonomia
        Serial.print( "s" );
        Serial.print("\"");
        end_send_nextion();
      }
    //}

    /*//{ TENSION BATERIA
      Serial.print(" V:");
      Serial.print(voltajeBatt);                         //Print Tension
    //}*/
  }

  void end_send_nextion()
  {
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }

  /*void string_print()
  {
    //{ CORRIENTE BATERIA
      if ( corriente_hall > 0 ) { Serial.print( "+" ) ; }
      Serial.print(" A:");
      Serial.print ( corriente_hall, 2 ) ;    //Print Corriente
    //}

    //{ Print STRING PORCENTAJE BATERIA
      if (porcentaje_bateria < 100) { Serial.print(0); }  //para mostrar 1 cero a la izquierda en caso de que el valor sea de 2 cifras esto es necesario para mantener el string estable
      if (porcentaje_bateria < 10) { Serial.print(00); }    //para mostrar 2 ceros a la izquierda en caso de que el valor sea de 1 cifras
      Serial.print(porcentaje_bateria);                     //Serial print porcentaje bateria
      Serial.print("% ");                                //Mostrar el simbolo de porcentaje tras el valor
    //}

    //{ Print STRING AUTONOMIA
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
    //}

    //{ Print STRING POTENCIA
      //Watt = 11111.1;                                                    //DEBUG
      Serial.print(" W:");                                             //imprime el indicador de watios para hacer mas comprensible el string
      if ( Watt >= 0 ) { Serial.print( "+" ); }                        //imprime signo positivo
      if ( Watt <  0 ) { Serial.print( "-" ); }                        //o negativo segun corresponda
      Watt = abs(Watt);                                                //ahora Watt se convierte en un valor absoluto pues no es deseable que se imprima mas vezes el signo
      if ( Watt >= 9999.99 ) { Watt = 9999.99 ; }                      //impide que se supere este valor pues que aumente el numero de cifras estropearia el string
      if ( (Watt < 1000 ) && ( Watt >= 100 ) ) { Serial.print("0"); }  //para mostrar 1 ceros a la izquierda en caso de que el valor sea de 1 cifras
      if ( (Watt < 100) && (Watt >= 10) ) { Serial.print("00"); }      //para mostrar 2 ceros a la izquierda en caso de que el valor sea de 1 cifras
      if ( Watt < 10 ) { Serial.print("000"); }                        //para mostrar 3 ceros a la izquierda en caso de que el valor sea de 1 cifras
      Serial.println( Watt );                                            //Print Potencia
    //}
  }*/
//}

void setup() 
{                                 //SETUP
  Serial.begin(9600);             //Se activa comunicacion serie
  pinMode(5,OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
}

void loop()                       //LOOP
{

  demo_values();
  get_battery_values();           //
  nextion_prints();

  tiempo_real = (millis()/1000);  //tiempo_real almacenara el tiempo en segundos para ello hemos dividido millis entre 1000
  
}

void demo_values()
{
  demo_50_50 = demo_50_50 + 1 ;
  if (demo_50_50 > 50){
    demo_50_50 = -50 ;
  }

  demo_0_50 = demo_0_50 + 1 ;
  if (demo_0_50 > 50){
    demo_0_50 = 0 ;
  }
}