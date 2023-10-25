#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowsClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//To be defined in Client
	Application* CreateApplication();
}



