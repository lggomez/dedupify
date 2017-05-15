#pragma once

const size_t QUANTIZATION_SIZE = 9;
const size_t HASH_SIZE = ((QUANTIZATION_SIZE - 1) * (QUANTIZATION_SIZE - 1)) * 2;
const size_t HASH_THRESHOLD = 3;
const size_t PRE_TRANSFORM_SIZE = 512;
const size_t DFT_IMAGE_SIZE = PRE_TRANSFORM_SIZE;