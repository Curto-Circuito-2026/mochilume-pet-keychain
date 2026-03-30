# 📄 Product Requirements Document (PRD) – Mini Console Portátil com Comunicação LoRaWAN / LoRaMesh

## 🎯 1. Visão Geral

**Objetivo:**
Desenvolver um mini console portátil com foco em uma experiência de jogo única, com a disponibilidade de um jogo interno que utiliza a tecnologia LoRa mesh / LoRaMesh P2P para comunicação entre consoles.

O jogo será uma mistura de Pokémon GO com Tamagotchi, sendo um pet de batalha virtual onde, quanto mais o usuário se movimenta, mais forte ele fica.

O console permitirá que, ao se aproximar de outro jogador com o mesmo dispositivo, seja possível iniciar batalhas via comunicação LoRa, recompensando o vencedor dentro do jogo.

Além disso, o dispositivo contará com giroscópio e acelerômetro para captação de passos, movimentação e rotação do dispositivo.

**Público-alvo:**
Jovens e jovens adultos, na faixa etária de 10 a 25 anos.

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
Criar um esquemático da junção das partes físicas, visando a criação da PCB  

3. Teste da placa  
Validação do esquemático desenvolvido  

4. Criação da placa PCB  
Criar a placa PCB em uma prototipadora  

5. Montagem da PCB  
Montar a parte física do projeto na placa  

6. Criar framework gráfico  
Criar uma base reutilizável para facilitar a implementação de interfaces  

7. Atualização fixa dos dados dos sensores  
Criar uma interrupção com intervalo fixo para atualizar os sensores  

8. Integrar sensores e inputs  
Conectar sensores e entradas físicas às suas respectivas funcionalidades  

9. Desenvolver interfaces  
Projetar e implementar todas as interfaces do sistema  

10. Integrar comunicação LoRa  
Implementar comunicação entre dispositivos utilizando LoRaMesh/P2P  

11. Desenvolver case 3D / encapsulamento  
Criar e fabricar o encapsulamento da PCB  

12. Fabricar PCB final  
Produzir a versão final com hardware e firmware validados  

13. Teste do projeto final  
Testar o console com o aplicativo já implementado, incluindo comunicação LoRaMesh e possíveis APIs  

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