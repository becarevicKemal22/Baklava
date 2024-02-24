//
// Created by kemal on 2/24/2024.
//

#ifndef MATURSKI_2_ERRORMESSAGEARGUMENT_H
#define MATURSKI_2_ERRORMESSAGEARGUMENT_H

#include <variant>
#include <string>

typedef std::variant<std::wstring, std::string, const char*> ErrorMessageArgument;

#endif //MATURSKI_2_ERRORMESSAGEARGUMENT_H
