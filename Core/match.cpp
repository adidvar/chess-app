#include "match.hpp"

#include <map>
#include <algorithm>
#include <iostream>

/*
void Match::update_state_and_hash_turns()
{
    this->current_turns = TurnGenerate(boards_history.back(),boards_history.back().CurrentColor());
}

bool Match::Push(Position turn)
{
    if(!TurnTest(boards_history.back(),turn))
        return false;

    this->turns_history.push_back(turn);
    Board copy = boards_history.back();
    ExecuteTurn(copy,turn);
    this->boards_history.push_back(std::move(copy));

    update_state_and_hash_turns();
    return true;
}

*/

Match::Match()
{

}

Match::~Match()
{

}

bool Match::Push(Turn turn)
{
    if(result_ != Unknown)
        return false;
    if(board_.ExecuteTurn(turn))
    {
        turns_.push_back(turn);

        if(board_.WhiteWin())
            result_ = WhiteWin;
        else if(board_.BlackWin())
            result_ = BlackWin;
        else if(board_.Tie())
            result_ = Tie;

        return true;
    }
    return false;
}

bool Match::Surrender(Color color)
{
    if(result_ == Unknown) {
        if(color == Color::kWhite)
            result_ = BlackWin;
        else
            result_ = WhiteWin;
        return true;
    }
    return false;
}

void Match::SetTurns(const std::vector<Turn> &turns)
{
    turns_ = turns;
}

void Match::SetBoard(const BitBoard &board)
{

}

bool Match::HaveOpTag(const std::string& name) const
{
    return optags_.count(name) != 0;
}

std::string Match::GetOpTagValue(const std::string& name) const
{
    return optags_.at(name);
}

void Match::AddOpTag(const std::string &name, const std::string &value)
{
    optags_[name]  = value;
}


std::string Match::Event() const
{
    return event_;
}

void Match::setEvent(const std::string &newEvent)
{
    event_ = newEvent;
}

std::string Match::Site() const
{
    return site_;
}

void Match::setSite(const std::string &newSite)
{
    site_ = newSite;
}

std::string Match::Date() const
{
   return date_;
}

void Match::setDate(const std::string &newDate)
{
   date_ = newDate;
}

std::string Match::Round() const
{
    return round_;
}

void Match::setRound(const std::string &newRound)
{
    round_ = newRound;
}

std::string Match::White() const
{
    return white_;
}

void Match::setWhite(const std::string &newWhite)
{
    white_ = newWhite;
}

std::string Match::Black() const
{
    return black_;
}

void Match::setBlack(const std::string &newBlack)
{
   black_ = newBlack;
}

Match::Result_t Match::GetResult() const
{
    return result_;
}

void Match::SetResult(Result_t value)
{
    result_ = value;
}

using Tags =  std::unordered_map<std::string,std::string>;

Tags ParseTags(std::string_view data){
    Tags tags;

    size_t index = 0;

    for(;;){
        auto block_begin = data.find('[',index);
        auto block_end = data.find(']',block_begin);

        if(block_begin == std::string::npos || block_end == std::string::npos)break;

        auto sub_block = data.substr(block_begin,block_end-block_begin+1);

        auto space = sub_block.find(' ',0);
        assert(space != std::string::npos);
        auto tag = sub_block.substr(1,space-1);
        auto value = sub_block.substr(space+2,sub_block.size()-space-4);

        tags[std::string(tag)] = std::string(value);

        index = block_end;
    }

    return tags;
}

