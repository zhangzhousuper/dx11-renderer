#include "SSAO.hlsli"

// ���ɹ۲�ռ�ķ����������ֵ��RTT��������ɫ��
float4 PS(VertexPosHVNormalVTex pIn, uniform bool alphaClip) : SV_TARGET
{
    // ������������׼��
    pIn.NormalV = normalize(pIn.NormalV);
    
    if (alphaClip)
    {
        float4 g_TexColor = g_DiffuseMap.Sample(g_SamLinearWrap, pIn.Tex);
        
        clip(g_TexColor.a - 0.1f);
    }
    
    // ���ع۲�ռ�ķ����������ֵ
    return float4(pIn.NormalV, pIn.PosV.z);
}