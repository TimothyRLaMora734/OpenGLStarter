#include "H264Parser.h"

// https://yumichan.net/video-processing/video-compression/introduction-to-h264-nal-unit/

#define NAL_IDC_FIELD (1 << 5) // 0x20
#define NAL_IDC_FRAME (2 << 5) // 0x40
#define NAL_IDC_PICTURE (3 << 5) // 0x60

#define NAL_TYPE_CSNIDRP 0x01 // Coded slice of a non-IDR picture
#define NAL_TYPE_CSDPA 0x02 // Coded slice data partition A
#define NAL_TYPE_CSDPB 0x03 //   Coded slice data partition B
#define NAL_TYPE_CSDPC 0x04 //   Coded slice data partition C
#define NAL_TYPE_CSIDRP 0x05 //   Coded slice of an IDR picture
#define NAL_TYPE_SEI 0x06 //   Supplemental enhancement information (SEI)
#define NAL_TYPE_SPS 0x07 //   Sequence parameter set
#define NAL_TYPE_PPS 0x08 //   Picture parameter set
#define NAL_TYPE_AUD 0x09 //   Access unit delimiter
#define NAL_TYPE_EOSEQ 0x0a // 10 //   End of sequence
#define NAL_TYPE_EOSTREAM 0x0b // 11 //   End of stream
#define NAL_TYPE_FillerData 0x0c // 12 //   Filler data
#define NAL_TYPE_SPSE 0x0d // 13 //   Sequence parameter set extension
#define NAL_TYPE_PrefixNALU 0x0e // 14 //   Prefix NAL unit
#define NAL_TYPE_SSPS 0x0f //15 //   Subset sequence parameter set
#define NAL_TYPE_CSOAACPWP 0x13 //19 //   Coded slice of an auxiliary coded picture without partitioning
#define NAL_TYPE_CSE 0x14 // 20 //   Coded slice extension
#define NAL_TYPE_CSEFDVC 0x15 // 21 //   Coded slice extension for depth view components

void H264Parser::putByte(uint8_t byte) {
    //
    // Detect start code 00 00 01 and 00 00 00 01
    //
    // It returns the buffer right after the start code
    //
    if (byte == 0x00 && nalState == None)
        nalState = NAL0;
    else if (byte == 0x00 && (nalState == NAL0 || nalState == NAL1) )
        nalState = NAL1;
    else if (byte == 0x01 && nalState == NAL1){
        nalState = None;

        if (writingState == None){
            writingState = Data;
            return;
        } else if (writingState == Data){

            buffer.pop_back();// 0x00
            buffer.pop_back();// 0x00

            //in the case using the format 00 00 00 01, remove the last element detected
            if (buffer.size()-2 >=0 &&
                buffer[buffer.size()-1] == 0x00 &&
                buffer[buffer.size()-2] != 0x03 )//keep value, if emulation prevention is present
                buffer.pop_back();

            OnWriteData(&buffer[0], buffer.size());

            buffer.clear();

            return;
        }
    } else
        nalState = None;

    if (writingState == Data){
        //
        // increase raw buffer size
        //
        buffer.push_back(byte);
    }
}

H264Parser::H264Parser(const FLV_OnDataMethodPtrT &_OnWriteData) {
    OnWriteData = _OnWriteData;
    nalState = None;
    writingState = None;
}

H264Parser::~H264Parser(){
}

void H264Parser::endOfStream() {
    if (buffer.size() <= 0)
        return;

    OnWriteData(&buffer[0],buffer.size());

    buffer.clear();
    writingState = None;
    nalState = None;
}

void H264Parser::parse(const uint8_t* ibuffer, int size) {
    for(int i=0;i<size;i++){
        putByte(ibuffer[i]);
    }
}


uint32_t H264Parser::readbit(int bitPos,  const uint8_t* data, int size){
    int dataPosition = bitPos / 8;
    int bitPosition = 7 - bitPos % 8;
    if (dataPosition >= size){
        fprintf(stderr,"error to access bit...\n");
        exit(-1);
    }
    return (data[dataPosition] >> bitPosition) & 0x01;
}

