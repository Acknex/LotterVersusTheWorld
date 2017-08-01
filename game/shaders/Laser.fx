#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture entSkin1;
float4 vecTime;
float4 vecSkill41;

sampler sTexture = sampler_state { Texture = <entSkin1>; AddressU = Wrap; AddressV = Clamp; MipFilter = Linear; };

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
	float a1 = tex2D(sTexture, float2((In.uv.x - 0.8*vecTime.w)*vecSkill41.w, In.uv.y)).r;
	float a2 = tex2D(sTexture, float2((In.uv.x - 0.4*vecTime.w)*vecSkill41.w, In.uv.y)).g;
	float a3 = tex2D(sTexture, float2((In.uv.x - 0.2*vecTime.w)*vecSkill41.w, In.uv.y)).b;
	float f = (a1 + a2 + a3)/3.0;
	float g = saturate(f-0.95)/0.05;
	return float4(f, g, g, max(f,g));
}


technique object
{
	pass one
	{
		ZWriteEnable = False;
		AlphaBlendEnable = False;
		
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
