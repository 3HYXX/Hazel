#include "hzpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {


	///////////////////////////////////////////////////////////
	//////////////////////VertexBuffer/////////////////////////
	///////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID); // Create a buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // Bind the buffer object to the target
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // Creates and initializes a buffer object's data store
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID); // Delete named buffer objects
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // Bind the buffer object to the target
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Bind the buffer object to the target
	}


	///////////////////////////////////////////////////////////
	//////////////////////Index Buffer/////////////////////////
	///////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID); // Create a buffer object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // Bind the buffer object to the target
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW); // Creates and initializes a buffer object's data store
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID); // Delete named buffer objects
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // Bind the buffer object to the target
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Bind the buffer object to the target
	}
}