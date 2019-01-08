#include "Network.h"
#include "memory_stream.h"

K::MemoryStream::MemoryStream()
{
	buffer_ = std::make_shared<std::vector<uint8_t>>();
	buffer_->resize(DEFAULT_MEMORY_STREAM_SIZE);
}

void K::MemoryStream::Clear()
{
	head_ = 0;
	buffer_->clear();
}

void K::MemoryStream::Resize(size_t _size)
{
	buffer_->resize(_size);
}

uint32_t K::MemoryStream::head() const
{
	return head_;
}

std::shared_ptr<std::vector<uint8_t>> const& K::MemoryStream::buffer() const
{
	return buffer_;
}

K::MEMORY_STREAM_TYPE K::InputMemoryStream::_GetType() const
{
	return MEMORY_STREAM_TYPE::INPUT;
}

void K::InputMemoryStream::_Serialize(void* _data, uint32_t _size)
{
	_Read(_data, _size);
}

void K::InputMemoryStream::_Read(void* _data, uint32_t _size)
{
	memcpy_s(_data, _size, buffer_->data() + head_, _size);

	head_ += _size;
}

K::MEMORY_STREAM_TYPE K::OutputMemoryStream::_GetType() const
{
	return MEMORY_STREAM_TYPE::OUTPUT;
}

void K::OutputMemoryStream::_Serialize(void* _data, uint32_t _size)
{
	_Write(_data, _size);
}

void K::OutputMemoryStream::_Write(void const* _data, uint32_t _size)
{
	size_t capacity = buffer_->capacity();

	if (head_ + _size >= capacity)
		Resize(capacity * 2);

	memcpy_s(buffer_->data() + head_, _size, _data, _size);

	head_ += _size;
}
