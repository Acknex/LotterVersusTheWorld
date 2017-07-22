#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

float4 vecTime;
float4 entSkill41;

Texture mtlSkin1;
sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };

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
	float3 localPos : TEXCOORD2;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	Out.Pos = DoTransform(inPos);
	Out.uv = inTexCoord0;
	Out.localPos = inPos.xyz;
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float4 col1 = tex2D(sTexture,float2(atan2(In.localPos.x, In.localPos.z), 0.01 * In.localPos.y - 0.01 * entSkill41.x * vecTime.w));
	float4 col2 = tex2D(sTexture,float2(atan2(In.localPos.x, In.localPos.z) + 0.001 * vecTime.w, 0.01 * In.localPos.y - 0.012 * entSkill41.x *  vecTime.w));
	
	float blendout = clamp(2.5 - 0.01 * In.localPos.y, 0, 1);
	
	return pow(saturate(1.1 * min(col1, col2) - 0.2), 1.5) * blendout;
}


technique object
{
	pass one
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
		AlphaBlendEnable = true;
		SrcBlend = One;  
        DestBlend = One;   
		ZWriteEnable = false;
		CullMode = None;
	}
}

// fallback if nothing works
technique fallback { pass one { } }
