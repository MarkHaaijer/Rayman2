#include "stdafx.h"
#include "Barrel.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "Projectile.h"
#include "ContentManager.h"
#include "SoundManager.h"

Barrel::Barrel(DirectX::XMFLOAT3 position, Character* player):
	m_Position(position),
	m_Player(player)
{
	UNREFERENCED_PARAMETER(player);
}

void Barrel::Initialize(const GameContext& gameContext)
{
	SetTag(L"Barrel");
	UNREFERENCED_PARAMETER(gameContext);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetKinematic(true);
	pRigidBody->SetCollisionGroup(Group3);
	AddComponent(pRigidBody);

	auto physx = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 1.f);

	// Model
	auto pModelComponent = new ModelComponent(L"./Resources/Meshes/Barrel.ovm");

	pModelComponent->SetMaterial(8);
	AddComponent(pModelComponent);

	// Hitbox collider
	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(3.f,3.f,3.f));
	const auto collider = new ColliderComponent(boxGeom, *pDefaultMaterial);
	AddComponent(collider);

	GetTransform()->Translate(m_Position);

	SetOnTriggerCallBack(PhysicsCallback{[this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
    {
        if (action == GameObject::TriggerAction::ENTER)
        {
            if (trigger != other)
			{
				if (other == m_Player)
				{
					m_Player->TakeDamage(15.f, false);
					m_Player->CreateSparkler(GetTransform()->GetPosition(), {0.3f,0.3f,0.3f,1.f}, 0.5f, 10.f);
					m_Player->CreateSparkler(GetTransform()->GetPosition(), {0.91f,0.91f,0.72f,1.f}, 0.5f, 2.f);

					FMOD::Sound* pSound;
					FMOD_RESULT sound;
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Explosion.wav", FMOD_DEFAULT,NULL, &pSound);
					UNREFERENCED_PARAMETER(sound);
					SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

					m_IsDestroyed = true;
				}
			}
        }
	} });
	collider->EnableTrigger(true);
}

void Barrel::Update(const GameContext& gameContext)
{
	GetTransform()->Translate(DirectX::XMFLOAT3{GetTransform()->GetPosition().x, GetTransform()->GetPosition().y-gameContext.pGameTime->GetElapsed()*70.f, GetTransform()->GetPosition().z});

	DirectX::XMFLOAT3 targetPosition = m_Player->GetTransform()->GetPosition();
	DirectX::XMFLOAT3 currentPosition = GetTransform()->GetPosition();
	m_XZDistanceToPlayer = sqrt(pow(targetPosition.x-currentPosition.x,2.f) + pow(targetPosition.z-currentPosition.z,2.f));

	if (m_IsDestroyed)
		this->~Barrel();
}

void Barrel::Destroy()
{
	m_IsDestroyed = true;
}

float Barrel::GetDistanceToPlayer()
{
	return m_XZDistanceToPlayer;
}