// leading 0`s count equals the number of next bits after bit 1
//
//  Example: 01x  001xx 0001xxx 00001xxxx
//
//  The max number of bits is equal 32 in this sintax
//
int H264Parser::bitsToSkip_ue( int start,  const uint8_t* data, int size){
    int bitPos = start;
    int dataPosition = start / 8;
    int bitPosition;
    while (dataPosition < size){
        dataPosition = bitPos / 8;
        bitPosition = 7 - bitPos % 8;
        int bit = (data[dataPosition] >> bitPosition) & 0x01;
        if (bit == 1)
            break;
        bitPos++;
    }
    int leadingZeros = bitPos - start;
    int totalBits = leadingZeros + 1 + leadingZeros;
    if (totalBits > 32){
        fprintf(stderr,"bitsToSkip_ue length greated than 32\n");
        exit(-1);
    }
    return totalBits;
}

uint32_t H264Parser::read_golomb_ue(int start,  const uint8_t* data, int size){
    int bitPos = start;
    int dataPosition = start / 8;
    int bitPosition;
    while (dataPosition < size){
        dataPosition = bitPos / 8;
        bitPosition = 7 - bitPos % 8;
        int bit = (data[dataPosition] >> bitPosition) & 0x01;
        if (bit == 1)
            break;
        bitPos++;
    }
    uint32_t leadingZeros = (uint32_t)(bitPos - start);
    uint32_t num = readbits(bitPos+1, leadingZeros, data, size);
    num += (1 << leadingZeros) - 1;

    return num;
}

uint32_t H264Parser::readbits(int bitPos, int length, const uint8_t* data, int size){
    if (length > 32){
        fprintf(stderr,"readbits length greated than 32\n");
        exit(-1);
    }
    uint32_t result = 0;
    for(int i=0;i<length;i++){
        result <<= 1;
        result = result | readbit(bitPos+i, data, size);
    }
    return result;
}

sps_info H264Parser::parseSPS(const std::vector<uint8_t> sps_rbsp) {
    const uint8_t *data = &sps_rbsp[0];
    uint32_t size = sps_rbsp.size();

    sps_info result;

    result.profile_idc = sps_rbsp[1];
    result.constraints = sps_rbsp[2];
    result.level_idc = sps_rbsp[3];

    uint32_t startIndex = 8+24;//NAL bit + profile_idc (8bits) + constraints (8bits) + level_idc (8bits)
    startIndex += bitsToSkip_ue(startIndex, data, size);//seq_parameter_set_id (ue)

    uint32_t log2_max_frame_num_minus4 = read_golomb_ue(startIndex, data, size);

    if (log2_max_frame_num_minus4 < 0 ||
        log2_max_frame_num_minus4 > 12){
        fprintf(stderr,"parseSPS_log2_max_frame_num_minus4 value not in range [0-12] \n");
        exit(-1);
    }

    result.log2_max_frame_num = log2_max_frame_num_minus4 + 4;
    
    result.frame_num_mask = (( 1 << result.log2_max_frame_num ) - 1);
    
    result.set = true;
    return result;
}

// Raw Byte Sequence Payload (RBSP) -- without the emulation prevention bytes
// maxSize is used to parse just the beggining of the nal structure...
// avoid process all buffer size on NAL new frame check
void H264Parser::nal2RBSP(const uint8_t *buffer, size_t size,
                            std::vector<uint8_t> *rbsp,
                            int maxSize){
    if (maxSize <= 0 || ( size < maxSize ) )
        maxSize = size;
    rbsp->resize(maxSize);

    State emulationState = None;
    int count = 0;

    for(int i=0; i < maxSize ;i++){
        uint8_t byte = buffer[i];

        if (byte == 0x00 && emulationState == None)
            emulationState = EMULATION0;
        else if (byte == 0x00 && (emulationState == EMULATION0 || emulationState == EMULATION1) )
            emulationState = EMULATION1;
        else if (byte == 0x03 && emulationState == EMULATION1)
        {
            emulationState = EMULATION_FORCE_SKIP;
            continue;
        }
        else if (emulationState == EMULATION_FORCE_SKIP) { //skip 00 01 02 or 03
            if ( byte != 0x00 && byte != 0x01 && byte != 0x02 && byte != 0x03 ){
                fprintf(stdout, "H264 NAL emulation prevention pattern detected error (%u)\n", byte);
                exit(-1);
            }
            emulationState = None;
        } else
            emulationState = None;

        (*rbsp)[count] = byte;
        count++;
    }
    if (count != rbsp->size())
        rbsp->resize(count);
}
