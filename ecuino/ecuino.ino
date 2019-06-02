//Suport by Pablo Hernandez | @scraphero on github

int tiempo_real;

// PINS                       | Actually only Tachometer and Battery functions are using pins
  // BATTERY
    const int PIN_AMP_METER = A0;       // AMP METER
    const int PIN_VOLT_METER = A7;      // VOLT METER
  // TACHOMETER
    int short TACHOMETER_PIN     = 3 ;  // Pin for tachometer
    int short TACHOMETER_GND_PIN = 4 ;  // Pin for tachometer gnd [NOT NECESSARY BUT FOR LOCATION CONVENIENCE]

// DEMO VALUES                | For test and demonstrations values generator
  // Variables
    int demo_50_50 = 0 ;   // -50 to 50
    int demo_0_50 = 0 ;    // 0 to 50
    int demo_0_100 = 0 ;   // 0 to 100
    int demo_100_0 = 100 ; // 100 to 0
  // Function
    void demo_values()
    {
      demo_50_50 = demo_50_50 + 1 ;                 //value increments by 1
      if ( demo_50_50 > 50 ){ demo_50_50 = -50 ; }  //values from -50 to +50 again and again

      demo_0_50 = demo_0_50 + 1 ;                   //value increments by 1
      if ( demo_0_50 > 50 ){ demo_0_50 = 0 ; }      //value from 0 to 50 again and again

      demo_0_100 = demo_0_100 + 1 ;                 //value increments by 1
      if ( demo_0_100 > 100 ) {demo_0_100 = 0 ; }   //value from 0 to 100 again and again

      demo_100_0 = demo_100_0 - 1 ;                 //value decriments by 1
      if ( demo_100_0 < 0 ) {demo_100_0 = 100 ; }   //when 0 starts by 100 again
    }

