#include "../nclgl/window.h"
#include "../nclgl/CameraDemo.h"
#include "Renderer.h"
#include <iostream>

int main() {
	Window w("Project", 1280, 720, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	CameraDemo demoObj;
	Camera* cam = renderer.GetCamera();
	float yaw = 0.0f;
	double startTime = w.GetTimer()->GetTotalTimeSeconds();

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float timestep = w.GetTimer()->GetTimeDeltaSeconds();
		renderer.UpdateScene(timestep);
		renderer.RenderScene();
		renderer.SwapBuffers();

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			renderer.MoveCrystal(renderer.GetRoot());
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
			demoObj.SetIsPlaying();
		}

		if (demoObj.GetIsPlaying()) {
			if (w.GetTimer()->GetTotalTimeSeconds() - startTime < 10) {
				demoObj.MoveToPosition(0, cam);
			}
			else if (w.GetTimer()->GetTotalTimeSeconds() - startTime < 20) {
				demoObj.MoveToPosition(1, cam);
			}
			else if (w.GetTimer()->GetTotalTimeSeconds() - startTime < 30) {
				demoObj.MoveToPosition(2, cam);
			}
			else {
				startTime = w.GetTimer()->GetTotalTimeSeconds();
			}
		}
	}

	return 0;
}