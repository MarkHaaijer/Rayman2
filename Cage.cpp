#include "stdafx.h"
#include "Cage.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "Projectile.h"
#include "ContentManager.h"
#include "Sparkler.h"
#include "SoundManager.h"

Cage::Cage(DirectX::XMFLOAT3 position):
	m_Position(position)
{
}

void Cage::Initialize(const GameContext& gameContext)
{
	SetTag(L"Cage");
	UNREFERENCED_PARAMETER(gameContext);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetKinematic(true);
	pRigidBody->SetCollisionGroup(Group2);
	pRigidBody->SetCollisionIgnoreGroups(Group0);
	AddComponent(pRigidBody);

	auto physx = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 1.f);

	// Model
	auto pModelComponent = new ModelComponent(L"./Resources/Meshes/Cage.ovm");

	pModelComponent->SetMaterial(27);
	AddComponent(pModelComponent);

	// Physical collider
	physx::PxConvexMesh* convexMesh = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Cage.ovpc");
	std::shared_ptr<physx::PxGeometry> convexGeom(new physx::PxConvexMeshGeometry(convexMesh));
	AddComponent(new ColliderComponent(convexGeom, *pDefaultMaterial));

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
				Projectile* projectile = dynamic_cast<Projectile*>(other);

				if (projectile)
				{
					--m_Health;

					FMOD::Sound* pSound;
					FMOD_RESULT sound;
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/CageHit.wav", FMOD_DEFAULT,NULL, &pSound);
					UNREFERENCED_PARAMETER(sound);
					SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

					if (m_Health == 0 && !m_IsCollected)
					{
						m_Player = dynamic_cast<Character*>(projectile->GetParent());
						m_Player->IncreaseCageCount();
						m_IsCollected = true;
					}

					projectile->Die();
				}
			}
        }
	} });
	collider->EnableTrigger(true);
}

void Cage::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_IsCollected && m_Player != nullptr && GetTransform()->GetPosition().x != 0.f)
	{
		FMOD::Sound* pSound;
		FMOD_RESULT sound;
		sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/CageExplode.wav", FMOD_DEFAULT,NULL, &pSound);
		UNREFERENCED_PARAMETER(sound);
		SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

		m_Player->CreateLum(GetTransform()->GetPosition());
		auto sparkler1 = new Sparkler(GetTransform()->GetPosition(),{0.97f, 0.95f, 0.40f, 1.f}, 0.75f, L"./Resources/Textures/Sparkle.png", 5.f, 4.f, 3);
		AddChild(sparkler1);
		auto sparkler2 = new Sparkler(GetTransform()->GetPosition(),{1.f, 1.f, 1.f, 1.f}, 0.75f, L"./Resources/Textures/CageBreak.png", 5.f, 4.f, 3);
		AddChild(sparkler2);
		GetTransform()->Translate(0.f,0.f,0.f);
	}

	if (m_Player != nullptr)
	{
		if (m_IsCollected && m_Player->GetReset() )
		{
			m_Health = 3;
			m_IsCollected = false;
			GetTransform()->Translate(m_Position);
		}
	}
}