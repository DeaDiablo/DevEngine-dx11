struct VS_INPUT
{
    float4 Pos : POSITION; 
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT input )
{
    VS_OUTPUT output;

    output.Pos = input.Pos;
    output.Tex = output.Pos * 0.5f + 0.5f;
    output.Tex.y = 1.0f - output.Tex.y;   

    return output;
}

