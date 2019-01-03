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
        SemanticVector(ChatEngine*);
        SemanticVector(ChatEngine*, const float*);
        SemanticVector(const SemanticVector&) = default;
        SemanticVector(SemanticVector&&) = default;

        SemanticVector& operator=(const SemanticVector&);

        friend float operator*(const SemanticVector&, const SemanticVector&);
        friend float cosine(const SemanticVector&, const SemanticVector&);

        ChatEngine* getParent();
        const float* getValues() const;
        float getValue(size_t) const;

        void setValues(const float*);
        void setValue(size_t, float);

        float length() const;
    private:
        ChatEngine* parent;
        float* values;
    };

    class WordInfo {
    public:
        WordInfo() = default;
        WordInfo(ChatEngine*, tag_t, const std::string&);
        WordInfo(const WordInfo&) = default;
        WordInfo(WordInfo&&) = default;

        WordInfo& operator=(const WordInfo&) = default;

        tag_t getTag() const;
        std::string getText() const;
        const SemanticVector* getSemanticEmbedding() const;
        const SemanticVector* getContextEmbedding() const;

        void setSemanticEmbedding(const SemanticVector&);
        void setContextEmbedding(const SemanticVector&);
    private:
        tag_t tag;
        std::string text;
        SemanticVector* semEmb;
        SemanticVector* ctxEmb;
    };

    typedef std::unordered_map<tag_t, WordInfo> Infotbl;

    class ChatEngine {
    public:
        static ChatEngine *instance;

        ChatEngine(unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD);
        ChatEngine(const std::string&, unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD);

        static void initialize();
        static void initialize(const std::string&);

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
