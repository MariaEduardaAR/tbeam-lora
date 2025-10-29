//ESP32 + RFM95 + DHT11 + CJMCU-811 (CCS811)
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <DHT.h>
#include <SPI.h>
#include <LoRa.h>

// DHT instance
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// CCS811 instance
Adafruit_CCS811 ccs;

// I2C (CCS811)
#define CCS_SDA 21
#define CCS_SCL 22

// LoRa SPI pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_BAND 915E6

#define HIVE_ID "H001"

float temp = 0.0;
float hum = 0.0;
uint16_t eco2 = 0;
uint16_t tvoc = 0;
  
void setup()
{
  Serial.begin(115200);
  delay(200);

  // I2C
  Wire.begin(CCS_SDA, CCS_SCL);
  // DHT init
  dht.begin();

  // CCS811 init
  Serial.println("Inicializando CCS811...");
  if (!ccs.begin())
  {
    Serial.println("Falha ao iniciar CCS811.");
    while (1) delay(1000);
  }
  delay(100);
  // LoRa init
  SPI.begin(SCK, MISO, MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  Serial.println("Inicializando LoRa...");
  if (!LoRa.begin(LORA_BAND))
  {
    Serial.println("Falha ao iniciar LoRa!");
    while (1) delay(1000);
  }
  Serial.println("LoRa OK");

  // Configuração avançada para longo alcance
  LoRa.setTxPower(20);
  LoRa.setSignalBandwidth(62500);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(16);
  LoRa.setSyncWord(0xAB);
  LoRa.disableCrc();
  LoRa.disableInvertIQ();
}

void loop() 
{
  // read DHT block
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  if(isnan(temp) || isnan(hum))
  {
    Serial.println("Erro ao ler DHT11");
  }
  else
  {
    ccs.setEnvironmentalData(hum, temp);
  }

  // read CCS811 block
  if(ccs.available())
  {
    if (!ccs.readData())
    {
      eco2 = ccs.geteCO2();
      tvoc = ccs.getTVOC();
    }
    else
    {
      Serial.println("Erro de leitura no CCS811");
    }
  }
  else
  {
    Serial.println("CCS811 não está disponível");
  }
  
  // Send data via LoRa
  // id=H001;temp=25.4;hum=60.2;gas=400;tvoc=50
  String payload = "id=" + String(HIVE_ID) +
                   ";temp=" + String(temp, 1) +
                   ";hum="  + String(hum, 1) +
                   ";gas="  + String(eco2) +
                   ";tvoc=" + String(tvoc);
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();
  Serial.print("Enviado: ");
  Serial.println(payload);

  delay(5000); // enviar a cada 5s
}