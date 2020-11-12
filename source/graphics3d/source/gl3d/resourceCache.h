#pragma once

#include <map>
#include <memory>

namespace gl3d
{

	/**
	 * @brief Represents a generic resource storage container.
	 * Each element has a unique id, by which it can be queried and deleted.
	 * Elements are copied to the heap and stored as shared pointers.
	 * 
	 * @tparam T the resource type
	 */
	template <typename T>
	class ResourceCache
	{
	private:
		unsigned int counter = 0;
		std::map<unsigned int, std::shared_ptr<T>> data;

	public:
		/**
		 * @brief Adds a resource to the cache.
		 * 
		 * @param item the resource to add
		 * @return the id associated with the resource
		 */
		unsigned int add(std::shared_ptr<T> item)
		{
			counter++;
			data.insert(std::pair<unsigned int, std::shared_ptr<T>>(counter, item));
			return counter;
		}

		/**
		 * @brief Removes an element from the cache.
		 * 
		 * @param key the id of the resource to delete
		 */
		void remove(unsigned int key)
		{
			data.erase(key);
		}

		/**
		 * @brief Returns the pointer to the resource associated with the given id.
		 * 
		 * @param key the resources id
		 * @return the pointer to the resource
		 */
		std::shared_ptr<T> get(unsigned int key)
		{
			return data.at(key);
		}
	};
}