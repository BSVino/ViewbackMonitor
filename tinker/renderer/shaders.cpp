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

#include "shaders.h"

#include <memory>
#include <time.h>

#include <common.h>
#include <tinker_platform.h>
#include <worklistener.h>

#include <tinker/application.h>
#include <tinker/cvar.h>
#include <datamanager/data.h>
#include <datamanager/dataserializer.h>

#include "tinker_gl.h"

CShaderLibrary* CShaderLibrary::s_pShaderLibrary = NULL;
static CShaderLibrary g_ShaderLibrary = CShaderLibrary();

CShaderLibrary::CShaderLibrary()
{
	s_pShaderLibrary = this;
}

CShaderLibrary::~CShaderLibrary()
{
	DestroyNonStatic();

	s_pShaderLibrary = NULL;
}

void CShaderLibrary::Initialize()
{
	s_pShaderLibrary->InitializeNonStatic();
}

void CShaderLibrary::InitializeNonStatic()
{
	m_sHeader.clear();
	m_sFunctions.clear();
	m_sMain.clear();

	m_bCompiled = false;
	m_iSamples = -1;

	tstring sFunctions = "shaders/functions.si";
	FILE* f = tfopen_asset(sFunctions, "r");

	if (f)
	{
		tstring sLine;
		while (fgetts(sLine, f))
			m_sFunctions += sLine;

		fclose(f);
	}
	else
		TMsg(tstring("Warning: Couldn't find shader functions file: ") + sFunctions + "\n");

#if defined(TINKER_OPENGLES_3)
	tstring sHeader = "shaders/header_gles3.si";
#elif defined(TINKER_OPENGLES_2)
	tstring sHeader = "shaders/header_gles2.si";
#else
	tstring sHeader = "shaders/header_gl3.si";
#endif

	f = tfopen_asset(sHeader, "r");

	if (f)
	{
		tstring sLine;
		while (fgetts(sLine, f))
			m_sHeader += sLine;

		fclose(f);
	}
	else
		TMsg(tstring("Warning: Couldn't find shader header file: ") + sHeader + "\n");

#if defined(TINKER_OPENGLES_3)
	tstring sMain = "shaders/main_gles3.si";
#elif defined(TINKER_OPENGLES_2)
	tstring sMain = "shaders/main_gles2.si";
#else
	tstring sMain = "shaders/main_gl3.si";
#endif

	f = tfopen_asset(sMain, "r");

	if (f)
	{
		tstring sLine;
		while (fgetts(sLine, f))
			m_sMain += sLine;

		fclose(f);
	}
	else
		TMsg(tstring("Warning: Couldn't find shader main file: ") + sMain + "\n");
}

void CShaderLibrary::Destroy()
{
	s_pShaderLibrary->DestroyNonStatic();
}

void CShaderLibrary::DestroyNonStatic()
{
	for (size_t i = 0; i < m_aShaders.size(); i++)
	{
		CShader* pShader = &m_aShaders[i];
		glDetachShader((GLuint)pShader->m_iProgram, (GLuint)pShader->m_iVShader);
		glDetachShader((GLuint)pShader->m_iProgram, (GLuint)pShader->m_iFShader);
		glDeleteProgram((GLuint)pShader->m_iProgram);
		glDeleteShader((GLuint)pShader->m_iVShader);
		glDeleteShader((GLuint)pShader->m_iFShader);
	}

	m_aShaderNames.clear();
	m_aShaders.clear();
	m_sHeader.clear();
	m_sFunctions.clear();
	m_sMain.clear();
}

