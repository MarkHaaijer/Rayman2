#pragma once
#include "GameObject.h"

class Character;

class Cage : public GameObject
{
public:
	Cage(DirectX::XMFLOAT3 position = {});
	virtual ~Cage() = default;

	Cage(const Cage& other) = delete;
	Cage(Cage&& other) noexcept = delete;
	Cage& operator=(const Cage& other) = delete;
	Cage& operator=(Cage&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	DirectX::XMFLOAT3 m_Position;
	UINT m_Health = 3;
	bool m_IsCollected = false;
	Character* m_Player = nullptr;
};