#include "SSAO.hlsli"
// ���ɹ۲�ռ�ķ����������ֵ��RTT�Ķ�����ɫ��
VertexPosHVNormalVTex VS(InstancePosNormalTex vIn)
{
    VertexPosHVNormalVTex vOut;
    
    vector posW = mul(float4(vIn.PosL, 1.0f), vIn.World);
    matrix viewProj = mul(g_View, g_Proj);
    matrix worldView = mul(vIn.World, g_View);
    matrix worldInvTransposeView = mul(vIn.WorldInvTranspose, g_View);
    
    // �任���۲�ռ�
    vOut.PosV = mul(float4(vIn.PosL, 1.0f), worldView).xyz;
    vOut.NormalV = mul(vIn.NormalL, (float3x3) worldInvTransposeView);
    
    // �任���ü��ռ�
    vOut.PosH = mul(posW, viewProj);
    
    vOut.Tex = vIn.Tex;
    
    return vOut;
}