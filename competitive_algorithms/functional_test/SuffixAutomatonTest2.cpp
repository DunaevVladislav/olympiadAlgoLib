#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/SuffixAutomaton.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::string data = "ababb";
    cpa::SuffixAutomaton suffixAutomaton(data);
    std::cout << "Суффиксный автомат успешно построен" << std::endl;
    std::cout << "Позиция всех вхождения \"ab\": " << std::endl;
    for (auto i : suffixAutomaton.findAll("ab")) {
        std::cout << i + 1 << std::endl;
    }
    return 0;
}
