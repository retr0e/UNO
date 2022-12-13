#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    char color;
    int number;
    char type;
    char inDeck;
} card;

typedef struct {
    char nickName[40];
    card playerHand[108];
    char isPerson;
    int cardsInHand;
} player;

void createProfiles(player playersArr[], int amountOfPlayers) {
    for (int i = 0; i < amountOfPlayers; i++) {
        char buffor[50];
        do {
            printf("How should i call you? (Enter your nickname)\n");
            scanf("%s", buffor);
        } while (strlen(buffor) > 40);
        
        strcpy(playersArr[i].nickName,buffor);
        playersArr[i].isPerson = 'T';
        system("clear");
    }
}

void cardSchuffle(card arr[], int size) {
    srand(time(NULL));
    int r1 = 0;
    int r2 = 0;

    // Tasowanie kart
    for (int i = 0; i < 4000; i++) {
        card helpful;
        r1 = (rand() % size);
        r2 = (rand() % size);

        helpful = arr[r1];
        arr[r1] = arr[r2];
        arr[r2] = helpful;
    }
}

int addAI(int numberOfPlayers, player registeredPlayers[]) {
    int botNumber = 1;
    for (int i = numberOfPlayers; i < 4; i++) {
        registeredPlayers[i].isPerson = 'N';
        char buffor[6];
        sprintf(buffor, "Bot %d", botNumber);
        strcpy(registeredPlayers[i].nickName, buffor);
        numberOfPlayers++;
        botNumber++;
    }
    return numberOfPlayers;
}

