#include "trlpch.h"
#include "Buffer.h"
#include <glad/glad.h>
namespace Trail { 
	static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_INT;
		}
		TRL_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}
	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //we will change the implementation of
		//the usage because we might need to use a different usage later
	}
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	IndexBuffer::IndexBuffer(uint32_t* indeces, uint32_t count) : m_Count(count){
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indeces, GL_STATIC_DRAW); //we will change the implementation of
		//the usage because we might need to use a different usage later
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}
	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	VertexArray::VertexArray(){
		glCreateVertexArrays(1, &m_RendererID);
	}
	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer){
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind(); 

		TRL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "No layout provided");

		const auto& layout = vertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : layout.GetElements()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGlBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer){
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
	void VertexArray::Bind() const{
		glBindVertexArray(m_RendererID);
	}
	void VertexArray::Unbind() const{
		glBindVertexArray(0);
	}
}