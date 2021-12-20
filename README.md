# Multiplicador de matrizes com _threads_

## Sobre

Trabalho prático realizado para a primeira unidade da disciplina de Programação Concorrente (DIM0124), ministrada pelo Prof. Dr. Everton Ranielly de Sousa Cavalcante, do curso do BTI da UFRN. 2021.2.

Consiste em implementar a solução sequencial e concorrente do algoritmo de multiplicação de matrizes, executá-las com diferentes dimensões de matrizes e comparar seus respectivos desempenhos ao final dos testes.

## Instruções

Primeiramente, abra o terminal e insira o comando abaixo para clonar o repositório:

```bash
git clone https://github.com/arturo32/Matrizes-threads.git
```

Em seguida, dentro do diretório do projeto clonado, digite:
```bash
$ make
```

Para executar o programa, você deve inserir o comando seguindo o formato a seguir:

```bash
$ ./main 4 S
```

Em que o primeiro argumento é um número que representa a dimensão das matrizes quadradas que serão tratadas pelo programa (potências de 2 entre 4 e 2048) e o segundo argumento é um caracter (S ou C) que define qual algoritmo será utilizado para multiplicar as matrizes, o sequencial ou o concorrente.

Assim que o programa finalizar sua execução, a matriz de ordem `i` resultante  da operação poderá ser encontrada no arquivo `/matrizes_resultantes/Cixi.txt`. Além disso, o tempo em milissegundos que essa operação levou para ser finalizada será anexada ao arquivo `/resultados/X_ixi.csv`, em que `X` pode ser `S` ou `C` dependendo da solução foi utilizada para calcular a multiplicação.

Caso deseje executar a bateria de testes completa do algoritmo sequencial, insira o seguinte comando no terminal:

```bash
$ ./script_matrizes_seq.sh
```

Para realizar os testes utilizando o algoritmo concorrente, insira:

```bash
$ ./script_matrizes_conc.sh
```

Por fim, para apagar o arquivo executável, digite o comando:

```bash
$ make clean
```

# Autoria
Programa desenvolvido por **Arturo Fonseca de Souza** e **Diego Filgueiras Balderrama**.