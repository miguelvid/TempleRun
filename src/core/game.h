#ifndef GAME_H
#define GAME_H

void gameInit(void);
void gameUpdate(void);
void gameRender(void);

void gameOnKeyDown(unsigned char key);
void gameOnKeyUp(unsigned char key);
void gameOnSpecialDown(int key);
void gameOnSpecialUp(int key);
void gameOnMouseClick(int button, int state, int x, int y);

#endif