Turn ParseTurn(std::string_view data, const BitBoard& board){

    const static std::map<char,Figure> bindf{
        {'N',Figure::kKnight},
        {'B',Figure::kBishop} ,
        {'R',Figure::kRook} ,
        {'Q',Figure::kQueen} ,
        {'K',Figure::kKing}
    };

    const static std::map<char,int> bindpx{
        {'a',0},
        {'b',1},
        {'c',2},
        {'d',3},
        {'e',4},
        {'f',5},
        {'g',6},
        {'h',7}
    };

    const static std::map<char,int> bindpy{
        {'1',7},
        {'2',6},
        {'3',5},
        {'4',4},
        {'5',3},
        {'6',2},
        {'7',1},
        {'8',0}
    };

    int from_x = -1;
    int from_y = -1;

    Figure figure = Figure::kPawn;
    Figure figure_transform = Figure::kEmpty;

    if(data.size()!=0 && data.back() == '+'){
        data.remove_suffix(1);
    }

    if(data == "O-O")
        return Turn::GetShortCastling(board.CurrentColor());
    if(data == "O-O-O")
        return Turn::GetLongCastling(board.CurrentColor());

    auto pturns = board.GenerateTurns();

    if(data.size()>=2 && data[data.size()-2] == '='){
        figure_transform = bindf.at(data.back());
        data.remove_suffix(2);
    }

    auto pos = Position::FromString(data.substr(data.size()-2));
    data.remove_suffix(2);


    if(data.size()!=0 && data.back() == 'x'){
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindpy.count(data.back()) == 1){
        from_y = bindpy.at(data.back());
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindpx.count(data.back()) == 1){
        from_x = bindpx.at(data.back());
        data.remove_suffix(1);
    }

    if(data.size()!=0 && bindf.count(data.back()) == 1){
        figure = bindf.at(data.back());
        data.remove_suffix(1);
    }

    auto predicate = [&](Turn turn)
    {
        return (turn.to() == pos) && board.Test(figure,turn.from())
                && (from_x == -1 || turn.from().x() == from_x)
                && (from_y == -1 || turn.from().y() == from_y)
                && (figure_transform == turn.figure());
    };

    auto result = std::find_if(pturns.begin(),pturns.end(),predicate);
    auto count = std::count_if(pturns.begin(),pturns.end(),predicate);

    if(count == 1)
        return *result;
    else
        return Turn();
}

std::pair<std::vector<Turn>,BitBoard> ParseTurns(std::string_view data, BitBoard start_pos){
    size_t index = 0;

    std::vector<size_t> split_p;
    std::vector<std::string_view> turns_pre;
    std::vector<std::string_view> turns;
    std::vector<Turn> result;

    split_p.push_back(0);

    for(size_t pos = data.find(".", 0); pos != std::string_view::npos; pos = data.find(".",pos+1))
        split_p.push_back(pos+1);

    for(size_t pos = data.find("\n", 0); pos != std::string_view::npos; pos = data.find("\n",pos+1))
        split_p.push_back(pos+1);

    for(size_t pos = data.find(" ", 0); pos != std::string_view::npos; pos = data.find(" ",pos+1))
        split_p.push_back(pos+1);

    std::sort(split_p.begin(),split_p.end());

    for(size_t i = 0 ; i < split_p.size() - 1 ; i++)
        turns_pre.push_back(data.substr(split_p[i],split_p[i+1]-split_p[i]-1));
    turns_pre.pop_back();

    auto match_result = data.substr(split_p.back());
    for(size_t i = 0 ; i < turns_pre.size(); i++)
        if(i%3!=0)
            turns.push_back(turns_pre[i]);

    BitBoard board(start_pos);
    for(auto turn_data : turns){
        auto turn  = ParseTurn(turn_data, board);
        if(!board.ExecuteTurn(turn))
            exit(-1);
        else
            result.push_back(turn);
    }
    return {result,board};
}

std::string_view delim = "\n\n";

std::string_view ReadBlock(std::string_view data, size_t & index)
{
    while(data[index] == '\n')
        index++;


    auto block_index = data.find( delim, index);
    std::string_view block_text;
    if(block_index != std::string_view::npos)
        block_text = data.substr( index, block_index - index);
    else
        block_text = data.substr( index);

    while(block_text.back() == '\n')
        block_text.remove_suffix(1);

    if(block_index != std::string_view::npos)
        index = block_index + delim.size();
    else
        index = std::string_view::npos;

    return block_text;
}

Match ReadMatch(std::string_view data, size_t & index){

    auto header = ReadBlock(data,index);
    auto content = ReadBlock(data,index);

    auto tags = ParseTags(header);
    auto turns = ParseTurns(content,BitBoard());

    Match match;

    match.SetTurns(turns.first);
    match.SetBoard(turns.second);
    match.setEvent(tags.at("Event"));
    match.setSite(tags.at("Site"));
    match.setDate(tags.at("Date"));
    match.setRound(tags.at("Round"));
    match.setWhite(tags.at("White"));
    match.setBlack(tags.at("Black"));

    if(tags.at("Result") == "1-0")
        match.SetResult(Match::WhiteWin);
    else if(tags.at("Result") == "0-1")
        match.SetResult(Match::BlackWin);
    else if(tags.at("Result") == "1/2-1/2")
        match.SetResult(Match::Tie);
    else
        match.SetResult(Match::Unknown);

    static size_t i = 1 ;
    std::cout << i << std::endl;
    i++;

    return match;
}

std::vector<Match> Match::LoadFromPGN(std::string text)
{
    std::vector<Match> matches;
    size_t len = 0;
    while(len != std::string_view::npos)
        matches.push_back(ReadMatch(text,len));
    return matches;
}
