//***************************************************************************************
// SkyRender.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// ��պм�������Ⱦ��
// Skybox loader and render classes.
//***************************************************************************************

#ifndef SKYRENDER_H
#define SKYRENDER_H

#include <vector>
#include <string>
#include "Effects.h"
#include "Camera.h"

class SkyRender
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SkyRender() = default;
	~SkyRender() = default;
	// ���������������ƶ�
	SkyRender(const SkyRender&) = delete;
	SkyRender& operator=(const SkyRender&) = delete;
	SkyRender(SkyRender&&) = default;
	SkyRender& operator=(SkyRender&&) = default;


	// ��Ҫ�ṩ��������պ���ͼ ���� �Ѿ������õ���պ�����.dds�ļ�
	HRESULT InitResource(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const std::wstring& cubemapFilename,
		float skySphereRadius,		// �����뾶
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps

	// ��Ҫ�ṩ��պе�������������ͼ
	HRESULT InitResource(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const std::vector<std::wstring>& cubemapFilenames,
		float skySphereRadius,		// �����뾶
		bool generateMips = false);	// Ĭ�ϲ�Ϊ��̬��պ�����mipmaps

	ID3D11ShaderResourceView* GetTextureCube();

	void Draw(ID3D11DeviceContext* deviceContext, SkyEffect& skyEffect, const Camera& camera);

	// ���õ��Զ�����
	void SetDebugObjectName(const std::string& name);

private:
	HRESULT InitResource(ID3D11Device* device, float skySphereRadius);


private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	UINT m_IndexCount = 0;

	ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;
};

#endif