// BATTERY                    | All stuff related with the battery, functions, const and variables etc.
  // Variables y Constantes
    // Setup
      const int Ah = 45 ; //        | Manually set the battery capacity
      //
    // Time
      const int TIEMPO_DE_LECTURA = 100;       //sera el tiempo en milisegundos 
      const int DELAY_MEDICION = 1;            //En este caso el valor es 1 porque queremos que se realice una medicion cada segundo
      int tiempo_medicion = 0;                 //Esta variable se usara para tomar los valores de medida cada cierto intervalo de tiempo definido en la siguiente constante
    // Voltmeter
      const float CORRECT_VOLT_BATT = 49.8;    //La bateria proporciona alrededor de 50V a la que fisicamente se le ha aplicado un divisor de tension de 10:1 asique para 50V de bateria medimos 5
      //
    // Ampmeter [hall]
      const float CORRECT_VOLT_HALL = -2.50;   //Para restar el voltage que registra el sensor hall cuando la corriente es = 0
      const float SENSIBILIDAD = 65;           //sensibilidad en Voltios/Amperio para sensor de 50A
      float corriente_hall = 0 ;
    // Battery level
      int total_battery = 0 ;
      float bateriaRestante = 162000;          //valor asignado como capacidad total inicialmente, resultado de multiplicar la capacidad de la bateria "45Ah" x 3600s puesto que restaremos el valor medio de corriente medido cada segundo
      int porcentaje_bateria = 0;              //Almacenara el porcentaje de bateria que se calculara a partir de bateriaRestante posteriormente
    // Autonomy
      int long autonomia_segundos_totales = 0; //Medira la autonomia instantanea expresada en segundos
      int short autonomia_segundos = 0;        //Autonomia en segundos restantes 
      int long autonomia_minutos_totales = 0; 
      int short autonomia_minutos = 0;
      int long autonomia_horas_totales = 0;

    float Watt = 0 ;
    float voltajeBatt = 0 ;
    float voltajeSensor = 0 ;
    int get_battery_values_loop = 0 ;
    float pile_current = 0 ;
    float average_current = 0 ;
  // Functions
    // Battery Setup
      void battery_setup()
      {
        total_battery = Ah * 3600 ;
        porcentaje_bateria = total_battery ;
      }
    // Get Values
      void get_battery_values()
      {
        voltajeSensor= analogRead(PIN_AMP_METER)*(5.0 / 1023.0);                //lectura del sensor hall, medida transformada en valores de 0 a 5 para conocer el valor de tension real del output del sensor HALL
        corriente_hall = ( voltajeSensor + CORRECT_VOLT_HALL ) * SENSIBILIDAD;  //Ecuación  para obtener la corriente a partir del valor anterior, ajuste de offset y sensibilidad, convertir la señal de voltaje output del sensor HALL en el valor de corriente real
        voltajeBatt= analogRead(PIN_VOLT_METER)*(CORRECT_VOLT_BATT / 1023.0);   //lectura de volaje 
        Watt = voltajeBatt * corriente_hall ;                                   //Potencia = V * I
        porcentaje_bateria = (bateriaRestante / 162000)*100;                    //Sencilla operacion para calcular en forma de porcentaje la bateria restante a partir de esas unidades sin magnitud que nos hemos inventado

        autonomia_segundos_totales = abs( bateriaRestante / corriente_hall ) ;  //autonomia_segundos_totales sera tiempo restante de bateria en segundos, sera un valor instantaneo
        autonomia_segundos = autonomia_segundos_totales % 60;                   //autonomia en segundos como dato complementario de horas y minutos es el resultado de calcular el resto de la division de los segundos totales entre 60
        autonomia_minutos_totales = autonomia_segundos_totales / 60;            //minutos restantes totales es el valor resultante de dividir los segundos totales entre 60
        autonomia_minutos = autonomia_minutos_totales % 60;                     //autonomia en minutos como dato complementario de horas y segundos es el resultado de calcular el resto de la division de los minutos totales entre 60
        autonomia_horas_totales = autonomia_minutos_totales / 60;               //horas de autonomia es el resultado de dividir los minutos totales entre 60

        if ( abs(autonomia_horas_totales) > 999 )  // En caso de que el tiempo de autonomia sea excesivo le ponemos este limite ya que desfiguraria los datos en la pantalla nextion
        {
          autonomia_horas_totales = 999 ;          // Valor maximo para la autonomia en horas
          autonomia_minutos = 59 ;                 // minutos
          autonomia_segundos = 59 ;                // y segundos
        }

        pile_current = pile_current + corriente_hall ;              // pile current va acumulando la suma de los valores de corriente de cada medicion para despues realizar la media cada segudo

        get_battery_values_loop = get_battery_values_loop + 1 ;     // cuenta el numero de vueltas que se da a esta funcion, sera reseteado desde la funcion average_battery_current 

        if ( tiempo_medicion <= tiempo_real )                       // Al final de la funcion get_battery_values le sumamos a tiempo_medicion el tiempo en segundos DELAY_MEDICION asi no se volvera a ejecutar la funcion hasta que tiempo_real vuelva a igualar tiempo_medicion
        {
          average_battery_current();
          tiempo_medicion = tiempo_medicion + DELAY_MEDICION ;      // Como hemos comentado antes esta linea le añade el tiempo que queremos que retarde entre lecturas
        }
      }
    // Current Average
      void average_battery_current()
      {
        average_current = pile_current / get_battery_values_loop ;  // Calcula la corriente media dividiendo la suma de valores de corriente acumulado entre el numero de veces que se ha tomado esa medicion
        bateriaRestante = bateriaRestante - average_current;        // Cada vez que se ejecute esta linea se restara el valor medido de corriente a las unidades totales de bateria

        pile_current = 0 ;                                          // Resetea la variable donde se acumulan los valores de corriente
        get_battery_values_loop = 0 ;                               // Resetea el numero de veces que se ha medido la corriente ya que se empieza de cero cada segundo
      }

