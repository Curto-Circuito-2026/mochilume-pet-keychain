# 🎮 Mochilume: Portable Wireless Battle Console & OS

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://en.cppreference.com/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-An_Awesome_Framework-orange.svg)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino_ESP32-blue.svg)](https://github.com/espressif/arduino-esp32)
[![Backend](https://img.shields.io/badge/Backend-.NET_C%23-purple.svg)](https://dotnet.microsoft.com/)
[![Database](https://img.shields.io/badge/Database-PostgreSQL_%26_Supabase-green.svg)](https://supabase.com/)

O **Mochilume** é um console portátil de bolso open-hardware projetado para trazer de volta a nostalgia dos bichinhos virtuais (estilo Tamagotchi), combinando mecânicas de exploração do mundo real (estilo Pokémon GO) com batalhas multijogador locais e conectividade em nuvem. 

O ecossistema opera em uma arquitetura híbrida de rede: o progresso do jogador e os status do pet são sincronizados na nuvem via **Wi-Fi**, enquanto as batalhas de proximidade entre consoles ocorrem de forma descentralizada via rádio **LoRa**.

---

## 📸 Galeria do Projeto

### Hardware & Placas (Fabricação Caseira)
<table>
  <tr>
    <td align="center"><strong>Placa do Console</strong></td>
    <td align="center"><strong>Placa de Energia</strong></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/64ca1cf1-ec55-487e-9007-d7edbc1d7687" width="450" alt="Placa do Console" /></td>
    <td><img src="https://github.com/user-attachments/assets/37908d9f-f0de-4681-8e5d-bfc4a1f44aa2" width="450" alt="Placa de Energia" /></td>
  </tr>
</table>

---

### Interface do Usuário & Gameplay (Tela 240x240)
<table>
  <tr>
    <td align="center"><strong>Menu Principal</strong></td>
    <td align="center"><strong>Tela de Batalha (LoRa PvP)</strong></td>
    <td align="center"><strong>Tela do Mochilume (Status)</strong></td>
  </tr>
  <tr>
    <td valign="top"><img src="https://github.com/user-attachments/assets/8774848d-6147-4b6e-a2ab-a165ea4f31b2" width="280" alt="Menu Principal" /></td>
    <td valign="top"><img src="https://github.com/user-attachments/assets/f0fae588-b94c-40d9-9bb8-6b9c3ac51faf" width="280" alt="Tela de Batalha" /></td>
    <td valign="top"><img src="https://github.com/user-attachments/assets/1539a879-324c-4171-a5d8-564361e8ea49" width="280" alt="Tela do Mochilume" /></td>
  </tr>
</table>

---

---
## 🕹️ Dinâmica do Jogo & Gameplay

* **Tamagotchi + Pokémon GO:** O jogador cuida, alimenta e treina o seu "Mochilume". A evolução e os atributos do pet estão diretamente atrelados à movimentação do jogador no mundo real.
* **Mova-se para Evoluir:** Utilizando os sensores de movimento integrados, o console rastreia os passos do jogador. **Quanto mais você anda na vida real, mais forte o seu pet fica**, ganhando experiência, atributos e subindo de nível.
* **Batalhas Locais via LoRa:** Encontrou um colega que também tem um Mochilume? Os consoles se conectam ponto a ponto (P2P) via rádio de longo alcance para batalhas em tempo real, sem depender de internet, roteadores ou redes móveis.

---

## 🛠️ Engenharia de Hardware & Fabricação

O hardware foi inteiramente projetado, roteado e fabricado de forma artesanal, utilizando processos avançados de prototipagem rápida:

* **Fabricação da PCB:** As placas de circuito impresso (PCBs) foram confeccionadas manualmente utilizando o método de **termotransferência e corrosão química**, com layout em **Dual Layer (Dupla Face)** para otimização de espaço e plano de terra.
* **Módulo de Energia Independente:** Equipado com um sistema de gerenciamento de carga para **célula de lítio recarregável** integrado à placa, permitindo o uso totalmente portátil do console.
* **Interface Visual:** Display circular colorido de **240x240 pixels** (Driver GC9A01A), operando com taxa de atualização acelerada via barramento SPI de alta frequência.

---

## 🧠 Arquitetura de Firmware (Sistemas Embarcados)

O software do console foi desenvolvido em **C++** sobre o framework **PlatformIO**, focado em gerenciamento eficiente de recursos de rádio e energia:

* **Gerenciamento de Rede Híbrido:**
  * **Conectividade Wi-Fi:** Utilizada estritamente para autenticação do jogador e comunicação HTTP assíncrona com a API externa para salvar o estado do jogo.
  * **Comunicação LoRa (LLCC68):** Dedicada exclusivamente ao protocolo de rede sem fio de baixa latência para transmissão dos pacotes de dados das batalhas PvP (Player vs Player).
* **Filtro de Bateria Não-Bloqueante:** Monitoramento do divisor de tensão da bateria filtrado com **Média Móvel (100 amostras consecutivas)** e controlado por temporização com **`millis()`** encapsulado de forma estática. A leitura roda estritamente a cada 5 segundos para garantir que picos de consumo do Wi-Fi/LoRa não gerem *falsos positivos* de bateria fraca ou travem as requisições de rede.
* **Pedômetro via Hardware:** Algoritmo baseado em máquina de estados para validação de passos reais através da magnitude de aceleração tridimensional da IMU (MPU6050), armazenando os dados em buffers (*batches*) para sincronização otimizada.

---

## 🌐 Nuvem e Infraestrutura (Backend)

O Mochilume não perde o progresso caso seja desligado. Os dados persistentes são salvos globalmente através de uma infraestrutura escalável:

* **Mochilume API:** Desenvolvida em **C# (.NET Web API)** e hospedada na nuvem da **Azure**. Ela atua como um Gateway de serviços que recebe chamadas HTTP puras originadas do Wi-Fi do console. A API processa a lógica de evolução dos pets, validação de dados e pontuação global.
* **Persistência de Dados:** Banco de dados relacional **PostgreSQL** hospedado de forma isolada no **Supabase**. O esquema de tabelas (jogadores, inventário, histórico e status dos pets) é versionado e gerenciado por **Migrations** via Entity Framework Core (EF Core) a partir do projeto C#.

---

## 🚀 Como Compilar e Rodar

### Backend (API)
1. Navegue até a pasta da API em C#.
2. Configure a string de conexão do Supabase/Postgres no seu `appsettings.json`.
3. Execute as migrations e suba o servidor:
   ```bash
   dotnet ef database update
   dotnet run

### Firmware (Console)

> ⚠️ **ATENÇÃO CRÍTICA DE HARDWARE (eFuses do ESP32):**
> Como o design da PCB em Dual Layer otimizou ao máximo as trilhas manuais por corrosão, alguns pinos de boot/strapping (como o GPIO 12 / MTDI) foram utilizados para os barramentos de periféricos. 
> Antes de realizar o upload do firmware, é **obrigatório** configurar/queimar os eFuses de tensão interna do regulador flash do ESP32 para travar em 3.3V nativo e ignorar o estado do pino no boot. Caso contrário, a placa entrará em loop de *boot fail*.
>
> Para queimar o eFuse via terminal (ferramenta `esptool.py` do Espressif):
> ```bash
> espefuse.py --port COM_DO_SEU_CONSOLE set_flash_voltage 3.3V
> ```
> *Nota: Este processo é irreversível e configura o chip especificamente para o layout desta PCB.*

1. Abra a pasta `mochilume-code` no VS Code com a extensão **PlatformIO** ativa.
2. Abra o arquivo `include/HalConfig.h` (ou onde estiverem as suas constantes de hardware) e atualize a variável da URL da API com o endereço de produção gerado pela sua Azure Free Tier:
   ```cpp
   const char* API_URL = "[https://seu-app-service.azurewebsites.net/api/v1/pet](https://seu-app-service.azurewebsites.net/api/v1/pet)";

