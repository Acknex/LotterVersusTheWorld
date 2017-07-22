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
	
	Out.WorldPos = mul(matWorld, float4(inPos.xyz, 1.0));
	
	float fCycle = vecTime.w*0.05;
	float fAmp = 0.002;
	float fMod = 1+25;
	Out.uv = Out.WorldPos.xz*0.0001;
	Out.uv.x += fAmp*sin(Out.uv.y*fMod + fCycle);
	Out.uv.y -= fAmp*sin(Out.uv.x*fMod + fCycle);
	
	inPos.y += 5 * sin(Out.uv.x);
	inPos.y -= 5 * cos(Out.uv.y);
	
	Out.Pos = DoTransform(inPos);
	return Out;
}

float4 ps(out_ps In): COLOR
{
	//float fac = pow(tex2D(sTexture, 10 * In.uv).r * tex2D(sTexture, In.uv).r, 0.5);
	float fac = tex2D(sTexture, 10 * In.uv + float2(0.004 * vecTime.w, 0.0005 * vecTime.w)).r;
	float fac2 = tex2D(sTexture, 8 * In.uv + float2(0.0035 * vecTime.w, -0.0005 * vecTime.w)).r;
	float fac3 = tex2D(sTexture, 1 * In.uv + float2(0.00035 * vecTime.w, 0)).r;
	fac = clamp(fac * (fac2 * 2) * (fac3 * 4),0,1);
	
	float hl0 = 1.0 - tex2D(sTexture, 2 * In.uv + float2(0.0008 * vecTime.w, -0.0001 * vecTime.w)).r;
	float hl1 = 1.0 - tex2D(sTexture, 2 * In.uv + float2(0.0012 * vecTime.w,  0.0003 * vecTime.w)).r;
	
	float highlight = hl0 * hl1;
	
	float4 a = tex2D(sLUT, float2(0.5 * fac, 1.5/64.0));
	float4 b = tex2D(sLUT, float2(0.5 * saturate(highlight), 2.5/64.0));
	return a+(b*0.4) * saturate(length(a));
}

technique object
{
	pass one
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
		AlphaBlendEnable = false;
	}
}

// fallback if nothing works
technique fallback { pass one { } }
