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
	
	float4 attributes = tex2D(sTexture, In.uv);
	
	//float ioffset = 7 * floor((In.WorldPos.x - 100) / 200) + floor((In.WorldPos.z - 100) / 200);
	//float toff = 0.03 * vecTime.w;
	//
	//   toff *= 2.3;
	//  	toff -= 0.3;
	//toff -= frac(toff);
	//
	//ioffset += toff;
	
	float2 textcoord = patternUV;// + float2(0, 0.111 * ioffset);
	
	textcoord.x -= 0.0005 * vecTime.w;
	
	textcoord.x -= 0.0009 * (1.2 + sin(0.3 * floor(textcoord.y * 17))) * vecTime.w;
	
	// return float4(frac(0.1 * ioffset), 0, 0, 1);
	
	float4 text = tex2D(sWallTest, textcoord);
	
	//return float4(text, 1);
	
	float4 col1 = tex2D(sLUT, float2(0.5 * saturate(ColorVariation_flt), 21.0/64.0));
	float4 col2 = tex2D(sLUT, float2(0.5 * saturate(ColorVariation_flt), 22.0/64.0));
	
	float4 color = 
		col1 * attributes.r +
		col2 * attributes.g +
		text * attributes.b;
	
	return color;
}


technique object
{
	pass one
	{
		sampler[0] = (sTexture);
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_3_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
