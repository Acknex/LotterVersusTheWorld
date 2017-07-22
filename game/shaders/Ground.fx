#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
Texture mtlSkin2;
sampler sReflection = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };
sampler sTexture = sampler_state { Texture = <mtlSkin2>; MipFilter = Linear; };

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
	float3 color = tex2D(sReflection, texcoords);
	float3 floorcol = tex2D(sTexture, In.uv);
	float fresnel = normalize(vecViewPos.xyz - In.worldPos).y;
	fresnel = pow(fresnel, 3);
	color = lerp(floorcol, color, saturate(fresnel));
	
	return float4(color, 1.0);
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
