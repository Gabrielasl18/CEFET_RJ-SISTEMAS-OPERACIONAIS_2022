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