void showClearCard(card primitiveCard) {
    char colors[4][8] = {"red", "green", "blue", "yellow"};
    // Obsluga wild and draw four kart
    if (primitiveCard.color == 'B') {
        switch (primitiveCard.number) {
            case 70:
                printf("Draw 4  |\n");
                break;
            case 87:
                printf("Wild    |\n");
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < sizeof(colors)/sizeof(colors[0]); i++) {   
        // Sprawdzanie kolorow r g b y i dopasowanie do wzorca karty
        if (colors[i][0] == primitiveCard.color) {
            printf("%s %c", colors[i], primitiveCard.number);
            switch (colors[i][0]) {
                case 'r':
                    printf("   |\n");
                    break;
                case 'b':
                    printf("  |\n");
                    break;
                case 'g':
                    printf(" |\n");
                    break;
                case 'y':
                    printf("|\n");
                    break;
                default:
                    break;
            }
        }
    }
}

void showTheBoard(card topOfTheStack) {
    printf(" -------- \n");
    for (int i = 0; i < 7; i++) {
        if (i == 3) {
            printf("|");
            showClearCard(topOfTheStack);
            continue;
        }
        printf("|        |\n");
    }
    printf(" -------- \n");
}

void printHand(player player, int sizeOfHand) {
    printf("Player %s cards:\n", player.nickName);
    for (int i = 0; i < sizeOfHand; i++) {
        char colorToPrint[8];
        switch (player.playerHand[i].color) {
            case 'r':
                strcpy(colorToPrint, "Red");
                break;
            case 'g':
                strcpy(colorToPrint, "Green");
                break;
            case 'y':
                strcpy(colorToPrint, "Yellow");
                break;
            case 'b':
                strcpy(colorToPrint, "Blue");
                break;
            default:
                strcpy(colorToPrint, " ");
                break;
        }

        char numberToPrint[10];
        switch (player.playerHand[i].number) {
            case 'R':
                strcpy(numberToPrint, "Reverse");
                break;
            case 'S':
                strcpy(numberToPrint, "Skip");
                break;
            case 'T':
                strcpy(numberToPrint, "Draw Two");
                break;
            case 'W':
                strcpy(numberToPrint, "Wild");
                break;
            case 'F':
                strcpy(numberToPrint, "Draw Four");
                break;
            default:
                sprintf(numberToPrint, "%c", player.playerHand[i].number);
                break;
        }

        if (i == sizeOfHand - 1) {
            printf("%s %s\n", colorToPrint, numberToPrint);
        } else {
            printf("%s %s | ", colorToPrint, numberToPrint);
        }
    }
}

int cardDraw(card deck[], card playerCards[], int cardsInHand) {
    int sizeOfDeck = 108;
    for (int i = 0; i < sizeOfDeck; i++) {
        if (deck[i].inDeck == 'Y') {
            // printf("Dodana karta: %c %c", deck[i].color, deck[i].number);
            playerCards[cardsInHand] = deck[i];
            deck[i].inDeck = 'N';
            break;
        }
    }
    return 1;
}

int deleteCardFromHand(card arrOfCards[], card cardForDeletion, player playerCards) {
    card pom[playerCards.cardsInHand];
    int pomIterator = 0;
    int deletedCard = 0;
    for (int i = 0; i < playerCards.cardsInHand; i++) {
        if (arrOfCards[i].color == cardForDeletion.color && arrOfCards[i].number == cardForDeletion.number) {
            deletedCard++;
            continue;
        } else {
            pom[pomIterator] = arrOfCards[i];
            pomIterator++;
        }
    }
    
    playerCards.cardsInHand -= deletedCard;

    for (int i = 0; i < playerCards.cardsInHand; i++) {
        arrOfCards[i] = pom[i];
    }

    return deletedCard;
}

void addToDeck(card deck[], card cardToAdd) {
    int deckSize = 108;
    for (int i = 0; i < deckSize; i++) {
        if (deck[i].color == cardToAdd.color && deck[i].number == cardToAdd.number && deck[i].inDeck == 'N') {
            deck[i].inDeck = 'Y';
            break;
        }
    }
}

void gamePhase(player players[], card deck[], int amountOfPlayers) {
    // Rozpoczecie rozgrywki
    int startCardIndexAssign = 0;
    int assigned = 0;
    card cardOnTheTop;
    do {
        if (deck[startCardIndexAssign].color != 'B') {
            cardOnTheTop = deck[startCardIndexAssign];
            deck[startCardIndexAssign].inDeck = 'N';
            assigned++;
        }
        startCardIndexAssign++;
    } while (assigned == 0);

    // Przypisanie poczatkowych talii gracza
    for (int i = 0; i < amountOfPlayers; i++) {
        // Ustawienie ilosci kart na 0
        players[i].cardsInHand = 0;
        int correctAssign = 0;
        int x = 0;
        while (correctAssign < 8) {
            if (deck[x].inDeck == 'Y') {
                players[i].playerHand[correctAssign] = deck[x];
                deck[x].inDeck = 'N';
                players[i].cardsInHand++;
                correctAssign++;
            }
            x++;
        }
    }

    // Przebieg rozgrywki
    int playerMove = 0;
    int endOfTheGame = 0;
    int reverse = 0;
    do {
        int skipped = 0;
        int throwPassed = 0;
        do {
            printf("Card on the top:\n");
            showTheBoard(cardOnTheTop);
            printHand(players[playerMove], players[playerMove].cardsInHand);
            printf("Choose card to throw or draw one (by pressing \"d\"): ");
            int throw = 0;
            fflush(stdin);
            scanf("%d", &throw);

            if (throw == 0) {
                // Dobieranie karty
                int drawnCard = cardDraw(deck, players[playerMove].playerHand, players[playerMove].cardsInHand);
                players[playerMove].cardsInHand++;
                throwPassed = 1;
                system("clear");
                break;
            }
            
            // Sprawdzanie czy rzut jest zgodny z zasadami
            if (players[playerMove].playerHand[throw - 1].color == cardOnTheTop.color || players[playerMove].playerHand[throw - 1].number == cardOnTheTop.number ||
                    players[playerMove].playerHand[throw - 1].color == 'B') {
                cardOnTheTop = players[playerMove].playerHand[throw - 1];
                // Sprawdzenie czy karta jest karta specjalna
                if (cardOnTheTop.type == 'S') {
                    switch (cardOnTheTop.number) {
                        case 'F':
                            for (int i = 0; i < 4; i++) {
                                if (playerMove + 1 >= amountOfPlayers) {
                                    int drawnCard = cardDraw(deck, players[0].playerHand, players[0].cardsInHand);
                                    players[0].cardsInHand++;
                                } else {
                                    int drawnCard = cardDraw(deck, players[playerMove + 1].playerHand, players[playerMove + 1].cardsInHand);
                                    players[playerMove + 1].cardsInHand++;
                                }
                            }
                            char color[8];
                            printf("Pick a color: ");
                            scanf("%s", color);
                            cardOnTheTop.color = tolower(color[0]);
                            break;
                        case 'T':
                            for (int i = 0; i < 2; i++) {
                                if (playerMove + 1 >= amountOfPlayers) {
                                    int drawnCard = cardDraw(deck, players[0].playerHand, players[0].cardsInHand);
                                    players[0].cardsInHand++;
                                } else {
                                    int drawnCard = cardDraw(deck, players[playerMove + 1].playerHand, players[playerMove + 1].cardsInHand);
                                    players[playerMove + 1].cardsInHand++;
                                }
                            }
                            break;
                        case 'W':
                            fflush(stdin);
                            char colorOfWild[8];
                            printf("Pick a color: ");
                            scanf("%s", colorOfWild);
                            cardOnTheTop.color = tolower(colorOfWild[0]);
                            break;
                        case 'S':
                            fflush(stdin);
                            // int temporaryAdd = playerMove + 1;
                            // int temporarySub = playerMove - 1; 
                            if (reverse == 0) {
                                int cardToDelete = deleteCardFromHand(players[playerMove].playerHand, players[playerMove].playerHand[throw - 1], players[playerMove]);
                                players[playerMove].cardsInHand -= cardToDelete;
                                throwPassed = 1;
                                skipped = 1;
                                if (!(++playerMove < amountOfPlayers)) {
                                    playerMove = 0;
                                }
                            } else {
                                int cardToDelete = deleteCardFromHand(players[playerMove].playerHand, players[playerMove].playerHand[throw - 1], players[playerMove]);
                                players[playerMove].cardsInHand -= cardToDelete;
                                throwPassed = 1;
                                skipped = 1;
                                if (--playerMove < 0) {
                                    playerMove = amountOfPlayers -1;
                                }
                            }
                            break;
                        case 'R':
                            if (reverse == 0) {
                                reverse = 1;
                            } else {
                                reverse = 0;
                            }
                            break;
                        default:
                            break;
                    }
                }

                // Usuwanie karty z talii gracza
                if (skipped == 0) {
                    int cardToDelete = deleteCardFromHand(players[playerMove].playerHand, players[playerMove].playerHand[throw - 1], players[playerMove]);
                    players[playerMove].cardsInHand -= cardToDelete;
                    throwPassed = 1;
                }
            }

            // Dodawanie poprzedniej karty ze stosu do talii w sytuacji poprawnego rzucenia karty
            addToDeck(deck, cardOnTheTop);
            system("clear");
        } while (throwPassed == 0);
        
        cardSchuffle(deck, 108);
        // Zmniejszenie prawdopodobienstwa zobaczenia cudzych kart
        sleep(1);

        // Warunek zakonczenia gry
        if (players[playerMove].cardsInHand == 0) {
            printf("%s Congratulations! You won!\n", players[playerMove].nickName);
            endOfTheGame = 1;
        }

        // Sprawdzenie czy istnieje nastepny gracz/komputer do gry
        if (reverse == 0) {
            if (!(++playerMove < amountOfPlayers)) {
                playerMove = 0;
            }
        } else {
            if (--playerMove < 0) {
                playerMove = amountOfPlayers -1;
            }
        }


    } while (endOfTheGame == 0);

}

void cardRead(card arr[]) {
    FILE *file;
    file = fopen("deck.txt", "r");
    int i = 0;
    char buffor[16];
    do {
        fgets(buffor, 16, file);
        // printf("%s\n", buffor);
        arr[i].color = buffor[0];
        arr[i].number = buffor[2];
        arr[i].type = buffor[4];
        arr[i].inDeck = buffor[6];
        // printf("%c", arr[i].color);
        i++;
    } while (!feof(file));
    fclose(file);
}

void addSpecialCards(FILE *f) {
    // Sa 32 karty specjalnych w jednej talii UNO
    char colors[4] = {'r', 'g', 'b', 'y'};
    int reverse = 82;
    for (int i = 0; i < 3; i++) {
        int colorChange = 0;
        for (int j = 0; j < 4; j++) {
            card card;
            card.color = colors[colorChange];
            card.number = reverse;
            card.type = 'S';
            card.inDeck = 'Y';
            fprintf(f,"%c %c %c %c\n", card.color, card.number, card.type, card.inDeck);
            fprintf(f,"%c %c %c %c\n", card.color, card.number, card.type, card.inDeck);
            colorChange++;
        }
        reverse++;
    }

    int wild = 87;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            card card;
            card.color = 'B';
            card.number = wild;
            card.type = 'S';
            card.inDeck = 'Y';
            fprintf(f,"%c %c %c %c\n", card.color, card.number, card.type, card.inDeck);
        }
        wild = 70;
    }

}

