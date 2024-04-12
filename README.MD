Visão geral

Nesta tarefa, você implementará um interpretador de linha de comando (CLI) ou, como é mais conhecido, um shell. O shell deve operar desta maneira básica: quando você digita um comando (em resposta ao seu prompt), o shell cria um processo filho que executa o comando que você digitou e então solicita mais informações do usuário quando terminar.

Os shells que você implementar serão semelhantes, mas mais simples do que aquele que você executa todos os dias no Unix. Se você não sabe qual shell está executando, provavelmente é o bash. Uma coisa que você deve fazer no seu tempo livre é aprender mais sobre seu shell, lendo as páginas de manual ou outros materiais online.

_______________________

Especificações do programa
Shell Básico: desejo

Seu shell básico, chamado wish (abreviação de Wisconsin Shell, naturalmente), é basicamente um loop interativo: ele imprime repetidamente um prompt wish> (observe o espaço após o sinal de maior que), analisa a entrada, executa o comando especificado naquele linha de entrada e aguarda a conclusão do comando. Isso é repetido até que o usuário digite exit. O nome do seu executável final deve ser desejado.

O shell pode ser invocado sem argumentos ou com um único argumento; qualquer outra coisa é um erro. Aqui está a maneira sem argumentos:

prompt> ./desejo
desejo>

Neste ponto, o desejo está em execução e pronto para aceitar comandos. Digite agora!

O modo acima é chamado de modo interativo e permite ao usuário digitar comandos diretamente. O shell também suporta um modo em lote, que lê a entrada de um arquivo em lote e executa comandos a partir dele. Veja como você executa o shell com um arquivo em lote chamado batch.txt:

prompt> ./wish batch.txt

Uma diferença entre os modos batch e interativo: no modo interativo, um prompt é impresso (wish> ). No modo em lote, nenhum prompt deve ser impresso.

Você deve estruturar seu shell de forma que ele crie um processo para cada novo comando (a exceção são os comandos integrados, discutidos abaixo). Seu shell básico deve ser capaz de analisar um comando e executar o programa correspondente ao comando. Por exemplo, se o usuário digitar ls -la /tmp, seu shell deverá executar o programa /bin/ls com os argumentos fornecidos -la e /tmp (como o shell sabe executar /bin/ls? É algo chamado shell caminho; mais sobre isso abaixo).


_______________________

Estrutura
Concha Básica

O shell é muito simples (conceitualmente): ele é executado em um loop while, solicitando repetidamente uma entrada para informar qual comando executar. Em seguida, ele executa esse comando. O loop continua indefinidamente, até que o usuário digite o comando interno exit, momento em que ele termina. É isso!

Para ler linhas de entrada, você deve usar getline(). Isso permite que você obtenha linhas de entrada longas e arbitrárias com facilidade. Geralmente, o shell será executado em modo interativo, onde o usuário digita um comando (um de cada vez) e o shell atua sobre ele. No entanto, seu shell também suportará o modo batch, no qual o shell recebe um arquivo de entrada de comandos; neste caso, o shell não deve ler a entrada do usuário (de stdin), mas sim deste arquivo para executar os comandos.

Em qualquer modo, se você atingir o marcador de fim de arquivo (EOF), deverá chamar exit(0) e sair normalmente.

Para analisar a linha de entrada em partes constituintes, você pode usar strsep(). Leia a página de manual (com atenção) para obter mais detalhes.

Para executar comandos, consulte fork(), exec() e wait()/waitpid(). Consulte as páginas de manual para essas funções e leia também o capítulo do livro relevante para uma breve visão geral.

Você notará que há vários comandos na família executiva; para este projeto, você deve usar execv. Você não deve usar a chamada de função da biblioteca system() para executar um comando. Lembre-se que se execv() for bem-sucedido, ele não retornará; se retornar, houve um erro (por exemplo, o comando não existe). A parte mais desafiadora é especificar corretamente os argumentos.

_______

paths

Em nosso exemplo acima, o usuário digitou ls mas o shell sabia que deveria executar o programa /bin/ls. Como seu shell sabe disso?

Acontece que o usuário deve especificar uma variável de caminho para descrever o conjunto de diretórios para procurar executáveis; o conjunto de diretórios que compõem o caminho às vezes é chamado de caminho de pesquisa do shell. A variável path contém a lista de todos os diretórios a serem pesquisados, em ordem, quando o usuário digita um comando.

Importante: Observe que o próprio shell não implementa ls ou outros comandos (exceto comandos internos). Tudo o que ele faz é encontrar esses executáveis ​​em um dos diretórios especificados pelo caminho e criar um novo processo para executá-los.

Para verificar se um arquivo específico existe em um diretório e é executável, considere a chamada de sistema access(). Por exemplo, quando o usuário digita ls e o caminho está definido para incluir /bin e /usr/bin, tente access("/bin/ls", X_OK). Se isso falhar, tente "/usr/bin/ls". Se isso também falhar, é um erro.

Seu caminho inicial do shell deve conter um diretório: /bin

Nota: A maioria dos shells permite especificar um binário especificamente sem usar um caminho de pesquisa, usando caminhos absolutos ou relativos. Por exemplo, um usuário pode digitar o caminho absoluto /bin/ls e executar o binário ls sem a necessidade de um caminho de pesquisa. Um usuário também pode especificar um caminho relativo que começa com o diretório de trabalho atual e especifica o executável diretamente, por exemplo, ./main. Neste projeto, você não precisa se preocupar com esses recursos.