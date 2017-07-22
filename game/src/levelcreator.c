
#include <DynamicModels.h>

/*
typedef struct {
	float x,y,z;	// position in DirectX coordinates
	float nx,ny,nz;	// normal
	float u1,v1;    // first coordinate set, for textures
	float u2,v2;    // second coordinate set, for shadow maps
	float x3,y3,z3,w3; // third coordinate set, for tangent vector and handedness
	float tu4,tv4;  // 4th coordinate set, for additional data
} D3DVERTEX;
*/

LPD3DXMESH stage_wallMesh, stage_groundMesh, stage_pillarMesh;

MATERIAL * stageMtlLava = 
{
	effect = "Lava.fx";
}

void stageRenderInit()
{
	ENTITY * ent = ent_create("tile-ground.mdl", vector(0,0,0), NULL);
	stage_groundMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-wall.mdl", vector(0,0,0), NULL);
	stage_wallMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
}

void stage_load(STAGE * stage)
{
	level_load(NULL);
	
	// Initialize models
	stageRenderInit();
	
	ENTITY * ent = ent_create("lava.hmp", vector(100 * stage->size[0], 100 * stage->size[1], -250), NULL);
	ent->material = stageMtlLava;
	
	DMDLSettings.flags |= DMDL_FIXNORMALS;
	DynamicModel * model = dmdl_create();
	
	DYNAMIC_QUAD quad;
	
	int i,j;
	TILE* tile;
	VECTOR vColor;
	for(i = 0; i < stage->size[0]; i++)
	{
		for(j = 0; j < stage->size[1]; j++)
		{
			tile = stageGetTile(stage, i, j);
			if(tile->value == 1) {
				continue;
			}
			
			VECTOR center;
			center.x = i * 200;
			center.y = j * 200;
			center.z = 0;
			
			dmdl_add_mesh(model, stage_groundMesh, &center, vector(0,0,0));
		}
	}
	
	for(i = 1; i < (stage->size[0] - 1); i++) {
		for(j = 1; j < (stage->size[1] - 1); j++) {
			tile = stageGetTile(stage, i, j);
			if(tile->value == 1) {
				continue;
			}
			
			VECTOR center;
			center.x = i * 200;
			center.y = j * 200;
			center.z = 0;
			
			int k;
			int coords[] = {
				 1,  0,   0,
				 0,  1,  90,
				-1,  0, 180,
				 0, -1, 270
			};
			for(k = 0; k < 4; k++) {
				TILE * n = stageGetTile(stage, i + coords[3*k+0], j + coords[3*k+1]);
				if(n->value != 1) {
					continue;
				}
			
				dmdl_add_mesh(model, stage_wallMesh, &center, vector(coords[3*k+2],0,0));
				
			}
		}	
	}
	
	LPD3DXMESH mesh = dmdl_create_mesh(model);
	
	dmdl_delete(model);
	
	ENTITY * ent = ent_create("level_stub.mdl", vector(0,0,0), NULL);
	
	set(ent, POLYGON);
	
	ent_setmesh(ent, mesh, 0, 0);
	
	c_updatehull(ent, 0);
}