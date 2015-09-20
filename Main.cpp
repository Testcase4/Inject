#include "Configurator.hpp"
#include <iostream>

struct  Service1;
struct  Service2;
struct  Service3;

struct  Service1
{
    Service1(Service2*, Service3*)
    {
        std::cout << "Service1" << std::endl;
    }
};

std::unique_ptr<Service1> Service1Factory(Service3* s3, Service2* s2)
{
    return std::unique_ptr<Service1>(new Service1(s2, s3));
}

struct  Service2
{
    Service2(const Service3*)
    {
        std::cout << "Service2" << std::endl;
    }
};

std::unique_ptr<Service2> Service2Factory(const Service3* s3)
{
    return std::unique_ptr<Service2>(new Service2(s3));
}

struct  Service3
{
    Service3()
    {
        std::cout << "Service3" << std::endl;
    }
};

std::unique_ptr<Service3> Service3Factory()
{
    return std::unique_ptr<Service3>(new Service3());
}

int main(int, char**)
{
    Injection::Configurator cfg;
    cfg.AddFactory(Service1Factory);
    cfg.AddFactory(Service2Factory);
    cfg.AddFactory(Service3Factory);

    auto injector = cfg.CreateInjector();
    Service1* svc = injector.Get<Service1>();
}

