#include "wrapping_integers.hh"

#include <iostream>

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { 
    // cout<<"n:"<<n<<endl;
    // cout<<"unit32_t n:"<<uint32_t(n)<<endl;
    // cout<<"wrap value:"<<(isn + uint32_t(n))<<endl;
    cout<<"isn:"<<isn<<endl;
    cout<<"static_cast<uint32_t>(n):"<<static_cast<uint32_t>(n)<<endl;
    // converting absolute seqno to seqno
    return isn + static_cast<uint32_t>(n); }

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // converting seqno to absolute seqno
    WrappingInt32 val=wrap(checkpoint, isn);
    
    int32_t offset = n - val;

    int64_t unwrapped = checkpoint + offset;
    
    if(unwrapped<0){
        unwrapped+=(1UL<<32);
    }

    return unwrapped;
}
