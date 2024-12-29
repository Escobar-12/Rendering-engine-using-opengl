#include "Model.h"



#include "Model.h"
#include <iostream> // For std::cerr and std::cout
#include <stdexcept> // For std::runtime_error

Model::Model(std::string Path) {
	try {
		loadModel(Path);
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load model: " << e.what() << std::endl;
	}
}

void Model::Draw(unsigned int Program) {
	if (Meshes.empty()) {
		std::cout << "Warning: No meshes to draw." << std::endl;
		return; 
	}
	
	for (size_t partMesh = 0; partMesh < Meshes.size(); ++partMesh) {
		Meshes[partMesh].Draw(Program);
	}
}

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;

	// Read the file using Assimp
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality);

	// Error handling
	if (!scene) {
		throw std::runtime_error(std::string("Assimp Error: ") + importer.GetErrorString());
	}
	if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		throw std::runtime_error("Model loading incomplete. Check file integrity.");
	}
	if (!scene->mRootNode) {
		throw std::runtime_error("Model has no root node.");
	}

	std::cout << "Model loaded successfully: " << path << std::endl;

	// Extract the directory from the file path
	size_t lastSlashIdx = path.find_last_of("/\\");
	if (lastSlashIdx != std::string::npos) {
		directory = path.substr(0, lastSlashIdx);
	}
	else {
		directory.clear(); // Clear directory if path doesn't contain any slashes
	}

	// Process the root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	// Process each mesh located at the node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Recursively process each child node
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene);
	}
}

// Add other necessary methods like ProcessMesh here...


Mesh Model::ProcessMesh(aiMesh* OMesh, const aiScene* Scene)
{
	std::vector<float> MeshVecrtices;
	std::vector<unsigned int> MeshIndeces;
	std::vector<Texture> MeshTextures;
	Material Matsies;
	// vertecis
	for (int v = 0; v < OMesh->mNumVertices; v++)
	{
		Vertex vertex;
		glm::vec3 vdata;
		vdata.x = OMesh->mVertices[v].x;
		MeshVecrtices.push_back(OMesh->mVertices[v].x);
		vdata.y = OMesh->mVertices[v].y;
		MeshVecrtices.push_back(OMesh->mVertices[v].y);
		vdata.z = OMesh->mVertices[v].z;
		MeshVecrtices.push_back(OMesh->mVertices[v].z);
		vertex.position = vdata;

		glm::vec3 ndata;
		ndata.x = OMesh->mNormals[v].x;
		MeshVecrtices.push_back(OMesh->mNormals[v].x);
		ndata.y = OMesh->mNormals[v].y;
		MeshVecrtices.push_back(OMesh->mNormals[v].y);
		ndata.z = OMesh->mNormals[v].z;
		MeshVecrtices.push_back(OMesh->mNormals[v].z);
		vertex.normal = ndata;

		glm::vec2 uvdata;
		if (OMesh->mTextureCoords[0]) {
			uvdata.x = OMesh->mTextureCoords[0][v].x;
			MeshVecrtices.push_back(OMesh->mTextureCoords[0][v].x);
			uvdata.y = OMesh->mTextureCoords[0][v].y;
			MeshVecrtices.push_back(OMesh->mTextureCoords[0][v].y);
		}
		else
		{
			uvdata = glm::vec2(0.0f, 0.0f);
			MeshVecrtices.push_back(0.0f);
			MeshVecrtices.push_back(0.0f);
		}
		vertex.texcoord = uvdata;

		//MeshVecrtices.push_back(vertex);

	}
	// indecis
	for (int i = 0; i < OMesh->mNumFaces; i++)
	{
		aiFace face = OMesh->mFaces[i];
		for (int num = 0; num < face.mNumIndices; num++)
		{
			MeshIndeces.push_back(face.mIndices[num]);
		}
	}
	// mats
	if (OMesh->mMaterialIndex >= 0)
	{
		aiMaterial* MeshMat = Scene->mMaterials[OMesh->mMaterialIndex];
		std::vector<Texture> DeffuseText = loadMaterialTextures(MeshMat, aiTextureType_DIFFUSE, "diffuse");
		MeshTextures.insert(MeshTextures.end(), DeffuseText.begin(), DeffuseText.end());
		std::vector<Texture> SpecularText = loadMaterialTextures(MeshMat, aiTextureType_SPECULAR, "specular");
		MeshTextures.insert(MeshTextures.end(), SpecularText.begin(), SpecularText.end());
		Matsies = loadMaterial(MeshMat);
	}
	Mesh partMesh(
		MeshVecrtices.data(),
		MeshVecrtices.size(),
		MeshIndeces.data(),
		MeshIndeces.size(),
		{ MeshTextures[0], MeshTextures[0] });
	All nall = { MeshVecrtices , MeshIndeces , MeshTextures };
	return partMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (int j = 0; j < Loaded_Textures.size(); j++)
		{
			if (Loaded_Textures[j].path == str.C_Str())
			{
				textures.push_back(Loaded_Textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			Loaded_Textures.push_back(texture);
		}
	}
	return textures;
}
unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
Material Model::loadMaterial(aiMaterial* mat) {
	Material material;
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.Diffuse = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.Ambient = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.Specular = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_SHININESS, shininess);
	material.Shininess = shininess;

	return material;
}