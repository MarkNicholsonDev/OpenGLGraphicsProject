#include "CameraDemo.h"

CameraDemo::CameraDemo() {
	isPlaying = false;
}

CameraDemo::~CameraDemo(){
	delete camera;
}

void CameraDemo::MoveToPosition(int posInd, Camera* camera) {
	camera->SetPosition(GetPositions()[posInd]);
	camera->SetPitch(GetPitch()[posInd]);
	camera->SetYaw(GetYaw()[posInd]);
}