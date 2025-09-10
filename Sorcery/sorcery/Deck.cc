#include "Deck.h"
#include <random>
using namespace std;

// followed from https://cplusplus.com/reference/random/uniform_int_distribution/

void Deck::shuffle(){
    default_random_engine generator;
	uniform_int_distribution<int> uid(0, getSize() - 1);
	vector<std::unique_ptr<Card>> *c = getCards();
	for (int i = 0; i < 20; ++i){
		int a = uid(generator);
		int b = uid(generator);
		swap((*c)[a], (*c)[b]);
	}
}







