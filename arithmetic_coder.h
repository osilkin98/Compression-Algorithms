#ifndef ARITHMETIC_CODING_H
#define ARITHMETIC_CODING_H
#include <map>
#include <iostream>
#include <string>
#include <list>

typedef long double dl;

template <typename K, typename S>
void clean_map(std::map<K,S*>& to_clean) {
  for(typename std::map<K, S*>::iterator it = to_clean.begin();
      it != to_clean.end(); ++it) {
    delete (*it).second;
  }
}

struct symbol {
  //symbol* next;
  void operator()(void);
  char ch;
  dl probability, start, end;
  unsigned int frequency;
};

struct encoded {
  std::list<symbol*> coding_data;
  void operator()(void);
  dl codeword_lower, codeword_upper, tag;
encoded(const dl lower, const dl upper,
	std::list<symbol*>& _symbols) :
  codeword_lower(lower), codeword_upper(upper),
    tag((lower+upper)/2), coding_data(_symbols) { }
  ~encoded(void);
};

encoded* encode_cstr(const char* str);

encoded* encode(std::string str);

encoded* encode(const std::string str, std::ostream& os);

std::string decode(encoded* message);

#endif
