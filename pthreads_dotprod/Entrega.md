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

2-  Speedup = 1.8