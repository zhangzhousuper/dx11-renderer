#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <DirectXColors.h>
#include "d3dApp.h"
#include "ScreenGrab.h"
class GameApp : public D3DApp
{
public:
	// 摄像机模式
	enum class CameraMode { FirstPerson, ThirdPerson, Free };
	// 天空盒模式
	enum class SkyBoxMode { Daylight, Sunset, Desert };
	// 球体当前模式
	enum class SphereMode { None, Reflection, Refraction };
	// 地面模式
	enum class GroundMode { Floor, Stones };
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void Compute();

private:
	bool InitResource();
	
private:
	ComPtr<ID3D11ComputeShader> m_pTextureMul_R32G32B32A32_CS;
	ComPtr<ID3D11ComputeShader> m_pTextureMul_R8G8B8A8_CS;
	
	ComPtr<ID3D11ShaderResourceView> m_pTextureInputA;
	ComPtr<ID3D11ShaderResourceView> m_pTextureInputB;

	ComPtr<ID3D11Texture2D> m_pTextureOutputA;
	ComPtr<ID3D11Texture2D> m_pTextureOutputB;
	ComPtr<ID3D11UnorderedAccessView> m_pTextureOutputA_UAV;
	ComPtr<ID3D11UnorderedAccessView> m_pTextureOutputB_UAV;
};


#endif