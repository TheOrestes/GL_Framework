
#include "UIManager.h"
#include "glfw3.h"
#include "../MaterialSystem/Material.h"
#include "../Helpers/Helper.h"

#include <glm/gtc/type_ptr.hpp>

extern GLFWwindow* window;
//////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager()
{
	m_pWindow = nullptr;
	bUIActive = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
UIManager::~UIManager()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::InitUIManager(GLFWwindow* window)
{
	m_pWindow = window;
	// ImGui binding...
	ImGui_ImplGlfwGL3_Init(m_pWindow, true);
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::RenderMaterialUI(Material* material)
{
	static float color[4];
	Helper::glm_vec4_to_float4(material->m_color, color);
	
	if (bUIActive)
	{
		glfwSetCursorPosCallback(m_pWindow, NULL);
		
		ImGui::Begin("Material Editor");
		ImGui::ColorEdit4("Albedo", color);
		ImGui::End();
	}

	material->m_color.r = color[0];
	material->m_color.g = color[1];
	material->m_color.b = color[2];
	material->m_color.a = color[3];
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::BeginRender()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::EndRender()
{
	ImGui::Render();
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::Kill()
{
	ImGui_ImplGlfwGL3_Shutdown();
}



