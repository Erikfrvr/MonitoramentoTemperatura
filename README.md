## 🌡️ Medidor de Temperatura e Umidade Inteligente para Ambientes Industriais

Este projeto monitora em tempo real a temperatura e a umidade em ambientes industriais, utilizando o sensor DHT11 e o atuador buzzer, controlados pela placa NodeMCU ESP8266. Os dados coletados são enviados para a plataforma Adafruit IO via protocolo MQTT, permitindo visualização e monitoramento remoto.

## 📝 Visão Geral do Projeto

A placa principal utilizada é o **NodeMCU ESP8266**, responsável pelo controle de toda a prototipagem e comunicação via Wi-Fi. O NodeMCU está conectado ao sensor DHT11, que monitora a temperatura e a umidade, e ao buzzer, que emite um alerta sonoro caso a temperatura ultrapasse um limite seguro. A comunicação com o message broker **Adafruit IO** é feita utilizando o protocolo MQTT, permitindo que os dados sejam monitorados em tempo real.

### Objetivo

O objetivo é criar um sistema de monitoramento de temperatura e umidade para ambientes industriais. Caso a temperatura ultrapasse 20°C, o buzzer é ativado, emitindo um alerta sonoro. Quando a temperatura volta ao normal, o buzzer é desativado, e uma mensagem de normalidade é enviada ao Adafruit IO.

## 🔧 Componentes Utilizados

- **NodeMCU ESP8266**: Controlador principal para comunicação e coleta de dados.
- **Sensor DHT11**: Sensor de temperatura e umidade com precisão de ±2°C e ±5% para umidade.
- **Buzzer Ativo 5V**: Emite um alarme sonoro quando a temperatura ultrapassa o limite seguro.
- **Protoboard de 400 pontos**: Utilizada para interligação dos componentes.
- **Fonte de Alimentação**: O sistema pode ser alimentado por uma power bank, facilitando a mobilidade.

## 📡 Comunicação e Protocolo MQTT

O projeto utiliza o protocolo MQTT para comunicação entre o NodeMCU e o broker Adafruit IO. No modelo de "publicação e assinatura", o NodeMCU publica os dados de temperatura e umidade em tópicos específicos no Adafruit IO. Caso a temperatura ultrapasse o limite seguro, um alerta é enviado ao tópico de alarme, e o buzzer é ativado.

## 🔧 Como Executar o Projeto

### Código Principal: `MonitoramentoTemperatura.ino`

Este código é utilizado para monitorar em tempo real a temperatura e a umidade em ambientes industriais. Ele coleta dados do sensor DHT11, envia essas informações para o Adafruit IO via protocolo MQTT e aciona o buzzer caso a temperatura ultrapasse o limite seguro de 20°C.

#### Passo a Passo para Executar o Monitoramento com o Código `MonitoramentoTemperatura.ino`

1. **Conecte o NodeMCU ao seu computador** e abra o código `MonitoramentoTemperatura.ino` na Arduino IDE.
2. **Realize o upload do código** para a placa NodeMCU.
3. **Configure as credenciais** no código para conectar o NodeMCU à sua rede Wi-Fi e ao Adafruit IO. Substitua os seguintes campos no código:
   - `const char* WIFI_SSID = "SUA_SSID";` - Substitua `"SUA_SSID"` pelo nome da sua rede Wi-Fi.
   - `const char* WIFI_PASSWORD = "SUA_SENHA";` - Substitua `"SUA_SENHA"` pela senha da sua rede Wi-Fi.
   - `const char* AIO_USERNAME = "seu_usuario_adafruit";` - Substitua `"seu_usuario_adafruit"` pelo seu nome de usuário no Adafruit IO.
   - `const char* AIO_KEY = "sua_chave_de_API";` - Substitua `"sua_chave_de_API"` pela chave de API do Adafruit IO.
4. **Abra o painel do Adafruit IO**: Acesse a sua conta no Adafruit IO e prepare os feeds para receber os dados de temperatura, umidade e o status do alarme.
5. **Monitore os dados no Adafruit IO**: Após o upload e a configuração, os dados coletados pelo sensor DHT11 (temperatura e umidade) serão enviados periodicamente para o Adafruit IO.
6. **Ação do Buzzer**: Caso a temperatura ultrapasse 20°C, o buzzer será ativado para emitir um alerta sonoro. Quando a temperatura voltar ao nível seguro (abaixo de 20°C), o buzzer será desativado, e uma mensagem de normalidade será enviada ao Adafruit IO.

#### Funções Monitoradas

- **Monitoramento de Temperatura e Umidade**: Envia leituras periódicas de temperatura e umidade para o Adafruit IO, permitindo monitoramento remoto em tempo real.
- **Acionamento do Buzzer**: Emite um alerta sonoro caso a temperatura exceda 20°C, sinalizando uma condição de temperatura elevada.
- **Controle Automático do Alarme**: O alarme é automaticamente desativado e uma notificação de temperatura normal é enviada ao Adafruit IO quando a temperatura volta ao nível seguro.

> **Nota**: O código `MonitoramentoTemperatura.ino` inclui comentários explicativos em todas as partes importantes, facilitando o entendimento do funcionamento e da lógica implementada.

---

### Código de Medição de Resposta: `MonitoramentoTemperatura_MedicaoResposta.ino`

Este código é utilizado exclusivamente para medir os tempos de resposta do sensor DHT11, do envio de dados via MQTT e do acionamento do buzzer. As medições de tempo são exibidas no Monitor Serial.

#### Passo a Passo para Medir os Tempos de Resposta com o Código `MonitoramentoTemperatura_MedicaoResposta.ino`

1. **Conecte o NodeMCU ao seu computador** e abra o código `MonitoramentoTemperatura_MedicaoResposta.ino` na Arduino IDE.
2. **Realize o upload do código** para a placa NodeMCU.
3. **Abra o Monitor Serial** na Arduino IDE para observar os tempos de resposta.
4. O Monitor Serial exibirá os seguintes tempos de resposta:
   - **Tempo de resposta do sensor DHT11**: Mede o tempo necessário para ler a temperatura e a umidade do ambiente.
   - **Tempo de resposta do MQTT (temperatura e umidade)**: Mede o tempo que leva para enviar as leituras de temperatura e umidade para o Adafruit IO.
   - **Tempo de resposta do buzzer**: Mede o tempo necessário para acionar e desativar o buzzer quando o limite de temperatura é ultrapassado.

#### Tabelas de Resultados

Após realizar as medições, os tempos de resposta observados no Monitor Serial foram:

##### Tabela de Medições do Tempo de Resposta para o Sensor DHT11

| Núm. medida | Sensor/Atuador | Tempo de resposta (ms) |
|-------------|-----------------|------------------------|
| 1           | Sensor DHT11    | 25                    |
| 2           | Sensor DHT11    | 25                    |
| 3           | Sensor DHT11    | 25                    |
| 4           | Sensor DHT11    | 25                    |
| **Média**   | Sensor DHT11    | **25**                |

##### Tabela de Medições do Tempo de Resposta para o Buzzer

| Núm. medida | Sensor/Atuador | Tempo de resposta (ms) |
|-------------|-----------------|------------------------|
| 1           | Buzzer          | 4.0                   |
| 2           | Buzzer          | 4.0                   |
| 3           | Buzzer          | 4.0                   |
| 4           | Buzzer          | 4.0                   |
| **Média**   | Buzzer          | **4.0**               |

---

> **Nota**: O código `MonitoramentoTemperatura_MedicaoResposta.ino` também contém comentários em todas as partes importantes, permitindo uma compreensão fácil de como as medições de tempo de resposta são realizadas.