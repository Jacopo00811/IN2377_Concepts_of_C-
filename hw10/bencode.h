#pragma once

#include <optional>
#include <string_view>
#include <charconv>
#include <cctype>

namespace bencode {
// TODO: Implement int parsing for the bencode fromat
// Bencode encoded integers have the format `i<number>e`. Your function should return the number.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the integer number,
// which might be negative.
//
// We assume that the passed string_view contains only one encoding, and the last character
// of a valid encoding is 'e'. Therefore, you do not need to handle cases like 'i4ei42e'.
//
// Example of a valid encoding: "i42e", which should be parsed to the integer 42.
// Example of a valid encoding: "i-42e", which should be parsed to the integer -42.
//
// Edges cases you might need to look out for:
// - String not starting with 'i', or ending with 'e'
// - Handle empty string
// - Handle if a non-digit number is between 'i' and 'e'

consteval std::optional<int> parse_int(std::string_view str) {
    if (str.empty() || str.front() != 'i' || str.back() != 'e')
        return {}; // Invalid

    str.remove_prefix(1); // Remove 'i'
    str.remove_suffix(1); // Remove 'e'
    
    if (str.empty())
        return {}; // Empty string
    
    int result = 0;
    bool negative = false;

    if (str.front() == '-') {
        negative = true;
        str.remove_prefix(1); // Remove '-'
    }

    for (char c : str) {
        if (c >= '0' && c <= '9') {
            result = result * 10 + (c - '0');
        } else {
            return {}; // Non-digit character found
        }
    }

    // if (auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), result); ec == std::errc()) {
    //     return result;
    // } else {
    //     return {}; // Conversion failed
    // }
    return negative ? -result : result;
}

// TODO: Implement byte string parsing for the bencode fromat
// Bencode byte strings have the format <length>:<string>, where `length` is a number.
// The colon ':' separates the length and the actual string.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the string.
// For failure, the optional should be empty.
//
// Example of a valid byte string: "5:hello", where the length is 5, and the string is "hello".
//
// You may assume that all characters in the byte string are ASCII characters.
//
// We do not check for all edge cases, important edgecases your implementation should handle:
// - The byte string doesn't start with a number
// - The byte string doesn't have a colon
// - It is fine for the length to be shorter than the string, just return the string according to the length
// - It is NOT valid for the string to be shorter than the specified length
// - The string may contain colons

consteval bool my_is_digit(char c) {
    return c >= '0' && c <= '9';
}

consteval std::optional<std::string_view> parse_byte_string(std::string_view str) {
    if (str.empty() || !my_is_digit(str.front())) {
        return {}; // Invalid encoding format
    }

    size_t colonPos = str.find(':');
    if (colonPos == std::string_view::npos) {
        return {}; // No colon found
    }

    size_t length = 0;
    for (char c : str.substr(0, colonPos)) {
        if (c >= '0' && c <= '9') {
            length = length * 10 + (c - '0');
        } else {
            return {}; // Non-digit character found in length
        }
    }

    // if (auto [p, ec] = std::from_chars(str.data(), str.data() + colonPos, length); ec != std::errc()) {
    //     return {}; // Length parsing failed
    // }

    str.remove_prefix(colonPos + 1); // Move past the colon

    if (str.size() < length) {
        return {}; // Invalid length
    }

    return str.substr(0, length);
}
} // namespace bencode
