#include "input.h"
#include "myDirect3D.h"
#include "sprite.h"
#include "texture.h"
#include "main.h"
#include "ui.h"
#include "scene.h"
#include "weapon.h"
#include "enemyweapon.h"

#define TIME_UI_SIZE_X		(600)	//����?UI��X�T�C�Y
#define TIME_UI_SIZE_Y		(600)	//����?UI��Y�T�C�Y
#define ATTACK_UI_SIZE_X	(600)	//�U���w����UI��X�T�C�Y
#define ATTACK_UI_SIZE_Y	(600)	//�U���w����UI��Y�T�C�Y
#define DEFENSIVE_UI_SIZE_X	(600)	//�K�[�hUI��X�T�C�Y
#define DEFENSIVE_UI_SIZE_Y	(600)	//�K�[�hUI��Y�T�C�Y
#define HEALTH_UI_SIZE_X	(600)		//�̗�UI��X�T�C�Y
#define HEALTH_UI_SIZE_Y	(600)	//�̗�UI��Y�T�C�Y
#define TIME_FIGURE			(2)		//�^�C�}�[�̌���
#define SCORE_FIGURE		(1)		//�X�R�A�̌���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int g_Time;			//��������
int g_CountTime;	//���ԃJ�E���g
int g_PlayerScore;	//�v���C���[�X�R�A
int g_EnemyScore;	//�G�l�~�[�X�R�A
bool g_timerflag;
// UI�𕪊����ĕ\�����邽�߂̕ϐ�
int g_HealthTable;		//�v���C���[�̗̓Q�[�W�����ϐ�
int g_GuardTable;		//�v���C���[�K�[�h�Q�[�W�����ϐ�
static int FlameCount;
static int GuardFrameCount;
int g_EnemyAtk;
int g_PlayerDef;

typedef struct
{
	D3DXVECTOR2 pos;
}UI;

UI g_ui[5];//UI�\���̔z��


void Ui_Initialize(void)
{
	g_Time = 90;			//�^�C�}�[�̏����l
	g_CountTime = 0;		//�^�C���J�E���g
	g_PlayerScore = 0;		//�v���C���[�X�R�A�̏�����
	g_EnemyScore = 0;		//�G�l�~�[�X�R�A�̏�����
	g_timerflag = false;	//�������ԏI���t���O
	FlameCount = 0;			//�t���[���J�E���g�̏�����
	GuardFrameCount = 0;	
	g_HealthTable = 0;		
	g_GuardTable = 0;		
	g_EnemyAtk = 0;
	g_PlayerDef = 0;

	//�^�񒆏�
	g_ui[0].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f);							//����?UI�̍��W
																									//�^�񒆉�
	g_ui[1].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.70f);							//�U���w����UI�̕`��
	g_ui[2].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.735f);							//�������
	g_ui[3].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f));		//�K�[�hUI�̕\��
	g_ui[4].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.15f));		//�̗�UI�̕\��
}

void Ui_Finalize(void)
{

}

void Ui_Update(void)
{
	//���ԁA�̗́A�K�[�h�A�U����؂�ւ��鏈�������邱��
	if (g_CountTime > 60)
	{
		g_Time -= 1;
		g_CountTime = 0;
	}
	g_CountTime++;

	//�X�O�b�o������t���O��true�ɂ���
	if (g_Time == 0)
	{
		g_timerflag = true;
	}

	//�X�O�b�o�����烊�U���g�V�[���ֈړ�
	if (g_timerflag)
	{
		Scene_Change(SCENE_INDEX_RESULT);

	}

	//�̗͂�0�ȏォ��5�ȉ�
	if ((FlameCount >= 90) && (g_HealthTable > 0) && (g_HealthTable<5))
	{
		g_HealthTable -= 1;
		FlameCount = 0;
	}
	else if (FlameCount >= 90)
	{
		FlameCount = 0;
	}
	FlameCount++;

	//�K�[�h�Q�[�W��0�ȏォ��5�ȉ�
	if ((GuardFrameCount >= 120) && (g_GuardTable > 0) && (g_GuardTable<5))
	{
		g_GuardTable -= 1;
		GuardFrameCount = 0;
	}
	else if (GuardFrameCount >= 120)
	{
		GuardFrameCount = 0;
	}
	GuardFrameCount++;
}

