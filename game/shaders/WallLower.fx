#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
Texture mtlSkin2;
Texture ColorLUT_bmap;
float4 vecTime;
float ColorVariation_flt;

sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sDetails = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; AddressV = Clamp; };
sampler sLava = sampler_state { Texture = <mtlSkin2>; MipFilter = Linear; };
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
	Out.uv = Out.WorldPos.xz*0.0001;
	
	float fCycle = vecTime.w*0.05;
	float fAmp = 0.002;
	float fMod = 1+25;
	Out.uv.x += fAmp*sin(Out.uv.y*fMod + fCycle);
	Out.uv.y -= fAmp*sin(Out.uv.x*fMod + fCycle);
	
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float height = -In.WorldPos.y*0.0025;
	float2 patternUV = 0.0013 * float2(In.WorldPos.x - In.WorldPos.z, In.WorldPos.y);
	float lineValue = tex2D(sDetails, float2(patternUV.x, height)).a;
	
	float lutPosition = ColorVariation_flt * 0.5;
	float3 color2 = tex2D(sLUT, float2(lutPosition, 10.5/64.0));
	float3 color1 = tex2D(sLUT, float2(lutPosition, 11.5/64.0));
	float3 color3 = tex2D(sLUT, float2(lutPosition, 12.5/64.0));
	
	float3 color = lerp(color1, color2, tex2D(sTexture, patternUV).r);
	color += lineValue * color3;
	
	float3 stuff = tex2D(sTexture, float2(patternUV.x, height)).rgb;
	
	
	//Lava color
	float fac = tex2D(sLava, 10 * In.uv + float2(0.002 * vecTime.w, 0.000125 * vecTime.w)).r;
	float fac2 = tex2D(sLava, 8 * In.uv + float2(0.00175 * vecTime.w, -0.000125 * vecTime.w)).r;
	float fac3 = tex2D(sLava, 1 * In.uv + float2(0.000174 * vecTime.w, 0)).r;
	fac = clamp(fac * (fac2 * 2) * (fac3 * 4),0,1);
	
	float hl0 = 1.0 - tex2D(sLava, 2 * In.uv + float2(0.0004 * vecTime.w, -0.0001 * vecTime.w)).r;
	float hl1 = 1.0 - tex2D(sLava, 2 * In.uv + float2(0.0006 * vecTime.w,  0.0003 * vecTime.w)).r;
	
	float highlight = hl0 * hl1;
	float4 a = tex2D(sLUT, float2(0.5 * fac, 1.5/64.0));
	float b = tex2D(sLUT, float2(0.5 * saturate(highlight), 2.5/64.0)).a;
	float lava = a.a+(b*0.4) * saturate(length(a));
	
	float4 lightColor = tex2D(sLUT, float2(lutPosition, 3.5/64.0));
	float brightness = stuff.g;
	brightness += lava * 0.5 * max(height + lava * 0.1 - 0.4, 0.0); // 0.1 ist stärke, hinten höhe
	color += lightColor * brightness;
	return float4(color, lineValue + brightness);
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
