#pragma once
#include "Character/Player.h"


class JobClass
{
public:
	JobClass() { this->owner = nullptr; }
	JobClass(Player* player) :owner(player) {}
	~JobClass() {}

	virtual void Update(float elapsedTime);
	void ChangeState(Player::State state);
	
	// �ҋ@�ƈړ��͑S�����ʁB
	// �ҋ@State
	virtual void TransitionIdleState();
	virtual void UpdateIdleState(float elapsedTime);

	// �ړ�State
	virtual void TransitionMoveState();
	virtual void UpdateMoveState(float elapsedTime);

	// �U���ƃX�L���̓W���u�ɂ���ĈقȂ邽�߁A�������z�֐��w��B�h����Ŏ�������B
	// �U���X�e�[�g
	virtual void TransitionAttackState();
	virtual void UpdateAttackState(float elapsedTime);

	// �X�L���X�e�[�g
	virtual void TransitionSkillState() = 0;
	virtual void UpdateSkillState(float elapsedTime);

protected:
	Player* owner;
};

class ArcherClass : public JobClass
{
public:
	ArcherClass(Player* player) :JobClass(player) {}

	// �U���X�e�[�g
	void TransitionAttackState()override;
	void UpdateAttackState(float elapsedTime)override;

	// �X�L���X�e�[�g
	void TransitionSkillState()override;
	void UpdateSkillState(float elapsedTime)override;
};
class MageClass : public JobClass
{
public:
	MageClass(Player* player) :JobClass(player) {}

	// �U���X�e�[�g
	void TransitionAttackState()override;
	void UpdateAttackState(float elapsedTime)override;

	// �X�L���X�e�[�g
	void TransitionSkillState()override;
	void UpdateSkillState(float elapsedTime)override;
};
class RogueClass : public JobClass
{
public:

	RogueClass(Player* player) :JobClass(player) {}

	// �U���X�e�[�g
	void TransitionAttackState()override;
	void UpdateAttackState(float elapsedTime)override;

	// �X�L���X�e�[�g
	void TransitionSkillState()override;
	void UpdateSkillState(float elapsedTime)override;
};
class TankClass : public JobClass
{
public:

	TankClass(Player* player) :JobClass(player) {}

	// �U���X�e�[�g
	void TransitionAttackState()override;
	void UpdateAttackState(float elapsedTime)override;

	// �X�L���X�e�[�g
	void TransitionSkillState()override;
	void UpdateSkillState(float elapsedTime)override;
};