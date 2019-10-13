#include "stdafx.h"
#include "Projectile.h"
#include "PhysxManager.h"
#include "Components.h"
#include "Sparkler.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "SoundManager.h"

Projectile::Projectile(float angle, DirectX::XMFLOAT3 position, float radius, std::wstring assetFile) :
	m_Radius(radius),
	m_Angle(angle),
	m_ElapsedTime(0.f),
	m_Position(position),
	m_AssetFile(assetFile)
{
}

void Projectile::Initialize(const GameContext& gameContext)
{
	SetTag(L"Projectile");
	UNREFERENCED_PARAMETER(gameContext);

	auto physx = PhysxManager::GetInstance()->GetPhysics();
	
	auto particleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Projectile.png", 10);
	AddComponent(particleEmitter);

	particleEmitter->SetMinEnergy(0.2f);
	particleEmitter->SetMaxEnergy(0.2f);
	particleEmitter->SetMaxSizeGrow(0.1f);

	auto pBouncyMaterial = physx->createMaterial(0.f, 0.f, 1.f);
	std::shared_ptr<physx::PxGeometry> sphereGeom(new physx::PxSphereGeometry(m_Radius));
	const auto collider = new ColliderComponent(sphereGeom, *pBouncyMaterial);
	AddComponent(collider);

	std::shared_ptr<physx::PxGeometry> sphereGeom2(new physx::PxSphereGeometry(m_Radius*0.0001f));
	const auto bounceCollider = new ColliderComponent(sphereGeom, *pBouncyMaterial);
	AddComponent(bounceCollider);

	auto angleRad = m_Angle*(DirectX::XM_PI/180.f);
	GetTransform()->Translate(m_Position.x + sin(angleRad)*3.f, m_Position.y, m_Position.z + cos(angleRad)*3.f);

	SetOnTriggerCallBack(PhysicsCallback{[this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
    {
        if (action == GameObject::TriggerAction::ENTER)
        {
            if (trigger != other)
			{
				if (other->GetTag() == L"Level")
				{
					++m_Bounces;
					m_DoCreateParticle = true;
				}
			}
        }
	} });
	collider->EnableTrigger(true);
}

void Projectile::Update(const GameContext& gameContext)
{
	if (m_DoCreateParticle && m_Bounces <= m_MaxBounces)
	{
		FMOD::Sound* pSound;
		FMOD_RESULT sound;
		sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/ProjectileBounce.wav", FMOD_DEFAULT,NULL, &pSound);
		UNREFERENCED_PARAMETER(sound);
		SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

		Sparkler* sparkler = new Sparkler(GetTransform()->GetPosition());
		AddChild(sparkler);
		m_DoCreateParticle = false;
	}

	m_ElapsedTime += gameContext.pGameTime->GetElapsed();
	if ((m_ElapsedTime > 2.5f || m_Bounces > m_MaxBounces) && !m_CanDie) // let the final sparkler play out before dying
	{
		Sparkler* sparkler = new Sparkler(GetTransform()->GetPosition());
		AddChild(sparkler);
		m_ElapsedTime = 3.f;
		m_CanDie = true;
		GetTransform()->Translate(DirectX::XMFLOAT3{99999.f,99999.f,99999.f});
	}

	if (m_ElapsedTime >= 6.f) // finally die
	{
		this->~Projectile();
	}
}

void Projectile::Die()
{
	m_Bounces = 3;
}
