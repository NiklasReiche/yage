#pragma once

#include <span>
#include <vector>

namespace gl
{
	/**
	 * @brief Interface for objects that can be rendered.
	 *
	 * Objects of this class represent a reference to a geometry
	 * buffer on the GPU containing vertex data.
	 */
	class IDrawable
	{
	public:
		virtual ~IDrawable() = default;
		
		/**
		 * @brief Overwrites the content of the referenced GPU buffer with the given data.
		 *
		 * The memory on the GPU will be reallocated with the new data. If the container is empty the memory
		 * will be invalidated instead.
		 * 
		 * @param vertices Container whose contents will be uploaded to the GPU.
		 */
		virtual void setData(const std::span<const float>& vertices) = 0;
		
		/**
		 * @brief Overwrites part of the referenced GPU buffer with new data.
		 *
		 * If the container is empty, no data will be overwritten.
		 * 
		 * @param offset The index at which to insert the new data.
		 * @param vertices The elements with which to update the buffer.
		 *
		 * @throws std::invalid_argument The offset or container size is out of bounds.
		 */
		virtual void setSubData(unsigned int offset, const std::span<const float>& vertices) = 0;

		/**
		 * @brief Reads data from the referenced GPU buffer.
		 *
		 * Size amount of elements are read beginning at the given offset.
		 * The returned container will be of the given size.
		 * If the size is zero, no data will be read and the returned container will be empty.
		 * 
		 * @param offset The index from which to read the data.
		 * @param size The amount of elements to read from the GPU.
		 * 
		 * @return A copy of the data on the GPU.
		 *
		 * @throws std::invalid_argument The offset or size is out of bounds.
		 */
		[[nodiscard]]
		virtual std::vector<float> getSubData(unsigned int offset, unsigned int size) = 0;

		/**
		 * @brief Returns whether the referenced GPU buffer is empty.
		 * 
		 * @return True if the buffer is empty, false otherwise.
		 */
		[[nodiscard]]
		virtual bool isEmpty() = 0;

	protected:
		IDrawable() = default;
		IDrawable(const IDrawable& other) = default;
		IDrawable(IDrawable&& other) = default;
		IDrawable& operator=(const IDrawable& other) = default;
		IDrawable& operator=(IDrawable && other) = default;
	};
}