void CShaderLibrary::AddShader(const tstring& sFile)
{
	TAssert(!Get()->m_bCompiled);
	if (Get()->m_bCompiled)
		return;

	FILE* fp = tfopen_asset(sFile, "r");

	if (!fp)
	{
		TError("Couldn't open shader file: " + sFile + "\n");
		return;
	}

	std::shared_ptr<CData> pData(new CData());
	CDataSerializer::Read(fp, pData.get());

	tstring sDefines;

	CData* pDefine = pData->FindChild("Define");
	if (pDefine)
	{
		tstring sValue = pDefine->GetValueString();
		tvector<tstring> asTokens;
		tstrtok(sValue, asTokens, " ");

		for (auto& sToken : asTokens)
			sDefines += "#define " + sToken + "\n";
	}

	CData* pName = pData->FindChild("Name");
	CData* pVertex = pData->FindChild("Vertex");
	CData* pFragment = pData->FindChild("Fragment");

	TAssert(pName);
	if (!pName)
	{
		TError("Malformed shader file. " + sFile + " has no Name entry.\n");
		return;
	}

	TAssert(pVertex);
	if (!pVertex)
	{
		TError("Malformed shader file. " + sFile + " has no Vertex entry.\n");
		return;
	}

	TAssert(pFragment);
	if (!pFragment)
	{
		TError("Malformed shader file. " + sFile + " has no Fragment entry.\n");
		return;
	}

	Get()->m_aShaders.push_back(CShader(pName->GetValueString(), pVertex->GetValueString(), pFragment->GetValueString()));
	Get()->m_aShaderNames[pName->GetValueString()] = Get()->m_aShaders.size()-1;

	auto& oShader = Get()->m_aShaders.back();

	oShader.m_sDefines = sDefines;

	for (size_t i = 0; i < pData->GetNumChildren(); i++)
	{
		CData* pChild = pData->GetChild(i);
		if (pChild->GetKey() == "Parameter")
		{
			auto& oParameter = oShader.m_aParameters[pChild->GetValueString()];
			oParameter.m_sName = pChild->GetValueString();

			for (size_t j = 0; j < pChild->GetNumChildren(); j++)
			{
				CData* pUniform = pChild->GetChild(j);
				if (pUniform->GetKey() == "Uniform")
				{
					auto& oUniform = oParameter.m_aActions.push_back();
					oUniform.m_sName = pUniform->GetValueString();
					oUniform.m_bTexture = false;
					CData* pValue = pUniform->FindChild("Value");
					CData* pTexture = pUniform->FindChild("Texture");
					TAssert(!(pValue && pTexture));
					TAssert(pValue || pTexture);

					if (pValue)
						oUniform.m_sValue = pValue->GetValueString();
					else if (pTexture)
					{
						oUniform.m_sValue = pTexture->GetValueString();
						oShader.m_asTextures.push_back(pUniform->GetValueString());
						oUniform.m_bTexture = true;
					}
				}
				else if (pUniform->GetKey() == "Blend")
				{
					tstring& sBlend = oParameter.m_sBlend;
					sBlend = pUniform->GetValueString();
				}
			}
		}
		else if (pChild->GetKey() == "Defaults")
		{
			for (size_t j = 0; j < pChild->GetNumChildren(); j++)
			{
				CData* pUniform = pChild->GetChild(j);
				auto& oDefault = oShader.m_aDefaults[pUniform->GetKey()];
				oDefault.m_sName = pUniform->GetKey();
				oDefault.m_sValue = pUniform->GetValueString();
			}
		}
	}
}

void CShaderLibrary::CompileShaders(int iSamples)
{
	TAssert(Get()->m_sFunctions.length());
	TAssert(Get()->m_sHeader.length());
	TAssert(Get()->m_sMain.length());

	if (iSamples != -1)
		Get()->m_iSamples = iSamples;

	TAssert(Get()->m_iSamples != -1);

	Get()->ClearLog();

//	if (GameServer()->GetWorkListener())
//		GameServer()->GetWorkListener()->SetAction("Compiling shaders", Get()->m_aShaders.size());

	if (Get()->m_bCompiled)
	{
		// If this is a recompile just blow through them.
		for (size_t i = 0; i < Get()->m_aShaders.size(); i++)
			Get()->m_aShaders[i].Compile();
	}
	else
	{
		bool bShadersCompiled = true;
		for (size_t i = 0; i < Get()->m_aShaders.size(); i++)
		{
			bShadersCompiled &= Get()->m_aShaders[i].Compile();

			if (!bShadersCompiled)
				break;

	//		if (GameServer()->GetWorkListener())
	//			GameServer()->GetWorkListener()->WorkProgress(i);
		}

		if (bShadersCompiled)
			Get()->m_bCompiled = true;
	}
}

void CShaderLibrary::DestroyShaders()
{
	for (size_t i = 0; i < Get()->m_aShaders.size(); i++)
		Get()->m_aShaders[i].Destroy();

	Get()->m_bCompiled = false;
}

