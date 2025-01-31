#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "tarefa_interrupcoes.pio.h"

#define NUM_PIXELS 25 //número de LEDs
#define OUT_PIN 7 //pino de saída 

//botões de interupção
const uint button_0 = 5;
const uint button_1 = 6;

//variáveis globais 
PIO pio;
uint sm;

//vetores com animação dos números
double numero_zero[25] =    {0.0, 0.0, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.5, 0.0, 0.0, 
                             0.0, 0.0, 0.5, 0.0, 0.5,
                             0.5, 0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.5, 0.5};

double numero_um[25] =      {0.0, 0.0, 0.0, 0.0, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.0, 0.0, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.5};

double numero_dois[25] =    {0.0, 0.0, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.5, 0.5, 0.5,
                             0.0, 0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.5, 0.5};  

double numero_tres[25] =    {0.0, 0.0, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.5, 0.5};  

double numero_quatro[25] =  {0.0, 0.0, 0.5, 0.0, 0.5,
                             0.5, 0.0, 0.5, 0.0, 0.0, 
                             0.0, 0.0, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.5}; 

double numero_cinco[25] =  {0.0, 0.0, 0.5, 0.5, 0.5,
                            0.0, 0.0, 0.5, 0.0, 0.0, 
                            0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.5, 0.5}; 

double numero_seis[25] =   {0.0, 0.0, 0.5, 0.5, 0.5,
                            0.0, 0.0, 0.5, 0.0, 0.0, 
                            0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.5, 0.5};
                             
double numero_sete[25] =   {0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.5, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.5,
                            0.5, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.5};

double numero_oito[25] =   {0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.5, 0.0, 0.0, 
                            0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.5, 0.5};

double numero_nove[25] =   {0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.5, 0.0, 0.0, 
                            0.0, 0.0, 0.5, 0.5, 0.5,
                            0.5, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.5, 0.5};

//rotina pra definição de cores do led
uint32_t matrix_rgb(double r, double g, double b){
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}  

//rotina para acionar a matriz de LEDs - ws2812b
//adiciona a cor aos LEDs
void desenho_pio(double *desenho){
    uint32_t valor_led;
    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(desenho[24 - i], 0.0, desenho[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void animaco_numeros(){
    desenho_pio(numero_zero);
    sleep_ms(2000);
    desenho_pio(numero_um);
    sleep_ms(2000);
    desenho_pio(numero_dois); 
    sleep_ms(2000);   
    desenho_pio(numero_tres);
    sleep_ms(2000);
    desenho_pio(numero_quatro);
    sleep_ms(2000);
    desenho_pio(numero_cinco);
    sleep_ms(2000);
    desenho_pio(numero_seis);
    sleep_ms(2000);
    desenho_pio(numero_sete);
    sleep_ms(2000);
    desenho_pio(numero_oito);
    sleep_ms(2000);
    desenho_pio(numero_nove);
    sleep_ms(2000);
}


//funcao principal
int main(){
    pio = pio0; 
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configurações da PIO
    uint offset = pio_add_program(pio, &tarefa_interrupcoes_program);
    sm = pio_claim_unused_sm(pio, true);
    tarefa_interrupcoes_program_init(pio, sm, offset, OUT_PIN);
   

    while (true){
       animaco_numeros();
    }
    
}