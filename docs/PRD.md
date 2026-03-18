# Product Requirements Document (PRD) - Console portátil LoraMesh

## 🎯 1. Visão Geral
* **Objetivo:** Criar um console portátil de baixo consumo de batéria que utiliza a tecnologia LoraMesh para se conectar entre si e um giroscópio com acelerômetro para rastrear movimentação e quantidade de passos. Essas tecnólogias podem ser utilizadas por aplicações dentro do console para criar interações únicas. O console vai contar com um aplicativo inicial chamado Mochilumes, que é um pet virtual que pode interagir com outros pets na rede LoraMesh, e te recompensa baseado em quantos passos você da com ele.
* **Público-alvo:** Jovens adultos (12 - 25 anos)

## 🛠️ 2. Stack Tecnológica (Single Source of Truth)
* **Firmware:** [C++ com Arduino]
* **Hardware:** [ESP32, MPU-6050, GC9A01]

## ✅ 3. Estado Atual do Sistema (Features Implementadas)
*(A IA deve atualizar esta lista sempre que terminar um ciclo).*
* [x] **Setup Inicial:** Configuração do repositório e linting.

## 🚀 4. Backlog Imediato (Próximas Implementações)
1. **[Criação de um framework gráfico]:** Framework para facilitar criação de telas e componentes interativos dentro das telas, como botões e caixas de texto. Também facilitará a navegação.
2. **[API de Comunicação LoRaMesh]:** Funções para conectar, visualizar, enviar e receber mensagens dentro da rede Lora de forma fácil para ser acessada pelas aplicações.
3. **[API de Giroscópio e Acelerômetro]:** Funções para interagir com o sensor MPU, que fornece dados de aceleração e rotação, criar abstrações para facilitar o uso como "detectar shake" e "quantidade de passos".
4. **[API de Dados/Memória]:** Funções para salvar e ler arquivos dentro da memória (cartão SD) do console.
5. **[Implementação de Menu e Configurações]:** Criar telas bases do console, menu para listar aplicações e configurações do usuário, como o nome.
6. **[Criação da base da aplicação Mochilume]:** Criar aplicativo do Mochilume, com o pet virtual que interage a movimentação.
7. **[Sistema de batalhas do Mochilume]:** Criar o sistema de batalhas e recompensas utilizando a rede Lora.

## 📜 5. Regras de Ouro (Constraints)
* Use **C++**. Se atenha a boas práticas de memória (Desinicialize objetos quando forem removidos) e somente utilize as bibliotecas incluidas no projeto e as STD libs.
* Padrão de commits: **Conventional Commits** (feat:, fix:, chore:).
* Estilo de código limpo (Clean Code) e modular.
* **NUNCA** apague funcionalidades existentes para criar uma nova sem permissão explícita.
