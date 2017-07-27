
#include "DynamicModels.h"

#include "materials.h"
#include "turret.h"
#include "spikes.h"
#include "hole.h"
#include "sphere_of_death.h"
#include "bat.h"
#include "sputnik.h"
#include "teleporter.h"
#include "entity_defs.h"
#include "marker.h"
#include "level_furniture.h"

#define PRAGMA_BIND "tile-wall-upper-01.mdl"
#define PRAGMA_BIND "tile-wall-upper-02.mdl"
#define PRAGMA_BIND "tile-wall-upper-03.mdl"

LPD3DXMESH stage_groundMesh, stage_upperWallMesh[3], stage_lowerWallMesh, stage_upperWallOutlineMesh, stage_outlinePostMesh;

BMAP * stage_bmpBlackSkin = "#32x32x24";

MATERIAL * stageMtlLava = 
{
	effect = "Lava.fx";
}

BOOL stageTileHasNoGround(TILE * tile)
{
	if(!tile) return FALSE;
	return (tile->flags & TILE_FLAG_TRAP_SPIKES)
		|| (tile->flags & TILE_FLAG_TRAP_HOLE)
		|| (tile->flags & TILE_FLAG_TRAP_TURRET);
}

void stageRenderInit()
{
	int i;
	
	ENTITY * ent = ent_create("tile-ground.mdl", vector(0,0,0), NULL);
	stage_groundMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	for(i = 0; i < 3; i++) {
		ent = ent_create(str_printf(NULL, "tile-wall-upper-%02d.mdl", (i+1)), vector(0,0,0), NULL);
		stage_upperWallMesh[i] = ent_getmesh(ent, 0, 0);
		ent_remove(ent);
	}
	
	ent = ent_create("tile-wall-lower.mdl", vector(0,0,0), NULL);
	stage_lowerWallMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-wall-upper-outline.mdl", vector(0,0,0), NULL);
	stage_upperWallOutlineMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
	
	ent = ent_create("tile-outline-post.mdl", vector(0,0,0), NULL);
	stage_outlinePostMesh = ent_getmesh(ent, 0, 0);
	ent_remove(ent);
}

void stage_unload()
{
	int i;
	// stage_groundMesh->Release();
	for(i = 0; i < 3; i++) {
		// (stage_upperWallMesh[0])->Release();
	}
	// stage_lowerWallMesh->Release();
	// stage_upperWallOutlineMesh->Release();
	// stage_outlinePostMesh->Release();
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
			if(tile->value == TILE_EMPTY) {
				continue;
			}
			
			if(!stageTileHasNoGround(tile))
			{
				VECTOR center;
				center.x = i * 200;
				center.y = j * 200;
				center.z = 0;
				
				dmdl_add_mesh(model, stage_groundMesh, &center, vector(0,0,0));
			}
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
			if(tile->value == TILE_EMPTY) {
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
				if(n->value != TILE_EMPTY) {
					continue;
				}
				dmdl_add_mesh(model, stage_upperWallMesh[random(3)], &center, vector(coords[3*k+2],0,0));
			}
		}
	}
}


