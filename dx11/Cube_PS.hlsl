// Cube_PS.hlsl
#include "Cube.hlsli"

float4 PS(VertexOut pIn) : SV_Target
{
    return pIn.color;
}