/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h> /* fork() */


int ehPrimo(unsigned int i);

int main() {
  unsigned int i, j;
  int pidaux = 0;
  pid_t *pids = malloc(3*sizeof(pid_t));

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  //Criando um contador visivel para todos os processos
  int *c = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  *c = 0;

  unsigned int *num = (unsigned int*) mmap(NULL, 100 * sizeof(unsigned int), protection, visibility, 0, 0); // vetor em que serão guardadas as entradas
  int *numc = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0); // contador de num
  *numc = 0;

  while (scanf("%ud ", &i) != EOF) {
      num[(*numc)++] = i;
  }
  int *numt = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0); // tamanho de num
  *numt = *numc;
  *numc = 0;



  while (*numc<*numt) {
    if ((pids[0] != 0) && (pids[1] != 0) && (pids[2] != 0)) { // Se o vetor de pids está cheio
      pidaux = waitpid(-1, NULL, 0);
    } else {
      pidaux = 0;
    }

    //Procurando qual posição de pid possui pidaux (0 ou o último pid liberado)
    if(pids[0] == pidaux)
      pidaux = 0;
    else if(pids[1] == pidaux)
      pidaux = 1;
    else if(pids[2] == pidaux)
      pidaux = 2;
    pids[pidaux] = fork();
    if(pids[pidaux] == 0) {
      *c += ehPrimo(num[(*numc)++]);
      exit(0);
    }

  }
  for (j=0; j<3; j++) {
    waitpid(pids[j], NULL, 0);
  }

  printf("%d\n", *c);

  return 0;
}

int ehPrimo(unsigned int i) {
  unsigned int j;
  if (i==0 || i==1)
    return 0;
  for(j=2;j<i;j++) {
    if(!(i%j))
      return 0;
  }
  return 1;
}
