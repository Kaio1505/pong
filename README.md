# pong

RA:11201810069

Jogo do pong feito com o opengl


MODO DE JOGO

Este é um classico jogo, aqui você possui duas plataformas que são responsáveis por impedir que a bolinha saia da tela.

Cada plataforma é operada de uma forma, a primeira da direita é movimentada com as setas do teclado UP e DOWN, já a da esquerda é movimentada com as teclas W e S do teclado.

O jogo Pong foi implmentado utilizando o opengl, neste caso temos muitos reaproveitamentos do jogo asteroids, sendo ele a nave ships que se tornou nossa plataforma.

Ships.cpp

Esta classe é responsável por instanciar as plataformas no jogo, elas servem para para rebater a bola, ao ser criada todas possuem 4 pontos, possuem uma escala e ao apertar o botão UP ou W do teclado elas irão deslocar-se para cima, de maneira analoga ao apertar S ou DOWN a plataforma irá se deslocar para baixo.

balls.cpp

Esta classe é herdada do jogo asteroids, ela instância uma bola e é responsável por controlar a rebatida da bola nos eixos y.

window.cpp

Esta classe é a que coordena todo o jogo ela possui um contador de pontos para cada jogador, dessa forma sempre que um jogador faz um ponto é incrementado no placar dele um ponto,a rebatida também é controlada por essa classe, que observa se a bola está dentro do raio esperado e da distancia esperada para rebater a bola, ao sair do mapa 

![image](https://user-images.githubusercontent.com/61333997/198894460-cb631729-16a8-4d7b-9e18-a9fbf8eb702b.png)
