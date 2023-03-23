#pragma once

# include "method.hpp"

class Get : public Method
{
    public:
        Get(Request request, Server server);
};