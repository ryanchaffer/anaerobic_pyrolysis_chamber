#include <Adafruit_MCP9601.h> //Furnace Temperture Module
#include <Adafruit_BME280.h>  //O2 Box RH, Pressure, and Temperature Module
#include <DFRobot_OxygenSensor.h> //O2 sensor Module
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Display Module




#define COLLECT_NUMBER    10             // collect number, the collection range is 1-100.
#define Oxygen_IICAddress ADDRESS_3

#define Furnace_Temperature (0x67)
Adafruit_MCP9601 mcp;  

DFRobot_OxygenSensor Oxygen;
LiquidCrystal_I2C lcd(0x26, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_BME280 bme;

bool label =  true;
void setup()
{

  Serial.begin(9600);
  while (!Oxygen.begin(Oxygen_IICAddress)) {
//    Serial.println("I2c device number error !");
    delay(1000);
  }
//  Serial.println("I2c connect success !");

// Furnace Temperture Sensor
  if (! mcp.begin(Furnace_Temperature)) {
//        Serial.println("Sensor not found. Check wiring!");
        while (1);
    }
//  Serial.println("Found MCP9601!");

  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
//  Serial.print("ADC resolution set to ");
  switch (mcp.getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:   ; break;
    case MCP9600_ADCRESOLUTION_16:   ; break;
    case MCP9600_ADCRESOLUTION_14:   ; break;
    case MCP9600_ADCRESOLUTION_12:   ; break;
  }
//  Serial.println(" bits");

  mcp.setThermocoupleType(MCP9600_TYPE_K);
//  Serial.print("Thermocouple type set to ");
  switch (mcp.getThermocoupleType()) {
    case MCP9600_TYPE_K:  ; break;
    case MCP9600_TYPE_J: ; break;
    case MCP9600_TYPE_T:  ; break;
    case MCP9600_TYPE_N:  ; break;
    case MCP9600_TYPE_S:  ; break;
    case MCP9600_TYPE_E:  ; break;
    case MCP9600_TYPE_B:  ; break;
    case MCP9600_TYPE_R:  ; break;
  }
//  Serial.println(" type");

  mcp.setFilterCoefficient(3);
//  Serial.print("Filter coefficient value set to: ");
//  Serial.println(mcp.getFilterCoefficient());

  


// default settings for Relative Humidity Sensor
  unsigned status;
  status = bme.begin();


  lcd.init();                      // initialize the lcd
  lcd2.init();
  
  lcd.backlight();
  lcd2.backlight();
  
  // Print Start Up Message
    lcd.setCursor(5, 0);
    lcd.print("Welcome");
    lcd2.setCursor(6, 0);
    lcd2.print("FSRI");
    delay(10000);
 
  lcd.clear();                      // clear the lcd after Start Up Message
  lcd2.clear();

  // lcd 1 display parameters
  lcd.setCursor(0, 0);
  lcd.print("O2:");
  lcd.setCursor(10, 0);
  lcd.print("RH: ");
    lcd.setCursor(4, 1);
  lcd.print("P: ");

  // lcd 2 display parameters
  lcd2.setCursor(0, 0);
  lcd2.print("TG: ");
  lcd2.setCursor(0, 1);
  lcd2.print("TC: ");


  
}


void loop()
{



  
  // Temperature inside chamber display

  uint8_t status = mcp.getStatus();
//  Serial.print("MCP Status: 0x"); 
//  Serial.print(status, HEX);  
//  Serial.print(": ");
  
  
   double c = (mcp.readThermocouple());;
   lcd2.setCursor(4, 1);
   
   if (status & MCP9601_STATUS_OPENCIRCUIT) { 
    lcd2.print(c);
    lcd2.print("*");    
   }
   else if (status & MCP9601_STATUS_SHORTCIRCUIT) { 
   lcd2.print(c);
   lcd2.print("**");
   }
   else
   {
     lcd2.print(c);
     lcd2.print("      ");
    
   }
   
  // Oxygen Display
  float oxygenData = Oxygen.getOxygenData(COLLECT_NUMBER);
  lcd.setCursor(4, 0);
  lcd.print(oxygenData);


  // Relative Humidity Display
  lcd.setCursor(14, 0);
  float RH = (bme.readHumidity());
  String RH_Out = String(RH, 0); //Makes the float a string with 0 decimal places
  lcd.print(RH_Out);

  // Pressure Display
  lcd.setCursor(6, 1);
  float P = (bme.readPressure() / 1000.0F);
  String P_Out = String(P, 3); //Pressure in kPa
  lcd.print(P_Out);

  // Temperature of gas at Oxygen Sensor Display
  lcd2.setCursor(4, 0);
  float T = (bme.readTemperature());
  String T_Out = String(T, 2);
  lcd2.print(T_Out);


// Set up Serial Monitor to log data to .CSV

// 
  while(label){// runs once, sets up column headers
    Serial.print("Chamber Temperature [°C]");
    Serial.print(",");
    Serial.print("Oxygen Concentration [%]");
    Serial.print(",");
    Serial.print("Relative Humidity [%]");
    Serial.print(",");
    Serial.print("Pressure [kPa]");
    Serial.print(",");
    Serial.print("Oxygen Box Temperature [°C]");
    Serial.println(",");
    
    label=false;
  }

// Print the collected data out on the same line, then repeat every second 
  Serial.print(c);
  Serial.print(",");
  Serial.print(oxygenData);
  Serial.print(",");
  Serial.print(RH);
  Serial.print(",");
  Serial.print(P_Out);
  Serial.print(",");
  Serial.print(T);
  Serial.println(",");
 
  
  
  delay(1000);

 
}
