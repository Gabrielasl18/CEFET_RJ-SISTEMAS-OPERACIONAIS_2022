Paginação

Páginas de memória são unidades de alocação de memória utilizadas pelo sistema operacional para gerenciar o espaço de memória virtual dos processos em execução. Elas não são exclusivamente trechos de código, mas sim blocos de dados que podem conter código, dados ou uma combinação de ambos.

Quando um programa é executado, ele é carregado na memória principal em forma de páginas. Essas páginas podem conter tanto o código do programa quanto os dados necessários para sua execução. Por exemplo, uma página pode conter instruções de código executáveis, variáveis, pilha de chamadas e outras estruturas de dados.

Quando o sistema operacional precisa de mais memória para alocar novos processos ou para lidar com a demanda de memória dos processos existentes, ele pode mover algumas páginas menos utilizadas da memória principal para o disco rígido, liberando espaço na memória principal. Essa operação é parte do mecanismo de paginação que você mencionou anteriormente.

Portanto, enquanto páginas de memória podem conter trechos de código, elas também podem conter dados e outras informações necessárias para a execução dos programas. Elas são uma maneira eficiente de gerenciar a memória em sistemas operacionais modernos.

Paginação: Mecanismo de Swapping

Em casos onde tenha alta utilizacao da memoria e a gente tenha pouca memoria, a gente pode usar a segunda parte do disco secundario como extencao da memoria.
Mas perde no desempenho

Toda vez que o programa vai acessar pag na memoria uma das coisas pra se ter é um bit de presença (NA ENTRADA pelo PTE) pra indicar se a pag tá na memória ou nao
No riscv nao existe, ele fica como inválido, daí na hora de tratar vai ver que precisa trazer do armazendo secundario pra memoria

1: tá
0: não tá 

Se nao tiver a pagina vem do disco pra memoria 

mas tem uma questao

se na hora de trazer n tiver espaco fisico pra armazenar a pagina, precisa remover uma pagina pra colocar a outra, entao precisa fazer a remocao
politica de remocao de cache - pra usar quando tira da memoria fisica para o disco

Esse mecanismo: copia uma parte dos dados e já vai executando e depois vai puxando do disco. traz as páginas sob demanda

Area de swap space nunca guarda código, pois nunca é modificado, os dados também, vai no programa original. Mas se forem dados dinamicos que foram criados no momento de execução do programa, entao se usa.

Qual escolher pagina da memoria

grande trafego de operacao de e/s (trafego de swap) 
essa escolha é feita com politica de pagina


> tirar da memoria fisica para secundaria? determinar politica

- algoritmo ótimo

não da pra implementar

o acesso a pagina gerou a falta de pagina

nesse momento existem varias pages na memoria fisica, a outra vai ser acessada em seguiad e as outras mais la pra frente

nesse algoritmo tiram da memoria fisica a pagina que vai levar mais instrucoes pra chegar nela (pra gerar uma pagina nova), ou seja, atrasam ao maximo uma falta de página 

isso não é viável, porque pra cada page vc associa a ela a quant de algoritmos que vc usa pra chegar na próxima, vc nao sabe quantas vc vai ter que usar pra chegar naquela

* Algoritmo FIFO (First in, First out)

Fácil de implementar, mas nem sempre uma boa ideia, nesse caso escolhe a página que está há mais tempo na memória física.
Mas às vezes o FIFO se comporta melhor do que alguns algoritmos mais complexos, depende da situação

* Algoritmo aleatório

às vezes muito bom, às vezes ruim, o único que salvou foi ele (além do algoritmo ótimo)


* Algoritmo LRU

o problema é implementar ele de maneira eficiente

teria que criar uma lista/conjunto de quadros sempre ordenados pelos menos recentemente acessados, com suporte de hardware seria evitado, mas o SO ainda teria que encontrar a entrada com maior tempo

manter uma lista de paginas ordenadas da menos frequentemente utilizadas para a mais frequentemente utilizadas. estamos olhando pra trás agora e nao pra frente como o algoritmo otimo


a prova vai até escalonamento ULFQ

Escalonamento

Preciso escolher algum processo pra executar, existe uma lista de processos prontos para executar, mas tem só 1 processador, sempre que tenho que fazer essa escolha, aciono o escalonador e o escalonador implementa alguma políticas

