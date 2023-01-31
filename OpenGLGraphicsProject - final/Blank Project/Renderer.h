#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"

class Camera;
class SceneNode;
class Mesh;
class Shader;

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	SceneNode* GetRoot() { return root; }

	void CameraDemo(){
		demoStart ? demoStart = false : demoStart = true; 
	}
	bool GetDemoStart() { return demoStart; }
	Camera* GetCamera() { return camera; }

	void MoveCrystal(SceneNode*);
	void UpdateScene(float msec) override;
	void RenderScene() override;

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	SceneNode* root;
	Camera* camera;
	HeightMap* heightMap;
	bool demoStart;

	Shader* shader;
	Shader* skyboxShader;

	GLuint terrainTex;
	GLuint terrainTexBump;
	GLuint terrainTexHeight;
	GLuint terrainTex2;
	GLuint terrainTex2Bump;
	GLuint terrainTex2Height;
	GLuint terrainTex3;
	GLuint terrainTex3Bump;
	GLuint terrainTex3Height;
	GLuint waterTex;
	GLuint waterTexBump;
	GLuint crystalTex;
	GLuint gradientTex;
	GLuint texture;
	GLuint cubeMap;
	GLuint blendMap;
	
	Mesh* quad;
	Mesh* crystalImp;
	Mesh* testCube;
	Mesh* treesMesh;

	MeshMaterial* material;
	vector<GLuint> matTextures;

	float waterRotate;
	float waterCycle;

	Light* lights;
	GLuint lightBuffer[2];

	Frustum frameFrustum;
	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

