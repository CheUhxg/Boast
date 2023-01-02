#ifndef BOAST_BOAST_CARD_H
#define BOAST_BOAST_CARD_H

enum CardColor {
    Spades,
    Hearts,
    Diamonds,
    Clubs
};

struct BoastCard {
    char num = '0';
    CardColor color = Spades;
};

#endif //BOAST_BOAST_CARD_H
