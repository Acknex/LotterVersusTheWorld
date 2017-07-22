Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;
float4 vecSkill1;

float4 threshold(float2 Tex : TEXCOORD0) : COLOR0 
{
   float4 color = 1.0//;//tex2D( g_samSrcColor, Tex.xy);
   float luminance = dot(float4(0.299, 0.587, 0.114, 0.0), color);
   if(luminance < vecSkill1.x) color = 0.0;
	return color;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_3_0 threshold();
	}
}