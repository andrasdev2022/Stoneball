#include "ParameterParser.h"
#include <iostream>

ParameterParser::ParameterParser([[maybe_unused]]int argc, [[maybe_unused]]const char* argv[])
{
    for(int i = 0; i < argc; ++i) {
        args_.push_back(argv[i]);
    }
}

std::string ParameterParser::at(size_t pos) const noexcept {
    if(pos >= args_.size()) return "";
    return args_.at(pos);
}

size_t ParameterParser::find(std::string param) const noexcept {
    size_t retV = 0;
    for(auto& item : args_) {
        if(!item.compare(param)) break;
        ++retV;
    }
    return retV;
}
