#include "stdafx.h"
#include "Sparkler.h"
#include "Components.h"

Sparkler::Sparkler(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color, float lifeTime,  std::wstring assetFile, float size, float speed, UINT amount) :
	m_ElapsedTime(0.f),
	m_LifeTime(lifeTime),
	m_Size(size),
	m_Speed(speed),
	m_Position(position),
	m_AssetFile(assetFile),
	m_pParticleEmitter(nullptr),
	m_Color(color),
	m_Amount(amount)
{
}

void Sparkler::Initialize(const GameContext& gameContext)
{
	SetTag(L"Sparkler");
	UNREFERENCED_PARAMETER(gameContext);
	
	m_pParticleEmitter = new ParticleEmitterComponent(m_AssetFile, m_Amount);
	AddComponent(m_pParticleEmitter);

	m_pParticleEmitter->SetMinSize(m_Size*0.5f);
	m_pParticleEmitter->SetMaxSize(m_Size);
	m_pParticleEmitter->SetMinEnergy(0.2f);
	m_pParticleEmitter->SetMaxEnergy(0.5f);
	m_pParticleEmitter->SetVelocity({m_Speed, m_Speed, m_Speed});
	m_pParticleEmitter->SetColor(m_Color);

	GetTransform()->Translate(m_Position);
}

void Sparkler::Update(const GameContext& gameContext)
{
	m_ElapsedTime += gameContext.pGameTime->GetElapsed();
	if (m_ElapsedTime > m_LifeTime)
		this->~Sparkler();
}