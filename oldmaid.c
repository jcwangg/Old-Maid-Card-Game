//Joyce Wang and Corbin Blomquist
//ECE 175 Final Project
//The purpose of this program is to execute a card game, Joker, using linked lists. 

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>							//to use malloc
#include <string.h>

typedef struct card_s {
	char suit[10];							//to store clubs, spades, diamonds, or hearts
	int face;								// to store card number 1-13
	struct card_s *listp;					//pointer to next card 
}card;

int countCards(card*hp) {
	int count = 0; 
	card *curr = hp;
	while (curr != NULL) {
		curr = curr->listp; 
		count++; 
	}
	return count; 
}

void CreateDeck(card **hp) {				 //creates initial deck of 53 cards with a joker as linked list and card variables
	card *temp = NULL;						 //pointer to the new card created
	card *curr = NULL;						 //pointer to current last card of list
	char suits[10] = "clubs";
	int i, j;
	for (j = 0; j < 4; j++) {
		for (i = 1; i <= 13; i++) {				//creating each suit
			temp = (card*)malloc(sizeof(card)); //memory allocation for new card
			temp->face = i;						//setting face attribute of new card
			strcpy(temp->suit, suits);			//setting suit attribute of new card
			if (*hp == NULL) {					//if list is empty, head pointer to new card 
				*hp = temp;
				curr = *hp;
			}
			else {
				curr->listp = temp;				//if list is not empty, last card points to new card
				curr = curr->listp;
			}
		}
		if (j == 0) {							//looping to create diamond suit
			strcpy(suits, "diamonds");
		}
		else if (j == 1) {						//looping to create spades suit
			strcpy(suits, "spades");
		}
		else if (j == 2) {						//looping to create hearts suit
			strcpy(suits, "hearts");
		}
	}
	temp = (card*)malloc(sizeof(card));			//memory allocation for new card (joker)
	temp->face = 69;							//face value of joker
	strcpy(temp->suit, "joker");				//suit value of joker
	curr->listp = temp;							//attaching joker to end of deck linked list
	curr = curr->listp;
	curr->listp = NULL;
}


void DealDeck(card*hp, card **p1hp, card **p2hp) { //Alternates dealing cards to a player 1 linked list and player 2 linked list
	int i=0;
	card*temp = hp;
	card*p1Curr = NULL;						//pointer to current last card in p1 list
	card*p2Curr = NULL;						//pointer to current last card in p2 list
	int count;
	count=countCards(hp);					//gets the number of cards in the deck
	while (temp != NULL) {					//while the end of the list has not been reached
		if (*p1hp == NULL) {				//if p1's hand list is empty, point p1 head pointer to first card
			*p1hp = temp;
			p1Curr = *p1hp;
		}
		else if (*p2hp == NULL) {			//if p2's hand list is empty, point p2 head pointer to first card
			*p2hp = temp;
			p2Curr = *p2hp;
		}
		else {
			if (i % 2 == 0) {				//if p1 hand is not empty and i is odd, add card to end of p1 list
				p1Curr->listp = temp;
				p1Curr = p1Curr->listp;
			}
			else if (i%2==1){				//if p2 hand is not empty and i is even, add card to end of p2 list
				p2Curr->listp = temp;
				p2Curr = p2Curr->listp;
			}
		}
		temp = temp->listp;					//update temp pointer to next card in deck
		i++;								//increment i to keep track of whose hand to deal card to 
	}
	p1Curr->listp = NULL;					//point last card of p1's hand to NULL
	p2Curr->listp = NULL;					//point last card of p2's hand to NULL
}

void RemoveCard(card **hp, card **tp, int target) { //removes a card from the list with the target face value
	card *targetp = *hp;							//pointer to the card to be deleted
	card **temp = hp;								//pointer to head of the list 

	while ((targetp->face != target) && (targetp != NULL)) { //exits loop if found the card to be deleted 
		targetp = targetp->listp;
	}
	if (targetp == NULL) {									 //can't find card to delete
		return;
	}
	else {
		if (targetp == *hp) {								 //if the first card is deleted, update head pointer
			*hp = targetp->listp; 
		}
		else {
			while ((*temp)->listp != targetp) {
				temp = &(*temp)->listp;						//locating address of the previous element (1 before target)
			}
			if (targetp == *tp) {							//if last card is to be deleted
				*tp = *temp;								//update tail 
				(*tp)->listp = NULL;						//updating last element to point to NULL 
			}
			else {
				(*temp)->listp = targetp->listp;			//if middle card deleted, previous element points to next element 
			}
		}
		printf("%d %s ", targetp->face, targetp->suit);		//printing deleted card
		free(targetp);										//deallocating memory of deleted card
	}
	return; 
}

