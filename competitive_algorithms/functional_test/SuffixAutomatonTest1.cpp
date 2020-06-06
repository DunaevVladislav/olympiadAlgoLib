#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/SuffixAutomaton.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::string data = "abacaba";
    cpa::SuffixAutomaton suffixAutomaton(data);
    std::cout << "Суффиксный автомат успешно построен" << std::endl;
    std::cout << "Позиция первого вхождения \"bac\": " << suffixAutomaton.find("bac") + 1 << std::endl;
    return 0;
}