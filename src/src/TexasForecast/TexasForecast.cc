#include "TexasForecast.h"
#include <algorithm>
#include <xutility>
#include <iosfwd>
#include <sstream>
#include <functional>
#include <windows.h>

TexasForecast::TexasForecast():is_set_flop_(false), is_set_turn_(false), is_set_river_(false){
  user_poker_.resize(0);
  texas_showdown_.resize(0);
  texas_cards_.resize(0);
  std::vector<PokerTable> init_pokers;
  for (int i = 1;i <= 52;i++) {
    init_pokers.push_back((PokerTable)i);
  }
  ConversionPokerTypeToTexasCard(init_pokers, texas_cards_);
}
TexasForecast::~TexasForecast(){
  user_poker_.resize(0);
  texas_showdown_.resize(0);
  texas_cards_.resize(0);
}
void TexasForecast::SetFlop(std::vector<PokerTable> flop_poker) {
  user_poker_ = flop_poker;
}
bool TexasForecast::SetFlop(PokerTable flop_poker) {
  if (std::find(user_poker_.begin(), user_poker_.end(), flop_poker) != user_poker_.end())
    return false;
  if (!is_set_flop_&&user_poker_.size() <= 3) {
    user_poker_.push_back(flop_poker);
    if (user_poker_.size()==3){
      is_set_flop_ = true;
      std::sort(user_poker_.begin(), user_poker_.end());
    }
    return true;
  }
  return false;
}
bool TexasForecast::SetTurn(PokerTable turn_poker) {
  if (std::find(user_poker_.begin(), user_poker_.end(), turn_poker) != user_poker_.end())
    return false;
  if (!is_set_turn_&&user_poker_.size() == 3) {
    user_poker_.push_back(turn_poker);
    is_set_turn_ = true;
    std::sort(user_poker_.begin(), user_poker_.end());
    return true;
  }
  return false;
}
bool TexasForecast::SetRiver(PokerTable river_poker) {
  if (std::find(user_poker_.begin(), user_poker_.end(), river_poker) != user_poker_.end())
    return false;
  if (!is_set_river_&&user_poker_.size() == 4) {
    user_poker_.push_back(river_poker);
    is_set_river_ = true;
    std::sort(user_poker_.begin(), user_poker_.end());
    return true;
  }
  return false;
}
void TexasForecast::Reset() {
  if (IsSetFlop() && IsSetTurn() && IsSetRiver()) {
    is_set_flop_ = false;
    is_set_turn_ = false;
    is_set_river_ = false;
    user_poker_.resize(0);
  }
}
bool TexasForecast::IsRefreshUI() {
  if (IsSetFlop() && IsSetTurn() && IsSetRiver()) {
    return true;
  }
  return false;
}
void TexasForecast::Calc() {
  std::function<const wchar_t*(bool)> YesOrNo = [](bool status) ->const wchar_t*{
    if (status){
      return L"Yes";
    }
    else {
      return L"No";
    }
  };
  std::wstringstream show_msg;
  ConversionPokerTypeToTexasCard(user_poker_, texas_showdown_);
  BubbleSort(texas_showdown_);
  bool is_royal_straight_flush = CalcRoyalStraightFlush();
  bool is_straight_flush = CalcStraightFlush();
  bool is_flush = CalcFlush();
  bool iss_straight = is_straight_flush ? false : CalcStraight();
  show_msg << L"---------机率最小的牌型---------" << std::endl;
  show_msg << L"皇家同花顺:" << YesOrNo(is_royal_straight_flush) << std::endl;
  show_msg << L"同花顺:" << YesOrNo(is_straight_flush) << std::endl;
  show_msg << L"四条:" << YesOrNo(CalcFourOfAKind()) << std::endl;
  show_msg << L"---------机率中上的牌型---------" << std::endl;
  show_msg << L"同花:" << YesOrNo(is_flush) << std::endl;
  show_msg << L"顺子:" << YesOrNo(iss_straight) << std::endl;
  show_msg << L"---------机率一般的牌型---------" << std::endl;
  show_msg << L"葫芦:" << YesOrNo(CalcFullHouse()) << std::endl;
  show_msg << L"三条:" << std::to_wstring(CalcThreeOfAKind()) << L"个可能" << std::endl;
  show_msg << L"---------机率最大的牌型---------" << std::endl;
  show_msg << L"两对:" << std::to_wstring(CalcTwoPair()) << L"种方案" << std::endl;
  show_msg << L"一对:" << std::to_wstring(CalcOnePair()) << L"种方案" <<std::endl;
  MessageBoxW(GetActiveWindow(), show_msg.str().c_str(), L"OK", MB_OK);
  return;
}
bool TexasForecast::CalcRoyalStraightFlush() {
  if(!IsFlush(kSpade) && !IsFlush(kHeart) && !IsFlush(kClub) && !IsFlush(kDiamond)){
    return false;
  }
  std::wstring value;
  std::vector <std::wstring> card_colour;
  std::vector<TexasCard>::iterator it;
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  for (it = texas_showdown.begin(); it != texas_showdown.end(); it++){
    if (value.size()){
      value.append(L",");
    }
    if (it->GetValue() == L"10" || 
      it->GetValue() == L"J" || 
      it->GetValue() == L"Q" || 
      it->GetValue() == L"K" || 
      it->GetValue() == L"A"){
      value.append(it->GetValue());
      value.append(it->GetSuit());
      card_colour.push_back(it->GetSuit());
    }
    else {
      texas_showdown.erase(it);
      if (texas_showdown.size() < kMinStraightCardNum){
        value.resize(0);
        break;
      }
      it = texas_showdown.begin();
    }
  }
  if (value.size()){
    std::wstring t;
    t.append(1, kSpade);
    size_t sc = std::count_if(card_colour.begin(), card_colour.end(), [&t](const std::wstring& v) {return v == t;});
    t.resize(0);
    t.append(1, kHeart);
    size_t hc = std::count_if(card_colour.begin(), card_colour.end(), [&t](const std::wstring& v) {return v == t;});
    t.resize(0);
    t.append(1, kClub);
    size_t cc = std::count_if(card_colour.begin(), card_colour.end(), [&t](const std::wstring& v) {return v == t;});
    t.resize(0);
    t.append(1, kDiamond);
    size_t dc = std::count_if(card_colour.begin(), card_colour.end(), [&t](const std::wstring& v) {return v == t;});
    return (sc >= 3 || hc >= 3 || cc >= 3 || dc >= 3);
  }
  return false;
}
bool TexasForecast::CalcStraightFlush() {
  if (!IsFlush(kSpade) && !IsFlush(kHeart) && !IsFlush(kClub) && !IsFlush(kDiamond)) {
    return false;
  }
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  std::vector<TexasCard>::iterator it1 = texas_showdown.begin();
  int ispade = 0;
  int iheart = 0;
  int iclub = 0;
  int idiamond = 0;
  while (it1 < texas_showdown.end()){
    if (it1->GetSuit()[0]==kSpade){
      ispade++;
    }
    if (it1->GetSuit()[0] == kHeart) {
      iheart++;
    }
    if (it1->GetSuit()[0] == kClub) {
      iclub++;
    }
    if (it1->GetSuit()[0] == kDiamond) {
      idiamond++;
    }
    if(it1->GetValue()==L"A"){
      texas_showdown.erase(it1);
      it1 = texas_showdown.begin();
    }
    it1++;
  }
  it1 = texas_showdown.begin();
  while (it1 < texas_showdown.end()) {
    if (ispade<3&&it1->GetSuit()[0] == kSpade) {
      texas_showdown.erase(it1);
      it1 = texas_showdown.begin();
    }
    if (iheart<3&&it1->GetSuit()[0] == kHeart) {
      texas_showdown.erase(it1);
      it1 = texas_showdown.begin();
    }
    if (iclub<3&&it1->GetSuit()[0] == kClub) {
      texas_showdown.erase(it1);
      it1 = texas_showdown.begin();
    }
    if (idiamond<3&&it1->GetSuit()[0] == kDiamond) {
      texas_showdown.erase(it1);
      it1 = texas_showdown.begin();
    }
    it1++;
  }
  if (!IsStraight(texas_showdown)) {
    return false;
  }
  return true;
}
bool TexasForecast::CalcFourOfAKind() {
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  std::vector<TexasCard>::iterator it1 = texas_showdown.begin();
  std::vector<TexasCard>::iterator it2;
  while(it1<texas_showdown.end()){
    it2 = texas_showdown.begin();
    while (it2<texas_showdown.end()){
      if(it2->GetSuit()!=it1->GetSuit()&&it2->GetValue()==it1->GetValue()){
        return true;
      }
      it2++;
    }
    it1++;
  }
  return false;
}
bool TexasForecast::CalcFlush() {
  if (!IsFlush(kSpade) && !IsFlush(kHeart) && !IsFlush(kClub) && !IsFlush(kDiamond)) {
    return false;
  }
  return true;
}
bool TexasForecast::CalcStraight() {
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  if (!IsStraight(texas_showdown)) {
    return false;
  }
  return true;
}
int TexasForecast::CalcPairNum() {
  int pair_num = 0;
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  std::vector<TexasCard>::iterator it1 = texas_showdown.begin();
  std::vector<TexasCard>::iterator it2;
  while (it1 < texas_showdown.end()) {
    it2 = texas_showdown.begin();
    while (it2 < texas_showdown.end()) {
      if (it2->GetSuit() != it1->GetSuit() && it2->GetValue() == it1->GetValue()) {
        pair_num++;
        texas_showdown.erase(it1);
        it1 = texas_showdown.begin();
        it2 = texas_showdown.begin();
      }
      it2++;
    }
    it1++;
  }
  return pair_num;
}
bool TexasForecast::CalcFullHouse() {
  return (CalcPairNum() >= 1);
}
size_t TexasForecast::CalcThreeOfAKind() {
  size_t pair_num = CalcPairNum();
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  std::vector<TexasCard>::iterator it1 = texas_showdown.begin();
  std::vector<TexasCard>::iterator it2;
  while (it1 < texas_showdown.end()) {
    it2 = texas_showdown.begin();
    while (it2 < texas_showdown.end()) {
      if (it2->GetSuit() != it1->GetSuit() && it2->GetValue() == it1->GetValue()) {
        texas_showdown.erase(it1);
        it1 = texas_showdown.begin();
        it2 = texas_showdown.begin();
      }
      it2++;
    }
    it1++;
  }
  pair_num = pair_num + (texas_showdown.size() - pair_num);
  return pair_num;
}
size_t TexasForecast::CalcTwoPair() {
  size_t two_pair_method = 0;
  size_t pair_num = CalcThreeOfAKind();
  if (pair_num >= 1){
    ++two_pair_method;
  }
  two_pair_method++;
  return two_pair_method;
}
size_t TexasForecast::CalcOnePair() {
  size_t pair_num = CalcThreeOfAKind();
  return pair_num;
}
void TexasForecast::BubbleSort(std::vector<TexasCard>& in_out) {
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  size_t size = texas_showdown.size();
  for (size_t i=0;i<size;i++){
    for (size_t j=0;j<size;j++){
      if (texas_showdown[i].SameColorValue() < texas_showdown[j].SameColorValue()) {
        TexasCard t = texas_showdown[i];
        texas_showdown[i] = texas_showdown[j];
        texas_showdown[j] = t;
      }
    }
  }
  in_out = texas_showdown;
  return;
}
void TexasForecast::ConversionPokerTypeToTexasCard(std::vector<PokerTable>& pokers, 
  std::vector<TexasCard>& out) {
  out.resize(0);
  std::vector<PokerTable>::iterator it;
  for (it = pokers.begin();it!= pokers.end();it++){
    int card_index = (int)*it;
    if (*it>= PokerTable::kSpade3&&*it<= PokerTable::kSpade2){
      if (kCardTable[card_index - 1]==L'T'){
        TexasCard texas_card(kReverseSuitIndex[0], L"10", card_index);
        out.push_back(texas_card);
      }
      else {
        TexasCard texas_card(kReverseSuitIndex[0], kCardTable[card_index - 1], card_index);
        out.push_back(texas_card);
      }
    }
    else if (*it >= PokerTable::kHeart3&&*it <= PokerTable::kHeart2){
      if (kCardTable[card_index - 1 - 13] == L'T') {
        TexasCard texas_card(kReverseSuitIndex[1], L"10", card_index);
        out.push_back(texas_card);
      }
      else {
        TexasCard texas_card(kReverseSuitIndex[1], kCardTable[card_index - 1 - 13], card_index);
        out.push_back(texas_card);
      }
    }
    else if (*it >= PokerTable::kClub3&&*it <= PokerTable::kClub2){
      if (kCardTable[card_index - 1 - 26] == L'T') {
        TexasCard texas_card(kReverseSuitIndex[2], L"10", card_index);
        out.push_back(texas_card);
      }
      else {
        TexasCard texas_card(kReverseSuitIndex[2], kCardTable[card_index - 1 - 26], card_index);
        out.push_back(texas_card);
      }
    }
    else if (*it >= PokerTable::kDiamond3&&*it <= PokerTable::kDiamond2){
      if (kCardTable[card_index - 1 - 39] == L'T') {
        TexasCard texas_card(kReverseSuitIndex[3], L"10", card_index);
        out.push_back(texas_card);
      }
      else {
        TexasCard texas_card(kReverseSuitIndex[3], kCardTable[card_index - 1 - 39], card_index);
        out.push_back(texas_card);
      }
    }
  }
}
bool TexasForecast::IsFlush(wchar_t flush_card) {
  std::vector<TexasCard>::iterator it;
  std::wstring tstraight;
  tstraight.append(1, flush_card);
  bool is_success = true;
  std::vector<TexasCard> texas_showdown = texas_showdown_;
  for (it = texas_showdown.begin(); it != texas_showdown.end(); it++){
    if (it->GetSuit() == tstraight){
      continue;
    }
    else {
      texas_showdown.erase(it);
      if (texas_showdown.size() <= kMinFlushCardNum){
        is_success = false;
        break;
      }
      it = texas_showdown.begin();
    }
  }
  return is_success;
}
bool TexasForecast::IsStraight(std::vector<TexasCard>& texas_showdown) {
  int num = 0;
  TexasCard minimal_texas_card = texas_showdown[0];
  TexasCard maximum_texas_card = texas_showdown[texas_showdown.size() - 1];
  if ((maximum_texas_card.SameColorValue() - minimal_texas_card.SameColorValue())>kMaxStraightGap) {
    bool is_sucess = false;
    for (size_t i = texas_showdown.size() - 1; i>=2; i--){
      maximum_texas_card = texas_showdown[i];
      if((maximum_texas_card.SameColorValue() - minimal_texas_card.SameColorValue())<=kMaxStraightGap){
        ++num;
        if (num>=3) {
          is_sucess = true;
          break;
        }
      }
    }
    num = 0;
    for (size_t j = texas_showdown.size() - 1; j>=2; j--){
      maximum_texas_card = texas_showdown[texas_showdown.size() - 1];
      for (int i = 0; i < texas_showdown.size(); i++) {
        minimal_texas_card = texas_showdown[i];
        if ((maximum_texas_card.SameColorValue() - minimal_texas_card.SameColorValue()) <= kMaxStraightGap) {
          ++num;
          if (num >= 3) {
            is_sucess = true;
            break;
          }
        }
      }
    }
    return is_sucess;
  }
  return true;
}