void CShaderLibrary::ClearLog()
{
	m_bLogNeedsClearing = true;
}

void CShaderLibrary::WriteLog(const tstring& sFile, const char* pszLog, const char* pszShaderText)
{
	if (!pszLog || strlen(pszLog) == 0)
		return;

	tstring sLogFile = Application()->GetAppDataDirectory("shaders.txt");

	TMsg(tsprintf("Log file location: %s\n", sLogFile.c_str()));

	if (m_bLogNeedsClearing)
	{
		// Only clear it if we're actually going to write to it so we don't create the file.
		FILE* fp = tfopen(sLogFile, "w");
		fclose(fp);
		m_bLogNeedsClearing = false;
	}

	FILE* fp = tfopen(sLogFile, "a");
	fprintf(fp, ("Shader compile output for file: " + sFile + " timestamp: %d\n").c_str(), (int)time(NULL));
	fprintf(fp, "%s\n\n", pszLog);
	fprintf(fp, "Shader text follows:\n\n");

	tvector<tstring> asTokens;
	explode(pszShaderText, asTokens, "\n");
	for (size_t i = 0; i < asTokens.size(); i++)
		fprintf(fp, "%d: %s\n", i, asTokens[i].c_str());

	fprintf(fp, "\n\n");

	fclose(fp);
}

CShader* CShaderLibrary::GetShader(const tstring& sName)
{
	tmap<tstring, size_t>::const_iterator i = Get()->m_aShaderNames.find(sName);
	if (i == Get()->m_aShaderNames.end())
		return NULL;

	return &Get()->m_aShaders[i->second];
}

CShader* CShaderLibrary::GetShader(size_t i)
{
	if (i >= Get()->m_aShaders.size())
		return NULL;

	return &Get()->m_aShaders[i];
}

size_t CShaderLibrary::GetProgram(const tstring& sName)
{
	TAssert(Get());
	if (!Get())
		return 0;

	TAssert(Get()->GetShader(sName));
	if (!Get()->GetShader(sName))
		return 0;

	return Get()->GetShader(sName)->m_iProgram;
}

CShader::CShader(const tstring& sName, const tstring& sVertexFile, const tstring& sFragmentFile)
{
	m_sName = sName;
	m_sVertexFile = sVertexFile;
	m_sFragmentFile = sFragmentFile;
	m_iVShader = 0;
	m_iFShader = 0;
	m_iProgram = 0;
}

