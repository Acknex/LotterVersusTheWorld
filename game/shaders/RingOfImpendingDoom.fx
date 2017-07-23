Texture TargetMap;

sampler sScreen = sampler_state
{
	Texture = <TargetMap>; 
	AddressU = Clamp;
	AddressV = Clamp; 
	MipFilter = Linear;
};

float4 vecSkill1;
float4 vecSkill5;
float4 vecViewPort;

float4 bloomMix(float2 Tex : TEXCOORD0) : COLOR0 
{
	float4 color = tex2D(sScreen, Tex.xy);
	
	float2 pos = (Tex - vecSkill1.xy) * float2((vecViewPort.x / vecViewPort.y), 1);
	
	float distanceCenter = 2.0 * length(pos);
	
	float radius = vecSkill1.z;
	
	float distanceRing = abs(distanceCenter - radius);
	
	float ring = pow(1 - saturate(5 * distanceRing), 8);
	
	color.rgb += vecSkill5.a * vecSkill5.rgb * ring;
	
	return color;
}

technique PostProcess 
{
	pass p1 
	{
		AlphaBlendEnable = false;
		VertexShader = null;
		PixelShader = compile ps_2_0 bloomMix();
	}
}
