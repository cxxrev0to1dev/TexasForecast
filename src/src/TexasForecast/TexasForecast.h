#ifndef TEXASFORECAST_H_
#define TEXASFORECAST_H_

#include <string>
#include <vector>

//同花大顺（皇家同花顺，Royal Straight Flush）
//同花顺（Straight Flush）
//四条（铁支，Four of a kind）
//葫芦（满堂红，Full house）
//同花（Flush）
//顺子（Straight）
//三条（Three of a kind）
//两对（Two pair）
//一对（One pair）
//散牌（高牌, High card，No - pair，Zitch）

enum class PokerTable
{
  kSpade3 = 1,
  kSpade4,
  kSpade5,
  kSpade6,
  kSpade7,
  kSpade8,
  kSpade9,
  kSpade10,
  kSpadeJ,
  kSpadeQ,
  kSpadeK,
  kSpadeA,
  kSpade2,
  kHeart3,
  kHeart4,
  kHeart5,
  kHeart6,
  kHeart7,
  kHeart8,
  kHeart9,
  kHeart10,
  kHeartJ,
  kHeartQ,
  kHeartK,
  kHeartA,
  kHeart2,
  kClub3,
  kClub4,
  kClub5,
  kClub6,
  kClub7,
  kClub8,
  kClub9,
  kClub10,
  kClubJ,
  kClubQ,
  kClubK,
  kClubA,
  kClub2,
  kDiamond3,
  kDiamond4,
  kDiamond5,
  kDiamond6,
  kDiamond7,
  kDiamond8,
  kDiamond9,
  kDiamond10,
  kDiamondJ,
  kDiamondQ,
  kDiamondK,
  kDiamondA,
  kDiamond2
};

static const wchar_t kReverseSuitIndex[] = { L's', L'h', L'c', L'd', 0 };
static const wchar_t kSpade = L's';
static const wchar_t kHeart = L'h';
static const wchar_t kClub = L'c';
static const wchar_t kDiamond = L'd';
static const wchar_t kCardTable[] = L"3456789TJQKA2";
static const int kMaxStraightGap = 4;
static const int kMinStraightCardNum = 3;
static const int kMinFlushCardNum = 3;


class TexasCard
{
public:
  TexasCard() {
    suit.resize(0);
    value.resize(0);
    valude_int = 0;
    same_color_value = 0;
  }
  TexasCard(const wchar_t a, const wchar_t b, int c):TexasCard(){
    suit.append(1, a);
    value.append(1,b);
    valude_int = c;
    same_color_value = c - CalcSameColorValue(a);
  }
  TexasCard(const wchar_t a, const wchar_t* b, int c) :TexasCard() {
    suit.append(1, a);
    value.append(b);
    valude_int = c;
    same_color_value = c - CalcSameColorValue(a);
  }
  virtual ~TexasCard() {
    suit.resize(0);
    value.resize(0);
  }
  std::wstring GetSuit() const{
    return suit;
  }
  std::wstring GetValue() const {
    return value;
  }
  int GetValueInt() const {
    return valude_int;
  }
  int SameColorValue() const {
    return same_color_value;
  }
private:
  int CalcSameColorValue(wchar_t a) {
    if (a == kSpade){
      return 0;
    }
    else if (a == kHeart){
      return 13;
    }
    else if (a == kClub) {
      return (13 * 2);
    }
    else if (a == kDiamond) {
      return (13 * 3);
    }
    return 0;
  }
  std::wstring suit;
  std::wstring value;
  int valude_int;
  int same_color_value;

};

class TexasForecast
{
public:
  TexasForecast();
  ~TexasForecast();
  void SetFlop(std::vector<PokerTable> flop_poker);
  bool SetFlop(PokerTable flop_poker);
  bool SetTurn(PokerTable turn_poker);
  bool SetRiver(PokerTable river_poker);
  bool IsSetFlop() const{
    return is_set_flop_;
  }
  bool IsSetTurn() const {
    return is_set_turn_;
  }
  bool IsSetRiver() const {
    return is_set_river_;
  }
  void Reset();
  bool IsRefreshUI();
  void Calc();
private:
  bool CalcRoyalStraightFlush();
  bool CalcStraightFlush();
  bool CalcFourOfAKind();
  bool CalcFlush();
  bool CalcStraight();
  int CalcPairNum();
  bool CalcFullHouse();
  size_t CalcThreeOfAKind();
  size_t CalcTwoPair();
  size_t CalcOnePair();
  void BubbleSort(std::vector<TexasCard>& in_out);
  void ConversionPokerTypeToTexasCard(std::vector<PokerTable>& pokers, std::vector<TexasCard>& out);
  bool IsFlush(wchar_t flush_card);
  bool IsStraight(std::vector<TexasCard>& texas_showdown);
  std::vector<PokerTable> user_poker_;
  std::vector<TexasCard> texas_showdown_;
  std::vector<TexasCard> texas_cards_;
  bool is_set_flop_;
  bool is_set_turn_;
  bool is_set_river_;
};

#endif