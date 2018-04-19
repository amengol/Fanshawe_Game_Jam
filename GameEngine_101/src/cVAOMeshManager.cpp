#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include "sVertex.h"

cVAOMeshManager::cVAOMeshManager()
{
	return;
}

cVAOMeshManager::~cVAOMeshManager()
{
	return;
}

bool cVAOMeshManager::loadMeshesIntoStaticVAO(std::vector<cMesh>& meshes, int shaderID, bool bKeepMesh)
{
    std::vector<sVAOInfo> vecVAOs;

    for (size_t i = 0; i < meshes.size(); i++)
    {
        // ******************************************************************************
        //__      __       _              ____         __  __          
        //\ \    / /      | |            |  _ \       / _|/ _|         
        // \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
        //  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
        //   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
        //    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
        //                                                             
        // ******************************************************************************

        sVAOInfo theVAOInfo;

        // Create a Vertex Array Object (VAO)
        glGenVertexArrays(1, &(theVAOInfo.VAO_ID));
        glBindVertexArray(theVAOInfo.VAO_ID);

        glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID));
        glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

        // Allocate the global vertex array
        sVertex* pVertices = new sVertex[meshes[i].numberOfVertices];

        for (int index = 0; index < meshes[i].numberOfVertices; index++)
        {
            pVertices[index].x = meshes[i].pVertices[index].x;
            pVertices[index].y = meshes[i].pVertices[index].y;
            pVertices[index].z = meshes[i].pVertices[index].z;

            pVertices[index].r = meshes[i].pVertices[index].r;
            pVertices[index].g = meshes[i].pVertices[index].g;
            pVertices[index].b = meshes[i].pVertices[index].b;

            pVertices[index].nx = meshes[i].pVertices[index].nx;
            pVertices[index].ny = meshes[i].pVertices[index].ny;
            pVertices[index].nz = meshes[i].pVertices[index].nz;

            // Other additions: texture coords, tangent, and bi-normal
            pVertices[index].u1 = meshes[i].pVertices[index].u1;
            pVertices[index].v1 = meshes[i].pVertices[index].v1;
            pVertices[index].u2 = meshes[i].pVertices[index].u2;
            pVertices[index].v2 = meshes[i].pVertices[index].v2;

            pVertices[index].bx = meshes[i].pVertices[index].bx;
            pVertices[index].by = meshes[i].pVertices[index].by;
            pVertices[index].bz = meshes[i].pVertices[index].bz;
           
            pVertices[index].tx = meshes[i].pVertices[index].tx;
            pVertices[index].ty = meshes[i].pVertices[index].ty;
            pVertices[index].tz = meshes[i].pVertices[index].tz;

            pVertices[index].boneID[0] = meshes[i].pVertices[index].boneID[0];
            pVertices[index].boneID[1] = meshes[i].pVertices[index].boneID[1];
            pVertices[index].boneID[2] = meshes[i].pVertices[index].boneID[2];
            pVertices[index].boneID[3] = meshes[i].pVertices[index].boneID[3];

            pVertices[index].boneWeights[0] = meshes[i].pVertices[index].boneWeights[0];
            pVertices[index].boneWeights[1] = meshes[i].pVertices[index].boneWeights[1];
            pVertices[index].boneWeights[2] = meshes[i].pVertices[index].boneWeights[2];
            pVertices[index].boneWeights[3] = meshes[i].pVertices[index].boneWeights[3];
        }

        // Copy the local vertex array into the GPUs memory
        int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * meshes[i].numberOfVertices;
        glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

        delete[] pVertices;

        // ******************************************************************************
        //  _____           _           ____         __  __          
        // |_   _|         | |         |  _ \       / _|/ _|         
        //   | |  _ __   __| | _____  _| |_) |_   _| |_| |_ ___ _ __ 
        //   | | | '_ \ / _` |/ _ \ \/ /  _ <| | | |  _|  _/ _ \ '__|
        //  _| |_| | | | (_| |  __/>  <| |_) | |_| | | | ||  __/ |   
        // |_____|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_| \___|_|   
        //                                                           
        // ******************************************************************************

        // Create a Index Array
        glGenBuffers(1, &(theVAOInfo.index_buffer_ID));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID); // It's now an Index Buffer

        int numberOfIndices = meshes[i].numberOfTriangles * 3;

        unsigned int* indexArray = new unsigned int[numberOfIndices];

        // Copy the triangle data into this 1D array 
        int triIndex = 0;		// Index into the triangle array (from mesh)
        int indexIndex = 0;		// Index into the "vertex index" array (1D)
        for (; triIndex < meshes[i].numberOfTriangles; triIndex++, indexIndex += 3)
        {
            indexArray[indexIndex + 0] = meshes[i].pTriangles[triIndex].vertex_ID_0;
            indexArray[indexIndex + 1] = meshes[i].pTriangles[triIndex].vertex_ID_1;
            indexArray[indexIndex + 2] = meshes[i].pTriangles[triIndex].vertex_ID_2;
        }
        int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, indexArray, GL_STATIC_DRAW);

        // Don't need local array anymore
        delete[] indexArray;

        // ******************************************************************************

        // At this point, the vertex and index buffers are stored on GPU

        // Now set up the vertex layout (for this shader)

        GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
        GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");
        GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");
        GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");
        GLuint vTangent_location = glGetAttribLocation(shaderID, "vTangent");
        GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");
        GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");
        GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");

        // Size of the vertex we are using in the array.
        // This is called the "stride" of the vertices in the vertex buffer
        const unsigned int VERTEX_STRIDE = sizeof(sVertex);

        glEnableVertexAttribArray(vpos_location);
        const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
        glVertexAttribPointer(vpos_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));

        glEnableVertexAttribArray(vcol_location);
        const unsigned int OFFSET_TO_R = offsetof(sVertex, r);
        glVertexAttribPointer(vcol_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R)));

        glEnableVertexAttribArray(vnorm_location);
        const unsigned int OFFSET_TO_NX = offsetof(sVertex, nx);
        glVertexAttribPointer(vnorm_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX)));

        glEnableVertexAttribArray(vUVx2_location);
        const unsigned int OFFSET_TO_UVs = offsetof(sVertex, u1);
        glVertexAttribPointer(vUVx2_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs)));

        glEnableVertexAttribArray(vTangent_location);
        const unsigned int OFFSET_TO_vTangent = offsetof(sVertex, tx);
        glVertexAttribPointer(vTangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vTangent)));

        glEnableVertexAttribArray(vBitangent_location);
        const unsigned int OFFSET_TO_vBitangent = offsetof(sVertex, bx);
        glVertexAttribPointer(vBitangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBitangent)));

        glEnableVertexAttribArray(vBoneIDs_x4_location);
        const unsigned int OFFSET_TO_vBoneIDs = (unsigned int)offsetof(sVertex, boneID[0]);
        glVertexAttribPointer(vBoneIDs_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneIDs)));

        glEnableVertexAttribArray(vBoneWeights_x4_location);
        const unsigned int OFFSET_TO_vBoneWeights_x4 = (unsigned int)offsetof(sVertex, boneWeights[0]);
        glVertexAttribPointer(vBoneWeights_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneWeights_x4)));

        // Copy the information into the VAOInfo structure
        theVAOInfo.numberOfIndices = meshes[i].numberOfTriangles * 3;
        theVAOInfo.numberOfTriangles = meshes[i].numberOfTriangles;
        theVAOInfo.numberOfVertices = meshes[i].numberOfVertices;
        theVAOInfo.friendlyName = meshes[i].name;
        theVAOInfo.shaderID = shaderID;

        meshes[i].CalculateExtents();
        theVAOInfo.scaleForUnitBox = meshes[i].scaleForUnitBox;

        vecVAOs.push_back(theVAOInfo);

        // CRITICAL 
        // Unbind the VAO first!
        glBindVertexArray(0);	// 

                                // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vcol_location);
        glDisableVertexAttribArray(vpos_location);
        glDisableVertexAttribArray(vnorm_location);
        glDisableVertexAttribArray(vUVx2_location);
        glDisableVertexAttribArray(vTangent_location);				// in vec3 vTangent;
        glDisableVertexAttribArray(vBitangent_location);			// in vec3 vBitangent;
        glDisableVertexAttribArray(vBoneIDs_x4_location);			// in vec4 vBoneIDs_x4;		
        glDisableVertexAttribArray(vBoneWeights_x4_location);		// 	in vec4 vBoneWeights_x4;	
   
    }


    // Store the VAO info by mesh name
    this->m_mapNameToStaticVAOs[meshes[0].name] = vecVAOs;

    if (bKeepMesh)
    {	
        this->m_mapNameToMeshes[meshes[0].name] = meshes;
    }

	return true;
}

