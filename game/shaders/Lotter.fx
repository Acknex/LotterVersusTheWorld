const float4x4 matWorldViewProj;
const float4x4 matView;
const float4x4 matWorld;
const float4 vecViewDir;
const float4 vecSunDir;

texture entSkin1;
texture chrome_tga_bmap;

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
	float diffuse = saturate(dot(InNormal,-vecViewDir)*1.25)*0.75+0.25-0.25*Color.a; //
	float chrome = tex2D(ChromeMapSampler,Lookup.xy);
	float chromeFac = 1-Color.a;
	
	float4 final = Color*diffuse+chrome*chrome*0.5*chromeFac;
	
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
