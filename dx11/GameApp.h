#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <random>
#include "d3dApp.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjReader.h"
#include "Collision.h"
#include "SkyRender.h"
#include "TextureRender.h"

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
	void DrawScene(BasicEffect* pBasicEffect, bool enableNormalMap = false);
	void DrawScene(ShadowEffect* pShadowEffect);

private:
	bool InitResource();

private:

	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // 单色笔刷
	ComPtr<IDWriteFont> m_pFont;								// 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// 文本格式

	bool m_EnableNormalMap;										// 开启法线贴图
	bool m_EnableDebug;											// 开启调试模式
	bool m_GrayMode;											// 深度值以灰度形式显示
	int m_SlopeIndex;			     							// 斜率索引

	GameObject m_Ground;										// 地面
	GameObject m_GroundT;										// 带切线向量的地面

	GameObject m_Cylinder;										// 圆柱体
	GameObject m_CylinderT;										// 带切线向量的圆柱体
	std::vector<Transform> m_CylinderTransforms;				// 圆柱体变换信息

	GameObject m_Sphere;										// 球体
	GameObject m_SphereT;										// 带切线向量的球体
	std::vector<Transform> m_SphereTransforms;					// 球体变换信息

	GameObject m_House;											// 房屋

	GameObject m_DebugQuad;										// 调试用四边形
	GameObject m_FullScreenDebugQuad;							// 全屏调试用四边形

	DirectionalLight m_DirLights[3] = {};						// 方向光
	DirectX::XMFLOAT3 m_OriginalLightDirs[3] = {};				// 初始光方向

	std::unique_ptr<BasicEffect> m_pBasicEffect;				// 基础特效
	std::unique_ptr<ShadowEffect> m_pShadowEffect;				// 阴影特效
	std::unique_ptr<SkyEffect> m_pSkyEffect;					// 天空盒特效
	std::unique_ptr<DebugEffect> m_pDebugEffect;				// 调试用显示纹理的特效

	std::unique_ptr<TextureRender> m_pShadowMap;				// 阴影贴图
	std::unique_ptr<TextureRender> m_pGrayShadowMap;			// 用于调试的RGB阴影贴图
	std::unique_ptr<SkyRender> m_pDesert;						// 天空盒(沙漠)

	std::shared_ptr<Camera> m_pCamera;						    // 摄像机
};


#endif