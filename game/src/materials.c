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

void pp_bloom(float threshold, float strength)
{
	PPThresholdLuminanceMaterial.skill1 = floatv(threshold);
	PPBloomMixMaterial.skill1 = floatv(strength);
	
	if(!pp_isBloomEnabled)
	{
		pp_isBloomEnabled = true;
		
		PPBlurHMaterial.skill1 = floatv(2.0);
		PPBlurHMaterial.skill2 = floatv(0.0);
		
		PPBlurVMaterial.skill1 = floatv(0.0);
		PPBlurVMaterial.skill2 = floatv(2.0);
		
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


MATERIAL *WallMainMaterial =
{
	effect = "WallMain.fx";
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

MATERIAL *GroundMaterial =
{
	effect = "Ground.fx";
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
	
	pp_view = cam;
	pp_stage = cam;
		
	pp_add(PPThresholdLuminanceMaterial);
	
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
