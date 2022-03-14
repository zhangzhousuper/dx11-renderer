#include "Basic.hlsli"

[maxvertexcount(9)]
void GS(triangle VertexPosColor input[3], inout TriangleStream<VertexPosColor> output)
{
	//
    // 将一个三角形分裂成三个三角形，即没有v3v4v5的三角形
    //       v1
    //       /\
    //      /  \
    //   v3/____\v4
    //    /\xxxx/\
    //   /  \xx/  \
    //  /____\/____\
    // v0    v5    v2


    VertexPosColor vertexes[6];
    int i;
    [unroll]
    for (i = 0; i < 3; ++i)
    {
        vertexes[i] = input[i];
        vertexes[i + 3].Color = (input[i].Color + input[(i + 1) % 3].Color) / 2.0f;
        vertexes[i + 3].PosL = (input[i].PosL + input[(i + 1) % 3].PosL) / 2.0f;
    }

    [unroll]
    for (i = 0; i < 3; ++i)
    {
        output.Append(vertexes[i]);
        output.Append(vertexes[3 + i]);
        output.Append(vertexes[(i + 2) % 3 + 3]);

        output.RestartStrip();
    }
}