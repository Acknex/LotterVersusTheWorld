#include <mtlView.c>

static BOOL pp_isBloomEnabled = false;
VIEW *BloomImageView = NULL;

MATERIAL *PPThresholdLuminanceMaterial =
{
	effect = "ThresholdLum.fx";
	flags = AUTORELOAD;
}

MATERIAL *PPBloomMixMaterial =
{
	effect = "BloomMix.fx";
	flags = AUTORELOAD;
}

MATERIAL *PPBlurHMaterial =
{
	effect = "Blur.fx";
	flags = AUTORELOAD;
}

MATERIAL *PPBlurVMaterial =
{
	effect = "Blur.fx";
	flags = AUTORELOAD;
}

void pp_bloom_resize()
{
	if(pp_isBloomEnabled)
	{
		if(PPBloomMixMaterial.skin1)
			bmap_remove(PPBloomMixMaterial.skin1);
			
		PPBloomMixMaterial.skin1 = bmap_createblack(screen_size.x, screen_size.y, 8888);
		BloomImageView.bmap = PPBloomMixMaterial.skin1;
	}
}

void pp_bloom(float strength)
{
	PPBloomMixMaterial.skill1 = floatv(strength);
	
	if(!pp_isBloomEnabled)
	{
		pp_isBloomEnabled = true;
		
		PPBlurHMaterial.skill1 = floatv(1.0);
		PPBlurHMaterial.skill2 = floatv(0.0);
		
		PPBlurVMaterial.skill1 = floatv(0.0);
		PPBlurVMaterial.skill2 = floatv(1.0);
		
		pp_view = cam;
		pp_stage = cam;
		
		BloomImageView = cam;
		pp_bloom_resize();
		
		pp_add(PPThresholdLuminanceMaterial);
		pp_add(PPBlurHMaterial);
		pp_add(PPBlurVMaterial);
		pp_add(PPBloomMixMaterial);
	}
}



BMAP * WallMainAtlas = "tile-wall-atlas_01.dds";
BMAP * WallMainTextImage = "tile-wall-text.png";
FONT * WallMainTextFont = "Courier#15b";
BMAP *ColorLUT = "color-lut_01.dds";
float ColorVariation = 0.0;

TEXT * WallMainText = 
{
	pos_x = 0;
	pos_y = 0;
	blue = 0;
	red = 0;
	green = 255;
	// font = WallMainTextFont;
	strings = 20;
}

float bloomFactor = 2.5;

function ColorLUT_Bounce()
{
	while(key_p)
	{
		ColorVariation = 0.5 + 0.5 * sinv(4 * total_ticks);
		wait(1);
	}
	
	while(key_o)
	{
		bloomFactor = 2.0 + 2.0 * sinv(4 * total_ticks);
		pp_bloom(bloomFactor);
		
		wait(1);
	}
}

function WallMainText_startup()
{
	// WallMainText.size_x = bmap_width(WallMainTextImage);
	WallMainText.size_y = bmap_height(WallMainTextImage);
	str_cpy(delimit_str, "");
	txt_load(WallMainText, "wallpaper.txt");
	
	wait(1);
	bmap_to_mipmap(WallMainTextImage);
	
	on_p = ColorLUT_Bounce;
	on_o = ColorLUT_Bounce;
	
	while(1)
	{
		DEBUG_VAR(bloomFactor, 100);
		
		int idx = random(WallMainText.strings);
		
		STRING * str = (WallMainText.pstring)[idx];
		
		int idx = random(str_len(str));
		
		str_setchr(str, 1 + idx, 0x20 + integer(random(128-0x20)));
		
//		bmap_rendertarget(WallMainTextImage, 0, 0);
//
//		draw_quad(
//			NULL,
//			NULL, NULL,
//			vector(bmap_width(WallMainTextImage), bmap_height(WallMainTextImage), 0), NULL,
//			COLOR_RED, 100, 0);
//		draw_obj(WallMainText);
//
//		bmap_rendertarget(NULL, 0, 0);
//		
//		bmap_to_mipmap(WallMainTextImage);
		
		
		//draw_quad(
		//	WallMainText.target_map,
		//	NULL, NULL,
		//	NULL, NULL,
		//	NULL, 100,
		//	0);
		wait(3);
	}
}

MATERIAL *WallMainMaterial =
{
	effect = "WallMain.fx";
	skin1 = WallMainAtlas;
	skin2 = WallMainTextImage;
}

MATERIAL *WallOutlineMaterial =
{
	effect = "WallOutline.fx";
}

BMAP *WallLower01BMAP = "graphics/tile-wall-lower_01.dds";

MATERIAL *WallLowerMaterial =
{
	effect = "WallLower.fx";
	skin1 = WallLower01BMAP;
}

BMAP * GroundAtlas = "tile-floor-atlas_01.dds";

MATERIAL *GroundMaterial =
{
	effect = "Ground.fx";
	skin2 = GroundAtlas;
}

VIEW *ReflectionView = NULL;

void ground_reflections()
{
	if(ReflectionView)
		return;
	
	ReflectionView = view_create(-1);
	ReflectionView.size_x = 512;
	ReflectionView.size_y = 512;
	set(ReflectionView, NOFLAG1);
	
/*	pp_view = cam;
	pp_stage = cam;
		
	pp_add(PPThresholdLuminanceMaterial);*/
	
	ReflectionView.bmap = bmap_createblack(512, 512, 8888);
	
	GroundMaterial.skin1 = ReflectionView.bmap;
	
	set(ReflectionView, SHOW);
	
	while(1)
	{
		ReflectionView.aspect = (screen_size.x/screen_size.y)*camera.aspect; // screen aspect, independent of render target
		ReflectionView.arc    = camera.arc;
		ReflectionView.fog_start = camera.fog_start;
		ReflectionView.fog_end   = camera.fog_end;
		ReflectionView.clip_far  = camera.clip_far;
		ReflectionView.clip_near = camera.clip_near;
		
		vec_set(ReflectionView.x, cam.x);
		ReflectionView.z *= -1.0;
		
		vec_set(ReflectionView.pan, cam.pan);
		ReflectionView.tilt *= -1.0;
		ReflectionView.roll *= -1.0;
		
		wait(1);
	}
}
