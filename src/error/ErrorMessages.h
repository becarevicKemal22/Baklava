//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_ERRORMESSAGES_H
#define BAKLAVA_ERRORMESSAGES_H

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
        {ERROR_CANNOT_OPEN_FILE,             L"Nemoguće otvoriti datoteku: {}"},

        // 2xx
        {ERROR_UNEXPECTED_CHARACTER,         L"Pronađen nepoznat znak '{}' na liniji {}, pozicija {}."},
        {ERROR_UNTERMINATED_STRING,          L"Neterminiran string pronađen na liniji {}, pozicija {}."},

        // 3xx
        {ERROR_EXPECTED_X_BEFORE_Y,          L"Očekivano '{}' prije '{}'."},
        {ERROR_UNINITIALIZED_CONST,          L"Konstanta '{}' mora biti inicijalizirana."},
        {ERROR_EXPECTED_X_BEFORE_Y,          L"Očekivano '{}' poslije '{}'."},
        {ERROR_VARIABLE_REDECLARATION,       L"Varijabla '{}' je već deklarisana."},
        {ERROR_UNDECLARED_VARIABLE,          L"Nepoznata varijabla '{}'."},
        {ERROR_CONST_REASSIGNMENT,           L"Nedozvoljeno dodjeljivanje vrijednosti konstanti '{}'."},
        {ERROR_EXPECTED_EXPRESSION_AT_START, L"Očekivan izraz prije '{}'."},
        {ERROR_INVALID_LVALUE,               L"Nedozvoljena l-vrijednost '{}'."},
        {ERROR_INVALID_CALL,                 L"Nedozvoljen poziv tipa '{}'. Moguće je pozivati samo funkcije."},
        {ERROR_TOO_MANY_ARGUMENTS,           L"Nedozvoljen broj argumenata. Očekivano {}, proslijeđeno {}."},
        {ERROR_TOO_FEW_ARGUMENTS,           L"Nedozvoljen broj argumenata. Nedostaje '{}' obaveznih argumenata."},
        {ERROR_INVALID_RETURN_POSITION,      L"Izjava 'vrati' ne smije se nalaziti izvan tijela funkcije."},
        {ERROR_SELF_REFERENCING_INITIALIZER, L"Varijabla '{}' ne može se inicijalizirati sama sobom."},

        // 4xx
        {ERROR_WRONG_TYPE_OPERAND,           L"Tip '{}' nije dozvoljen za operator '{}'."},
        {ERROR_WRONG_TYPE_OPERAND_BINARY,    L"Tipovi '{}' i '{}' nisu dozvoljeni za operator '{}'."},
        {ERROR_INDEX_OUT_OF_BOUNDS,          L"Indeks '{}' je izvan granica niza."},
        {ERROR_NON_INTEGER_INDEX,            L"Indeksiranje brojem '{}' nije moguće. Indeks mora biti cijeli broj."},
        {ERROR_INDEXING_NON_ARRAY,           L"Tip '{}' nije dozvoljen za operator '[]'. Indeksiranje je moguće vršiti samo nad nizovima."},
};

#endif //BAKLAVA_ERRORMESSAGES_H
