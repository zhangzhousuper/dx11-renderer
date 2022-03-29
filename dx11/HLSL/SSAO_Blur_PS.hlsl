#include "SSAO.hlsli"

// ˫���˲�
float4 PS(VertexPosHTex pIn, uniform bool horizontalBlur) : SV_Target
{
    // �������������
    float blurWeights[12] = (float[12]) g_BlurWeights;
    
    float2 texOffset;
    if (horizontalBlur)
    {
        texOffset = float2(1.0f / g_InputImage.Length.x, 0.0f);
    }
    else
    {
        texOffset = float2(0.0f, 1.0f / g_InputImage.Length.y);
    }
    
    // ���ǰ�����ֵ�ӽ�ȥ����
    float4 color = blurWeights[g_BlurRadius] * g_InputImage.SampleLevel(g_SamBlur, pIn.Tex, 0.0f);
    float totalWeight = blurWeights[g_BlurRadius];
    
    float4 centerNormalDepth = g_NormalDepthMap.SampleLevel(g_SamBlur, pIn.Tex, 0.0f);
    // �ֲ���۲�ռ�ķ����������
    float3 centerNormal = centerNormalDepth.xyz;
    float centerDepth = centerNormalDepth.w;
    
    for (float i = -g_BlurRadius; i <= g_BlurRadius; ++i)
    {
        // �����Ѿ�������ֵ�ӽ�ȥ��
        if (i == 0)
            continue;
        
        float2 tex = pIn.Tex + i * texOffset;
        
        float4 neighborNormalDepth = g_NormalDepthMap.SampleLevel(g_SamBlur, tex, 0.0f);
        // �ֲ�������������
        float3 neighborNormal = neighborNormalDepth.xyz;
        float neighborDepth = neighborNormalDepth.w;
        
        //
        // �������ֵ������ֵ����Ȼ��������̫�����Ǿ���Ϊ��ǰ�����㴦�ڱ�Ե����
        // ��˲����Ǽ��뵱ǰ����ֵ
        //
        
        if (dot(neighborNormal, centerNormal) >= 0.8f && abs(neighborDepth - centerDepth) <= 0.2f)
        {
            float weight = blurWeights[i + g_BlurRadius];
            
            // ���������ؼ������ģ��
            color += weight * g_InputImage.SampleLevel(g_SamBlur, tex, 0.0f);
            totalWeight += weight;
        }
        
    }

    // ͨ������Ȩֵ��Ϊ1�����������Ĳ�������
    return color / totalWeight;
}
