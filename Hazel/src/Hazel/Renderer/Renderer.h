#pragma once

namespace Hazel {
	enum class RendererAPI
	{
		None = 0, OpenGL, DirectX12
	};

	class Renderer
	{
	public:

		static RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;

	};

}