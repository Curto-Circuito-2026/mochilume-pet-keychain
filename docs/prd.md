# 📄 Product Requirements Document (PRD) – Mini Console Portátil com Comunicação LoRaWAN / LoRaMesh

## 🎯 1. Visão Geral

**Objetivo:**
Desenvolver um mini console portátil de baixo custo e alto valor de durabilidade, com foco em uma experiência de jogo única, utilizando tecnologia LoRa mesh / LoRaMesh P2P para comunicação entre consoles.

O dispositivo contará com giroscópio e acelerômetro para rastreio da movimentação, incluindo contagem de passos, movimentação e rotação do dispositivo.

O console terá um jogo interno baseado em uma mistura de Pokémon GO com Tamagotchi, sendo um pet de batalha virtual onde, quanto mais o usuário se movimenta, mais forte ele fica.

A princípio, o console contará com um aplicativo inicial chamado **Mochilume**, no qual o pet virtual poderá interagir com outros dispositivos na rede LoRaMesh, permitindo batalhas e outras interações, recompensando o usuário à medida da distância percorrida com seu pet.

Ao se aproximar de outro jogador com o mesmo dispositivo, será possível iniciar interações via comunicação LoRa (mesh/P2P), incluindo batalhas, onde o vencedor será recompensado de alguma forma dentro do jogo.

**Público-alvo:**
Adolescentes, jovens e adultos, na faixa etária de 10 a 35 anos.

---

## 🛠️ 2. Stack Tecnológica (Single Source of Truth)

**Firmware:**
C++ com framework Arduino

**Hardware:**
ESP32  
Tela GC9A01  
MPU6050 / MPU-6050  

---

## ✅ 3. Estado Atual do Sistema (Features Implementadas)

[x] Setup inicial: Configuração do repositório e linting

(A IA deve atualizar esta lista sempre que terminar um ciclo)

---

## 🚀 4. Backlog Imediato (Próximas Implementações)

1. Montar protótipo de hardware  
Desenvolver o hardware inicial para possibilitar o desenvolvimento do firmware  

2. Criação do esquemático de hardware  
Criar um esquemático da parte elétrica do projeto, organizando componentes e visando a criação da PCB  

3. Criação de esquemático PCB  
Organizar o esquemático para prototipação em PCB utilizando prototipadora  

4. Teste da placa / esquemático  
Validação das funcionalidades e alinhamento com as propostas do projeto  

5. Criação da placa PCB  
Fabricar a PCB posicionando e soldando os componentes  

6. Montagem da PCB  
Montar a parte física do projeto na placa  

7. Criar framework gráfico  
Criar uma base reutilizável para facilitar a implementação de interfaces  

8. Atualização fixa dos dados dos sensores  
Criar uma interrupção com intervalo fixo para atualizar os sensores  

9. Integrar sensores e inputs  
Conectar sensores e entradas físicas às suas respectivas funcionalidades  

10. Desenvolver interfaces  
Projetar e implementar todas as interfaces do sistema  

11. Integrar comunicação LoRa  
Implementar comunicação entre dispositivos utilizando LoRaMesh/P2P  

12. Desenvolver case 3D / encapsulamento  
Criar e fabricar o encapsulamento da PCB  

13. Fabricar PCB final  
Produzir a versão final com hardware e firmware validados  

14. Teste do projeto final  
Testar o console com o aplicativo já implementado, incluindo comunicação LoRaMesh, APIs e integração entre hardware e software  

---

## 📜 5. Regras de Ouro (Constraints)

Utilizar C++ com Arduino  
Seguir boas práticas de gerenciamento de memória  
Utilizar apenas bibliotecas já importadas no projeto  
Ao alterar a base do projeto, atualizar o arquivo `platformio.ini`  
Padrão de commits: Conventional Commits (feat:, fix:, chore:)  
Manter código limpo (Clean Code) e modular  

Utilização do KiCad para fabricação de esquemáticos e PCBs, visando boa organização dos componentes, evitando jumpers e possíveis zonas de calor  

NUNCA remover ou apagar funcionalidades existentes sem permissão explícita