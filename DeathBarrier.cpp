#include "stdafx.h"
#include "DeathBarrier.h"
#include "PhysxManager.h"
#include "Components.h"
#include "../OverlordProject/CourseObjects/Week 2/Character.h"
#include "SoundManager.h"
#include "Barrel.h"

DeathBarrier::DeathBarrier(DirectX::XMFLOAT3 position, float length, float width):
	m_Position(position),
	m_Length(length),
	m_Width(width)
{
}

void DeathBarrier::Initialize(const GameContext& gameContext)
{
	SetTag(L"DeathBarrier");
	UNREFERENCED_PARAMETER(gameContext);

	auto pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(Group2);
	pRigidBody->SetCollisionIgnoreGroups(Group0);
	AddComponent(pRigidBody);

	auto physx = PhysxManager::GetInstance()->GetPhysics();

	auto pDefaultMaterial = physx->createMaterial(0.f, 0.f, 0.f);
	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(m_Length, 2.f, m_Width));
	const auto collider = new ColliderComponent(boxGeom, *pDefaultMaterial);
	AddComponent(collider);

	GetTransform()->Translate(m_Position);

	SetOnTriggerCallBack(PhysicsCallback{[this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
    {
        if (action == GameObject::TriggerAction::ENTER)
        {
            if (trigger != other)
			{
				Character* player = dynamic_cast<Character*>(other);

				if (player)
				{
					player->TakeDamage(10, true);
					FMOD::Sound* pSound;
					FMOD_RESULT sound;
					sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Splash.wav", FMOD_DEFAULT,NULL, &pSound);
					UNREFERENCED_PARAMETER(sound);
					SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);
				}
				else
				{
					Barrel* barrel = dynamic_cast<Barrel*>(other);

					if (barrel)
					{
						if (barrel->GetDistanceToPlayer() < 50.f)
						{
							FMOD::Sound* pSound;
							FMOD_RESULT sound;
							sound = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Splash.wav", FMOD_DEFAULT,NULL, &pSound);
							UNREFERENCED_PARAMETER(sound);
							SoundManager::GetInstance()->GetSystem()->playSound(pSound, NULL, false, 0);

							FMOD::Sound* pSound2;
							FMOD_RESULT sound2;
							sound2 = SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Sounds/Explosion.wav", FMOD_DEFAULT,NULL, &pSound2);
							UNREFERENCED_PARAMETER(sound2);
							SoundManager::GetInstance()->GetSystem()->playSound(pSound2, NULL, false, 0);
						}
						barrel->Destroy();
					}
				}
			}
        }
	} });
	collider->EnableTrigger(true);
}