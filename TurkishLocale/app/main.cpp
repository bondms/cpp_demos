// Copyright 2021 Mark Bond

#include <cstdlib>
#include <iostream>
#include <locale>

namespace {

// Cannot use non-wide characters since toupper works on single characters
// and UTF-8 requires two chars to represent a single character for the
// special characters.
constexpr auto dotted_lowercase{'i'};
// constexpr auto dotted_uppercase{ '\xc4\xb0' };
// constexpr auto dotless_lowercase{ '\xc4\xb1' };
constexpr auto dotless_uppercase{'I'};

constexpr auto dotted_lowercase_w{L'i'};
constexpr auto dotted_uppercase_w{L'\u0130'};
constexpr auto dotless_lowercase_w{L'\u0131'};
constexpr auto dotless_uppercase_w{L'I'};

auto as_string(wchar_t ch) {
  switch (ch) {
  case dotted_lowercase_w:
    return "Dotted lowercase";
  case dotted_uppercase_w:
    return "Dotted uppercase";
  case dotless_lowercase_w:
    return "Dotless lowercase";
  case dotless_uppercase_w:
    return "Dotless uppercase";
  }
  return "Unrecognised";
}

constexpr wchar_t test_characters[]{
    dotted_lowercase_w,
    dotted_uppercase_w,
    dotless_lowercase_w,
    dotless_uppercase_w,
};

constexpr const char *locale_names[]{
    "C",
    "en_GB.utf8",
    "tr_TR.utf8",
};

} // namespace

int main() {
  try {
    std::cout << "Current C locale\n";
    for (const auto from : test_characters) {
      const auto to{
          static_cast<wchar_t>(std::towupper(static_cast<wint_t>(from)))};
      std::cout << "From " << as_string(from) << " to " << as_string(to)
                << '\n';
    }

    for (const auto &locale_name : locale_names) {
      std::cout << "Locale set globally: " << locale_name << '\n';
      std::setlocale(LC_ALL, locale_name);
      for (const auto from : test_characters) {
        const auto to{
            static_cast<wchar_t>(std::towupper(static_cast<wint_t>(from)))};
        std::cout << "From " << as_string(from) << " to " << as_string(to)
                  << '\n';
      }
    }

    for (const auto &locale_name : locale_names) {
      std::cout << "Locale specified in toupper: " << locale_name << '\n';
      std::locale locale{locale_name};

      for (const auto from : test_characters) {
        const auto to{std::toupper(from, locale)};
        std::cout << "From " << as_string(from) << " to " << as_string(to)
                  << '\n';
      }
    }

    std::cout << "Success" << std::endl;
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  std::cout << "Failure" << std::endl;
  return EXIT_FAILURE;
}