bool cVAOMeshManager::loadMeshIntoStaticVAO(cMesh& theMesh, int shaderID, bool bKeepMesh)
{
    std::map< std::string, std::vector<sVAOInfo> >::iterator itVAOs 
        = m_mapNameToStaticVAOs.find(theMesh.name);

    if (itVAOs == m_mapNameToStaticVAOs.end())
    {
        // There is no sVAOInfo vector with that name yet
        std::vector<sVAOInfo> vecVAOs;

        // ******************************************************************************
        //__      __       _              ____         __  __          
        //\ \    / /      | |            |  _ \       / _|/ _|         
        // \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
        //  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
        //   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
        //    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
        //                                                             
        // ******************************************************************************

        sVAOInfo theVAOInfo;

        // Create a Vertex Array Object (VAO)
        glGenVertexArrays(1, &(theVAOInfo.VAO_ID));
        glBindVertexArray(theVAOInfo.VAO_ID);

        glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID));
        glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

        // Init secondary UVs in case it is a multi layer object
        theMesh.InitPlaneSecondaryUvs();

        // Allocate the global vertex array
        sVertex* pVertices = new sVertex[theMesh.numberOfVertices];

        for (int index = 0; index < theMesh.numberOfVertices; index++)
        {
            pVertices[index].x = theMesh.pVertices[index].x;
            pVertices[index].y = theMesh.pVertices[index].y;
            pVertices[index].z = theMesh.pVertices[index].z;

            pVertices[index].r = theMesh.pVertices[index].r;
            pVertices[index].g = theMesh.pVertices[index].g;
            pVertices[index].b = theMesh.pVertices[index].b;

            pVertices[index].nx = theMesh.pVertices[index].nx;
            pVertices[index].ny = theMesh.pVertices[index].ny;
            pVertices[index].nz = theMesh.pVertices[index].nz;

            // Other additions: texture coords, tangent, and bi-normal
            pVertices[index].u1 = theMesh.pVertices[index].u1;
            pVertices[index].v1 = theMesh.pVertices[index].v1;
            pVertices[index].u2 = theMesh.pVertices[index].u2;
            pVertices[index].v2 = theMesh.pVertices[index].v2;

            pVertices[index].bx = theMesh.pVertices[index].bx;
            pVertices[index].by = theMesh.pVertices[index].by;
            pVertices[index].bz = theMesh.pVertices[index].bz;

            pVertices[index].tx = theMesh.pVertices[index].tx;
            pVertices[index].ty = theMesh.pVertices[index].ty;
            pVertices[index].tz = theMesh.pVertices[index].tz;

            pVertices[index].boneID[0] = theMesh.pVertices[index].boneID[0];
            pVertices[index].boneID[1] = theMesh.pVertices[index].boneID[1];
            pVertices[index].boneID[2] = theMesh.pVertices[index].boneID[2];
            pVertices[index].boneID[3] = theMesh.pVertices[index].boneID[3];

            pVertices[index].boneWeights[0] = theMesh.pVertices[index].boneWeights[0];
            pVertices[index].boneWeights[1] = theMesh.pVertices[index].boneWeights[1];
            pVertices[index].boneWeights[2] = theMesh.pVertices[index].boneWeights[2];
            pVertices[index].boneWeights[3] = theMesh.pVertices[index].boneWeights[3];
        }

        // Copy the local vertex array into the GPUs memory
        int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * theMesh.numberOfVertices;
        glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

        delete[] pVertices;

        // ******************************************************************************
        //  _____           _           ____         __  __          
        // |_   _|         | |         |  _ \       / _|/ _|         
        //   | |  _ __   __| | _____  _| |_) |_   _| |_| |_ ___ _ __ 
        //   | | | '_ \ / _` |/ _ \ \/ /  _ <| | | |  _|  _/ _ \ '__|
        //  _| |_| | | | (_| |  __/>  <| |_) | |_| | | | ||  __/ |   
        // |_____|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_| \___|_|   
        //                                                           
        // ******************************************************************************

        // Create a Index Array
        glGenBuffers(1, &(theVAOInfo.index_buffer_ID));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID); // It's now an Index Buffer

        int numberOfIndices = theMesh.numberOfTriangles * 3;

        unsigned int* indexArray = new unsigned int[numberOfIndices];

        // Copy the triangle data into this 1D array 
        int triIndex = 0;		// Index into the triangle array (from mesh)
        int indexIndex = 0;		// Index into the "vertex index" array (1D)
        for (; triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3)
        {
            indexArray[indexIndex + 0] = theMesh.pTriangles[triIndex].vertex_ID_0;
            indexArray[indexIndex + 1] = theMesh.pTriangles[triIndex].vertex_ID_1;
            indexArray[indexIndex + 2] = theMesh.pTriangles[triIndex].vertex_ID_2;
        }
        int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, indexArray, GL_STATIC_DRAW);

        // Don't need local array anymore
        delete[] indexArray;

        // ******************************************************************************

        // At this point, the vertex and index buffers are stored on GPU

        // Now set up the vertex layout (for this shader)

        GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
        GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");
        GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");
        GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");
        GLuint vTangent_location = glGetAttribLocation(shaderID, "vTangent");
        GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");
        GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");
        GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");

        // Size of the vertex we are using in the array.
        // This is called the "stride" of the vertices in the vertex buffer
        const unsigned int VERTEX_STRIDE = sizeof(sVertex);

        glEnableVertexAttribArray(vpos_location);
        const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
        glVertexAttribPointer(vpos_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));

        glEnableVertexAttribArray(vcol_location);
        const unsigned int OFFSET_TO_R = offsetof(sVertex, r);
        glVertexAttribPointer(vcol_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R)));

        glEnableVertexAttribArray(vnorm_location);
        const unsigned int OFFSET_TO_NX = offsetof(sVertex, nx);
        glVertexAttribPointer(vnorm_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX)));

        glEnableVertexAttribArray(vUVx2_location);
        const unsigned int OFFSET_TO_UVs = offsetof(sVertex, u1);
        glVertexAttribPointer(vUVx2_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs)));

        glEnableVertexAttribArray(vTangent_location);
        const unsigned int OFFSET_TO_vTangent = offsetof(sVertex, tx);
        glVertexAttribPointer(vTangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vTangent)));

        glEnableVertexAttribArray(vBitangent_location);
        const unsigned int OFFSET_TO_vBitangent = offsetof(sVertex, bx);
        glVertexAttribPointer(vBitangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBitangent)));

        glEnableVertexAttribArray(vBoneIDs_x4_location);
        const unsigned int OFFSET_TO_vBoneIDs = (unsigned int)offsetof(sVertex, boneID[0]);
        glVertexAttribPointer(vBoneIDs_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneIDs)));

        glEnableVertexAttribArray(vBoneWeights_x4_location);
        const unsigned int OFFSET_TO_vBoneWeights_x4 = (unsigned int)offsetof(sVertex, boneWeights[0]);
        glVertexAttribPointer(vBoneWeights_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneWeights_x4)));

        // Copy the information into the VAOInfo structure
        theVAOInfo.numberOfIndices = theMesh.numberOfTriangles * 3;
        theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
        theVAOInfo.numberOfVertices = theMesh.numberOfVertices;
        theVAOInfo.friendlyName = theMesh.name;
        theVAOInfo.shaderID = shaderID;

        theMesh.CalculateExtents();
        theVAOInfo.scaleForUnitBox = theMesh.scaleForUnitBox;

        vecVAOs.push_back(theVAOInfo);

        // CRITICAL 
        // Unbind the VAO first!
        glBindVertexArray(0);	// 

                                // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vcol_location);
        glDisableVertexAttribArray(vpos_location);
        glDisableVertexAttribArray(vnorm_location);
        glDisableVertexAttribArray(vUVx2_location);
        glDisableVertexAttribArray(vTangent_location);				// in vec3 vTangent;
        glDisableVertexAttribArray(vBitangent_location);			// in vec3 vBitangent;
        glDisableVertexAttribArray(vBoneIDs_x4_location);			// in vec4 vBoneIDs_x4;		
        glDisableVertexAttribArray(vBoneWeights_x4_location);		// 	in vec4 vBoneWeights_x4;	

        // Store the VAO info by mesh name
        this->m_mapNameToStaticVAOs[theMesh.name] = vecVAOs;

        if (bKeepMesh)
        {
            std::vector<cMesh> vecMeshes;
            vecMeshes.push_back(theMesh);
            this->m_mapNameToMeshes[theMesh.name] = vecMeshes;
        }
    }
    else
    {
        // There is a mesh with that name already
        // ******************************************************************************
        //__      __       _              ____         __  __          
        //\ \    / /      | |            |  _ \       / _|/ _|         
        // \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
        //  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
        //   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
        //    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
        //                                                             
        // ******************************************************************************

        sVAOInfo theVAOInfo;

        // Create a Vertex Array Object (VAO)
        glGenVertexArrays(1, &(theVAOInfo.VAO_ID));
        glBindVertexArray(theVAOInfo.VAO_ID);

        glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID));
        glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

        // Allocate the global vertex array
        sVertex* pVertices = new sVertex[theMesh.numberOfVertices];

        for (int index = 0; index < theMesh.numberOfVertices; index++)
        {
            pVertices[index].x = theMesh.pVertices[index].x;
            pVertices[index].y = theMesh.pVertices[index].y;
            pVertices[index].z = theMesh.pVertices[index].z;

            pVertices[index].r = theMesh.pVertices[index].r;
            pVertices[index].g = theMesh.pVertices[index].g;
            pVertices[index].b = theMesh.pVertices[index].b;

            pVertices[index].nx = theMesh.pVertices[index].nx;
            pVertices[index].ny = theMesh.pVertices[index].ny;
            pVertices[index].nz = theMesh.pVertices[index].nz;

            // Other additions: texture coords, tangent, and bi-normal
            pVertices[index].u1 = theMesh.pVertices[index].u1;
            pVertices[index].v1 = theMesh.pVertices[index].v1;
            pVertices[index].u2 = theMesh.pVertices[index].u2;
            pVertices[index].v2 = theMesh.pVertices[index].v2;

            pVertices[index].bx = theMesh.pVertices[index].bx;
            pVertices[index].by = theMesh.pVertices[index].by;
            pVertices[index].bz = theMesh.pVertices[index].bz;

            pVertices[index].tx = theMesh.pVertices[index].tx;
            pVertices[index].ty = theMesh.pVertices[index].ty;
            pVertices[index].tz = theMesh.pVertices[index].tz;

            pVertices[index].boneID[0] = theMesh.pVertices[index].boneID[0];
            pVertices[index].boneID[1] = theMesh.pVertices[index].boneID[1];
            pVertices[index].boneID[2] = theMesh.pVertices[index].boneID[2];
            pVertices[index].boneID[3] = theMesh.pVertices[index].boneID[3];

            pVertices[index].boneWeights[0] = theMesh.pVertices[index].boneWeights[0];
            pVertices[index].boneWeights[1] = theMesh.pVertices[index].boneWeights[1];
            pVertices[index].boneWeights[2] = theMesh.pVertices[index].boneWeights[2];
            pVertices[index].boneWeights[3] = theMesh.pVertices[index].boneWeights[3];
        }

        // Copy the local vertex array into the GPUs memory
        int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * theMesh.numberOfVertices;
        glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

        delete[] pVertices;

        // ******************************************************************************
        //  _____           _           ____         __  __          
        // |_   _|         | |         |  _ \       / _|/ _|         
        //   | |  _ __   __| | _____  _| |_) |_   _| |_| |_ ___ _ __ 
        //   | | | '_ \ / _` |/ _ \ \/ /  _ <| | | |  _|  _/ _ \ '__|
        //  _| |_| | | | (_| |  __/>  <| |_) | |_| | | | ||  __/ |   
        // |_____|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_| \___|_|   
        //                                                           
        // ******************************************************************************

        // Create a Index Array
        glGenBuffers(1, &(theVAOInfo.index_buffer_ID));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID); // It's now an Index Buffer

        int numberOfIndices = theMesh.numberOfTriangles * 3;

        unsigned int* indexArray = new unsigned int[numberOfIndices];

        // Copy the triangle data into this 1D array 
        int triIndex = 0;		// Index into the triangle array (from mesh)
        int indexIndex = 0;		// Index into the "vertex index" array (1D)
        for (; triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3)
        {
            indexArray[indexIndex + 0] = theMesh.pTriangles[triIndex].vertex_ID_0;
            indexArray[indexIndex + 1] = theMesh.pTriangles[triIndex].vertex_ID_1;
            indexArray[indexIndex + 2] = theMesh.pTriangles[triIndex].vertex_ID_2;
        }
        int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, indexArray, GL_STATIC_DRAW);

        // Don't need local array anymore
        delete[] indexArray;

        // ******************************************************************************

        // At this point, the vertex and index buffers are stored on GPU

        // Now set up the vertex layout (for this shader)

        GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
        GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");
        GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");
        GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");
        GLuint vTangent_location = glGetAttribLocation(shaderID, "vTangent");
        GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");
        GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");
        GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");

        // Size of the vertex we are using in the array.
        // This is called the "stride" of the vertices in the vertex buffer
        const unsigned int VERTEX_STRIDE = sizeof(sVertex);

        glEnableVertexAttribArray(vpos_location);
        const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
        glVertexAttribPointer(vpos_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));

        glEnableVertexAttribArray(vcol_location);
        const unsigned int OFFSET_TO_R = offsetof(sVertex, r);
        glVertexAttribPointer(vcol_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R)));

        glEnableVertexAttribArray(vnorm_location);
        const unsigned int OFFSET_TO_NX = offsetof(sVertex, nx);
        glVertexAttribPointer(vnorm_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX)));

        glEnableVertexAttribArray(vUVx2_location);
        const unsigned int OFFSET_TO_UVs = offsetof(sVertex, u1);
        glVertexAttribPointer(vUVx2_location, 4,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs)));

        glEnableVertexAttribArray(vTangent_location);
        const unsigned int OFFSET_TO_vTangent = offsetof(sVertex, tx);
        glVertexAttribPointer(vTangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vTangent)));

        glEnableVertexAttribArray(vBitangent_location);
        const unsigned int OFFSET_TO_vBitangent = offsetof(sVertex, bx);
        glVertexAttribPointer(vBitangent_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBitangent)));

        glEnableVertexAttribArray(vBoneIDs_x4_location);
        const unsigned int OFFSET_TO_vBoneIDs = (unsigned int)offsetof(sVertex, boneID[0]);
        glVertexAttribPointer(vBoneIDs_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneIDs)));

        glEnableVertexAttribArray(vBoneWeights_x4_location);
        const unsigned int OFFSET_TO_vBoneWeights_x4 = (unsigned int)offsetof(sVertex, boneWeights[0]);
        glVertexAttribPointer(vBoneWeights_x4_location,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneWeights_x4)));

        // Copy the information into the VAOInfo structure
        theVAOInfo.numberOfIndices = theMesh.numberOfTriangles * 3;
        theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
        theVAOInfo.numberOfVertices = theMesh.numberOfVertices;
        theVAOInfo.friendlyName = theMesh.name;
        theVAOInfo.shaderID = shaderID;

        theMesh.CalculateExtents();
        theVAOInfo.scaleForUnitBox = theMesh.scaleForUnitBox;

        itVAOs->second.push_back(theVAOInfo);

        // CRITICAL 
        // Unbind the VAO first!
        glBindVertexArray(0);	// 

                                // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vcol_location);
        glDisableVertexAttribArray(vpos_location);
        glDisableVertexAttribArray(vnorm_location);
        glDisableVertexAttribArray(vUVx2_location);
        glDisableVertexAttribArray(vTangent_location);				// in vec3 vTangent;
        glDisableVertexAttribArray(vBitangent_location);			// in vec3 vBitangent;
        glDisableVertexAttribArray(vBoneIDs_x4_location);			// in vec4 vBoneIDs_x4;		
        glDisableVertexAttribArray(vBoneWeights_x4_location);		// 	in vec4 vBoneWeights_x4;	


        if (bKeepMesh)
        {
            std::map<std::string, std::vector<cMesh>>::iterator itNameToMeshes 
                = this->m_mapNameToMeshes.find(theMesh.name);
            if (itNameToMeshes == this->m_mapNameToMeshes.end())
            {
                std::vector<cMesh> vecMeshes;
                vecMeshes.push_back(theMesh);
                this->m_mapNameToMeshes[theMesh.name] = vecMeshes;
            }
            else
            {
                itNameToMeshes->second.push_back(theMesh);
            }
            
        }
    }
    

    return true;
}

