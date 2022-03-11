#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>


class RenderStates
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static bool IsInit();

	static void InitAll(ID3D11Device* device);
	// 使用ComPtr无需手工释放

public:
	static ComPtr<ID3D11RasterizerState> RSWireframe;		            // 光栅化器状态：线框模式
	static ComPtr<ID3D11RasterizerState> RSNoCull;			            // 光栅化器状态：无背面裁剪模式
	static ComPtr<ID3D11RasterizerState> RSCullClockWise;	            // 光栅化器状态：顺时针裁剪模式
	static ComPtr<ID3D11RasterizerState> RSDepthBias;					// 光栅化器状态：深度偏移模式

	static ComPtr<ID3D11SamplerState> SSPointClamp;						// 采样器状态：点过滤与Clamp模式
	static ComPtr<ID3D11SamplerState> SSLinearWrap;			            // 采样器状态：线性过滤与Wrap模式
	static ComPtr<ID3D11SamplerState> SSAnistropicWrap16x;		            // 采样器状态：各向异性过滤与Wrap模式
	static ComPtr<ID3D11SamplerState> SSShadow;							// 采样器状态：深度比较与Border模式

	static ComPtr<ID3D11BlendState> BSTransparent;		                // 混合状态：透明混合
	static ComPtr<ID3D11BlendState> BSAlphaToCoverage;	                // 混合状态：Alpha-To-Coverage
	static ComPtr<ID3D11BlendState> BSAdditive;			                // 混合状态：加法混合

	static ComPtr<ID3D11DepthStencilState> DSSEqual;					// 深度/模板状态：仅允许绘制深度值相等的像素
	static ComPtr<ID3D11DepthStencilState> DSSGreaterEqual;             // 深度/模板状态：用于反向Z绘制
	static ComPtr<ID3D11DepthStencilState> DSSNoDepthTest;              // 深度/模板状态：关闭深度测试
	static ComPtr<ID3D11DepthStencilState> DSSWriteStencil;		        // 深度/模板状态：无深度测试，写入模板值
	static ComPtr<ID3D11DepthStencilState> DSSEqualStencil;	            // 深度/模板状态：反向Z，检测模板值
};
