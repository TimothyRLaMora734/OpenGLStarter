#include "ADTSParser.h"

#pragma pack(push, 1)

struct aac_info{
    //bits 0 - 11
    uint16_t syncword;// A	12	syncword 0xFFF, all bits must be 1
    //bit 12
    uint8_t MPEGVersion;//B	1	MPEG Version: 0 for MPEG-4, 1 for MPEG-2
    //bits 13-14
    uint8_t Layer;//C	2	Layer: always 0
    //bit 15
    uint8_t ProtectionAbsent;//D	1	protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC
    //bits 16 - 17
    uint8_t MPEG4AudioObjectType_minus_1;//E	2	profile, the MPEG-4 Audio Object Type minus 1
    //bits 18 - 21
    uint8_t MPEG4SamplingFrequencyIndex;//F	4	MPEG-4 Sampling Frequency Index (15 is forbidden)
    //if 15 -> 24 bits: frequency
    //uint32_t frequency;// fill this value with automatic from sample freq index, or read from header
    //bit 22
    uint8_t private_bit;//G	1	private bit, guaranteed never to be used by MPEG, set to 0 when encoding, ignore when decoding
    //bits 23 - 25
    uint8_t MPEG4ChannelConfiguration;//H	3	MPEG-4 Channel Configuration (in the case of 0, the channel configuration is sent via an inband PCE)
    //frame length flag 0: Each packet contains 1024 samples, 1: Each packet contains 960 samples
    //bit 26
    uint8_t originality;//I	1	originality, set to 0 when encoding, ignore when decoding
    //bit 27
    uint8_t home;//J	1	home, set to 0 when encoding, ignore when decoding
    //bit 28
    uint8_t copyrighted_id_bit;//K	1	copyrighted id bit, the next bit of a centrally registered copyright identifier, set to 0 when encoding, ignore when decoding
    //bit 29
    uint8_t copyright_id_start;//L	1	copyright id start, signals that this frame's copyright id bit is the first bit of the copyright id, set to 0 when encoding, ignore when decoding
    //bits 30 - 42
    uint16_t frame_length;//M	13	frame length, this value must include 7 or 9 bytes of header length: FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    //bits 43 - 53
    uint16_t Buffer_fullness;//O	11	Buffer fullness
    //bits 54 - 55
    uint8_t Number_of_AACframes;//P	2	Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame
    // Above is the ADTS Header - 7 bytes
    //bits 56 - 71
    uint16_t CRC;//Q	16	CRC if protection absent is 0
};

#pragma pack(pop)

static inline uint32_t readbit(int bitPos,  const uint8_t* data, int size){
    int dataPosition = bitPos / 8;
    int bitPosition = 7 - bitPos % 8;
    if (dataPosition >= size){
        fprintf(stderr,"error to access bit...\n");
        exit(-1);
    }
    return (data[dataPosition] >> bitPosition) & 0x01;
}

