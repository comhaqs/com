#ifndef PLUGIN_H__
#define PLUGIN_H__

#include <memory>
#include <string>
#include <vector>
#include <boost/asio.hpp>


typedef std::shared_ptr<std::vector<unsigned char>> frame_ptr;
typedef std::shared_ptr<boost::asio::io_context> context_ptr;
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class plugin{
public:
    virtual ~plugin();
    virtual void start_before();
    virtual void start();
    virtual void start_after();
    virtual void stop_before();
    virtual void stop();
    virtual void stop_after();
};
typedef std::shared_ptr<plugin> plugin_ptr;


class param_base{
public:
    virtual ~param_base();

    std::string id;
};
typedef std::shared_ptr<param_base> param_base_ptr;

template <typename T>
class param_one : public param_base{
public:
    T  param1;
};

template <typename T1, typename T2>
class param_two : public param_one<T1>{
public:
    T2  param2;
};

template <typename T1, typename T2, typename T3>
class param_three : public param_two<T1, T2>{
public:
    T3  param3;
};


class module_manager;
class module : public plugin{
public:
    virtual void exec(std::shared_ptr<module_manager> p_manager, param_base_ptr p_param) = 0;
};
typedef std::shared_ptr<module> module_ptr;

class module_manager{
public:
    virtual bool add_plugin(const std::string& name, plugin_ptr p_plugin) = 0;
    virtual void notify(const std::string& name, param_base_ptr p_param) = 0;
    virtual void subject(const std::string& name, module_ptr p_module) = 0;
};
typedef std::shared_ptr<module_manager> module_manager_ptr;

template <typename T>
class module_proxy : public module{
public:
    typedef std::function<void (std::shared_ptr<module_manager>, std::shared_ptr<T>)> fun_type;

    module_proxy(fun_type fun):m_fun(fun){}

    virtual void exec(std::shared_ptr<module_manager> p_manager, param_base_ptr p_param){
        auto p = std::dynamic_point_cast<T>(p_param);
        if(!p){

        }else if(!m_fun){
            exec(p_manager, p);
        }else{
            m_fun(p_manager, p);
        }
    }
    virtual void exec(std::shared_ptr<module_manager> p_manager, std::shared_ptr<T> p_param){}
protected:
    fun_type m_fun;
};


class work_base{
public:
    typedef std::shared_ptr<boost::asio::deadline_timer> timer_ptr;
    typedef std::function<void ()> fun_type;

    virtual context_ptr get_context() = 0;
    virtual void post(fun_type fun) = 0;
    virtual timer_ptr post(fun_type fun, const int64_t& duration) = 0;
    virtual void stop_timer(timer_ptr p_timer) = 0;
};

namespace soci
{
    class session;
}
class database_base{
public:
    typedef std::shared_ptr<soci::session> session_ptr;
    typedef std::function<void (session_ptr)> fun_type;

    virtual std::function<void ()> post(fun_type fun) = 0;
};




#endif // PLUGIN_H__