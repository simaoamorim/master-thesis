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
