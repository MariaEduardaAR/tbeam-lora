#include <Arduino.h>
#include <PubSubClient.h>
#include <LoRa.h>
#include <WiFi.h>
#include "LoRaBoards.h"

// Configurações padrão
#ifndef CONFIG_RADIO_FREQ
#define CONFIG_RADIO_FREQ           915.0
#endif
#ifndef CONFIG_RADIO_OUTPUT_POWER
#define CONFIG_RADIO_OUTPUT_POWER   20
#endif
#ifndef CONFIG_RADIO_BW
#define CONFIG_RADIO_BW             125.0
#endif

#if !defined(USING_SX1276) && !defined(USING_SX1278)
#error "LoRa example is only allowed to run SX1276/78. For other RF models, please run examples/RadioLibExamples"
#endif

// ---- WiFi ----
const char* ssid = "WIFI111D";
const char* password = "wifi111d";

// ---- MQTT Broker ----
const char* mqtt_server = "192.168.1.104";
const uint16_t mqtt_port = 1883;
const char* mqtt_client_id = "LilygoGateway";
const char* MQTT_username = NULL;
const char* MQTT_password = NULL;

// Tópicos MQTT
const char* TOPIC_ID = "colonymon/id";
const char* TOPIC_TEMP = "colonymon/temperature";
const char* TOPIC_HUM = "colonymon/humidity";
const char* TOPIC_GAS = "colonymon/gas";
const char* TOPIC_TVOC = "colonymon/tvoc"; // Existe?

// Timeouts e intervalos
const unsigned long WIFI_CONNECT_TIMEOUT = 30000; // 30 segundos
const unsigned long MQTT_RECONNECT_DELAY = 5000;  // 5 segundos
const unsigned long WATCHDOG_INTERVAL = 60000;    // 1 minuto

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastReconnectAttempt = 0;
unsigned long lastWatchdogCheck = 0;

// Protótipos de função
void connectToWiFi(void);
bool connectToMQTT(void);
void MQTTreconnect(void);
void mqttCallback(char* topic, byte* payload, unsigned int length);
void processLoRaPacket(const String& receivedData);
bool parseSensorData(const String& data, String& id, float& temp, float& hum, int& gas, int& tvoc);
void safePublishToBroker(const char* topic, const String& payload);

void connectToWiFi() 
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  unsigned long startAttemptTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected - ESP IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
    Serial.println("Restarting in 10 seconds...");
    delay(10000);
    ESP.restart();
  }
}

bool connectToMQTT() 
{
  Serial.print("Attempting MQTT connection...");
  
  bool connected = false;
  if (MQTT_username && MQTT_password) {
    connected = client.connect(mqtt_client_id, MQTT_username, MQTT_password);
  } else {
    connected = client.connect(mqtt_client_id);
  }
  
  if (connected) {
    Serial.println("connected");
    return true;
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println();
    return false;
  }
}

void MQTTreconnect() 
{
  unsigned long currentTime = millis();
  
  // Evita tentativas muito frequentes
  if (currentTime - lastReconnectAttempt < MQTT_RECONNECT_DELAY) {
    return;
  }
  
  lastReconnectAttempt = currentTime;
  
  if (connectToMQTT()) {
    lastReconnectAttempt = 0;
  }
}

void safePublishToBroker(const char* topic, const String& payload) 
{
  if (client.connected()) {
    bool published = client.publish(topic, payload.c_str());
    if (published) {
      Serial.printf("Published to %s: %s\n", topic, payload.c_str());
    } else {
      Serial.printf("Failed to publish to %s\n", topic);
    }
  }
}

bool parseSensorData(const String& data, String& id, float& temp, float& hum, int& gas, int& tvoc) 
{
  // Valores padrão para indicar erro
  id = "UNKNOWN";
  temp = -999.0;
  hum = -999.0;
  gas = -999;
  tvoc = -999;
  
  // Verifica se contém todos os campos necessários
  if (data.indexOf("temp=") == -1 || 
      data.indexOf("hum=") == -1 || 
      data.indexOf("gas=") == -1 ||
      data.indexOf("tvoc=") == -1) {
    Serial.println("Campos obrigatórios não encontrados");
    return false;
  }
  
  try {
    // Extrai ID (opcional mas esperado)
    int idIndex = data.indexOf("id=");
    if (idIndex != -1) {
      int idEnd = data.indexOf(";", idIndex);
      if (idEnd == -1) idEnd = data.length();
      id = data.substring(idIndex + 3, idEnd);
      id.trim(); // Remove espaços em branco
    }
    
    // Extrai temperatura
    int tempIndex = data.indexOf("temp=");
    int tempEnd = data.indexOf(";", tempIndex);
    if (tempEnd == -1) tempEnd = data.length();
    String tempStr = data.substring(tempIndex + 5, tempEnd);
    tempStr.trim();
    temp = tempStr.toFloat();
    
    // Extrai umidade
    int humIndex = data.indexOf("hum=");
    int humEnd = data.indexOf(";", humIndex);
    if (humEnd == -1) humEnd = data.length();
    String humStr = data.substring(humIndex + 4, humEnd);
    humStr.trim();
    hum = humStr.toFloat();
    
    // Extrai gás (eCO2)
    int gasIndex = data.indexOf("gas=");
    int gasEnd = data.indexOf(";", gasIndex);
    if (gasEnd == -1) gasEnd = data.length();
    String gasStr = data.substring(gasIndex + 4, gasEnd);
    gasStr.trim();
    gas = gasStr.toInt();
    
    // Extrai TVOC
    int tvocIndex = data.indexOf("tvoc=");
    int tvocEnd = data.indexOf(";", tvocIndex);
    if (tvocEnd == -1) tvocEnd = data.length();
    String tvocStr = data.substring(tvocIndex + 5, tvocEnd);
    tvocStr.trim();
    tvoc = tvocStr.toInt();
    
    // Validação básica dos dados
    if (temp < -40.0 || temp > 85.0) {
      Serial.println("Temperatura fora da faixa esperada (-40 a 85ºC)");
      return false;
    }
    
    if (hum < 0.0 || hum > 100.0) {
      Serial.println("Umidade fora da faixa esperada (0 a 100%)");
      return false;
    }
    
    if (gas < 0 || gas > 5000) {
      Serial.println("Valor de eCO2 fora da faixa esperada (0 a 5000 ppm)");
      return false;
    }
    
    if (tvoc < 0 || tvoc > 1000) {
      Serial.println("Valor de TVOC fora da faixa esperada (0 a 1000 ppb)");
      return false;
    }
    
    return true;
    
  } catch (...) {
    Serial.println("Erro ao processar dados do sensor");
    return false;
  }
}

