#include "SSAO.hlsli"

// ����SSAOͼ�Ķ�����ɫ��
VertexPosHTex VS(VertexPosNormalTex vIn)
{
    VertexPosHTex vOut;
    
    // �Ѿ���NDC�ռ�
    vOut.PosH = float4(vIn.PosL, 1.0f);
    
    vOut.Tex = vIn.Tex;
    
    return vOut;
}