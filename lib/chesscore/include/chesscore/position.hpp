#pragma once

#include <cassert>
#include <cinttypes>
#include <string>

/*
+--+--+--+--+--+--+--+--+
| 0| 1| 2| 3| 4| 5| 6| 7|
+--+--+--+--+--+--+--+--+
| 8| 9|10|11|12|13|14|15|
+--+--+--+--+--+--+--+--+
|16|17|18|19|20|21|22|23|
+--+--+--+--+--+--+--+--+
|24|25|26|27|28|29|30|31|
+--+--+--+--+--+--+--+--+
|32|33|34|35|36|37|38|39|
+--+--+--+--+--+--+--+--+
|40|41|42|43|44|45|46|47|
+--+--+--+--+--+--+--+--+
|48|49|50|51|52|53|54|55|
+--+--+--+--+--+--+--+--+
|56|57|58|59|60|61|62|63|
+--+--+--+--+--+--+--+--+
*/

/**
 * @brief The Position class describes a chess position
 */
class Position {
 public:
     constexpr Position(uint8_t x, uint8_t y) noexcept
     {
         m_index = y * 8 + x;
     }

     constexpr Position() noexcept
         : m_index(k_invalid_position)
     {}

     constexpr Position(uint8_t index) noexcept
         : m_index(index)
     {}
     constexpr explicit Position(std::string_view string)
     {
         if (string.size() == 2 && string[0] >= 'a' && string[0] <= 'h' && string[1] >= '1'
             && string[1] <= '8')
             *this = Position(string[0] - 'a', '8' - string[1]);
         else
             *this = {};
     }

     [[nodiscard]] constexpr bool isValid() const noexcept { return m_index < k_invalid_position; }

     [[nodiscard]] constexpr uint8_t index() const noexcept { return m_index; }

     [[nodiscard]] constexpr uint8_t x() const noexcept
     {
         assert(isValid());
         return m_index % 8;
     }
     [[nodiscard]] constexpr uint8_t y() const noexcept
     {
         assert(isValid());
         return m_index / 8;
     }

     [[nodiscard]] constexpr Position rotate() const noexcept
     {
         assert(isValid());
         return Position(63 - m_index);
     }

     [[nodiscard]] std::string toString() const
     {
         if (!isValid())
             return "--";
         std::string str = "00";
         str[0] = static_cast<char>('a' + x());
         str[1] = static_cast<char>('8' - y());
         return str;
     }

     bool operator==(const Position& pos) const noexcept { return m_index == pos.m_index; }
     bool operator!=(const Position& pos) const noexcept { return m_index != pos.m_index; }
     bool operator<(const Position& pos) const noexcept { return m_index < pos.m_index; }

 private:
     static constexpr uint8_t k_invalid_position = 64;
     uint8_t m_index;
};

constexpr Position operator"" _p(const char* str, std::size_t len)
{
    return Position(std::string_view(str, len));
}

constexpr uint8_t operator"" _pv(const char* str, std::size_t len)
{
    return Position(std::string_view(str, len)).index();
}
