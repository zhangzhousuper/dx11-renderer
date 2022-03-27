#include "SkyRender.h"
#include "Geometry.h"
#include "d3dUtil.h"

#pragma warning(disable: 26812)

using namespace DirectX;
using namespace Microsoft::WRL;

HRESULT SkyRender::InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::wstring& cubemapFilename, float skySphereRadius, bool generateMips)
{
	// ��ֹ�ظ���ʼ������ڴ�й©
	m_pIndexBuffer.Reset();
	m_pVertexBuffer.Reset();
	m_pTextureCubeSRV.Reset();

	HRESULT hr;
	// ��պ��������
	if (cubemapFilename.substr(cubemapFilename.size() - 3) == L"dds")
	{
		hr = CreateDDSTextureFromFile(device,
			generateMips ? deviceContext : nullptr,
			cubemapFilename.c_str(),
			nullptr,
			m_pTextureCubeSRV.GetAddressOf());
	}
	else
	{
		hr = CreateWICTexture2DCubeFromFile(device,
			deviceContext,
			cubemapFilename,
			nullptr,
			m_pTextureCubeSRV.GetAddressOf(),
			generateMips);
	}

	if (FAILED(hr))
		return hr;

	return InitResource(device, skySphereRadius);
}

HRESULT SkyRender::InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubemapFilenames, float skySphereRadius, bool generateMips)
{
	// ��ֹ�ظ���ʼ������ڴ�й©
	m_pIndexBuffer.Reset();
	m_pVertexBuffer.Reset();
	m_pTextureCubeSRV.Reset();

	HRESULT hr;
	// ��պ��������
	hr = CreateWICTexture2DCubeFromFile(device,
		deviceContext,
		cubemapFilenames,
		nullptr,
		m_pTextureCubeSRV.GetAddressOf(),
		generateMips);
	if (FAILED(hr))
		return hr;

	return InitResource(device, skySphereRadius);
}

ID3D11ShaderResourceView* SkyRender::GetTextureCube()
{
	return m_pTextureCubeSRV.Get();
}

void SkyRender::Draw(ID3D11DeviceContext* deviceContext, SkyEffect& skyEffect, const Camera& camera)
{
	UINT strides[1] = { sizeof(XMFLOAT3) };
	UINT offsets[1] = { 0 };
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), strides, offsets);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Ĩ��ƽ�Ʒ���������������ƶ�������պж���
	XMMATRIX V = camera.GetViewXM();
	V.r[3] = g_XMIdentityR3;
	
	skyEffect.SetWorldMatrix(XMMatrixIdentity());
	skyEffect.SetViewMatrix(V);
	skyEffect.SetProjMatrix(camera.GetProjXM());

	skyEffect.SetTextureCube(m_pTextureCubeSRV.Get());
	skyEffect.Apply(deviceContext);
	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}

void SkyRender::SetDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	// ����տ��ܴ��ڵ�����
	D3D11SetDebugObjectName(m_pTextureCubeSRV.Get(), nullptr);

	D3D11SetDebugObjectName(m_pTextureCubeSRV.Get(), name + ".CubeMapSRV");
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), name + ".VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), name + ".IndexBuffer");
#else
	UNREFERENCED_PARAMETER(name);
#endif
}

HRESULT SkyRender::InitResource(ID3D11Device* device, float skySphereRadius)
{
	HRESULT hr;
	auto sphere = Geometry::CreateSphere<VertexPos>(skySphereRadius);

	// ���㻺��������
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * (UINT)sphere.vertexVec.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = sphere.vertexVec.data();

	hr = device->CreateBuffer(&vbd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// ��������������
	m_IndexCount = (UINT)sphere.indexVec.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indexVec.data();

	return device->CreateBuffer(&ibd, &InitData, &m_pIndexBuffer);
}

HRESULT DynamicSkyRender::InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::wstring& cubemapFilename,
	float skySphereRadius, int dynamicCubeSize, bool generateMips)
{
	// ��ֹ�ظ���ʼ������ڴ�й©
	m_pCacheRTV.Reset();
	m_pCacheDSV.Reset();
	m_pDynamicCubeMapDSV.Reset();
	m_pDynamicCubeMapSRV.Reset();
	for (auto& ptr : m_pDynamicCubeMapRTVs)
	{
		ptr.Reset();
	}

	HRESULT hr;
	hr = SkyRender::InitResource(device, deviceContext, cubemapFilename, skySphereRadius, generateMips);
	if (FAILED(hr))
		return hr;
	return DynamicSkyRender::InitResource(device, dynamicCubeSize);
}

HRESULT DynamicSkyRender::InitResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubemapFilenames,
	float skySphereRadius, int dynamicCubeSize, bool generateMips)
{
	// ��ֹ�ظ���ʼ������ڴ�й©
	m_pCacheRTV.Reset();
	m_pCacheDSV.Reset();
	m_pDynamicCubeMapDSV.Reset();
	m_pDynamicCubeMapSRV.Reset();
	for (auto& ptr : m_pDynamicCubeMapRTVs)
	{
		ptr.Reset();
	}

	HRESULT hr;
	hr = SkyRender::InitResource(device, deviceContext, cubemapFilenames, skySphereRadius, generateMips);
	if (FAILED(hr))
		return hr;
	return DynamicSkyRender::InitResource(device, dynamicCubeSize);
}

void DynamicSkyRender::Cache(ID3D11DeviceContext* deviceContext, BasicEffect& effect)
{
	deviceContext->OMGetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.GetAddressOf());
	// ���������ɫ���Ķ�̬��պУ���Ҫ������Ч
	effect.SetTextureCube(nullptr);
	effect.Apply(deviceContext);
}

