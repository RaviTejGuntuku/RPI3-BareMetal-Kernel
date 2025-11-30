#pragma once
#include "io.h"

class Uart : public OutputStream<char>
{
public:
    Uart();
    virtual void put(char ch) override;
    virtual char get();
};