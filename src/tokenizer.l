%{
#include "lingua/chat.hxx"
%}

%option noyywrap c++

%x TITLE
%x ENDTITLE
%x BODY
%x ENDBODY

%%

"<TITLE>"               BEGIN(TITLE);
"<BODY>"                BEGIN(BODY);
<TITLE>[^<>]+           { BEGIN(ENDTITLE); lingua::ChatEngine::Tokenizer::currentTitle = YYText(); }
<ENDTITLE>"</TITLE>"    BEGIN(INITIAL);
<BODY>[^<>]+            { BEGIN(ENDBODY); lingua::ChatEngine::Tokenizer::currentBody = YYText(); }
<ENDBODY>"</BODY>"      { BEGIN(INITIAL); lingua::ChatEngine::instance->addDoc(lingua::ChatEngine::instance->tokenize(lingua::ChatEngine::Tokenizer::currentBody)); }
.                       ;

%%
