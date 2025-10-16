#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "bh1750.h"

#define SERVO_PIN 28
#define LUX_THRESHOLD 100.0f  // limite de luminosidade
#define PWM_FREQ 50.0f        // frequência padrão dos servos (50Hz)
#define SERVO_MIN_US 500      // pulso de 0°
#define SERVO_MAX_US 2400     // pulso de 180°

void set_servo_angle(uint pin, float angle) {
    // Clampeia o ângulo entre 0 e 180
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Calcula o wrap e o nível de pulso
    uint slice = pwm_gpio_to_slice_num(pin);
    uint16_t wrap = pwm_hw->slice[slice].top;

    // Converte ângulo em largura de pulso (µs)
    float pulse_us = SERVO_MIN_US + (angle / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);
    uint16_t level = (uint16_t)((pulse_us / 20000.0f) * wrap);

    pwm_set_gpio_level(pin, level);
}

int main() {
    stdio_init_all();

    // === Inicializa o I2C e o BH1750 ===
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    bh1750_init();
    printf("Sistema iniciado: Sensor BH1750 + Servo\n");

    // === Configuração do PWM do servo ===
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062); // 50Hz com clock base 125MHz
    pwm_init(slice_num, &config, true);

    // === Estado inicial ===
    bool iluminado = false;
    float current_angle = 0;
    set_servo_angle(SERVO_PIN, current_angle);
    sleep_ms(500);

    while (1) {
        uint16_t raw = bh1750_read_lux();
        float lux = bh1750_raw_to_lux(raw);
        printf("Luminosidade: %.2f lux\n", lux);

        // Se acima do limite e ainda não está iluminado → move para 180°
        if (lux > LUX_THRESHOLD && !iluminado) {
            iluminado = true;
            printf("Ambiente iluminado! Girando servo para 180°.\n");

            for (float a = current_angle; a <= 180; a += 1) {
                set_servo_angle(SERVO_PIN, a);
                sleep_ms(20); // controle de velocidade (aumente pra mover mais devagar)
            }
            current_angle = 180;
        }

        // Se abaixo do limite e estava iluminado → volta pra 0°
        else if (lux <= LUX_THRESHOLD && iluminado) {
            iluminado = false;
            printf("Ambiente escuro! Voltando servo para 0°.\n");

            for (float a = current_angle; a >= 0; a -= 1) {
                set_servo_angle(SERVO_PIN, a);
                sleep_ms(20);
            }
            current_angle = 0;
        }

        sleep_ms(500); // tempo entre leituras do sensor
    }
}
