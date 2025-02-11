#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int main() {
    stdio_init_all();

    const uint servo_pin = 22;
    gpio_set_function(servo_pin, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(servo_pin);
    uint channel = pwm_gpio_to_channel(servo_pin);

    // Configuração do PWM para 50Hz (período de 20ms)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.f);    // Divisor de clock 100
    pwm_config_set_wrap(&config, 24999);      // Wrap para 20ms
    pwm_init(slice_num, &config, true);

    // Posição 180 graus (2400µs)
    uint32_t level = (2400 * 5 + 2) / 4;      // 3000
    pwm_set_chan_level(slice_num, channel, level);
    sleep_ms(5000);

    // Posição 90 graus (1470µs)
    level = (1470 * 5 + 2) / 4;               // 1838
    pwm_set_chan_level(slice_num, channel, level);
    sleep_ms(5000);

    // Posição 0 graus (500µs)
    level = (500 * 5 + 2) / 4;                // 625
    pwm_set_chan_level(slice_num, channel, level);
    sleep_ms(5000);

    // Movimento suave entre 0° e 180°
    int current_pulse = 500;
    int direction = 5;  // Incremento inicial de 5µs

    while (true) {
        // Calcula o nível PWM arredondado
        uint32_t level = (current_pulse * 5 + 2) / 4;
        pwm_set_chan_level(slice_num, channel, level);

        // Atualiza a posição
        current_pulse += direction;

        // Inverte direção nos limites
        if (current_pulse > 2400) {
            current_pulse = 2400;
            direction = -5;
        } else if (current_pulse < 500) {
            current_pulse = 500;
            direction = 5;
        }

        sleep_ms(10); // Atraso para movimento suave
    }

    return 0;
}