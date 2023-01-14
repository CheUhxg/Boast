#ifndef BOAST_BOAST_CARD_H
#define BOAST_BOAST_CARD_H

enum CardColor {
    Spades,
    Hearts,
    Diamonds,
    Clubs
};

struct BoastCard {
    short num = 0;
    CardColor color = Spades;
};

#endif //BOAST_BOAST_CARD_H
