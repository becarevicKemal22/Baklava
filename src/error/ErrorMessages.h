//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_ERRORMESSAGES_H
#define MATURSKI_2_ERRORMESSAGES_H

#include <unordered_map>
#include <string>

#include "ErrorCode.h"

/**
 * @brief Map of error codes to error messages.
 *
 * All error codes defined in ErrorCode.h must have a corresponding error message in this map.
 * @see ErrorCode.h
 */

const std::unordered_map<int , std::wstring> ERRORMESSAGES = {
        // 1xx
        {ERROR_CANNOT_OPEN_FILE, L"Nemoguće otvoriti datoteku: {}"},

        // 2xx
        {ERROR_UNEXPECTED_CHARACTER, L"Pronađen nepoznat znak '{}' na liniji {}, pozicija {}."},

};

#endif //MATURSKI_2_ERRORMESSAGES_H
