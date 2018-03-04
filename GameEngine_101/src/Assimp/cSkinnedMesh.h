#ifndef _cSkinnedMesh_HG
#define _cSkinnedMesh_HG

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

//#include "vert_XYZ_RGBA_N_STU_TanBi_4Bones.h"
#include "..\sVertex.h"

// Used by the game object and the rendering
#include "../cMesh.h"			// For loading into the VAO manager

//#include "../sMeshDrawInfo.h"	// For the draw call


class cSkinnedMesh 
{
private:
	static const int MAX_BONES_PER_VERTEX = 4;

	struct sVertexBoneData
	{
		//std::array<unsigned int, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> weights;

		void AddBoneData(unsigned int BoneID, float Weight);
	};

	struct sBoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
		glm::mat4 ObjectBoneTransformation;
	};

public:
	// Mesh and VAO and VBO infor
	//unsigned int m_VBO_ID;				// Vertex buffer object 
	unsigned int m_numberOfVertices;
	//unsigned int m_indexBuf_ID;			// Index buffer referring to VBO
	unsigned int m_numberOfIndices;
	unsigned int m_numberOfTriangles;
	//bool m_bVBO_created;
	//// Remember: a VAO is a VBO+shaderProgram combination (i.e. you need 1 VAO ++PER++ VBO+shaderID)
	//// (this is only an issue if you are using mulitple shaders; if you are using a single "uber" shader, you're fine)
	//unsigned int m_VAO_ID;				// Vertex Array Object
	//unsigned int m_shaderIDMatchingVAO;	// The shader prod ID matching this VBO
	//bool m_bVAO_created;
//	// Actual vertex data
//	vert_XYZ_RGBA_N_STU_TanBi_4Bones* m_pVertexData;

public:
	cSkinnedMesh(void);
	virtual ~cSkinnedMesh(void);

	bool LoadMeshFromFile(const std::string &filename);		// mesh we draw
	bool LoadMeshAnimation(const std::string &path, const std::string &filename);	// Only want animations
    void AddAnimationScene(const aiScene* scene, const std::string &animName);

	std::string fileName;
	std::string friendlyName;

	const aiScene* pScene;		// "pretty" mesh we update and draw
	Assimp::Importer mImporter;

	std::map< std::string /*animationfile*/,
		      const aiScene* > mapAnimationNameTo_pScene;		// Animations

	// Looks in the animation map and returns the total time
	float FindAnimationTotalTime( std::string animationName );

    // Looks in the animation map and returns the duration of an animation
    float GetAnimationDuration(const std::string animName);

	// Creates a VBO, loads the current mesh, then creates a VAO for the current VBO+shader
//	bool CreateVBOfromCurrentMesh(void);
//	bool CreateVBOandVOAfromCurrentMesh( int shaderProgID, std::string &error );

	// Returns NULL (0) if there is no mesh at that index
	cMesh* CreateMeshObjectFromCurrentModel( unsigned int meshIndex = 0 );

	// The sMeshDrawInfo is used during the render call to lookup
	//	VAO infomation. Also, the gross orientation of the model is from 
	//	here. So this meshDrawInfo is the mesh bind pose, really.
//	bool CreateMeshDrawInfoFromCurrentModel( sMeshDrawInfo &meshDrawInfo );

//	void BoneTransform(float TimeInSeconds, 
//					   std::vector<glm::mat4> &FinalTransformation, 
//					   std::vector<glm::mat4> &Globals, 
//					   std::vector<glm::mat4> &Offsets);
	void BoneTransform(float TimeInSeconds, 
					   std::string animationName,		// Now we can pick the animation
					   std::vector<glm::mat4> &FinalTransformation, 
					   std::vector<glm::mat4> &Globals, 
					   std::vector<glm::mat4> &Offsets);
	
	unsigned int GetNums(void) const { return this->mNumBones; }

	float GetDuration(void);
	
	std::vector<sVertexBoneData> vecVertexBoneData;	//Bones;
	glm::mat4 mGlobalInverseTransformation;


	bool Initialize(void);
	
	void CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion& out);
	void CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);
	void CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);

	void CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
	void CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
	void CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);
	
	//void ReadNodeHeirarchy(float AnimationTime, 
	//					   const aiNode* pNode, 
	//					   const glm::mat4 &parentTransformMatrix);

	void ReadNodeHeirarchy(float AnimationTime, 
						   std::string animationName,		// Now with more "choose animation"
						   const aiNode* pNode, 
						   const glm::mat4 &parentTransformMatrix);

	void LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);
	
	void ShutErDown(void);

//	std::vector<sMeshEntry> mMeshEntries;
	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> m_mapBoneNameToBoneIndex;	//mMapping;
	std::vector<sBoneInfo> mBoneInfo;
	unsigned int mNumBones;	//mNums;

};

#endif
