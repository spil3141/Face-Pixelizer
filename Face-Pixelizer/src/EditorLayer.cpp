#include "EditorLayer.h"

#include "ImGui/imgui.h"
#include "glm/gtx/transform.hpp"

#include <thread>
#if defined(_WIN64)
	//#include <cstdlib>      // std::system
	#include "Windows.h"      // WinExec
#endif


static void ConvertFile(std::string& command)
{
	// Method 1 : Non-blocking
	WinExec(command.c_str(), SW_HIDE); // TODO: Replace with CreateProcess
	
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
	m_Default_Preview = Texture2D::Create("res/Textures/converting.png");

	Tiny::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Tiny::Framebuffer::Create(fbSpec);

	m_ActiveScene = CreateRef<Scene>();
	e_Camera = m_ActiveScene->CreateEntity("Camera");
	e_Camera.AddComponent<CameraComponent>();
	auto& cc = e_Camera.GetComponent<TransformComponent>();
	cc.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.1f));

	e_PreviewImage = m_ActiveScene->CreateEntity("PreviewImage");
	auto& pee = e_PreviewImage.AddComponent<SpriteRendererComponent>();
	pee.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	auto& pe = e_PreviewImage.GetComponent<TransformComponent>();
	pe.transform = glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 0.0f));
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
}

void EditorLayer::OnImGuiRender()
{
	static bool b_ActivateDockspace = true;
	if (b_ActivateDockspace)
	{
		static bool* p_open = &b_ActivateDockspace;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
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

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		{
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			else
			{
				//ShowDockingDisabledMessage();
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					static bool test = false;
					if (ImGui::MenuItem("Open File...")) 
					{
						std::string file = FileDialogs::OpenFile("image (*.png)\0*.png\0");
						if (!file.empty())
						{
							// TEMP : you need to have a virtual python env
							std::string command =
								m_python +
								std::string(" ") +
								m_PyScript +
								std::string(" ") +
								std::string("--img") +
								std::string(" ") +
								file +
								std::string(" ") +
								std::string("--output_dir") +
								std::string(" ") +
								m_output_dir;
							m_Command = command;
							if (!std::filesystem::is_empty(m_output_dir))
								if (std::filesystem::remove(m_output_dir + std::string("/output.png")) != 0)
									T_ERROR("FILE was not initially deleted.");
							m_ImageAfterOpen = Texture2D::Create(file.c_str());
							m_DisplayConvertBTN = true;
						}
						else
						{
							// Open file action was cancelled
						}
					} 
					//if (ImGui::MenuItem("New Project")) {} 
					/*if (ImGui::MenuItem("Open Project...")) {}
					if (ImGui::MenuItem("Save")) {}
					if (ImGui::MenuItem("Save Project As...")) {} */
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", NULL, false))
					{
						Tiny::Application::Get().ExitApplication();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			{ // Right side
				ImGui::Begin("Navigation");

				if (m_ImageAfterOpen)
				{
					uint32_t textureid = m_ImageAfterOpen->GetRendererID();
					ImGui::Image((void*)textureid, ImVec2(200.0f, 200.0f), ImVec2(1,1), ImVec2(0, 0));
				}

				ImGui::Separator();
				//if (ImGui::CollapsingHeader("Commands"))
				//{ 
				//	
				//	
				//}
				if (m_DisplayConvertBTN)
				{
					ImGui::Dummy(ImVec2(100, 0));
					ImGui::SameLine();
					if (ImGui::Button("Pixelate") && m_Command != "")
					{
						// TODO : Find a way to close an active thread before creating a new one. 
						t_worker = std::thread(ConvertFile,m_Command);
						T_INFO("Waiting for image to convert.");
						auto& sre = e_PreviewImage.GetComponent<SpriteRendererComponent>();
						sre.Texture = m_Default_Preview;
					}
					// Image conversion check 
					if (t_worker.joinable() && !std::filesystem::is_empty(m_output_dir))
					{
						t_worker.join();
						
						std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TEMP SOLUTION: Find a FIX

						m_ImagePreview = Texture2D::Create(m_output_dir + std::string("/output.png"));
						auto& sre = e_PreviewImage.GetComponent<SpriteRendererComponent>();
						sre.Texture = m_ImagePreview;
						sre.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
			//ImGui::Begin("Performance Check");
			//{
			//	/*static bool show = true;
			//	ImGui::ShowDemoWindow(&show);*/
			//	/*auto stats = Tiny::Renderer2D::GetStats();
			//	ImGui::Text("Renderer2D Stats:");
			//	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			//	ImGui::Text("Quads: %d", stats.QuadCount);
			//	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			//	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());*/
			//	//ImGui::Separator();
			//	float framerate = ImGui::GetIO().Framerate;
			//	ImGui::Text("Framerate: %5.0f fps", framerate);
			//	//float averageRenderTime = (float)(1000.0f * stats.FrameRenderTime); // nb: wont be accurate until we have gathered at least stats.FrameRenderTime().size() results
			//	//ImGui::Text("Average frame render time: %8.5f ms (%5.0f fps)", averageRenderTime, framerate);
			//}
			//ImGui::End();
		}
		ImGui::End();


	}
}
