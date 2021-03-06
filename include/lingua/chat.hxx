#ifndef LINGUA_CHAT_HXX
#define LINGUA_CHAT_HXX

#include <CoDiPack/codi.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "lingua/types.hxx"
#include "lingua/document.hxx"

#define PARAM_DEFAULT_VECTOR_LENGTH         128
#define PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD  4
#define PARAM_DEFAULT_NOISE_RATIO           3
#define PARAM_DEFAULT_INITIAL_LEARNING_RATE 0.01

namespace lingua {
    class ChatEngine;

    class SemanticVector {
    public:
        SemanticVector();
        SemanticVector(const float*);
        SemanticVector(const SemanticVector&) = default;
        SemanticVector(SemanticVector&&) = default;

        SemanticVector& operator=(const SemanticVector&);

        friend float operator*(const SemanticVector&, const SemanticVector&);
        friend float cosine(const SemanticVector&, const SemanticVector&);

        const float* getValues() const;
        float getValue(size_t) const;

        void setValues(const float*);
        void setValue(size_t, float);

        float magnitude() const;
    private:
        float* values;
    };

    class WordInfo {
    public:
        WordInfo() = default;
        WordInfo(tag_t, const std::string&);
        WordInfo(const WordInfo&) = default;
        WordInfo(WordInfo&&) = default;

        WordInfo& operator=(const WordInfo&) = default;

        tag_t getTag() const;
        std::string getText() const;
        SemanticVector* getSemanticEmbedding();
        SemanticVector* getContextEmbedding();

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
        struct Tokenizer {
            static std::string currentTitle;
            static std::string currentBody;
        };

        static ChatEngine *instance;

        ChatEngine(unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, unsigned = PARAM_DEFAULT_NOISE_RATIO);
        ChatEngine(const std::string&, unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, unsigned = PARAM_DEFAULT_NOISE_RATIO);

        static void initialize(unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, unsigned = PARAM_DEFAULT_NOISE_RATIO);
        static void initialize(const std::string&, unsigned = PARAM_DEFAULT_VECTOR_LENGTH, unsigned = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, unsigned = PARAM_DEFAULT_NOISE_RATIO);

        void analyzeSemantics();
        void learnSemantics();
        void trainOn(const Document&);
        void trainTargetEmbedding(SemanticVector*, const std::vector<SemanticVector*>&, const std::vector<SemanticVector*>&);

        std::vector<tag_t> generateNoise(const std::vector<tag_t>&);

        std::string getSourceFile() const;
        std::vector<Document> getDocs() const;
        unsigned getVectorLength() const;
        unsigned getContextNeighborhood() const;
        unsigned getNoiseRatio() const;
        float getLearningRate() const;
        Dictionary getDict() const;
        Infotbl getInfotbl() const;

        void setSourceFile(const std::string&);

        void addDoc(const Document&);

        Document tokenize(const std::string&);
        void printDocuments() const;
    private:
        void preprocess();

    private:
        std::string sourceFile;
        std::vector<Document> docs;
        unsigned pVectorLength;
        unsigned pContextNeighborhood;
        unsigned pNoiseRatio;
        float learningRate;
        Dictionary dict;
        Infotbl infotbl;
    };

    template <typename Real> Real dot(const Real *lhs, const Real *rhs) {
        auto vl = ChatEngine::instance->getVectorLength();
        Real result = 0;

        for (auto i = 0; i < vl; ++i)
            result += lhs[i] * rhs[i];

        return result;
    }

    template <typename Real> Real logisticRegression(const Real *targetEmbedding, const Real *contextEmbedding, std::vector<const Real*> noiseEmbeddings) {
        auto k = ChatEngine::instance->getNoiseRatio();
        Real result = codi::log(1 / (1 + codi::exp(-1 * dot(contextEmbedding, targetEmbedding))));

        for (auto i = 0; i < k; ++i)
            result += codi::log(1 / (1 + codi::exp(dot(noiseEmbeddings[i], targetEmbedding))));

        return result;
    }
}

#endif // LINGUA_CHAT_HXX
