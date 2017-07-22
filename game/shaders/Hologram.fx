#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture entSkin1;
float4 vecTime;

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
	float3 worldPos : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0,
	float3 inNormal : NORMAL)
{
	out_ps Out;
	
	Out.Pos = DoTransform(inPos);
	Out.uv = inTexCoord0;
	Out.worldPos = mul(matWorld, inPos);
	Out.normal = mul(matWorld, float4(inNormal, 0.0));
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float3 a = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (20.5)/64.0)).rgb;
	
	float3 camDir = normalize(vecViewPos - In.worldPos);
	float fresnel = dot(camDir, normalize(In.normal));
	fresnel *= fresnel;
	return float4(a * fresnel, fresnel);
}


technique object
{
	pass one
	{
		ZWriteEnable = False;
		AlphaBlendEnable = True;
		DestBlend = One;
		
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
