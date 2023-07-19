// obj.h

#ifndef CROSS_CLIENT
#define CROSS_CLIENT

class Session
{

};

class Object
{
    public:
        Session connect_to_server(const char* ip, int port);
        

};

#endif