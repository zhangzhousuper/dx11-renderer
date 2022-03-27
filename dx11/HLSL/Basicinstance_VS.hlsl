#include "Basic.hlsli"

// ¶¥µã×ÅÉ«Æ÷
VertexOutBasic VS(InstancePosNormalTex vIn)
{
    VertexOutBasic vOut;
    
    vector posW = mul(float4(vIn.PosL, 1.0f), vIn.World);
    matrix viewProj = mul(g_View, g_Proj);

    vOut.PosW = posW.xyz;
    vOut.PosH = mul(posW, viewProj);
    vOut.NormalW = mul(vIn.NormalL, (float3x3) vIn.WorldInvTranspose);
    vOut.Tex = vIn.Tex;
    vOut.ShadowPosH = mul(posW, g_ShadowTransform);
    return vOut;
}
