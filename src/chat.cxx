#include "lingua/chat.hxx"
#include <re2/re2.h>
#include <boost/tokenizer.hpp>
#include <random>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <chrono>

namespace lingua {
    SemanticVector::SemanticVector(const ChatEngine *ce) : parent(ce), values(new float[ce->getVectorLength()]) {
        auto vl = ce->getVectorLength();
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine gen(seed);
        std::uniform_real_distribution<float> dist;

        for (auto i = 0; i < vl; ++i)
            values[i] = dist(gen);
    }

    SemanticVector::SemanticVector(const ChatEngine *ce, const float *vs) : parent(ce), values(new float[ce->getVectorLength()]) {
        auto vl = ce->getVectorLength();

        for (auto i = 0; i < vl; ++i)
            values[i] = vs[i];
    }

    SemanticVector& SemanticVector::operator=(const SemanticVector &other) {
        parent = other.parent;
        values = other.values;
        return *this;
    }

    ChatEngine* SemanticVector::getParent() {
        return parent;
    }

    const float* SemanticVector::getValues() const {
        return values;
    }

    float SemanticVector::getValue(size_t ix) const {
        return values[ix];
    }

    void SemanticVector::setValues(const float *vs) {
        auto vl = parent->getVectorLength();

        for (auto i = 0; i < vl; ++i)
            values[i] = vs[i];
    }

    void SemanticVector::setValue(size_t ix, float value) {
        values[ix] = value;
    }

    WordInfo::WordInfo(ChatEngine *ce, tag_t t, const std::string &txt) : tag(t), text(txt), semvec(new SemanticVector(ce)) { }

    tag_t WordInfo::getTag() const {
        return tag;
    }

    std::string WordInfo::getText() const {
        return text;
    }

    const SemanticVector* WordInfo::getSemVec() const {
        return semvec;
    }

    void WordInfo::setSemVec(const SemanticVector &sv) {
        if (semvec)
            *semvec = sv;
        else {
            semvec = new SemanticVector(sv);
        }
    }

    ChatEngine::ChatEngine(unsigned pvl, unsigned pcn) : sourceFile("-"), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), dict(), infotbl() { }

    ChatEngine::ChatEngine(const std::string &src, unsigned pvl, unsigned pcn) : sourceFile(src), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), dict(), infotbl() { }

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

    Dictionary ChatEngine::getDict() const {
        return dict;
    }

    Infotbl ChatEngine::getInfotbl() const {
        return infotbl;
    }

    void ChatEngine::setSourceFile(const std::string &src) {
        sourceFile = src;
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
                toks.push_back(tkn);
            }
        }

        return Document(toks);
    }

    void ChatEngine::printDocuments() const {
        for (auto it = docs.begin(); it != docs.end(); ++it)
            it->printTokens();
    }

    void ChatEngine::preprocess() {
        auto fh = std::fstream(sourceFile);
        auto opts = re2::RE2::Options();

        opts.set_longest_match(false);
        opts.set_dot_nl(true);

        auto re = re2::RE2("\\<TITLE\\>(.*)\\</TITLE\\>.*\\<BODY\\>(.*)\\</BODY\\>", opts);
        std::size_t length;
        char* buffer;
        re2::StringPiece sp;

        fh.seekg(0, std::ios::end);
        length = fh.tellg();
        fh.seekg(0, std::ios::beg);
        buffer = new char[length];
        fh.read(buffer, length);
        fh.close();

        sp = re2::StringPiece(buffer);

        while (true) {
            std::string title;
            std::string body;

            if (!re2::RE2::FindAndConsume(&sp, re, &title, &body))
                break;

            docs.push_back(tokenize(body));
        }
    }
}
