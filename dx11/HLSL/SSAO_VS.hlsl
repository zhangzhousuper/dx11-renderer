#include "SSAO.hlsli"

// ����SSAOͼ�Ķ�����ɫ��
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    
    // �Ѿ���NDC�ռ�
    vOut.PosH = float4(vIn.PosL, 1.0f);
    
    // ����������x������������׶��Զƽ��Ķ�������
    vOut.ToFarPlane = g_FrustumCorners[vIn.ToFarPlaneIndex.x].xyz;
    
    vOut.Tex = vIn.Tex;
    
    return vOut;
}