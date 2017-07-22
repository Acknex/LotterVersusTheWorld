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

struct out_ps // Output to the pixelshader fragment
{
	float4 Pos		: POSITION;
	float2 uv       : TEXCOORD0;
	float3 WorldPos : TEXCOORD1;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	
	inPos.y += 10 * sin(0.005 * inPos.x + vecTime.w*0.032);
	inPos.y -= 10 * cos(0.005 * inPos.z + vecTime.w*0.028);
	
	float fCycle = vecTime.w*0.05;
	float fAmp = 0.002;
	float fMod = 1+25;
	inTexCoord0.x += fAmp*sin(inTexCoord0.y*fMod + fCycle);
	inTexCoord0.y -= fAmp*sin(inTexCoord0.x*fMod + fCycle);
	
	inPos.y += 5 * sin(inTexCoord0.x);
	inPos.y -= 5 * cos(inTexCoord0.y);
	
	Out.Pos = DoTransform(inPos);
	Out.uv = inTexCoord0;
	Out.WorldPos = mul(matWorld, float4(inPos.xyz, 1.0));
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float3 blendmap = pow(tex2D(sTexture, 10 * In.uv).rgb * tex2D(sTexture, In.uv).rgb, 0.5);
	float fac = blendmap.r;
	
	float hl0 = 1-pow(tex2D(sTexture, 2 * In.uv + float2(0.0008 * vecTime.w, -0.0002 * vecTime.w)).r, 3);
	float hl1 = 1-pow(tex2D(sTexture, 2 * In.uv + float2(0.0012 * vecTime.w,  0.0003 * vecTime.w)).r, 3);
	float highlight = hl0 * hl1;
	
	float4 b = tex2D(sLUT, float2(0.5 * saturate(highlight), 2.5/64.0));
	float4 a = tex2D(sLUT, float2(0.5 * blendmap.r, 1.5/64.0));
	return a+b;

	// return float4(fac,fac,fac, 0);

	// return float4(lerp(a, b, fac), 0.0);
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
