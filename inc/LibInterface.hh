#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH

#include "AbstractInterp4Command.hh"

class LibInterface {

public:
    LibInterface() = default;
    ~LibInterface() = default;

private:
    void* _LibHandler = nullptr;
    std::string _CmdName;
    AbstractInterp4Command* (*_pCreateCmd)(void) = nullptr;
};
#endif