void addNeutralCards(FILE *f) {
    char colors[4] = {'r', 'g', 'b', 'y'};
    int colorChange = 0;
    int number = 0;
    // Jest 76 kart neutralnych w jednej talii UNO
    for (int i = 0; i <= 77/2+1; i++) {
        card card;

        if (number > 9) {
            number = 0;
        }
        card.number = number;
        card.color = colors[colorChange];
        card.type = 'N';
        card.inDeck = 'Y';

        if (number % 9 == 0 && number != 0) {
            colorChange++;
        }

        if (number == 0) {
            fprintf(f, "%c %d %c %c\n", card.color, card.number, card.type, card.inDeck);
        } else {
            fprintf(f, "%c %d %c %c\n", card.color, card.number, card.type, card.inDeck);
            fprintf(f, "%c %d %c %c\n", card.color, card.number, card.type, card.inDeck);
        }

        number++;
    }
}

int createTheDeck() {
    FILE *file = fopen("deck.txt", "r");
    int control = 0;
    while (file == NULL) {
        // Tworzenie pliku, w ktorym bedzie przechowywania talia
        file = fopen("deck.txt", "w");
        control++;
        if (control == 2) {
            // Obsluga problemu utworzenia pliku przez np. brak permisji do folderu.
            printf("ERROR! FAIL TO CREATE DECK!");
            return 1;
        }
    }

    // Dodawanie kart do talii
    addNeutralCards(file);
    addSpecialCards(file);

    fclose(file);

    return 0;
}

