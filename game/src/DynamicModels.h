#ifndef _DYNAMICMODELS_H_
#define _DYNAMICMODELS_H_

#include <acknex.h>
#include <d3d9.h>
 
 /**
 * \defgroup  DynamicModelsModule Dynamic Models
 * \ingroup   GenericScriptLibrary
 * \brief     Build and combine meshes at runtime.
 *
 * \{
 *
 * \file DynamicModels.h
 * \brief Header of the module to create models at runtime.
 *
 * \file DynamicModels.c
 * \brief Code of the module to create models at runtime.
 *
 * \file  test_dmdl.c
 * \brief Test code of the module to create models at runtime.
 */

/**
 * \struct DYNAMIC_FACE
 * \brief A single face definition of a dynamic model.
 */
typedef struct
{
	/**
	 * \brief Array of three vertices. The vertices are ordered CCW.
	 */
	D3DVERTEX v[3];
} DYNAMIC_FACE;


/**
 * \struct DYNAMIC_QUAD
 * \brief A two-faced quad definition of a dynamic model.
 */
typedef struct
{
	/**
	 * \brief Array of four vertices. The vertices are ordered CCW.
	 * \image html dynamic_quad_order.png
	 */
	D3DVERTEX v[4];
} DYNAMIC_QUAD;

/**
 * \brief Maximum vertex count of a dynamic model.
 */
#define DMDL_MAX_VERTEXCOUNT 65536

/**
 * \brief Maximum index count of a dynamic model.
 */
#define DMDL_MAX_INDEXCOUNT 128000

/** 
 * \struct DynamicModel
 * \brief A dynamic model. It contains all data to build a mesh from it.
 */
typedef struct
{
	/**
	 * \brief Number of vertices stored in the model.
	 */
	int vertexCount;
	
	/**
	 * \brief Number of faces stored in the model.
	 */
	int faceCount;
	
	/**
	 * \brief The vertex buffer of the dynamic model. Contains actual vertex data.
	 */
	D3DVERTEX vertexBuffer[DMDL_MAX_VERTEXCOUNT];
	
	DWORD attributeBuffer[DMDL_MAX_VERTEXCOUNT];
	
	/**
	 * \brief The index buffer of the dynamic model. Contains all faces of the model.
	 */
	short indexBuffer[DMDL_MAX_INDEXCOUNT];
	
	/**
	 * \brief Array with the four first skins of a newly created dynamic entity.
	 */
	BMAP* skin[4];
} DynamicModel;

/** 
 * \brief Activates normal fixing when creating the mesh.
 * Normals will be smoothed out and calculated correctly for each vertex.
 */
#define DMDL_FIXNORMALS	(1<<0)

/** 
 * \brief Activates mesh optimizations when creating the mesh.
 * Vertices will be welded if they are close together and enables a smoother lighting and faster rendering.
 */
#define DMDL_OPTIMIZE	(1<<1)

/** 
 * \brief Clones the textures of a newly created entity from the original ones.
 */
#define DMDL_CLONE_TEX	(1<<2)

/** 
 * \brief Save dynamic models in a binary x format.
 */
#define D3DXF_FILEFORMAT_BINARY		0

/** 
 * \brief Save dynamic models in a text x format.
 */
#define D3DXF_FILEFORMAT_TEXT		1

/** 
 * \brief Save dynamic models in a compressed x format.
 */
#define D3DXF_FILEFORMAT_COMPRESSED	2

/**
 * \struct DynamicModelSettings
 * \brief Contains global settings for dynamic models.
 */
typedef struct
{
	/**
	 * \brief The x format DynamicModels should save its meshes with.
	 * 
	 * Values: \ref D3DXF_FILEFORMAT_BINARY, \ref D3DXF_FILEFORMAT_TEXT, \ref D3DXF_FILEFORMAT_COMPRESSED
	 */
	int xFormat;
	
	/**
	 * \brief DynamicModels System Flags.
	 * 
	 * Enable or disable the system flags to change the behaviour of DynamicModels.
	 * Flags: \ref DMDL_FIXNORMALS, \ref DMDL_OPTIMIZE, \ref DMDL_CLONE_TEX
	 */
	long flags;
} DynamicModelSettings;