void Ui_Draw(void)
{


	
	//���ԁH�̕`��
	Sprite_Draw(TEXTURE_INDEX_LIGHT, g_ui[0].pos.x, g_ui[0].pos.y, 
	GetLightTable()*600, 0,600,600,
	TIME_UI_SIZE_X * 0.5f, TIME_UI_SIZE_Y *0.5f, 1.0f, 1.0f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, GetAtkAlpha()));

	//�G�̍U�������̕`��
	Sprite_Draw(TEXTURE_INDEX_ATK, g_ui[1].pos.x, g_ui[1].pos.y,
		g_EnemyAtk *600, 0, 600, 600,
		ATTACK_UI_SIZE_X*0.5f, ATTACK_UI_SIZE_Y*0.5f,0.7f, 0.7f, 0.0f);
	
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//�G�̃K�[�h�����̕`��
	Sprite_Draw(TEXTURE_INDEX_DEF, g_ui[2].pos.x, g_ui[2].pos.y,
		g_PlayerDef*600, 0, 600, 600,
		ATTACK_UI_SIZE_X*0.5f, ATTACK_UI_SIZE_Y*0.5f,0.7f, 0.7f, 0.0f);
	
	//�K�[�h�Q�[�W�̕`��
	Sprite_Draw(TEXTURE_INDEX_GUARD, g_ui[3].pos.x, g_ui[3].pos.y,
		g_GuardTable*DEFENSIVE_UI_SIZE_X, 0, 600, 600,
		600*0.5, 600*0.5, 1.0f, 1.0f, 0.0f);
	

	//�̗̓Q�[�W�̕`��
	Sprite_Draw(TEXTURE_INDEX_HP, g_ui[4].pos.x, g_ui[4].pos.y,
		g_HealthTable * HEALTH_UI_SIZE_X,0, 600, 600,
		600*0.5, 600 * 0.5, 1.0f, 1.0f, 0.0f);

}


//�������Ԃ̕`��
void DrawTime()
{

	int Time = g_Time;

	for (int i = 0; i<TIME_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Time % 10;      //��ԉE�̐��̔����o��
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Time /= 10;         //��ԉE�̐��𗎂Ƃ�


		float Sx, Sy;

		Sx = 870.0f + 80.0f * (TIME_FIGURE - 1 - i);

		Sy = 0.0f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	}
}

//�v���C���[�X�R�A�`��
void DrawPlayerScore()
{

	int Score = g_PlayerScore;

	for (int i = 0; i<SCORE_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Score % 10;      //��ԉE�̐��̔����o��
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Score /= 10;         //��ԉE�̐��𗎂Ƃ�


		float Sx, Sy;

		Sx = 700.0f + 70.0f * (SCORE_FIGURE - 1 - i);

		Sy = 8.7f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			1.0f, 1.0f, 0.8f, 0.8f, 0.0f);
	}
}

//�G�l�~�[�X�R�A�`��
void DrawEnemyScore()
{

	int Score = g_EnemyScore;

	for (int i = 0; i<SCORE_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Score % 10;      //��ԉE�̐��̔����o��
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Score /= 10;         //��ԉE�̐��𗎂Ƃ�


		float Sx, Sy;

		Sx = 1140.0f + 70.0f * (SCORE_FIGURE - 1 - i);

		Sy = 8.7f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			0.0f, 0.0f, 0.8f, 0.8f, 0.0f);
	}
}

//�v���C���[�X�R�A�����Z
void AddPlayerScore(int Score)
{
	g_PlayerScore += Score;
}

//�G�l�~�[�X�R�A�����Z
void AddEnemyScore(int Score)
{
	g_EnemyScore += Score;
}

//���݂̃v���C���[�X�R�A���擾
int GetPlayerScore()
{
	return g_PlayerScore;
}

//���݂̃G�l�~�[�X�R�A���擾
int GetEnemyScore()
{
	return g_EnemyScore;
}


// UI�������l��ς���
int AddTable(int n)
{
	if ((g_HealthTable >= 0) || (g_HealthTable<5))
	{
		g_HealthTable += n;
	}
	return g_HealthTable;
}

int GetTable()
{
	return g_HealthTable;
}

// UI�������l��ς���
int AddGuardTable(int n)
{
	if ((g_GuardTable >= 0) || (g_GuardTable<5))
	{
		g_GuardTable += n;
	}
	return g_GuardTable;
}

int GetGuardTable()
{
	return g_GuardTable;
}

void SetEnemyAtkTable(int n)
{
	g_EnemyAtk = n;
}

void SetPlayerDefTable(int n)
{
	g_PlayerDef = n;
}

