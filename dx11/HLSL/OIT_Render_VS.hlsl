#include "OIT.hlsli"

// ¶¥µã×ÅÉ«Æ÷
float4 VS(float3 vPos : POSITION) : SV_Position
{
    return float4(vPos, 1.0f);
}