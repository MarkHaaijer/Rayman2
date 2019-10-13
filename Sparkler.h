#pragma once
#include "GameObject.h"

class ParticleEmitterComponent;

class Sparkler : public GameObject
{
public:
	Sparkler(DirectX::XMFLOAT3 position = {}, DirectX::XMFLOAT4 color = {1.f, 1.f, 1.f, 1.f}, float lifeTime = 1.f, std::wstring assetFile = L"./Resources/Textures/Sparkle.png", float size = 2.f, float speed = 1.f, UINT amount = 10);
	virtual ~Sparkler() = default;

	Sparkler(const Sparkler& other) = delete;
	Sparkler(Sparkler&& other) noexcept = delete;
	Sparkler& operator=(const Sparkler& other) = delete;
	Sparkler& operator=(Sparkler&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_ElapsedTime, m_LifeTime, m_Size, m_Speed;
	DirectX::XMFLOAT3 m_Position;
	std::wstring m_AssetFile;
	ParticleEmitterComponent* m_pParticleEmitter;
	DirectX::XMFLOAT4 m_Color;
	UINT m_Amount;
};