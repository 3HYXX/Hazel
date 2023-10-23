#include "Application.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Log.h"

namespace Hazel {
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		KeyPressedEvent event('k', 2);
		HZ_TRACE(event);

		while (true)
		{

		}
	}
}