void processLoRaPacket(const String& receivedData) 
{
  int rssi = LoRa.packetRssi();
  float snr = LoRa.packetSnr();

  Serial.println("======================================");
  Serial.print("Pacote recebido: ");
  Serial.println(receivedData);
  Serial.printf("RSSI: %d dBm | SNR: %.2f dB\n", rssi, snr);

  String id;
  float temperatureC;
  float humidity;
  int gasValue;
  int tvocValue;
  
  if (parseSensorData(receivedData, id, temperatureC, humidity, gasValue, tvocValue)) {
    // Publica no MQTT
    safePublishToBroker(TOPIC_ID, id);
    safePublishToBroker(TOPIC_TEMP, String(temperatureC, 1)); // 1 casa decimal
    safePublishToBroker(TOPIC_HUM, String(humidity, 1));     // 1 casa decimal
    safePublishToBroker(TOPIC_GAS, String(gasValue));
    safePublishToBroker(TOPIC_TVOC, String(tvocValue));      // Nova publicação
    
    Serial.printf("ID: %s | Temp: %.1f ºC | Hum: %.1f %% | Gas: %d ppm | TVOC: %d ppb\n", 
                  id.c_str(), temperatureC, humidity, gasValue, tvocValue);
  } else {
    Serial.println("Pacote malformado ou dados inválidos, ignorado.");
  }

  Serial.printf("ID: %s | Temp: %.1f ºC | Hum: %.1f %% | Gas: %d ppm | TVOC: %d ppb | RSSI: %d dBm | SNR: %.2f dB\n",
              id.c_str(), temperatureC, humidity, gasValue, tvocValue, rssi, snr);
  Serial.println("======================================");

}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("Inicializando Gateway LoRa-MQTT");
  
  // Configura WiFi
  connectToWiFi();

  // Configura MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
  client.setBufferSize(512); // Aumenta buffer para mensagens maiores
  
  // Configura LoRa
  setupBoards();
  delay(1500);
  
  Serial.println("LoRa Receiver");
  
#ifdef RADIO_TCXO_ENABLE
  pinMode(RADIO_TCXO_ENABLE, OUTPUT);
  digitalWrite(RADIO_TCXO_ENABLE, HIGH);
#endif

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  
  if (!LoRa.begin(CONFIG_RADIO_FREQ * 1000000)) {
    Serial.println("Starting LoRa failed!");
    Serial.println("Verifique a fiação e configuração do rádio");
    while (1) {
      delay(1000);
    }
  }
  
  // Configuração do LoRa
  // Configuração do LoRa (para longo alcance)
  LoRa.setTxPower(20);
  LoRa.setSignalBandwidth(62500);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(16);
  LoRa.setSyncWord(0xAB);
  LoRa.disableCrc();
  LoRa.disableInvertIQ();
  LoRa.receive();
  
  Serial.println("LoRa inicializado com sucesso");
  lastReconnectAttempt = millis();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  // Callback para mensagens MQTT recebidas (se necessário)
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void loop() 
{
  // Mantém conexão MQTT
  if (!client.connected()) {
    MQTTreconnect();
  }
  client.loop();
  
  // Watchdog para verificar conexões periodicamente
  unsigned long currentTime = millis();
  if (currentTime - lastWatchdogCheck > WATCHDOG_INTERVAL) {
    lastWatchdogCheck = currentTime;
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, reconnecting...");
      connectToWiFi();
    }
  }
  
  // Processa pacotes LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    String receivedData = "";
    
    // Lê todos os bytes disponíveis
    while (LoRa.available()) {
      char c = (char)LoRa.read();
      // Limita o tamanho máximo para evitar estouro de memória
      if (receivedData.length() < 256) {
        receivedData += c;
      }
    }
    
    // Processa apenas se não estiver vazia
    if (receivedData.length() > 0) {
      processLoRaPacket(receivedData);
    }
    
    // Retorna para modo de recepção
    LoRa.receive();
  }
  
  // Pequeno delay para evitar sobrecarga
  delay(10);
}