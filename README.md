# Monitoramento de Apiários no Semiárido Cearense via Rede Sem Fio LoRa

[cite_start]Este projeto detalha o desenvolvimento e a validação de um sistema de monitoramento remoto de colmeias focado em apicultura de precisão[cite: 6, 102]. [cite_start]Utilizando Internet das Coisas (IoT), a solução acompanha variáveis ambientais em tempo real dentro de apiários no semiárido, ajudando apicultores a protegerem a saúde das abelhas contra o estresse térmico e variações climáticas da região[cite: 10, 108, 109].

[cite_start]O ecossistema realiza a leitura local de temperatura, umidade e gases na colmeia, transmitindo esses dados via radiofrequência (LoRa) até um gateway, que se encarrega de enviá-los para uma interface de monitoramento em nuvem via protocolo MQTT[cite: 103, 104, 129].

---

## 🎯 Objetivos do Projeto

### Objetivo Geral
* [cite_start]Desenvolver e validar um sistema estável de monitoramento remoto para colmeias utilizando sensores de baixo consumo e comunicação de longo alcance[cite: 110, 113, 196].

### Objetivos Específicos
* [cite_start]Coletar e tratar dados contínuos de temperatura e umidade interna[cite: 115].
* [cite_start]Monitorar a qualidade do ar nas colmeias através dos índices de $CO_{2}$ e TVOC[cite: 116].
* [cite_start]Garantir a integridade da transmissão sem fio em ambientes rurais de difícil acesso via protocolo LoRa[cite: 110, 117].
* [cite_start]Validar a estabilidade e o consumo individual de cada módulo de hardware[cite: 118].

---

## ⚙️ Arquitetura e Engenharia do Sistema

[cite_start]O sistema é modular e opera de forma integrada em três divisões principais[cite: 129]:

1. [cite_start]**Nó Sensor (End-Node):** Instalado diretamente na colmeia[cite: 103]. [cite_start]É baseado em um microcontrolador ESP32 conectado aos sensores DHT11 e CCS811, que realizam leituras periódicas e empacotam as informações para transmissão via rádio SX1276[cite: 104, 169].
2. [cite_start]**Gateway:** Um módulo TTGO T-Beam responsável por receber os pacotes de rádio do nó sensor na frequência de 915 MHz e fazer a ponte de comunicação com a internet local via Wi-Fi[cite: 104, 134, 143].
3. [cite_start]**Interface de Usuário:** Uma central de processamento que recebe as strings de dados do gateway através de um Broker MQTT (Mosquitto) e renderiza gráficos interativos em tempo real utilizando um Dashboard no Node-RED[cite: 103, 143].

---

## 📡 Hardware e Tecnologias Utilizadas

* [cite_start]**Microcontroladores:** ESP32 (Nó Sensor) e TTGO T-Beam (Gateway) [cite: 104]
* [cite_start]**Transceptor RF:** Módulo LoRa SX1276 / RFM95 [cite: 104, 122]
* [cite_start]**Sensores Ambientais:** * DHT11 (Temperatura e Umidade Relativa) [cite: 104, 123]
  * [cite_start]CCS811 / CJMCU-811 (Dióxido de Carbono Equivalente - eCO₂ e Compostos Orgânicos Voláteis Totais - TVOC) [cite: 104, 124, 179]
* [cite_start]**Protocolos e Aplicações:** MQTT (Mosquitto), Node-RED, Barramento I2C e SPI[cite: 103, 143, 150, 157].

---

## 📊 Resultados e Desempenho em Campo

[cite_start]Os testes práticos mostraram que a rede LoRa operou de forma confiável em médias distâncias no cenário rural[cite: 188]. O sistema manteve uma transmissão estável com alcance de até 300 metros em linha de visada, apresentando um RSSI médio de −115 dBm e SNR limite de −19 dB, validando a imunidade ao ruído da modulação mesmo diante da vegetação típica do semiárido.

---

## 👩‍💻 Autores e Vínculo Acadêmico

Desenvolvido por alunos e pesquisadores da **Universidade Federal do Ceará (UFC) – Campus Quixadá**:

* [cite_start]**Maria Eduarda Almeida Rodrigues** [cite: 100]
* **Francisco Ítalo de Andrade Moraes**
* **Prof. Dr. Antonio Rafael Braga** (Orientador)

### Referência Acadêmica
> Rodrigues, M. E. A.; Moraes, F. I. A.; Braga, A. R. **Análise de Comunicação Sem Fio LoRa para Monitoramento de Apiários de Abelhas no Semiárido Cearense.** 2025.