static inline uint32_t readbits(int bitPos, int length, const uint8_t* data, int size){
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

aac_info fillAACStructure(const uint8_t* ibuffer, int size){
    aac_info result;
    
    //bits 0 - 11
    result.syncword = readbits(0, 12, ibuffer, size);// A    12    syncword 0xFFF, all bits must be 1
    //bit 12
    result.MPEGVersion = readbits(12, 1, ibuffer, size);//B    1    MPEG Version: 0 for MPEG-4, 1 for MPEG-2
    //bits 13-14
    result.Layer = readbits(13, 2, ibuffer, size);//C    2    Layer: always 0
    //bit 15
    result.ProtectionAbsent = readbits(15, 1, ibuffer, size);//D    1    protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC
    //bits 16 - 17
    result.MPEG4AudioObjectType_minus_1 = readbits(16, 2, ibuffer, size);//E    2    profile, the MPEG-4 Audio Object Type minus 1
    //bits 18 - 21
    result.MPEG4SamplingFrequencyIndex = readbits(18, 4, ibuffer, size);//F    4    MPEG-4 Sampling Frequency Index (15 is forbidden)
    //if 15 -> 24 bits: frequency
    //uint32_t frequency;// fill this value with automatic from sample freq index, or read from header
    //bit 22
    result.private_bit = readbits(22, 1, ibuffer, size);//G    1    private bit, guaranteed never to be used by MPEG, set to 0 when encoding, ignore when decoding
    //bits 23 - 25
    result.MPEG4ChannelConfiguration = readbits(23, 3, ibuffer, size);//H    3    MPEG-4 Channel Configuration (in the case of 0, the channel configuration is sent via an inband PCE)
    //frame length flag 0: Each packet contains 1024 samples, 1: Each packet contains 960 samples
    //bit 26
    result.originality = readbits(26, 1, ibuffer, size);//I    1    originality, set to 0 when encoding, ignore when decoding
    //bit 27
    result.home = readbits(27, 1, ibuffer, size);//J    1    home, set to 0 when encoding, ignore when decoding
    //bit 28
    result.copyrighted_id_bit = readbits(28, 1, ibuffer, size);//K    1    copyrighted id bit, the next bit of a centrally registered copyright identifier, set to 0 when encoding, ignore when decoding
    //bit 29
    result.copyright_id_start = readbits(29, 1, ibuffer, size);//L    1    copyright id start, signals that this frame's copyright id bit is the first bit of the copyright id, set to 0 when encoding, ignore when decoding
    //bits 30 - 42
    result.frame_length = readbits(30, 13, ibuffer, size);//M    13    frame length, this value must include 7 or 9 bytes of header length: FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    //bits 43 - 53
    result.Buffer_fullness = readbits(43, 11, ibuffer, size);//O    11    Buffer fullness
    //bits 54 - 55
    result.Number_of_AACframes = readbits(54, 2, ibuffer, size);//P    2    Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame
    // Above is the ADTS Header - 7 bytes
    //bits 56 - 71
    //result.CRC = readbits(0, 12, ibuffer, size);//Q    16    CRC if protection absent is 0
    
    return result;
}


void printInfo(const aac_info &info){
    printf("info \n");
    printf("     syncword: 0x%x (0xFFF)\n", info.syncword);
    printf("     MPEGVersion: 0x%x (0 for MPEG-4, 1 for MPEG-2)\n", info.MPEGVersion);
    printf("     Layer: 0x%x (0x00)\n", info.Layer);
    printf("     ProtectionAbsent: 0x%x (1 protection absent - No CRC)\n", info.ProtectionAbsent);
    printf("     MPEG4AudioObjectType_minus_1: 0x%x (MP4 profile)\n", info.MPEG4AudioObjectType_minus_1);
    printf("     MPEG4SamplingFrequencyIndex: 0x%x (!= 0x%x)\n", info.MPEG4SamplingFrequencyIndex, 15);
    printf("     private_bit: 0x%x (0x00)\n", info.private_bit);
    printf("     MPEG4ChannelConfiguration: 0x%x (!=0x00)\n", info.MPEG4ChannelConfiguration);
    printf("     originality: 0x%x (0x01)\n", info.originality);
    printf("     home: 0x%x (0x00)\n", info.home);
    printf("     copyrighted_id_bit: 0x%x (0x00)\n", info.copyrighted_id_bit);
    printf("     copyright_id_start: 0x%x (0x00)\n", info.copyright_id_start);
    printf("     frame_length: 0x%x (%u)\n", info.frame_length, info.frame_length);
    printf("     Buffer_fullness: 0x%x\n", info.Buffer_fullness);
    printf("     Number_of_AACframes: 0x%x\n", info.Number_of_AACframes);
}


void ADTSParser::putByte(uint8_t byte) {

    const uint32_t AAC_ADTS_HEADER_SIZE = 7;

    if ( byte == 0xff && (adtsState == None) ) { // && adtsState == None ) {
        adtsState = ADTS_SECOND_BYTE;
    } else if ( (byte & 0xf0) == 0xf0 && adtsState == ADTS_SECOND_BYTE) {
        adtsState = ADTS_HEADER_SIZE;

        buffer.clear();
        buffer.push_back(0xff);
        buffer.push_back(byte);
        
    } else if (adtsState == ADTS_HEADER_SIZE) {
        buffer.push_back(byte);
        if (buffer.size() == AAC_ADTS_HEADER_SIZE) {
            
            frameLength = ((buffer[3] & 0x03) << 11) | (buffer[4] << 3) | ((buffer[5] & 0xe0) >> 5);
            
            uint8_t ProtectionAbsent = readbit(15, (uint8_t*)&buffer[0], buffer.size());
            
            if (ProtectionAbsent == 0){
                fprintf(stderr, "Error: CRC skip not implemented...\n");
                exit(-1);
            }
            
            //aac_info info = fillAACStructure((uint8_t*)&buffer[0], buffer.size());
            //printInfo(info);
            
            adtsState = ADTS_LENGTH_SIZE;
        }
    } else if (adtsState == ADTS_LENGTH_SIZE) {
        buffer.push_back(byte);
        if (buffer.size() == frameLength) {//AAC_ADTS_HEADER_SIZE +
            OnWriteData(&buffer[0], buffer.size());
            adtsState = None;
        }
    } else {
        adtsState = None;
    }
}

ADTSParser::ADTSParser(const OnDataMethodPtrT &_OnWriteData) {
    adtsState = None;
    OnWriteData = _OnWriteData;
}

ADTSParser::~ADTSParser(){
}

void ADTSParser::parse(const uint8_t* ibuffer, int size) {
    
    //const uint32_t AAC_ADTS_HEADER_SIZE = 7;
    
    for(int i=0;i<size;i++){
        putByte(ibuffer[i]);
        
        /*
        if (adtsState == ADTS_LENGTH_SIZE && buffer.size() == AAC_ADTS_HEADER_SIZE){
            if (frameLength != size) {
                uint32_t  MPEG4SamplingFrequencyIndex = readbits(18, 4, (const uint8_t*)&buffer[0], buffer.size());
                
                printf("error on parsing ADTS frameSize: %u (%u, %u) size: %u (0x%x) sampleIndex: %u\n",
                       frameLength,
                       ((buffer[3] & 0x03) << 11) | (buffer[4] << 3) | ((buffer[5] & 0xe0) >> 5),
                       readbits(30, 13, (const uint8_t*)&buffer[0], buffer.size()),
                       size,
                       size,
                       MPEG4SamplingFrequencyIndex);
                exit(-1);
            }
        }
        */
    }
}
