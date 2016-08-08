#pragma once

namespace Game
{
  class Card
  {
  protected:
    Graphics::Texture2D tex; //TODO: make this a collection of meshes, etc for 3D cards?
  public:
    Card() {}
    ~Card() {}

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual void simulate(const double frame_time) = 0;
    virtual void render(const double game_time, const double frame_time) = 0;
  };

  class CombatantCard : public Card
  {
  protected:
    uint32_t attack;
    uint32_t attacks_per_turn;
    uint32_t defense;
    uint32_t shield;
  public:
    CombatantCard() : Card() {}
    ~CombatantCard() {}
  };

  class Hand
  {
  protected:
    uint32_t max_cards;
    std::vector<Card *> hand;
  public:
    Hand() {}
    ~Hand() {}

    void discard(uint32_t c_idx);
    void play(uint32_t c_idx);
  };

  class Deck
  {
  protected:
    std::vector<Card *> deck;
  public:
    Deck() {}
    ~Deck() {}

    void shuffle();
    Card *draw_top();
    Card *draw_random();
  };
};
