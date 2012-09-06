Texture2D		textureRes : register( t0 );
SamplerState    linearSample : register( s0 );

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
    output.Tex = float2((output.Pos.xy + 1) * 0.5f);	
    output.Tex.y *= -1;
    
    return output;
}

float4 ps_main( VS_OUTPUT input ) : SV_Target
{
    return textureRes.Sample(linearSample, input.Tex);
}
