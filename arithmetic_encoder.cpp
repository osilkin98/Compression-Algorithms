#include <iostream>
#include <string>
#include <map>
#include <cmath>
//#include "arithmetic_coder.h"

typedef long double dl;
/*
struct range {
  dl probability, start, end;
  unsigned int frequency;
  //char symbol;;
  range(unsigned int _frequency = 1) : frequency(_frequency), probability(0),
				       start(0), end(1) { }
  range(unsigned int _frequency, dl _probability) :
    probability(_probability), frequency(_frequency), start(0),
    end(1) { }
    };*/

template <typename K, typename S>
void clean_map(std::map<K,S*>& to_clean) {
  for(typename std::map<K, S*>::iterator it = to_clean.begin();
      it != to_clean.end(); ++it) {
    delete (*it).second;
  }
}

struct symbol {
  void operator()(void) {
    std::cout << ch << ": {\n\tprobability: " << probability
	      << "\n\tstart: " << start << "\n\tend: "
	      << end << "\n}\n" << std::endl;
  }
  char ch;
  dl probability, start, end;
  unsigned int frequency;
};

dl encode(const std::string str) {
  std::map<char, symbol*> symbols;
  for(register size_t i = 0; i < str.size(); ++i) {
    if(symbols[str[i]] == NULL) {
      symbols[str[i]] = new symbol;
      symbols[str[i]] -> frequency = 1;
      symbols[str[i]] -> ch = str[i];
    } else {
      ++(*symbols[str[i]]).frequency;
    }
  }
  dl last = 0;
  for(std::map<char, symbol*>::iterator it = symbols.begin();
      it != symbols.end(); ++it) {
    (*it).second -> probability =
      static_cast<dl>((*it).second -> frequency)/static_cast<dl>(str.size());
    (*it).second -> start = last;
    (*it).second -> end = last + (*it).second -> probability;
    last = (*it).second -> end;
    (*(*it).second)();
  }
  
  /*
  dl high = 1.0, low = 0.0;
  dl range;

  for(register size_t i = 0; i < str.size(); ++i) {
    range = high - low;
    
    }*/
  
  
  return 0.0;
}

int main(void) {
  std::string my_str = "AAABBC";
  encode(my_str);
  return 0;
}

