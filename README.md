# Projeto de Algoritmos Evolutivos

O projeto de algoritmos evolutivos implementado é a inteligência artificial do personagem de um jogo. Neste jogo, o personagem é uma bolinha verde que deve coletar bolinhas amarelas para ganhar pontos, desviando dos círculos azuis.


## Instruções de Instalação

Executar os comandos abaixo para instalar a biblioteca gráfica ![Allegro](https://github.com/liballeg/allegro_wiki/wiki).

```
sudo add-apt-repository ppa:allegro/5.2
sudo apt-get update
sudo apt-get install liballegro5-dev
```

## Instruções de compilação e execução

Para instalar, apenas rode o makefile com
```make```

Para executar, você tem três opções:

- **Jogar**

    Se você quiser apenas jogar o jogo, rode `./main game`. 

- **Evoluir**

    Se você quiser evoluir a população utilizando o algoritmo genético, rode `./main evolve`

    *Opções*

    Se você quiser definir o tamanho da população que vai evoluir, adicione a flag `--n=<tamanho_populacao>`. Por exemplo, `--n=10`.
    
    Se você quiser ver a população evoluindo, adicione a flag `--render`

    Se você quiser salvar o resultado da evolução em um arquivo, adicione a flag `--to=<nome_arquivo>`, exemplo: `--to=evolucao.out`

    Se você quiser retomar a evolução a partir de um arquivo salvo, adicione a flag `--from=<nome_arquivo>`. Exemplo: `--from=evolucao.in`

- **Simular**

    Se você quiser ver o resultado de uma evolução, rode `./main simulate --from=<nome_arquivo> --render`.
    Isso irá rodar o jogo com o melhor indivíduo dessa evolução. Se você não determinar um arquivo, ele será um indivíduo aleatório.

    Caso queira mostrar mais indivíduos da população, adicione a flag `--n=<numero_individuos>`. Exemplo: caso você quiser ver os 10 melhores indivíduos, rode `./main simulate --from=arquivo.in --n=10 --render`.
    

