#include "hzpch.h"

#include "Application.h"

#include "Hazel/Input.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	

	Application::Application()
		: m_Camera(-1.6f, 1.5f, -0.9f, 0.9f)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application Already Exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,1.0f,
			 0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,1.0f,
			 0.0f, 0.5f,0.0f, 0.0f,1.0f,1.0f,1.0f
		};
	
		std::shared_ptr<VertexBuffer>m_VertexBuffer = nullptr;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		
		BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f,-0.75f,0.0f,
			 0.75f,-0.75f,0.0f,
			 0.75f, 0.75f,0.0f,
			 -0.75f, 0.75f,0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB = nullptr;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareVBLayout = {
		{ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0,1,2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB = nullptr;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
		
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

	
			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			void main()
			{
				color = vec4(1.0,0.0,0.5, 1.0);
			}
		)";

		m_BlueShader = std::make_unique<Shader>(blueVertexSrc, blueFragmentSrc);

	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowsClose));

		//HZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
 			if(e.m_Handle)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
			RenderCommand::Clear();

			//m_Camera.SetPosition({ 0.5f,0.5f,0.0f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_BlueShader, m_SquareVA);

			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			//auto[x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0},{1}", x, y);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowsClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	
}