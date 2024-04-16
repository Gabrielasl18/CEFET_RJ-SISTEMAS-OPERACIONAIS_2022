Paginação

Paginação: Mecanismo de Swapping

Em casos onde tenha alta utilizacao da memoria e a gente tenha pouca memoria, a gente pode usar a segunda parte do disco secundario como extencao da memoria.
Mas perde no desempenho

Toda vez que o programa vai acessar pag na memoria uma das coisas pra se ter é um bit de presença (NA ENTRADA) pra indicar se a pag tá na memória ou nao
No riscv nao existe, ele fica como inválido, daí na hora de tratar vai ver que precisa trazer do armazendo secundario pra memoria

Se nao tiver a pagina vem do disco pra memoria 

mas tem uma questao

se na hora de trazer n tiver espaco fisico pra armazenar a pagina, precisa remover uma pagina pra colocar a outra, entao precisa fazer a remocao
politica de remocao de cache - pra usar quando tira da memoria fisica para o disco

Esse mecanismo: copia uma parte dos dados e já vai executando e depois vai puxando do disco. traz as páginas sob demanda

