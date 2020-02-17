#ifndef ADTS_PARSER_H
#define ADTS_PARSER_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "common.h"

class ADTSParser {

    enum State{
        None,
        Data,
        ADTS_SECOND_BYTE,
        ADTS_HEADER_SIZE,
        ADTS_LENGTH_SIZE
    };

    OnDataMethodPtrT OnWriteData;
    std::vector<uint8_t> buffer;
    State adtsState;
    uint32_t frameLength;

    void putByte(uint8_t byte);

public:

    ADTSParser(const OnDataMethodPtrT &_OnWriteData);
    virtual ~ADTSParser();
    void parse(const uint8_t* ibuffer, int size);

};

#endif