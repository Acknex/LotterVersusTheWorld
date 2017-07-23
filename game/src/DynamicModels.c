#include <acknex.h>
#include <d3d9.h>

#define PRAGMA_BIND "test.bmp"

/************************************************************************************************/
/*	DX9 API Includes                                                                             */
/************************************************************************************************/
typedef struct _D3DXWELDEPSILONS {
  FLOAT Position;
  FLOAT BlendWeights;
  FLOAT Normal;
  FLOAT PSize;
  FLOAT Specular;
  FLOAT Diffuse;
  FLOAT Texcoord[8];
  FLOAT Tangent;
  FLOAT Binormal;
  FLOAT TessFactor;
} D3DXWELDEPSILONS;

#define D3DXWELDEPSILONS_WELDALL             1
#define D3DXWELDEPSILONS_WELDPARTIALMATCHES  2
#define D3DXWELDEPSILONS_DONOTREMOVEVERTICES 4
#define D3DXWELDEPSILONS_DONOTSPLIT          8 

HRESULT WeldVertices(
	LPD3DXMESH pMesh,
	DWORD Flags,
	D3DXWELDEPSILONS *pEpsilons,
	DWORD *pAdjacencyIn,
	DWORD *pAdjacencyOut,
	DWORD *pFaceRemap,
	LPD3DXBUFFER *ppVertexRemap);

typedef void D3DXEFFECTINSTANCE;

HRESULT SaveMeshToX(
	char *pFilename,
	LPD3DXMESH pMesh,
	DWORD *pAdjacency,
	D3DXMATERIAL *pMaterials,
	D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	DWORD Format);

DynamicModel* dmdl_create()
{
	DynamicModel *model = sys_malloc(sizeof(DynamicModel));
	
	model->vertexCount = 0;
	model->faceCount = 0;
	model->skin[0] = NULL;
	model->skin[1] = NULL;
	model->skin[2] = NULL;
	model->skin[3] = NULL;
	
	return model;
}

void dmdl_delete(DynamicModel *model)
{
	sys_free(model);
}

LPD3DXMESH dmdl_create_mesh(DynamicModel *model)
{
	if(model == NULL) return 0;

	LPD3DXMESH mesh;
	
	if(model->vertexCount <= 0)
	{
		return NULL;
	}
	if(model->faceCount <= 0)
	{
		return NULL;
	}

	D3DVERTEX *vertexBuffer;
	short *indexBuffer;
	DWORD *attributeBufferResult;
	
	D3DXCreateMesh(model->faceCount,model->vertexCount,D3DXMESH_MANAGED,pvertexdecl,pd3ddev,&mesh);
	mesh->LockVertexBuffer(0, &vertexBuffer);
	mesh->LockIndexBuffer(0, &indexBuffer);
	mesh->LockAttributeBuffer(0, &attributeBufferResult);
	
	memcpy(vertexBuffer,model->vertexBuffer,model->vertexCount * sizeof(D3DVERTEX));
	memcpy(indexBuffer,model->indexBuffer,3 * model->faceCount * sizeof(short));
	//memcpy(attributeBufferResult,model->attributeBuffer,model->vertexCount * sizeof(DWORD));
	
	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
	mesh->UnlockAttributeBuffer();
	
	if((DMDLSettings.flags & DMDL_OPTIMIZE) != 0)
	{
		D3DXWELDEPSILONS Epsilons;
		// Set epsilon values
		Epsilons.Normal = 0.001;
		Epsilons.Position = 0.1; 
		WeldVertices(	mesh,
						D3DXWELDEPSILONS_WELDPARTIALMATCHES,
						&Epsilons,
						NULL,
						NULL,
						NULL,
						NULL);
	}
	
	if((DMDLSettings.flags & DMDL_FIXNORMALS) != 0)
	{
		D3DXComputeNormals((LPD3DXBASEMESH)mesh, NULL);
	}
	
	return mesh;
}

