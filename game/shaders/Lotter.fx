const float4x4 matWorldViewProj;
const float4x4 matView;
const float4x4 matWorld;
const float4 vecViewDir;
const float4 vecSunDir;

float ColorVariation_flt;
float4 vecSkill41;

texture entSkin1;
texture chrome_tga_bmap;

Texture ColorLUT_bmap;
sampler sLUT = sampler_state
{
	Texture = <ColorLUT_bmap>; 
	AddressU = Clamp;
	AddressV = Clamp; 
	MipFilter = Linear;
};

sampler ColorMapSampler = sampler_state 
{ 
	Texture = <entSkin1>; 
	AddressU  = Wrap; 
	AddressV  = Wrap; 
}; 

sampler ChromeMapSampler = sampler_state 
{ 
	Texture = <chrome_tga_bmap>; 
	AddressU  = Wrap; 
	AddressV  = Wrap; 
}; 

void DiffuseVS( 
in float4 InPos: POSITION, 
in float3 InNormal: NORMAL, 
in float2 InTex: TEXCOORD0, 
out float4 OutPos: POSITION, 
out float2 OutTex: TEXCOORD0, 
out float3 OutNormal: TEXCOORD1,
out float2 OutLookup: TEXCOORD2) 
{ 
	OutPos = mul(InPos, matWorldViewProj); 
	OutNormal = normalize(mul(InNormal, matWorld));
	OutTex = InTex;
	OutLookup = mul(OutNormal,matView).xy*0.5+0.5+InTex*0.1;
} 

// Pixel Shader: 
float4 DiffusePS( 
in float2 InTex: TEXCOORD0, 
in float3 InNormal: TEXCOORD1,
in float2 Lookup: TEXCOORD2): COLOR
{ 
InNormal = normalize(InNormal);

	float4 Color = tex2D(ColorMapSampler, InTex);
	float3 blendmap = Color.rgb; //tex2D(sTexture, InTex.uv).rgb;
	
	float3 a = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (48.5 + vecSkill41.x*3.0)/64.0)).rgb;
	float3 b = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (49.5 + vecSkill41.x*3.0)/64.0)).rgb;
	float3 c = tex2D(sLUT, float2(0.5 * ColorVariation_flt, (50.5 + vecSkill41.x*3.0)/64.0)).rgb;
	return float4(a * blendmap.r + b * blendmap.g + c * blendmap.b, blendmap.r+blendmap.g+blendmap.b) + vecSkill41.w;
	
	
	

	//float4 Color = tex2D(ColorMapSampler, InTex);
	float diffuse = saturate(dot(InNormal,-vecViewDir)*1.25)*0.75+0.25-0.25*Color.a; //
	float chrome = tex2D(ChromeMapSampler,Lookup.xy);
	float chromeFac = 1-Color.a;
	
	float ColorBrightness = Color.rgb*(1-Color.a*0.5)*2;
	float4 final = Color*diffuse+chrome*chrome*0.5*chromeFac;
	final.a = ColorBrightness;
	
	return final;
} 

technique DiffuseTechnique 
{ 
	pass P0 
	{ 
		ZWriteEnable = true;
		AlphaBlendEnable=False;
		VertexShader = compile vs_2_0 DiffuseVS(); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
} 