int playersCount() {
    int dialogBlockChoice = 0;
    int amountOfPlayers = 1;
    do {
        printf("Are you playing alone or with somebody?\n");
        printf("(1) - I am playing alone!\n");
        printf("(2) - I am playing with friends!\n");
        printf("Your choice: ");
        scanf("%d", &dialogBlockChoice);
        switch(dialogBlockChoice) {
            case 1:
                dialogBlockChoice = 1;
                break;
            case 2:
                dialogBlockChoice = 2;
                break;
            default:
                dialogBlockChoice = 0;
        }
    } while (dialogBlockChoice == 0);

    if (dialogBlockChoice == 1) {
        printf("Thanks for information enjoy the game!\n");
        sleep(1);
        system("clear");
        return amountOfPlayers;
    }

    int control = 0;
    do {
        // Kominukat errora wartosci przekraczajacej ilosc graczy
        if (control > 0) {
            system("clear");
            printf("Error! Let's try again. The amount of players have to be below or equal 10 and greater or equal 1\n\n");
        }

        printf("How many people will play the game: ");
        scanf("%d", &amountOfPlayers);
        control++;
    } while (amountOfPlayers < 1 || amountOfPlayers > 10);

    printf("Thanks for information enjoy the game!\n");
    sleep(1);
    system("clear");
    return amountOfPlayers;
}

void gameController() {
    // Tworzenie tali kart
    createTheDeck();

    card *deck;
    int size = 108;
    deck = (card*)malloc(size * sizeof(*deck));
    
    // Wczytanie kart z pliku, lub pierw tworzenie pliku z talia
    cardRead(deck);
    cardSchuffle(deck, size);

    // Wybor ilosci graczy z weryfikacja i 1 sekundowym opoznieniem
    int players = playersCount();

    // Tworzenie profili graczy
    player tableOfPlayersInGame[4];
    createProfiles(tableOfPlayersInGame, players);

    // Jezeli liczba graczy jest mniejsza od 4 uzupelnianie miejsc botami
    if (players < 4) {
        players = addAI(players, tableOfPlayersInGame);
    }

    // Rozpoczecie gry
    gamePhase(tableOfPlayersInGame, deck, players); 
    
}

void printRules() {
    fflush(stdin);
    char read = ' ';
    do {
        system("clear");
        printf("Rule nr. 1: First to get rid of all cards win.\n");
        printf("Rule nr. 2: Contrary to original UNO you do not have to say \"UNO\" when you would have one card in hand.\n");
        printf("Rule nr. 3: To throw a correct card you need to match color or number of the card in your and and the card on top of the stack.\n");
        printf("Rule nr. 4: Exception to Rule nr. 3 are \"Wild\" and \"Draw Four\" cards, because they have ability to change color.\n");
        printf("Done reading? Press \"e\": ");
        scanf("%c", &read);
    } while (read != 'e');
    fflush(stdin);
}

int welcomeChatBlock() {
    int decision = 0;
    do {
        system("clear");
        printf("====== Welcome in UNO! ======\n");
        printf("(1) - Play the game\n");
        printf("(2) - Show me rules of the game\n");
        printf("(3) - Exit\n");
        printf("Your choice: ");
        scanf("%d", &decision);
        switch(decision) {
            case 1:
                return 1;
            case 2:
                printRules();
                decision = 0;
                break;
            case 3:
                return 3;
            default:
                decision = 0;
        }
    } while (decision == 0);

    return 0;
}

int main() {
    // Poczatkowa sekwencja decyzji
    int helloChatBlock = welcomeChatBlock();
    
    system("clear");

    // Rozpoczecie rozgrywki/Zakonczenie programu
    if (helloChatBlock == 1) {
        gameController();
    } else {
        printf("Thanks for playing! Goodbye! \n");
    }

    return 0;
}