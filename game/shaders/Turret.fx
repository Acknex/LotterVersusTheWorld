#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
Texture mtlSkin2;
Texture entSkin1;
sampler sReflection = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sTexture = sampler_state { Texture = <mtlSkin2>; MipFilter = Linear; };
sampler sDetails = sampler_state { Texture = <entSkin1>; MipFilter = Linear; };

float4 vecSkill41;

Texture ColorLUT_bmap;
float ColorVariation_flt;
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
	float2 uv       : TEXCOORD0;
	float3 projection : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	Out.Pos = DoTransform(inPos);
	Out.uv = inTexCoord0;
	Out.projection = Out.Pos.xyw;
	Out.worldPos = mul(matWorld, float4(inPos.xyz, 1.0));
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float2 texcoords = In.projection.xy / In.projection.z;
	texcoords.xy *= 0.5;
	texcoords.xy += 0.5;
	
	float4 col1 = tex2D(sLUT, float2(0.5 * saturate(ColorVariation_flt), 15.5/64.0));
	float4 col2 = tex2D(sLUT, float2(0.5 * saturate(ColorVariation_flt), 16.5/64.0));
	float4 col3 = tex2D(sLUT, float2(0.5 * saturate(ColorVariation_flt), 50.5/64.0));
	float3 attributes = tex2D(sTexture, In.worldPos.xz / 200 + 0.5);
	
	attributes += tex2D(sDetails, In.uv);
	
	float4 floorcol = 
		col1 * attributes.r +
		col2 * attributes.g;
	
	float4 reflection = tex2D(sReflection, texcoords);
	return float4(reflection.rgb * 0.6 + floorcol.rgb, 0.0) + col3 * attributes.b * vecSkill41.x;
}


technique object
{
	pass one
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
		AlphaBlendEnable = false;
		ZWriteEnable = true;
	}
}

// fallback if nothing works
technique fallback { pass one { } }
