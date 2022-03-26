#include "OIT.hlsli"

StructuredBuffer<FLStaticNode> g_FLBuffer : register(t0);
ByteAddressBuffer g_StartOffsetBuffer : register(t1);
Texture2D g_BackGround : register(t2);

#define MAX_SORTED_PIXELS 8

static FragmentData g_SortedPixels[MAX_SORTED_PIXELS];

// ʹ�ò����������ֵ�Ӵ�С
void SortPixelInPlace(int numPixels)
{
    FragmentData temp;
    for (int i = 1; i < numPixels; ++i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (g_SortedPixels[j].Depth < g_SortedPixels[j + 1].Depth)
            {
                temp = g_SortedPixels[j];
                g_SortedPixels[j] = g_SortedPixels[j + 1];
                g_SortedPixels[j + 1] = temp;
            }
            else
            {
                break;
            }
        }
    }
}



float4 PS(float4 posH : SV_Position) : SV_Target
{
    // ȡ����ǰ����λ�ö�Ӧ�ı���ɫ
    float4 currColor = g_BackGround.Load(int3(posH.xy, 0));
    
    // ȡ����ǰ����λ��������
    uint2 vPos = (uint2) posH.xy;
    int startOffsetAddress = 4 * (g_FrameWidth * vPos.y + vPos.x);
    int numPixels = 0;
    uint offset = g_StartOffsetBuffer.Load(startOffsetAddress);
    
    FLStaticNode element;
    
    // ȡ���������нڵ�
    while (offset != 0xFFFFFFFF)
    {
        // ����ǰ����ȡ������
        element = g_FLBuffer[offset];
        // �����ؿ�������ʱ����
        g_SortedPixels[numPixels++] = element.Data;
        // ȡ����һ���ڵ�������������ֻȡ��ǰMAX_SORTED_PIXELS��
        offset = (numPixels >= MAX_SORTED_PIXELS) ?
            0xFFFFFFFF : element.Next;
    }
    
    // ������ȡ��������ƬԪ�����ֵ�Ӵ�С����
    SortPixelInPlace(numPixels);
    
    // ʹ��SrcAlpha-InvSrcAlpha���
    for (int i = 0; i < numPixels; ++i)
    {
        // ���������ɫ�������
        float4 pixelColor = UnpackColorFromUInt(g_SortedPixels[i].Color);
        // ���л��
        currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
    }
    
    // �����ֹ���ϵ���ɫ
    return currColor;
}