#pragma once

#include <d3d11_1.h>
#include <wrl/client.h>
#include <string>

class OITRender
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	OITRender() = default;
	~OITRender() = default;
	// ���������������ƶ�
	OITRender(const OITRender&) = delete;
	OITRender& operator=(const OITRender&) = delete;
	OITRender(OITRender&&) = default;
	OITRender& operator=(OITRender&&) = default;

	HRESULT InitResource(ID3D11Device* device,
		UINT width,			// ֡���
		UINT height,		// ֡�߶�
		UINT multiple);		// �ö��ٱ���֡�������Ļ������洢����ƬԪ
	
	// ��ʼ�ռ�͸����������ƬԪ
	void BeginDefaultStore(ID3D11DeviceContext* deviceContext);
	// �����ռ�����ԭ״̬
	void EndStore(ID3D11DeviceContext* deviceContext);

	// ��������͸����������ƬԪ������������Ⱦ
	void Draw(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* background);

	void SetDebugObjectName(const std::string& name);

private:
	struct {
		int width;
		int height;
		int pad1;
		int pad2;
	} m_CBFrame{};												// ��ӦOIT.hlsli�ĳ���������
private:
	ComPtr<ID3D11InputLayout> m_pInputLayout;					// ������Ļ�Ķ������벼��

	ComPtr<ID3D11Buffer> m_pFLBuffer;							// ƬԪ/���ӻ�����
	ComPtr<ID3D11Buffer> m_pStartOffsetBuffer;					// ��ʼƫ�ƻ�����
	ComPtr<ID3D11Buffer> m_pVertexBuffer;						// ���Ʊ����õĶ��㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;						// ���Ʊ����õ�����������
	ComPtr<ID3D11Buffer> m_pConstantBuffer;						// ����������

	ComPtr<ID3D11ShaderResourceView> m_pFLBufferSRV;			// ƬԪ/���ӻ���������ɫ����Դ��ͼ
	ComPtr<ID3D11ShaderResourceView> m_pStartOffsetBufferSRV;	// ��ʼƫ�ƻ���������ɫ����Դ��ͼ

	ComPtr<ID3D11UnorderedAccessView> m_pFLBufferUAV;			// ƬԪ/���ӻ����������������ͼ
	ComPtr<ID3D11UnorderedAccessView> m_pStartOffsetBufferUAV;	// ��ʼƫ�ƻ����������������ͼ

	ComPtr<ID3D11VertexShader> m_pOITRenderVS;					// ͸�������Ⱦ�Ķ�����ɫ��
	ComPtr<ID3D11PixelShader> m_pOITRenderPS;					// ͸�������Ⱦ��������ɫ��
	ComPtr<ID3D11PixelShader> m_pOITStorePS;					// ���ڴ洢͸������ƬԪ��������ɫ��

	ComPtr<ID3D11PixelShader> m_pCachePS;						// ��ʱ�����������ɫ��

	UINT m_FrameWidth = 0;										// ֡���ؿ��
	UINT m_FrameHeight = 0;										// ֡���ظ߶�
	UINT m_IndexCount = 0;										// ����������

};

