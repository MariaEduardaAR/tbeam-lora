# Análise de Comunicação Sem Fio LoRa para Monitoramento de Apiários de Abelhas no Semiárido Cearense

Este projeto apresenta o desenvolvimento e a análise de um sistema de monitoramento remoto de colmeias, utilizando **comunicação LoRa** entre nó sensor e gateway, integrados ao **Node-RED** via protocolo **MQTT**.

---

## 🎯 Objetivo
Avaliar a viabilidade da tecnologia **LoRa** para monitoramento ambiental em apiários localizados no semiárido cearense, medindo temperatura, umidade, CO₂ e TVOC.

---

## ⚙️ Arquitetura do Sistema
O sistema é dividido em três blocos:
1. **Nó Sensor (ESP32 LoRa)** — coleta dados ambientais.
2. **Gateway (TTGO T-Beam)** — recebe e repassa via Wi-Fi para o broker MQTT.
3. **Interface Node-RED** — exibe os dados em dashboards.

---

## 📡 Tecnologias Utilizadas
- LoRa SX1276  
- ESP32 e TTGO T-Beam  
- MQTT (Mosquitto Broker)  
- Node-RED  
- Sensores DHT11 e CJMCU-811  

---

## 📊 Resultados
O sistema atingiu um alcance estável de até **300 metros**, com RSSI médio de **−115 dBm** e SNR mínimo de **−19 dB**, comprovando a viabilidade da comunicação LoRa em ambientes semiáridos.

---

## 👩‍💻 Autores
- **Maria Eduarda Almeida Rodrigues** – UFC Quixadá  
- **Francisco Ítalo de Andrade Moraes** – UFC Quixadá  
- **Antonio Rafael Braga** – UFC Quixadá

---

## 📚 Referência
Rodrigues, M. E. A.; Moraes, F. I. A.; Braga, A. R. *Análise de Comunicação Sem Fio LoRa para Monitoramento de Apiários de Abelhas no Semiárido Cearense*. 2025.
