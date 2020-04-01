# Trabalho prático 1 (Programação)

# Simulação da Propagação de Vírus


# Notas:

- O modelo descrito não pretende siumular de forma precisa a propagação do COVID-19. É apenas um enunciado do trabalho prático de Programação.
- O enunciado é possivelmente vago, genérico e incompleto em alguns pontos. O que se pretende é que os alunos avaliem as opções existentes e escolham a que considerarem mais apropriada para cada uma das situações com que se deparem. Todas as escolhas devem ser referidas e justificadas no relatório.
- O programa deve ser implementado em C standard, não deve ter instruções que o tornem específico para um determinado ambiente/plataforma de desenvolvimento. Deverá ser respeitada a norma C99.
 - O programa entregue deve ter uma interface simples e amigável, indicando o que pode ser feito em cada situação. Não são valorizados programas com interfaces gráficos.
 - Deve distribuir o código fonte por vários ficheiros. Para além do ficheiro com código disponibilizado com este enunciado, deverão existir, no mínimo, outros dois ficheiros com código fonte.
 - Deverá utilizar header files para gerir as dependências entre os vários ficheiros com código fonte.
 - Todos os ficheiros devem ter a identificação do aluno (nome completo e número), em comentário, nas linhas iniciais dos ficheiros.

---

# 1. Introdução

  Pretende-se que seja implementado um programa que simule, de forma simplificada, a propagação de um vírus entre uma população. Os indivíduos da população encontram-se num espaço constituído por locais, interligados entre si. A execução do programa consiste em 2 fases sequenciais:

 - Preparação: No início da execução, a aplicação carrega todos os dados necessários a partir de ficheiros: Configuração do espaço e dimensão e características da população inicial.
 - Simulação: Quando termina a fase de preparação, inicia-se um processo de siumulação iterativo que representa a propagação do vírus ao longo do tempo (1 iteração de cada vez, correspondendo cada uma destas iterações a 1 dia). No final da simulação, o programa apresenta um resumo dos resultados e grava informação relevante em ficheiros.

---

# 2. Espaço

  O espaço em que decorre a siumulação é constituído por um conjunto de locais interligados entre si, por exemplo, várias salas de um mesmo edifício.
  Cada local tem um ID numérico positivo (único), uma capacidade máxima e algumas ligações diretas a outros locais. No máximo, cada local pode ter 3 ligações.
  Os possíveis espaços para realizar a simulação estão armazenados em ficheiros binários (1 ficheiro por espaço). Cada um destes ficheiros contém várias estruturas do tipo local, uma para cada um dos locais existentes nesse espaço:
  `typedef struct sala local, *plocal;`
  `struct sala {`
    `int id;         // id numérico do local`
    `int capacidade; // capacidade máxima`
    `int liga[3];    // id das ligações (-1 nos casos não usados)`
  `};`
  No início da execução, o utilizador indica o nome do ficheiro onde se encontra a informação do espaço a usar. Antes de se iniciar a fase de simulação, esta informação é obrigatoriamente transferida para um vetor dinâmico de estruturas.
  O programa deve validar a organização do espaço: todos os locais devem ter ids positivos únicos e as ligações devem estar corretas (por exemplo, se o espaço A tiver ligação para o espaço B, então B também deve ter ligação para A). Caso exista algum erro detetado na validação, deverá ser apresentada uma mensagem adequada e o programa termina sem iniciar a fase de simulação.
  Juntamente com o enunciado são disponibilizados 3 ficheiros binários com possíveis espaços para simulação. São apenas exemplos e durante a defesa poderão ser utilizados outros ficheiros, desde que sigam a mesma organização indicada neste enunciado.

---

