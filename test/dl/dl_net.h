#ifndef DL_NET_H__
#define DL_NET_H__


#include <module_net.h>


class dl_net : public module_net{
public:
    typedef frame_ptr::element_type::iterator iter_type;
    typedef std::shared_ptr<param_three<socket_ptr, frame_ptr, std::string>> param_ptr;
protected:
    virtual void notify(socket_ptr p_socket, frame_ptr p_frame, module_manager_ptr p_manager);
    virtual std::string get_address_from_frame(frame_ptr p_frame);
    virtual frame_ptr find_frame(frame_ptr& p_frame);
    virtual unsigned char cal_crc(const iter_type& begin, const iter_type& end);

    frame_ptr mp_frame;
};


#endif // DL_NET_H__