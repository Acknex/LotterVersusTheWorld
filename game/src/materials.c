#include <mtlView.c>

static BOOL pp_isBloomEnabled = false;

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
		pp_view.bmap = PPBloomMixMaterial.skin1;
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

MATERIAL *WallLowerMaterial =
{
	effect = "WallLower.fx";
}

MATERIAL *GroundMaterial =
{
	effect = "Ground.fx";
}