bool CShader::Compile()
{
	tstring sShaderHeader = CShaderLibrary::GetShaderHeader();

	if (CShaderLibrary::Get()->m_iSamples)
		sShaderHeader += "#define USE_MULTISAMPLE_TEXTURES 1\n";

	sShaderHeader += CShaderLibrary::GetShaderFunctions();

	tstring sVertexHeader;
	sVertexHeader += sShaderHeader;
	sVertexHeader += "#define VERTEX_PROGRAM\n";
	sVertexHeader += m_sDefines;

	tstring sFragmentHeader;
	sFragmentHeader += sShaderHeader;
	sFragmentHeader += "#define FRAGMENT_PROGRAM\n";
	sFragmentHeader += m_sDefines;

	tstring sVertexFile = tstring("shaders/") + m_sVertexFile + ".vs";
	FILE* f = tfopen_asset(sVertexFile, "r");

	TAssert(f);
	if (!f)
	{
		TMsg(tstring("Could not open vertex program source: ") + sVertexFile + "\n");
		return false;
	}

	tstring sVertexShader;
	sVertexShader += "uniform mat4x4 mProjection;\n";
	sVertexShader += "uniform mat4x4 mView;\n";
	sVertexShader += "uniform mat4x4 mGlobal;\n";

	tstring sLine;
	while (fgetts(sLine, f))
		sVertexShader += sLine;

	fclose(f);

	tstring sFragmentFile = tstring("shaders/") + m_sFragmentFile + ".fs";
	f = tfopen_asset(sFragmentFile, "r");

	TAssert(f);
	if (!f)
	{
		TMsg(tstring("Could not open fragment program source: ") + sFragmentFile + "\n");
		return false;
	}

	tstring sFragmentShader;

	while (fgetts(sLine, f))
		sFragmentShader += sLine;

	fclose(f);

	tstring sFullVertexShader = sVertexHeader + sVertexShader + CShaderLibrary::Get()->m_sMain;
	const char* pszFullVertexShader = sFullVertexShader.c_str();

	size_t iVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource((GLuint)iVShader, 1, &pszFullVertexShader, NULL);
	glCompileShader((GLuint)iVShader);

	int iVertexCompiled;
	glGetShaderiv((GLuint)iVShader, GL_COMPILE_STATUS, &iVertexCompiled);

	if (iVertexCompiled != GL_TRUE || Application()->HasCommandLineSwitch("--debug-gl"))
	{
		int iLogLength = 0;
		char szLog[1024];
		glGetShaderInfoLog((GLuint)iVShader, 1024, &iLogLength, szLog);
		CShaderLibrary::Get()->WriteLog(m_sVertexFile + ".vs", szLog, pszFullVertexShader);
	}

	tstring sFullFragmentShader = sFragmentHeader + sFragmentShader + CShaderLibrary::Get()->m_sMain;
	const char* pszFullFragmentShader = sFullFragmentShader.c_str();

	size_t iFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource((GLuint)iFShader, 1, &pszFullFragmentShader, NULL);
	glCompileShader((GLuint)iFShader);

	int iFragmentCompiled;
	glGetShaderiv((GLuint)iFShader, GL_COMPILE_STATUS, &iFragmentCompiled);

	if (iFragmentCompiled != GL_TRUE || Application()->HasCommandLineSwitch("--debug-gl"))
	{
		int iLogLength = 0;
		char szLog[1024];
		glGetShaderInfoLog((GLuint)iFShader, 1024, &iLogLength, szLog);
		CShaderLibrary::Get()->WriteLog(m_sFragmentFile + ".fs", szLog, pszFullFragmentShader);
	}

	size_t iProgram = glCreateProgram();

	glBindAttribLocation(iProgram, 0, "vecPosition");		// Force position at location 0. ATI cards won't work without this.

	glAttachShader((GLuint)iProgram, (GLuint)iVShader);
	glAttachShader((GLuint)iProgram, (GLuint)iFShader);
	glLinkProgram((GLuint)iProgram);

	int iProgramLinked;
	glGetProgramiv((GLuint)iProgram, GL_LINK_STATUS, &iProgramLinked);

	if (iProgramLinked != GL_TRUE || Application()->HasCommandLineSwitch("--debug-gl"))
	{
		int iLogLength = 0;
		char szLog[1024];
		glGetProgramInfoLog((GLuint)iProgram, 1024, &iLogLength, szLog);
		CShaderLibrary::Get()->WriteLog("link", szLog, "link");
	}

	if (iVertexCompiled != GL_TRUE || iFragmentCompiled != GL_TRUE || iProgramLinked != GL_TRUE)
	{
		TError("Shader compilation failed for shader " + m_sName + ". Check shaders.txt\n");

		Destroy();

		return false;
	}

	m_iProgram = iProgram;
	m_iVShader = iVShader;
	m_iFShader = iFShader;

	m_iPositionAttribute = glGetAttribLocation(m_iProgram, "vecPosition");
	m_iNormalAttribute = glGetAttribLocation(m_iProgram, "vecNormal");
	m_iTangentAttribute = glGetAttribLocation(m_iProgram, "vecTangent");
	m_iBitangentAttribute = glGetAttribLocation(m_iProgram, "vecBitangent");
	for (size_t i = 0; i < MAX_TEXTURE_CHANNELS; i++)
		m_aiTexCoordAttributes[i] = glGetAttribLocation(m_iProgram, tsprintf("vecTexCoord%d", i).c_str());
	m_iColorAttribute = glGetAttribLocation(m_iProgram, "vecVertexColor");

	TAssert(m_iPositionAttribute != ~0);

	int iNumUniforms;
	glGetProgramiv(m_iProgram, GL_ACTIVE_UNIFORMS, &iNumUniforms);

	char szUniformName[256];
	GLsizei iLength;
	GLint iSize;
	GLenum iType;

	m_asUniformNames.clear();

	// Allocate all names first. Don't want a vector resize to screw it up.
	for (int i = 0; i < iNumUniforms; i++)
	{
		glGetActiveUniform(m_iProgram, i, sizeof(szUniformName), &iLength, &iSize, &iType, szUniformName);

		tstring sUniformName = szUniformName;

		if (sUniformName.endswith("[0]"))
			sUniformName = sUniformName.substr(0, sUniformName.length() - 3);

		m_asUniformNames.push_back(sUniformName);
	}

	m_aUniforms.clear();

	for (int i = 0; i < iNumUniforms; i++)
	{
		glGetActiveUniform(m_iProgram, i, sizeof(szUniformName), &iLength, &iSize, &iType, szUniformName);

		CShader::CUniform& oUniform = m_aUniforms[m_asUniformNames[i].c_str()];
		oUniform.m_pDefault = nullptr;
		switch (iType)
		{
		case GL_FLOAT: oUniform.m_eType = UT_FLOAT; break;
		case GL_FLOAT_VEC2: oUniform.m_eType = UT_VECTOR2D; break;
		case GL_FLOAT_VEC3: oUniform.m_eType = UT_VECTOR3D; break;
		case GL_FLOAT_VEC4: oUniform.m_eType = UT_VECTOR4D; break;
		case GL_INT: oUniform.m_eType = UT_INT; break;
		case GL_BOOL: oUniform.m_eType = UT_BOOL; break;
		case GL_FLOAT_MAT4: oUniform.m_eType = UT_MATRIX4X4; break;
		case GL_SAMPLER_2D: oUniform.m_eType = UT_SAMPLER; break;
		default: TUnimplemented();
		}

		oUniform.m_iUniform = glGetUniformLocation((GLuint)m_iProgram, m_asUniformNames[i].c_str());
	}

	for (auto it = m_aParameters.begin(); it != m_aParameters.end(); it++)
	{
		for (size_t j = 0; j < it->second.m_aActions.size(); j++)
		{
			auto it2 = m_aUniforms.find(it->second.m_aActions[j].m_sName.c_str());
			TAssert(it2 != m_aUniforms.end());
			if (it2 == m_aUniforms.end())
			{
				TError("Shader '" + m_sName + "' specifies a uniform '" + it->second.m_aActions[j].m_sName + "' that is not in the linked program.\n");
				continue;
			}

			CShader::CUniform& oUniform = it2->second;

			// This is almost cheating
			CData d;
			d.SetValue(it->second.m_aActions[j].m_sValue);

			if (oUniform.m_eType == UT_FLOAT)
				it->second.m_aActions[j].m_flValue = d.GetValueFloat();
			else if (oUniform.m_eType == UT_VECTOR2D)
				it->second.m_aActions[j].m_vec2Value = d.GetValueVector2D();
			else if (oUniform.m_eType == UT_VECTOR3D)
				it->second.m_aActions[j].m_vecValue = d.GetValueVector();
			else if (oUniform.m_eType == UT_VECTOR4D)
				it->second.m_aActions[j].m_vec4Value = d.GetValueVector4D();
			else if (oUniform.m_eType == UT_INT)
				it->second.m_aActions[j].m_iValue = d.GetValueInt();
			else if (oUniform.m_eType == UT_BOOL)
				it->second.m_aActions[j].m_bValue = d.GetValueBool();
			else if (oUniform.m_eType == UT_MATRIX4X4)
			{
				TUnimplemented();
			}
			else if (oUniform.m_eType == UT_SAMPLER)
			{
				// No op.
			}
			else
				TUnimplemented();
		}
	}

	for (auto it = m_aDefaults.begin(); it != m_aDefaults.end(); it++)
	{
		auto it2 = m_aUniforms.find(it->first.c_str());
		TAssert(it2 != m_aUniforms.end());
		if (it2 == m_aUniforms.end())
		{
			TError("Shader '" + m_sName + "' specifies a default for uniform '" + it->second.m_sName + "' that is not in the linked program.\n");
			continue;
		}

		CShader::CUniform& oUniform = it2->second;
		oUniform.m_pDefault = &it->second;

		// Again with the cheating.
		CData d;
		d.SetValue(it->second.m_sValue);

		if (oUniform.m_eType == UT_FLOAT)
			it->second.m_flValue = d.GetValueFloat();
		else if (oUniform.m_eType == UT_VECTOR2D)
			it->second.m_vec2Value = d.GetValueVector2D();
		else if (oUniform.m_eType == UT_VECTOR3D)
			it->second.m_vecValue = d.GetValueVector();
		else if (oUniform.m_eType == UT_VECTOR4D)
			it->second.m_vec4Value = d.GetValueVector4D();
		else if (oUniform.m_eType == UT_INT)
			it->second.m_iValue = d.GetValueInt();
		else if (oUniform.m_eType == UT_BOOL)
			it->second.m_bValue = d.GetValueBool();
		else if (oUniform.m_eType == UT_MATRIX4X4)
		{
			TUnimplemented(); 
		}
		else if (oUniform.m_eType == UT_SAMPLER)
		{
			TUnimplemented(); // Can't set a default texture... yet.
		}
		else
			TUnimplemented();
	}

	m_aDefaultsBuffer.clear();

	for (auto it = m_aUniforms.begin(); it != m_aUniforms.end(); it++)
	{
		CShader::CUniform& pUniformName = it->second;
		CShader::CParameter::CUniform* pUniform = it->second.m_pDefault;

		if (pUniform)
		{
			if (pUniformName.m_eType == UT_FLOAT)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_FLOAT, pUniform->m_flValue));
			else if (pUniformName.m_eType == UT_VECTOR2D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, pUniform->m_vec2Value));
			else if (pUniformName.m_eType == UT_VECTOR3D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, pUniform->m_vecValue));
			else if (pUniformName.m_eType == UT_VECTOR4D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, pUniform->m_vec4Value));
			else if (pUniformName.m_eType == UT_INT)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_INT, pUniform->m_iValue));
			else if (pUniformName.m_eType == UT_BOOL)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_BOOL, pUniform->m_bValue));
			else if (pUniformName.m_eType == UT_MATRIX4X4)
				TUnimplemented();
			else if (pUniformName.m_eType == UT_SAMPLER)
				TUnimplemented();
			else
				TUnimplemented();
		}
		else
		{
			if (pUniformName.m_eType == UT_FLOAT)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_FLOAT, 0.0f));
			else if (pUniformName.m_eType == UT_VECTOR2D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, Vector2D()));
			else if (pUniformName.m_eType == UT_VECTOR3D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, Vector()));
			else if (pUniformName.m_eType == UT_VECTOR4D)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, Vector4D()));
			else if (pUniformName.m_eType == UT_INT)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_INT, 0));
			else if (pUniformName.m_eType == UT_BOOL)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_BOOL, false));
			else if (pUniformName.m_eType == UT_MATRIX4X4)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, Matrix4x4()));
			else if (pUniformName.m_eType == UT_SAMPLER)
				m_aDefaultsBuffer.push_back(CShaderDefault(it->first, UT_SAMPLER, 0));
			else
				TUnimplemented();
		}
	}

	return true;
}

