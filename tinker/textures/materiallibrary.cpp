/*
Copyright (c) 2012, Lunar Workshop, Inc.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
   This product includes software developed by Lunar Workshop, Inc.
4. Neither the name of the Lunar Workshop nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LUNAR WORKSHOP INC ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LUNAR WORKSHOP BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "materiallibrary.h"

#include <memory>
#include <fstream>

#include <files.h>

#include <tinker/shell.h>
#include <renderer/renderer.h>
#include <datamanager/data.h>
#include <datamanager/dataserializer.h>
#include <renderer/shaders.h>

CMaterialLibrary* CMaterialLibrary::s_pMaterialLibrary = NULL;
static CMaterialLibrary g_MaterialLibrary = CMaterialLibrary();

CMaterialLibrary::CMaterialLibrary()
{
	s_pMaterialLibrary = this;
}

CMaterialLibrary::~CMaterialLibrary()
{
	// Should be TAssert(m_aMaterials.size() == 0); ?
	DebugPrint(tsprintf("Material library closing, %d materials\n", m_aMaterials.size()).c_str());

	m_aMaterials.clear();

	s_pMaterialLibrary = NULL;
}

CMaterialHandle CMaterialLibrary::AddMaterial(const tstring& sMaterial, int iClamp)
{
	CMaterialHandle hMaterial = FindAsset(sMaterial);
	if (hMaterial.IsValid())
		return hMaterial;

	return CMaterialHandle(sMaterial, AddAsset(sMaterial, iClamp));
}

CMaterialHandle CMaterialLibrary::AddMaterial(const class CData* pData, const tstring& sMaterial)
{
	CMaterialHandle hMaterial = FindAsset(sMaterial);
	if (hMaterial.IsValid())
		return hMaterial;

	CMaterial* pMaterial = CreateMaterial(pData, sMaterial);
	if (!pMaterial)
		return CMaterialHandle();

	return CMaterialHandle(pMaterial->m_sFile, pMaterial);
}

CMaterial* CMaterialLibrary::AddAsset(const tstring& sMaterial, int)
{
	if (!sMaterial.length())
		return nullptr;

	if (!sMaterial.endswith(".mat"))
		return nullptr;

	FILE* fp = tfopen_asset(sMaterial, "r");

	if (!fp)
		return nullptr;

	std::shared_ptr<CData> pData(new CData());
	CDataSerializer::Read(fp, pData.get());

	return CreateMaterial(pData.get(), sMaterial);
}

CMaterial* CMaterialLibrary::CreateMaterial(const CData* pData, const tstring& sMaterialOutput)
{
	tstring sMaterial = ToForwardSlashes(sMaterialOutput);
	if (!sMaterialOutput.length())
	{
		static int i = 0;
		sMaterial = tsprintf("[from data %x]", i++);
	}

	CData* pShaderData = pData->FindChild("Shader");
	if (!pShaderData)
	{
		TError("Material file with no shader: " + sMaterial + "\n");
		return nullptr;
	}

	CShader* pShader = CShaderLibrary::GetShader(pShaderData->GetValueString());
	TAssert(pShader);

	if (!pShader)
	{
		TError("Material file with invalid shader: " + sMaterial + "\n");
		return nullptr;
	}

	CMaterial& oMat = Get()->m_aMaterials[sMaterial];
	oMat.Clear();

	oMat.m_sFile = sMaterial;
	oMat.m_sTextureDirectory = pShaderData->FindChildValueString("_TextureDirectory");

	oMat.SetShader(pShader);

	for (size_t i = 0; i < pShaderData->GetNumChildren(); i++)
	{
		CData* pParameter = pShaderData->GetChild(i);
		tstring sParameter = pParameter->GetKey();

		if (sParameter == "_TextureDirectory")
			continue;

		if (sParameter == "TexelsPerMeter")
		{
			oMat.m_iTexelsPerMeter = pParameter->GetValueInt();
			continue;
		}

		if (sParameter == "Clamp")
		{
			oMat.m_bClamp = pParameter->GetValueBool();
			continue;
		}

		auto it = pShader->m_aParameters.find(sParameter);
		TAssert(it != pShader->m_aParameters.end());
		if (it == pShader->m_aParameters.end())
		{
			TError("Material file has a property '" + sParameter + "' that's not in the shader '" + pShader->m_sName + "': " + sMaterial + "\n");
			continue;
		}

		if (it->second.m_sBlend.length())
		{
			oMat.m_sBlend = it->second.m_sBlend;
			if (oMat.m_sBlend == "[value]")
				oMat.m_sBlend = pParameter->GetValueString();
			continue;
		}

		if (!pParameter->GetValueString().length())
			continue;

		CMaterial::CParameter& oPar = oMat.m_aParameters.push_back();
		oPar.m_sName = sParameter;

		oMat.FillParameter(oMat.m_aParameters.size()-1, pParameter->GetValueString(), pShader);
	}

	return &oMat;
}

CMaterialHandle CMaterialLibrary::CreateBlankMaterial(const tstring& sName)
{
	CMaterial& oMat = Get()->m_aMaterials[sName];
	oMat.Clear();

	oMat.m_sFile = sName;

	return CMaterialHandle(sName, &oMat);
}

CMaterialHandle CMaterialLibrary::FindAsset(const tstring& sMaterial)
{
	tstring sMaterialForward = ToForwardSlashes(sMaterial);
	tmap<tstring, CMaterial>::iterator it = Get()->m_aMaterials.find(sMaterialForward);
	if (it == Get()->m_aMaterials.end())
		return CMaterialHandle();

	return CMaterialHandle(sMaterialForward, &it->second);
}

void CMaterialLibrary::RemoveAsset(const tstring& sName)
{
	Get()->m_aMaterials.erase(sName);
}

bool CMaterialLibrary::IsAssetLoaded(const tstring& sMaterial)
{
	if (!Get())
		return false;

	tstring sMaterialForward = ToForwardSlashes(sMaterial);
	tmap<tstring, CMaterial>::iterator it = Get()->m_aMaterials.find(sMaterialForward);
	if (it == Get()->m_aMaterials.end())
		return false;

	return true;
}

void CMaterialLibrary::ClearUnreferenced()
{
	for (auto it = Get()->m_aMaterials.begin(); it != Get()->m_aMaterials.end();)
	{
		if (!it->second.m_iReferences)
			Get()->m_aMaterials.erase(it++);
		else
			it++;
	}
}

void CMaterial::Save() const
{
	if (m_sFile.substr(strlen("[from data ")) == "[from data ")
	{
		TAssert(m_sFile.substr(strlen("[from data ")) != "[from data ");
		return;
	}

	std::basic_ofstream<tchar> f(m_sFile.c_str());

	TAssert(f.is_open());
	if (!f.is_open())
		return;

	tstring sShader = "Shader: " + m_sShader + "\n{\n";
	f.write(sShader.data(), sShader.length());

	if (m_sBlend.length())
	{
		tstring sOut = "\tBlend: " + m_sBlend + "\n";
		f.write(sOut.data(), sOut.length());
	}

	for (size_t i = 0; i < m_aParameters.size(); i++)
	{
		tstring sOut = "\t" + m_aParameters[i].m_sName + ": " + m_aParameters[i].m_sValue + "\n";
		f.write(sOut.data(), sOut.length());
	}

	tstring sEnd = "}\n";
	f.write(sEnd.data(), sEnd.length());
}

void CMaterial::Reload()
{
	FILE* fp = tfopen_asset(m_sFile, "r");

	TAssert(fp);
	if (!fp)
		return;

	std::shared_ptr<CData> pData(new CData());
	CDataSerializer::Read(fp, pData.get());

	CMaterialLibrary::CreateMaterial(pData.get(), m_sFile);
}

void CMaterial::SetShader(class CShader* pShader)
{
	TAssert(pShader);
	if (!pShader)
		return;

	m_sShader = pShader->m_sName;
	m_pShader = pShader;
	m_ahTextures.resize(pShader->m_asTextures.size());
}

bool CMaterial::SetShader(const tstring& sShader)
{
	CShader* pShader = CShaderLibrary::GetShader(sShader);

	if (!pShader)
		return false;

	SetShader(pShader);

	return true;
}

size_t CMaterial::FindParameter(const tstring& sParameterName, bool bCreate)
{
	for (size_t i = 0; i < m_aParameters.size(); i++)
	{
		if (m_aParameters[i].m_sName == sParameterName)
			return i;
	}

	if (bCreate)
	{
		m_aParameters.push_back();
		m_aParameters.back().m_sName = sParameterName;
		return m_aParameters.size()-1;
	}

	return (size_t)~0;
}

void CMaterial::SetParameter(const tstring& sParameterName, const CTextureHandle& hTexture)
{
	CShader* pShader = m_pShader;
	TAssert(pShader);
	if (!pShader)
		return;

	size_t iParameter = FindParameter(sParameterName, true);

	CMaterial::CParameter& oPar = m_aParameters[iParameter];

	oPar.SetValue(oPar.m_sName, pShader);

	auto it = pShader->m_aParameters.find(oPar.m_sName);
	TAssert(it != pShader->m_aParameters.end());
	if (it == pShader->m_aParameters.end())
	{
		TError("Invalid parameter name " + oPar.m_sName + " in material " + m_sFile + "\n");
		return;
	}

	CShader::CParameter* pShaderPar = &it->second;
	oPar.m_pShaderParameter = pShaderPar;

	for (size_t j = 0; j < pShaderPar->m_aActions.size(); j++)
	{
		for (size_t k = 0; k < pShader->m_asTextures.size(); k++)
		{
			if (pShader->m_asTextures[k] == pShaderPar->m_aActions[j].m_sName)
				m_ahTextures[k] = hTexture;
		}
	}
}

void CMaterial::SetParameter(const tstring& sParameterName, const tstring& sValue)
{
	CShader* pShader = m_pShader;
	TAssert(pShader);
	if (!pShader)
		return;

	size_t iParameter = FindParameter(sParameterName, true);

	CMaterial::CParameter& oPar = m_aParameters[iParameter];

	oPar.SetValue(sValue, pShader);

	if (!oPar.m_pShaderParameter)
	{
		auto it = pShader->m_aParameters.find(oPar.m_sName);
		TAssert(it != pShader->m_aParameters.end());
		if (it == pShader->m_aParameters.end())
		{
			TError("Invalid parameter name " + oPar.m_sName + " in material " + m_sFile + "\n");
			return;
		}

		CShader::CParameter* pShaderPar = &it->second;
		oPar.m_pShaderParameter = pShaderPar;
	}
}

void CMaterial::SetParameter(const tstring& sParameterName, const Vector& vecValue)
{
	SetParameter(sParameterName, tsprintf("%f %f %f 1", vecValue.x, vecValue.y, vecValue.z));
}

void CMaterial::SetParameter(const tstring& sParameterName, float flValue)
{
	SetParameter(sParameterName, tsprintf("%f", flValue));
}

void CMaterial::FillParameter(size_t iParameter, const tstring& sData, class CShader* pShader)
{
	if (!pShader)
		pShader = CShaderLibrary::GetShader(m_sShader);

	TAssert(pShader);
	if (!pShader)
		return;

	CMaterial::CParameter& oPar = m_aParameters[iParameter];

	oPar.SetValue(sData, pShader);

	auto it = pShader->m_aParameters.find(oPar.m_sName);
	TAssert(it != pShader->m_aParameters.end());
	if (it == pShader->m_aParameters.end())
	{
		TError("Invalid parameter name " + oPar.m_sName + " in material " + m_sFile + "\n");
		return;
	}

	CShader::CParameter* pShaderPar = &it->second;
	oPar.m_pShaderParameter = pShaderPar;

	for (size_t j = 0; j < pShaderPar->m_aActions.size(); j++)
	{
		for (size_t k = 0; k < pShader->m_asTextures.size(); k++)
		{
			if (pShader->m_asTextures[k] == pShaderPar->m_aActions[j].m_sName)
			{
				if (sData.length())
				{
					m_ahTextures[k] = CTextureLibrary::AddTexture(sData, m_bClamp);
					if (!m_ahTextures[k].IsValid())
					{
						tstring sPrefix = "";
						if (oPar.m_sValue.startswith("["))
						{
							oPar.m_sValue = oPar.m_sValue.substr(1);
							sPrefix = "[";
						}

						if (m_sTextureDirectory.length())
							m_ahTextures[k] = CTextureLibrary::AddTexture(sPrefix + m_sTextureDirectory + "/" + oPar.m_sValue, m_bClamp);

						if (!m_ahTextures[k].IsValid())
							m_ahTextures[k] = CTextureLibrary::AddTexture(sPrefix + GetDirectory(m_sFile) + "/" + oPar.m_sValue, m_bClamp);

						if (!m_ahTextures[k].IsValid())
							TError("Couldn't load texture '" + sData + "' in material " + m_sFile + "\n");
					}
				}
				else
					m_ahTextures[k] = CTextureHandle();
			}
		}
	}
}

void CMaterial::CParameter::SetValue(const tstring& sValue, class CShader* pShader)
{
	TAssert(pShader);
	if (!pShader)
		return;

	CData oData;
	oData.SetValue(sValue);
	CData* pData = &oData;

	m_sValue = pData->GetValueString();

	if (m_eType == UT_NONE)
	{
		m_eType = pShader->FindType(m_sName);
		TAssert(m_eType);
		if (!m_eType)
		{
			TError("Can't find shader parameter " + m_sName + " in shader " + pShader->m_sName + "\n");
			return;
		}
	}

	if (m_eType == UT_FLOAT)
		m_flValue = pData->GetValueFloat();
	else if (m_eType == UT_VECTOR2D)
		m_vec2Value = pData->GetValueVector2D();
	else if (m_eType == UT_VECTOR3D)
		m_vecValue = pData->GetValueVector();
	else if (m_eType == UT_VECTOR4D)
		m_vec4Value = pData->GetValueVector4D();
	else if (m_eType == UT_INT)
		m_iValue = pData->GetValueInt();
	else if (m_eType == UT_BOOL)
		m_bValue = pData->GetValueBool();
	else if (m_eType == UT_MATRIX4X4)
		TUnimplemented();
	else if (m_eType == UT_SAMPLER)
	{
		// No op. Texture is read below.
	}
	else
		TUnimplemented();
}
