
#include <iostream>
#include "AssimpLoader.h"

//////////////////////////////////////////////////////////////////////////////////////////
AssimpLoader::AssimpLoader()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
AssimpLoader::~AssimpLoader()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
bool	AssimpLoader::LoadAssimpMesh(const std::string& path, GLMesh** mesh)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_Quality);

	if(!scene)
	{
		std::cout << "============== AssimpLoader::LoadAssimpMesh() ==============" << std::endl;
		std::cout << "invalid aiScene* ptr value" << std::endl;
		return false; 
	}

	// if assimp scene has scene information
	if (scene->HasMeshes())
	{
		// for each mesh, extract material information
		for (unsigned int i = 0 ; i<scene->mNumMeshes ; ++i)
		{
			aiMesh* tmpMesh = scene->mMeshes[i];

			if(!tmpMesh)
			{
				std::cout << "============== AssimpLoader::LoadAssimpMesh() ==============" << std::endl;
				std::cout << "invalid aiMesh* ptr value" << std::endl;
				return false;
			}

			// Material loading..

			// Vertex loading...;
			// map vertex position, normals & texture co-ordinates
			unsigned int nextVertex = 0;
			for (unsigned int i = 0 ; i<tmpMesh->mNumVertices ; ++i)
			{
				VertexPN* tempVertex = new VertexPN(glm::vec3(tmpMesh->mVertices[i].x, tmpMesh->mVertices[i].y, tmpMesh->mVertices[i].z), 
													glm::vec3(tmpMesh->mNormals[i].x, tmpMesh->mNormals[i].y, tmpMesh->mNormals[i].z));
	


				/*if (tmpMesh->HasTextureCoords(0))
				{
					tmpVertex[nextVertex].texcoord = glm::vec2(tmpMesh->mTextureCoords[i]->x, tmpMesh->mTextureCoords[i]->y);
				}
				else
				{
					tmpVertex[nextVertex].texcoord = glm::vec2(0,0);
				}*/

				// push data to vector of vertices before moving onto next vertex
				m_vecVertices.push_back(*tempVertex);

				delete tempVertex;

				nextVertex++;
			}

			/// Just like Vertex buffer, we use mNumFaces variable from aiMesh to create the
			/// index buffer. We extract indices from the mesh & store them to our newly 
			/// created index buffer.
			GLuint nextIndex = 0;
			for (unsigned int j = 0 ; j<tmpMesh->mNumFaces ; ++j)
			{
				GLuint index1 = tmpMesh->mFaces[j].mIndices[0];
				m_vecIndices.push_back(index1);
				nextIndex++;

				GLuint index2 = tmpMesh->mFaces[j].mIndices[1];
				m_vecIndices.push_back(index2);
				nextIndex++;

				GLuint index3 = tmpMesh->mFaces[j].mIndices[2];
				m_vecIndices.push_back(index3);
				nextIndex++;
			}
		}
	}

	// Now feed the gathered data to GLMesh component
	(*mesh)->vertices = m_vecVertices;
	(*mesh)->indices = m_vecIndices;

	return true;
}