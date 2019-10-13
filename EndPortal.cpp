#include "stdafx.h"
#include "EndPortal.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "ContentManager.h"
#include "SoundManager.h"

EndPortal::EndPortal(DirectX::XMFLOAT3 position, Character* player):
	m_Position(position),
	m_Player(player),
	m_pSound(nullptr)
{
}

void EndPortal::Initialize(const GameContext& gameContext)
{
	SetTag(L"EndPortal");
	UNREFERENCED_PARAMETER(gameContext);

	auto pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(Group2);
	pRigidBody->SetCollisionIgnoreGroups(Group0);
	AddComponent(pRigidBody);

	auto physx = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 1.f);

	// Model
	auto pModelComponent = new ModelComponent(L"./Resources/Meshes/Cage.ovm");

	pModelComponent->SetMaterial(28);
	AddComponent(pModelComponent);

	// Physical collider
	physx::PxConvexMesh* convexMesh = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Cage.ovpc");
	std::shared_ptr<physx::PxGeometry> convexGeom(new physx::PxConvexMeshGeometry(convexMesh));
	AddComponent(new ColliderComponent(convexGeom, *pDefaultMaterial));

	// Hitbox collider
	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(2.5f,4.5f,2.5f));
	const auto collider = new ColliderComponent(boxGeom, *pDefaultMaterial);
	AddComponent(collider);

	GetTransform()->Rotate(0.f,0.f,90.f);
	GetTransform()->Translate(m_Position);

	SetOnTriggerCallBack(PhysicsCallback{[this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
    {
		UNREFERENCED_PARAMETER(trigger);
        if (action == GameObject::TriggerAction::ENTER)
        {
            if (other == m_Player && m_IsOpen)
			{
				m_Player->TakeDamage(100,true);
				m_Player->Win({GetTransform()->GetPosition().x + 4.f, GetTransform()->GetPosition().y,GetTransform()->GetPosition().z});
			}
        }
	} });
	collider->EnableTrigger(true);
}

void EndPortal::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_Player->GetScore() >= 12 && !m_IsOpen)
	{
		m_IsOpen = true;
		// Portal emitter
		auto particleHolder = new GameObject();
		auto particleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/EndPortal.png", 5);

		particleEmitter->SetMinEnergy(4.f);
		particleEmitter->SetMaxEnergy(4.f);
		particleEmitter->SetMinSize(0.2f);
		particleEmitter->SetMaxSize(0.2f);
		particleEmitter->SetMinSizeGrow(45.f);
		particleEmitter->SetMaxSizeGrow(45.f);

		particleHolder->AddComponent(particleEmitter);
		AddChild(particleHolder);
		particleHolder->GetTransform()->Translate(GetTransform()->GetPosition().x + 3.7f, GetTransform()->GetPosition().y,GetTransform()->GetPosition().z);
	}

	if (m_IsOpen)
	{
		DirectX::XMFLOAT3 targetPosition = m_Player->GetTransform()->GetPosition();
	    DirectX::XMFLOAT3 currentPosition = GetTransform()->GetPosition();
		float distance = sqrt(pow(targetPosition.x-currentPosition.x,2.f) + pow(targetPosition.y-currentPosition.y,2.f) + pow(targetPosition.z-currentPosition.z,2.f));

		if (distance < 30.f)
		{
			if (!m_SoundIsPlaying)
			{
				FMOD_RESULT sound;
				sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/EndPortal.wav", FMOD_LOOP_NORMAL,NULL, &m_pSound);
				UNREFERENCED_PARAMETER(sound);
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound, NULL, false, 0);
				m_SoundIsPlaying = true;
			}
		}
		else if (m_SoundIsPlaying)
		{
			m_pSound->release();
			m_SoundIsPlaying = false;
		}
	}
}