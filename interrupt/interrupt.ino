//Este sketch se cargara en un arduino dedicado a la medicion de la velocidad del vehiculo
//esta velocidad sera enviada al arduino principal mediante comunicacion en paralelo usando 7 pines digitales.


// TACHOMETER PINS {
	int short TACHOMETER_PIN     = 3 ;    // Pin for tachometer
	int short TACHOMETER_GND_PIN = 4 ;    // Pin for tachometer gnd
// TACHOMETER CONSTANTS {
	int short PULSE_LIMIT        = 1 ;    // Define how many pulses are needed until calcs
	int short PULSES_X_SPIN      = 1 ;    // Define how much pulses we get per wheel spin
	float       WHEEL_DISTANCE   = 0.0015  ; // Define the wheel spin straight travel
// TACHOMETER VARIABLES {
	int short tachometer_count = 0 ; 
	int       new_time = 0 ;
	int       last_time = 0 ;
	int       measuring_time = 0 ;
	float     pulse_x_second = 0 ;
	int       rpm = 0 ;
	int       kmh = 0 ;
// TACHOMETER FUNCTIONS {
	void tachometer_increment()  // tachometer_count | measuring_time | pulse_x_second | rpm | kmh
	{		
		tachometer_count = tachometer_count + 1 ;
	}
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

// PARALLEL SPEED PINS {
 int parallel_pin_1  = 13 ;
 int parallel_pin_2  = 12 ;
 int parallel_pin_4  = 11 ;
 int parallel_pin_8  = 10 ;
 int parallel_pin_16 =  9 ;
 int parallel_pin_32 =  8 ; //}
// PARALLEL SPEED FUNCTION {

	int bit_p = 0 ;  // Variable que me invento para realizar las siguientes operaciones, almacenara en cada caso el valor de un bit dentro de un byte 

	void parallel_speed() 
	{
		//  BIT 1  {
			bit_p = kmh & 1 ;
			if (bit_p == 1) { digitalWrite(parallel_pin_1, HIGH); }
			else { digitalWrite(parallel_pin_1, LOW); }  //}
		//  BIT 2  {
			bit_p = kmh & 2 ;
			if (bit_p == 2) { digitalWrite(parallel_pin_2, HIGH); }
			else { digitalWrite(parallel_pin_2, LOW); }  //}
		//  BIT 4  {
			bit_p = kmh & 4 ;
			if (bit_p == 4) { digitalWrite(parallel_pin_4, HIGH); }
			else { digitalWrite(parallel_pin_4, LOW); }  //}
		//  BIT 8  {
			bit_p = kmh & 8 ;
			if (bit_p == 8) { digitalWrite(parallel_pin_8, HIGH); }
			else { digitalWrite(parallel_pin_8, LOW); }  //}
		//  BIT 16 {
			bit_p = kmh & 16 ;
			if (bit_p == 16) { digitalWrite(parallel_pin_16, HIGH); }
			else { digitalWrite(parallel_pin_16, LOW); }  //}
		//  BIT 32 {
			bit_p = kmh & 32 ;
			if (bit_p == 32) { digitalWrite(parallel_pin_32, HIGH); }
			else { digitalWrite(parallel_pin_32, LOW); }  //}
	}

// SETUP {
	void setup(){
		
		Serial.begin(9600);
		
		// PARALLEL COMMUNICATION {
			pinMode(parallel_pin_1,  OUTPUT);
			pinMode(parallel_pin_2,  OUTPUT);
			pinMode(parallel_pin_4,  OUTPUT);
			pinMode(parallel_pin_8,  OUTPUT);
			pinMode(parallel_pin_16, OUTPUT);
			pinMode(parallel_pin_32, OUTPUT); //}
		// TACHOMETER {
			pinMode        ( TACHOMETER_GND_PIN, OUTPUT );
			digitalWrite   ( TACHOMETER_GND_PIN, LOW );
			digitalWrite   ( TACHOMETER_PIN    , HIGH );
			//pinMode        ( TACHOMETER_PIN    , INPUT );
			attachInterrupt( digitalPinToInterrupt(TACHOMETER_PIN), tachometer_increment, RISING);  //}
	}

// LOOP {
	void loop()
	{	
		//kmh = kmh + 1 ;          //DEBUG
	  //if (kmh > 50){kmh = 0;}  //DEBUG
	  Serial.println(kmh);     //DEBUG
		//delay(300);              //DEBUG

		tachometer_cacls();
		parallel_speed();
	}