void stage_loadWallOutline(DynamicModel * model, STAGE * stage)
{
	int i,j,k;
	TILE* tile;
	VECTOR vColor;
	for(i = 1; i < (stage->size[0] - 1); i++) {
		for(j = 1; j < (stage->size[1] - 1); j++) {
			tile = stageGetTile(stage, i, j);
			
			VECTOR center;
			center.x = i * 200;
			center.y = j * 200;
			center.z = 0;
			if(tile->value != TILE_EMPTY) {
				
				int coords[] = {
					 1,  0,   0,
					 0,  1,  90
					// -1,  0, 180,
					//  0, -1, 270
				};
				for(k = 0; k < 2; k++) {
					TILE * n = stageGetTile(stage, i + coords[3*k+0], j + coords[3*k+1]);
					if(n->value != TILE_EMPTY) {
						continue;
					}
					dmdl_add_mesh(model, stage_upperWallOutlineMesh, &center, vector(coords[3*k+2],0,0));
				}
			}
			int outlines[] = {
				 1,  0, // p0
				 1, -1, // p1
				 0, -1, // p2
				100, -100,
				
				-1,  0,
				-1,  1,
				 0,  1,
				-100, 100
			};
			
			for(k = 0; k < 16; k += 8) {
				TILE * a = stageGetTile(stage, i+outlines[k+0], j+outlines[k+1]);
				TILE * b = stageGetTile(stage, i+outlines[k+2], j+outlines[k+3]);
				TILE * c = stageGetTile(stage, i+outlines[k+4], j+outlines[k+5]);
				
				BOOL valid = FALSE;
				if(!!tile->value != !!a->value && !!a->value == !!b->value && !!b->value == !!c->value) {
					valid = TRUE;
				}
				
				if(valid) {
					VECTOR fo;
					vec_set(&fo, &center);
					fo.x += outlines[k+6];
					fo.y += outlines[k+7];
					dmdl_add_mesh(model, stage_outlinePostMesh, &fo, vector(0,0,0));
				}
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
				if(tile->value != TILE_EMPTY && n->value != TILE_EMPTY && !(n->flags & TILE_FLAG_TRAP_HOLE)) {
					continue;
				}
				if(tile->value == TILE_EMPTY && (n->value == TILE_EMPTY || !(n->flags & TILE_FLAG_TRAP_HOLE))) {
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
	ent_setskin(ent, stage_bmpBlackSkin, 1);
	
	c_updatehull(ent, 0);
	
	return ent;
}

VECTOR * stage_load(STAGE * stage)
{
	level_load(NULL);
	
	// Initialize models
	stageRenderInit();
	
	
	ENTITY * entLava = ent_create("lava.hmp", vector(100 * stage->size[0], 100 * stage->size[1], -350), NULL);
	entLava->material = stageMtlLava;
	
	DMDLSettings.flags |= DMDL_FIXNORMALS;
	
	ENTITY * entUpperWall = stage_genEntity(stage, stage_loadUpperWall);
	ENTITY * entGround = stage_genEntity(stage, stage_loadGround);
	ENTITY * entLowerWall = stage_genEntity(stage, stage_loadLowerWall);
	ENTITY * entOutlines = stage_genEntity(stage, stage_loadWallOutline);
	
	set(entUpperWall	, FLAG2);
	set(entOutlines	, FLAG2);
	
	entGround->group = 30;
	entLowerWall->group = 30;
	entOutlines->group = 30;
	
	entGround->material    = GroundMaterial;
	entUpperWall->material = WallMainMaterial;
	entLowerWall->material = WallLowerMaterial;
	entOutlines->material = WallOutlineMaterial;
	
	entUpperWall->type = TypeWall;
	set(entGround, FLAG1);
	set(entLowerWall, FLAG1);
	set(entLava, FLAG1);
	
	entOutlines->flags |= PASSABLE;
	
	{
		int i, j;
		for(i = 1; i < (stage->size[0] - 1); i++) {
			for(j = 1; j < (stage->size[1] - 1); j++) {
				ENTITY * ent;
				TILE * tile = stageGetTile(stage, i, j);
				
				VECTOR center;
				center.x = i * 200;
				center.y = j * 200;
				center.z = 0;
				
				if(tile->flags & TILE_FLAG_TRAP_TURRET) {
					var rand = integer(random(4));
					switch (rand)
					{
						case 0:
							ent = ent_create("tile-floor-turret.mdl", &center, enemy_turret_rotcw);
							break;

						case 1:
							ent = ent_create("tile-floor-turret.mdl", &center, enemy_turret_rotccw);
							break;

						case 2:
							ent = ent_create("tile-floor-turret.mdl", &center, enemy_turret_alternate);
							break;

						case 3:
							ent = ent_create("tile-floor-turret.mdl", &center, enemy_turret_aim);
							break;

					}
					
					//ent = ent_create("tile-floor-turret.mdl", &center, enemy_turret);
					ent->material = TurretMaterial;
				//moved to turret
				/*	set(ent, POLYGON);
					set(ent, FLAG1);
					ent_animate(ent, "closed", 0, 0);
					*/
				} else if(tile->flags & TILE_FLAG_TRAP_HOLE) {
					ent = ent_create("tile-floor-hole.mdl", &center, enemy_hole);
					ent->material = TurretMaterial;
				}  else if(tile->flags & TILE_FLAG_TRAP_SPIKES) {
					ent = ent_create("tile-floor-spikes.mdl", &center, enemy_spikes);
					ent->material = TurretMaterial;
				} else if(tile->flags & TILE_FLAG_TRAP_BAT) {
					ent = ent_create("bat.mdl", vec_add(vector(0, 0, 32), &center), enemy_bat); // implement virus !!!
				} else if(tile->flags & TILE_FLAG_TRAP_SPHERE) {
					ent = ent_create("sphere_of_death.mdl", vec_add(vector(0, 0, 32), &center), enemy_sphere);
				}
				else if(tile->flags & TILE_FLAG_TRAP_SPUTNIK) {
					ent = ent_create("Sputnik.mdl", vec_add(vector(0, 0, 80), &center), enemy_sputnik);
					ent->type = TypeEnemy;
				} else if(tile->value != 0) {
					var r = random(100);
					var rot = 0;
					var offx = 0;
					var offy = 0;

					TILE* neighborw = stageGetTile(stage, i-1, j);
					TILE* neighbore = stageGetTile(stage, i+1, j);
					TILE* neighborn = stageGetTile(stage, i, j-1);
					TILE* neighbors = stageGetTile(stage, i, j+1);
					if (neighbors->value == 0)
					{
						rot = -90;
						offy = 95;
					}
					else if (neighborn->value == 0)
					{
						rot = 90;
						offy = -95;
					}
					else if (neighborw->value == 0)
					{
						rot = 0;
						offx = -95;
					}
					else if (neighbore->value == 0)
					{
						rot = 180;
						offx = 95;
					}
					else
					{
						r = 200;
					}

					if (r < 7)
					{
						ENTITY* desk = ent_create("desk.mdl", vec_add(vector(offx, offy, 0), &center), desk_buildup);
						desk->skill1 = rot;
					}
					else if (r < 15)
					{
						ENTITY* rack = ent_create("rack_case.mdl", vec_add(vector(offx, offy, 0), &center), rack_buildup);
						rack->skill1 = rot;
					}
					else if ((r > 96) && (r < 100))
					{
						ENTITY* screen = ent_create("screen.mdl", vec_add(vector(offx, offy, 0), &center), screenOnWall);
						screen->pan = rot;
					}
				}
			}
		}
	}
	
	ENTITY *ent = ent_create("teleporter-effect.mdl", stageGetExitPos(stage, NULL, NULL, NULL), teleporter_out);
	ent->material = TeleporterEffectMaterial;
	
	return stageGetEntrancePos(stage, NULL, NULL, NULL);
}
