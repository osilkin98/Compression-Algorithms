#include <iostream>
#include <string>
//#include <map>
//#include <cmath>
#include "arithmetic_coder.h"

void encoded::operator()(void) {
  std::cout << "compressed object {\n\tlower limit: "
	    << codeword_lower << "\n\tupper limit: "
	    << codeword_upper << "\n\ttag: " << tag
	    << "\n};" << std::endl;
}

void symbol::operator()(void) {
  std::cout << ch << ": {\n\tprobability: " << probability
	      << "\n\tstart: " << start << "\n\tend: "
	      << end << "\n}\n" << std::endl;
}

encoded::~encoded(void) {
  while(!coding_data.empty()) {
    delete coding_data.front();
    coding_data.front() = NULL;
    coding_data.pop_front();
  }
}

encoded* encode(std::string str) {
  str.push_back('\0');
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
  char partition_scheme[symbols.size()];
  dl last = 0;
  size_t i = 0;
  std::list<symbol*> symbol_list;
  for(std::map<char, symbol*>::iterator it = symbols.begin();
      it != symbols.end(); ++it) {
    partition_scheme[i] = (*it).first;
    (*it).second -> probability =
      static_cast<dl>((*it).second -> frequency)/static_cast<dl>(str.size());
    (*it).second -> start = last;
    (*it).second -> end = last + (*it).second -> probability;
    last = (*it).second -> end;
    symbol_list.push_back((*it).second);
  }
  dl high = 1.0, low = 0.0, range;
  for(register size_t i = 0; i < str.size(); ++i) {
    size_t sym_i;
    for(sym_i = 0; partition_scheme[sym_i] != str[i]; ++sym_i);
    range = high - low;
    high = low + range * (symbols[str[i]] -> end);
    low = low + range * (symbols[str[i]] -> start);
  }
  //clean_map<char, symbol>(symbols);
  encoded* compressed_object = new encoded(low, high, symbol_list);
  return compressed_object;
}

encoded* encode_cstr(const char* str) {
  std::string temporary_string(str);
  return encode(temporary_string);
}

encoded* encode(const std::string str, std::ostream& os) {
  encoded* d = encode(str);
  os << d -> tag;
  return d;
}

std::string decode(encoded* message) {
  std::list<symbol*>::iterator it = message -> coding_data.begin();
  dl high = 1.0, low = 0.0;
  dl range = high - low;
  std::string decoded_message;
  // we have to scan through the list until we find a symbol such that
  // our codeword is within its bounds, we then add it to the message,
  // and scale the bounds so that we search within those
  // appropriately
  size_t i = 1;
  while(it != message -> coding_data.end()) {
    dl a = low + range*((*it) -> start),
      b = low + range*((*it) -> end);
    if((a <= message -> tag)
       and (b >= message -> tag)) {
      if((*it) -> frequency == 0) {
	throw "the program select an item with frequency 0";
      }
      if((*it) -> ch == '\0') {
	return decoded_message;
      }
      decoded_message.push_back((*it) -> ch);
      high = low + range*((*it) -> end);
      low = low + range*((*it) -> start);
      range = high - low;
      --((*it) -> frequency);
      it = message -> coding_data.begin();
    }
    ++i;
    ++it;
  }
}

/*
int main(void) {
  try{
  std::cout << "Enter a string to compress: " << std::flush;
  std::string my_str;
  std::getline(std::cin, my_str);
  encoded* d = encode(my_str);
  (*d)();
  std::string decoded = decode(d);
  std::cout << "Decoded message: " << decoded << std::endl;
  delete d;
  } catch(const char* msg) {

    std::cerr << msg << "\n";
  }
  //std::cout << "encoded string: " << d << std::endl;
  return 0;
}

*/
