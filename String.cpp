#include "String.hpp"
#include "Allocator.hpp"
#include "VulkanErrors.hpp"
#include "Assert.hpp"

namespace Sol {

size_t cstr_len(const char* str) {
    size_t size = 0;
    while (str[size] != '\0')
        ++size;
    return size;
}

// StringView ///////////////////
void StringView::copy_to_buf(StringBuffer *buf, size_t start, size_t end)
{
    char *ptr = buf->data;
    DEBUG_ASSERT(start < end, "StringView::copy_to_buf: start > end");
    mem_cpy(buf->data, ptr + start, end - start);
}

// StringBuffer /////////////////
StringBuffer StringBuffer::nil() {
    StringBuffer buf;
    return buf;
}
StringBuffer StringBuffer::get(const char *str) {
    StringBuffer buf;
	size_t size = cstr_len(str);
    buf.init(size);
    buf.copy_here(str, size);
    return buf;
}
StringBuffer StringBuffer::get(const char *str, Allocator *alloc_) {
    StringBuffer buf;
	size_t size = cstr_len(str);
    buf.init(size, alloc_);
    buf.copy_here(str, size);
    return buf;
}

StringBuffer StringBuffer::get(const char *str, size_t size)
{
    StringBuffer buf;
    buf.init(size);
    buf.copy_here(str, size);
    return buf;
}
StringBuffer StringBuffer::get(std::string std_str, size_t size)
{
    StringBuffer buf;
    buf.init(size);
    buf.copy_here(std_str, size);
    return buf;
}
StringBuffer StringBuffer::get(const char *str, size_t size, Allocator *alloc_)
{
    StringBuffer buf;
    buf.init(size, alloc_);
    buf.copy_here(str, size);
    return buf;
}
StringBuffer StringBuffer::get(std::string std_str, size_t size, Allocator *alloc_)
{
    StringBuffer buf;
    buf.init(size, alloc_);
    buf.copy_here(std_str, size);
    return buf;
}

void StringBuffer::init(size_t size)
{
    cap = size;
    alloc = SCRATCH;
	data = reinterpret_cast<char*>(mem_alloca(cap + 1, 1, alloc));

	len = 0;
    data[len] = '\0';
}
void StringBuffer::init(size_t size, Allocator *alloc_)
{
    cap = size;
    alloc = alloc_;
    data = reinterpret_cast<char*>(mem_alloca(size + 1, 1, alloc));

	len = 0;
    data[len] = '\0';
}
void StringBuffer::kill()
{
    if (alloc == &MemoryService::instance()->system_allocator)
        mem_free(data, alloc);
}

void StringBuffer::grow(size_t size)
{
    // +1 for null byte is not in the cap
    data = (char *)mem_realloc(size + cap + 1, len + 1, data, 1, alloc);
    cap += size;
    // Null byte should already be copied over
    // str[len] = '\0'; 
}
void StringBuffer::copy_here(const char *str, size_t size)
{
    DEBUG_ASSERT(size <= cap,  "StringBuffer::copy_here: Allocate more memory!");

    mem_cpy(data, str, size);
    len = size;
    data[len] = '\0';
}

void StringBuffer::copy_here(std::string std_str, size_t size)
{
    DEBUG_ASSERT(size <= cap, "StringBuffer::copy_here: Allocate more memory!");

    mem_cpy((void *)data, (void *)std_str.c_str(), size);
    len = size;
    data[len] = '\0';
}

void StringBuffer::push(const char *str)
{
    size_t size = cstr_len(str);

    DEBUG_ASSERT(size <= cap - len, "StringBuffer::push: Allocate more memory (size > cap - len)!")

    mem_cpy((void*)(data + len), str, size);
    len += size;
    data[len] = '\0';
}
void StringBuffer::push(std::string std_str)
{
    size_t size = std_str.length();

    DEBUG_ASSERT(size <= cap - len, "StringBuffer::push: Allocate more memory (size > cap - len)!")

    mem_cpy(data + len, std_str.c_str(), size);
    len += size;
    data[len] = '\0';
}

const char *StringBuffer::as_cstr()
{
    return (const char *)data;
}
StringView StringBuffer::view(size_t start, size_t end)
{
    StringView view;
    view.buf = this;
    view.start = start;
    view.end = end;
    return view;
}

} // namespace Sol
