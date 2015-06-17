#include "utils_bytestream.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

void bytestream_init(bytestream *stream, unsigned int size)
{
	if (size)
	{
		stream->data = (byte*)mem_alloc(sizeof(byte) * size);
		memset(stream->data, 0, sizeof(byte) * size);
	}
	else
	{
		stream->data = NULL;
	}
	stream->len = size;
	stream->cursor = 0;
}

int bytestream_write(bytestream *stream, byte *data, unsigned int size)
{
	assert(stream->cursor + size <= stream->len);
	if (size > 0)
	{
		memcpy(stream->data+stream->cursor, data, size);
	}
	stream->cursor += size;
	return size;
}

int bytestream_read(bytestream *stream, byte *out, unsigned int size)
{
	assert(stream->cursor + size <= stream->len);
	memcpy(out, stream->data+stream->cursor, size);
	stream->cursor += size;
	return size;
}

void bytestream_destroy(bytestream *stream)
{
	stream->len = 0;
	stream->cursor = 0;
	if (stream->data)
	{
		mem_free(stream->data);
	}
}

byte randomByte()
{ // This is sooooooooo inefficient
	byte res = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		res = (2*res) + rand() % 2;
	}
	return res;
}

void fillWithRandom(void *dst, size_t size)
{
	uint i;
	for (i = 0; i < size; i++)
	{
		((byte*)dst)[i] = randomByte();
	}
}