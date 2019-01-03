%option dotall fast unicode freespace nostdinit

%top {
#include <vector>
#include <string>
#include <iostream>
#include "lingua/types.hxx"
#include "lingua/document.hxx"

#define TOKENIZER_CONTEXT_DEFAULT   0
#define TOKENIZER_CONTEXT_IN_TITLE  1
#define TOKENIZER_CONTEXT_IN_BODY   2
%}

%class {
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
%}

WHITESPACE  [\r\n\t ]+
OPENTITLE   <TITLE>
CLOSETITLE  </TITLE>
OPENBODY    <BODY>
CLOSEBODY   </BODY>
WORD        \w+

%%

\<TITLE\>                 { switchContext(TOKENIZER_CONTEXT_IN_TITLE); }
\<\/TITLE\>                { switchContext(TOKENIZER_CONTEXT_DEFAULT); }
\<BODY\>                  { switchContext(TOKENIZER_CONTEXT_IN_BODY); }
\<\/BODY\>                 { switchContext(TOKENIZER_CONTEXT_DEFAULT); emitDocument(); }
\w+                     { switch (context) {
                          case TOKENIZER_CONTEXT_DEFAULT:
                              std::cout << "Word found elsewhere: " << str() << std::endl;
                              break;
                          case TOKENIZER_CONTEXT_IN_TITLE:
                              std::cout << "Word found in title: " << str() << std::endl;
                              currentTitle += str();
                              break;
                          case TOKENIZER_CONTEXT_IN_BODY:
                              std::cout << "Word found in body: " << str() << std::endl;
                              currentBody.push_back(handleToken(str()));
                              break;
                          default:
                              break; } }
.                       { }

%%
