#pragma once


#include <vector>
#include <string>
#include "Vertex.h"
#include "Effects.h"
#include "Transform.h"

class WavesRender
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	void SetMaterial(const Material& material);

	Transform& GetTransform();
	const Transform& GetTransform() const;

	UINT RowCount() const;
	UINT ColumnCount() const;

protected:
	// ������ֱ�ӹ���WavesRender�����CpuWavesRender��GpuWavesRender����
	WavesRender() = default;
	~WavesRender() = default;
	// ���������������ƶ�
	WavesRender(const WavesRender&) = delete;
	WavesRender& operator=(const WavesRender&) = delete;
	WavesRender(WavesRender&&) = default;
	WavesRender& operator=(WavesRender&&) = default;

	void Init(
		UINT rows,			// ��������
		UINT cols,			// ��������
		float texU,			// ��������U�������ֵ
		float texV,			// ��������V�������ֵ
		float timeStep,		// ʱ�䲽��
		float spatialStep,	// �ռ䲽��
		float waveSpeed,	// ����
		float damping,		// ճ��������
		float flowSpeedX,	// ˮ��X�����ٶ�
		float flowSpeedY);	// ˮ��Y�����ٶ�
	
protected:
	UINT m_NumRows = 0;					// ��������
	UINT m_NumCols = 0;					// ��������

	UINT m_VertexCount = 0;				// ������Ŀ
	UINT m_IndexCount = 0;				// ������Ŀ

	Transform m_Transform = {};			// ˮ��任
	DirectX::XMFLOAT2 m_TexOffset = {};	// ��������ƫ��
	float m_TexU = 0.0f;				// ��������U�������ֵ
	float m_TexV = 0.0f;				// ��������V�������ֵ
	Material m_Material = {};			// ˮ�����

	float m_FlowSpeedX = 0.0f;			// ˮ��X�����ٶ�
	float m_FlowSpeedY = 0.0f;			// ˮ��Y�����ٶ�
	float m_TimeStep = 0.0f;			// ʱ�䲽��
	float m_SpatialStep = 0.0f;			// �ռ䲽��
	float m_AccumulateTime = 0.0f;		// �ۻ�ʱ��

	//
	// ���ǿ���Ԥ�ȼ�������ĳ���
	//

	float m_K1 = 0.0f;
	float m_K2 = 0.0f;
	float m_K3 = 0.0f;
};

class CpuWavesRender : public WavesRender
{
public:
	CpuWavesRender() = default;
	~CpuWavesRender() = default;
	// ���������������ƶ�
	CpuWavesRender(const CpuWavesRender&) = delete;
	CpuWavesRender& operator=(const CpuWavesRender&) = delete;
	CpuWavesRender(CpuWavesRender&&) = default;
	CpuWavesRender& operator=(CpuWavesRender&&) = default;

	HRESULT InitResource(ID3D11Device* device,
		const std::wstring& texFileName,	// �����ļ���
		UINT rows,			// ��������
		UINT cols,			// ��������
		float texU,			// ��������U�������ֵ
		float texV,			// ��������V�������ֵ
		float timeStep,		// ʱ�䲽��
		float spatialStep,	// �ռ䲽��
		float waveSpeed,	// ����
		float damping,		// ճ��������
		float flowSpeedX,	// ˮ��X�����ٶ�
		float flowSpeedY);	// ˮ��Y�����ٶ�

	void Update(float dt);

	// �ڶ���[i][j]������߶�Ϊmagnitude�Ĳ���
	// ��������1 < i < rows��1 < j < cols�ķ�Χ�ڼ���
	void Disturb(UINT i, UINT j, float magnitude);
	// ����ˮ��
	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect);

	void SetDebugObjectName(const std::string& name);

