
#pragma once

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"

struct  Material;
struct	PostFXData;

class UIManager
{
public:
	static UIManager& getInstance()
	{
		static UIManager instance;
		return instance;
	}

	~UIManager();

	void			InitUIManager(GLFWwindow* window);
	void			RenderMaterialUI(Material* material);
	void			RenderPostFxUI(PostFXData* data);
	void			RenderPropertiesUI(glm::vec3& pos, glm::vec3& rot, 
									   glm::vec3& scale, bool showBB);
	void			Kill();
	void			BeginRender();
	void			EndRender();
private:
	UIManager();
	UIManager(const UIManager&);
	void operator=(const UIManager&);

	bool			bUIActive;
	GLFWwindow*     m_pWindow;
};