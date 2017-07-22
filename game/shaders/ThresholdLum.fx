Texture TargetMap;
sampler g_samSrcColor = sampler_state
{
	Texture = <TargetMap>; 
	AddressU = Clamp;
	AddressV = Clamp; 
	MipFilter = Linear;
};

float4 vecViewPort;
float4 vecSkill1;

float4 threshold(float2 Tex : TEXCOORD0) : COLOR0 
{
	float4 color = tex2D( g_samSrcColor, Tex.xy);
	return color * color.a;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 threshold();
	}
}