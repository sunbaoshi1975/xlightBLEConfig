#define BLE_CHIP_HC05             5
#define BLE_CHIP_HC06             6
#define BLE_CHIP_TYPE             BLE_CHIP_HC06

#define BLE_EN_PIN                D1

// Configuration Settings:
#define XLIGHT_BLE_SSID           "Xlight"        // BT module name
#define XLIGHT_BLE_CLASS          0x9A050C        // Class of Device (CoD)
#define XLIGHT_BLE_PIN            "0000"          // Pairing password for the BT module

#if BLE_CHIP_TYPE == BLE_CHIP_HC05
#define XLIGHT_BLE_BAUD           "38400,0,0"
#define XLIGHT_BLE_ROLE		        '0'		          // 0=>Slave role; 1=>Master role; 2=>Loopback
#else
#define XLIGHT_BLE_BAUD           '4'             // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600 (default), 5=>19200, 6=>38400, 7=>57600, 8=>115200
#define XLIGHT_BLE_ROLE		        'S'		          // S=>Slave role; M=>Master role
#endif

// AT commands for HC-05, ending with \r\n
// Input              Output
// AT                 OK
// AT+RESET           OK
// AT+VERSION?        +VERSION:version
//                    OK
// AT+ADDR?           +ADDR:address (NAP:UAP:LAP)
//                    OK
// AT+NAME?           +NAME:name
//                    OK or FAIL
// AT+NAME=name       OK
// AT+PSWD?           +PSWD:password
//                    OK
// AT+PSWD=password   OK
// AT+UART?           +UART=baud,stopbits,parity
//                    OK
// AT+UART=38400,0,0  OK
// AT+ROLE:           +ROLE:0|1|2
//                    OK
// AT+ROLE=0|1|2      OK
// AT+CLASS?          +CLASS:clas
//                    OK or FAIL
// AT+CLASS=class     OK

// AT commands for HC-06
// Input              Output
// AT                 OK
// AT+BAUD1-8         OK1200, OK2400...
// AT+NAMEname        OKsetname
// AT+PINpin          OKsetPIN
// AT+ROLE=S|M        OK+ROLE:S|M

// Specify system serial port, could be Serial, Serial1 or Seria2
#ifndef TheSerial
#define TheSerial       Serial
#endif

#ifndef SERIAL
#define SERIAL          TheSerial.printf
#endif

#ifndef SERIAL_LN
#define SERIAL_LN       TheSerial.printlnf
#endif

#ifndef BLEPort
#define BLEPort         Serial1             // Could be serial1 or serial2
#endif

char myChar;

void MyDelay1S()
{
#if BLE_CHIP_TYPE == BLE_CHIP_HC05
  delay(250);
  Particle.process();
#else
  delay(250);
  Particle.process();
  delay(250);
  Particle.process();
  delay(250);
  Particle.process();
  delay(250);
#endif
}

void setup()
{
  TheSerial.begin(9600);

#if BLE_CHIP_TYPE == BLE_CHIP_HC05
  BLEPort.begin(38400);
  pinMode(BLE_EN_PIN, INPUT);
#else
  BLEPort.begin(9600);
#endif

  // Wait Serial connection so that we can see the starting information
	while(!TheSerial.available()) { Particle.process(); }
	SERIAL_LN(F("BLE demo is starting..."));

  // Module configuration:
#if BLE_CHIP_TYPE == BLE_CHIP_HC05

  // Starts configuration
  digitalWrite(BLE_EN_PIN, HIGH);

  // Starts configuration
  BLEPort.println("AT"); MyDelay1S();

  // Adjusting bluetooth name
  BLEPort.printlnf("AT+NAME=%s", XLIGHT_BLE_SSID); MyDelay1S();

  // Baud rate adjust
  BLEPort.printlnf("AT+UART=%s", XLIGHT_BLE_BAUD); MyDelay1S();

  // Password adjust
  BLEPort.printlnf("AT+PSWD=%s", XLIGHT_BLE_PIN); MyDelay1S();

  // CoD adjust
  BLEPort.printlnf("AT+CLASS=0x%x", XLIGHT_BLE_CLASS); MyDelay1S();

  // Role adjust
  //BLEPort.printlnf("AT+ROLE=%c", XLIGHT_BLE_ROLE); MyDelay1S();

  // Stop configuration
  digitalWrite(BLE_EN_PIN, LOW);

#else

  // Starts configuration
  BLEPort.print("AT"); MyDelay1S();

  // Adjusting bluetooth name
  BLEPort.print("AT+NAME"); BLEPort.print(XLIGHT_BLE_SSID); MyDelay1S();

  // Baud rate adjust
  BLEPort.print("AT+BAUD"); BLEPort.print(XLIGHT_BLE_BAUD); MyDelay1S();

  // Password adjust
  BLEPort.print("AT+PIN"); BLEPort.print(XLIGHT_BLE_PIN); MyDelay1S();

  // Role adjust
  //BLEPort.print("AT+ROLE"); BLEPort.print(XLIGHT_BLE_ROLE); MyDelay1S();

#endif

  SERIAL_LN("\r\nEnter AT commands:");
}

void loop()
{
  // Keep reading from HC-05/06 and send to Serial Monitor
  while (BLEPort.available()) {
    myChar = BLEPort.read();
    TheSerial.print(myChar);
  }

  // Keep reading from Serial Monitor and send to HC-05/06
  while (TheSerial.available()) {
    myChar = TheSerial.read();
    Serial.print(myChar); //echo
    BLEPort.print(myChar);
  }
}
