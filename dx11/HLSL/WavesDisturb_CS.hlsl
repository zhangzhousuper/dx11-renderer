#include "Waves.hlsli"

[numthreads(1, 1, 1)]
void CS(uint DTid : SV_DispatchThreadID)
{
    // ���ǲ���Ҫ���б߽���飬��Ϊ:
    // --��ȡ�����߽��������Ϊ0�������ǶԱ߽紦�������һ��
    // --�Գ����߽������д�벢����ִ��
    uint x = g_DisturbIndex.x;
    uint y = g_DisturbIndex.y;
    
    float halfMag = 0.5f * g_DisturbMagnitude;
    
    // RW����Դ�����д������+=�������
    g_Output[uint2(x, y)] += g_DisturbMagnitude;
    g_Output[uint2(x + 1, y)] += halfMag;
    g_Output[uint2(x - 1, y)] += halfMag;
    g_Output[uint2(x, y + 1)] += halfMag;
    g_Output[uint2(x, y - 1)] += halfMag;
}