bool cVAOMeshManager::lookupStaticVAOsFromName(std::string name, std::vector<sVAOInfo>& vecVAOInfo)
{
	// look up in map for the name of the mesh we want to draw
	std::map< std::string, std::vector<sVAOInfo> >::iterator itVAOs 
					= this->m_mapNameToStaticVAOs.find( name );

	// Did I find something?
	if (itVAOs == this->m_mapNameToStaticVAOs.end() )
	{
		return false;
	}

    vecVAOInfo = itVAOs->second;

	return true;
}

bool cVAOMeshManager::lookupMeshesFromName(std::string name, std::vector<cMesh>& meshes)
{
    // Search for mesh by name using iterator based find
    std::map< std::string, std::vector<cMesh> >::iterator itMeshes =
        this->m_mapNameToMeshes.find(name);

    // Find it? 
    if (itMeshes == this->m_mapNameToMeshes.end())
    {	// Nope. 
        return false;
    }

    meshes = itMeshes->second;
    return true;
}

bool cVAOMeshManager::lookupMeshFromName(std::string name, cMesh & theMesh)
{
    // Search for mesh by name using iterator based find
    std::map< std::string, std::vector<cMesh> >::iterator itMeshes =
        this->m_mapNameToMeshes.find(name);

    // Find it? 
    if (itMeshes == this->m_mapNameToMeshes.end())
    {	// Nope. 
        return false;
    }

    theMesh = itMeshes->second[0];
    return true;
}

