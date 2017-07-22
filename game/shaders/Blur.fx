Texture TargetMap;
sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

float4 vecViewPort;
float4 vecSkill1;

const float kernel[11] = {0.000003, 0.000229, 0.005977, 0.060598, 0.24173, 0.382925, 0.24173, 0.060598, 0.005977, 0.000229, 0.000003};

float4 blur(float2 Tex : TEXCOORD0) : COLOR0 
{
   float4 color = 0.0;
   
   for(int i = -5; i <= 5; i++)
   {
   	color += tex2D(g_samSrcColor, Tex.xy + vecSkill1.xy * i * vecViewPort.zw) * kernel[i + 5];
	}
	
	return color;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 blur();
	}
}