#include "ArmPCH.h"
#include "ImGuiLayer.h"

#include"imgui.h"
#include"imgui_impl_opengl3.h"
#include"Armed/core/Application.h"

#define BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

Arm::ImGuiLayer::ImGuiLayer()
	:Layer("ImGuiLayer")
{
}

Arm::ImGuiLayer::~ImGuiLayer()
{
}

void Arm::ImGuiLayer::begin()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::get();
	io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void Arm::ImGuiLayer::end()
{
	static bool status = true;
	ImGui::ShowDemoWindow(&status);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Arm::ImGuiLayer::onAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init("#version 460");
}
void Arm::ImGuiLayer::onDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
void Arm::ImGuiLayer::onUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::get();
	io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool status = true;
	ImGui::ShowDemoWindow(&status);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Arm::ImGuiLayer::onEvent(Event& event)
{
	/*if (m_ImGuiEvent) {
		ImGuiIO& io = ImGui::GetIO();
		event.handled |= event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		event.handled |= event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}*/

	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonReleasedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
}

bool Arm::ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
{
	return false;
}

bool Arm::ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e)
{
	return false;
}
