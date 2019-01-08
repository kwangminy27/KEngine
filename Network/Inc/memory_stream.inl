#pragma once

template <typename T>
void K::MemoryStream::Serialize(T& _data)
{
	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "MemoryStream::Serialize");

	uint16_t platform_endianess = 0x1234;
	uint16_t stream_endianess = htons(platform_endianess);

	if (platform_endianess == stream_endianess)
		_Serialize(&_data, sizeof(T));
	else
	{
		T data{};

		switch (_GetType())
		{
		case MEMORY_STREAM_TYPE::INPUT:
			_Serialize(&data, sizeof(T));

			_data = _ByteSwap(data);
			break;

		case MEMORY_STREAM_TYPE::OUTPUT:
			data = _ByteSwap(_data);

			_Serialize(&data, sizeof(T));
			break;
		}
	}
}

template <>
void K::MemoryStream::Serialize(std::string& _data)
{
	uint32_t size{};

	switch (_GetType())
	{
	case MEMORY_STREAM_TYPE::INPUT:
		_Serialize(&size, sizeof(uint32_t));

		_data.resize(size);
		break;

	case MEMORY_STREAM_TYPE::OUTPUT:
		size = static_cast<uint32_t>(_data.size());

		_Serialize(&size, sizeof(uint32_t));
		break;
	}

	_Serialize(_data.data(), size);
}

template <>
void K::MemoryStream::Serialize(std::wstring& _data)
{
	uint16_t platform_endianess = 0x1234;
	uint16_t stream_endianess = htons(platform_endianess);

	uint32_t size{};

	switch (_GetType())
	{
	case MEMORY_STREAM_TYPE::INPUT:
		_Serialize(&size, sizeof(uint32_t));

		if (platform_endianess != stream_endianess)
			size = _ByteSwap(size);

		_data.resize(size);
		break;

	case MEMORY_STREAM_TYPE::OUTPUT:
		size = static_cast<uint32_t>(_data.size());

		if (platform_endianess != stream_endianess)
			size = _ByteSwap(size);

		_Serialize(&size, sizeof(uint32_t));
		break;
	}

	for (auto& e : _data)
		Serialize(e);
}

template<typename T>
void K::MemoryStream::Serialize(std::vector<T>& _data_vector)
{
	uint16_t platform_endianess = 0x1234;
	uint16_t stream_endianess = htons(platform_endianess);

	uint32_t size{};

	switch (_GetType())
	{
	case MEMORY_STREAM_TYPE::INPUT:
		_Serialize(&size, sizeof(uint32_t));

		if (platform_endianess != stream_endianess)
			size = _ByteSwap(size);

		_data_vector.resize(size);
		break;

	case MEMORY_STREAM_TYPE::OUTPUT:
		size = static_cast<uint32_t>(_data_vector.size());

		if (platform_endianess != stream_endianess)
			size = _ByteSwap(size);

		_Serialize(&size, sizeof(uint32_t));
		break;
	}

	for (auto& data : _data_vector)
		Serialize(data);
}

template<typename T>
T K::MemoryStream::_ByteSwap(T _data)
{
	return ByteSwapper<T, sizeof(T)>{}.Swap(_data);
}

template<typename T>
T K::ByteSwapper<T, 1>::Swap(T _data) const
{
	return _data;
}

template<typename T>
T K::ByteSwapper<T, 2>::Swap(T _data) const
{
	T result{};

	char* src = reinterpret_cast<char*>(&_data);
	char* dest = reinterpret_cast<char*>(&result);

	dest[0] = src[1];
	dest[1] = src[0];

	return result;
}

template<typename T>
T K::ByteSwapper<T, 4>::Swap(T _data) const
{
	T result{};

	char* src = reinterpret_cast<char*>(&_data);
	char* dest = reinterpret_cast<char*>(&result);

	dest[0] = src[3];
	dest[1] = src[2];
	dest[2] = src[1];
	dest[3] = src[0];

	return result;
}

template<typename T>
T K::ByteSwapper<T, 8>::Swap(T _data) const
{
	T result{};

	char* src = reinterpret_cast<char*>(&_data);
	char* dest = reinterpret_cast<char*>(&result);

	dest[0] = src[7];
	dest[1] = src[6];
	dest[2] = src[5];
	dest[3] = src[4];
	dest[4] = src[3];
	dest[5] = src[2];
	dest[6] = src[1];
	dest[7] = src[0];

	return result;
}
