#include "SSAO.hlsli"

[domain("tri")]
VertexPosHVNormalVTex DS(PatchTess patchTess,
             float3 bary : SV_DomainLocation,
             const OutputPatch<HullOut, 3> tri)
{
    VertexPosHVNormalVTex dOut;
    
    // ����Ƭ���Խ��в�ֵ�����ɶ���
    float3 posW = bary.x * tri[0].PosW + bary.y * tri[1].PosW + bary.z * tri[2].PosW;
    float3 normalW = bary.x * tri[0].NormalW + bary.y * tri[1].NormalW + bary.z * tri[2].NormalW;
    dOut.Tex = bary.x * tri[0].Tex + bary.y * tri[1].Tex + bary.z * tri[2].Tex;
    
    // �Բ�ֵ��ķ��������б�׼��
    normalW = normalize(normalW);
    
    //
    // λ��ӳ��
    //
    
    // ���������������ľ���ѡȡmipmap�ȼ����ر�أ���ÿ��MipInterval��λѡ����һ��mipLevel
    // Ȼ��mipLevel������[0, 6]
    const float MipInterval = 20.0f;
    float mipLevel = clamp((distance(posW, g_EyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);
    
    // �Ը߶�ͼ���������ڷ�����ͼ��alphaͨ����
    float h = g_NormalMap.SampleLevel(g_SamLinearWrap, dOut.Tex, mipLevel).a;
    
    // ���ŷ���������ƫ��
    posW += (g_HeightScale * (h - 1.0f)) * normalW;
    
    // �任���۲�ռ�
    dOut.PosV = mul(float4(posW, 1.0f), g_View).xyz;
    dOut.NormalV = mul(normalW, (float3x3) g_View);
    
    // ͶӰ����βü��ռ�
    dOut.PosH = mul(float4(posW, 1.0f), g_ViewProj);
    
    return dOut;
}