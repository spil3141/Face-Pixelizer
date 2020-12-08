#pragma once
#include "Tiny.h"
using namespace Tiny;

#include <filesystem> // std::filesystem::current_path()

class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep dt) override;
	void OnEvent(Event& e) override;
	void OnImGuiRender() override;


private:
	Ref<Framebuffer> m_Framebuffer = nullptr;
	Tiny::Ref<Tiny::Scene> m_ActiveScene;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	Entity e_Camera;
	Entity e_PreviewImage;

	Ref<Texture2D> m_ImageAfterOpen;
	Ref<Texture2D> m_ImagePreview;
	Ref<Texture2D> m_Default_Preview;

	// python script loc
	std::string m_PyScript = std::filesystem::current_path().string() + std::string("/res/python/src/pixelize.py");
	std::string m_python = "vendor/python";
	std::string m_output_dir = std::filesystem::current_path().string() + std::string("/res/python/res/output");
	std::string m_Command = "";
	bool m_DisplayConvertBTN = false;

	std::thread t_worker;
};