void CShader::Destroy()
{
	glDetachShader((GLuint)m_iProgram, (GLuint)m_iVShader);
	glDetachShader((GLuint)m_iProgram, (GLuint)m_iFShader);
	glDeleteShader((GLuint)m_iVShader);
	glDeleteShader((GLuint)m_iFShader);
	glDeleteProgram((GLuint)m_iProgram);
}

uniform_type_t CShader::FindType(const tstring& sName) const
{
	auto it = m_aParameters.find(sName);
	TAssert(it != m_aParameters.end());
	if (it == m_aParameters.end())
		return UT_NONE;

	const CShader::CParameter* pShaderPar = &it->second;

	for (size_t j = 0; j < pShaderPar->m_aActions.size(); j++)
		if (pShaderPar->m_aActions[j].m_sValue == "[value]")
			return m_aUniforms.find(pShaderPar->m_aActions[j].m_sName.c_str())->second.m_eType;

	return UT_NONE;
}

size_t CShader::FindTextureByUniform(const tstring& sUniform) const
{
	for (size_t i = 0; i < m_asTextures.size(); i++)
	{
		if (m_asTextures[i] == sUniform)
			return i;
	}

	return (size_t)~0;
}

void ReloadShaders(class CCommand*, tvector<tstring>&, const tstring&)
{
	CShaderLibrary::CompileShaders();
	if (CShaderLibrary::Get()->IsCompiled())
		TMsg("Shaders reloaded.\n");
	else
		TMsg("Shaders compile failed. See shaders.txt\n");
}

CCommand shaders_reload("shaders_reload", ::ReloadShaders);
