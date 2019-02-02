#ifndef BinaryReader__H
#define BinaryReader__H


#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include <zlib/zlib.h>

class BinaryReader {

	//FILE* in;

	std::vector<char> buffer;
	int readPos;

	bool eof() {
		return readPos >= buffer.size();
	}
public:

	BinaryReader(const char* filename, bool compressed) {
		readPos = 0;
		FILE* in = fopen(filename, "rb");

		if (in != NULL) {
			while (!feof(in)) {
				char inChar;
				if (fread(&inChar, sizeof(char), 1, in) <=0)
					break;
				buffer.push_back(inChar);
			}
			fclose(in);

			if (compressed) {
				std::vector<char> zlibInputBuffer(buffer);
				
				uLongf zlibUncompressed_Length = (uLongf)(*((unsigned int*)&zlibInputBuffer[0]));

				buffer.resize(zlibUncompressed_Length);
				int result = uncompress((Bytef *)&buffer[0], &zlibUncompressed_Length, (Bytef *)&zlibInputBuffer[sizeof(unsigned int)], zlibInputBuffer.size());

				if (result != Z_OK || buffer.size() != zlibUncompressed_Length) {
					fprintf(stderr, "[%s:%i]\n", __FILE__, __LINE__);
					fprintf(stderr, "Error to uncompress input file: %s\n", filename);
					exit(-1);
				}

			}

		}

	}

	~BinaryReader() {
		close();
	}

	void close() {
		/*
		if (in != NULL) {
			fclose(in);
			in = NULL;
		}
		*/
	}

	template <typename T>
	T read() {
		if (eof())
			return T();
		T result;
		
		memcpy(&result, &buffer[readPos], sizeof(T));
		readPos += sizeof(T);

		//fread(&result, sizeof(T), 1, in);

		return result;
	}

	std::string readString() {
		if (eof())
			return "";
		std::string result;
		unsigned short size = read<unsigned short>();
		result.resize(size + 1, '\0');

		memcpy(&result[0], &buffer[readPos], size);
		readPos += size;

		//fread(&result[0], size, 1, in);

		return &result[0];
	}


	template <typename T>
	void readVector(std::vector<T> *result) {
		if (eof())
			return;

		int size = read<int>();
		result->resize(size);
		//std::vector<T> result(size);
		if (size > 0) {

			memcpy(&(*result)[0], &buffer[readPos], size * sizeof(T));
			readPos += size * sizeof(T);

			//fread(&(*result)[0], size, sizeof(T), in);
		}
		//return result;
	}

	template <typename T>
	void readStringMap(std::map<std::string, T> *result) {
		if (eof())
			return;

		int size = read<int>();
		//std::map<std::string, T> result;
		(*result).clear();
		for (int i = 0; i < size; i++) {
			std::string aux = readString();
			T value = read<T>();
			(*result)[aux] = value;
		}

		//return result;
	}



};


#endif