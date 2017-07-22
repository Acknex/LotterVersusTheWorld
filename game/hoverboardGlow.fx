const float4x4 matWorldViewProj;
const float4x4 matView;
const float4x4 matWorld;
const float4 vecViewDir;
float4 vecTime;

texture chrome_tga_bmap;

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
in uniform float scale,
out float4 OutPos: POSITION, 
out float2 OutTex: TEXCOORD0, 
out float3 OutNormal: TEXCOORD1,
out float2 OutLookup: TEXCOORD2) 
{ 
	InPos.xyz *= 2+InNormal.xyz*scale*0.25;
	InPos.y *= 2+0.125*sin(vecTime.w*4)+scale;
	//InPos.xyz += InNormal.xyz*scale*4;
	OutPos = mul(InPos, matWorldViewProj); 
	OutNormal = normalize(mul(InNormal, matWorld));
	OutTex = InTex;
	OutLookup = mul(OutNormal,matView).xy*0.5+0.5+InTex*0.1;
} 

// Pixel Shader: 
float4 DiffusePS( 
in float2 InTex: TEXCOORD0, 
in float3 InNormal: TEXCOORD1,
in float2 Lookup: TEXCOORD2,
in uniform float alpha): COLOR 
{ 
	InNormal = normalize(InNormal);

	float diffuse = saturate(dot(InNormal,-vecViewDir)); //
	float4 Color = float4(1,0.1,0.9,1);
	float chrome = tex2D(ChromeMapSampler,Lookup.xy);
	
	float4 final = Color+diffuse*diffuse+chrome*chrome*0.5;
	final.a = alpha*diffuse*2;
	
	return final;
} 

technique DiffuseTechnique 
{ 
	pass P0 
	{ 
		//cullmode = none;
		ZWriteEnable = true;
		AlphaBlendEnable=False;
		ZWriteEnable = False;
		AlphaBlendEnable=true;
		destBlend = one;
		VertexShader = compile vs_2_0 DiffuseVS(0); 
		PixelShader  = compile ps_2_0 DiffusePS(1); 
	} 
	pass P1
	{ 
		ZWriteEnable = False;
		AlphaBlendEnable=true;
		destBlend = one;
		VertexShader = compile vs_2_0 DiffuseVS(1); 
		PixelShader  = compile ps_2_0 DiffusePS(0.5); 
	} 
	pass P2
	{ 
		ZWriteEnable = False;
		AlphaBlendEnable=true;
		destBlend = one;
		VertexShader = compile vs_2_0 DiffuseVS(2); 
		PixelShader  = compile ps_2_0 DiffusePS(0.25); 
	} 
	pass P3
	{ 
		ZWriteEnable = False;
		AlphaBlendEnable=true;
		destBlend = one;
		VertexShader = compile vs_2_0 DiffuseVS(3); 
		PixelShader  = compile ps_2_0 DiffusePS(0.125); 
	} 
} 