ENTITY* dmdl_create_instance(DynamicModel *model, VECTOR *pos, EVENT act)
{
	if(model == NULL) return NULL;
	
	ENTITY* ent = ent_create(CUBE_MDL,pos,NULL);
	ent_clone(ent);
	
	LPD3DXMESH mesh = dmdl_create_mesh(model);
	
	if(mesh == NULL) return NULL;
	
	ent_setmesh(ent,mesh,0,0);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		BMAP* skinSource = model->skin[i];
		if(skinSource)
		{
			if(DMDLSettings.flags & DMDL_CLONE_TEX)
			{
				var format = bmap_lock(skinSource, 0); bmap_unlock(skinSource);
				BMAP* skinTarget = bmap_createblack(bmap_width(skinSource),bmap_height(skinSource),format);
				bmap_blit(skinTarget,skinSource,NULL,NULL);
				ent_setskin(ent,skinTarget,i + 1);
			}
			else
			{
				ent_setskin(ent,skinSource,i + 1);
			}
		}
	}
	
	if(act != NULL)
	{
		me = ent;
		action dmdl_temporary_action();
		dmdl_temporary_action = act;
		dmdl_temporary_action();
		me = NULL;
	}
	return ent;
}

void dmdl_add_face(DynamicModel *model, DYNAMIC_FACE *face)
{
	if(model == NULL) return;
	if(face == NULL) return;
	
	int iVertexStartOffset = model->vertexCount;
	int iFaceStartOffset = model->faceCount;
	int i;
	// Copy vertices
	for(i = 0; i < 3; i++)
	{
		D3DVERTEX* vSource = &(face->v[i]);
		D3DVERTEX* vTarget = &(model->vertexBuffer[iVertexStartOffset + i]);
		memcpy(vTarget,vSource,sizeof(D3DVERTEX));
		model->indexBuffer[3 * iFaceStartOffset + i] = iVertexStartOffset + i;
	}
	
	model->vertexCount += 3;
	model->faceCount += 1;
}

void dmdl_add_quad(DynamicModel *model, DYNAMIC_QUAD *quad)
{
	if(model == NULL) return;
	if(quad == NULL) return;
	
	short vertices[4];
	int iVertexStartOffset = model->vertexCount;
	int iFaceStartOffset = model->faceCount;
	int i;
	
	// Copy vertices
	for(i = 0; i < 4; i++)
	{
		D3DVERTEX* vSource = &(quad->v[i]);
		D3DVERTEX* vTarget = &(model->vertexBuffer[iVertexStartOffset + i]);
		memcpy(vTarget,vSource,sizeof(D3DVERTEX));
		
		vertices[i] = iVertexStartOffset + i;
	}
	
	// Face 1
	model->indexBuffer[3 * iFaceStartOffset + 0] = vertices[2];
	model->indexBuffer[3 * iFaceStartOffset + 1] = vertices[1];
	model->indexBuffer[3 * iFaceStartOffset + 2] = vertices[0];
	
	// Face 2
	model->indexBuffer[3 * iFaceStartOffset + 3] = vertices[1];
	model->indexBuffer[3 * iFaceStartOffset + 4] = vertices[2];
	model->indexBuffer[3 * iFaceStartOffset + 5] = vertices[3];
	
	// Index count per dynamic face
	model->vertexCount += 4;
	model->faceCount += 2;
}

void dmdl_add_entity(DynamicModel *model, ENTITY *ent)
{
	dmdl_add_mesh(model, ent_getmesh(ent, 0 ,0), &(ent->x), &(ent->pan), &(ent->scale_x));
}

void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh)
{
	dmdl_add_mesh(model, mesh, nullvector, nullvector, vector(1,1,1));
}

void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset)
{
	dmdl_add_mesh(model, mesh, offset, nullvector, vector(1,1,1));
}

void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation)
{
	dmdl_add_mesh(model, mesh, offset, rotation, vector(1,1,1));
}

