#ifndef PARAMETERPARSER_H
#define PARAMETERPARSER_H

#include <memory>
#include <vector>

class ParameterParser
{
public:
    ParameterParser(int argc, const char* argv[]);
    size_t size() const noexcept {return args_.size();}
    std::string at(size_t pos) const noexcept;
    size_t find(std::string param) const noexcept;

private:
    std::vector<std::string> args_;
};

using ParameterParserUPtr = std::unique_ptr<ParameterParser>;

#endif // PARAMETERPARSER_H
