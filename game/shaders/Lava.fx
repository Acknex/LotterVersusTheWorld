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
	return Out;
}

float4 ps(out_ps In): COLOR
{
	return float4(pow(tex2D(sTexture, 10 * In.uv).rgb * tex2D(sTexture, In.uv).rgb, 0.5), 0.0);
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
