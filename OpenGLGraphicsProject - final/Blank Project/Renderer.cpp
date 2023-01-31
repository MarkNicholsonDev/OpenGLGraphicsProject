#include "Renderer.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/MeshAnimation.h"
#include <algorithm> //For usage of std::sort

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	heightMap = new HeightMap(TEXTUREDIR"heightmapnew.png", Vector2(1 / 16.0f, 1 / 16.0f));
	quad = Mesh::GenerateQuad();
	testCube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	crystalImp = Mesh::LoadFromMeshFile("Crystal2.msh");
	
	treesMesh = Mesh::LoadFromMeshFile("Trees.msh");
	//material = new MeshMaterial("Trees.mat");

	//Texture importing for the tree meshes unfinished:
	/*for (int i = 0; i < treesMesh->GetSubMeshCount(); ++i) {
		for (int x = 0; x < treesMesh->GetSubMesh(i)->GetSubMeshCount(); ++x) {
			const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
			const string* filename = nullptr;
			matEntry->GetEntry("Diffuse", &filename);
			string path = TEXTUREDIR + *filename;
			GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
			matTextures.emplace_back(texID);
		}
	}*/

	camera = new Camera(0, 0, Vector3());
	Vector3 heightmapSize = heightMap->GetHeightmapSize();
	camera->SetPosition(heightmapSize * Vector3(0.0, 0.22, 0.43));
	camera->SetYaw(277.0f);
	bool startdemo = false;
	
	lights = new Light[2];
	lights[0] = Light(heightmapSize * Vector3(0.25f, 2.0f, 0.5f), Vector4(1, 1, 1, 0.75), heightmapSize.x * 0.5f);
	lights[1] = Light(heightMap->GetHeightmapSize() * Vector3(0.69f, 0.8f, 0.5f), Vector4(1, 0, 0, 1), 500.0f);

	glGenBuffers(2, lightBuffer);

	shader = new Shader("ProjectVertex.glsl", "ProjectFragment.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");

	cubeMap =  SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg",
		TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	terrainTex = SOIL_load_OGL_texture(TEXTUREDIR"grass2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainTexBump = SOIL_load_OGL_texture(TEXTUREDIR"grass2_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	terrainTex2 = SOIL_load_OGL_texture(TEXTUREDIR"grassyrocks.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	terrainTex2Bump = SOIL_load_OGL_texture(TEXTUREDIR"grassyrocks_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	terrainTex3 = SOIL_load_OGL_texture(TEXTUREDIR"rocks.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	terrainTex3Bump = SOIL_load_OGL_texture(TEXTUREDIR"rocks_normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

#pragma region GenerateMipmaps
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTex2);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrainTex3);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrainTexBump);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrainTex2Bump);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, terrainTex3Bump);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, blendMap);
	glGenerateMipmap(GL_TEXTURE_2D);
