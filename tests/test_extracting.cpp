/**
 * @file        test_extracting.cpp
 * @brief       Tests extracting of bitfields.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include "jungles/bitfield.hpp"

#include <iostream>
using namespace jungles;

enum class Reg
{
    field1,
    field2,
    field3,
    field4,
    field5,
    field6,
    field7,
    field8
};

TEST_CASE("Extracts bitfields on one-byte long bitfield", "[extract]")
{
    SECTION("For three fields, almost evenly aligned")
    {
        Bitfield<uint8_t, ByteOrder::little, Field{Reg::field1, 3}, Field{Reg::field2, 2}, Field{Reg::field3, 3}> bf;

        bf.at<Reg::field1>() = 0b101;
        bf.at<Reg::field2>() = 0b11;
        bf.at<Reg::field3>() = 0b010;

        REQUIRE(bf.extract<Reg::field1>() == 0b10100000);
        REQUIRE(bf.extract<Reg::field2>() == 0b00011000);
        REQUIRE(bf.extract<Reg::field3>() == 0b00000010);
    }

    SECTION("For four fields, with various sizes")
    {
        Bitfield<uint8_t,
                 ByteOrder::little,
                 Field{Reg::field1, 1},
                 Field{Reg::field2, 4},
                 Field{Reg::field3, 1},
                 Field{Reg::field4, 2}>
            bf;

        bf.at<Reg::field1>() = 0b1;
        bf.at<Reg::field2>() = 0b0101;
        bf.at<Reg::field3>() = 0b0;
        bf.at<Reg::field4>() = 0b11;

        REQUIRE(bf.extract<Reg::field1>() == 0b10000000);
        REQUIRE(bf.extract<Reg::field2>() == 0b00101000);
        REQUIRE(bf.extract<Reg::field3>() == 0b00000000);
        REQUIRE(bf.extract<Reg::field4>() == 0b00000011);
    }

    SECTION("For each field having size one")
    {
        Bitfield<uint8_t,
                 ByteOrder::little,
                 Field{Reg::field1, 1},
                 Field{Reg::field2, 1},
                 Field{Reg::field3, 1},
                 Field{Reg::field4, 1},
                 Field{Reg::field5, 1},
                 Field{Reg::field6, 1},
                 Field{Reg::field7, 1},
                 Field{Reg::field8, 1}>
            bf;

        bf.at<Reg::field1>() = 0b1;
        bf.at<Reg::field2>() = 0b0;
        bf.at<Reg::field3>() = 0b0;
        bf.at<Reg::field4>() = 0b1;
        bf.at<Reg::field5>() = 0b0;
        bf.at<Reg::field6>() = 0b1;
        bf.at<Reg::field7>() = 0b1;
        bf.at<Reg::field8>() = 0b0;

        REQUIRE(bf.extract<Reg::field1>() == 0b10000000);
        REQUIRE(bf.extract<Reg::field2>() == 0b00000000);
        REQUIRE(bf.extract<Reg::field3>() == 0b00000000);
        REQUIRE(bf.extract<Reg::field4>() == 0b00010000);
        REQUIRE(bf.extract<Reg::field5>() == 0b00000000);
        REQUIRE(bf.extract<Reg::field6>() == 0b00000100);
        REQUIRE(bf.extract<Reg::field7>() == 0b00000010);
        REQUIRE(bf.extract<Reg::field8>() == 0b00000000);
    }

    SECTION("For one field, occupying the whole bitfield")
    {
        Bitfield<uint8_t, ByteOrder::little, Field{Reg::field1, 8}> bf;
        bf.at<Reg::field1>() = 0b10010110;
        REQUIRE(bf.extract<Reg::field1>() == 0b10010110);
    }
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, big endian", "[extract]")
{
    SECTION("For three fields, evenly aligned")
    {
        Bitfield<uint32_t, ByteOrder::big, Field{Reg::field1, 11}, Field{Reg::field2, 11}, Field{Reg::field3, 10}> bf;

        bf.at<Reg::field1>() = 0b10101010101;
        bf.at<Reg::field2>() = 0b01101101100;
        bf.at<Reg::field3>() = 0b0111011110;

        REQUIRE(bf.extract<Reg::field1>() == 0b10101010101000000000000000000000);
        REQUIRE(bf.extract<Reg::field2>() == 0b00000000000011011011000000000000);
        REQUIRE(bf.extract<Reg::field3>() == 0b00000000000000000000000111011110);
    }

    SECTION("For a two bit bitfield lying across two bytes")
    {
        Bitfield<uint32_t, ByteOrder::big, Field{Reg::field1, 15}, Field{Reg::field2, 2}, Field{Reg::field3, 15}> bf;
        bf.at<Reg::field1>() = 0b000001111100000;
        bf.at<Reg::field2>() = 0b11;
        bf.at<Reg::field3>() = 0b000001111100000;

        REQUIRE(bf.extract<Reg::field2>() == 0b00000000000000011000000000000000);
    }
}

TEST_CASE("Extracts bitfields on 4-byte long bitfield, little endian", "[extract]")
{
    SECTION("For three fields, evenly aligned")
    {
        Bitfield<uint32_t, ByteOrder::little, Field{Reg::field1, 11}, Field{Reg::field2, 11}, Field{Reg::field3, 10}>
            bf;

        bf.at<Reg::field1>() = 0b10101010101;
        bf.at<Reg::field2>() = 0b01101101100;
        bf.at<Reg::field3>() = 0b0111011110;

        REQUIRE(bf.extract<Reg::field1>() == 0b00000000000000001010000010101010);
        REQUIRE(bf.extract<Reg::field2>() == 0b00000000101100000000110100000000);
        REQUIRE(bf.extract<Reg::field3>() == 0b11011110000000010000000000000000);
    }

    SECTION("For a two bit bitfield lying across two bytes")
    {
    }

    SECTION("For four fields evenly aligned")
    {
    }

    SECTION("For four fields unevenly aligned")
    {
    }
}

TEST_CASE("Extracts bitfields on 8-byte long bitfield, little endian", "[extract]")
{
}

