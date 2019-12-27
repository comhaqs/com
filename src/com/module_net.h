#ifndef MODULE_NET__
#define MODULE_NET__


#include "plugin.h"
#include <boost/asio/spawn.hpp>

class module_net : public module, std::enable_shared_from_this<module_net>{
public:
    typedef std::shared_ptr<param_two<socket_ptr, frame_ptr>> param_ptr;

    module_net(const int& port);
    virtual void start();
    virtual void stop();
    virtual void exec(module_manager_ptr p_manager, param_ptr p_param);

protected:
    virtual void notify(socket_ptr p_socket, frame_ptr p_frame, module_manager_ptr p_manager);
    virtual void async_accept(boost::asio::yield_context yield, context_ptr p_context);
    virtual void async_read(boost::asio::yield_context yield, socket_ptr p_socket, module_manager_ptr p_manager);

    int m_port = 0;
    context_ptr mp_context;
    std::weak_ptr<module_manager> mp_manager;
};
typedef std::shared_ptr<module_net> module_net_ptr;









#endif // MODULE_NET__