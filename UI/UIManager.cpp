
#include "UIManager.h"
#include "../MaterialSystem/Material.h"
#include "../Renderables/FrameBuffer.h"
#include "../Helpers/Helper.h"
#include "../Helpers/LogManager.h"

#include "GLFW\glfw3.h"
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
	std::string msg;

	m_pWindow = window;
	// ImGui binding...
	if(ImGui_ImplGlfwGL3_Init(m_pWindow, true))
	{
		msg = "IMGUI Initialized...";
		LogManager::getInstance().WriteToConsole(LOG_INFO, msg);
	}
	else
	{
		msg = "IMGUI Initialization FAILED...!!!";
		LogManager::getInstance().WriteToConsole(LOG_ERROR, msg);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::RenderMaterialUI(Material* material)
{
	static float albedo[4];
	Helper::glm_vec4_to_float4(material->m_colAlbedo, albedo);
	
	if (bUIActive)
	{
		glfwSetCursorPosCallback(m_pWindow, NULL);
		
		ImGui::Begin("Material Editor");
		ImGui::ColorEdit4("Albedo", albedo);
		ImGui::End();
	}

	// Albedo
	material->m_colAlbedo.r = albedo[0];
	material->m_colAlbedo.g = albedo[1];
	material->m_colAlbedo.b = albedo[2];
	material->m_colAlbedo.a = albedo[3];
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIManager::RenderPostFxUI( PostFXData* data )
{
	if (bUIActive)
	{
		glfwSetCursorPosCallback(m_pWindow, NULL);
		ImGui::Begin("PostProcess Effects");
		ImGui::Checkbox("Bloom On", &(data->m_bBloomOn));
		ImGui::SliderFloat("Exposure", &(data->m_fExposure), 0, 10);
		ImGui::SliderInt("Bloom Spread", &(data->m_iBlurIter), 1, 10);
		ImGui::End();
	}
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





