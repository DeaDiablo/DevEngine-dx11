//view matrix
cbuffer cb0: register (b0)
{
    matrix	ViewProjection;
};

cbuffer cb1: register (b1)
{
    matrix	World;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD0;
};

PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;
	output.Tex = input.Pos.xyz;
	output.Pos = mul(mul(ViewProjection, World), input.Pos).xyww;
    return output;
}
