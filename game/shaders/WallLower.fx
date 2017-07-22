#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
Texture ColorLUT_bmap;
float4 vecTime;
float ColorVariation_flt;

sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sDetails = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; AddressV = Clamp; };
sampler sLUT = sampler_state
{
	Texture = <ColorLUT_bmap>; 
	AddressU = Clamp;
	AddressV = Clamp; 
	MipFilter = Linear;
};

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
	float height = -In.WorldPos.y*0.003;
	float2 patternUV = 0.0013 * float2(In.WorldPos.x - In.WorldPos.z, In.WorldPos.y);
	float lineValue = tex2D(sDetails, float2(patternUV.x, height)).a;
	
	float lutPosition = ColorVariation_flt * 0.5;
	float3 color2 = tex2D(sLUT, float2(lutPosition, 10.5/64.0));
	float3 color1 = tex2D(sLUT, float2(lutPosition, 11.5/64.0));
	float3 color3 = tex2D(sLUT, float2(lutPosition, 12.5/64.0));
	
	float3 color = lerp(color1, color2, tex2D(sTexture, patternUV).r);
	color += lineValue * color3;
	color += max(height - 0.5, 0) * float3(1.0, 0.0, 0.0);
	return float4(color, lineValue);
}


technique object
{
	pass one
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
