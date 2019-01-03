#ifndef LINGUA_CHAT_HXX
#define LINGUA_CHAT_HXX

#include <string>
#include <unordered_map>
#include <vector>
#include "lingua/types.hxx"
#include "lingua/document.hxx"

#define PARAM_DEFAULT_VECTOR_LENGTH         128
#define PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD  4

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

    typedef std::unordered_map<tag_t, WordInfo> Infotbl;

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

        Document tokenize(const std::string&);
        void printDocuments() const;
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
