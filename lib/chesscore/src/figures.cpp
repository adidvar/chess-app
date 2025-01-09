#include "figures.hpp"

#include <algorithm>
#include <sstream>
#include <tuple>

const static char codes[7] = {'\0', 'p', 'k', 'b', 'r', 'q', '\0'};

Turn::Turn(std::string_view chess_format)
{
    if (chess_format.size() != 5 && chess_format.size() != 4) {
        *this = {};
        return;
    }

    auto pos_from = Position(chess_format.substr(0, 2));
    auto pos_to = Position(chess_format.substr(2, 2));

    if (!pos_from.isValid() || !pos_to.isValid()) {
        *this = {};
        return;
    }

    m_from = pos_from.index();
    m_to = pos_to.index();

    if (chess_format.size() == 5) {
        auto index = std::find(codes, codes + 7, chess_format[4]);
        if (index == codes + 7) {
            *this = {};
            return;
        }
        m_figure = index - codes;
    }
}

std::string Turn::toString() const
{
    if (!isValid()) {
        return "0000";
    } else if (m_figure == Figure::Empty) {
        std::stringstream ss;
        ss << Position(m_from).toString() << Position(m_to).toString();
        return ss.str();
    } else {
        std::stringstream ss;
        ss << Position(m_from).toString() << Position(m_to).toString() << codes[(size_t) m_figure];
        return ss.str();
    }
}
