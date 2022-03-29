#include "SSAO.hlsli"

// ������r��p����Ȳ�����������q�Ե�p���ڱγ̶�
float OcclusionFunction(float distZ)
{
    //
    // ���depth(q)��depth(p)֮��(��������Χ)���ǵ�q�����ڱε�p�����⣬���depth(q)��depth(p)���ڽӽ���
    // ����Ҳ��Ϊ��q�����ڱε�p����Ϊdepth(p)-depth(r)��Ҫ�����û��ٶ���Epsilonֵ������Ϊ��q�����ڱε�p
    //
    // ����������ĺ�����ȷ���ڱγ̶�
    //
    //    /|\ Occlusion
    // 1.0 |      ---------------\
    //     |      |             |  \
    //     |                         \
    //     |      |             |      \
    //     |                             \
    //     |      |             |          \
    //     |                                 \
    // ----|------|-------------|-------------|-------> zv
    //     0     Eps          zStart         zEnd
    float occlusion = 0.0f;
    if (distZ > g_SurfaceEpsilon)
    {
        float fadeLength = g_OcclusionFadeEnd - g_OcclusionFadeStart;
        // ��distZ��g_OcclusionFadeStart��������g_OcclusionFadeEnd���ڱ�ֵ��1���Լ�С��0
        occlusion = saturate((g_OcclusionFadeEnd - distZ) / fadeLength);
    }
    return occlusion;
}


// ����SSAOͼ�Ķ�����ɫ��
float4 PS(VertexOut pIn, uniform int sampleCount) : SV_TARGET
{
    // p -- ����Ҫ����Ļ������ڱ�Ŀ���
    // n -- ����p�ķ�����
    // q -- ��p�����ڰ����ڵ����һ��
    // r -- �п����ڵ���p��һ��
    
    // ��ȡ�۲�ռ�ķ������͵�ǰ���ص�z����
    float4 normalDepth = g_NormalDepthMap.SampleLevel(g_SamNormalDepth, pIn.Tex, 0.0f);
    
    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;
    
    //
    // �ؽ��۲�ռ����� (x, y, z)
    // Ѱ��tʹ���ܹ����� p = t * pIn.ToFarPlane
    // p.z = t * pIn.ToFarPlane.z
    // t = p.z / pIn.ToFarPlane.z
    //
    float3 p = (pz / pIn.ToFarPlane.z) * pIn.ToFarPlane;
    
    // ��ȡ�����������[0, 1]^3ӳ�䵽[-1, 1]^3
    float3 randVec = g_RandomVecMap.SampleLevel(g_SamRandomVec, 4.0f * pIn.Tex, 0.0f).xyz;
    randVec = 2.0f * randVec - 1.0f;
    
    float occlusionSum = 0.0f;
    
    // ����pΪ���ĵİ����ڣ����ݷ���n��p��Χ�ĵ���в���
    for (int i = 0; i < sampleCount; ++i)
    {
        // ƫ���������ǹ̶��Ҿ��ȷֲ���(�������ǲ��õ�ƫ������������ͬһ����������)��
        // ������ǽ���Щƫ������������һ������������з��䣬��õ��ıض�Ϊһ����ȷֲ�
        // �����ƫ������
        float3 offset = reflect(g_OffsetVectors[i].xyz, randVec);
        
        // ���ƫ������λ��(p, n)�����ƽ��֮�󣬽��䷭ת
        float flip = sign(dot(offset, n));
        
        // �ڵ�p�����ڱΰ뾶�İ���Χ�ڽ��в���
        float3 q = p + flip * g_OcclusionRadius * offset;
    
        // ��q����ͶӰ���õ�ͶӰ��������
        float4 projQ = mul(float4(q, 1.0f), g_ViewToTexSpace);
        projQ /= projQ.w;
        
        // �ҵ��۾��۲��q�������ܹ۲쵽�������r���������ֵ(�п��ܵ�r�����ڣ���ʱ�۲쵽
        // ����Զƽ����һ��)��Ϊ�ˣ�������Ҫ�鿴�˵������ͼ�е����ֵ
        float rz = g_NormalDepthMap.SampleLevel(g_SamNormalDepth, projQ.xy, 0.0f).w;
        
        // �ؽ���r�ڹ۲�ռ��е����� r = (rx, ry, rz)
        // ����֪����rλ���۾�����q�������ϣ�����r = t * q
        // r.z = t * q.z ==> t = t.z / q.z
        float3 r = (rz / q.z) * q;
        
        // ���Ե�r�Ƿ��ڱ�p
        //   - ���dot(n, normalize(r - p))�����ڱε�r��ƽ��(p, n)ǰ��ľ��롣Խ�ӽ���
        //     ��ƽ���ǰ�࣬���Ǿ͸����趨Խ����ڱ�Ȩ�ء�ͬʱ����Ҳ�ܷ�ֹλ����б��
        //     (p, n)��һ��r������Ӱ��������������ڱ�ֵ(ͨ������g_SurfaceEpsilon)����
        //     ����Ϊ���Թ۲����ӽ��������������Ų�ͬ�����ֵ������ʵ�ϣ�λ����б��
        //     (p, n)�ϵĵ�rȴû���ڵ�Ŀ���p
        //   - �ڱ�Ȩ�صĴ�Сȡ�����ڱε�����Ŀ���֮��ľ��롣����ڱε�r��Ŀ���p��
        //     Զ������Ϊ��r�����ڵ���p
        
        float distZ = p.z - r.z;
        float dp = max(dot(n, normalize(r - p)), 0.0f);
        float occlusion = dp * OcclusionFunction(distZ);
        
        occlusionSum += occlusion;
    }
    
    occlusionSum /= sampleCount;
    
    float access = 1.0f - occlusionSum;
    
    // ��ǿSSAOͼ�ĶԱȶȣ��ǵ�SSAOͼ��Ч����������
    return saturate(pow(access, 4.0f));
}