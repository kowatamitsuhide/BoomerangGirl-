#include"EnemyManeger.h"
#include"EnemyFactory/EnemyBase/EnemyBase.h"
#include <fstream>
#include"../EffectManager/EffectManager.h"
#include "../Data/WordsTable.h"
#include"../TimeManager/TimeManager.h"
#include"../SceneManager/SceneManager.h"
#include"../ResouceManager/ResouceManager.h"

EnemyManager::EnemyManager()
{
	_enemy = {};
	
}

EnemyManager::~EnemyManager()
{
	for (auto& enemy : _enemy) {
		 delete enemy;
	}
}

bool EnemyManager::Initialize()
{
	explode = ResouceManager::Instance().LoadSoundFile(_T("Audio/SoundEffect/explosion.wav"));

	//!�Ǎ��O�̓G�̏�����
	for (int i = 0; i < ENEMY_NUM; ++i) {
		appear_pos [i] = Vector3_Zero;
		tag[i]         = INT_MAX;
		appear_time[i] = INT_MAX;
		appear_flag[i] = false;
	}

	explode->SetVolume(0.9f);
	LoadCSV();
	return true;
}

int EnemyManager::Update(PlayerManager* playermanager)
{
	_playermanager = playermanager;

	if(_frame < GAME_FRAME)
	   _frame++;
	else {
		_frame = 0;
		_time++;
	}

	if (_time > appear_time[count] && count < ENEMY_NUM) {
			Generate(_playermanager);
			count++;
	}

	auto itr = _enemy.begin();
	while (itr != _enemy.end()) {
		//!Update��return���ꂽ�l LIVING�E�E�����Ă� AUTODEAD�E�E�����폜�@DEATH�E�E����
			if ((*itr)->Update(_playermanager) == LIVING && (*itr)->AutoDead() == LIVING)
				itr++;
			else
			{
				//!�v���C���[�̃u�[�������œG�����S�����ꍇ�̏���
				if ((*itr)->Update(_playermanager) == DEATH) {
					EffectManager::Instance().Create(EffectTag::SMALLEXPLOSION, (*itr)->GetPosition());
					TimeManager::Instance().AddTime(ENEMYADDTIME);
					SceneManager::Instance().AddDeathEnemy();
					explode->Play();
				}
				//!�����폜�̏ꍇ�̏���
				else
					EffectManager::Instance().Create(EffectTag::EXPLOSION, (*itr)->GetPosition());
				if ((*itr)->AutoDead() != LIVING) {itr = _enemy.erase(itr); continue;}
				itr = _enemy.erase(itr);
			}
	}

	return 0;
}

void EnemyManager::Draw()
{
	for (auto& enemy : _enemy) {
		enemy->Draw();
	}
}

/**
 * @fn �G�̐����̖���
 * @brief 
 * @param (PlayerManager* player_manager) �v���C���[�̍��W
 * @return �Ȃ�
 * @detail �ǂݍ��񂾎�ނ̓G�̐�����EnemyFactory��Create�֐��ɖ���
 */

void EnemyManager::Generate(PlayerManager* player_manager)
{
	unique_ptr<EnemyFactory> factory = std::make_unique<EnemyFactory>();

	if (!appear_flag[count])
	{
		//!�G�̎�ށE�������W��n���ēG�𐻑�
		_enemy.push_back(factory->Create(tag[count], appear_pos[count], player_manager));
		appear_flag[count] = true;
	}

}

void EnemyManager::OnCollisionEnter(EnemyBase* enemy)
{
	enemy->Damage();
	EffectManager::Instance().Create(EffectTag::HIT, _playermanager->GetBoomerang().GetCollision()->GetPosition());
}

void EnemyManager::LoadCSV() {
	std::ifstream pos_time_infile("csvFile/Enemy/EnemyPosition_AppearTime.txt");

	std::string dummy_line;
	
	//! 1�`3�s��ǂݔ�΂�
	for (int i = 0; i < DUMMYLINENUM; i++) {
		getline(pos_time_infile, dummy_line);
	}
	
	//!�f�[�^�ǂݍ���
	for (int i = 0; i < ENEMY_NUM; ++i) {
		pos_time_infile >> tag[i] >> appear_pos[i].x >> appear_pos[i].y >> appear_pos[i].z >> appear_time[i];
	}
}