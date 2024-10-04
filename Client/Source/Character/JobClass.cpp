#include "JobClass.h"
#include "Graphics/Graphics.h"
#include "Camera/Camera.h"
#include "Input/Input.h"
#include "Collision/Collision.h"
#include "Stage/Stage.h"

// ***************************************************************************************
// *
// * JobClass
// *
// ***************************************************************************************
void JobClass::Update(float elapsedTime)
{
	switch (owner->GetState())
	{
	case Player::State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case Player::State::Move:
		UpdateMoveState(elapsedTime);
		break;
	case Player::State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	case Player::State::Skill:
		UpdateSkillState(elapsedTime);
		break;
	}
}
void JobClass::ChangeState(Player::State state)
{
	// 現在のステートと異なるときだけTransition関数を呼び出す
	if (owner->GetState() != state)
	{
		switch (state)
		{
		case Player::State::Idle:
			TransitionIdleState();
			break;
		case Player::State::Move:
			TransitionMoveState();
			break;
		case Player::State::Attack:
			TransitionAttackState();
			break;
		case Player::State::Skill:
			TransitionSkillState();
			break;
		}
	}
}
void JobClass::TransitionIdleState()
{
	owner->SetState(Player::State::Idle);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Idle), true);
}
void JobClass::UpdateIdleState(float elapsedTime)
{

}
void JobClass::TransitionMoveState()
{
	owner->SetState(Player::State::Move);
	owner->LookAt(owner->GetTargetPosition().x, owner->GetTargetPosition().z);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Run), true);
}
void JobClass::UpdateMoveState(float elapsedTime)
{

}
void JobClass::TransitionAttackState()
{
	owner->SetState(Player::State::Attack);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack1), false);
}
void JobClass::UpdateAttackState(float elapsedTime)
{

}
void JobClass::TransitionSkillState()
{
	owner->SetState(Player::State::Skill);
	owner->SetState(Player::State::Attack);

	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack2), true);
}
void JobClass::UpdateSkillState(float elapsedTime)
{

}
// ***************************************************************************************
// *
// * ArcharClass
// *
// ***************************************************************************************
void ArcherClass::TransitionAttackState()
{
	owner->SetState(Player::State::Attack);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack1), false);
}
void ArcherClass::UpdateAttackState(float elapsedTime)
{

}

void ArcherClass::TransitionSkillState()
{
	owner->SetState(Player::State::Skill);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::SkillPowershotStart), false);
}
void ArcherClass::UpdateSkillState(float elapsedTime)
{
}

// ***************************************************************************************
// *
// * MageClass
// *
// ***************************************************************************************
void MageClass::TransitionAttackState()
{
	owner->SetState(Player::State::Attack);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack1), false);
}
void MageClass::UpdateAttackState(float elapsedTime)
{
}

void MageClass::TransitionSkillState()
{
	owner->SetState(Player::State::Skill);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::SkillHeal), false);

}
void MageClass::UpdateSkillState(float elapsedTime)
{

}
// ***************************************************************************************
// *
// * RogueClass
// *
// ***************************************************************************************
void RogueClass::TransitionAttackState()
{
	owner->SetState(Player::State::Attack);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack1), false);
}
void RogueClass::UpdateAttackState(float elapsedTime)
{
}

void RogueClass::TransitionSkillState()
{
	owner->SetState(Player::State::Skill);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::SkillSneakPoof), false);
}
void RogueClass::UpdateSkillState(float elapsedTime)
{

}
// ***************************************************************************************
// *
// * TankClass
// *
// ***************************************************************************************
void TankClass::TransitionAttackState()
{
	owner->SetState(Player::State::Attack);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::Attack1), false);
}
void TankClass::UpdateAttackState(float elapsedTime)
{
}

void TankClass::TransitionSkillState()
{
	owner->SetState(Player::State::Skill);
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::Animation::SkillShieldBufStart), false);
}
void TankClass::UpdateSkillState(float elapsedTime)
{

}