#include <iostream>
#include "lingua/document.hxx"

namespace lingua {
    Document::Document(const std::vector<tag_t> &tkns) : tokens(tkns) { }

    std::ostream& operator<<(std::ostream &os, const Document &doc) {
        for (auto it = doc.tokens.cbegin(); it != doc.tokens.cend(); ++it) {
            os << *it << std::endl;
        }

        return os;
    }

    std::vector<tag_t> Document::getTokens() const { return tokens; }

    tag_t Document::getToken(size_t ix) const { return tokens[ix]; }

    void Document::printTokens() const {
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
}
