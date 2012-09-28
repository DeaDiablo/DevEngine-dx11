Texture2D texSa : register( t0 );
SamplerState samplerLinear : register( s0 );

cbuffer cb0: register (b0)
{
    matrix ViewProjection;
};

cbuffer cb1: register (b1)
{
    matrix World;
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
};

struct PS_OUT {
    float4 diff : SV_TARGET0;
    float4 norm : SV_TARGET1;
};

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(mul(ViewProjection, World), float4(input.Pos.xyz, 1.0f));
    output.Normal = mul(World, input.Normal);
    output.TexCoord = input.TexCoord;
    return output;
}

PS_OUT ps_main(PS_INPUT input)
{
    PS_OUT output;
    output.diff = texSa.Sample(samplerLinear, input.TexCoord.xy);
    output.norm.xyz = normalize(input.Normal) * 0.5f + 0.5f;
    output.norm.w  = /*ambient and matID*/1.0f;
    return output;
}
