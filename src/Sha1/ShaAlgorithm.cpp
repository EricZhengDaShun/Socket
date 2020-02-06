#include "Sha1/ShaAlgorithm.hpp"

ShaAlgorithm::ShaAlgorithm()
{

}

ShaAlgorithm::~ShaAlgorithm()
{

}

void ShaAlgorithm::processBlock(const std::vector<Data32Bit>& blockData, Data32Bit& h0, Data32Bit& h1, Data32Bit& h2, Data32Bit& h3, Data32Bit& h4)
{
    method(blockData, h0, h1, h2, h3, h4);
    return;
}