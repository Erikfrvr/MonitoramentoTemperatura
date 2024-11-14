#include <ESP8266WiFi.h>               // Biblioteca para conectar o NodeMCU ao Wi-Fi
#include <Adafruit_MQTT.h>             // Biblioteca para uso do protocolo MQTT
#include <Adafruit_MQTT_Client.h>      // Biblioteca para comunicação com o Adafruit IO via MQTT
#include <DHT.h>                       // Biblioteca para uso do sensor DHT

#define SENSOR_TYPE DHT11              // Definindo o tipo de sensor como DHT11
const uint8_t SENSOR_PIN = D2;         // Pino do NodeMCU ao qual o sensor DHT11 está conectado
DHT sensorDHT(SENSOR_PIN, SENSOR_TYPE); // Inicializando o sensor DHT11

#define BUZZER_PIN D5                  // Pino do NodeMCU ao qual o buzzer está conectado

// Credenciais da rede Wi-Fi
const char* WIFI_SSID = "SUA_SSID";        // Substitua "SUA_SSID" pelo nome da sua rede Wi-Fi
const char* WIFI_PASSWORD = "SUA_SENHA";   // Substitua "SUA_SENHA" pela senha da sua rede Wi-Fi

// Informações para conectar ao Adafruit IO
const char* AIO_SERVER = "io.adafruit.com";
const uint16_t AIO_PORT = 1883;
const char* AIO_USERNAME = "seu_usuario_adafruit"; // Substitua "seu_usuario_adafruit" pelo seu nome de usuário no Adafruit IO
const char* AIO_KEY = "sua_chave_de_API";          // Substitua "sua_chave_de_API" pela sua chave de API do Adafruit IO

WiFiClient wifiClient;                 // Cliente Wi-Fi para comunicação
Adafruit_MQTT_Client mqttClient(&wifiClient, AIO_SERVER, AIO_PORT, AIO_USERNAME, AIO_KEY); // Cliente MQTT

// Feeds para envio dos dados ao Adafruit IO
char feedTemp[64];
char feedHum[64];
char feedAlarm[64];  

Adafruit_MQTT_Publish* feedTemperature;
Adafruit_MQTT_Publish* feedHumidity;
Adafruit_MQTT_Publish* feedTempAlarm;  

// Intervalo de tempo para envio dos dados
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 10000;  // Envia dados a cada 10 segundos

// Limites de temperatura
const int TEMPERATURE_THRESHOLD = 19;  // Temperatura normal até 19°C
const int BUZZER_THRESHOLD = 20;       // Buzzer ativa a partir de 20°C

bool highTempSent = false;             // Controle para envio do alerta de alta temperatura
bool normalTempSent = false;           // Controle para envio do status de temperatura normal

const int NUM_SAMPLES = 5;             // Número de amostras para cálculo da média
float temperatureSamples[NUM_SAMPLES]; // Armazena as amostras de temperatura
float humiditySamples[NUM_SAMPLES];    // Armazena as amostras de umidade
int sampleIndex = 0;                   // Índice para armazenamento das amostras

// Função para conectar ao Wi-Fi
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
}