private:
	std::vector<VertexPosNormalTex> m_Vertices;			// ���浱ǰģ�����Ķ����ά�����һάչ��
	std::vector<VertexPos> m_PrevSolution;				// ������һ��ģ�����Ķ���λ�ö�ά�����һάչ��

	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// ��ǰģ��Ķ��㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// ��ǰģ�������������

	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuse;	// ˮ������
	bool m_isUpdated = false;							// ��ǰ�Ƿ��ж������ݸ���
};

class GpuWavesRender : public WavesRender
{
public:
	GpuWavesRender() = default;
	~GpuWavesRender() = default;
	// ���������������ƶ�
	GpuWavesRender(const GpuWavesRender&) = delete;
	GpuWavesRender& operator=(const GpuWavesRender&) = delete;
	GpuWavesRender(GpuWavesRender&&) = default;
	GpuWavesRender& operator=(GpuWavesRender&&) = default;

	// Ҫ�󶥵��������������ܱ�16�������Ա�֤�����ж���
	// �Ķ��㱻���뵽�µ��߳��鵱��
	HRESULT InitResource(ID3D11Device* device,
		const std::wstring& texFileName,	// �����ļ���
		UINT rows,			// ��������
		UINT cols,			// ��������
		float texU,			// ��������U�������ֵ
		float texV,			// ��������V�������ֵ
		float timeStep,		// ʱ�䲽��
		float spatialStep,	// �ռ䲽��
		float waveSpeed,	// ����
		float damping,		// ճ��������
		float flowSpeedX,	// ˮ��X�����ٶ�
		float flowSpeedY);	// ˮ��Y�����ٶ�

	void Update(ID3D11DeviceContext* deviceContext, float dt);

	// �ڶ���[i][j]������߶�Ϊmagnitude�Ĳ���
	// ��������1 < i < rows��1 < j < cols�ķ�Χ�ڼ���
	void Disturb(ID3D11DeviceContext* deviceContext, UINT i, UINT j, float magnitude);
	// ����ˮ��
	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect);

	void SetDebugObjectName(const std::string& name);

private:
	struct {
		DirectX::XMFLOAT4 waveInfo;
		DirectX::XMINT4 index;
	} m_CBUpdateSettings = {};								// ��ӦWaves.hlsli�ĳ���������

private:
	ComPtr<ID3D11Texture2D> m_pNextSolution;				// ������һ��ģ������yֵ��ά����
	ComPtr<ID3D11Texture2D> m_pCurrSolution;				// ���浱ǰģ������yֵ��ά����
	ComPtr<ID3D11Texture2D> m_pPrevSolution;				// ������һ��ģ������yֵ��ά����

	ComPtr<ID3D11ShaderResourceView> m_pNextSolutionSRV;	// ������һ��ģ������yֵ��ɫ����Դ��ͼ
	ComPtr<ID3D11ShaderResourceView> m_pCurrSolutionSRV;	// ���浱ǰģ������yֵ��ɫ����Դ��ͼ
	ComPtr<ID3D11ShaderResourceView> m_pPrevSolutionSRV;	// ������һ��ģ������yֵ��ɫ����Դ��ͼ

	ComPtr<ID3D11UnorderedAccessView> m_pNextSolutionUAV;	// ������һ��ģ������yֵ���������ͼ
	ComPtr<ID3D11UnorderedAccessView> m_pCurrSolutionUAV;	// ���浱ǰģ������yֵ���������ͼ
	ComPtr<ID3D11UnorderedAccessView> m_pPrevSolutionUAV;	// ������һ��ģ������yֵ���������ͼ

	ComPtr<ID3D11Buffer> m_pVertexBuffer;					// ��ǰģ��Ķ��㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;					// ��ǰģ�������������
	ComPtr<ID3D11Buffer> m_pConstantBuffer;					// ��ǰģ��ĳ���������

	ComPtr<ID3D11ComputeShader> m_pWavesUpdateCS;			// ���ڼ���ģ��������ɫ��
	ComPtr<ID3D11ComputeShader> m_pWavesDisturbCS;			// ���ڼ���ˮ������ɫ��

	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuse;		// ˮ������
};