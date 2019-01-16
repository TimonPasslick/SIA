/*
 * array_lib.h - Improves Serial.print. 
 * Created by Timon Paßlick, October 31, 2018.
 * No license, please use this code however you want. No guarantees too, though.
 */

#ifdef DEBUG
constexpr bool debug{true};
#else
constexpr bool debug{false};
#endif

template <typename T> struct binary      { const T value; };
template <typename T> struct octal       { const T value; };
template <typename T> struct decimal     { const T value; };
template <typename T> struct hexadecimal { const T value; };

template <typename T> inline binary     <T> bin(const T& value) { return binary     <T>{value}; }
template <typename T> inline octal      <T> oct(const T& value) { return octal      <T>{value}; }
template <typename T> inline decimal    <T> dec(const T& value) { return decimal    <T>{value}; }
template <typename T> inline hexadecimal<T> hex(const T& value) { return hexadecimal<T>{value}; }

template <typename T>
void inline debugPrint(const binary<T>& encoded) {
  if (debug) {
    Serial.print(encoded.value, BIN);
  }
}

template <typename T>
void inline debugPrint(const octal<T>& encoded) {
  if (debug) {
    Serial.print(encoded.value, OCT);
  }
}

template <typename T>
void inline debugPrint(const decimal<T>& encoded) {
  if (debug) {
    Serial.print(encoded.value, DEC);
  }
}

template <typename T>
void inline debugPrint(const hexadecimal<T>& encoded) {
  if (debug) {
    Serial.print(encoded.value, HEX);
  }
}

template <typename T>
void inline debugPrint(const T& value) {
  if (debug) {
    Serial.print(value);
  }
}

template <typename H, typename ... T>
void inline debugPrint(const H& head, const T&... tail) {
  debugPrint(head);
  debugPrint(tail...);
}

template <typename ... T>
void inline debugPrintLine(const T&... values) {
  debugPrint(values..., '\n');
}
