cbuffer cb0: register (b0)
{
    matrix	viewproj;
};

cbuffer cb1: register (b1)
{
    matrix	world;
};

float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
    return mul(mul(viewproj, world), Pos);
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}
