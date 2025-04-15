#include <stdio.h>
#include <stdlib.h>
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"

#define SDA_PIN 14
#define SCL_PIN 15
#define BOTAO_A 5
#define BOTAO_B 6

volatile bool verf = false;
volatile int8_t i = 0;
volatile uint16_t qtd_b = 0;

struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

uint8_t buffer[ssd1306_buffer_length];
repeating_timer_t timer;

void atualizar_display() {
    char texto[32];
    ssd1306_clear(buffer);
    
    sprintf(texto, "Contador: %d", i);
    ssd1306_draw_string(buffer, 0, 0, texto);
    
    sprintf(texto, "Cliques B: %d", qtd_b);
    ssd1306_draw_string(buffer, 0, 12, texto);
    
    render_on_display(buffer, &frame_area);
}

bool tick_timer(repeating_timer_t *t) {
    if (!verf) return false;
    if (--i < 0) {
        i = 0;
        verf = false;
        cancel_repeating_timer(&timer);
    }
    atualizar_display();
    return verf;
}

void isr_geral(uint gpio, uint32_t events) {
    if (gpio == BOTAO_A) {
        if (!verf) {
            verf = true;
            i = 9;
            qtd_b = 0;
            atualizar_display();
            add_repeating_timer_ms(1000, tick_timer, NULL, &timer);
        }
    } else if (gpio == BOTAO_B) {
        if (verf) {
            qtd_b++;
            atualizar_display();
        }
    }
}

void iniciacao() {
    stdio_init_all();

    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);
    render_on_display(buffer, &frame_area);

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    gpio_set_irq_enabled(BOTAO_A, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BOTAO_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &isr_geral);
}

int main() {
    iniciacao();
    atualizar_display();

    while (true) {
        tight_loop_contents();
    }
}