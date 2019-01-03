// include/lex.yy.h generated by reflex 1.0.10 from src/tokenizer.l

#ifndef REFLEX_HEADER_H
#define REFLEX_HEADER_H
#define IN_HEADER 1

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_dotall              true
#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         include/lex.yy.h
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               Lexer
#define REFLEX_OPTION_matcher             boost
#define REFLEX_OPTION_nostdinit           true
#define REFLEX_OPTION_outfile             src/lex.yy.cxx
#define REFLEX_OPTION_unicode             true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top{ user code %}                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 3 "src/tokenizer.l"

#include <vector>
#include <string>
#include <iostream>
#include "lingua/types.hxx"
#include "lingua/document.hxx"

#define TOKENIZER_CONTEXT_DEFAULT   0
#define TOKENIZER_CONTEXT_IN_TITLE  1
#define TOKENIZER_CONTEXT_IN_BODY   2


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/boostmatcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class Lexer : public reflex::AbstractLexer<reflex::BoostPosixMatcher> {
#line 15 "src/tokenizer.l"

public:
    std::vector<lingua::Document> getDocs() const { return docs; }
private:
    void switchContext(unsigned ctx) { context = ctx; }

    lingua::tag_t handleToken(const std::string &tok) {
        if (dict.find(tok) != dict.end())
            return dict[tok];
        else {
            lingua::tag_t tag = dict.size();
            dict[tok] = tag;
            return tag;
        }
    }

    void emitDocument() {
        docs.push_back(lingua::Document(currentTitle, currentBody));
        currentTitle = "";
        currentBody.clear();
    }

    std::string currentTitle;
    std::vector<lingua::tag_t> currentBody;
    std::vector<lingua::Document> docs;
    lingua::Dictionary dict;
    unsigned context;

 public:
  typedef reflex::AbstractLexer<reflex::BoostPosixMatcher> AbstractBaseLexer;
  Lexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  virtual int lex();
  int lex(
      const reflex::Input& input,
      std::ostream        *os = NULL)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

#endif