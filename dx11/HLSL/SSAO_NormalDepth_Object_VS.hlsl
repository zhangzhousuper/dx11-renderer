#include "SSAO.hlsli"

// ���ɹ۲�ռ�ķ����������ֵ��RTT�Ķ�����ɫ��
VertexPosHVNormalVTex VS(VertexPosNormalTex vIn)
{
    VertexPosHVNormalVTex vOut;
    
    // �任���۲�ռ�
    vOut.PosV = mul(float4(vIn.PosL, 1.0f), g_WorldView).xyz;
    vOut.NormalV = mul(vIn.NormalL, (float3x3) g_WorldInvTransposeView);
    
    // �任���ü��ռ�
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), g_WorldViewProj);
    
    vOut.Tex = vIn.Tex;
    
    return vOut;
}