#include "NameGenerator.h"
#include <array>
#include <regex>
#include "CommonUtil.h"

namespace {
std::array<wchar_t, 6> vowelsEng = {L'a', L'e', L'i', L'o', L'u', L'y'};
std::array<std::wstring, 27> consonantsEng = {L"b", L"c", L"d", L"f", L"g", L"h", L"j", L"k", L"l", L"m", L"n", L"p", L"q", L"r", L"s", L"t", L"v", L"w", L"x", L"z",
                                              L"sh", L"zh", L"ch", L"kh", L"th"};
std::array<wchar_t, 8> vowelsRu = {L'а', L'е', L'и', L'о', L'у', L'э', L'ю', L'я'};
std::array<wchar_t, 20> consonantsRu = {L'б', L'в', L'г', L'д', L'ж', L'з', L'к', L'л', L'м', L'н', L'п', L'р', L'с', L'т', L'ф', L'х', L'ц',
                                        L'ч', L'ш', L'щ'};

std::array<std::wstring, 78> digraphs = {
     L"a", L"ac", L"ad", L"ar", L"as", L"at", L"ax", L"ba", L"bi", L"bo", L"ce", L"ci", L"co", L"de", L"di", L"e", L"ed", L"en", L"es", L"ex", L"fa", L"fo",
        L"ga", L"ge", L"gi", L"gu", L"ha", L"he", L"in", L"is", L"it", L"ju", L"ka", L"ky", L"la", L"le", L"le", L"lo", L"mi", L"mo", L"na", L"ne", L"ne", L"ni",
        L"no", L"o", L"ob", L"oi", L"ol", L"on", L"or", L"or", L"os", L"ou", L"pe", L"pi", L"po", L"qt", L"re", L"ro", L"sa", L"se", L"so", L"ta", L"te", L"ti",
        L"to", L"tu", L"ud", L"um", L"un", L"us", L"ut", L"va", L"ve", L"ve", L"za", L"zi"};


std::array<std::wstring, 18> trigraphs = {
     L"cla", L"clu", L"cra", L"cre", L"dre", L"dro", L"pha", L"phi", L"pho", L"sha", L"she", L"sta", L"stu", L"tha", L"the", L"thi", L"thy", L"tri"};
std::wregex pattern {LR"(.*[aeiou]{3}.*$)"};

} // namespace

NameGenerator::NameGenerator()
{
    srand(time(0));
}

std::wstring NameGenerator::generate(TranslateInterface::Language lang, bool unique) noexcept {
    return (rand() % 2) ? generate1(lang, unique) : generate2(lang, unique);
}

template<typename Ts>
using UnorderedSetInsertReturnValueType = decltype(std::declval<std::unordered_set<Ts>>().insert(std::declval<Ts>()));

std::wstring NameGenerator::generate1(TranslateInterface::Language lang, bool unique) noexcept {
    constexpr int minLength = 6;
    constexpr int maxLength = 9;

    UnorderedSetInsertReturnValueType<std::wstring> insertResult;
    std::wstring name;

    do {
        name = L"";
        bool vowelFirst = (rand() % 2);
        int length = minLength + (rand() % (maxLength - minLength + 1));

        for(int i = 0; i <= length; ++i) {
            bool even = (i%2) == 0;
            if((vowelFirst && even) || (!vowelFirst && !even)) {
                if(/*lang == TranslateInterface::Language::RUSSIAN || */lang == TranslateInterface::Language::BULGARIAN) {
                    name += vowelsRu[rand() % vowelsRu.size()];
                } else {
                    name += vowelsEng[rand() % vowelsEng.size()];
                }
            } else {
                if(/*lang == TranslateInterface::Language::RUSSIAN || */lang == TranslateInterface::Language::BULGARIAN) {
                    name += consonantsRu[rand() % consonantsRu.size()];
                } else {
                    name += consonantsEng[rand() % consonantsEng.size()];
                }
            }
        }
        /* TODO: can it work on a mobile? Which locale should be checked?
        if(lang == TranslateInterface::Language::RUSSIAN || lang == TranslateInterface::Language::BULGARIAN) {
            name[0] = std::toupper(name[0], std::locale("bg_BG.UTF-8"));
        } else {
            name[0] = std::toupper(name[0], std::locale("en_US.utf8"));
        }
        */
        name[0] = std::toupper(name[0]);
        if(unique) {
            insertResult = generatedNames_.insert(name);
        } else {
            break;
        }
    } while(!insertResult.second);
    return name;
}

std::wstring NameGenerator::generate2([[maybe_unused]]TranslateInterface::Language lang, bool unique) noexcept {
    UnorderedSetInsertReturnValueType<std::wstring> insertResult;
    std::wsmatch matches;
    std::wstring name;
    do {
        name = L"";
        int syllableCount = 2 + (rand() % 3);
        for(int i = 0; i < syllableCount; ++i) {
            name += digraphs[rand() % digraphs.size()];
        }
        size_t lastIndex = (rand() % (digraphs.size() + trigraphs.size()));
        if(lastIndex >= digraphs.size()) {
            name += trigraphs[lastIndex - digraphs.size()];
        } else {
            name += digraphs[lastIndex];
        }
        if(std::regex_search(name, matches, pattern) && matches.size() > 0) {
            std::string n(name.begin(), name.end());
            debug("Illegal name: %s", n.c_str());
            continue;
        }
        name[0] = std::toupper(name[0]);
        if(unique) {
            insertResult = generatedNames_.insert(name);
        } else {
            break;
        }
    } while(!insertResult.second);
    return name;
}
