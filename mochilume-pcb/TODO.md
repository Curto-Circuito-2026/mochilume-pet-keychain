Após a fabricação da PCB foram encontrados alguns problemas de projeto que foram resolvidos na famosa "gambiarra", após o término do projeto deverão ser corrijidas:

MAIN PCB: 
    1 - Footprint do módulo LORA está virado de jeito que o chip fique virado para placa, talvez o ideal seja trocar de camada

    2 - Não tem necessidade dos 2 bornes de 2 vias que estão na placa, uma vez que o borne de 3 vias é o responsável por receber a alimentação

POWER PCB:
    1 - O diodo de proteção impossibita o TP4056 de reconhecer a bateria, deve ser retirado e será adicionado um suporte físico que não permita que o usuário inverta a célula de alimentação.