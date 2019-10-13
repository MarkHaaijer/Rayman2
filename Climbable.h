#pragma once
#include "GameObject.h"

class Climbable : public GameObject
{
public:
	Climbable(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float yawAngle, float rollAngle);
	virtual ~Climbable() = default;

	Climbable(const Climbable& other) = delete;
	Climbable(Climbable&& other) noexcept = delete;
	Climbable& operator=(const Climbable& other) = delete;
	Climbable& operator=(Climbable&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;

private:
	DirectX::XMFLOAT3 m_Position; 
	DirectX::XMFLOAT3 m_Size;
	float m_yawAngle, m_rollAngle;
};