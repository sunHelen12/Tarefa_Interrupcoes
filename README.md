<h1 align="center">Tarefa Interrupções-Embarcatech 💻</h1>

<p>Este projeto implementa uma matriz de LEDs controlada por uma Raspberry Pi Pico utilizando a PIO (Programmable Input/Output) e interrupções por botões. 
O sistema permite a exibição de números de 0 a 9 em uma matriz de 5x5 LEDs WS2812B (endereçáveis). Além disso, um LED RGB pisca cinco vezes por segundo como 
indicador de funcionamento.</p>
<p>Os números exibidos na matriz podem ser alterados através dos botões conectados aos pinos GPIO,um botão incrementa e outro decrementa. 
Há também um sistema de debounce para evitar leituras errôneas.</p>

<h2>Importante⚠️</h2>
<p>Esse código foi produzido em um Sistema Operacional Linux. Certifique-se de importar o projeto adequadamente e verificar as configurações do <i>c_cpp_properties.json</i>.</p>

<h2>Componentes e Funcionamento</h2>

<ul>
  <li>Placa BitDogLab: todos os componentes pertencem a placa;</li>
  <li>Matriz 5x5 de LEDs WS2812B: representa os números de 0 à 9;</li>
  <li>Botão A: incrementa números;</li>
  <li>Botão B: decrementa números;</li>
  <li>LED RGB: pisca 5 vezes por segundo.</li>
</ul>

<h2>Tecnologias Utilizadas</h2>

<ul>
  <li>Linguagem C;</li>
  <li>IDE Visual Studio Code;</li>
  <li>Pico SDK;</li>
  <li>Placa BitDogLab;</li>
</ul>

<h2>Vídeo</h2>
<p>Vídeo no Drive: https://drive.google.com/file/d/1eHIh1nfqicTTv6n3bG0wVoVCTjvF96Yg/view?usp=sharing</p>
<p>Vídeo no YouTube: https://youtu.be/7FVu9zKdY_A</p>

<h2>Demonstração</h2>

<img src="assets/1.gif" width="500" height="300">
<p><i>Incrementação - Botão A</i></p>

<img src="assets/2.gif" width="500" height="300">
<p><i>Decrementação - Botão B</i></p>

<img src="assets/3.gif" width="500" height="300">
<p><i>LED RGB cor vermelho</i></p>

  <h2>Como Executar o Projeto</h2>
        <h2>Clone o Repositório</h2>
    <ol>
        <li>Abra o <strong>Prompt de Comando</strong> ou o terminal de sua preferência.</li>
        <li>Clone o repositório usando o Git:
            <pre><code>git clone https://github.com/seu-usuario/seu-repositorio.git</code></pre>
        </li>
        <li>Entre no diretório do projeto:
            <pre><code>cd seu-repositorio</code></pre>
        </li>
    </ol>
 <h2>Configuração do Ambiente Local</h2>
    <ol>
        <li>Baixe e instale o <a href="https://cmake.org/download/" target="_blank">CMake</a>.</li>
        <li>Configure o <strong>Pico SDK</strong> seguindo o guia oficial em 
            <a href="https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf" target="_blank">
                Raspberry Pi Pico SDK</a>.
        </li>
        <li>Crie um diretório de construção:
            <pre><code>mkdir build</code></pre>
            <pre><code>cd build</code></pre>
        </li>
        <li>Execute o CMake para gerar os arquivos de construção:
            <pre><code>cmake ..</code></pre>
        </li>
    </ol>
