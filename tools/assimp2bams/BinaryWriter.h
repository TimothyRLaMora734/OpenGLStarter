#ifndef BinaryWritter__H
#define BinaryWritter__H

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include <zlib/zlib.h>
#include "MD5.h"

class BinaryWriter {

	std::string name;

	//FILE* out;
	std::vector<char> buffer;

	bool compress;
public:

	BinaryWriter(const char* filename, bool compress) {
		name = filename;
		this->compress = compress;
		//out = fopen(filename, "wb");
	}

	~BinaryWriter() {
		close();
	}

	void close() {
		if (buffer.size() == 0)
        {
            //skip when trying to write zero size buffer...
            return;
            //fprintf(stderr, "[%s:%i]\n", __FILE__, __LINE__ );
            //fprintf(stderr, "Error: trying to write buffer with size = 0\n");
            //exit(-1);
        }

		FILE *out = fopen(name.c_str(), "wb");
		if (out != NULL) {
			
			if (compress) {
				uLongf zlibOutput_Length = compressBound(buffer.size());

				std::vector<char> zlibOutput(zlibOutput_Length + sizeof(unsigned int));
                
                //
                // copy the size bits to the outputbuffer
                //
                unsigned int size = buffer.size();
                memcpy(&zlibOutput[0], &size, sizeof(unsigned int));
                
                //
                // Compress the remaining bits
                //
				int result = compress2((Bytef *)&zlibOutput[sizeof(unsigned int)], &zlibOutput_Length, (Bytef *)&buffer[0], buffer.size(), Z_BEST_COMPRESSION);

				if (result != Z_OK) {
					fprintf(stderr, "[%s:%i]\n", __FILE__, __LINE__ );
					fprintf(stderr, "Error to compress output file: %s\n", name.c_str());
					fclose(out);
					exit(-1);
				}
                
                //
                // write the md5 of the compressed buffer (16 bytes)
                //
                unsigned char md5[16];
                MD5::get16bytesHashFromBytes(&zlibOutput[0], zlibOutput_Length + sizeof(unsigned int), md5);
                fwrite(&md5, sizeof(unsigned char), 16, out);
                
                //
				//write the buffer
                //
				fwrite(&zlibOutput[0], sizeof(char), zlibOutput_Length + sizeof(unsigned int), out);

			} else
				//write binary
				fwrite(&buffer[0], sizeof(char), buffer.size(), out);



			fclose(out);
		}
		
		buffer.clear();

		/*
		if (out != NULL) {
			fclose(out);
			out = NULL;
		}
		*/
	}

	template <typename T>
	int write(const T &v) {
		//if (out == NULL)
			//return 0;
		
		int startWrite = buffer.size();
		buffer.resize(startWrite + sizeof(T));
		memcpy(&buffer[startWrite], &v, sizeof(T));

		//fwrite(&v, sizeof(T), 1, out);
		return sizeof(T);
	}

	void writeString(const std::string &s) {
		//if (out == NULL)
			//return;

		write<unsigned short>((unsigned short)s.size());

		int startWrite = buffer.size();
		buffer.resize(startWrite + s.size());
		memcpy(&buffer[startWrite], s.c_str(), s.size());

		//fwrite(s.c_str(), s.size(), 1, out);

		/*
		int count = s.size() + 1;
		if (count > size) {
			printf(" .... WARNING: string greated than the model exports: %s\n", s);
			count = size;
		}
		fwrite(s.c_str(), count, 1, out);
		while (count < size) {
			fwrite("\0", 1, 1, out);
			count++;
		}
		*/
	}

	template <typename T>
	void writeVector(const std::vector<T> &v) {
		//if (out == NULL)
			//return;
		write<int>(v.size());
		if (v.size() > 0) {

			int startWrite = buffer.size();
			buffer.resize(startWrite + v.size() * sizeof(T));
			memcpy(&buffer[startWrite], &v[0], v.size() * sizeof(T));

			//fwrite(&v[0], v.size(), sizeof(T), out);
		}
	}

	template <typename T>
	void writeStringMap(const std::map<std::string,T> &v) {
		//if (out == NULL)
			//return;
		write<int>(v.size());
		typename std::map<std::string, T>::const_iterator it;
		for (it = v.begin(); it != v.end(); it++) {
			writeString(it->first);
			write<T>(it->second);
		}
	}


};


#endif