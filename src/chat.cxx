#include "lingua/chat.hxx"
#include <FlexLexer.h>
#include <re2/re2.h>
#include <boost/tokenizer.hpp>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <random>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <chrono>

namespace lingua {
    float operator*(const SemanticVector &lhs, const SemanticVector &rhs) {
        float result = 0;
        auto vl = ChatEngine::instance->getVectorLength();
        for (auto i = 0; i < vl; ++i)
            result += lhs.values[i] * rhs.values[i];
        return result;
    }

    float cosine(const SemanticVector &lhs, const SemanticVector &rhs) {
        return (lhs * rhs) / (lhs.magnitude() * rhs.magnitude());
    }

    SemanticVector::SemanticVector() : values(new float[ChatEngine::instance->getVectorLength()]) {
        auto vl = ChatEngine::instance->getVectorLength();
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine gen(seed);
        std::uniform_real_distribution<float> dist;

        for (auto i = 0; i < vl; ++i)
            values[i] = dist(gen);
    }

    SemanticVector::SemanticVector(const float *vs) : values(new float[ChatEngine::instance->getVectorLength()]) {
        auto vl = ChatEngine::instance->getVectorLength();

        for (auto i = 0; i < vl; ++i)
            values[i] = vs[i];
    }

    SemanticVector& SemanticVector::operator=(const SemanticVector &other) {
        values = other.values;
        return *this;
    }

    const float* SemanticVector::getValues() const {
        return values;
    }

    float SemanticVector::getValue(size_t ix) const {
        return values[ix];
    }

    void SemanticVector::setValues(const float *vs) {
        auto vl = ChatEngine::instance->getVectorLength();

        for (auto i = 0; i < vl; ++i)
            values[i] = vs[i];
    }

    void SemanticVector::setValue(size_t ix, float value) {
        values[ix] = value;
    }

    float SemanticVector::magnitude() const {
        auto vl = ChatEngine::instance->getVectorLength();
        float result = 0;

        for (auto i = 0; i < vl; ++i)
            result += values[i] * values[i];

        return std::sqrt(result);
    }

    WordInfo::WordInfo(tag_t t, const std::string &txt) : tag(t), text(txt), semEmb(new SemanticVector), ctxEmb(new SemanticVector) { }

    tag_t WordInfo::getTag() const {
        return tag;
    }

    std::string WordInfo::getText() const {
        return text;
    }

    const SemanticVector* WordInfo::getSemanticEmbedding() const {
        return semEmb;
    }

    const SemanticVector* WordInfo::getContextEmbedding() const {
        return ctxEmb;
    }

    void WordInfo::setSemanticEmbedding(const SemanticVector &sv) {
        if (semEmb)
            *semEmb = sv;
        else {
            semEmb = new SemanticVector(sv);
        }
    }

    void WordInfo::setContextEmbedding(const SemanticVector &cv) {
        if (ctxEmb)
            *ctxEmb = cv;
        else {
            ctxEmb = new SemanticVector(cv);
        }
    }

    std::string ChatEngine::Tokenizer::currentTitle = "";

    std::string ChatEngine::Tokenizer::currentBody;

    ChatEngine* ChatEngine::instance = nullptr;

    ChatEngine::ChatEngine(unsigned pvl, unsigned pcn, unsigned pnr) : sourceFile("-"), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), pNoiseRatio(pnr), learningRate(PARAM_DEFAULT_INITIAL_LEARNING_RATE), dict(), infotbl() { }

    ChatEngine::ChatEngine(const std::string &src, unsigned pvl, unsigned pcn, unsigned pnr) : sourceFile(src), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), pNoiseRatio(pnr), learningRate(PARAM_DEFAULT_INITIAL_LEARNING_RATE), dict(), infotbl() { }

    void ChatEngine::initialize(unsigned pvl, unsigned pcn, unsigned pnr) {
        instance = new ChatEngine(pvl, pcn, pnr);
    }

    void ChatEngine::initialize(const std::string &src, unsigned pvl, unsigned pvn, unsigned pnr) {
        instance = new ChatEngine(src, pvl, pvn, pnr);
    }

    void ChatEngine::analyzeSemantics() {
        preprocess();
    }

    std::string ChatEngine::getSourceFile() const {
        return sourceFile;
    }

    std::vector<Document> ChatEngine::getDocs() const {
        return docs;
    }

    unsigned ChatEngine::getVectorLength() const {
        return pVectorLength;
    }

    unsigned ChatEngine::getContextNeighborhood() const {
        return pContextNeighborhood;
    }

    unsigned ChatEngine::getNoiseRatio() const {
        return pNoiseRatio;
    }

    float ChatEngine::getLearningRate() const {
        return learningRate;
    }

    Dictionary ChatEngine::getDict() const {
        return dict;
    }

    Infotbl ChatEngine::getInfotbl() const {
        return infotbl;
    }

    void ChatEngine::setSourceFile(const std::string &src) {
        sourceFile = src;
    }

    void ChatEngine::addDoc(const Document &doc) {
        docs.push_back(doc);
    }

    Document ChatEngine::tokenize(const std::string &text) {
        boost::tokenizer<> tknzr(text);
        std::vector<tag_t> toks;

        for (auto it = tknzr.begin(); it != tknzr.end(); ++it) {
            if (dict.find(*it) != dict.end())
                toks.push_back(dict[*it]);
            else {
                tag_t tkn = dict.size();
                dict[*it] = tkn;
                infotbl[tkn] = WordInfo(tkn, *it);
                toks.push_back(tkn);
            }
        }

        return Document(toks);
    }

    Document ChatEngine::handleToks(const std::vector<std::string> &toks) {
        std::vector<tag_t> tkns;

        for (auto it = toks.cbegin(); it != toks.cend(); ++it) {
            if (dict.find(*it) != dict.end())
                tkns.push_back(dict[*it]);
            else {
                tag_t tkn = dict.size();
                dict[*it] = tkn;
                infotbl[tkn] = WordInfo(tkn, *it);
                tkns.push_back(tkn);
            }
        }

        return Document(tkns);
    }

    void ChatEngine::printDocuments() const {
        for (auto it = docs.begin(); it != docs.end(); ++it)
            it->printTokens();
    }

    void ChatEngine::preprocess() {
        std::ifstream fh(sourceFile);
        std::ofstream nullDev("/dev/null");
        FlexLexer *lexer = new yyFlexLexer(fh, nullDev);

        lexer->yylex();
    }
}
