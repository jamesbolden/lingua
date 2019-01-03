#include <iostream>
#include "lingua/document.hxx"

namespace lingua {
    Document::Document(const std::string &ti, const std::vector<tag_t> &tkns) : title(ti), tokens(tkns) { }

    std::string Document::getTitle() const { return title; }

    std::vector<tag_t> Document::getTokens() const { return tokens; }

    tag_t Document::getToken(size_t ix) const { return tokens[ix]; }

    void Document::printTokens() const {
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
}
