#pragma once
#include "GameObject.h"

class Character;

class Lum : public GameObject
{
public:
	enum Type : UINT
	{
		YELLOW = 0,
		RED,
		GREEN
	};

	Lum(Character* player = nullptr, DirectX::XMFLOAT3 position = {}, Type type = YELLOW, float size = 1.5f);
	virtual ~Lum() = default;

	Lum(const Lum& other) = delete;
	Lum(Lum&& other) noexcept = delete;
	Lum& operator=(const Lum& other) = delete;
	Lum& operator=(Lum&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	Character* m_Player;
	DirectX::XMFLOAT3 m_Position; 
	float m_Size;
	Type m_Type;
	bool m_IsCollected = false;
	DirectX::XMFLOAT4 m_Color;
};

