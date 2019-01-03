%{
#include "lingua/chat.hxx"
%}

%option noyywrap c++

%x TITLE
%x BODY

OPENTITLE   \<TITLE\>
CLOSETITLE  \</TITLE\>
OPENBODY    \<BODY\>
CLOSEBODY   \</BODY\>

%%

{OPENTITLE}             BEGIN(TITLE);
{OPENBODY}              BEGIN(BODY);
<TITLE>{
    "<\/TITLE>"         BEGIN(INITIAL);
    (.|\n)*             lingua::ChatEngine::Tokenizer::currentTitle = YYText();
}
<BODY>{
    "<\/BODY>"          { lingua::ChatEngine::instance->addDoc(lingua::ChatEngine::instance->handleToks(lingua::ChatEngine::Tokenizer::currentBody)); lingua::ChatEngine::Tokenizer::currentBody.clear(); BEGIN(INITIAL); }
    (\w|['\-])+         lingua::ChatEngine::Tokenizer::currentBody.push_back(YYText());
    .                   ;
}
.                       ;

%%
