#include "stdafx.h"
#include "PostSludge.h"
#include "RenderTarget.h"

PostSludge::PostSludge()
	: PostProcessingMaterial(L"./Resources/Effects/Post/SludgeAir.fx", 3),
	m_pTextureMapVariabele(nullptr)
{
}

void PostSludge::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	m_pTextureMapVariabele = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
	//Check if valid!
	if (!m_pTextureMapVariabele->IsValid())
	{
		Logger::LogWarning(L"gTexture not found in PostSludge");
	}
}

void PostSludge::UpdateEffectVariables(RenderTarget* pRendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(pRendertarget->GetShaderResourceView());
}