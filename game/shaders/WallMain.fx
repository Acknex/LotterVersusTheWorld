#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture entSkin1;
float4 vecTime;

sampler sTexture = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };

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
	float3 color = tex2D(sTexture, In.uv).a * float3(0.7, 0.3, 0.0);
	return float4(color, 1.0);
}


technique object
{
	pass one
	{
		sampler[0] = (sTexture);
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
