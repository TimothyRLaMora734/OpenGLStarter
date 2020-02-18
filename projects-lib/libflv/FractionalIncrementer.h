#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#ifndef FRACTIONAL_INCREMENTER_H
#define FRACTIONAL_INCREMENTER_H


// use only int increment to advance video coding,
//  based on video framerate...
//
//  the original way was to do: timestamp_ms = (mH264NewFrameDetection.currentFrame * 1000)/30;
// with this class could be: timestamp_ms = increment()
class FractionalIncrementer {
    
    static uint32_t gcd( uint32_t a, uint32_t b );

    uint32_t incr_int;
    uint32_t incr_1_max_count;
    uint32_t count;
public:
    uint32_t samplerate;
    //the current timestamp in ms
    uint32_t value_ms;
    
    FractionalIncrementer();
    
    // creates a new FractionalIncrementer
    // Example: 
    //
    // Video with 30 FPS with 1 frame input sample per time:
    //   FractionalIncrementer(1000,30)
    //
    // Audio with 44.1khz with 1024 input samples per time:
    //   FractionalIncrementer(1000*1024,44100)
    FractionalIncrementer(uint32_t num, uint32_t den);
    
    uint32_t increment();
    
    //will not affect the current value_ms
    void changeSampleRate(uint32_t num, uint32_t den);
    
};


#endif
