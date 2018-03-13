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
#include <../src/sVertex.h>
#include <../src/cMesh.h>


class cSkinnedMesh 
{
private:
    // The max number of bones that can affect a vertex
	static const int MAX_BONES_PER_VERTEX = 4;

	struct sVertexBoneData
	{
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

    std::string fileName;                                               //  Filename from where the skinned mesh was loaded
    std::string friendlyName;                                           //  A friendely name for the skinned mesh
    const aiScene* pScene;	                                            //  The aiScene
    Assimp::Importer mImporter;                                         //  The Importer
    std::map< std::string, const aiScene* > mapAnimationNameTo_pScene;	//  Maps animation names to scenes
    std::vector<sVertexBoneData> vecVertexBoneData;                     //  Vector of vertex bone data
    glm::mat4 mGlobalInverseTransformation;                             //  Global Inverse transformation matrix
    std::map<std::string, unsigned int> m_mapBoneNameToBoneIndex;       //  Maps bone names to bone indexes
    std::vector<sBoneInfo> mBoneInfo;                                   //  Vector of bones info
    unsigned int mNumBones;                                             //  Number of bones of the skinned mesh
    

public:

    glm::vec3 mLastHipPosition;     // The most recent hip position

	unsigned int m_numberOfVertices;
	unsigned int m_numberOfIndices;
	unsigned int m_numberOfTriangles;

	cSkinnedMesh(void);

    // Loads a skinned mesh from a file
	bool LoadMeshFromFile(const std::string &path, const std::string &filename);

    // Loads a mesh animation from a file
	bool LoadMeshAnimation(const std::string &path, const std::string &filename);

    // Adds a new animation scene
    void AddAnimationScene(const aiScene* scene, const std::string &animName);

    // Looks in the animation map and returns the total time
    float FindAnimationTotalTime(std::string animationName);

    // Looks in the animation map and returns the duration of an animation
    float GetAnimationDuration(const std::string animName);

    // Returns NULL (0) if there is no mesh at that index
    cMesh* CreateMeshObjectFromCurrentModel(unsigned int meshIndex = 0);

    // Transforms all bones according to an animation
    void BoneTransform(float TimeInSeconds,
                       std::string animationName,		// Now we can pick the animation
                       std::vector<glm::mat4> &FinalTransformation,
                       std::vector<glm::mat4> &Globals,
                       std::vector<glm::mat4> &Offsets);

    // Returns the duration of the default animationy
    float GetDefaultAnimationDuration(void);

    // Initializes the skinned mesh
    bool Initialize(void);

    // Calculates the Interpolated Rotatrion
    void CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion& out);
    // Calculates the Interpolated Position
    void CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);
    // Calculates the Interpolated Scaling
    void CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);

    // Calculates the GLM Interpolated Rotation
    void CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
    // Calculates the GLM Interpolated Position
    void CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
    // Calculates the GLM Interpolated Scale
    void CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

    // Returns the rotation of a node
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    // Returns the position of a node
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    // Returns the scaling of a node
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

    // Returns the NodeAnimation Channel
    const aiNodeAnim* FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);

    // Reads the node hierarchy
    void ReadNodeHierarchy(float AnimationTime,
                           std::string animationName,
                           const aiNode* pNode,
                           const glm::mat4 &parentTransformMatrix);

    // Loads the bones into the container
    void LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);

	// Sets a friendly name for the skinned mesh
    void SetFriendlyName(std::string name) { this->friendlyName = name; }

};

#endif
