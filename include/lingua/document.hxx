#ifndef LINGUA_DOCUMENT_HXX
#define LINGUA_DOCUMENT_HXX

#include <vector>
#include <ostream>
#include "lingua/types.hxx"

namespace lingua {
    class Document {
    public:
        Document(const std::vector<tag_t>&);
        Document(const Document&) = default;
        Document(Document&&) = default;

        Document& operator=(const Document&) = default;

        friend std::ostream& operator<<(std::ostream&, const Document&);

        std::vector<tag_t> getTokens() const;
        tag_t getToken(size_t) const;

        void printTokens() const;
    private:
        std::vector<tag_t> tokens;
    };
}

#endif // LINGUA_DOCUMENT_HXX
