#pragma once

#include "RendererAPI.h"

namespace Hazel {
	class RenderCommand
	{
	public:

		static void SetClearColor(const glm::vec4& color)
		{
			s_RenderderAPI->SetClearColor(color);
		}
		static void Clear()
		{
			s_RenderderAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderderAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RenderderAPI;
	};
}