void cVAOMeshManager::genDynamicVAO(std::string name, int shaderID)
{
    sVAOInfo theVAOInfo;

    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(1, &(theVAOInfo.VAO_ID));
    glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID));
    glGenBuffers(1, &(theVAOInfo.index_buffer_ID));
   
    theVAOInfo.friendlyName = name;
    theVAOInfo.shaderID = shaderID;

    // Store the VAO info by name
    this->m_mapNameToDynamicVAO[name] = theVAOInfo;

}

void cVAOMeshManager::lookupDynamicVAOFromName(std::string name, sVAOInfo & theVAOInfo)
{
    // look up in map for the name of the mesh we want to draw
    std::map< std::string, sVAOInfo >::iterator itVAO
        = this->m_mapNameToDynamicVAO.find(name);

    // Did I find something?
    if (itVAO == this->m_mapNameToDynamicVAO.end())
    {
        return;
    }

    theVAOInfo = itVAO->second;
}

void cVAOMeshManager::loadMeshIntoDynamicVAO(cMesh& theMesh, std::string SVAOInfoName)
{
    // ******************************************************************************
    //__      __       _              ____         __  __          
    //\ \    / /      | |            |  _ \       / _|/ _|         
    // \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
    //  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
    //   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
    //    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
    //                                                             
    // ******************************************************************************

    sVAOInfo theVAOInfo = this->m_mapNameToDynamicVAO[SVAOInfoName];
    int shaderID = theVAOInfo.shaderID;

    glBindVertexArray(theVAOInfo.VAO_ID);
    glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

    // Allocate the global vertex array
    sVertex* pVertices = new sVertex[theMesh.numberOfVertices];

    for (int index = 0; index < theMesh.numberOfVertices; index++)
    {
        pVertices[index].x = theMesh.pVertices[index].x;
        pVertices[index].y = theMesh.pVertices[index].y;
        pVertices[index].z = theMesh.pVertices[index].z;

        pVertices[index].r = theMesh.pVertices[index].r;
        pVertices[index].g = theMesh.pVertices[index].g;
        pVertices[index].b = theMesh.pVertices[index].b;

        pVertices[index].nx = theMesh.pVertices[index].nx;
        pVertices[index].ny = theMesh.pVertices[index].ny;
        pVertices[index].nz = theMesh.pVertices[index].nz;

        // Other additions: texture coords, tangent, and bi-normal
        pVertices[index].u1 = theMesh.pVertices[index].u1;
        pVertices[index].v1 = theMesh.pVertices[index].v1;
        pVertices[index].u2 = theMesh.pVertices[index].u2;
        pVertices[index].v2 = theMesh.pVertices[index].v2;

        pVertices[index].bx = theMesh.pVertices[index].bx;
        pVertices[index].by = theMesh.pVertices[index].by;
        pVertices[index].bz = theMesh.pVertices[index].bz;

        pVertices[index].tx = theMesh.pVertices[index].tx;
        pVertices[index].ty = theMesh.pVertices[index].ty;
        pVertices[index].tz = theMesh.pVertices[index].tz;

        pVertices[index].boneID[0] = theMesh.pVertices[index].boneID[0];
        pVertices[index].boneID[1] = theMesh.pVertices[index].boneID[1];
        pVertices[index].boneID[2] = theMesh.pVertices[index].boneID[2];
        pVertices[index].boneID[3] = theMesh.pVertices[index].boneID[3];

        pVertices[index].boneWeights[0] = theMesh.pVertices[index].boneWeights[0];
        pVertices[index].boneWeights[1] = theMesh.pVertices[index].boneWeights[1];
        pVertices[index].boneWeights[2] = theMesh.pVertices[index].boneWeights[2];
        pVertices[index].boneWeights[3] = theMesh.pVertices[index].boneWeights[3];
    }

    // Copy the local vertex array into the GPUs memory
    int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * theMesh.numberOfVertices;
    glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_DYNAMIC_DRAW);

    delete[] pVertices;

    // ******************************************************************************
    //  _____           _           ____         __  __          
    // |_   _|         | |         |  _ \       / _|/ _|         
    //   | |  _ __   __| | _____  _| |_) |_   _| |_| |_ ___ _ __ 
    //   | | | '_ \ / _` |/ _ \ \/ /  _ <| | | |  _|  _/ _ \ '__|
    //  _| |_| | | | (_| |  __/>  <| |_) | |_| | | | ||  __/ |   
    // |_____|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_| \___|_|   
    //                                                           
    // ******************************************************************************

    // Create a Index Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID);

    int numberOfIndices = theMesh.numberOfTriangles * 3;

    unsigned int* indexArray = new unsigned int[numberOfIndices];

    // Copy the triangle data into this 1D array 
    int triIndex = 0;		// Index into the triangle array (from mesh)
    int indexIndex = 0;		// Index into the "vertex index" array (1D)
    for (; triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3)
    {
        indexArray[indexIndex + 0] = theMesh.pTriangles[triIndex].vertex_ID_0;
        indexArray[indexIndex + 1] = theMesh.pTriangles[triIndex].vertex_ID_1;
        indexArray[indexIndex + 2] = theMesh.pTriangles[triIndex].vertex_ID_2;
    }
    int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, indexArray, GL_DYNAMIC_DRAW);

    // Don't need local array anymore
    delete[] indexArray;

    // ******************************************************************************

    // At this point, the vertex and index buffers are stored on GPU

    // Now set up the vertex layout (for this shader)

    GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
    GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");
    GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");
    GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");
    GLuint vTangent_location = glGetAttribLocation(shaderID, "vTangent");
    GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");
    GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");
    GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");

    // Size of the vertex we are using in the array.
    // This is called the "stride" of the vertices in the vertex buffer
    const unsigned int VERTEX_STRIDE = sizeof(sVertex);

    glEnableVertexAttribArray(vpos_location);
    const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
    glVertexAttribPointer(vpos_location, 3,
                          GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));

    glEnableVertexAttribArray(vcol_location);
    const unsigned int OFFSET_TO_R = offsetof(sVertex, r);
    glVertexAttribPointer(vcol_location, 4,
                          GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R)));

    glEnableVertexAttribArray(vnorm_location);
    const unsigned int OFFSET_TO_NX = offsetof(sVertex, nx);
    glVertexAttribPointer(vnorm_location, 3,
                          GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX)));

    glEnableVertexAttribArray(vUVx2_location);
    const unsigned int OFFSET_TO_UVs = offsetof(sVertex, u1);
    glVertexAttribPointer(vUVx2_location, 4,
                          GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs)));

    glEnableVertexAttribArray(vTangent_location);
    const unsigned int OFFSET_TO_vTangent = offsetof(sVertex, tx);
    glVertexAttribPointer(vTangent_location,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vTangent)));

    glEnableVertexAttribArray(vBitangent_location);
    const unsigned int OFFSET_TO_vBitangent = offsetof(sVertex, bx);
    glVertexAttribPointer(vBitangent_location,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBitangent)));

    glEnableVertexAttribArray(vBoneIDs_x4_location);
    const unsigned int OFFSET_TO_vBoneIDs = (unsigned int)offsetof(sVertex, boneID[0]);
    glVertexAttribPointer(vBoneIDs_x4_location,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneIDs)));

    glEnableVertexAttribArray(vBoneWeights_x4_location);
    const unsigned int OFFSET_TO_vBoneWeights_x4 = (unsigned int)offsetof(sVertex, boneWeights[0]);
    glVertexAttribPointer(vBoneWeights_x4_location,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneWeights_x4)));

    // Copy the information into the VAOInfo structure
    theVAOInfo.numberOfIndices = theMesh.numberOfTriangles * 3;
    theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
    theVAOInfo.numberOfVertices = theMesh.numberOfVertices;

    theMesh.CalculateExtents();
    theVAOInfo.scaleForUnitBox = theMesh.scaleForUnitBox;

    // Store the VAO info back (updated)
    this->m_mapNameToDynamicVAO[SVAOInfoName] = theVAOInfo;

    // CRITICAL 
    // Unbind the VAO first!
    glBindVertexArray(0);	// 

    // Unbind (release) everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vnorm_location);
    glDisableVertexAttribArray(vUVx2_location);
    glDisableVertexAttribArray(vTangent_location);				// in vec3 vTangent;
    glDisableVertexAttribArray(vBitangent_location);			// in vec3 vBitangent;
    glDisableVertexAttribArray(vBoneIDs_x4_location);			// in vec4 vBoneIDs_x4;		
    glDisableVertexAttribArray(vBoneWeights_x4_location);		// 	in vec4 vBoneWeights_x4;
}
