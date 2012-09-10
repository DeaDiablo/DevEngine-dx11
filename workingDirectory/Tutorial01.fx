
cbuffer cb0: register (b0)
{
    matrix	ViewProjection;
};

cbuffer cb1: register (b1)
{
    matrix	World;
};

cbuffer cb2: register (b2)
{
    float4	eyePos;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Normal: NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Normal: NORMAL0;
    float2 TexCoord : TEXCOORD0;
   float3 v_Light : 		TEXCOORD2;
   float3 v_View:   		TEXCOORD3;
};

PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;
	output.Pos = mul(mul(ViewProjection, World), input.Pos);
	output.Normal = mul(World, input.Normal);
	output.TexCoord = input.TexCoord;
    float3 p_Pos	= mul(input.Pos,World);
    output.v_Light	= float3(0.0f,50.0f, 0.0f)-p_Pos;
    output.v_View	= eyePos.xyz-p_Pos;
    return output;
}
