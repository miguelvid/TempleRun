#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "game.h"
#include "audio.h"
#include "config.h"

int g_posicao_x = 50;
int g_posicao_y = 50;
int g_largura = 1248;
int g_altura = 756;
char g_titulo[] = "Temple Run 3D";
int g_idle = 0;
int g_timer = 1;
int g_timer_value = 5;   // de quanto em quanto tempo amostramos o tempo real (ms); a
                         // simulação ainda avança em passos de MS_PER_FRAME

void gDesenha (void) {
    gameRender ();
    glutSwapBuffers ();
}

void gRedimensiona (GLsizei largura, GLsizei altura) {
    if (altura == 0) altura = 1; // Evita a divisão por 0

    glViewport (0, 0, largura, altura); // Especifica as dimensões da viewport

    glMatrixMode (GL_PROJECTION); // Inicializa o sistema de coordenadas
    glLoadIdentity ();            // Inicializa a matriz de transformação corrente
    gluPerspective (70.0, (double) largura / (double) altura, 0.1, 200.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

void gTeclado (unsigned char tecla, int x, int y) {
    (void) x; (void) y;
    gameOnKeyDown (tecla);
}

void gTecladoLiberado (unsigned char tecla, int x, int y) {
    (void) x; (void) y;
    gameOnKeyUp (tecla);
}

void gEspeciais (int tecla, int x, int y) {
    (void) x; (void) y;
    gameOnSpecialDown (tecla);
}

void gEspeciaisLiberado (int tecla, int x, int y) {
    (void) x; (void) y;
    gameOnSpecialUp (tecla);
}

void gMouse (int botao, int estado, int x, int y) {
    gameOnMouseClick (botao, estado, x, y);
}

void gMousePressionado (int x, int y) {;}
void gMouseLiberado (int x, int y) {;}
void gMouseScroll (int botao, int direcao, int x, int y) {;}

void gSistemaOcioso (void) {;}

void gTempoExecucao (int valor) {
    (void) valor;

    // Passo fixo com acumulador: avança a simulação em passos fixos de
    // MS_PER_FRAME conforme o tempo real decorrido, para que o jogo se comporte
    // igual independentemente de quando (ou de forma quão irregular) este timer dispara.
    static int ultimo_tempo = -1;
    static float acumulador = 0.0f;

    int agora = glutGet (GLUT_ELAPSED_TIME);
    if (ultimo_tempo < 0) {
        ultimo_tempo = agora;   // primeiro tick: ainda não há tempo decorrido
    }
    acumulador += (float) (agora - ultimo_tempo);
    ultimo_tempo = agora;

    // limita para evitar a espiral da morte após um travamento (ex.: janela perdeu o foco)
    if (acumulador > MAX_FRAME_MS) {
        acumulador = MAX_FRAME_MS;
    }

    while (acumulador >= MS_PER_FRAME) {
        gameUpdate ();
        acumulador -= MS_PER_FRAME;
    }

    glutPostRedisplay ();
    glutTimerFunc (g_timer_value, gTempoExecucao, 1);
}

void gMenuTeclado (void) {;}
void gMenuJanela (void) {;}

void gInicializa (void) {
    GLfloat corNeblina[4] = {0.05f, 0.05f, 0.08f, 1.0f};

    glClearColor (0.05f, 0.05f, 0.08f, 1.0f);
    glEnable (GL_DEPTH_TEST); // Habilita buffer de profundidade

    // neblina: dá profundidade ao corredor e suaviza como os obstáculos surgem
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, corNeblina);
    glFogf (GL_FOG_START, 20.0f);
    glFogf (GL_FOG_END, 70.0f);

    audioInit ();
    atexit (audioShutdown);   // encerra o motor de áudio de forma limpa na saída
    gameInit ();
}

int main (int argc, char *argv[]) {

    // Instruções de inicialização da utilização da api

    glutInit (&argc, argv); // Inicialização da GLUT
    // buffer duplo para construção de imagem
    // remoção de superfícies escondidas e sistema cor
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    // posição da imagem - canto superior esquerdo (x,y)
    glutInitWindowPosition (g_posicao_x, g_posicao_y);
    // definição da largura e altura da janela
    glutInitWindowSize (g_largura, g_altura);
    // título e criação da janela
    glutCreateWindow (g_titulo);

    // Declaração de callbacks

    // desenho de objetos gráficos
    glutDisplayFunc (gDesenha);
    // redimensionamento da janela de exibição
    glutReshapeFunc (gRedimensiona);
    // pressionamento de teclas comuns
    glutKeyboardFunc (gTeclado);
    // liberação de teclas comuns
    glutKeyboardUpFunc (gTecladoLiberado);
    // ignora o auto-repeat do teclado (movimento contínuo depende de press/release limpos)
    glutIgnoreKeyRepeat (1);
    // pressionamento de teclas especiais
    glutSpecialFunc (gEspeciais);
    // liberação de teclas especiais
    glutSpecialUpFunc (gEspeciaisLiberado);
    // eventos de pressionamento/liberação mouse
    glutMouseFunc (gMouse);
    // movimento do mouse enquanto pressionado
    glutMotionFunc (gMousePressionado);
    // movimento do mouse enquanto liberado
    glutPassiveMotionFunc (gMouseLiberado);
    // eventos do mouse scroll
    glutMouseWheelFunc (gMouseScroll);

    // Callbacks e Funções auxiliares
    if (g_idle) glutIdleFunc (gSistemaOcioso); // Quando da ociosidade do sistema
    if (g_timer) glutTimerFunc (g_timer_value, gTempoExecucao, 1); // Quando trabalhar com animação

    gMenuTeclado ();
    gMenuJanela ();
    gInicializa ();

    glutMainLoop ();

    return 0;
}
