#ifndef FLV__H
#define FLV__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#ifndef O_BINARY
    #define O_BINARY 0
#endif

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

struct sps_info{
    uint8_t profile_idc;
    uint8_t constraints;
    uint8_t level_idc;
    uint32_t log2_max_frame_num;

    bool set;
};

class ParserH264 {
    std::vector<uint8_t> buffer;

    enum State{
        None,
        Data,
        NAL0,
        NAL1,

        EMULATION0,
        EMULATION1,
        EMULATION_FORCE_SKIP
    };
    State nalState; // nal = network abstraction layer
    State writingState;

    void putByte(uint8_t byte) {
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

                chunkDetectedH264(buffer);

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

public:

    ParserH264() {
        nalState = None;
        writingState = None;
    }

    virtual ~ParserH264(){
    }

    // rawBuffer might has emulation bytes
    // Raw Byte Sequence Payload (RBSP)
    virtual void chunkDetectedH264(const std::vector<uint8_t> &nal){

    }

    void endOfStreamH264() {
        if (buffer.size() <= 0)
            return;

        chunkDetectedH264(buffer);

        buffer.clear();
        writingState = None;
        nalState = None;
    }

    void parseH264(const uint8_t* ibuffer, int size) {
        for(int i=0;i<size;i++){
            putByte(ibuffer[i]);
        }
    }


    static inline uint32_t readbit(int bitPos,  const uint8_t* data, int size){
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
    static inline int bitsToSkip_ue( int start,  const uint8_t* data, int size){
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

    static inline uint32_t read_golomb_ue(int start,  const uint8_t* data, int size){
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

    static inline sps_info parseSPS(const std::vector<uint8_t> sps_rbsp) {
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
        result.set = true;
        return result;
    }

    // Raw Byte Sequence Payload (RBSP) -- without the emulation prevention bytes
    // maxSize is used to parse just the beggining of the nal structure...
    // avoid process all buffer size on NAL new frame check
    static inline void nal2RBSP(const std::vector<uint8_t> &buffer,
                                std::vector<uint8_t> *rbsp,
                                int maxSize = -1){
        if (maxSize <= 0)
            maxSize = buffer.size();
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

};


#define AAC_ADTS_HEADER_SIZE 7


uint8_t aac2FlvAudioTag(uint8_t aacAudioObjectType,
                        uint8_t aacSampleFrequencyIndex,
                        uint8_t aacChannelConfiguration) {
    //
    // Convert to FLV audio tag
    //
    // 0 mono, 1 stero
    uint8_t flvSoundType = aacChannelConfiguration - 1;
    uint8_t flvSoundRate = 0;
    switch (aacSampleFrequencyIndex) {
        case 10:
            flvSoundRate = 1;//11025
            break;
        case 7:
            flvSoundRate = 2;//22050
            break;
        case 4:
            flvSoundRate = 3;//44100
            break;
        default:
            return 0;
    }
    return 0xa0 | (flvSoundRate << 2) | 0x02 | flvSoundType;
}

uint8_t aac2FlvObjTAndFreq(uint8_t aacAudioObjectType,
                           uint8_t aacSampleFrequencyIndex,
                           uint8_t aacChannelConfiguration) {
    uint8_t audioObjectType = aacAudioObjectType + 1;
    return (audioObjectType << 3) | (aacSampleFrequencyIndex >> 1);
}

struct aac_info{
    uint16_t syncword;// A	12	syncword 0xFFF, all bits must be 1
    uint8_t MPEGVersion;//B	1	MPEG Version: 0 for MPEG-4, 1 for MPEG-2
    uint8_t Layer;//C	2	Layer: always 0
    uint8_t ProtectionAbsent;//D	1	protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC
    
    uint8_t MPEG4AudioObjectType_minus_1;//E	2	profile, the MPEG-4 Audio Object Type minus 1
    uint8_t MPEG4SamplingFrequencyIndex;//F	4	MPEG-4 Sampling Frequency Index (15 is forbidden)
    //if 15 -> 24 bits: frequency
    uint32_t frequency;// fill this value with automatic from sample freq index, or read from header
    
    uint8_t private_bit;//G	1	private bit, guaranteed never to be used by MPEG, set to 0 when encoding, ignore when decoding
    uint8_t MPEG4ChannelConfiguration;//H	3	MPEG-4 Channel Configuration (in the case of 0, the channel configuration is sent via an inband PCE)
    
    //frame length flag 0: Each packet contains 1024 samples, 1: Each packet contains 960 samples
    uint8_t originality;//I	1	originality, set to 0 when encoding, ignore when decoding
    uint8_t home;//J	1	home, set to 0 when encoding, ignore when decoding
    uint8_t copyrighted_id_bit;//K	1	copyrighted id bit, the next bit of a centrally registered copyright identifier, set to 0 when encoding, ignore when decoding
    uint8_t copyright_id_start;//L	1	copyright id start, signals that this frame's copyright id bit is the first bit of the copyright id, set to 0 when encoding, ignore when decoding
    uint16_t frame_length;//M	13	frame length, this value must include 7 or 9 bytes of header length: FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    uint16_t Buffer_fullness;//O	11	Buffer fullness
    uint8_t Number_of_AACframes;//P	2	Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame
    uint16_t CRC;//Q	16	CRC if protection absent is 0
};


class ParserAAC {
    std::vector<uint8_t> buffer;

    enum State{
        None,
        Data,
        ADTS_SECOND_BYTE,
        ADTS_HEADER_SIZE,
        ADTS_LENGTH_SIZE
    };

    State adtsState;
    uint32_t frameLength;

    void putByte(uint8_t byte) {

        if ( byte == 0xff ) { // && adtsState == None ) {
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
                adtsState = ADTS_LENGTH_SIZE;
            }
        } else if (adtsState == ADTS_LENGTH_SIZE) {
            buffer.push_back(byte);
            if (buffer.size() == AAC_ADTS_HEADER_SIZE + frameLength) {
                chunkDetectedAAC(&buffer[0], buffer.size());
                adtsState = None;
            }
        } else {
            adtsState = None;
        }
    }

public:

    ParserAAC() {
        adtsState = None;
    }

    virtual ~ParserAAC(){
    }

    virtual void chunkDetectedAAC(const uint8_t* ibuffer, int size){
    }

    void parseAAC(const uint8_t* ibuffer, int size) {
        for(int i=0;i<size;i++){
            putByte(ibuffer[i]);
        }
    }
};


class FLVWritter{
public:
    std::vector<uint8_t> buffer;

    void flushToFD(int fd) {
        if (buffer.size() <= 0)
            return;
        ::write(fd,&buffer[0],buffer.size());
        buffer.clear();
    }

    void reset(){
        buffer.clear();
    }

    void writeFLVFileHeader(bool haveAudio, bool haveVideo){
        uint8_t flv_header[13] = {
            0x46, 0x4c, 0x56, 0x01, 0x05,
            0x00, 0x00, 0x00, 0x09, 0x00,
            0x00, 0x00, 0x00
        };

        if (haveAudio && haveVideo) {
            flv_header[4] = 0x05;
        } else if (haveAudio && !haveVideo) {
            flv_header[4] = 0x04;
        } else if (!haveAudio && haveVideo) {
            flv_header[4] = 0x01;
        } else {
            flv_header[4] = 0x00;
        }

        for(int i=0;i<13;i++)
            buffer.push_back(flv_header[i]);
    }

    void writeUInt8(uint8_t v) {
        buffer.push_back(v);
    }

    void writeUInt16(uint32_t v){
        buffer.push_back((v >> 8) & 0xff);
        buffer.push_back((v) & 0xff);
    }

    void writeUInt24(uint32_t v){
        buffer.push_back((v >> 16) & 0xff);
        buffer.push_back((v >> 8) & 0xff);
        buffer.push_back((v) & 0xff);
    }

    void writeUInt32(uint32_t v){
        buffer.push_back((v >> 24) & 0xff);
        buffer.push_back((v >> 16) & 0xff);
        buffer.push_back((v >> 8) & 0xff);
        buffer.push_back((v) & 0xff);
    }

    void writeUInt32(uint32_t v, std::vector<uint8_t> *data){
        data->push_back((v >> 24) & 0xff);
        data->push_back((v >> 16) & 0xff);
        data->push_back((v >> 8) & 0xff);
        data->push_back((v) & 0xff);
    }

    void writeUInt32Timestamp(uint32_t v){
        buffer.push_back((v >> 16) & 0xff);
        buffer.push_back((v >> 8) & 0xff);
        buffer.push_back((v) & 0xff);
        buffer.push_back((v >> 24) & 0xff);
    }

    void writeVideoSequenceHeader(const std::vector<uint8_t> &sps, const std::vector<uint8_t> &pps, const sps_info &spsinfo) {

        //
        // flv tag header = 11 bytes
        //
        writeUInt8(0x09);//tagtype video
        writeUInt24( sps.size() + pps.size() + 16 );//data len
        writeUInt32Timestamp( 0 );//timestamp
        writeUInt24( 0 );//stream id 0

        //
        // Message Body = 16 bytes + SPS bytes + PPS bytes
        //
        //flv VideoTagHeader
        writeUInt8(0x17);//key frame, AVC 1:keyframe 7:h264
        writeUInt8(0x00);//avc sequence header
        writeUInt24( 0x00 );//composition time

        //flv VideoTagBody --AVCDecoderCOnfigurationRecord
        writeUInt8(0x01);//configurationversion
        writeUInt8(spsinfo.profile_idc);//avcprofileindication
        writeUInt8(spsinfo.constraints);//profilecompatibilty
        writeUInt8(spsinfo.level_idc);//avclevelindication
        writeUInt8(0xFC | 0x03); //reserved (6 bits), NULA length size - 1 (2 bits)
        writeUInt8(0xe0 | 0x01); // first reserved, second number of SPS

        writeUInt16( sps.size() ); //sequence parameter set length
        //H264 sequence parameter set raw data
        for(int i=0;i<sps.size();i++)
            writeUInt8(sps[i]);

        writeUInt8(0x01); // number of PPS

        writeUInt16(pps.size()); //picture parameter set length
        //H264 picture parameter set raw data
        for(int i=0;i<pps.size();i++)
            writeUInt8(pps[i]);

        if (buffer.size() != sps.size() + pps.size() + 16 + 11 ){
            fprintf(stderr, "error writeVideoSequenceHeader\n");
            exit(-1);
        }

        // previous tag size
        writeUInt32(buffer.size());
    }

    void writeVideoFrame(const std::vector<uint8_t> &data, bool keyframe, uint32_t timestamp_ms, int streamID){

        writeUInt8(0x09);//tagtype video
        writeUInt24( data.size() + 5 );//data len
        writeUInt32Timestamp( timestamp_ms );//timestamp
        writeUInt24( streamID );//stream id 0)

        if (keyframe)
            writeUInt8(0x17);//key frame, AVC 1:keyframe 2:inner frame 7:H264
        else
            writeUInt8(0x27);//key frame, AVC 1:keyframe 2:inner frame 7:H264

        writeUInt8(0x01);//avc NALU unit
        writeUInt24(0x00);//composit time ??????????

        for(int i=0;i<data.size();i++)
            writeUInt8(data[i]);

        if (buffer.size() != data.size() + 5 + 11 ){
            fprintf(stderr, "error writeVideoFrame\n");
            exit(-1);
        }

        // previous size
        writeUInt32(buffer.size());

    }

    void writeVideoEndOfStream(uint32_t timestamp_ms, int streamID){
        writeUInt8(0x09);//tagtype video
        writeUInt24( 5 );//data len
        writeUInt32Timestamp( timestamp_ms );//timestamp
        writeUInt24( streamID );//stream id 0)

        writeUInt8(0x17);//key frame, AVC 1:keyframe 2:inner frame 7:H264
        writeUInt8(0x02);//avc EOS
        writeUInt24(0x00);//composit time ??????????

        if (buffer.size() != 5 + 11 ){
            fprintf(stderr, "error writeVideoEOS\n");
            exit(-1);
        }

        // previous size
        writeUInt32(buffer.size());
    }

};

enum FLV_TAG_TYPE {
    FLV_TAG_TYPE_AUDIO = 0x08,
    FLV_TAG_TYPE_VIDEO = 0x09,
    FLV_TAG_TYPE_META = 0x12
};

//  Detection of the first VCL NAL unit of a primary coded picture
// https://stackoverflow.com/questions/19642736/count-frames-in-h-264-bitstream?rq=1
class H264NewFrameDetection {
    std::vector<uint8_t> aux;

    bool newFrameOnNextIDR;
    uint32_t old_frame_num;
    bool spsinfo_set;

    bool firstFrame;
public:
    uint32_t currentFrame;
    bool newFrameFound;

    H264NewFrameDetection() {
        currentFrame = 0;
        newFrameOnNextIDR = false;
        old_frame_num = 0;
        spsinfo_set = false;
        firstFrame = true;
		newFrameFound = false;
    }

    void analyseBufferForNewFrame(const std::vector<uint8_t> &nal, const sps_info &spsinfo){

        uint8_t nal_bit = nal[0];
        uint8_t nal_type = (nal_bit & 0x1f);

        if ( nal_type == (NAL_TYPE_SPS) ||
            nal_type == (NAL_TYPE_PPS) ||
            nal_type == (NAL_TYPE_AUD) ||
            nal_type == (NAL_TYPE_SEI) ||
            (nal_type >= 14 && nal_type <= 18)
            ) {
            newFrameOnNextIDR = true;
        }

        if ((nal_type == NAL_TYPE_CSIDRP ||
             nal_type == NAL_TYPE_CSNIDRP)
            && spsinfo.set ){

            aux.clear();
            //(8 + 3*(32*2+1) + 16) = max header per NALU slice bits = 27.375 bytes
            // 32 bytes + 8 (Possibility of Emulation in 32 bytes)
            int RBSPMaxBytes = 32 + 8;
            if (nal.size() < (32 + 8))
                RBSPMaxBytes = nal.size();
            ParserH264::nal2RBSP(nal, &aux, RBSPMaxBytes );
            uint8_t * rbspBuffer = &aux[0];
            uint32_t rbspBufferSize = aux.size();

            uint32_t frame_num_index = 8;//start counting after the nal_bit
            //first_mb_in_slice (ue)
            frame_num_index += ParserH264::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
            //slice_type (ue)
            frame_num_index += ParserH264::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
            //pic_parameter_set_id (ue)
            frame_num_index += ParserH264::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
            //now can read frame_num
            uint32_t frame_num = ParserH264::readbits(frame_num_index,
                                                      spsinfo.log2_max_frame_num,
                                                      rbspBuffer, rbspBufferSize);

            if (!spsinfo_set){
                old_frame_num = frame_num;
                spsinfo_set = true;//spsinfo.set
            }

            if (old_frame_num != frame_num){
                newFrameOnNextIDR = true;
                old_frame_num = frame_num;
            }
        }


        if (newFrameOnNextIDR &&(nal_type == NAL_TYPE_CSIDRP ||
                                 nal_type == NAL_TYPE_CSNIDRP)) {
            newFrameOnNextIDR = false;
            if (firstFrame){//skip the first frame
                firstFrame = false;
            } else {
                newFrameFound = true;
                currentFrame++;
            }
        }
    }

    void reset(){
        newFrameFound = false;
    }

};

// use only int increment to advance video coding,
//  based on video framerate...
//
//  the original way was to do: timestamp_ms = (mH264NewFrameDetection.currentFrame * 1000)/30;
// with this class could be: timestamp_ms = increment()
class FractionalIntIncrementer {
    
    static uint32_t gcd( uint32_t a, uint32_t b )
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

    uint32_t incr_int;
    uint32_t incr_1_max_count;
    uint32_t count;
public:
    uint32_t timestamp_ms;
    
    FractionalIntIncrementer() {
        
    }
    
    FractionalIntIncrementer(uint32_t num, uint32_t den) {
        uint32_t gcd = FractionalIntIncrementer::gcd(num, den);
        this->incr_int = num/den;
        this->incr_1_max_count = den/gcd;
        timestamp_ms = 0;
        count = 0;
    }
    
    uint32_t increment(){
        timestamp_ms += incr_int;
        count++;
        if (incr_1_max_count > 1 && count >= incr_1_max_count){
            count = 0;
            timestamp_ms ++;
        }
        return timestamp_ms;
    }
    
};

class FLVFileWriter: public ParserAAC, public ParserH264 {

    std::vector<uint8_t> sps;
    std::vector<uint8_t> pps;

    sps_info spsInfo;

    H264NewFrameDetection mH264NewFrameDetection;
    FractionalIntIncrementer videoTimestampIncrementer;

public:
    FLVWritter mFLVWritter;
    int fd;
    bool firstAudioWrite;
    uint32_t audioTimestamp_ms;
    uint32_t videoTimestamp_ms;

    //contains the several NALs until complete a frame... after that can write to FLV
    std::vector<uint8_t> nalBuffer;

    FLVFileWriter ( const char* file ) {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY , 0644 );
        if (fd < 0){
            fprintf(stderr,"error to create flv file\n");
            exit(-1);
        }

        //have audio, have video
        mFLVWritter.writeFLVFileHeader(false, true);
        mFLVWritter.flushToFD(fd);

        firstAudioWrite = true;
        audioTimestamp_ms = 0;
        videoTimestamp_ms = 0;

        spsInfo.set = false;
        
        //1000 ms to 30 frames time incrementer
        videoTimestampIncrementer = FractionalIntIncrementer(1000,30);
    }

    virtual ~FLVFileWriter(){
        if (fd >= 0){

            //force write the last frame
            if (nalBuffer.size() > 0){
                uint8_t firstNALtype = nalBuffer[4] & 0x1f;

                bool iskeyframe = (firstNALtype == NAL_TYPE_CSIDRP);
                mFLVWritter.writeVideoFrame(nalBuffer, iskeyframe, videoTimestamp_ms, 0);
                mFLVWritter.flushToFD(fd);

                nalBuffer.clear();

                mFLVWritter.writeVideoEndOfStream(videoTimestamp_ms,0);
                mFLVWritter.flushToFD(fd);
            } else {
                //if (mH264NewFrameDetection.currentFrame > 0)
                    //videoTimestamp_ms = ((mH264NewFrameDetection.currentFrame-1) * 1000)/30;
                mFLVWritter.writeVideoEndOfStream(videoTimestamp_ms,0);
                mFLVWritter.flushToFD(fd);
            }
            close(fd);
        }
    }


    //decoding time stamp (DTS) and presentation time stamp (PTS)
    void chunkDetectedH264(const std::vector<uint8_t> &data) {

        //printf("[debug] Detected NAL chunk size: %i\n",rbspBufferSize);

        if (data.size() <= 0){
            fprintf(stdout, "  error On h264 chunk detection\n");
            return;
        }

        uint8_t nal_bit = data[0];
        uint8_t nal_type = (nal_bit & 0x1f);

        mH264NewFrameDetection.analyseBufferForNewFrame(data, spsInfo);

        if (mH264NewFrameDetection.newFrameFound){
            mH264NewFrameDetection.reset();

			//write the previous nalBuffer set related to previous frame.
            uint8_t firstNALtype = nalBuffer[4] & 0x1f;

            bool iskeyframe = (firstNALtype == NAL_TYPE_CSIDRP);
            mFLVWritter.writeVideoFrame(nalBuffer, iskeyframe, videoTimestamp_ms, 0);
            mFLVWritter.flushToFD(fd);

            nalBuffer.clear();

            videoTimestamp_ms = videoTimestampIncrementer.increment();
            //videoTimestamp_ms = (mH264NewFrameDetection.currentFrame * 1000)/30;
            
            /*
            if (videoTimestamp_ms != (mH264NewFrameDetection.currentFrame * 1000)/30){
                printf("error...\n");
                exit(-1);
            }*/
            
        }

        //0x67
        //if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_SPS) ) {
        if ( nal_type == (NAL_TYPE_SPS) ) {
            fprintf(stdout, " processing: 0x%x (SPS)\n",nal_bit);

            sps.clear();
            //max 26 bytes on sps header (read until log2_max_frame_num_minus4)
            nal2RBSP(data, &sps, 26 );
            spsInfo = parseSPS(sps);

            sps.clear();
            //mFLVWritter.writeUInt32(data.size(), &sps);
            for(int i=0;i<data.size();i++)
                sps.push_back(data[i]);


		}
        //0x68
        //else if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_PPS) ) {
        else if ( nal_type == (NAL_TYPE_PPS) ) {
            fprintf(stdout, " processing: 0x%x (PPS)\n",nal_bit);

            pps.clear();
            //mFLVWritter.writeUInt32(data.size(), &pps);
            for(int i=0;i<data.size();i++)
                pps.push_back(data[i]);

            mFLVWritter.writeVideoSequenceHeader(sps, pps, spsInfo);
            mFLVWritter.flushToFD(fd);

        }
        //0x65, 0x61, 0x41
        else if ( nal_type == NAL_TYPE_CSIDRP ||
                  nal_type == NAL_TYPE_CSNIDRP ) {

            mFLVWritter.writeUInt32( data.size(), &nalBuffer );
            for(int i=0;i<data.size();i++)
                nalBuffer.push_back(data[i]);

        } else if (nal_type == (NAL_TYPE_SEI)) {
            fprintf(stdout, " ignoring SEI bit: 0x%x type: 0x%x\n",nal_bit, nal_type);
        } else {
            // nal_bit type not implemented...
            fprintf(stdout, "Error: unknown NAL bit: 0x%x type: 0x%x\n",nal_bit, nal_type);
            exit(-1);
        }

    }

    void chunkDetectedAAC(const uint8_t* ibuffer, int size){
        //
        // Extract AAC audio configuration from AAC header
        //
        uint8_t aacAudioObjectType = (ibuffer[2] & 0xc0) >> 6;
        uint8_t aacSampleFrequencyIndex =  (ibuffer[2] & 0x3c) >> 2;
        uint8_t aacChannelConfiguration = (ibuffer[3] & 0xc0) >> 6;

        //
        // write audio config
        //
        if (firstAudioWrite){
            firstAudioWrite = false;

            uint32_t bodyLength = 2 + 2; //AudioTagHeader + AudioSpecificConfig

            //
            // flv tag header = 11 bytes
            //
            mFLVWritter.writeUInt8(0x08);//tagtype audio
            mFLVWritter.writeUInt24( bodyLength );//data len
            mFLVWritter.writeUInt32Timestamp( audioTimestamp_ms );//timestamp
            mFLVWritter.writeUInt24( 0 );//stream id 0
            //
            // Message Body = 4 bytes
            //
            //flv AudioTagHeader
            mFLVWritter.writeUInt8( aac2FlvAudioTag(aacAudioObjectType,
                                                    aacSampleFrequencyIndex,
                                                    aacChannelConfiguration ) ); // sound format aac
            mFLVWritter.writeUInt8(0x00); //aac sequence header
            //flv AudioSpecificConfig
            mFLVWritter.writeUInt8( aac2FlvObjTAndFreq(aacAudioObjectType,
                                                       aacSampleFrequencyIndex,
                                                       aacChannelConfiguration ) );
            mFLVWritter.writeUInt8(((aacSampleFrequencyIndex & 0x01) << 7) |
                                   (aacChannelConfiguration << 3));

            //
            // previous tag size
            //
            uint32_t currentSize = mFLVWritter.buffer.size();
            if (currentSize != bodyLength + 11 ){
                fprintf(stderr, "error to write flv audio tag\n");
                exit(-1);
            }
            mFLVWritter.writeUInt32(currentSize);//data len
            mFLVWritter.flushToFD(fd);
        }

        //
        // write audio data
        //
        uint32_t bodyLength = 2 + size - AAC_ADTS_HEADER_SIZE; // AudioTagHeader + raw AAC w/o header

        //
        // flv tag header = 11 bytes
        //
        mFLVWritter.writeUInt8(0x08);//tagtype audio
        mFLVWritter.writeUInt24( bodyLength );//data len
        mFLVWritter.writeUInt32Timestamp( audioTimestamp_ms );//timestamp
        mFLVWritter.writeUInt24( 0 );//stream id 0
        //
        // Message Body = 2 + size - AAC_ADTS_HEADER_SIZE bytes
        //
        //flv AudioTagHeader
        mFLVWritter.writeUInt8( aac2FlvAudioTag(aacAudioObjectType,
                                                aacSampleFrequencyIndex,
                                                aacChannelConfiguration ) ); // sound format aac
        mFLVWritter.writeUInt8(0x01); //aac raw data
        //flv raw aac data
        for (int i=AAC_ADTS_HEADER_SIZE;i<size;i++)
            mFLVWritter.writeUInt8(ibuffer[i]);
        //
        // previous tag size
        //
        uint32_t currentSize = mFLVWritter.buffer.size();
        if (currentSize != bodyLength + 11 ){
            fprintf(stderr, "error to write flv audio data\n");
            exit(-1);
        }
        mFLVWritter.writeUInt32(currentSize);//data len
        mFLVWritter.flushToFD(fd);
        audioTimestamp_ms += 100;
    }
};

#endif
