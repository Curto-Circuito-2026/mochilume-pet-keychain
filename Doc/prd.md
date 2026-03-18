# Product Requirements Document (PRD) - Console portátil LoraMesh

## 🎯 1. Visão Geral
* **Objetivo:** Criar um console portátil com a disponibilidade de um jogo interno que utiliza a tecnologia LoraMesh/P2P para a comunicação entre consoles, possuindo um acelerômetro e um giroscópio para a captação de passos. A crianção do jogo será baseado em Pokémon Go e Tamagochi, sendo um pet de batalha virtual.

* **Público-alvo:**  Jovens Adultos (10 - 25)

## 🛠️ 2. Stack Tecnológica (Single Source of Truth)
* **Firmware:** [C++ com Arduino]
* **Hardware:** [ESP32, MPU-6050, GC9A01]

## ✅ 3. Estado Atual do Sistema (Features Implementadas)
*(A IA deve atualizar esta lista sempre que terminar um ciclo).*
* [x] **Setup Inicial:** Configuração do repositório e linting.

## 🚀 4. Backlog Imediato (Próximas Implementações)
1. **[Criação do esquemático de Hardware]:** Criar um esquemático da junção das partes físicas, fazendo um esquemático para PCB.

2. **[Teste da Placa]:** Testamento do esquemático.

3. **[Criação da placa PCB]:** Criar a placa PCB em uma prototipadora.

4. **[Montagem da PCB]:** Montar a parte física do projeto na placa

5. **[Criação do encapsulamento]:** Fabricar encapsulamento para comportar o circuito.

6. **[Teste Projeto final]:** Testar o console com o aplicativo ja implementado testando comunicações LoraMesh e APIs.



## 📜 5. Regras de Ouro (Constraints)
* Utilização do KiCad para fabricação de esquemáticos de skechs e PCBs, visando pela boa organização dos componentes, evitando jumpers e possiveis zonas de calor. 
* **NUNCA** apague funcionalidades existentes para criar uma nova sem permissão explícita.
