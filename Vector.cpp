#include <iostream>
//custom ADT vector/dynamic array
template<typename T>
class Vector
{
private:
	//member data
	T* m_Data = nullptr;
	size_t m_Size = 0;
	size_t m_Capacity = 0;

public:
	//default constructor with some amount of memory
	Vector()
	{
		ReAlloc(3);
	};
private:
	//handles allocation for of necceseary memory and 
	void ReAlloc(size_t newCapacity)
	{
		T* newBlock = new T[newCapacity];

		if (newCapacity < m_Size)
		{
			m_Size = newCapacity;
		}

		//we can move data from old block to the new as we will be deleting our old block anyways thus preventing unneceaseary copying
		for (int i = 0; i < m_Size; i++)
		{
			newBlock[i] = std::move(m_Data[i]);
		}

		delete[] m_Data;
		m_Data = newBlock;
		m_Capacity = newCapacity;
	}

public:

	//coustructs our vector and its data from other preexisting vector and its data
	Vector(const Vector& other)
	{
		int stop = m_Size < other.size() ? m_Size : other.size();
		for (int i = 0; i < stop; ++i)
		{
			m_Data[i] = other[i];
		}
	}

	//assigns other existing vectors data in to our vector
	Vector& operator=(Vector other)
	{
		if (this != &other)
		{
			int stop = m_Size < other.size() ? m_Size : other.size();
			for (int i = 0; i < stop; ++i)
			{
				m_Data[i] = other[i];
			}
		}

		return *this;
	}

	//gets size of our vector
	int size() { return m_Size; }

	//pushis back element if it doesnt already contain it
	void PushBackUnique(const T& value)
	{
		for (size_t i = 0; i < m_Size; ++i)
		{
			if (value == m_Data[i]) { return; }
		}

		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		}
		m_Data[m_Size] = value;
		++m_Size;
	}

	//just pushes back an element
	void PushBack(const T& value)
	{
		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		}
		m_Data[m_Size] = value;
		++m_Size;
	}

	//uses move to pushback element
	void PushBack(T&& value)
	{
		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		}
		m_Data[m_Size] = std::move(value);
		++m_Size;
	}

	//variatic templates, used to construct an element in place
	template<typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		}

		m_Data[m_Size] = T(std::forward<Args>(args)...);
		return m_Data[m_Size++];
	}

	//checks if element is already in our vector or not
	bool contais(const T& value)
	{
		for (size_t i = 0; i < m_Size; ++i)
		{
			if (value == m_Data[i]) { return false; }
		}

		return true;
	}

	//reserves additional specified amount of memory
	void reserve(size_t size)
	{
		ReAlloc(m_Size + size);
	}

	//index operator so we can access our data trough indexes
	T& operator[](int index) { return m_Data[index]; }

	//returns pointer to our data
	T* Data() { return m_Data; }

	//ensures deleting memory when vector gets out of scope
	~Vector()
	{
		delete[] m_Data;
	}
};