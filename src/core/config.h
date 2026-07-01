#ifndef CONFIG_H
#define CONFIG_H

// ----- Pistas (eixo X) -----
#define NUM_LANES 3
#define LANE_WIDTH 2.0f
// converte o índice da pista (0=esquerda, 1=meio, 2=direita) no X do seu centro
#define laneToX(lane) (((lane) - 1) * LANE_WIDTH)

// ----- Jogador / pulo / agachar -----
#define EYE_HEIGHT 1.6f             // altura da câmera de pé
#define EYE_HEIGHT_DUCK 0.8f        // altura da câmera agachado
#define GRAVITY 0.018f
#define JUMP_SPEED 0.32f
#define JUMP_CLEARANCE 1.1f         // altura mínima do jogador para limpar um obstáculo baixo
#define LANE_LERP 0.25f             // suavização do movimento lateral
#define CROUCH_DOWN_RATE 0.45f      // rapidez com que o slide desce
#define CROUCH_UP_RATE 0.20f        // rapidez com que ele levanta
#define LOOK_LEGS_RATE 0.25f        // suavização do "olhar para as pernas" da câmera

// ----- Velocidade do corredor (os obstáculos vêm em direção à câmera) -----
#define INITIAL_SPEED 0.18f
#define MAX_SPEED 0.55f
#define SPEED_GAIN_PER_DIST 0.0006f // quanto a velocidade sobe por unidade de distância percorrida

// ----- Tempo por quadro (passo fixo) -----
#define MS_PER_FRAME 16.0f          // passo fixo da simulação (um gameUpdate)
#define MAX_FRAME_MS 250.0f         // teto de tempo acumulado por tick (anti espiral-da-morte)

// ----- Agachar (slide) -----
#define MAX_DUCK_TIME 500.0f        // tempo máximo agachado antes de levantar (ms)

// ----- Pontuação -----
#define SCORE_PASS 1                // pontos por ultrapassar um obstáculo em outra pista
#define SCORE_BONUS 5               // pontos por limpar um na sua pista (pulou por cima / agachou embaixo)
#define BONUS_POPUP_MS 700.0f       // por quanto tempo o texto "BONUS +5" fica na tela (ms)

// ----- Fim de jogo -----
#define GAME_OVER_DELAY 500.0f     // atraso (ms) entre a batida e a tela de game over

// ----- "Toin" da batida (tocado por cima do hitting-wall) -----
#define TOIN_PITCH 1.6f             // velocidade de reprodução (>1 = mais rápido e curto)
#define TOIN_SKIP_MS 0.0f           // corta estes ms do início do clipe

// ----- Mundo -----
#define SPAWN_DIST -70.0f           // Z onde os obstáculos nascem (bem longe)
#define Z_REMOVE 4.0f               // Z onde são removidos (atrás da câmera)
#define Z_PLAYER 2.5f               // Z do jogador (colide quando o bloco chega ao rosto)
#define COLLISION_WINDOW 0.01f      // meia-largura em Z da faixa de colisão do jogador (varrida)
#define MAX_OBSTACLES 32

// ----- Ajuste da câmera / sprites -----
#define CAM_DIP_SLIDE 0.40f         // câmera olha para baixo durante o slide
#define CAM_DIP_JUMP 0.9f           // câmera olha para baixo (para as pernas) durante o pulo
#define CAM_ROLL_SLIDE 0.14f        // inclinação lateral da câmera durante o slide (~8°)
#define LANE_ROLL 0.10f             // câmera inclina para dentro da troca de pista (rad por unidade de deslocamento lateral)
#define LANE_LOOK 0.09f             // câmera olha levemente para a pista para a qual está indo
#define LEGS_JUMP_DROP 0.22f        // quanto o sprite das pernas de pulo desce (fração da altura da tela)
#define ARMS_DROP_SLIDE 60.0f       // quanto o sprite dos braços desce durante o slide (px)
#define ARMS_DROP_JUMP 60.0f        // quanto o sprite dos braços desce durante o pulo (px)
#define FONT_REF_HEIGHT 756.0f      // altura de janela de referência para escalar a fonte

// ----- Popup de ação (sprite estilo quadrinho no pulo/slide) -----
#define POPUP_MS 450.0f             // por quanto tempo o popup fica na tela (ms)
#define POPUP_FRAC 0.20f            // tamanho do popup como fração da menor dimensão da tela

#endif
