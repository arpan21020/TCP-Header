#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    // Check if the segment is empty (no data)
    cout<<"seg.length_in_sequence_space():"<<seg.length_in_sequence_space()<<endl;

if (seg.length_in_sequence_space() == 0) {

    if (head.ack) {
        uint64_t checkpoint = _reassembler.ack_index();
        uint64_t abs_ackno = unwrap(head.ackno, _isn, checkpoint);
        
    }

    if (head.fin) {
        _finReceived = true;

    }

    return;
}



    // --- Hint ------
        // convert the seqno into absolute seqno
        
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
    // --- Hint ------  
      // Check if this is the first segment (SYN)
    cout<<"seg.payload().copy():"<<seg.payload().copy()<<endl;

   
    
    if (head.fin)
        _finReceived = true;
    if (head.syn) {
        _synReceived = true;
        _isn = head.seqno;
        _reassembler.push_substring(seg.payload().copy(), 0, head.fin);

    }
    else {
        _reassembler.push_substring(seg.payload().copy(), abs_seqno - 1, head.fin);

    }

    // ... 
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    // converting stream index to absolute seqno
    uint64_t n=_reassembler.ack_index()+1;
    WrappingInt32 isN=WrappingInt32(_isn);

    
    if(_finReceived && _reassembler.unassembled_bytes() == 0 && _synReceived){
        // converting absolute seq to seqno
        WrappingInt32 ackno=wrap(n+1,isN);
        return ackno;
    }
    else if (_synReceived){
        // converting absolute seq to seqno
        WrappingInt32 ackno=wrap(n,isN);
        return ackno;

    }
        
    else return {};
}

size_t TCPReceiver::window_size() const { 
    size_t bytes_in_buffer = _reassembler.stream_out().buffer_size();
    return _capacity - bytes_in_buffer;
    }
