
#pragma once

#include "glm/glm.hpp"
#include "AntTweakBar.h"

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
	void			Render();
private:
	UIManager();
	UIManager(const UIManager&);
	void operator=(const UIManager&);

	GLFWwindow*     m_pWindow;

	TwBar*			m_pFxUIBar;
};