#include "SSAO.hlsli"

PatchTess PatchHS(InputPatch<TessVertexOut, 3> patch,
                  uint patchID : SV_PrimitiveID)
{
    PatchTess pt;
	
    // ��ÿ���ߵ�����ϸ��������ƽ��ֵ����ѡ������һ���ߵ���Ϊ���ڲ���
    // ����ϸ�����ӡ����ڱߵ���������������ϸ�����ӵļ���ǳ���Ҫ����
    // ����Щ���������ι���ı߽���ӵ����ͬ������ϸ�����ӣ�����ᵼ
    // �¼�϶�Ĳ���
    pt.EdgeTess[0] = 0.5f * (patch[1].TessFactor + patch[2].TessFactor);
    pt.EdgeTess[1] = 0.5f * (patch[2].TessFactor + patch[0].TessFactor);
    pt.EdgeTess[2] = 0.5f * (patch[0].TessFactor + patch[1].TessFactor);
    pt.InsideTess = pt.EdgeTess[0];
	
    return pt;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
HullOut HS(InputPatch<TessVertexOut, 3> p,
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
    HullOut hOut;
	
	// ֱ��
    hOut.PosW = p[i].PosW;
    hOut.NormalW = p[i].NormalW;
    hOut.Tex = p[i].Tex;
	
    return hOut;
}
