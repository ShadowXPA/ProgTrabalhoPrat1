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
  `typedef struct sala local, *plocal;
  struct sala {
    int id;         // id numérico do local
    int capacidade; // capacidade máxima
    int liga[3];    // id das ligações (-1 nos casos não usados)
  };`
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
