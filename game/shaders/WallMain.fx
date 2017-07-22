#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
Texture mtlSkin2;
float4 vecTime;

sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };

sampler sWallTest = sampler_state { Texture = <mtlSkin2>; MipFilter = Linear; };

struct out_ps // Output to the pixelshader fragment
{
	float4 Pos		: POSITION;
	float3 WorldPos : TEXCOORD1;
	float2 uv       : TEXCOORD0;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	Out.Pos = DoTransform(inPos);
	Out.WorldPos = mul(matWorld, float4(inPos.xyz, 1.0));
	Out.uv = inTexCoord0;
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float2 patternUV = float2(0.001, 0.004) * float2(In.WorldPos.x - In.WorldPos.z, -In.WorldPos.y);
	
	float3 attributes = tex2D(sTexture, In.uv).rgb;
	
	float3 text = tex2D(sWallTest, patternUV);
	
	//return float4(text, 1);
	
	float3 color = lerp(
		0.0,
		text,
		attributes.b);
	
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
