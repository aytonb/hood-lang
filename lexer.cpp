// Compile as
// clang++ -g -O3 lexer.cpp -o lexer.out `llvm-config --cxxflags`


// I'm going to write this to read from a file pointer

// In hood, we do not want to be able to name variables as name-4,
// and should interpret this as name - 4.

#include <iostream>
#include <fstream>

enum token {
  tok_plus = -1,
  tok_minus = -2,
  tok_mult = -3,
  tok_div = -4,
  tok_exp = -5,
  tok_left_paren = -6,
  tok_right_paren = -7,
  tok_dot = -8,
  tok_comma = -9,
  tok_equal = -10,
  tok_less_than = -11,
  tok_greater_than = -12,

  tok_fnc = -20,
  tok_cls = -21,

  tok_identifier = -30,
  tok_number = -31,
};


static int arithmetic_token(const char& c) {
  switch(c) {
  case '+':
    return tok_plus;
  case '-':
    return tok_minus;
  case '*':
    return tok_mult;
  case '/':
    return tok_div;
  case '^':
    return tok_exp;
  case '(':
    return tok_left_paren;
  case ')':
    return tok_right_paren;
  case '.':
    return tok_dot;
  case ',':
    return tok_comma;
  case '=':
    return tok_equal;
  case '<':
    return tok_less_than;
  case '>':
    return tok_greater_than;
  default:
    return 0;
  }
}


bool get_token(std::ifstream& in_file, int& token, std::string& identifier) {
  char c;
  int breaking_token;
  identifier = "";
  bool end_token = false;
  
  in_file.get(c);

  // Skip any leading spaces
  while(isspace(c)) {
    in_file.get(c);
    if(in_file.eof()) {
      return 1;
    }
  }

  // Recognize arithmetic tokens
  token = arithmetic_token(c);
  if(token) {
    identifier += c; // Remove me
    return 0;
  }

  while(!in_file.eof()) {

    identifier += c;   
    in_file.get(c);

    // Once we get to a space, return
    if(isspace(c)) {
      end_token = true;
    } else {
      // Once we get to a breaking token, return
      breaking_token = arithmetic_token(c);
      if(breaking_token) {
        in_file.unget();
        end_token = true;
      }
    }

    if(end_token) {
      if(identifier == "fnc") {
        token = tok_fnc;
      } else if(identifier == "cls") {
        token = tok_cls;
      } else {
        token = tok_identifier;
      }
      
      return 0;
    }
  }

  return 1;
}


void read_file(const char file_name[]) {
  std::ifstream in_file(file_name);
  int token = 0;
  std::string identifier = "";
  bool file_ended = false;

  if(in_file.is_open()) {

    while(!file_ended) {
      file_ended = get_token(in_file, token, identifier);
      std::cout << "Token: " << token << " Identifier: " << identifier << "\n";
    }

  } else {
    std::cout << "Failed to open file\n";
  }

  in_file.close();
}


int main() {
  read_file("test.hd");
}
