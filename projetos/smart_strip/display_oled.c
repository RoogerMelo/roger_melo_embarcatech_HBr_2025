#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "pico/time.h"

// Configuração dos pinos
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BOTAO_A = 5;  
const uint BOTAO_B = 6;  
const uint LED_VERMELHO = 13;  
const uint LED_VERDE = 11;
const uint LED_AZUL = 12;  

// Função para debounce do botão
bool debouncing() {
    static uint64_t ultimo_tempo = 0;
    uint64_t agora = time_us_64();
    
    if (agora - ultimo_tempo < 300000)  // 300ms de debounce
        return false;
    
    ultimo_tempo = agora;
    return true;
}

// Função de inicialização
void iniciacao() {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_put(LED_AZUL, 0);

    // Configuração do display
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    // Configuração dos botões (Alterado para pull-up)
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
}

int main() {
    iniciacao();

    // Configuração do display
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Loop principal
    while (true) {
        // Limpa o display
        ssd1306_clear(ssd);

        // Verifica se BOTAO_B foi pressionado
        if (gpio_get(BOTAO_B) == 0 && debouncing()) {  
            ssd1306_draw_string(ssd, 0, 0, " AGUARDE A");
            ssd1306_draw_string(ssd, 0, 12, "EMPILHADEIRA");
            render_on_display(ssd, &frame_area);

            gpio_put(LED_AZUL, true);
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_VERDE, false);
            sleep_ms(5000);
        } 
        else {
            // Cor verde para passagem autorizada
            gpio_put(LED_AZUL, false);
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_VERDE, true);

            ssd1306_draw_string(ssd, 0, 0, "SEM");
            ssd1306_draw_string(ssd, 0, 12, "PEDESTRES");
            render_on_display(ssd, &frame_area);

            // Verifica se BOTAO_A foi pressionado
            if (gpio_get(BOTAO_A) == 0 && debouncing()) {  
                
                // Pedestre presente: passagem não autorizada
                gpio_put(LED_VERMELHO, true);
                gpio_put(LED_VERDE, false);
                gpio_put(LED_AZUL, false);

                bool pedestres_presente = true;
                
                while (pedestres_presente) {
                    // Contagem regressiva
                    for (int i = 5; i >= 0; i--) {
                        char tempo[12];
                        sprintf(tempo, "Tempo: %d", i);
                        
                        ssd1306_clear(ssd);
                        ssd1306_draw_string(ssd, 0, 0, "PEDESTRES");
                        ssd1306_draw_string(ssd, 0, 12, tempo);
                        render_on_display(ssd, &frame_area);
                        
                        sleep_ms(1000);
                    }

                    // Gera um novo valor aleatório para verificar pedestres
                    pedestres_presente = (rand() % 5) == 1;
                }

                // Quando não houver pedestres, liberar passagem
                ssd1306_draw_string(ssd, 0, 0, "SEM");
                ssd1306_draw_string(ssd, 0, 12, "PEDESTRES");
                gpio_put(LED_VERDE, true);
                gpio_put(LED_VERMELHO, false);
                render_on_display(ssd, &frame_area);
            }
        }       
        sleep_ms(50);
    }

    return 0;
}
