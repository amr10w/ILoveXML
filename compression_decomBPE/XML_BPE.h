#ifndef XML_BPE
#define XML_BPE
#include "BPE.h"
#include "FileUtils.h"
#include <iostream>
#include <chrono>

void BPE_compress(const std::string &inputPath, const std::string &outputPath);

void BPE_decompress(const std::string &inputPath, const std::string &outputPath);

#endif