#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "LightHelper.h"
#include "Camera.h"
#include "RenderStates.h"
#include "GameObject.h"

class GameApp : public D3DApp
{
public:
	// 摄像机模式
	enum class CameraMode { FirstPerson, ThirdPerson, Free };

public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitResource();

private:

	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // 单色笔刷
	ComPtr<IDWriteFont> m_pFont;								// 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// 文本格式

	GameObject m_BoltAnim;									    // 闪电动画
	GameObject m_WoodCrate;									    // 木盒
	GameObject m_Floor;										    // 地板
	std::vector<GameObject> m_Walls;							// 墙壁
	GameObject m_Mirror;										// 镜面

	std::vector<ComPtr<ID3D11ShaderResourceView>> mBoltSRVs;    // 闪电动画纹理

	Material m_ShadowMat;									    // 阴影材质
	Material m_WoodCrateMat;									// 木盒材质

	BasicEffect m_BasicEffect;								    // 对象渲染特效管理

	std::shared_ptr<Camera> m_pCamera;						    // 摄像机
	CameraMode m_CameraMode;									// 摄像机模式

};


