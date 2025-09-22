#pragma once

#include <vector>

#include "vert_buffer.h"

namespace Engine::Graphics {
	struct VertBufElement {
		unsigned int type;
		unsigned int count;
		unsigned int size;
		bool normalized;
	};

	class VertBufLayout {
	public:
		VertBufLayout() = default;

		explicit VertBufLayout(unsigned int capacity);

		VertBufLayout(const VertBufLayout &other) = delete;

		VertBufLayout &operator=(const VertBufLayout &other) = delete;

		VertBufLayout(VertBufLayout &&other) = delete;

		VertBufLayout &operator=(VertBufLayout &&other) = delete;

		~VertBufLayout() = default;

		[[nodiscard]] unsigned int GetSize() const;

		[[nodiscard]] unsigned int GetStride() const;

		template<typename T>
		void PushBack(unsigned int count);

		const VertBufElement &operator[](unsigned int i) const;

	private:
		std::vector<VertBufElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	class VertArr {
	public:
		explicit VertArr(const VertBuffer &buffer, const VertBufLayout &layout);

		VertArr(const VertArr &other) = delete;

		VertArr &operator=(const VertArr &other) = delete;

		VertArr(VertArr &&other) = delete;

		VertArr &operator=(VertArr &&other) = delete;

		~VertArr() = default;

		void Bind() const;

		static void Unbind();

	private:
		unsigned int m_Id = 0;
	};
} // namespace Engine::Graphics