> Políticas

Métricas

Para comparar políticas Métricas 

* Turnaround = Tfim - Tchegada

Tempo de execução + temmpo de espera

os algoritmos de escalonamento quer diminuir o turnaround time, pra minimizar o tempo de espera dos processos, porque consigo executar mais coisas

# poítica FIFO 

Não foi tão ruim. Supõe que chega ao mesmo tempo

* Turnaround = Tfim 

o processo que chegou primeiro, vai ser o primeiro atendido
tem um problema, que é do comboio, uma coisa muito grande na frente atrasando os pequenos de trás

resolver o problema do FIFO

# Shortest Job First (SJF)

* Turnaround = Tfim - Tchegada

sei o tamanho de cada processo, vc pega o que leva menos tempo primeiro. Vai pegar sempre o que é o menor, independente de quem é o primeiro
Desde que todos cheguem ao mesmo tempo, se nao chegarem, não é essa política que minimiza o turnaround time

(cai na prova)

problemas:

agora processos podem chegar a qualquer momento

o processo executa até o final, mas pode chegar um menor que ele, ele vai esperar o maior terminar pra ele executar


agora processos podem chegar a qualquer momento e agora processos podem ser preemptado, pode sofrer preempcao, pra resolver isso usamos a política

# Shortest Time-To-Completion First (STJF)

* Turnaround = Tfim - Tchegada

o processo pode ser interrompido, ou seja, preempcao 
vai escolher o que falta menos tempo pra terminar

Métrica: Tempo de Resposta

É dado pelo tempo onde o programa é executado pela primeira vez - o tempo de chegada, ou seja, quanto tempo demoorou ate que o processo conseguiu executar e exibiu pra vc


# Round Robin (RR)

escolhe os processos, faz a escolha com base no FIFO, mas é preemptivo, ou seja, todos eles tem o mesmo tempo pra executar

todos tem a oportunidade de executar um pouco

o tempo de resposta vai cair, acabando com o problema do STJF, tendo um tempo de resposta menor

se escolher uma fatia grande, vai partir pro FIFO, se for muito pequeno, vai trocar processos o tempo todo, ou seja, a cache do processador, o TLB, tudo isso vao se tornar um problema. ou seja, tem que achar uma fatia pra nao virar um fifo ou causar tanta troca de contexto.

ele posterga o tempo de fim, aumentando o tornaround time

Incluindo E/S

Política

# Multi-Level Feedback Queue (MLFQ)

política muito boa pro tornaround time e outras pro tempo de espera, mas queremos que seja boa pros 2, mas q no tornaround time se aproxime do STJF

parte da premissao que todos sao iguais

coloca todo mundo numa fila baseada no RR, baseado FIFO, quem terminar na fatia de tempo, é pequeno, quem nao terminar é grande, esses que nao terminarem, vao pra outra fila

varias filas e elas representam prioridades e prefiro pegar processos da filas de cima, cada fila usa a política RR (tentando minimizar o tempo de resposta), conforme ele nao vai terminando, vai ficando menos priortário e de tempos em tempos eles voltam pra fila de maior prioridade de novo

Como implementar?

tentativa 1: como alterar prioridades

Processos interativos geralmente requerem interação frequente com o usuário e têm tempos de resposta curtos. Eles são sensíveis ao tempo de espera e geralmente são colocados em filas de prioridade mais altas.

Por outro lado, processos CPU-bound (processos de uso intensivo de CPU) geralmente não exigem interação frequente com o usuário e podem realizar longas tarefas de computação. Eles são menos sensíveis ao tempo de espera e, portanto, podem ser colocados em filas de prioridade mais baixas.

Ao alterar as prioridades dos processos com base em seu comportamento (interativo vs. CPU-bound), o sistema pode adaptar dinamicamente o escalonamento para atender às necessidades dos diferentes tipos de processos e otimizar o desempenho geral do sistema. Isso significa que os processos interativos têm prioridade para serem executados mais rapidamente, enquanto os processos CPU-bound podem ter uma prioridade mais baixa para não monopolizar os recursos do sistema.

MLFQ têm 3 problemas