// TACHOMETER                 | All stuff related with tachometer, functions, const and variables etc.
  // Variables & Constants
    const int short PULSE_LIMIT        = 2 ;       // Define how many pulses are needed until calcs
    const int short PULSES_X_SPIN      = 1 ;       // Define how much pulses we get per wheel spin
    const float     WHEEL_DISTANCE     = 0.0015  ; // Define the wheel spin straight travel

    int short tachometer_count = 0 ; 
    int       new_time = 0 ;
    int       last_time = 0 ;
    int       measuring_time = 0 ;
    float     pulse_x_second = 0 ;
    int       rpm = 0 ;
    int       kmh = 0 ;
  // Functions
    // Pulse count increment | called from interrupt function
      void tachometer_increment()
      {   
        tachometer_count = tachometer_count + 1 ;
      }   
    // Calculations          | On loop
      void tachometer_cacls()
      {
        if( tachometer_count >= PULSE_LIMIT )
        {
          new_time       = millis();
          measuring_time = new_time - last_time ;
          pulse_x_second = float(tachometer_count * 1000) / measuring_time ;   // It was really meant to work as the equation below but this one gets same result and.. works!
          //pulse_x_second = tachometer_count / float(measuring_time / 1000) ;
          rpm            = ( pulse_x_second / PULSES_X_SPIN ) * 60 ;
          kmh            = rpm * WHEEL_DISTANCE * 60 ;
                
          tachometer_count = 0 ;
          last_time = new_time;
        }
      }
    // Setup                 | On Setup
      void tachometer_setup ()
      {
        pinMode        ( TACHOMETER_GND_PIN, OUTPUT );
        digitalWrite   ( TACHOMETER_GND_PIN, LOW );
        digitalWrite   ( TACHOMETER_PIN    , HIGH );
        //pinMode        ( TACHOMETER_PIN    , INPUT );
        attachInterrupt( digitalPinToInterrupt(TACHOMETER_PIN), tachometer_increment, RISING);
      }

// NEXTION                    | All stuff related with Nextion display, functions, const and variables etc.
  // Variables
    int corriente_nextion = 0;
    int speed_gauge = 0 ;      // map is needed as 50kmh equals 270 degree for de nextion gauge
  // Functions
    // Speed Gauge
      void nextion_speed_gauge ()   // z0.val
      {
        speed_gauge = map( kmh, 0, 50, 0, 270); //map is needed as 50kmh equals 270 degree for de nextion gauge
        Serial.print( "z0.val=" );
        Serial.print( speed_gauge );
        end_send_nextion();
      }
    // Speed Numeric
      void nextion_speed_numeric ()
      {
        Serial.print( "t4.txt=" );
        Serial.print("\"");
        Serial.print( kmh ) ;
        Serial.print("\"");
        end_send_nextion();
      }
    // Battery Current
      void nextion_battery_current ()
      {
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
      }
    // Battery Level          | Sends 
      void nextion_battery_level()
      {
        /*if ( porcentaje_bateria > 100 ){ porcentaje_bateria = 0 ; }
        if ( porcentaje_bateria < 10 ){
          Serial.print( "j0.bco=" );
          Serial.print( 63488 );
          end_send_nextion();
        }
        else {
          Serial.print( "j0.bco=" );
          Serial.print( 48631 );
          end_send_nextion();
        }*/
        Serial.print( "j0.val=" );
        Serial.print( porcentaje_bateria );
        end_send_nextion();

        Serial.print( "t3.txt=" );
        Serial.print("\"");
        Serial.print( abs( porcentaje_bateria ) ) ;
        Serial.print( "%" );
        Serial.print("\"");
        end_send_nextion();
      }
    // Autonomy h/m/s         | Sends autonomy values 
      void nextion_autonomy_hms ()
      {
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
      }
    // End communication      | Finishes the communication after any value sending
      void end_send_nextion()
      {
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
      }

// STRING PRINT [Not Used]    | This block functions are for sending the values in a serial string but for this project is not been used at the end
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

// SETUP                      | Arduino SETUP
  void setup()                      //SETUP 
  {
    Serial.begin(9600);             // Se activa comunicacion serie
    tachometer_setup();             // Activates pins, sets the interruption
    battery_setup();
  }

// LOOP                       | Arduino LOOP
  void loop()                         //LOOP
  {
    demo_values();                    // Demo Values generator
    
    get_battery_values();             // Battery

    //Serial.println(tachometer_increment);


    tachometer_cacls();               // Tachometer

    
    //kmh = demo_0_50 ;                 // Nextion | DEBUG
    //corriente_hall = demo_50_50 ;     // Nextion | DEBUG
    //porcentaje_bateria = demo_100_0 ; // Nextion | DEBUG


    nextion_speed_gauge();            // Nextion | z0.val speed gauge
    nextion_speed_numeric();          // Nextion | t4.txt speed numeric
    nextion_battery_current();        // Nextion | j1 & j2 bottom progress bars
    nextion_battery_level();          // Nextion | battery level
    nextion_autonomy_hms ();          // Nextion | autonomy hours/minutes/seconds


    //nextion_prints();

    tiempo_real = (millis()/1000);  // tiempo_real almacenara el tiempo en segundos para ello hemos dividido millis entre 1000
  }
