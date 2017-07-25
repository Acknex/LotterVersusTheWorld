#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
float4 vecTime;
float4 vecViewPort;
float fAlpha;

sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };

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
	float3 screenPos : TEXCOORD3;
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
	Out.normal = mul(matWorldView, float4(inNormal, 0.0));
	Out.screenPos = Out.Pos.xyw;
	return Out;
}

float4 ps(out_ps In): COLOR
{
/*	float3 a = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (20.5)/64.0)).rgb;
	
	float fresnel = -In.normal.z;
	fresnel *= fresnel;
	fresnel = 1.0-fresnel;*/
	
	//return float4(a * fresnel, fresnel)*fAlpha;
	
	float2 patternUV = In.screenPos.xy / In.screenPos.z;
	patternUV = patternUV * 0.5 + 0.5;
	patternUV.x *= vecViewPort.x / vecViewPort.y;
	//return float4(patternUV*4.0, 1, 0);
	
	//float2 patternUV = 2 * float2(In.worldPos.x, In.worldPos.y);
	float4 wireframe = tex2D(sTexture, patternUV * 60.0).r;
	float4 wireframe2 = tex2D(sTexture, patternUV * 60.0).r;
	
	//wireframe -= wireframe2 * 0.5;
	wireframe.a = 0.0;
	
	
	float4 baseColor = float4(0.7, 0.8, 0.5, 0.75);
	baseColor -= wireframe;
	
	return baseColor;
}


technique object
{
	pass one
	{
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		//DestBlend = One;
		//SrcBlend = One;
		
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
