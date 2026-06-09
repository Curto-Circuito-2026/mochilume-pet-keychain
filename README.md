# Chaveiro Pet Mochilume

Um console portátil baseado no ESP32 que funciona como um chaveiro interativo de pet virtual. O dispositivo combina rastreamento de atividade física no mundo real, multiplayer local sem fio e sincronização em nuvem para criar uma experiência completa de jogo em hardware e software.

## Especificações de Hardware

* Microcontrolador: ESP32
* Tela: GC9A01 (LCD TFT Redonda)
* Rastreamento de Movimento: MPU6050 (Acelerômetro/Giroscópio)
* Comunicação: Transceptor LoRa (Primária), Wi-Fi (Secundária)
* Entrada: Layout de 6 botões (cruz direcional de 4 vias + botões de ação A e B)

## Recursos Principais

### O Pet Virtual Mochilume
A aplicação principal executada no console é um simulador de pet virtual. Seu Mochilume cresce e sobe de nível com base na sua atividade física. O sensor MPU6050 integrado atua como um pedômetro, rastreando seus passos e convertendo-os em pontos de experiência (XP) para o seu pet.

### Batalhas via LoRa e Protocolo Customizado
Os jogadores podem batalhar contra outros dispositivos Mochilume localmente usando comunicação LoRa.
* Sistema de Combate: Cada pet pode equipar até 4 habilidades diferentes para usar durante o combate.
* Protocolo de Confiabilidade Customizado: O sistema de batalha opera sobre um protocolo LoRa desenvolvido sob medida, que apresenta um sistema rigoroso de ACK (confirmação). Isso confirma o envio e recebimento de pacotes, prevenindo perda de dados e garantindo uma jogabilidade baseada em turnos estável e sincronizada.

### Sincronização em Nuvem
Enquanto o LoRa lida com a comunicação ponto a ponto entre dispositivos, o Wi-Fi integrado do ESP32 conecta-se à internet para fazer backup e sincronizar os dados do jogador. O progresso do jogo, as estatísticas do pet e os níveis são salvos de forma segura em nossa API customizada no backend, garantindo que seu pet nunca seja perdido.

## Arquitetura de Software

* Algoritmo de Pedômetro: Processa dados brutos do MPU6050, filtrando o ruído do hardware para registrar com precisão os passos do usuário.
* Motor Gráfico: Renderização de interface customizada mapeada para a tela redonda GC9A01, gerenciando menus principais, animações do pet e a interface de batalha.
* Máquina de Estados: Gerencia transições fluidas entre o modo inativo (contagem de passos), modo de batalha (LoRa ativo) e modo de sincronização (Wi-Fi ativo).
* Gerenciamento de Rede: Lida com os estados TX/RX do LoRa e estruturação de payload, juntamente com o cliente HTTP para comunicação com a API REST.

## Configuração e Instalação

1. Clone este repositório.
2. Abra o projeto no seu ambiente de desenvolvimento preferido (PlatformIO é altamente recomendado).
3. Certifique-se de que todas as bibliotecas de dependência para o ESP32, MPU6050, tela GC9A01 e módulo LoRa estejam instaladas.
4. Atualize o arquivo de configuração de rede com as credenciais do seu Wi-Fi local e os endpoints da API.
5. Compile o código e faça o flash para o ESP32.
