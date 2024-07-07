#include "byte_stream.hh"

#include <algorithm>
#include<bits/stdc++.h>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa)
{
  capacity= capa;
  cout<<"============================"<<endl;
}

size_t ByteStream::write(const string &data) {
  // Get the size of the input data (length of the string).
  int data_size = data.size();

  // Check if the data size is greater than or equal to the current capacity.
  if ((data_size+written) >= capacity) {
    // If it is, limit the data size to the current capacity.
    data_size = capacity-written;
  }

  // Loop through each character in the input data.
  for (int i = 0; i < data_size; i++) {
    // Append each character to the end of the 'buffer' vector.
    buffer.push_back(data[i]);
  }

  // Decrease the capacity by the amount of data written.
  // capacity = capacity - data_size;
  cout<<"current written value:"<<written<<endl;
  cout<<"data size:"<<data_size<<" data:"<<data<<endl;
  // Increase the 'written' count by the amount of data written.
  written += data_size;
  cout<<"Bytes written to stream:"<<written<<endl;

  // Return the actual number of bytes written, which is 'data_size'.
  return data_size;
}


//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // Create an empty string to store the output.
    string output;

    // Check if the specified length 'len' is less than or equal to the size of the buffer.
    if (len <= buffer.size()) {
        // If it is, copy the first 'len' characters from the buffer into the 'output' string.
        output.assign(buffer.begin(), buffer.begin() + len);
    }

    // Return the 'output' string, which contains the peeked data.
    return output;
}


//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
   // Check if the specified length 'len' is less than or equal to the current buffer size.
   if (len <= buffer_size()) {
      // Create a variable 'n' to keep track of the number of characters to pop.
      int n = len;

      // Loop 'n' times to pop 'n' characters from the front of the buffer.
      while (n--) {
         buffer.pop_front();
      }

      // Increase the 'readed' count by the number of characters popped.
      readed += len;

      // Increase the available capacity by the number of characters popped.
      capacity += len;
   } else {
      // If 'len' is greater than the buffer size, set an error 
      set_error();
   }
}


//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  // Check if the specified length 'len' is less than or equal to the current buffer size.
  if (len <= buffer_size()) {
    // Call 'peek_output' to retrieve the first 'len' characters from the buffer.
    std::string reading = peek_output(len);
    
    // Call 'pop_output' to remove the same 'len' characters from the buffer.
    pop_output(len);
    
    // Return the 'reading', which contains the data that was read.
    return reading;
  } else {
    // If 'len' is greater than the buffer size, set an error condition .
    set_error();
  }
}


void ByteStream::end_input() {endinput=true;}

bool ByteStream::input_ended() const { return endinput;}


size_t ByteStream::buffer_size() const {return buffer.size(); }

bool ByteStream::buffer_empty() const {return buffer.size()==0; }

bool ByteStream::eof() const { return (buffer_empty() && input_ended()); }

size_t ByteStream::bytes_written() const { 
  cout<<"Bytes currently in the stream:"<<written<<endl;
  return written; }

size_t ByteStream::bytes_read() const { return readed;}

size_t ByteStream::remaining_capacity() const { return capacity-written; }
