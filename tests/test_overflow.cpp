/**
 * @file        test_overflow.cpp
 * @brief       Tests overflows are handled correctly by the bitfield group.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Overflows are not affecting other fields", "[overflow]")
{
    SECTION("Overflowing operation truncates the value")
    {
        Bitfield<uint16_t, ByteOrder::big, Field{Reg::field1, 2}, Field{Reg::field2, 8}, Field{Reg::field3, 6}> bf;

        bf.at<Reg::field2>() = 0xFFF;

        SECTION("When using operator at()")
        {
            REQUIRE(bf.at<Reg::field2>() == 0xFF);
        }

        SECTION("When extracting")
        {
            REQUIRE(bf.extract<Reg::field2>() == 0b0011111111000000);
        }

        SECTION("When serializing")
        {
            REQUIRE(bf.serialize() == 0b0011111111000000);
        }
    }

    SECTION("Serialize overflown bitfields with various sizes")
    {
        SECTION("Two bit bitfield, occupying bits by the border of two bytes")
        {
            Bitfield<uint16_t, ByteOrder::big, Field{Reg::field1, 7}, Field{Reg::field2, 2}, Field{Reg::field3, 7}> bf;
            bf.at<Reg::field1>() = 0b111000000;
            bf.at<Reg::field2>() = 0b101;
            bf.at<Reg::field3>() = 0b111100000;
            REQUIRE(bf.serialize() == 0b1000000011100000);
        }

        SECTION("One-bit-long bitfield")
        {
            Bitfield<uint32_t, ByteOrder::big, Field{Reg::field1, 17}, Field{Reg::field2, 1}, Field{Reg::field3, 14}>
                bf;
            bf.at<Reg::field1>() = 0b111000000;
            bf.at<Reg::field2>() = 0b11110;
            bf.at<Reg::field3>() = 0b0000111;
            REQUIRE(bf.serialize() == 0b111000000000000000000111);
        }

        SECTION("Evenly aligned three bitfields over four-byte-long bitfield")
        {
            Bitfield<uint32_t, ByteOrder::big, Field{Reg::field1, 10}, Field{Reg::field2, 11}, Field{Reg::field3, 11}>
                bf;
            bf.at<Reg::field1>() = 0b111111000000;
            bf.at<Reg::field2>() = 0b11111110010101001;
            bf.at<Reg::field3>() = 0b1111111111000111000;
            REQUIRE(bf.serialize() == 0b11110000001001010100111000111000);
        }
    }
}