void DynamicSkyRender::BeginCapture(ID3D11DeviceContext* deviceContext, BasicEffect& effect, const XMFLOAT3& pos,
	D3D11_TEXTURECUBE_FACE face, float nearZ, float farZ)
{
	static XMFLOAT3 ups[6] = {
		{ 0.0f, 1.0f, 0.0f },	// +X
		{ 0.0f, 1.0f, 0.0f },	// -X
		{ 0.0f, 0.0f, -1.0f },	// +Y
		{ 0.0f, 0.0f, 1.0f },	// -Y
		{ 0.0f, 1.0f, 0.0f },	// +Z
		{ 0.0f, 1.0f, 0.0f }	// -Z
	};

	static XMFLOAT3 looks[6] = {
		{ 1.0f, 0.0f, 0.0f },	// +X
		{ -1.0f, 0.0f, 0.0f },	// -X
		{ 0.0f, 1.0f, 0.0f },	// +Y
		{ 0.0f, -1.0f, 0.0f },	// -Y
		{ 0.0f, 0.0f, 1.0f },	// +Z
		{ 0.0f, 0.0f, -1.0f },	// -Z
	};

	// ������պ������
	m_pCamera.LookTo(pos, looks[face], ups[face]);

	// ���ﾡ���ܲ������������
	m_pCamera.SetFrustum(XM_PIDIV2, 1.0f, nearZ, farZ);

	// Ӧ�ù۲����ͶӰ����
	effect.SetViewMatrix(m_pCamera.GetViewXM());
	effect.SetProjMatrix(m_pCamera.GetProjXM());

	// ��ջ�����
	deviceContext->ClearRenderTargetView(m_pDynamicCubeMapRTVs[face].Get(), reinterpret_cast<const float*>(&Colors::Black));
	deviceContext->ClearDepthStencilView(m_pDynamicCubeMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// ������ȾĿ������ģ����ͼ
	deviceContext->OMSetRenderTargets(1, m_pDynamicCubeMapRTVs[face].GetAddressOf(), m_pDynamicCubeMapDSV.Get());
	// �����ӿ�
	D3D11_VIEWPORT viewPort = m_pCamera.GetViewPort();
	deviceContext->RSSetViewports(1, &viewPort);
}

void DynamicSkyRender::Restore(ID3D11DeviceContext* deviceContext, BasicEffect& effect, const Camera& camera)
{
	// �ָ�Ĭ���趨
	D3D11_VIEWPORT viewPort = camera.GetViewPort();
	deviceContext->RSSetViewports(1, &viewPort);
	deviceContext->OMSetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.Get());

	// ���ɶ�̬��պк����Ҫ����mipmap��
	deviceContext->GenerateMips(m_pDynamicCubeMapSRV.Get());

	effect.SetViewMatrix(camera.GetViewXM());
	effect.SetProjMatrix(camera.GetProjXM());
	// �ָ��󶨵Ķ�̬��պ�
	effect.SetTextureCube(m_pDynamicCubeMapSRV.Get());

	// �����ʱ�������ȾĿ����ͼ�����ģ����ͼ
	m_pCacheDSV.Reset();
	m_pCacheRTV.Reset();
}

ID3D11ShaderResourceView* DynamicSkyRender::GetDynamicTextureCube()
{
	return m_pDynamicCubeMapSRV.Get();
}

const Camera& DynamicSkyRender::GetCamera() const
{
	return m_pCamera;
}

void DynamicSkyRender::SetDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	SkyRender::SetDebugObjectName(name);
	D3D11SetDebugObjectName(m_pDynamicCubeMapSRV.Get(), name + ".dynamicCubeMapSRV");
	D3D11SetDebugObjectName(m_pDynamicCubeMapDSV.Get(), name + ".dynamicCubeMapDSV");
	for (size_t i = 0; i < 6; ++i)
	{
		D3D11SetDebugObjectName(m_pDynamicCubeMapRTVs[i].Get(), name + ".dynamicCubeMapRTVs[" + std::to_string(i) + "]");
	}
#else
	UNREFERENCED_PARAMETER(name);
#endif
}

HRESULT DynamicSkyRender::InitResource(ID3D11Device* device, int dynamicCubeSize)
{
	HRESULT hr;
	// ******************
	// 1. ������������
	//

	ComPtr<ID3D11Texture2D> texCube;
	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = dynamicCubeSize;
	texDesc.Height = dynamicCubeSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	// ����texCube�����½�����
	hr = device->CreateTexture2D(&texDesc, nullptr, texCube.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// ******************
	// 2. ������ȾĿ����ͼ
	//

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;
	// һ����ͼֻ��Ӧһ����������Ԫ��
	rtvDesc.Texture2DArray.ArraySize = 1;

	// ÿ��Ԫ�ش���һ����ȾĿ����ͼ
	for (int i = 0; i < 6; ++i)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateRenderTargetView(texCube.Get(), &rtvDesc,
			m_pDynamicCubeMapRTVs[i].GetAddressOf());
		if (FAILED(hr))
			return hr;
	}

	// ******************
	// 3. ������ɫ����Դ��ͼ
	//

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;	// ʹ�����е�mip�ȼ�

	hr = device->CreateShaderResourceView(texCube.Get(), &srvDesc,
		m_pDynamicCubeMapSRV.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// ******************
	// 4. �������/ģ�建�������Ӧ����ͼ
	//

	texDesc.Width = dynamicCubeSize;
	texDesc.Height = dynamicCubeSize;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthTex;
	hr = device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(
		depthTex.Get(),
		&dsvDesc,
		m_pDynamicCubeMapDSV.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// ******************
	// 5. ��ʼ���ӿ�
	//

	m_pCamera.SetViewPort(0.0f, 0.0f, static_cast<float>(dynamicCubeSize), static_cast<float>(dynamicCubeSize));

	return S_OK;
}