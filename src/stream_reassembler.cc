#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity)
{
    capacity_buffer=capacity;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t data_size=data.size();
    
    bool flag=true;
    // resizing data if it exceeds the capacity
    if((index+data.size())>capacity_buffer){
            data_size=capacity_buffer-index;
            if(eof)flag=false;
    }
     used_buffer+=data_size;

    if(buffer.find(index)==buffer.end())
     pq.push(index);        // converting absolute seq to seqno

     
    //  inserting data in the buffer
     if(eof){
        buffer[index]={data,true};
     }
     else {
        buffer[index]={data,false};
     }
     for (auto i:buffer)
     {
        /* code */
        cout<<"idx:"<<i.first<<endl;
        cout<<"data:"<<i.second.first<<endl;
        cout<<"eof:"<<i.second.second<<endl;
        cout<<"--------------------------\n";
     }
     
     
     
    
    
     size_t idx=pq.top();
     // iterating over the buffer and pushing the segments to bytestream
     while(!pq.empty() && idx<=ack_index()){
             cout<<"current index:"<<idx<<endl;
             cout<<"acknowledge index:"<<ack_index()<<endl;
             cout<<"buffer[idx].size():"<<buffer[idx].first.size()<<endl;
             cout<<"buffer[idx]:"<<buffer[idx].first<<endl;
             cout<<"buffer[idx]:eof:->"<<buffer[idx].second<<endl;
        // resizing data for handling overlapping case
        const std::string data=(idx==ack_index())?buffer[idx].first:(ack_index()-idx>buffer[idx].first.size())?"":buffer[idx].first.substr(ack_index()-idx,buffer[idx].first.size());

        pq.pop();
        // Checking for eof
        if(buffer[idx].second==true){
            if(_output.remaining_capacity()>=data.size()){
                _output.end_input();
            }
        }
        // updating buffer space
        used_buffer-=buffer[idx].first.size();
        
        
        if(ack_index()-idx>=buffer[idx].first.size()){
        //  cout<<"acknowledge index:"<<ack_index()<<endl;
             buffer.erase(idx);
             idx=pq.top();
             continue;

            
        }
        buffer.erase(idx);
       _output.write(data);

        current_index=_output.bytes_written();
         idx=pq.top();

     }
     
    //  if(eof && flag) {_output.end_input();}

    
    
}

size_t StreamReassembler::unassembled_bytes() const { 
    set<char> st;
    size_t count=0;
    for(auto ele:buffer){
        for(char i: ele.second.first){
            if(st.count(i)==0){
                count++;
                st.insert(i);
            }

        }
    }
    return count; }

bool StreamReassembler::empty() const { return pq.empty(); }

size_t StreamReassembler::ack_index() const { 

    return current_index; 
    }