#pragma once
#include "imgui.h"
#include "GLFW/glfw3.h"

class IAMAGUI
{
public :
	IAMAGUI(GLFWwindow *window);
	void DockingSpaceFunc(bool* DockingSpaceEnable);
	bool DockingSpaceEnable = true;
private:
	
};