#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "tarefa_interrupcoes.pio.h"

//matriz de LEDs
#define NUM_PIXELS 25 //número de LEDs
#define OUT_PIN 7 //pino de saída 

//LED RGB
#define LED_RED 13 //LED vermelho conectado ao GPIO 13

//tempo para LED vermelho piscar
#define TEMPO 100 // LED vai piscar 5 vezes por segundo

//botões de interupção
#define BUTTON_0  5 //botão A - GPIO 5
#define BUTTON_1  6 //botão B - GPIO 6

//prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

//variáveis globais 
PIO pio;
uint sm;
//variáveis globais para interrupcao e deboucing
static volatile uint numero_atual = 0; //variavel volátil que pode ser alterada - interrupcao
static volatile uint a = 1; //vai incrementar as vezes que o botão será apertado
static volatile uint32_t last_time = 0; //armazena o último evento de temo (microssegundos)

//vetores com animação dos números
double numero_zero[25] =    {0.0, 0.0, 0.3, 0.3, 0.3,
                             0.3, 0.0, 0.3, 0.0, 0.0, 
                             0.0, 0.0, 0.3, 0.0, 0.3,
                             0.3, 0.0, 0.3, 0.0, 0.0,
                             0.0, 0.0, 0.3, 0.3, 0.3};

double numero_um[25] =      {0.0, 0.0, 0.0, 0.0, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.0, 0.0, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.3};

double numero_dois[25] =    {0.0, 0.0, 0.3, 0.3, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.5, 0.3, 0.3,
                             0.0, 0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.3, 0.3, 0.3};  

double numero_tres[25] =    {0.0, 0.0, 0.3, 0.3, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.3, 0.3, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.3, 0.3, 0.3};  

double numero_quatro[25] =  {0.0, 0.0, 0.3, 0.0, 0.3,
                             0.3, 0.0, 0.3, 0.0, 0.0, 
                             0.0, 0.0, 0.3, 0.3, 0.3,
                             0.3, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.3}; 

double numero_cinco[25] =  {0.0, 0.0, 0.3, 0.3, 0.3,
                            0.0, 0.0, 0.3, 0.0, 0.0, 
                            0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.3, 0.3, 0.3}; 

double numero_seis[25] =   {0.0, 0.0, 0.3, 0.3, 0.3,
                            0.0, 0.0, 0.3, 0.0, 0.0, 
                            0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.3, 0.0, 0.0,
                            0.0, 0.0, 0.3, 0.3, 0.3};
                             
double numero_sete[25] =   {0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.3, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.3,
                            0.3, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.3};

double numero_oito[25] =   {0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.3, 0.0, 0.0, 
                            0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.3, 0.0, 0.0,
                            0.0, 0.0, 0.3, 0.3, 0.3};

double numero_nove[25] =   {0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.3, 0.0, 0.0, 
                            0.0, 0.0, 0.3, 0.3, 0.3,
                            0.3, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.3, 0.3, 0.3};

// guardando todos os números em um vetor
double *numeros [10] = {
    numero_zero, numero_um, numero_dois, numero_tres, numero_quatro,
    numero_cinco, numero_seis, numero_sete, numero_oito, numero_nove
};

// rotina para LED piscar de 5 vezes por segundo
void led_red(){
    gpio_put(LED_RED, 1); //ligar LED
    sleep_ms(TEMPO); //tempo que o LED vai ficar ligado
    gpio_put(LED_RED, 0); //desligar LED
    sleep_ms(TEMPO); //tempo que o LED vai ficar desligado
};

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
        valor_led = matrix_rgb(desenho[24 - i], 0.0, desenho[24 - i]); //adiciona a cor roxa aos LEDs
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

//rotina principal
int main(){
    pio = pio0; // para carregarmos nossa PIO
    //inicializa todos os códigos stdio padrão que estão ligados ao binário
    stdio_init_all();

    //configurações da PIO
    uint offset = pio_add_program(pio, &tarefa_interrupcoes_program);
    sm = pio_claim_unused_sm(pio, true);
    tarefa_interrupcoes_program_init(pio, sm, offset, OUT_PIN);

    //inicializando LED e botões
    gpio_init(LED_RED); //inicializa o LED Vermelho
    gpio_set_dir(LED_RED, GPIO_OUT); //configura como saída    

    gpio_init(BUTTON_0); // inicializa o Button
    gpio_set_dir(BUTTON_0, GPIO_IN); //configura como entrada
    gpio_pull_up(BUTTON_0); //configura resistor pull-up para pino GPIO

    gpio_init(BUTTON_1); //inicializa o Button
    gpio_set_dir(BUTTON_1, GPIO_IN); //configura como entrada
    gpio_pull_up(BUTTON_1); //configura resistor pull-up interno para pino GPIO
   
    //configuração de interrupção com callback para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   
    gpio_set_irq_enabled_with_callback(BUTTON_1, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    //loop para o led piscar 
    while (true){
        led_red();
    }
    
}

//rotina de interrupção 
void gpio_irq_handler(uint gpio, uint32_t events){
    //armazena o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    //será verificado se o tempo que passou foi suficiente desde o último evento 
    if (current_time - last_time > 200000){ //200ms de deboucing
        if (gpio == BUTTON_0){
            numero_atual = (numero_atual + 1) % 10; //vai acrescentar um ao número atual 
                                                    // %10 fará com que esse número fique entre 0 e 9
            printf("Número Alterado! A = %d\n", a); 
            a++; //incrementa o contador de pressionamentos de botão
        }

        if (gpio == BUTTON_1){
            numero_atual = (numero_atual - 1  + 10) % 10; //vai diminuir um número do número atual e somar com 
                                                        //10 para que ele não fique negativo 
                                                        // %10 fará com que esse número fique entre 0 e 9
            printf("Número Alterado! A = %d\n", a); //
            a++; //incrementa o contador de pressionamentos de botão
        }
        last_time = current_time; //atualizar o tempo
    }
    //atualiza a matriz com o novo número e implementa as cores dos LEDs
    desenho_pio(numeros[numero_atual]);
}