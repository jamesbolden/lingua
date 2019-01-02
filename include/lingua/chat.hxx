#ifndef LINGUA_CHAT_HXX
#define LINGUA_CHAT_HXX

#include <string>
#include <unordered_map>
#include <vector>
#include "lingua/types.hxx"

#define PARAM_DEFAULT_VECTOR_LENGTH         128
#define PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD  4

#define NOUN_TAG            0
#define VERB_TAG            1
#define INTERJECTION_TAG    2
#define ADJECTIVE_TAG       3
#define PREPOSITION_TAG     4
#define CONJUNCTION_TAG     5
#define PRONOUN_TAG         6
#define ADVERB_TAG          7

namespace lingua {
    class ChatEngine;

    class SemanticVector {
    public:
        SemanticVector(const ChatEngine*);
        SemanticVector(const ChatEngine*, const float*);
        SemanticVector(const SemanticVector&) = default;
        SemanticVector(SemanticVector&&) = default;

        SemanticVector& operator=(const SemanticVector&);

        ChatEngine* getParent();
        const float* getValues() const;
        float getValue(size_t) const;

        void setValues(const float*);
        void setValue(size_t, float);

    private:
        ChatEngine* parent;
        float* values;
    };

    class WordInfo {
    public:
        WordInfo(ChatEngine*, tag_t, const std::string&);
        WordInfo(const WordInfo&) = default;
        WordInfo(WordInfo&&) = default;

        tag_t getTag() const;
        std::string getText() const;
        const SemanticVector* getSemVec() const;

        void setSemVec(const SemanticVector&);
    private:
        tag_t tag;
        std::string text;
        SemanticVector* semvec;
    };

    typedef std::unordered_map<std::string, tag_t> Dictionary;
    typedef std::unordered_map<tag_t, WordInfo> Infotbl;

    class Document {
    public:
        Document(const ChatEngine*, const std::string&, const std::string&);
        Document(const Document&) = default;
        Document(Document&&) = default;

        ChatEngine* getParent();
        std::string getTitle() const;
        std::string getText() const;
        std::vector<tag_t> getTokens() const;

        void printTokens() const;
    private:
        ChatEngine* parent;
        std::string title;
        std::string text;
        std::vector<tag_t> tokens;
    };

    class ChatEngine {
    public:
        ChatEngine(unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD);
        ChatEngine(const std::string&, unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD);

        void analyzeSemantics();

        std::string getSourceFile() const;
        std::vector<Document> getDocs() const;
        unsigned getVectorLength() const;
        unsigned getContextNeighborhood() const;
        Dictionary getDict() const;
        Infotbl getInfotbl() const;

        void setSourceFile(const std::string&);

        void printDocuments() const;

        friend class Document;
    protected:
        std::vector<tag_t> tokenize(const std::string&);
    private:
        void preprocess();

    private:
        std::string sourceFile;
        std::vector<Document> docs;
        unsigned pVectorLength;
        unsigned pContextNeighborhood;
        Dictionary dict;
        Infotbl infotbl;
    };
}

#endif // LINGUA_CHAT_HXX
