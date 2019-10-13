#pragma once
#include "PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostSludge : public PostProcessingMaterial
{
public:
	PostSludge();
	virtual ~PostSludge() = default;

	PostSludge(const PostSludge& other) = delete;
	PostSludge(PostSludge&& other) noexcept = delete;
	PostSludge& operator=(const PostSludge& other) = delete;
	PostSludge& operator=(PostSludge&& other) noexcept = delete;
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* pRendertarget) override;
private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
};