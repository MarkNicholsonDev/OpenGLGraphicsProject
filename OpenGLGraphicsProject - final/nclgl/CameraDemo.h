#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include <algorithm>

class CameraDemo{
public:
	CameraDemo();
	~CameraDemo();

	bool GetIsPlaying() { return isPlaying; };
	void SetIsPlaying() { isPlaying ? isPlaying = false : isPlaying = true; };
	void MoveToPosition(int posInd, Camera* camera);

	vector<Vector3>GetPositions() { return positions; }
	vector<float>GetPitch() { return pitch; }
	vector<float>GetYaw() { return yaw; }

protected:
	bool isPlaying;
	Camera* camera;
	vector<Vector3> positions = { Vector3(350.0f, 500.0f, 100.0f), Vector3(50.0f, 150.0f, 1200.0f), Vector3(1100.0f, 190.0f, 950.0f) };
	vector<float> yaw = { 210.0f, 270.0f, 210.0f };
	vector<float> pitch = {-10.0f, -10.0f, 10.0f };
};

