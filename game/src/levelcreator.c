
#include <DynamicModels.h>

#include "materials.h"

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

LPD3DXMESH stage_groundMesh, stage_upperWallMesh, stage_lowerWallMesh, stage_upperWallOutlineMesh;

MATERIAL * stageMtlLava = 
{
	effect = "Lava.fx";
}

void stageRenderInit()
{
	ENTITY * ent = ent_create("tile-ground.mdl", vector(0,0,0), NULL);
	stage_groundMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-wall-upper.mdl", vector(0,0,0), NULL);
	stage_upperWallMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-wall-lower.mdl", vector(0,0,0), NULL);
	stage_lowerWallMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-wall-upper-outline.mdl", vector(0,0,0), NULL);
	stage_upperWallOutlineMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
}

void stage_unload()
{
	stage_groundMesh->Release();
	stage_upperWallMesh->Release();
	stage_lowerWallMesh->Release();
	stage_upperWallOutlineMesh->Release();
}

void stage_loadGround(DynamicModel * model, STAGE * stage)
{
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
}

void stage_loadUpperWall(DynamicModel * model, STAGE * stage)
{
	int i,j;
	TILE* tile;
	VECTOR vColor;
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
				dmdl_add_mesh(model, stage_upperWallMesh, &center, vector(coords[3*k+2],0,0));
			}
		}
	}
}


void stage_loadUpperWallOutline(DynamicModel * model, STAGE * stage)
{
	int i,j;
	TILE* tile;
	VECTOR vColor;
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
				 0,  1,  90
				// -1,  0, 180,
				//  0, -1, 270
			};
			for(k = 0; k < 2; k++) {
				TILE * n = stageGetTile(stage, i + coords[3*k+0], j + coords[3*k+1]);
				if(n->value != 1) {
					continue;
				}
				dmdl_add_mesh(model, stage_upperWallOutlineMesh, &center, vector(coords[3*k+2],0,0));
			}
		}
	}
}

void stage_loadLowerWall(DynamicModel * model, STAGE * stage)
{
	int i,j;
	TILE* tile;
	VECTOR vColor;
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
				dmdl_add_mesh(model, stage_lowerWallMesh, &center, vector(coords[3*k+2],0,0));
			}
		}
	}
}

ENTITY * stage_genEntity(STAGE * stage, void * foo)
{
	DynamicModel * model = dmdl_create();
	
	DYNAMIC_QUAD quad;
	
	void load(DynamicModel * model, STAGE * stage);
	load = foo;
	load(model, stage);
	
	
	LPD3DXMESH mesh = dmdl_create_mesh(model);
	
	dmdl_delete(model);
	
	ENTITY * ent = ent_create("level_stub.mdl", vector(0,0,0), NULL);
	ent_clone(ent);
	
	set(ent, POLYGON);
	
	ent_setmesh(ent, mesh, 0, 0);
	
	c_updatehull(ent, 0);
	
	return ent;
}

void stage_load(STAGE * stage)
{
	level_load(NULL);
	
	// Initialize models
	stageRenderInit();
	
	ENTITY * ent = ent_create("lava.hmp", vector(100 * stage->size[0], 100 * stage->size[1], -250), NULL);
	ent->material = stageMtlLava;
	
	DMDLSettings.flags |= DMDL_FIXNORMALS;
	
	ENTITY * entGround = stage_genEntity(stage, stage_loadGround);
	ENTITY * entUpperWall = stage_genEntity(stage, stage_loadUpperWall);
	ENTITY * entLowerWall = stage_genEntity(stage, stage_loadLowerWall);
	ENTITY * entOutlines = stage_genEntity(stage, stage_loadUpperWallOutline);
	
	entGround->material    = GroundMaterial;
	entUpperWall->material = WallMainMaterial;
	entLowerWall->material = WallLowerMaterial;
	entOutlines->material = WallOutlineMaterial;
}