# Revisão para o documento final

### State of the art
 - Melhorar explicação da EtherCAT
 (explicação MUITO profunda acerca de TUDO)

### Soluções propostas

- Explicar melhor o conceito: não focar tanto na parte técnica
e.g. "O que pode acontecer ao usar uma rede na malha de controlo"

# Início dos trabalhos - 15-02-2021

- Criar um guião sobre o deve ser feito com o demonstrador e quais os
resultados que devem ser esperados, quer eles sejam positivos ou
negativos.

- Arranjar um mecanismo de, sintéticamente, levar a rede ao simite:
  - simular congestionamento na rede (p.ep alteração de tempos de ciclo
controlo);
  - simular dessincronização (como???)
  - simular falhas de rede (como???)
  - ...

# Semana 2 - 22/02/2021

## Discussão
- O demonstrador é para ser **genérico** e **não** focado em EtherCAT !!!
  - Assumindo que EtherCAT é a melhor rede de tempo real, vamos usá-la
  por permitir sintéticamente demonstrar redes mais "fracas" (o
  contrário não é verdade).

## Tarefas
- Reformular ideias para apenas modificar, sintéticamente, o tempo de ciclo da rede.
  - Esquecer por completo a "injeção de falhas"
  - Permitir que o estudante altere manualmente o tempo de ciclo da rede
  - **Abstrair por completo o programa de controlo***. Não está no contexto das redes
  de comunicação de tempo real.

- Pensar em cenários apropriados para demonstrar _apenas_ tempo de ciclo da rede.

- Definir bem o que controlar a nível físico:
  - Controlo de velocidade ?
  - Controlo de posição ?
  - Será possível ter ambos ?

- Estruturar o programa de controlo.

# Semana 2 - 24/02/2021

## Discussão
- Impacto do tempo de ciclo da rede (genérica).

- Esquecer planos para braço robótico.

- Esquecer Arduinos e passar para os slaves para Raspberry Pi com shields da
 Hilscher (NetHAT 52-RTE).

- Arranjar shield para controlo TOTAL de motor para Raspberry Pi: tipo servo-motor.

### Disco com 1 perfuração, led continuamente ligado, fotorrecetor do lado oposto:
Manter LED constantemente ligado, controlar velocidade do motor em modo constante,
calcular janela temporal em que o fotorrecetor irá estar ativo (de acordo com
o tamanho da ranhura e a velocidade de rotação). Ter o slave a ler continuamente
o estado do recetor (a um período suficientemente elevado) e enviar esse estado
para o Master. Este limita-se a controlar o tempo de ciclo da rede EtherCAT e a
copiar para uma saída o valor da entrada que recebeu (idealmente o mais rápido
possível). O slave recebe este valor da saída e verifica se o estado do recetor
é igual ao da saída:
  - Se fôr, o deadline de entrega das mensagens cumpre a especificação necessária.
  - Senão, o tempo de ciclo da rede é demasiado baixo para cumprir os requisitos.

#### Modificação para possível controlo de atuação:
Colocar LED a piscar a frequencia constante e sincronizar o(s) disco(s)
de modo a manter o feixe de lux a atingir o fotorrecetor.

# Semana 3 - 01/03/2021

## Trabalho realizado
- Pesquisa de controlador de motor DC com encoder para Raspberry Pi
- Estudo de como implementar uma tarefa de tempo real em Linux, usando a API
genérica do kernel.

## Discussão
- Como implementar o slave?
  - Usar Raspberry Pi com placa DFRobot DFR0592
  - Motor diz funcionar confortávelmente entre os 3-9V, embora seja de 6V nominal.
  - Esquecer arduino
## Tarefas
- Verificar o que é o "MegaIO" e se é relevante
- Juntar execução RT e a API da Hilscher para ter uma tarefa de tempo real a
comunicar com o CODESYS
- Verificar o funcionamento da comunicação I2C no raspberry, necessária para
comunicar com o módulo de controlo do motor.