# 3. Pessoas

  Existe um conjunto inicial de pessoas que deve ser distribuída pelos locais onde será realizada a simulação. Cada uma das pessoas deste conjunto inicial tem as seguintes características:
 - Identificador único alfanumérico (1 palavra)
 - Idade
 - Estado: pode ser `S`audável, `D`oente, `I`mune
 - Caso esteja doente, existe a informação sobre há quantos dias foi infetado

  Ficheiros de texto armazenam informação sobre populações de pessoas. Cada ficheiro de texto tem obrigatoriamente a informação ãrmazenada no formato descrito a seguir.
  Uma linha tem toda a informação de uma pessoa, de acordo com a seguinte ordem:
  - Identificador
  - Idade
  - Estado (S, D ou I)

  Caso o estado seja D, surge ainda um intero indicando há quantos dias foi infetado. A informação numa linha está separada por um ou mais espaços em branco. Os dados sobre as várias pessoas surgem em linhas consecutivas.
  Durante a fase de preparação, o utilizador indica o nome do ficheiro de texto onde se encontra a informação das pessoas com as quais se vai iniciar a simulação. Cada uma destas pessoas deve ser colocada num dos locais existentes. A seleção do local para cada pessoa deve ser  feita de forma aleatória, satisfazendo a restrição de capacidade. Se existirem mais pessoas do que a capacidade total do espaço, algumas destas pessoas não participam na simulação.
  Durante a execução, a informação sobre as pessoas que estão a participar na simulação está armazenada em `estruturas dinâmicas do tipo lista ligada`. Fica ao critério do aluno escolher a melhor organização para as estruturas dinâmicas, sujeita apenas às restrições do enunciado.
  O programa deve validar a informação lida do ficheiro de pessoas. Caso esteja num formato inválido, deverá ser apresentada uma mensagem adequada e o programa termina sem iniciar a fase de simulação.

---

# 4. Configuração do Modelo de Propagação do Vírus

  Para que o modelo de propagação do vírus possa ser simulado ao longo do tempo, é necessário que sejam definidos alguns parâmetros. Este modelo tem os seguintes parâmetros, cujos valores são fixos:
  - `Taxa de disseminação`: Número de pessoas que um doente infeta em cada iteração da simulação. Neste modelo, o valor corresponde a 5% do número de pessoas que se encontram no mesmo local (valor arrendondado para baixo). Por exemplo, numa sala com 75 pessoas, um doente transmite o vírus a 3 outras pessoas  em cada iteração da simulação. A pessoas afetadas são escolhidas aleatoriamente, de entre todas as que se econtram no local. Pode acontecer que o vírus seja transmitido a outra pessoa que já está doente. Neste caso, a infeção não tem qualquer efeito.
  - `Probabilidade de recuperação`: Probabilidade que um doente tem de recuperar em cada iteração da simulação. Neste modelo, a probabilidade é 1/Idade, ou seja, para um doente com 50 anos, a probabilidade de recuperar em cada iteração é de 1/50 = 0.02.
  - `Duração máxima da infeção`: Número de dias em que uma pessoa está doente. No final deste período, qualquer pessoa volta a ficar saudável. Neste modelo, o valor é de 5 + 1 dia por cada dezena de anos de vida (arredondado para baixo). Por exemplo, uma pessoa infetada com 45 anos fica curada ao fim de 9 dias.
  - `Taxa de imunidade`: Quando uma pessoa infetada fica curada (por recuperação ou por ter ultrapassado a duração máxima de infeção), existe a probabilidade de ficar imune à doença. Se isto acontecer, mesmo que volte a ser infetado no futuro, não contrai novamente a doença. Neste modelo, a taxa de imunidade é de 20%.

---

# 5. Simulação do Modelo de Propagação do Vírus

  A fase de simulação inicia-se depois de terminar a leitura de informação do espaço e do grupo de pessoas a considerar. A simulação decorre ao longo de várias iterações sucessivas, cada uma delas correspondendo a um dia. Durante a fase de simulação, a aplicação deve disponibilizar um menu com, pelo menos, as seguintes opções:
  - `Avançar 1 iteração na simulação`: a aplicação aplica o modelo de propagação à população e atualiza o estado das pessoas.
  - `Apresentar estatística`: a aplicação apresenta dados estatísticos do estado atual da simulação. Entre outros resultados, deve mostrar:
    - Distribuição das pessoas por sala
    - Taxa de saudáveis
    - Taxa de infetados
    - Taxa de imunes
    - ...
  - `Adicionar doente`: 1 nova pessoa doente é adicionada a um local. O utilizador especifica o id do local e o identificador, a idade e o número de dias de infeção da nova pessoa.
  - `Transferir pessoas`: N pessoas são movidas de um local para outro dentro do espaço, desde que estes locais tenham uma ligação direta. Os ids dos locais de origem e de destino e o valor N são indicados e o valor N são indicados pelo utilizador (por exemplo, mover 3 pessoas do local 1 para o local 3). O programa escolhe N pessoas de forma aleatória, de entre as que se encontram no local de origem.
  - `Terminar Simulação`: Ao terminar a simulação, a aplicação gera um relatório final. Fica ao critério do aluno decidir que informação deve ser incluída, mas convém que seja completa. Este relatório é guardado num ficheiro de texto com o nome `report.txt`. Além disso, a população existe na última iteração deve ser guardada num ficheiro de texto. O nome deste ficheiro é indicado pelo utilizador.
