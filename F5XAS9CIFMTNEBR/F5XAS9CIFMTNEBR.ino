#include <OneWire.h>                                             // Library sensor suhu
#include <DallasTemperature.h>                                   // Library sensor suhu
 
#define ONE_WIRE_BUS 2                                           // Data wire is plugged into pin 2 on the Arduino
 
OneWire oneWire(ONE_WIRE_BUS);                                   // Setup a oneWire instance to communicate with any OneWire devices
                                                                 // (not just Maxim/Dallas temperature ICs)
 
DallasTemperature sensors(&oneWire);                             // Pass our oneWire reference to Dallas Temperature.
 
void setup(void)
{
  Serial.begin(9600);                                            // start serial port
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();                                               // Start up the library
}
 
void loop(void)
{
  Serial.print(" Requesting temperatures...");                   // call sensors.requestTemperatures() to issue a global temperature
                                                                 // request to all devices on the bus
  sensors.requestTemperatures();                                 // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0));                      // Why "byIndex"? 
    delay(1000);                                                 // You can have more than one IC on the same bus. 
                                                                 // 0 refers to the first IC on the wire
}
