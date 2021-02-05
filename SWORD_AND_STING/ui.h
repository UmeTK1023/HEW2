#pragma once

void Ui_Initialize(void);
void Ui_Finalize(void);
void Ui_Update(void);
void Ui_Draw(void);

void DrawTime(void);

void DrawPlayerScore();
void DrawEnemyScore();

void AddPlayerScore(int Score);
void AddEnemyScore(int Score);

int GetPlayerScore();
int GetEnemyScore();
int AddTable(int n);
int GetTable();
int AddGuardTable(int n);
int GetGuardTable();
void SetEnemyAtkTable(int n);
void SetPlayerDefTable(int n);