// Função para conectar ao Adafruit IO via MQTT
void connectMQTT() {
  while (mqttClient.connected() == false) {
    Serial.print("Attempting to connect to Adafruit IO... ");
    if (mqttClient.connect()) {
      Serial.println("Connected successfully!");
    } else {
      Serial.print("Connection failed. Error code: ");
      Serial.println(mqttClient.connectErrorString(mqttClient.connect()));
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

// Função para inicializar os feeds de temperatura, umidade e alerta
void initializeFeeds() {
  snprintf(feedTemp, sizeof(feedTemp), "%s/feeds/Temperature", AIO_USERNAME);
  snprintf(feedHum, sizeof(feedHum), "%s/feeds/Humidity", AIO_USERNAME);
  snprintf(feedAlarm, sizeof(feedAlarm), "%s/feeds/TemperatureAlarm", AIO_USERNAME);  

  feedTemperature = new Adafruit_MQTT_Publish(&mqttClient, feedTemp);
  feedHumidity = new Adafruit_MQTT_Publish(&mqttClient, feedHum);
  feedTempAlarm = new Adafruit_MQTT_Publish(&mqttClient, feedAlarm);  
}

// Função para controlar a intensidade do buzzer
void controlBuzzer(int intensity) {
  analogWrite(BUZZER_PIN, intensity);  
}

// Função para ler e armazenar as amostras de temperatura e umidade
void readAndStoreSamples() {
  float humidity = sensorDHT.readHumidity();
  float temperature = sensorDHT.readTemperature();

  if (isnan(humidity) || isnan(temperature)) { // Verifica se a leitura foi bem-sucedida
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Armazena as amostras nas arrays de temperatura e umidade
  temperatureSamples[sampleIndex] = temperature;
  humiditySamples[sampleIndex] = humidity;

  // Incrementa o índice para a próxima amostra
  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
}

// Função para calcular a média das amostras
float calculateAverage(float samples[], int numSamples) {
  float sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += samples[i];
  }
  return sum / numSamples;
}

// Função para ler e enviar os dados para o Adafruit IO
void readAndSendData() {
  readAndStoreSamples();  

  // Calcula a média das amostras
  float averageTemperature = calculateAverage(temperatureSamples, NUM_SAMPLES);
  float averageHumidity = calculateAverage(humiditySamples, NUM_SAMPLES);

  // Converte as médias para inteiros
  int tempInt = static_cast<int>(round(averageTemperature));
  int humInt = static_cast<int>(round(averageHumidity));

  // Envia a temperatura para o Adafruit IO
  if (!feedTemperature->publish(tempInt)) {
    Serial.println("Failed to send temperature to Adafruit IO.");
  }

  // Envia a umidade para o Adafruit IO
  if (!feedHumidity->publish(humInt)) {
    Serial.println("Failed to send humidity to Adafruit IO.");
  }

  Serial.print("Average Temperature: ");
  Serial.print(tempInt);
  Serial.print(" °C    Average Humidity: ");
  Serial.print(humInt);
  Serial.println(" %");

  // Verifica se a temperatura está acima do limite e controla o buzzer e o alerta
  if (tempInt >= BUZZER_THRESHOLD) {
    controlBuzzer(128);  // Aciona o buzzer
    if (!feedTempAlarm->publish("Alerta! Temperatura Alta!")) {
      Serial.println("Failed to send temperature alarm to Adafruit IO.");
    } else {
      Serial.println("Alerta de temperatura enviado ao Adafruit IO.");
    }
  } else {
    controlBuzzer(0);  // Desativa o buzzer
    if (!feedTempAlarm->publish("Temperatura normal")) {
      Serial.println("Failed to send normal temperature status to Adafruit IO.");
    } else {
      Serial.println("Temperatura normal enviada ao Adafruit IO.");
    }
  }
}

// Configuração inicial do sistema
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);  
  controlBuzzer(0);               // Inicializa o buzzer desligado
  connectWiFi();                  // Conecta ao Wi-Fi
  sensorDHT.begin();              // Inicia o sensor DHT11
  connectMQTT();                  // Conecta ao Adafruit IO via MQTT
  initializeFeeds();              // Inicializa os feeds de dados

  // Preenche as amostras iniciais
  for (int i = 0; i < NUM_SAMPLES; i++) {
    temperatureSamples[i] = sensorDHT.readTemperature();
    humiditySamples[i] = sensorDHT.readHumidity();
  }
}

// Loop principal do sistema
void loop() {
  if (!mqttClient.connected()) {  // Verifica a conexão com o MQTT
    connectMQTT();
  }

  mqttClient.ping();              // Mantém a conexão ativa

  // Envia dados a cada intervalo de tempo definido
  if (millis() - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = millis();
    readAndSendData();
  }
}

