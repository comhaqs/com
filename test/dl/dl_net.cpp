#include "dl_net.h"
#include <utility_tool.h>
#include <boost/format.hpp>

#define FRAME_LENGTH_MIN 14

void dl_net::notify(socket_ptr p_socket, frame_ptr p_frame, module_manager_ptr p_manager){
    if(!mp_frame){
        mp_frame = p_frame;
    }
    while (true)
    {
        auto p = find_frame(mp_frame);
        if(!p){
            break;
        }
        auto p_param = std::make_shared<param_ptr::element_type>();
        p_param->param1 = p_socket;
        p_param->param2 = p;
        p_param->param3 = get_address_from_frame(p);
        p_manager->notify("socket_read_dl", p_param);
    }
}

std::string dl_net::get_address_from_frame(frame_ptr p_frame){
    if(!p_frame || FRAME_LENGTH_MIN > p_frame->size()){
        LOG_ERROR("数据帧非法，无法获取终端地址");
        return "";
    }
    return (boost::format("%02X%02X%02X%02X") % static_cast<int>(p_frame->at(8)) % static_cast<int>(p_frame->at(7))
            % static_cast<int>(p_frame->at(10)) % static_cast<int>(p_frame->at(9))).str();
}

frame_ptr dl_net::find_frame(frame_ptr& p_frame){
    if(!p_frame || FRAME_LENGTH_MIN > p_frame->size()){
        return frame_ptr();
    }
    for(unsigned int i = 0; i <= p_frame->size() - FRAME_LENGTH_MIN; ++i){
        if(0x68 == p_frame->at(i) && 0x68 == p_frame->at(i + 5)){
            unsigned int len1 = ((static_cast<unsigned int>(p_frame->at(i + 1))>>2) | (static_cast<unsigned int>(p_frame->at(i + 2)<<6)));
            unsigned int len2 = ((static_cast<unsigned int>(p_frame->at(i + 3))>>2) | (static_cast<unsigned int>(p_frame->at(i + 4)<<6)));
            if(len1 != len2 || (len1 + 8 > p_frame->size() - i)){
                continue;
            }
            if(0x16 != p_frame->at(i + 6 + len1 + 1)
                    || p_frame->at(i + 6 + len1) != cal_crc(p_frame->begin() + i + 6, p_frame->begin() + i + 6 + len1)){
                continue;
            }
            if(0 == i && p_frame->size() == i + 6 + len1 + 2){
                frame_ptr p_frame_new;
                p_frame_new.swap(p_frame);
                return p_frame_new;
            }else{
                auto p_frame_new = frame_ptr(new frame_ptr::element_type(p_frame->begin() + i, p_frame->begin() + i + 6 + len1 + 2));
                p_frame->erase(p_frame->begin(), p_frame->begin() + i + 6 + len1 + 2);
                return p_frame_new;
            }
        }
    }
    return frame_ptr();
}

unsigned char dl_net::cal_crc(const iter_type& begin, const iter_type& end){

}