Texture2D texSa : register( t0 );
SamplerState samplerLinear : register( s0 );


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float3 Normal: NORMAL0;
    float2 TexCoord : TEXCOORD0;
   float3 v_Light : 		TEXCOORD2;
   float3 v_View:   		TEXCOORD3;
};

float4 ps_main(PS_INPUT input) : SV_TARGET0
{
	float3 n = normalize(input.Normal);
    float3 d = texSa.Sample(samplerLinear, input.TexCoord).xyz;

    float3  l  = normalize (  input.v_Light );
    float3  v  = normalize (  input.v_View );
    float3  h  = normalize ( l + v );
    float3 diff = max ( dot ( n, l ), 0.2 );
    float3 spec = pow ( max ( dot ( n, h ), 0.0 ), 40.0 );

    return float4 ( diff * d + spec, 1.0f);
}