void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation, VECTOR *scale)
{
	if(model == NULL) return;
	if(mesh == NULL) return;
	
	D3DVERTEX *vertexBufferMesh;
	short *indexBufferMesh;
	DWORD *attributeBuffer;
	
	int meshVertexCount = mesh->GetNumVertices();
	int meshFaceCount = mesh->GetNumFaces();
	
	if(model->vertexCount + meshVertexCount > DMDL_MAX_VERTEXCOUNT)
		return;
	
	if(model->faceCount + meshFaceCount > (DMDL_MAX_INDEXCOUNT / 3))
		return;
		
	mesh->LockVertexBuffer(0, &vertexBufferMesh);
	mesh->LockIndexBuffer(0, &indexBufferMesh);
	mesh->LockAttributeBuffer(0, &attributeBuffer);
	
	int i;
	for(i = 0; i < meshVertexCount; i++)
	{
		memcpy(&(model->vertexBuffer[model->vertexCount + i]),vertexBufferMesh,sizeof(D3DVERTEX));
		
		VECTOR pos;
		pos.x = model->vertexBuffer[model->vertexCount + i].x;
		pos.z = model->vertexBuffer[model->vertexCount + i].y;
		pos.y = model->vertexBuffer[model->vertexCount + i].z;
		
		vec_mul(&pos, scale);
		vec_rotate(&pos, rotation);
		vec_add(&pos, offset);
		
		model->vertexBuffer[model->vertexCount + i].x = pos.x;
		model->vertexBuffer[model->vertexCount + i].z = pos.y;
		model->vertexBuffer[model->vertexCount + i].y = pos.z;
		
		normal.x = model->vertexBuffer[model->vertexCount + i].nx;
		normal.z = model->vertexBuffer[model->vertexCount + i].ny;
		normal.y = model->vertexBuffer[model->vertexCount + i].nz;
		
		vec_rotate(&normal, rotation);
		
		model->vertexBuffer[model->vertexCount + i].nx = normal.x;
		model->vertexBuffer[model->vertexCount + i].nz = normal.y;
		model->vertexBuffer[model->vertexCount + i].ny = normal.z;
		
		vertexBufferMesh++;
	}
	
	
	for(i = 0; i < meshFaceCount; i++)
	{
		model->attributeBuffer[model->faceCount + i] = *attributeBuffer;
		attributeBuffer++;
	}
	
	for(i = 0; i < meshFaceCount * 3; i++)
	{
		model->indexBuffer[model->faceCount * 3 + i] = model->vertexCount + *indexBufferMesh;
		indexBufferMesh++;
	}
	
	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
	mesh->UnlockAttributeBuffer();
	
	model->vertexCount += meshVertexCount;
	model->faceCount += meshFaceCount;
}

void dmdl_save(DynamicModel *model, char *filename)
{
	LPD3DXMESH mesh = dmdl_create_mesh(model);
	
	D3DMATERIAL9 mat9_1;
	/*mat9_1.Diffuse.r = 255;
	mat9_1.Ambient.r = 255;
	mat9_1.Specular.r = 255;
	mat9_1.Emissive.r = 255;
	mat9_1.Power = 1.0;*/
	
	
	D3DXMATERIAL mat1;
	memcpy(&mat1.MatD3D, &mat9_1, sizeof(D3DMATERIAL9));
	
	char buffer[256];
	strcpy(buffer, "test.bmp");
	mat1.pTextureFilename = buffer;
	
	SaveMeshToX(
		filename,
		mesh,
		NULL,
		&mat1,
		NULL,
		1,
		DMDLSettings.xFormat);
	
	/*SaveMeshToX(
		filename,
		mesh,
		NULL,
		NULL,
		NULL,
		0,
		DMDLSettings.xFormat);*/
}

int dmdl_add_vertex(DynamicModel *_model, D3DVERTEX *_v)
{
	int index = _model->vertexCount;
	memcpy(&_model->vertexBuffer[index], _v, sizeof(D3DVERTEX));
	_model->vertexCount +=1;
	return index;
}

void dmdl_connect_vertices(DynamicModel *_model, int _v1, int _v2, int _v3)
{
	int bufferIndex = 3 * _model->faceCount;
	_model->indexBuffer[bufferIndex + 0] = _v1;
	_model->indexBuffer[bufferIndex + 1] = _v2;
	_model->indexBuffer[bufferIndex + 2] = _v3;
	_model->faceCount +=1;
}













