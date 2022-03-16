#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjReader.h"
#include "Collision.h"

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
	void CreateRandomTrees();
	
private:
	
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // 单色笔刷
	ComPtr<IDWriteFont> m_pFont;								// 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// 文本格式

	GameObject m_Trees;										    // 树
	GameObject m_Ground;										// 地面
	std::vector<Transform> m_InstancedData;						// 树的实例数据

	BasicEffect m_BasicEffect;								    // 对象渲染特效管理
	bool m_EnableFrustumCulling;								// 视锥体裁剪开启
	bool m_EnableInstancing;									// 硬件实例化开启

	std::shared_ptr<Camera> m_pCamera;						    // 摄像机
	CameraMode m_CameraMode;									// 摄像机模式

	ObjReader m_ObjReader;									    // 模型读取对象
};


#endif