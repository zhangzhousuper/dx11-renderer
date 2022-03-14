#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Effects.h"
#include "Vertex.h"

class GameApp : public D3DApp
{
public:
	enum class Mode { SplitedTriangle, SplitedSnow, SplitedSphere };
	
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitResource();

	void ResetTriangle();
	void ResetRoundWire();
	void ResetSplitedTriangle();
	void ResetSplitedSnow();
	void ResetSplitedSphere();


private:
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // 单色笔刷
	ComPtr<IDWriteFont> m_pFont;								// 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// 文本格式
	
	ComPtr<ID3D11Buffer> m_pVertexBuffers[7];					// 顶点缓冲区数组
	int m_InitVertexCounts;									    // 初始顶点数目
	int m_CurrIndex;											// 当前索引
	Mode m_ShowMode;											// 当前显示模式
	bool m_IsWireFrame;										    // 是否为线框模式
	bool m_ShowNormal;										    // 是否显示法向量

	BasicEffect m_BasicEffect;							        // 对象绘制特效管理类

};


#endif