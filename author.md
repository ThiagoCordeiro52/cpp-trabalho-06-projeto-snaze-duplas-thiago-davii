# Identificação Pessoal

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: *José Davi Viana Francelino*
- Email: *jose.davi@imd.ufrn.br*
- Turma: *DIM0120 - T01*

- Nome: *Thiago de Oliveira Cordeiro*
- Email: *thiagoty52@gmail.com*
- Turma: *DIM0120 - T01*

# Indique quais itens da avaliação foram realizados

- [ ] Valida corretamente arquivo de entrada (20 pontos)

- [ ] Simulador apenas sugere direções randômicas para cobra (10 pontos)
OU
- [ ] Simulador direciona a cobra corretamente para a comida (50 pontos)

- [ ] Apresentar e manipula corretamente os estados da simulação (30 pontos)

- [ ] **Extra**: simulação gera o menor caminho
- [ ] **Extra**: 

# Compilando e executando

## Manualmente com g++

Para compilar e executar o programa manualmente basta digitar os seguintes comandos em seu terminal:

```
$ git clone git@github.com:selan-active-classes/trabalho-06-projeto-snaze-duplas-thiago-davii.git (faz o clone do repositório em algum diretório do seu pc)
$ cd trabalho-06-projeto-snaze-duplas-thiago-davii/ (vai até a pasta do repositório clonado)
$ mkdir build (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ g++ -Wall -std=c++17 -g source/src/main.cpp source/src/snake.cpp source/src/snaze_manager.cpp -I source/include -o build/snaze(compilar)
$ ./build/snaze [<options>] <input_level_file> (executar)
```

## Cmake

Para compilar e executar o programa com o Cmake você precisa ter instalado pelo menos a versão 3.8. Em seguida, abra seu terminal e execute os seguintes comandos:
```
$ git clone git@github.com:selan-active-classes/trabalho-06-projeto-snaze-duplas-thiago-davii.git (faz o clone do repositório em algum diretório do seu pc)
$ cd trabalho-06-projeto-snaze-duplas-thiago-davii/ (vai até a pasta do repositório clonado)
$ mkdir build (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ cmake -S source -B build (criar o cmake do projeto)
$ cmake --build build (compilar)
$ ./snaze [<options>] <input_level_file> (executar)
```

--------
&copy; DIMAp/UFRN 2021.
