// Vertex shader
#version 420

uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace;

uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mModel;
uniform mat4 mWorldInvTranspose;

uniform bool isASkyBox;

uniform sampler2D texHeightMap;		// Is Texture Unit 0 no matter WHAT! WHY???
uniform bool bIsHeightMap;

in vec4 vCol;						// Color
in vec3 vPos;						// Position
in vec3 vNorm;						// Vertex normal
in vec4 uvX2;						// UV 1 and 2
// Added for bump mapping:
in vec3 vTangent;		// For bump (or normal) mapping
in vec3 vBitangent;		// For bump (or normal) mapping
// For skinned mesh
in vec4 vBoneIDs_x4;		// IS OK. Note these are only used in the vertex shader
in vec4 vBoneWeights_x4;	// IS OK. Note these are only used in the vertex shader  

// For skinned mesh
const int MAXNUMBEROFBONES = 100;
uniform mat4 bones[MAXNUMBEROFBONES];
// Pass the acutal number you are using to control how often this loops
uniform int numBonesUsed;			
uniform bool bIsASkinnedMesh;

out vec4 fColor;				// was: vec4
out vec3 fVertNormal;		// Also in "world" (no view or projection)
out vec3 fVecWorldPosition;	// 
out vec4 fUV_X2;			// Added: UV 1 and 2 to fragment
out mat3 fTBN;

void main()
{
	vec4 vertPosition = vec4(vPos, 1.0f);
	mat4 matModel = mModel;

	if (!bIsASkinnedMesh)
	{
		// Calculate the model view projection matrix here
		mat4 MVP = mProjection * mView * matModel;
		gl_Position = MVP * vertPosition;
		fVecWorldPosition = vec3( matModel * vertPosition ).xyz;
		fVertNormal = vec3( mWorldInvTranspose * vec4(vNorm, 1.0f) ).xyz;

		// TBN matrix
		vec3 T = normalize(vec3(mModel * vec4(vTangent,   0.0)));
		vec3 B = normalize(vec3(mModel * vec4(vBitangent, 0.0)));
		vec3 N = normalize(vec3(mModel * vec4(vNorm,    0.0)));
		fTBN = mat3(T, B, N);
	}
	else
	{
		mat4 BoneTransform = bones[ int(vBoneIDs_x4[0]) ] * vBoneWeights_x4[0];
		BoneTransform += bones[ int(vBoneIDs_x4[1]) ] * vBoneWeights_x4[1];
		BoneTransform += bones[ int(vBoneIDs_x4[2]) ] * vBoneWeights_x4[2];
		BoneTransform += bones[ int(vBoneIDs_x4[3]) ] * vBoneWeights_x4[3];

		vertPosition = BoneTransform * vertPosition;
		
		mat4 matMVP = mProjection * mView * mModel;
		
		// Final screen space position	
		gl_Position = matMVP * vertPosition;	

		// Inverse transform to keep ONLY rotation...
		mat4 matNormal =  mWorldInvTranspose * BoneTransform;
		//
		fVertNormal = mat3(matNormal) * normalize(vNorm.xyz);

		vec3 normal = mat3(matNormal) * normalize(vNorm.xyz);
		vec3 tangent = mat3(matNormal) * normalize(vTangent.xyz);
		vec3 bitangent = mat3(matNormal) * normalize(vBitangent.xyz);
		
		fVecWorldPosition = (mModel * vertPosition).xyz;

		// TBN matrix
		vec3 T = normalize(vec3(mModel * vec4(tangent,   0.0)));
		vec3 B = normalize(vec3(mModel * vec4(bitangent, 0.0)));
		vec3 N = normalize(vec3(mModel * vec4(normal,    0.0)));
		fTBN = mat3(T, B, N);
	}
	
    fColor = vCol;
	fUV_X2 = uvX2;
	vec3 FragPos = vec3(mModel * vec4(vPos, 1.0));
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}

