#include "EnemyFactory.h"
#include <fstream>
#include "../../Data/MyAlgorithm.h"

EnemyFactory::EnemyFactory() {
	//!�����G�̃X�e�[�^�X�ݒ�
	LoadCSV::Instance().LoadStatus("csvFile/Enemy/EnemyStatus.csv");
	_moveenemy_speed.x = LoadCSV::Instance()._status.at("#��������(X���W)");
	_moveenemy_speed.y = LoadCSV::Instance()._status.at("#��������(Y���W)");
	_moveenemy_speed.z = LoadCSV::Instance()._status.at("#��������(Z���W)");
	_moveenemy_hp      = LoadCSV::Instance()._status.at("#�����G��HP");

	//!�~�܂��Ă���G�̃X�e�[�^�X�ݒ�
	LoadCSV::Instance().LoadStatus("csvFile/Enemy/StopEnemyStatus.csv");
	_stopenemy_speed.x = LoadCSV::Instance()._status.at("#��������(X���W)");
	_stopenemy_speed.y = LoadCSV::Instance()._status.at("#��������(Y���W)");
	_stopenemy_speed.z = LoadCSV::Instance()._status.at("#��������(Z���W)");
	_stopenemy_hp      = LoadCSV::Instance()._status.at("#�~�܂��Ă���G��HP");

	//!�G�̎�ނ̃^�O���v�b�V���o�b�N
	_enemytag.push_back("move");
	_enemytag.push_back("stop");
	_enemytag.push_back("homing");
	_enemytag.push_back("dist_homing");

	//!�G�̃��f���̂���ꏊ��string�^�ŕێ�
	_enemy_model[MOVE]        = "MODEL/Enemies/Enemy/enemy_White.X";
	_enemy_model[STOP]        = "MODEL/Enemies/StopEnemy/enemy_light blue.X";
	_enemy_model[HOMING]      = "MODEL/Enemies/HomingEnemy/enemy_triangle.X";
	_enemy_model[DIST_HOMING] = "MODEL/Enemies/DistHomingEnemy/enemy_Han_en.X";
}

EnemyFactory::~EnemyFactory() {
	
}

/**
 * @fn    
 * @brief �G�̐���
 * @param (tag) �G�̎��
 * @param (_position) �G�̏������W
 * @param (PlayerManager* player_manager) �v���C���[�N���X
 * @return EnemyBase���p�������N���X��Initialize
 * @detail �G�̃��f���E�������W�E�ړ����x�EHP�̐ݒ�
 */

EnemyBase* EnemyFactory::Create(string tag, Vector3 _position, PlayerManager* player_manager)
{
	EnemyBase* enemy_factory = CreateProduct(tag, _position);
	enemy_factory->Initialize(SetModel(tag),_position, SetSpeed(tag), SetHp(tag));
	return enemy_factory;
}

EnemyBase* EnemyFactory::CreateProduct(string tag, Vector3 _position)
{
	EnemyBase* classes[] = { new NormalEnemy,new StopEnemy,new HomingEnemy,new DistHomingEnemy};

	for (int i = 0; i < _enemytag.size(); ++i)
	{
		if (tag == _enemytag[i]) { _enemy = classes[i]; }
	}

	return _enemy;
}

string  EnemyFactory::SetModel(string tag)
{
	string models[] = { _enemy_model[MOVE],_enemy_model[STOP],_enemy_model[HOMING] ,_enemy_model[DIST_HOMING] };
	for (int i = 0; i < _enemytag.size(); ++i)
	{
		if (tag == _enemytag[i]) { _model_name = models[i]; }
	}
	return _model_name;
}

Vector3 EnemyFactory::SetSpeed(string tag)
{
	Vector3 speeds[] = { _moveenemy_speed ,_stopenemy_speed ,_moveenemy_speed ,_moveenemy_speed };
	for (int i = 0; i < _enemytag.size(); ++i)
	{
		if (tag == _enemytag[i]){ _speed = speeds[i]; }
	}
	return _speed;
}

int EnemyFactory::SetHp(string tag)
{
	int hps[] = { _moveenemy_hp,_stopenemy_hp,_moveenemy_hp,_moveenemy_hp };

	for (int i = 0; i < _enemytag.size(); ++i)
	{
		if (tag == _enemytag[i]) { _hp = hps[i]; }
	}
	return _hp;
}