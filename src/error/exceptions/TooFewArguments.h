//
// Created by kemal on 9/17/2024.
//

#ifndef BAKLAVA_TOOFEWARGUMENTS_H
#define BAKLAVA_TOOFEWARGUMENTS_H

class TooFewArguments : public RuntimeError {
public:
    TooFewArguments(int expected, int got, Token *token, Token *paren) : RuntimeError(ERROR_TOO_FEW_ARGUMENTS),
                                                                          token(token), paren(paren) {
        messageArguments.emplace_back(std::to_wstring(expected - got));
    }

    Token *token;
    Token *paren;
};

#endif //BAKLAVA_TOOFEWARGUMENTS_H
