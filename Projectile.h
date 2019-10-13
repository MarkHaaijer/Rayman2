#pragma once
#include "GameObject.h"

class ParticleEmitterComponent;

class Projectile : public GameObject
{
public:
	Projectile(float angle = 0.f, DirectX::XMFLOAT3 position = {}, float radius = 1.f, std::wstring assetFile = L"./Resources/Textures/Projectile.png");
	virtual ~Projectile() = default;

	Projectile(const Projectile& other) = delete;
	Projectile(Projectile&& other) noexcept = delete;
	Projectile& operator=(const Projectile& other) = delete;
	Projectile& operator=(Projectile&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Die();

private:
	float m_Radius, m_Angle, m_ElapsedTime;
	DirectX::XMFLOAT3 m_Position;
	std::wstring m_AssetFile;
	UINT m_Bounces = 0;
	UINT m_MaxBounces = 2;
	bool m_DoCreateParticle = false;
	bool m_CanDie = false;
};