void RemovePairs(card**hp, card**tp) { //checks a list of cards to find all pairs, then calls RemoveCard function to delete pairs
	int i, j;
	int count = 0;
	int eachCard[13];				//keeps track of how many of each card there are in the hand
	int faceCard; 
	int targetRemove = 0;			//the face value of the card to remove
	for (i = 0; i < 13; i++) {		//initializes all elements of array to 0 
		eachCard[i] = 0;
	}
	card *temp = NULL;				//pointer to current card
	temp=*hp;						//starts temp at first card in hand 
	count = countCards(*hp);		//returns number of cards in hand
	for (j = 0; j < count; j++) {   //loops through entire hand, updating eachCard array count number of each card in hand
		faceCard = temp->face;
		eachCard[faceCard - 1] += 1;
		temp = temp->listp;
	}
	for (i = 0; i < 13; i++) {
		if ((eachCard[i] == 2) || (eachCard[i] == 3)) { //if there are two or three of the same card
			targetRemove = i + 1; 
			printf("\nRemove pair: "); 
			RemoveCard(hp,tp,targetRemove);				//function call twice to remove two cards (1 pair)
			RemoveCard(hp,tp,targetRemove);
		}
		else if (eachCard[i] == 4) {					//if there are four of the same card
			targetRemove = i + 1;
			printf("\nRemove pair: ");					//function call four times to remove four cards (2 pairs)
			RemoveCard(hp, tp, targetRemove);
			RemoveCard(hp, tp, targetRemove);
			printf("\nRemove pair: "); 
			RemoveCard(hp, tp, targetRemove);
			RemoveCard(hp, tp, targetRemove);
		}
	}
	printf("\n\n"); 
	return; 
}

void TransferCard(card **p1hp, card**p2hp, card **p1tp, card **p2tp, int turn) { //transfers a card from one hand to another each player's turn 
//and removes a pair if the transferred card forms a pair

	//Player 2 is choosing card from player 1's hand, removing card from p1, adding card to end of p2's hand
	int chooseFrom;											//number of cards in p1's hand
	int chooseNum;											//user entered number of card  number chosen out of chooseFrom
	int i; 
	int removeCardFace = 0; 
	int count = 1;											//find the card being added to p2's hand/being removed from p1's hand
	card *temp = NULL; 
	card *curr1 = NULL;										//points to target card to be removed from p1's hand
	curr1 = *p1hp; 
	card *curr2 = NULL;										//points to last card in p2's hand 
	curr2 = *p2hp;
	if (turn == 0) {										 //USER'S TURN
		chooseFrom = countCards(*p1hp);
		printf("\n\nI now have %d cards:\n", chooseFrom);	//printing which cards user can choose from
		for (i = 1; i <= chooseFrom; i++) {
			printf("%d ", i);
		}
		printf("\nWhich one do you choose (1-%d) ?\n", chooseFrom);		//scanning in user's choice of card
		scanf("%d", &chooseNum);
		while ((chooseNum<1) || (chooseNum>chooseFrom)) {				//while loop until user enters in a valid card number			
			printf("I have ONLY %d cards\n", chooseFrom);
			printf("Which one do you want to choose (1-%d)?\n", chooseFrom);
			scanf("%d", &chooseNum); 
		}
	}
	else if (turn == 1) {									//COMPUTER's TURN
		printf("\nI choose card number 1 from you\n");		//computer's strategy: Choose first card
		chooseNum = 1; 
	}
	while (count != chooseNum) {							//finding card to be added pointed to by curr1
		count++;
		curr1 = curr1->listp; 
	}
	temp = (card*)malloc(sizeof(card));						//memory allocation for new card to be added to p2's hand
	temp->face = curr1->face;								//temp = new card to add to p2's hand
	strcpy(temp->suit, curr1->suit);
	removeCardFace = temp->face;							//face value of card to be removed from p1's hand
	temp->listp = NULL; 
	while (curr2->listp != NULL) {							//finding last card in p2's hand
		curr2 = curr2->listp;
	}
	curr2->listp = temp;									//adding temp card to end of p2's hand
	curr2 = curr2->listp; 
	curr2->listp = NULL; 
	RemoveCard(p1hp, p1tp, removeCardFace);					//removing target card from p1's hand
	curr2 = *p2hp;
	while (curr2->listp != NULL) {
		if (curr2->face == temp->face) {					//CHECKING ENTIRE HAND TO SEE IF P2 CAN REMOVE A PAIR
			printf("\nRemove pair: ");						//If so, call RemoveCard function 
			RemoveCard(p2hp, p2tp, curr2->face); 
			RemoveCard(p2hp, p2tp, temp->face);
			return;
		}
		curr2 = curr2->listp;		
	}
	return; 

}

void PrintDeck(card *hp) { //Prints the face value and suit of every card in a list
	card *curr = hp;
	while (curr != NULL) {								//loops until the end of the list is reached
		printf("%d %s\n", curr->face, curr->suit);
		curr = curr->listp;								//find the next card
	}
	return;
}

