#include "FractionalIncrementer.h"

uint32_t FractionalIncrementer::gcd( uint32_t a, uint32_t b )
{
    const uint32_t zero = 0;
    while ( true )
    {
        if ( a == zero )
            return b;
        b %= a;
        
        if ( b == zero )
            return a;
        a %= b;
    }
}
   
FractionalIncrementer::FractionalIncrementer() {
    
}
    
// creates a new FractionalIncrementer
// Example: 
//
// Video with 30 FPS with 1 frame input sample per time:
//   FractionalIncrementer(1000,30)
//
// Audio with 48khz with 1024 input samples per time:
//   FractionalIncrementer(1000*1024,48000) 
FractionalIncrementer::FractionalIncrementer(uint32_t num, uint32_t den) {
    if(num <= den){
        fprintf(stderr,"[FractionalIncrementer] the num must be greater than den...\n");
        exit(-1);
    }
    uint32_t gcd = FractionalIncrementer::gcd(num, den);
    this->incr_int = num/den;
    this->incr_1_max_count = den/gcd;
    value_ms = 0;
    count = 0;
    samplerate = den;
}

void FractionalIncrementer::changeSampleRate(uint32_t num, uint32_t den) {
    if(num <= den){
        fprintf(stderr,"[FractionalIncrementer] the num must be greater than den...\n");
        exit(-1);
    }
    uint32_t gcd = FractionalIncrementer::gcd(num, den);
    this->incr_int = num/den;
    this->incr_1_max_count = den/gcd;
    //value_ms = 0;
    count = 0;
    samplerate = den;
}

uint32_t FractionalIncrementer::increment(){
    value_ms += incr_int;
    count++;
    if (incr_1_max_count > 1 && count >= incr_1_max_count){
        count = 0;
        value_ms ++;
    }
    return value_ms;
}
