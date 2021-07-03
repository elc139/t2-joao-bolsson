## ELC139 - Programação Paralela - Trabalho 2
**Nome** : João Víctor Bolsson Marques

## Questões Pthreads
1- 
**Particionamento**

Cada multiplicação pode ser feita em 1 thread diferente e, no final, podemos simplesmente somar todas elas.

```c
for (k = 0; k < dotdata.repeat; k++) {
    mysum = 0.0;
    for (i = start; i < end ; i++)  {
       mysum += (a[i] * b[i]);
    }
}

pthread_mutex_lock (&mutexsum);
dotdata.c += mysum;
pthread_mutex_unlock (&mutexsum);
```

Primeiro, fazemos a multiplicação entre elementos do vetor na mesma posição e colocamos o resultado dessa soma em uma variável exclusiva da thread. É importante ressaltar que cada thread irá trabalhar com uma seção específica dos vetores `a` e `b`, dessa forma não fazemos o mesmo trabalho mais que 1 vez.

Depois que a thread tiver somado multiplicado todos os elementos e somado na variável `mysum` é hora de acrescentar essa soma no vetor compartilhado entre as threads `dotdata`.

**Comunicação**

O cálculo da multiplicação precisa acessar os vetores `a` e `b`, e o cálculo das somas precisa do resultado das multiplicações.

Para fazer as multiplicações as threads precisão ler os vetores compartilhados `a` e `b`. Para somar as multiplicações cada thread tem uma variável exclusiva e não precisa ser compartilhada. Por fim, para somar todas as somas as thread precisarão escrever na variável compartilhada `dotdata.c`.

**Aglomeração**

```c
mysum += (a[i] * b[i]);
```

A estratégia aqui é agrupar várias somas, cada uma representando uma parcela dos vetores `a` e `b` e, depois disso, gerar as somas finais.

**Mapeamento**

Cada processador irá trabalhar com uma thread envolvendo o mesmo número de elementos e o mesmo tipo de operação. Balanceamento estático.

---
2-  Speedup = 1.8 (Tseq/ Tpar)

---
3- 

Cada linha da tabela foi executa 5 vezes para fazer uma amostragem mínima. O tempo é a média dessas execuções. Número máximo de threads igual à 2 devido ao número de núcleos disponíveis na máquina.

| Threads        | Elementos           | Repetições  | Tempo (ms) |
| ------------- |:-------------:| -----:| -----:|
| 1     | 1000000 | 2000 | 7472247 | 
| 1     | 2000000 | 2000 | 14724030 | 
| 1     | 3000000 | 2000 | 22141300 | 
| 1     | 4000000 | 2000 | 28713101 | 
| 2     | 1000000 | 2000 | 7539176 | 
| 2     | 2000000 | 2000 | 15563823 | 
| 2     | 3000000 | 2000 | 22910443 | 
| 2     | 4000000 | 2000 | 31922923 | 

---
4-
| Elementos            | Sem Threads (s) | Com Threads (s)| Aceleração |
| ------------- |:-------------:| -----:| -----:|
| 1000000     | 7472247 | 7539176 | 0,99 | 
| 2000000     | 14724030 | 15563823 | 0,94 | 
| 3000000     | 22141300 | 22910443 | 0,96 | 
| 4000000     | 28713101 | 31922923 | 0,89 | 


---

5- A principal diferença é a existência das linhas que contêm `pthread_mutex_lock` e `pthread_mutex_unlock`. Sem essas linhas o programa NÃO está correto, pois significa que 2 ou mais threads porem acessar "simultaneamente" a variável `dotdata.c`. Logo, em algum ponto o programa irá ler/gravar um valor errado com base em uma leitura de um valor errado anteriormente.

```c
pthread_mutex_lock (&mutexsum);
dotdata.c += mysum;
pthread_mutex_unlock (&mutexsum);
```

## Questões OpenMP

Foram usadas apenas 2 threads novamente, alterando o número de elementos nos casos e verificando, posteriormente, a aceleração, que se demonstrou melhor que no caso usando `pthreads`.

| Threads        | Elementos           | Repetições  | Tempo (ms) |
| ------------- |:-------------:| -----:| -----:|
| 1     | 1000000 | 2000 | 13430179 | 
| 1     | 2000000 | 2000 | 24525170 | 
| 1     | 3000000 | 2000 | 37364830 | 
| 1     | 4000000 | 2000 | 58817090 | 
| 2     | 1000000 | 2000 | 13348144 | 
| 2     | 2000000 | 2000 | 23243782 | 
| 2     | 3000000 | 2000 | 42245883 | 
| 2     | 4000000 | 2000 | 51351424 | 

---

| Elementos            | 1 Thread | 2 Threads |Aceleração |
| ------------- |:-------------:| -----:| -----:|
| 1000000     | 13430179 | 13348144 | 1,01 | 
| 2000000     | 24525170 | 23243782 | 1,05 | 
| 3000000     | 37364830 | 42245883 | 0,88 | 
| 4000000     | 58817090 | 51351424 | 1,08 | 
