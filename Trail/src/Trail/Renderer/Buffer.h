#pragma once

namespace Trail {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	static uint32_t ShaderDataTypeSize(ShaderDataType t) {
		switch (t) {
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		}

		TRL_CORE_ASSERT(false, "Unknown shader data type (not in shader data type enum class)");
		return 0;
	}
	struct BufferElementAttribute {
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;
		BufferElementAttribute(){}
		BufferElementAttribute( ShaderDataType type ,const std::string& name, const bool normalized = false) : Name(name), Offset(0), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const {
			switch (Type) {
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3: return 3*3;
			case ShaderDataType::Mat4: return 4*4;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Bool: return 1;
			}
		}
	};
	class BufferLayout {
	public:
		BufferLayout(){}
		BufferLayout(const std::initializer_list<BufferElementAttribute>& elements) : m_Elements(elements)
		{
			CalculateOffsetAndSize();
		}
			
		inline uint32_t GetStride() const {
			return m_Stride;
		}
		inline const std::vector<BufferElementAttribute>& GetElements() const{
			return m_Elements;
		}
	private:
		void CalculateOffsetAndSize(){
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElementAttribute> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer{
	public:	
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };
		const BufferLayout& GetLayout() { return m_Layout; };
		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* vertices, uint32_t count);
		~IndexBuffer();

		inline unsigned int GetCount() const {
			return m_Count;
		}

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		void Bind() const;
		void Unbind() const;

		inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const {
			return m_VertexBuffers;
		};
		inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const {
			return m_IndexBuffer;
		};

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}