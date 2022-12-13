#include "byte_stream.hh"

#include <sstream>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):
    _capcity(capacity), _buffer(capacity), _pw(0), _pr(0), _wcnt(0), _rcnt(0), _end(false) {}

size_t ByteStream::write(const string &data) {
    size_t cnt = 0;
    while (buffer_size() < _capcity && cnt < data.size()) {
        _buffer[_pw++ % _capcity] = data[cnt++];
    }
    _wcnt += cnt;
    return cnt;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    ostringstream oss;
    for (size_t p = _pr; p < _pw && p - _pr < len; p++) {
        oss << _buffer[p % _capcity];
    }
    return oss.str();
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    read(len);
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    ostringstream oss;
    size_t p = _pr;
    for (; p < _pw && p - _pr < len; p++) {
        oss << _buffer[p % _capcity];
    }
    _rcnt += (p - _pr);
    _pr = p;
    return oss.str();
}

void ByteStream::end_input() {
    _end = true;
}

bool ByteStream::input_ended() const {
    return _end;
}

size_t ByteStream::buffer_size() const {
    return _pw - _pr;
}

bool ByteStream::buffer_empty() const {
    return buffer_size() == 0;
}

bool ByteStream::eof() const {
    return buffer_empty() && input_ended();
}

size_t ByteStream::bytes_written() const {
    return _wcnt;
}

size_t ByteStream::bytes_read() const {
    return _rcnt;
}

size_t ByteStream::remaining_capacity() const {
    return _capcity - buffer_size();
}