/**
 * \brief Global dynamic models setting.
 *
 * All settings of DynamicModels can be set up in this variable.
 * Check \ref DynamicModelSettings for further information.
 */
DynamicModelSettings DMDLSettings;

/**
 * \brief Compares two vertices.
 * \return 0 if the both vertices aren't equal.
 */
int dmdl_vertex_equals(D3DVERTEX *v1,D3DVERTEX *v2);

/**
 * \brief Creates a new dynamic model
 * \return The newly created dynamic model.
 */
DynamicModel* dmdl_create();

/**
 * \brief Instantiates a dynamic model.
 * 
 * Creates a new instance of the model in the current level.
 * \param  model The dynamic model that should be instantiated.
 * \param  pos   The position where the model should be created.
 * \param  act   The action for the new ENTITY.
 * \return       A new ENTITY with the dynamic model as mesh.
 */
ENTITY* dmdl_create_instance(DynamicModel *model, VECTOR* pos, EVENT act);

/**
 * \brief Creates a new DirectX mesh for the dynamic model.
 * \param  model The dynamic model where the mesh should be created from.
 * \return       The DirectX mesh that represents the model.
 */
LPD3DXMESH dmdl_create_mesh(DynamicModel *model);

/**
 * \brief Frees the memory allocated by the dynamic model.
 * \param model The model to free.
 */
void dmdl_delete(DynamicModel *model);

/**
 * \brief Adds a new polygon to the dynamic model
 * \param  model The model where the face should be added.
 * \param  face  The face to be added.
 */
void dmdl_add_face(DynamicModel *model, DYNAMIC_FACE *face);

/**
 * \brief Adds a new polygon to the dynamic model
 * \param  model The model where the face should be added.
 * \param  quad  The quad to be added.
 */
void dmdl_add_quad(DynamicModel *model, DYNAMIC_QUAD *quad);

/**
 * \brief Adds an existing ENTITY to the dynamic model.
 * \param model The model where the ENTITY should be added.
 * \param ent   The entity to be added. Scale, Rotation and Position will be preserved.
 */
void dmdl_add_entity(DynamicModel *model, ENTITY *ent);

/**
 * \brief Adds an existing DirectX mesh to the dynamic model.
 * \param model The model where the mesh should be added.
 * \param mesh  The mesh to be added.
 */
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh);

/**
 * \brief Adds an existing DirectX mesh to the dynamic model.
 * \param model  The model where the mesh should be added.
 * \param mesh   The mesh to be added.
 * \param offset The offset for each vertex in the mesh.
 */
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset);

/**
 * \brief Adds an existing DirectX mesh to the dynamic model.
 * \param model    The model where the mesh should be added.
 * \param mesh     The mesh to be added.
 * \param offset   The offset for each vertex in the mesh.
 * \param rotation The rotation for each vertex to be applied before moving.
 */
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation);

/**
 * \brief Adds an existing DirectX mesh to the dynamic model.
 * \param model    The model where the mesh should be added.
 * \param mesh     The mesh to be added.
 * \param offset   The offset for each vertex in the mesh.
 * \param rotation The rotation for each vertex to be applied before moving.
 * \param scale    The scale for each vertex to be applied before rotating.
 */
void dmdl_add_mesh(DynamicModel *model, LPD3DXMESH mesh, VECTOR *offset, ANGLE *rotation, VECTOR *scale);

/**
 * \brief Saves a dynamic model into a DirectX x file
 * \param model    The model to be saved.
 * \param filename The path to the x file where the model should be saved.
 */
void dmdl_save(DynamicModel *model, char *filename);


/**
 * \brief Adds a vertex to a model
 * \param   _model          The model
 * \param   _v              The new vertex
 * \return                  Index of the new vertex in the vertex buffer
 */
int dmdl_add_vertex(DynamicModel *_model, D3DVERTEX *_v);

/**
 * \brief Connects 3 vertices to a face
 * \param   _model          The model
 * \param   _v1             First vertex (index)
 * \param   _v2             Second vertex (index)
 * \param   _v3             Third vertex (index)
 */
void dmdl_connect_vertices(DynamicModel *_model, int _v1, int _v2, int _v3);

#include "DynamicModels.c"

#endif