#pragma endregion

	blendMap = SOIL_load_OGL_texture(TEXTUREDIR"blendmapnew.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	waterTexBump = SOIL_load_OGL_texture(TEXTUREDIR"waterbump.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	crystalTex = SOIL_load_OGL_texture(TEXTUREDIR"crystal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	gradientTex = SOIL_load_OGL_texture(TEXTUREDIR"gradient.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!cubeMap || !waterTex || !waterTexBump || !crystalTex || !gradientTex || !terrainTex || !terrainTexBump || !terrainTex2 || !terrainTex2Bump
		|| !terrainTex3 || !terrainTex3Bump || !blendMap) { // add the other textures etc here please
		return;
	}

#pragma region TextureRepeating
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(waterTexBump, true);
	SetTextureRepeating(terrainTex, true);
	SetTextureRepeating(terrainTexBump, true);
	SetTextureRepeating(terrainTex2, true);
	SetTextureRepeating(terrainTex2Bump, true);
	SetTextureRepeating(terrainTex3, true);
	SetTextureRepeating(terrainTex3Bump, true);
	SetTextureRepeating(crystalTex, true);
	SetTextureRepeating(gradientTex, true);
#pragma endregion

	if (!shader->LoadSuccess() || !skyboxShader->LoadSuccess()) { return; }

	root = new SceneNode();

	SceneNode* terrain = new SceneNode();
	terrain->SetMesh(heightMap);
	terrain->SetBoundingRadius(10000.0f);
	root->AddChild(terrain);

	vector<Vector3> positions = {Vector3(0.18f, 0.14f, 0.3f), Vector3(0.39f, 0.14f, 0.37f), Vector3(0.42f, 0.14f, 0.5f), Vector3(0.31f, 0.14f, 0.46f),
		Vector3(0.37f, 0.14f, 0.48f), Vector3(0.48f, 0.14f, 0.48f), Vector3(0.7f, 0.14f, 0.25f), Vector3(0.75f, 0.14f, 0.24f), Vector3(0.65f, 0.14f, 0.25f), Vector3(0.78f, 0.14f, 0.35f) };
	for (int i = 0; i < positions.size(); ++i) {
		SceneNode* trees = new SceneNode();
		trees->SetMesh(treesMesh);
		trees->SetModelScale(Vector3(4.0f, 4.0f, 4.0f));
		trees->SetTransform(Matrix4::Translation(heightMap->GetHeightmapSize()* positions[i]));
		trees->SetBoundingRadius(100.0f);
		root->AddChild(trees);
	}

	SceneNode* crystal = new SceneNode();
	crystal->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
	crystal->SetModelScale(Vector3(400.0f, 400.0f, 400.0f));
	crystal->SetTransform(Matrix4::Translation(heightMap->GetHeightmapSize()* Vector3(0.69f, 0.6f, 0.5f)));
	crystal->SetMesh(Mesh::LoadFromMeshFile("Crystal2.msh"));
	crystal->AddShader(new Shader("CrystalVertex.glsl", "CrystalFragment.glsl"));
	crystal->SetTexture(crystalTex);
	crystal->SetBoundingRadius(10000.0f);
	root->AddChild(crystal);


	SceneNode* water = new SceneNode();
	water->SetMesh(quad);
	water->SetTransform(Matrix4::Translation(heightMap->GetHeightmapSize() * Vector3(0.5f, 0.1f, 0.5f)) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
	water->SetModelScale(Vector3(1800.0f, 1800.0f, 1800.0f));
	water->SetBoundingRadius(1000000.0f);
	water->SetTexture(waterTex);
	water->AddShader(new Shader("reflectvertex.glsl", "reflectFragment.glsl"));
	root->AddChild(water);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;
}

Renderer::~Renderer(void) {
	delete root;
	delete camera;
	delete shader;
	delete skyboxShader;
	delete quad;
	delete testCube;
	delete crystalImp;
	glDeleteTextures(0, &terrainTex);
	glDeleteTextures(1, &terrainTex2);
	glDeleteTextures(2, &terrainTex3);
	glDeleteTextures(3, &terrainTexBump);
	glDeleteTextures(4, &terrainTex2Bump);
	glDeleteTextures(5, &terrainTex3Bump);
	glDeleteTextures(6, &blendMap);
	glDeleteTextures(7, &cubeMap);
	glDeleteTextures(8, &texture);
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	waterRotate += dt * 1.0f;
	waterCycle += dt * 0.20f;

	root->Update(dt);
}

void Renderer::MoveCrystal(SceneNode* n) {
	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) { //Goes through scene nodes which are children of root
		if ((*i)->GetTexture() == crystalTex) { (*i)->SetTransform((*i)->GetWorldTransform() * Matrix4::Scale(Vector3(1.0, 1.0002, 1.0))); }
	}
}

void Renderer::BuildNodeLists(SceneNode* from) { //Creates the two node lists, Transparent and Opaque nodes depending
	if (frameFrustum.InsideFrustum(*from)) { // on the alpha value assuming they are inside the frame frustum
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists() { //Sorts the two node lists, Transparent(uses reverse iterators) and Opaque nodes
	std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {
		DrawNode(i);
	}

	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		textureMatrix.ToIdentity();

		Shader* shaderCheck = n->GetShader();
		if (shaderCheck != NULL) {
			if (!shaderCheck->LoadSuccess()) { return; }
			BindShader(shaderCheck);
			SetShaderLight(*lights);
			//Application of tree material onto tree meshes:
			/*if (n->GetMesh() == treesMesh) {
				for (int i = 0; i < treesMesh->GetSubMeshCount(); ++i) {
					glActiveTexture(GL_TEXTURE12);
					glBindTexture(GL_TEXTURE_2D, matTextures[i]);
					treesMesh->DrawSubMesh(i);
				}
				return;
			}*/
			if (n->GetTexture() == waterTex) {
				Vector3 h = heightMap->GetHeightmapSize();
				modelMatrix = Matrix4::Translation(h * 0.5f) * Matrix4::Scale(h * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));
				textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) *
								Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 11);
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, waterTex);
			}
			if (n->GetTexture() == crystalTex) {
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "gradientTex"), gradientTex);
				glActiveTexture(GL_TEXTURE9);
				glBindTexture(GL_TEXTURE_2D, gradientTex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 11);
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, waterTex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			UpdateShaderMatrices();
		}
		else { BindShader(shader); }

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& n->GetColour());

		if (n->GetTexture() && n->GetTexture() != waterTex) {
			texture = n->GetTexture();
			glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), texture);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 7);
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
		n->Draw(*this);
	}
}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	BindShader(skyboxShader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	UpdateShaderMatrices();
	quad->Draw();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	BindShader(shader);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTexs[0]"), 0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTexs[1]"), 1);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTexs[2]"), 2);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTexs[0]"), 3);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTexs[1]"), 4);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTexs[2]"), 5);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "blendmap"), 6);

#pragma region Texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrainTex3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrainTexBump);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrainTex2Bump);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, terrainTex3Bump);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, blendMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#pragma endregion

	UpdateShaderMatrices();

	int binding = 1;
	glBindBuffer(GL_UNIFORM_BUFFER, *lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 2, lights, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, *lightBuffer);
	int UBO = glGetUniformBlockIndex(shader->GetProgram(), "Lights");
	glUniformBlockBinding(shader->GetProgram(), UBO, binding);

	DrawNodes();
	ClearNodeLists();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}
