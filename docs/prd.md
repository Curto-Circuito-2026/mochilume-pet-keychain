# Product Requirements Document (PRD) - Console portátil LoraMesh

## 🎯 1. Visão Geral
* **Objetivo:** Criar um console portátil de baixo custo e alto valor de durabilidade que utiliza tecnologia LoraMesh para comunicação entre os consoles, um giroscópio com acelerômetro para fazer o rastreio da movimentação (contando com contagem de passos). Tais tecnologias irão ser utilizadas dentro do console para proporcionar interações únicas. A prior o console contará com um aplicativo inicial com o nome de Mochilume, um pet virtual que poderá interagir com outros na rede LoraMesh, te recompensando a medida da distancia percorrida com seu pet. 

* **Público-alvo:**  Adolecentes a Adultos (12 - 35 anos)

## 🛠️ 2. Stack Tecnológica (Single Source of Truth)
* **Firmware:** [C++ com Arduino]
* **Hardware:** [ESP32, MPU-6050, GC9A01]

## ✅ 3. Estado Atual do Sistema (Features Implementadas)
*(A IA deve atualizar esta lista sempre que terminar um ciclo).*
* [x] **Setup Inicial:** Configuração do repositório e linting.

## 🚀 4. Backlog Imediato (Próximas Implementações)
1. **[Criação do esquemático schech]:** Criar um esquemático da parte elétrica do projeto, organizando componentes.

2. **[Criação de um esquemático PCB]:** Criar um esquemático da parte elétrica já organizado para prototipação em PCB (utilizando Prototipadora).

3. **[Confecção da PCB]:** Fabricar a PCB posicionando e soldando os componentes na PCB.

4. **[Teste da PCB]:** Testar funcionalidades alinhando com propostas fornecidas, garantindo a comunicação back, front e hardware.

5. **[Criação do encapsulamento]:** Fabricar encapsulamento para comportar o circuito.

6. **[Teste Projeto final]:** Testar o console com o aplicativo ja implementado testando comunicações LoraMesh e APIs.



## 📜 5. Regras de Ouro (Constraints)
* Utilização do KiCad para fabricação de esquemáticos de skechs e PCBs, visando pela boa organização dos componentes, evitando jumpers e possiveis zonas de calor. 
* **NUNCA** apague funcionalidades existentes para criar uma nova sem permissão explícita.
