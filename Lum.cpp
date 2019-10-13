#include "stdafx.h"
#include "Lum.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "Sparkler.h"
#include "SoundManager.h"

Lum::Lum(Character* player, DirectX::XMFLOAT3 position, Type type, float size):
	m_Player(player),
	m_Position(position),
	m_Size(size),
	m_Type(type),
	m_Color({1.f, 1.f, 1.f, 1.f})
{
}

void Lum::Initialize(const GameContext& gameContext)
{
	SetTag(L"Lum");
	UNREFERENCED_PARAMETER(gameContext);

	RigidBodyComponent* pRigidBody;

	pRigidBody = new RigidBodyComponent();
	pRigidBody->SetKinematic(true);

	pRigidBody->SetCollisionGroup(Group2);
	pRigidBody->SetCollisionIgnoreGroups(Group0);
	AddComponent(pRigidBody);

	auto physx = PhysxManager::GetInstance()->GetPhysics();

	auto assetFile = L"";

	switch (m_Type)
	{
		case YELLOW:
			assetFile = L"./Resources/Textures/YellowLum.png";
			m_Color = {0.97f, 0.95f, 0.40f, 1.f};
			break;

		case RED:
			assetFile = L"./Resources/Textures/RedLum.png";
			m_Color = {0.93f, 0.26f, 0.25f, 1.f};
			break;

		case GREEN:
			assetFile = L"./Resources/Textures/GreenLum.png";
			m_Color = {0.05f, 0.79f, 0.38f, 1.f};
			break;
	} 	

	if (GetParent() == m_Player)
	{
		auto particleEmitter = new ParticleEmitterComponent(assetFile, 10);
		particleEmitter->SetMinEnergy(0.2f);
		particleEmitter->SetMaxEnergy(0.2f);
		particleEmitter->SetMaxSizeGrow(0.5f);
		particleEmitter->SetMinSizeGrow(0.5f);
		AddComponent(particleEmitter);
	}
	else
	{
		auto particleEmitter = new ParticleEmitterComponent(assetFile, 3);
		particleEmitter->SetMinEnergy(0.4f);
		particleEmitter->SetMaxEnergy(0.4f);
		particleEmitter->SetMaxSizeGrow(0.2f);
		particleEmitter->SetMinSizeGrow(0.2f);
		AddComponent(particleEmitter);
	}

	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 0.f);
	std::shared_ptr<physx::PxGeometry> sphereGeom(new physx::PxSphereGeometry(m_Size));
	const auto collider = new ColliderComponent(sphereGeom, *pDefaultMaterial);
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
					m_IsCollected = true;
				}
			}
        }
	} });
	collider->EnableTrigger(true);
}

void Lum::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Check if the lum was created by the player (by opening cage)
	if (GetParent() == m_Player)
	{
	    //move towards the center of the world (or where ever you like)
		DirectX::XMFLOAT3 targetPosition = m_Player->GetTransform()->GetPosition();
	    DirectX::XMFLOAT3 currentPosition = GetTransform()->GetPosition();

		DirectX::XMVECTOR directionOfTravel = {targetPosition.x-currentPosition.x, targetPosition.y-currentPosition.y,targetPosition.z-currentPosition.z};

	    // Retrieve direction
	    DirectX::XMVector3Normalize(directionOfTravel);

		auto speed = 4.f;

	    // Scale the movement on each axis by the directionOfTravel vector components
	    GetTransform()->Translate(
		GetTransform()->GetPosition().x + DirectX::XMVectorGetX(directionOfTravel) * speed * gameContext.pGameTime->GetElapsed(),
        GetTransform()->GetPosition().y + DirectX::XMVectorGetY(directionOfTravel) * speed * gameContext.pGameTime->GetElapsed(),
        GetTransform()->GetPosition().z + DirectX::XMVectorGetZ(directionOfTravel) * speed * gameContext.pGameTime->GetElapsed());
	}

	if (m_IsCollected && GetTransform()->GetPosition().x != 0.f)
	{
		auto lifeTime = 0.75f;

		if (GetParent() == m_Player)
		{
			FMOD::Sound* pSound;
			m_Player->IncreaseLumCount();
			FMOD_RESULT sound;
			sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/YellowLum.wav", FMOD_DEFAULT,NULL, &pSound);
			UNREFERENCED_PARAMETER(sound);
			SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);
			m_Player->CreateSparkler(GetTransform()->GetPosition(), m_Color, lifeTime, 2.f);
			this->~Lum();
		}
		else
		{
			if (m_Type == GREEN)
				lifeTime = 1.5f;

			auto sparkler = new Sparkler(GetTransform()->GetPosition(), m_Color, lifeTime);
			AddChild(sparkler);

			// Create sound
			FMOD::Sound* pSound;
			FMOD_RESULT sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/YellowLum.wav", FMOD_DEFAULT,NULL, &pSound);
			switch (m_Type)
			{
				case YELLOW:
					m_Player->IncreaseLumCount();
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/YellowLum.wav", FMOD_DEFAULT,NULL, &pSound);
					break;

				case RED:
					m_Player->Heal(10);
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Health.wav", FMOD_DEFAULT,NULL, &pSound);
					break;

				case GREEN:
					m_Player->SetSpawnPosition(GetTransform()->GetPosition());
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Checkpoint.wav", FMOD_DEFAULT,NULL, &pSound);
					break;
			}
			UNREFERENCED_PARAMETER(sound);
			SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

			GetTransform()->Translate(0.f,0.f,0.f);
		}
	}

	if (m_IsCollected && m_Player->GetReset())
	{
		m_IsCollected = false;
		GetTransform()->Translate(m_Position); // Which is the start position
	}
}
