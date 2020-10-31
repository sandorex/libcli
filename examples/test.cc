#include <cstdlib>
#include <iostream>
#include <string>
#include <unibilium.h>

int main() {
    auto terminfo = unibi_dummy();

    terminfo = unibi_from_term("xterm-256color");
    if (terminfo == NULL)
        return 1;

    auto smcup_str = unibi_get_str(terminfo, unibi_enter_ca_mode);
    auto rmcup_str = unibi_get_str(terminfo, unibi_exit_ca_mode);

    std::cout << smcup_str;
    std::cout << "hello" << '\n';
    std::cout << rmcup_str;

    delete smcup_str;
    delete rmcup_str;
    unibi_destroy(terminfo);

    return 1;
}
