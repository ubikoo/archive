/*
 * string.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_STRING_H_
#define CORE_STRING_H_

#include "base.hpp"
#include "memory.hpp"

namespace core {
namespace str {

/** ---- String namespace -----------------------------------------------------
 *
 * @brief Namespace str provides a set of string helper functions for:
 *
 * - String manipulation
 * - C-string manipulation
 * - String cast conversion
 * - String format generation
 *
 * @see Static member functions
 * https://stackoverflow.com/questions/9346076
 */

/** ---- String manipulation functions ----------------------------------------
 *
 * ltrim
 * @brief Trim off left characters from the string(white-space by default).
 *
 * string::find_first_not_of(const string &chars, size_t pos = 0)
 *
 * Find the *first* character in the string which is not in the
 * character sequence over the interval [pos = 0, size()).
 *
 * If a character is not found, npos is returned, i.e. greatest
 * possible value for an element of type size_t.
 *
 * If npos is returned, then effectively *every* character in the
 * string is in the character sequence.
 * Calling erase with count = npos means all elements are removed.
 *
 * string::erase(size_t pos, size_t count = npos)
 *
 * Erase the substring beginning at position pos and spanning count
 * characters(or until end of the string if either size() is too
 * short or if count is string::npos).
 */
core_inline_
std::string &ltrim(std::string &str, const std::string &chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

/**
 * rtrim
 * @brief Trim off right characters from the string(white-space by default).
 *
 * string::find_last_not_of(const string &chars, size_t pos = npos)
 *
 * Find the *last* character in the string which is not in the
 * character sequence over the interval [0, pos = npos). A pos value
 * greater or equal to size() means entire string is searched.
 *
 * If a character is not found, npos is returned, i.e. greatest
 * possible value for an element of type size_t.
 *
 * If npos is returned, then effectively *every* character in the
 * string is in the character sequence.
 * Calling erase with pos = npos + 1 (= 0) means all elements are
 * removed.
 *
 * string::erase(size_t pos, size_t count = npos)
 *
 * Erase the substring beginning at position pos and spanning count
 * characters(or until end of the string if either size() is too
 * short or if count is string::npos).
 */
core_inline_
std::string &rtrim(std::string &str, const std::string &chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

/**
 * trim
 * @brief Trim off left and right characters from the string.
 * The default characters are white-space characters as given
 * by the "C" locale:
 *
 * ' '  (0x20)    space(SPC)
 * '\t' (0x09)    horizontal tab(TAB)
 * '\n' (0x0a)    newline(LF)
 * '\v' (0x0b)    vertical tab(VT)
 * '\f' (0x0c)    feed(FF)
 * '\r' (0x0d)    carriage return (CR)
 *
 * The trim functions return a reference to the operated string,
 * thereby allowing these to be chained together.
 */
core_inline_
std::string &trim(std::string &str, const std::string &chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

/**
 * replace
 * @brief Scan the string and replace any characters given by search by
 * the characters given by replace.
 */
core_inline_
std::string &replace(std::string &str, const char search, const char replace)
{
    /* Check the trivial case */
    if (search == replace) {
        return str;
    }

    /* Scan the string for search and replace */
    std::size_t tok = 0;
    while ((tok = str.find_first_of(search, tok)) != std::string::npos) {
        str[tok++] = replace;
    }

    return str;
}

/**
 * uncomment
 * @brief Trim off string comments.
 *
 * Scan the string and remove all characters between the comment symbol '#'
 * and the end character.
 *
 * For a single line where the newline character has been replaced by the
 * null terminator, end='\0'.
 *
 * For a full stream buffer with newline characters, end='\n' should be
 * used instead.
 */
core_inline_
std::string &uncomment(std::string &str, const char end)
{
    /*
     * Find the first character that matches comment '#'
     * and replace all characters by space until the end.
     */
    std::size_t beg = 0;
    while ((beg = str.find_first_of('#', beg)) != std::string::npos) {
        std::size_t tok = beg++;
        while ((tok < str.size()) && (str[tok] != end)) {
            str[tok++] = '\0';
        }
    }

    return str;
}

/** ---- C-string manipulation functions --------------------------------------
 *
 * ltrim
 * @brief Trim off left spaces from the string.
 *
 * Find the first non space character or stop at the end.
 * Ensure the value isspace takes is of type unsigned char.
 */
core_inline_
char *ltrim(char *str)
{
    /* Scan from beginning and find the first non space character */
    char *beg = str;
    while ((*beg != '\0') && std::isspace((unsigned char) *beg)) {
        beg++;
    }

    /* Shift chars in place if needed */
    char *dst = str;
    if (beg != dst) {
        while ((*dst++ = *beg++));    /* stop at first null terminator */
    }

    return str;
}

/**
 * rtrim
 * @brief Trim off right spaces from the string.
 *
 * Start from the last character in the string and replace
 * any space character by a null terminator.
 * Stop at beginning or at the first non space character.
 * Ensure the value isspace takes is of type unsigned char.
 */
core_inline_
char *rtrim(char *str)
{
    /* Scan from end to beg and replace space by null terminator */
    char *beg = str;
    char *end = str + std::strlen(str) - 1;
    while ((end >= beg) && std::isspace((unsigned char) *end)) {
        *end-- = '\0';
    }

    return str;
}

/**
 * trim
 * @brief Trim off left and right spaces from the string.
 */
core_inline_
char *trim(char *str)
{
    return ltrim(rtrim(str));
}

/**
 * replace
 * @brief Scan the string and replace any characters given by search by
 * the characters given by replace.
 */
core_inline_
char *replace(char *str, const char search, const char replace)
{
    /* Check the trivial case */
    if (search == replace) {
        return str;
    }

    /* Scan the string for search and replace */
    char *tok = str;
    while ((tok = std::strchr(tok, search)) != nullptr) {
        *tok++ = replace;
    }

    return str;
}

/**
 * uncomment
 * @brief Trim off string comments.
 *
 * Scan the string and remove all characters between the comment symbol '#'
 * and the end character.
 *
 * For a single line where the newline character has been replaced by the
 * null terminator, end='\0'.
 *
 * For a full stream buffer with newline characters, end='\n' should be
 * used instead.
 */
core_inline_
char *uncomment(char *str, const char end)
{
    /*
     * Find the first character that matches comment '#'
     * Replace all characters by space until the end
     */
    char *beg = str;
    while ((beg = std::strchr(beg, '#')) != nullptr) {
        char *tok = beg++;
        while ((*tok != '\0') && (*tok != end)) {
            *tok++ = '\0';
        }
    }

    return str;
}

/**
 * numcpy
 * @brief Copy the first num-1 characters of source word to destination str.
 *
 * If the source is shorter than num-1 characters, the destination is padded
 * with zeros until a total num characters have been written to it.
 *
 * Unlike strncpy, a null character is implicitly appended at the end of
 * destination if source is shorter than num.
 */
core_inline_
char *numcpy(char *str, const char *word, long num)
{
    /*
     * Copy the first num-1 characters up to the
     * source string length to the destination...
     */
    long len = std::strlen(word);
    char *dst = str;
    while ((dst-str < len) && (dst-str < num-1)) {
        *dst++ = *word++;
    }

    /*
     * ...and pad the remaining destination characters
     * with zeros until num characters have been written
     */
    while (dst-str < num) {
        *dst++ = '\0';
    }

    /* Return destination string */
    return str;
}

/** ---- String token functions -----------------------------------------------
 *
 * next_token
 * @brief Retrive the next token found in a string.
 *
 * Unlike strtok, this function does not modify the input string
 * and copies instead the next token found into the word argument.
 *
 * If word = nullptr, the copy is not performed.
 *
 * If the word length is shorter than the token length, only the
 * first num-1 characters are copied.
 *
 * A null terminator is implictly added at the end.
 *
 * The implicit delimiter separating the string tokens is white-space
 * character as given by the "C" locale:
 *
 * ' '  (0x20)    space(SPC)
 * '\t' (0x09)    horizontal tab(TAB)
 * '\n' (0x0a)    newline(LF)
 * '\v' (0x0b)    vertical tab(VT)
 * '\f' (0x0c)    feed(FF)
 * '\r' (0x0d)    carriage return (CR)
 *
 * The function returns a pointer to the next token. A null pointer
 * is returned if there are no tokens left in the substring or an
 * EOF is reached.
 */
core_inline_
char *next_token(const char *str)
{
    /* Token of the first non space character */
    char *beg = (char *) str;
    while ((*beg != '\0') && std::isspace((unsigned char) *beg)) {
        beg++;
    }
    if (*beg == '\0') {
        return nullptr;     /* reached end-of-string */
    }

    /* Token of the next space character */
    char *end = beg;
    while ((*end != '\0') && !std::isspace((unsigned char) *end)) {
        end++;
    }

    return end;
}

/**
 * count_tokens
 * @brief Count the number of tokens delimited by white space in a string.
 */
core_inline_
size_t count_tokens(char *str)
{
    size_t count = 0;
    char *cur = str;
    while ((cur = next_token(cur)) != nullptr) {
        ++count;
    }
    return count;
}

/**
 * split_tokens
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan first the string to count the total number of tokens and create
 * a new token structure with a maximum word length given by the argument
 * length.
 *
 * Scan the string again and store all the tokens found into the newly
 * created object.
 */
core_inline_
char **split_tokens(char *str, size_t *n_tokens)
{
    /* Do not split a null string */
    if (str == nullptr) {
        return nullptr;
    }

    /* Scan the string and count number of token words */
    *n_tokens = count_tokens(str);
    if (*n_tokens == 0) {
        return nullptr;
    }

    /* Create an array of strings to store all the tokens */
    char **tokens = (char **) calloc(*n_tokens, sizeof(char *));
    core_assert_(tokens != nullptr, "failed to alloc tokens");

    /* Rescan the string and store the token words */
    size_t ix = 0;
    char *end = str;
    char *beg = end;
    while ((end = next_token(beg)) != nullptr) {
        /* Create a new token word with num+1 chars including null terminator */
        int num = (end - beg) / sizeof(char);
        char *word = (char *) calloc(num+1, sizeof(char));
        core_assert_(word != nullptr, "failed to alloc word");

        /* Copy the token characters */
        char *src = beg;
        char *dst = word;
        while ((dst-word < num) && (src < end)) {
            *dst++ = *src++;
        }
        *dst = '\0';

        /* Store the token word */
        tokens[ix++] = word;

        /* Next token */
        beg = end;
    }

    return tokens;
}

/**
 * split_tokens
 *
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan the string first and count the total number of tokens.
 * If the string has no tokens, do not clear the current token set.
 * Otherwise, rescan the string and store each token sequentially.
 */
core_inline_
std::vector<std::string> split_tokens(char *str)
{
    std::vector<std::string> tokens;

    /* Do not split a null string */
    if (str == nullptr) {
        return tokens;
    }

    /* Scan the string and store the token words */
    char *end = str;
    char *beg = end;
    while ((end = next_token(beg)) != nullptr) {
        tokens.push_back(std::string(beg, end - beg));
        beg = end;
    }
    return tokens;
}

/** ---- String cast conversion functions -------------------------------------
 *
 * @brief Parse the string interpreting its content as a number.
 * The cast helper functions handle the string conversion to the
 * fundamental data types:
 *
 * (floating point types)
 * lond double, double, float
 *
 * (integer types, signed and unsigned)
 * long long, long, int, short
 *
 * The cast function names are defined using the OpenGL suffix
 * convention {ld, d, f, s,, i, l, ll, us, ui, ul, ull}.
 *
 * These functions handle success or failure reported by the
 * corresponding strto functions - endptr and errno.
 *
 * If the endptr is not nullptr, a pointer to the character after
 * the last used in the conversion is stored in the location
 * referenced by endptr.
 *
 * If the correct value would cause overflow, plus or minus HUGE_VAL,
 * HUGE_VALF, or HUGE_VALL is returned - according to the sign and type
 * of the return value; and ERANGE is stored in errno.
 *
 * If the correct value would cause underflow, zero is returned and
 * ERANGE is stored in errno.
 */

/**
 * castld
 * @brief Parse the string as a long double.
 */
core_inline_
long double castld(const char *str)
{
    errno = 0;
    char *endptr;
    long double x = std::strtold(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castd
 * @brief Parse the string as a double.
 */
core_inline_
double castd(const char *str)
{
    errno = 0;
    char *endptr;
    double x = std::strtod(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castf
 * @brief Parse the string as a float.
 */
core_inline_
float castf(const char *str)
{
    errno = 0;
    char *endptr;
    float x = std::strtof(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castll
 * @brief Parse the string as a long long integer.
 */
core_inline_
long long castll(const char *str)
{
    errno = 0;
    char *endptr;
    long long x = std::strtoll(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castl
 * @brief Parse the string as a long integer.
 */
core_inline_
long castl(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * casti
 * @brief Parse the string as an integer.
 */
core_inline_
int casti(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (LONG_MIN < INT_MIN || LONG_MAX > INT_MAX)
    /*
     * Ensure 'long' value is within 'int' range
     */
    if (x < INT_MIN || x > INT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (int) x;
}

/**
 * casts
 * @brief Parse the string as a short integer.
 */
core_inline_
short casts(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (LONG_MIN < SHRT_MIN || LONG_MAX > SHRT_MAX)
    /*
     * Ensure 'long' value is within 'short' range
     */
    if (x < SHRT_MIN || x > SHRT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (short) x;
}

/**
 * castull
 * @brief Parse the string as a unsigned long long integer.
 */
core_inline_
unsigned long long castull(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long long x = std::strtoull(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castul
 * @brief Parse the string as a unsigned long integer.
 */
core_inline_
unsigned long castul(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * castui
 * @brief Parse the string as a usigned integer.
 */
core_inline_
unsigned int castui(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (ULONG_MAX > UINT_MAX)
    /*
     * Ensure 'unsigned long' value is within 'unsigned int' range
     */
    if (x > UINT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (unsigned int) x;
}

/**
 * castus
 * @brief Parse the string as a unsigned sort integer.
 */
core_inline_
unsigned short castus(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (ULONG_MAX > USHRT_MAX)
    /*
     * Ensure 'unsigned long' value is within 'unsigned int' range
     */
    if (x > USHRT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (unsigned short) x;
}

/**
 * cast<T>
 *
 * @brief Templated string cast conversion function.
 *
 * It acts as an interface for the set of helper functions
 * that handle string casting into one of the fundamental
 * types.
 *
 * If none found, then it tries to create a string stream
 * and extract the formatted result.
 *
 * If no conversion can be performed, 0 is returned.
 *
 * @see
 * https://en.cppreference.com/w/cpp/language/types
 * https://en.cppreference.com/w/cpp/string/byte/strto[ld,d,f]
 * https://en.cppreference.com/w/cpp/string/byte/strto[ll,l,ull,ul] *
 */
template<typename T>
T cast(const char *str)
{
    /*
     * Ensure we have a valid string to cast
     */
    core_assert_(str != nullptr, "invalid string");

    /*
     * Check fundamental types
     */
    if (std::is_same<T, long double>::value) {
        return castld(str);
    }

    if (std::is_same<T, double>::value) {
        return castd(str);
    }

    if (std::is_same<T, float>::value) {
        return castf(str);
    }

    if (std::is_same<T, long long>::value) {
        return castll(str);
    }

    if (std::is_same<T, long>::value) {
        return castl(str);
    }

    if (std::is_same<T, int>::value) {
        return casti(str);
    }

    if (std::is_same<T, short>::value) {
        return casts(str);
    }

    if (std::is_same<T, char>::value) {
        return casts(str);
    }

    if (std::is_same<T, unsigned long long>::value) {
        return castull(str);
    }

    if (std::is_same<T, unsigned long>::value) {
        return castul(str);
    }

    if (std::is_same<T, unsigned int>::value) {
        return castui(str);
    }

    if (std::is_same<T, unsigned short>::value) {
        return castus(str);
    }

    if (std::is_same<T, unsigned char>::value) {
        return castus(str);
    }

    /*
     * Check fixed width integer types
     */
    if (std::is_same<T, int64_t>::value) {
        return castl(str);
    }

    if (std::is_same<T, int32_t>::value) {
        return castd(str);
    }

    if (std::is_same<T, int16_t>::value) {
        return casts(str);
    }

    if (std::is_same<T, int8_t>::value) {
        return casts(str);
    }

    if (std::is_same<T, uint64_t>::value) {
        return castul(str);
    }

    if (std::is_same<T, uint32_t>::value) {
        return castui(str);
    }

    if (std::is_same<T, uint16_t>::value) {
        return castus(str);
    }

    if (std::is_same<T, uint8_t>::value) {
        return castus(str);
    }

    /*
     * Otherwise, create a string stream and extract the formatted result.
     */
    std::istringstream is(str);
    T result;
    if (is >> result) {
        return result;
    }

    /*
     * No valid conversion.
     */
    return static_cast<T>(0);
}

/**
 * cast<T>
 *
 * @brief Templated string cast conversion function.
 * Overloaded function using std::string signature.
 */
template<typename T>
T cast(const std::string &str)
{
    return cast<T>(str.c_str());
}


/** ---- String format generation functions -----------------------------------
 *
 * format
 * @brief Generate a formatted string using a snprintf format style.
 *
 * @see
 * https://en.cppreference.com/w/cpp/io/c/fprintf
 * https://stackoverflow.com/questions/2342162
 */
template<typename... Args>
std::string format(const std::string& fmt, const Args&... args)
{
    /*
     * Calling std::snprintf with zero size and nullptr for buffer
     * is useful to determine the necessary buffer size to contain
     * the output.
     *
     * The std::snprintf function returns the number of characters
     * written if successful or a negative value if an error occurred.
     */
    int size = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    core_assert_(size >= 0, "error generating formatted string");

    /*
     * Create a buffer with size+1 to include all the
     * format characters plus the null terminator '\0'.
     */
    std::vector<char> buffer(size+1, '\0');

    /*
     * Write the formatted string to the newly created buffer.
     */
    core_assert_(
        std::snprintf(buffer.data(), size+1, fmt.c_str(), args...) == size,
        "error generating formatted string");

    /*
     * Return the formatted string without the null terminator '\0'.
     */
    return std::string(buffer.data(), size);
}

/**
 * read_format<T>
 * @brief Define the fscanf input format specifier for each fundamental
 * type or fixed width integer type.
 */
template<typename T>
std::string read_format(void)
{
    /*
     * Read format specifiers of fundamental data types.
     */
    const std::map<std::type_index,std::string> read_format_spec = {
        /*
         * Fundamental data types.
         */
        {std::type_index(typeid(long double)),       std::string("%Lg")},
        {std::type_index(typeid(double)),            std::string("%lg")},
        {std::type_index(typeid(float)),             std::string("%g")},
        {std::type_index(typeid(long long)),         std::string("%lld")},
        {std::type_index(typeid(long)),              std::string("%ld")},
        {std::type_index(typeid(int)),               std::string("%d")},
        {std::type_index(typeid(short)),             std::string("%hd")},
        {std::type_index(typeid(char)),              std::string("%hhd")},
        {std::type_index(typeid(unsigned long long)),std::string("%llu")},
        {std::type_index(typeid(unsigned long)),     std::string("%lu")},
        {std::type_index(typeid(unsigned int)),      std::string("%u")},
        {std::type_index(typeid(unsigned short)),    std::string("%hu")},
        {std::type_index(typeid(unsigned char)),     std::string("%hhu")},
        /*
         * Fixed width integer types
         */
        {std::type_index(typeid(int64_t)),           std::string("%lld")},
        {std::type_index(typeid(int32_t)),           std::string("%d")},
        {std::type_index(typeid(int16_t)),           std::string("%hd")},
        {std::type_index(typeid(int8_t)),            std::string("%hhd")},
        {std::type_index(typeid(uint64_t)),          std::string("%llu")},
        {std::type_index(typeid(uint32_t)),          std::string("%u")},
        {std::type_index(typeid(uint16_t)),          std::string("%hu")},
        {std::type_index(typeid(uint8_t)),           std::string("%hhu")},
    };

    /*
     * Find the corresponding type in the read type map.
     */
    std::map<std::type_index,std::string>::const_iterator it =
        read_format_spec.find(std::type_index(typeid(T)));

    core_assert_(it != read_format_spec.end(), "invalid parameter type");

    return it->second;
}

/**
 * write_format<T>
 * @brief Define the fprintf output format specifier for each fundamental
 * type or fixed width integer type.
 */
template<typename T>
std::string write_format(void)
{
    /*
     * Write format specifiers of fundamental data types.
     */
    const std::map<std::type_index,std::string> write_format_spec = {
        /*
         * Fundamental data types.
         */
        {std::type_index(typeid(long double)),       std::string("%.15Le")},
        {std::type_index(typeid(double)),            std::string("%.15e")},
        {std::type_index(typeid(float)),             std::string("%.15e")},
        {std::type_index(typeid(long long)),         std::string("%lld")},
        {std::type_index(typeid(long)),              std::string("%ld")},
        {std::type_index(typeid(int)),               std::string("%d")},
        {std::type_index(typeid(short)),             std::string("%hd")},
        {std::type_index(typeid(char)),              std::string("%hhd")},
        {std::type_index(typeid(unsigned long long)),std::string("%llu")},
        {std::type_index(typeid(unsigned long)),     std::string("%lu")},
        {std::type_index(typeid(unsigned int)),      std::string("%u")},
        {std::type_index(typeid(unsigned short)),    std::string("%hu")},
        {std::type_index(typeid(unsigned char)),     std::string("%hhu")},
        /*
         * Fixed width integer types
         */
        {std::type_index(typeid(int64_t)),           std::string("%lld")},
        {std::type_index(typeid(int32_t)),           std::string("%d")},
        {std::type_index(typeid(int16_t)),           std::string("%hd")},
        {std::type_index(typeid(int8_t)),            std::string("%hhd")},
        {std::type_index(typeid(uint64_t)),          std::string("%llu")},
        {std::type_index(typeid(uint32_t)),          std::string("%u")},
        {std::type_index(typeid(uint16_t)),          std::string("%hu")},
        {std::type_index(typeid(uint8_t)),           std::string("%hhu")},
    };

    /*
     * Find the corresponding type in the write type map.
     */
    std::map<std::type_index,std::string>::const_iterator it =
        write_format_spec.find(std::type_index(typeid(T)));

    core_assert_(it != write_format_spec.end(), "invalid parameter type");

    return it->second;
}

}  /* namespace str */
}  /* namespace core */

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
__END_DECLS

#endif /* CORE_STRING_H_ */
