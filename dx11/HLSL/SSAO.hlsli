Texture2D g_DiffuseMap : register(t0);
Texture2D g_NormalDepthMap : register(t1);
Texture2D g_RandomVecMap : register(t2);
Texture2D g_InputImage : register(t3);

SamplerState g_SamLinearWrap : register(s0);
SamplerState g_SamNormalDepth : register(s1);
SamplerState g_SamRandomVec : register(s2);
SamplerState g_SamBlur : register(s3); // MIG_MAG_LINEAR_MIP_POINT CLAMP

cbuffer CBChangesEveryObjectDrawing : register(b0)
{
    //
    // ����SSAO_NormalDepth
    //
    matrix g_WorldView;
    matrix g_WorldViewProj;
    matrix g_WorldInvTransposeView;
}

cbuffer CBChangesEveryFrame : register(b1)
{
    matrix g_View;
}

cbuffer CBChangesOnResize : register(b2)
{
    matrix g_Proj;
    
    //
    // ����SSAO
    //
    matrix g_ViewToTexSpace; // Proj * Texture
    float4 g_FrustumCorners[4]; // ��׶��Զƽ���4���˵�
}

cbuffer CBChangesRarely : register(b3)
{
    // 14��������ȷֲ����������������
    float4 g_OffsetVectors[14];
    
    // �۲�ռ��µ�����
    float g_OcclusionRadius = 0.5f;
    float g_OcclusionFadeStart = 0.2f;
    float g_OcclusionFadeEnd = 2.0f;
    float g_SurfaceEpsilon = 0.05f;
    
    //
    // ����SSAO_Blur
    //
    float4 g_BlurWeights[3] =
    {
        float4(0.05f, 0.05f, 0.1f, 0.1f),
        float4(0.1f, 0.2f, 0.1f, 0.1f),
        float4(0.1f, 0.05f, 0.05f, 0.0f)
    };
    
    int g_BlurRadius = 5;
    int3 g_Pad;
};

//
// ����SSAO_NormalDepth��SSAO_Blur
//
struct VertexPosNormalTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct InstancePosNormalTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    matrix World : World;
    matrix WorldInvTranspose : WorldInvTranspose;
};

struct VertexPosHVNormalVTex
{
    float4 PosH : SV_POSITION;
    float3 PosV : POSITION;
    float3 NormalV : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct VertexPosHTex
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

//
// ����SSAO
//
struct VertexIn
{
    float3 PosL : POSITION;
    float3 ToFarPlaneIndex : NORMAL; // ��ʹ��x���������ж���׶��Զƽ�涥�������
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0; // Զƽ�涥������
    float2 Tex : TEXCOORD1;
};
