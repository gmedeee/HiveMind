#pragma once

class Exception
{
    const char *msg;

public:
    Exception(const char *m) : msg(m) {}
    const char *what() const
    {
        return msg;
    }
};