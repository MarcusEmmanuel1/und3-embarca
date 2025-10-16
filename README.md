# 🌞 Controle de Servo com Sensor de Luminosidade — BitDogLab

Este projeto utiliza a placa **BitDogLab** (Raspberry Pi Pico W) para monitorar a luminosidade ambiente através do **sensor BH1750** e controlar um **servo motor SG90** conforme o nível de luz detectado.

---

## ⚙️ Funcionamento

- O sensor **BH1750** mede a luminosidade em **lux** via comunicação **I2C**.  
- O **servo SG90** é acionado conforme a intensidade da luz:
  - 🌑 **Menos de 100 lux:** o servo gira **no sentido horário**.
  - 🌕 **100 lux ou mais:** o servo gira **no sentido anti-horário** e a leitura é exibida no **Serial Monitor**.
  
Esse comportamento simula um sistema simples de **reação automática à luz**, podendo ser adaptado para aplicações como persianas automáticas, robótica ou sistemas de energia solar.

---

## 🧠 Componentes Utilizados

| Componente | Função | Conexão |
|-------------|--------|----------|
| Raspberry Pi Pico W (BitDogLab) | Microcontrolador principal | — |
| Sensor BH1750 | Medição de luminosidade (I2C) | SDA (GP0), SCL (GP1) |
| Servo motor SG90 | Movimento rotacional | Pino de controle (ex: GP15) |
| Jumpers | Conexões elétricas | — |

---

## 🔌 Ligações Elétricas (I2C)

| BH1750 | Pico W |
|--------|--------|
| VCC | 3.3V |
| GND | GND |
| SDA | GP0 |
| SCL | GP1 |

**Servo SG90:**  
- VCC → 5V  
- GND → GND  
- Sinal → GP15 (ou pino configurado no código)

---

## 💻 Como Usar

1. Clone este repositório:
   ```bash
   git clone https://github.com/seuusuario/servermqtt.git
