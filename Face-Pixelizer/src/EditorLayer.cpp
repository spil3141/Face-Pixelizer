#include "EditorLayer.h"

#include "ImGui/imgui.h"
#include "glm/gtx/transform.hpp"

#include <thread>
#include <mutex>
#include <fstream>
#if defined(_WIN64)
	//#include <cstdlib>      // std::system
	#include "Windows.h"      // WinExec
#endif

//Scripts
#include "Scripts/CameraController.h"

//// Python Embedding Attempt
//#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h> 
//#include <stdlib.h>
//#define PY_SSIZE_T_CLEAN
//#include <Python.h>

static std::mutex s_mutex;
static void ConvertFile(std::string& command)
{
	// Method 1 : Non-blocking
	s_mutex.lock();
	WinExec(command.c_str(), SW_HIDE); // TODO: Replace with CreateProcess
	s_mutex.unlock();

	// Method 2 : Blocking
	/*if (std::system(command.c_str()) == 0)
	{
		T_TRACE("Command and Thread execution completed.");
	}else
		T_ERROR("Command execution Failed.");*/
}

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
	Application::Get().GetWindow().SetVSync(false);
}

EditorLayer::~EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	// Fonts Setup 
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("res/Fonts/OpenSans-Italic.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("res/Fonts/OpenSans-Bold.ttf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/Fonts/OpenSans-Regular.ttf", 18.0f);

	m_Default_Preview = Texture2D::Create("res/Textures/converting.png");

	Tiny::FramebufferSpecification fbSpec; fbSpec.Width = 1280; fbSpec.Height = 720;
	m_Framebuffer = Tiny::Framebuffer::Create(fbSpec);

	m_ActiveScene = CreateRef<Scene>();

	e_Camera = m_ActiveScene->CreateEntity("Camera"); e_Camera.AddComponent<CameraComponent>();
	e_Camera.GetComponent<TransformComponent>().position = glm::vec3(0.0f, 0.0f, 0.3f);
	e_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

	e_PreviewImage = m_ActiveScene->CreateEntity("PreviewImage");
	e_PreviewImage.AddComponent<SpriteRendererComponent>().Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	e_PreviewImage.GetComponent<TransformComponent>().scale = glm::vec3(6.0f, 6.0f, 0.0f);
	
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Timestep dt)
{
	// Resize when the viewport or screen gets resize
	Tiny::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		//m_OrthCamController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	Tiny::Renderer2D::ResetStats();
	m_Framebuffer->Bind();
	RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();
	// Render Editor Scene Here
	m_ActiveScene->OnUpdate(dt);
	m_Framebuffer->Unbind();
}

void EditorLayer::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::MouseScrolled && m_CanOpenNewFile && m_ViewportFocused && m_ViewportHovered)
	{
		CameraController* camscrip = (CameraController*)e_Camera.GetComponent<NativeScriptComponent>().Instance;
		camscrip->EnableControls = true;
		float zoom = camscrip->Zoom;
		MouseScrolledEvent* event = (MouseScrolledEvent*)&e;
		zoom -= event->GetYOffset() * 0.25f;
		zoom = std::max(zoom, 0.25f);
		zoom = std::clamp(zoom, 6.2f, 10.0f);
		camscrip->Zoom = zoom;
		//T_ERROR(zoom);
	}
}

void EditorLayer::OnImGuiRender()
{
	static bool b_ActivateDockspace = true;
	if (b_ActivateDockspace)
	{
		static bool* p_open = &b_ActivateDockspace;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;//dockspace_flags ^= ImGuiDockNodeFlags_NoResize;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", p_open, window_flags);
		{
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);


			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowMinSize.x = 304.0f;
			//ImGui::SetNextWindowSizeConstraints(ImVec2(100.0f, style.WindowMinSize.y), ImVec2(100.0f, 100.0f));
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open File...")) 
					{
						if (m_CanOpenNewFile)
						{
							std::string file = FileDialogs::OpenFile("image (*.png)\0*.png\0");
							if (!file.empty())
							{
								m_Command = m_python +std::string(" ") + m_PyScript + std::string(" ") + std::string("--img") +
									std::string(" ") +file + std::string(" ") + std::string("--output_dir") + std::string(" ") +
									m_output_dir;
								if (!std::filesystem::is_empty(m_output_dir))
									std::filesystem::remove(m_output_dir + std::string("/output.png"));
								m_ImageAfterOpen = Texture2D::Create(file.c_str());
								m_DisplayConvertBTN = true;
							}
						}
					} 

					ImGui::Separator();

					if (ImGui::MenuItem("Exit", NULL, false))
					{
						if (!m_CanOpenNewFile)
						{
							if(t_worker.joinable())
								t_worker.detach();
						}
						Tiny::Application::Get().ExitApplication();
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			{
				ImGui::Begin("Navigation");

				if (m_ImageAfterOpen)
				{
					uint32_t textureid = m_ImageAfterOpen->GetRendererID();
					ImGui::Image((void*)textureid, ImVec2(200.0f, 200.0f), ImVec2(1,1), ImVec2(0, 0));
				}

				ImGui::Separator();

				if (m_DisplayConvertBTN)
				{
					auto& font = io.Fonts->Fonts[1];

					ImGui::PushFont(font);
					ImGui::Dummy(ImVec2(100, 0));
					ImGui::SameLine();
					if (ImGui::Button("Pixelate") && m_Command != "")
					{
						// TODO : Find a way to close an active thread before creating a new one. 
						t_worker = std::thread(ConvertFile,m_Command);
						T_INFO("Waiting for image to convert.");
						auto& sre = e_PreviewImage.GetComponent<SpriteRendererComponent>();
						sre.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
						sre.Texture = m_Default_Preview;
						m_CanOpenNewFile = false;
					}
					ImGui::PopFont();

					if (t_worker.joinable() && !std::filesystem::is_empty(m_output_dir))
					{
						t_worker.join();
						std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TEMP SOLUTION: Find a FIX

						m_ImagePreview = Texture2D::Create(m_output_dir + std::string("/output.png"));
						auto& sre = e_PreviewImage.GetComponent<SpriteRendererComponent>();
						sre.Texture = m_ImagePreview;
						sre.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
						m_CanOpenNewFile = true;
					}
				}
				ImGui::End();

				{
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // Remove padding of viewport
					ImGui::Begin("Preview");
					m_ViewportFocused = ImGui::IsWindowFocused();
					m_ViewportHovered = ImGui::IsWindowHovered();

					//T_INFO(" Focused: {0} Hovered: {1}", m_ViewportFocused, m_ViewportHovered);

					Tiny::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

					uint32_t textureid = m_Framebuffer->GetColorAttachmentRendererID();
					ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
					m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
					ImGui::Image((void*)textureid, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::End();
					ImGui::PopStyleVar();
				}
			}

#ifdef T_DEBUG
			ImGui::Begin("Performance Check");
			{/*
				static bool show = true;
				ImGui::ShowDemoWindow(&show);*/
				auto stats = Tiny::Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				//ImGui::Separator();
				float framerate = ImGui::GetIO().Framerate;
				ImGui::Text("Framerate: %.0f fps", framerate);
				ImGui::Text("Frametime: %.3f ms", (float)(1/framerate) * 1000);

			}
			ImGui::End();
#endif
		}
		ImGui::End();


	}
}
