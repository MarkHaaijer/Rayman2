#pragma once
#include "GameObject.h"

class DeathBarrier : public GameObject
{
public:
	DeathBarrier(DirectX::XMFLOAT3 position = {}, float length = 20.f, float width = 20.f);
	virtual ~DeathBarrier() = default;

	DeathBarrier(const DeathBarrier& other) = delete;
	DeathBarrier(DeathBarrier&& other) noexcept = delete;
	DeathBarrier& operator=(const DeathBarrier& other) = delete;
	DeathBarrier& operator=(DeathBarrier&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;

private:
	DirectX::XMFLOAT3 m_Position;
	float m_Length, m_Width;
};