void ShuffleDeck(card*hp) { //Goes through each card and switches contents with a random card 
	int i;
	int numCards;										//stores the number of cards in the list
	card*temp1 = hp;									//the current card you are on (1 to the # of cards)
	card*temp2 = hp;									//the random card you are shuffling temp1 with (rand int)
	int count = 0;										//increments until temp2 is found
	int cardNum = 0;									//the random card number from srand()
	int numTemp;									    //stores face of temp1 
	char suitTemp[15] = "";								//stores suit of temp 1 
	numCards=countCards(hp); 
	for (i = 0; i < numCards; i++) {					//goes through every card in the deck 
		count = 0;										//resets count to 0 
		temp2 = hp;										//resets temp2 to beginning of hand
		srand(cardNum);
		cardNum = (rand() % numCards) + 1;				//storing a random number in cardNum 
		while (count < cardNum-1) {						//incrementing temp 2 to find the random card you are shuffling with 
			temp2 = temp2->listp;
			count++;
		}
		//Switching all the contents of temp1 and temp2
		numTemp = temp1->face;
		strcpy(suitTemp, temp1->suit);
		temp1->face = temp2->face;
		strcpy(temp1->suit, temp2->suit);
		temp2->face = numTemp;
		strcpy(temp2->suit, suitTemp);
		temp1 = temp1->listp;							//incrementing temp1 to the next card to go through entire hand
	}
	return;
}

int main(void) {
	char playAgain = 'a'; 
	while ((playAgain != 'q') && (playAgain !='Q')) {
		srand((int)time(NULL));								//to use random seed int generator
		card *headp = NULL;									//points to beginning of deck card list
		card *p1hp = NULL;									//points to beginning of p1's card list (user)
		card *p2hp = NULL;									//points to beginning of p2's card list (computer)
		card *p1tp = NULL; 									//points to end of p1's card list (user)
		card *p2tp = NULL; 									//points to end of p2's card list (user)
		int turn = 0;										//keeps track of computer or user's turn
		int winCondition = 3;								//becomes 0 when someone wins
		char name[99];										//name of user
		int handSizeUser = 0;								//size of User's deck
		int handSizeComp = 0;								//size of computer's deck

		printf("Enter your name: ");
		scanf("%s", name);
		printf("\n%s, let's play Joker\n\n", name);

		CreateDeck(&headp);									//function call to create deck of cards
		//PrintDeck(headp);									//PRINTS UNSHUFFLED DECK OF CARDS
		ShuffleDeck(headp);
		//PrintDeck(headp);									//PRINTS SHUFFLED DECK OF CARDS
		DealDeck(headp, &p1hp, &p2hp);						//splits deck of cards between user's hand and computer's hand 

		printf("%s's hand has %d cards \n", name, countCards(p1hp));
		PrintDeck(p1hp);									//PRINTS USER'S HAND AFTER DEALING
		//PrintDeck(p2hp);									//PRINTS COMPUTER'S HAND AFTER DEALING

		printf("\nDealer's hand: throw away pairs: \n");	//PRINTS COMPUTER'S HAND AFTER REMOVING PAIRS AND PRINTS NUMBER OF CARDS IN HAND
		RemovePairs(&p2hp, &p2tp);
		//PrintDeck(p2hp);
		printf("I now have %d cards\n\n", countCards(p2hp));

		printf("%s's hand: throw away pairs: \n", name);	//PRINTS USER'S HAND AFTER REMOVING PAIRS
		RemovePairs(&p1hp, &p1tp);
		printf("%s, this is your hand:\n\n", name);
		PrintDeck(p1hp);

		handSizeUser = countCards(p1hp);					//returns # of cards in user's hand
		handSizeComp = countCards(p2hp);					//returns # of cards in computer's hand
		if (handSizeUser > handSizeComp) {					//the player with more cards goes first
			printf("\nYou have more cards, you start\n");
			turn = 0;										//indicates it is the computer's turn
		}
		else if (handSizeComp > handSizeUser) {
			printf("\nI have more cards, I start\n");
			turn = 1;										 //indicates it is the user's turn 
		}

		//player's keep taking turns until someone wins (winCondition==0)
		while (winCondition != 0) {
			if (turn == 0) {								//User chooses card from computer's hand, shuffles and prints hand
				TransferCard(&p2hp, &p1hp, &p2tp, &p1tp, turn);
				printf("\n\n%s, this is your shuffled hand:\n", name);
				ShuffleDeck(p1hp);
				PrintDeck(p1hp);
				turn = 1;									//next iteration becomes computer's turn 
			}
			else if (turn == 1) {							//computer chooses card from user's hand, hand is shuffled
				TransferCard(&p1hp, &p2hp, &p1tp, &p2tp, turn);
				ShuffleDeck(p2hp);
				turn = 0;									//next iteration becomes user's turn 
			}
			handSizeUser = countCards(p1hp);				//size of user's hand
			handSizeComp = countCards(p2hp);				//size of computer's hand
			if (handSizeUser == 0) {						//user has no cards left
				winCondition = 0;
				printf("\nCongratulations, you won! I am Joker.\n");
			}
			else if (handSizeComp == 0) {					//computer has no cards left
				winCondition = 0;
				printf("\nSucks to suck, I won! You are Joker.\n");
			}
		}
		printf("\nDo you want to continue? (q or Q to quit) ");
		scanf(" %c", &playAgain); 
	}
	printf("Goodbye\n");
	return;
}
