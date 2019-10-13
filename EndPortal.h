#pragma once
#include "GameObject.h"

class Character;

class EndPortal : public GameObject
{
public:
	EndPortal(DirectX::XMFLOAT3 position = {}, Character* player = nullptr);
	virtual ~EndPortal() = default;

	EndPortal(const EndPortal& other) = delete;
	EndPortal(EndPortal&& other) noexcept = delete;
	EndPortal& operator=(const EndPortal& other) = delete;
	EndPortal& operator=(EndPortal&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	DirectX::XMFLOAT3 m_Position;
	bool m_IsOpen = false, m_SoundIsPlaying = false;
	Character* m_Player;
	FMOD::Sound* m_pSound;
};