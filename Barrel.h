#pragma once
#include "GameObject.h"

class Character;

class Barrel : public GameObject
{
public:
	Barrel(DirectX::XMFLOAT3 position = {}, Character* player = nullptr);
	virtual ~Barrel() = default;

	Barrel(const Barrel& other) = delete;
	Barrel(Barrel&& other) noexcept = delete;
	Barrel& operator=(const Barrel& other) = delete;
	Barrel& operator=(Barrel&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Destroy();
	float GetDistanceToPlayer();

private:
	DirectX::XMFLOAT3 m_Position;
	Character* m_Player = nullptr;
	bool m_IsDestroyed = false;
	float m_XZDistanceToPlayer = 0;
};