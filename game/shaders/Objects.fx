#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture entSkin1;
float4 vecTime;
float4 vecSkill41;

sampler sTexture = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };

Texture ColorLUT_bmap;
sampler sLUT = sampler_state
{
	Texture = <ColorLUT_bmap>; 
	AddressU = Clamp;
	AddressV = Clamp; 
	MipFilter = Linear;
};

float ColorVariation_flt;

struct out_ps // Output to the pixelshader fragment
{
	float4 Pos		: POSITION;
	float2 uv       : TEXCOORD0;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	
	Out.Pos = DoTransform(inPos);
	Out.uv = inTexCoord0;
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float3 blendmap = tex2D(sTexture, In.uv).rgb;
	
	float4 a = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (vecSkill41.x - 0.5)/64.0));
	float4 b = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (vecSkill41.x + 0.5)/64.0));
	float4 c = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (vecSkill41.x + 1.5)/64.0));
	return a * blendmap.r + b * blendmap.g + c * blendmap.b + vecSkill41.w;
}


technique object
{
	pass one
	{
		ZWriteEnable = True;
		AlphaBlendEnable = False;
		
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
