#include "stdafx.h"
#include "Climbable.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"

Climbable::Climbable(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float yawAngle, float rollAngle):
	m_Position(position),
	m_Size(size),
	m_yawAngle(yawAngle),
	m_rollAngle(rollAngle)
{
}

void Climbable::Initialize(const GameContext& gameContext)
{
	SetTag(L"Climbable");
	UNREFERENCED_PARAMETER(gameContext);
	auto physx = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 0.f);

	auto pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(Group2);
	AddComponent(pRigidBody);

	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(m_Size.x, m_Size.y, m_Size.z));
	const auto collider = new ColliderComponent(boxGeom, *pDefaultMaterial);
	AddComponent(collider);

	GetTransform()->Translate(m_Position.x, m_Position.y + m_Size.y*0.5f, m_Position.z);
	GetTransform()->Rotate(0, m_yawAngle, 0);

	SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
    {
		UNREFERENCED_PARAMETER(trigger);
        if (action == GameObject::TriggerAction::ENTER)
        {
			Character* player = dynamic_cast<Character*>(other);
            if (player)
				player->SetClimbing(true, m_yawAngle, m_rollAngle, GetTransform()->GetPosition().y+m_Size.y*0.5f);
        }
		if (action == GameObject::TriggerAction::LEAVE)
        {
			Character* player = dynamic_cast<Character*>(other);
            if (player)
				player->SetClimbing(false, m_yawAngle, 20.f, GetTransform()->GetPosition().y+m_Size.y*0.5f);                
        }

	});
	collider->EnableTrigger(true);
}
