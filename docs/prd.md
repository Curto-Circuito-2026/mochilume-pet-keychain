# 📄 Product Requirements Document (PRD) – Mini Console Portátil com Comunicação LoRaWAN

## 🎯 1. Visão Geral

* **Objetivo:**
  Desenvolver um mini console portátil com foco em uma experiência de jogo única.
  O jogo será uma mistura de *Pokémon GO* com *Tamagotchi*, onde quanto mais o usuário se movimenta, mais forte ele fica.

  O console contará com comunicação **LoRa mesh**, permitindo que, ao se aproximar de outro jogador com o mesmo dispositivo, seja possível iniciar batalhas. O vencedor será recompensado de alguma forma dentro do jogo.

  Além disso, o console terá **giroscópio** e **acelerômetro** para contabilizar:

  * Número de passos
  * Movimentação e rotação do dispositivo

* **Público-alvo:**
  Jovens de **12 a 18 anos**

---

## 🛠️ 2. Stack Tecnológica (Single Source of Truth)

* **Firmware:**
  C++ com framework Arduino

* **Hardware:**

  * ESP32
  * Tela GC9A01
  * MPU6050

---

## ✅ 3. Estado Atual do Sistema (Features Implementadas)

* [x] **Setup inicial:** Configuração do repositório e linting

---

## 🚀 4. Backlog Imediato (Próximas Implementações)

1. **Montar protótipo de hardware**
   Desenvolver o hardware inicial para possibilitar o desenvolvimento do firmware

2. **Criar framework gráfico**
   Criar uma base reutilizável para facilitar a implementação de interfaces

3. **Atualização fixa dos dados dos sensores**
   Criar uma interrupção com intervalo fixo para atualizar os sensores

4. **Desenvolver interfaces**
   Projetar e implementar todas as interfaces do sistema

5. **Integrar sensores e inputs**
   Conectar sensores e entradas físicas às suas respectivas funcionalidades

6. **Integrar comunicação LoRa**
   Implementar comunicação entre dispositivos

7. **Desenvolver case 3D**
   Criar o encapsulamento da PCB

8. **Fabricar PCB final**
   Produzir a versão final com hardware e firmware validados

---

## 📜 5. Regras de Ouro (Constraints)

* Utilizar **C++**
* Seguir boas práticas de **gerenciamento de memória**
* Utilizar apenas bibliotecas já importadas no projeto
* Ao alterar a base do projeto, atualizar o arquivo `platformio.ini`
* Padrão de commits: **Conventional Commits** (`feat:`, `fix:`, `chore:`)
* Manter código limpo (**Clean Code**) e modular
* **NUNCA** remover funcionalidades existentes sem permissão explícita
