# Monitoramento de Apiários no Semiárido Cearense via Rede Sem Fio LoRa

Este projeto detalha o desenvolvimento e a validação de um sistema de monitoramento remoto de colmeias focado em apicultura de precisão. Utilizando Internet das Coisas (IoT), a solução acompanha variáveis ambientais em tempo real dentro de apiários no semiárido, ajudando apicultores a protegerem a saúde das abelhas contra o estresse térmico e variações climáticas da região.

O ecossistema realiza a leitura local de temperatura, umidade e gases na colmeia, transmitindo esses dados via radiofrequência (LoRa) até um gateway, que se encarrega de enviá-los para uma interface de monitoramento em nuvem via protocolo MQTT.

---

## 🎯 Objetivos do Projeto

### Objetivo Geral
* Desenvolver e validar um sistema estável de monitoramento remoto para colmeias utilizando sensores de baixo consumo e comunicação de longo alcance.

### Objetivos Específicos
* Coletar e tratar dados contínuos de temperatura e umidade interna das colmeias.
* Monitorar a qualidade do ar através dos índices de $CO_{2}$ e TVOC.
* Garantir a integridade da transmissão sem fio em ambientes rurais de difícil acesso via protocolo LoRa.
* Validar a estabilidade e o consumo individual de cada módulo de hardware.

---

## ⚙️ Arquitetura e Engenharia do Sistema

O sistema é modular e opera de forma integrada em três divisões principais:

1. **Nó Sensor (End-Node):** Instalado diretamente na colmeia. É baseado em um microcontrolador ESP32 conectado aos sensores DHT11 e CCS811, que realizam leituras periódicas e empacotam as informações para transmissão via rádio SX1276.
2. **Gateway:** Um módulo TTGO T-Beam responsável por receber os pacotes de rádio do nó sensor na frequência de 915 MHz e fazer a ponte de comunicação com a internet local via Wi-Fi.
3. **Interface de Usuário:** Uma central de processamento que recebe as strings de dados do gateway através de um Broker MQTT (Mosquitto) e renderiza gráficos interativos em tempo real utilizando um Dashboard no Node-RED.

---

## 📡 Hardware e Tecnologias Utilizadas

* **Microcontroladores:** ESP32 (Nó Sensor) e TTGO T-Beam (Gateway)
* **Transceptor RF:** Módulo LoRa SX1276 / RFM95
* **Sensores Ambientais:** * DHT11 (Temperatura e Umidade Relativa)
  * CCS811 / CJMCU-811 (Dióxido de Carbono Equivalente - eCO₂ e Compostos Orgânicos Voláteis Totais - TVOC)
* **Protocolos e Aplicações:** MQTT (Mosquitto), Node-RED, Barramento I2C e SPI.

---

## 📊 Resultados e Desempenho em Campo

Os testes práticos mostraram que a rede LoRa operou de forma confiável em médias distâncias no cenário rural. O sistema manteve uma transmissão estável com alcance de até 300 metros em linha de visada, apresentando um RSSI médio de −115 dBm e SNR limite de −19 dB, validando a imunidade ao ruído da modulação mesmo diante da vegetação típica do semiárido.

---

## 👩‍💻 Autores e Vínculo Acadêmico

Desenvolvido por alunos e pesquisadores da **Universidade Federal do Ceará (UFC) – Campus Quixadá**:

* **Maria Eduarda Almeida Rodrigues**
* **Francisco Ítalo de Andrade Moraes**
* **Prof. Dr. Antonio Rafael Braga** (Orientador)

### Referência Acadêmica
> Rodrigues, M. E. A.; Moraes, F. I. A.; Braga, A. R. **Análise de Comunicação Sem Fio LoRa para Monitoramento de Apiários de Abelhas no Semiárido Cearense.** 2025.
