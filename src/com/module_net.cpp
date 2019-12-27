#include "module_net.h"
#include "utility_tool.h"


#define BUFFER_MAX 256


module_net::module_net(const int& port):m_port(port){

}

void module_net::start(){
    boost::asio::spawn(*mp_context, std::bind(&module_net::async_accept, shared_from_this(), std::placeholders::_1, mp_context));
}

void module_net::stop(){
}

void module_net::async_accept(boost::asio::yield_context yield, context_ptr p_context){
    try{
        if (0 >= m_port)
    {
        LOG_ERROR("监听端口非法:"<<m_port);
        return;
    }
    LOG_INFO("监听端口:"<<m_port);
    auto acceptor = boost::asio::ip::tcp::acceptor(*p_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(m_port)));
        boost::system::error_code ec;
        auto p_manager = mp_manager.lock();
        while(true){
            auto p_socket = std::make_shared<socket_ptr::element_type>(*p_context);
            acceptor.async_accept(*p_socket, yield[ec]);
            if(ec){
                break;
            }
            boost::asio::spawn(*p_context, std::bind(&module_net::async_read, shared_from_this(), std::placeholders::_1, p_socket, p_manager));
        }
    }catch(const std::exception& ){

    }
    
}

void module_net::async_read(boost::asio::yield_context yield, socket_ptr p_socket, module_manager_ptr p_manager){
    boost::system::error_code ec;
    while(true){
        frame_ptr p_frame(new frame_ptr::element_type(BUFFER_MAX));
        auto count = p_socket->async_read_some(boost::asio::buffer(*p_frame, BUFFER_MAX), yield[ec]);
        if(ec){
            break;
        }
        notify(p_socket, p_frame, p_manager);
    }
}

void module_net::notify(socket_ptr p_socket, frame_ptr p_frame, module_manager_ptr p_manager){
    auto p_param = std::make_shared<param_ptr::element_type>();
    p_param->param1 = p_socket;
    p_param->param2 = p_frame;
    p_manager->notify("socket_read", p_param);
}

