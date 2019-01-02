#include "lingua/chat.hxx"
#include <random>
#include <fstream>
#include <regex>
#include <cstdio>

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
        auto vl = ce->getVectorLength();

        for (auto i = 0; i < vl; ++i)
            values[i] = vs[i];
    }

    void SemanticVector::setValue(size_t ix, float value) {
        values[ix] = value;
    }

    WordInfo::WordInfo(tag_t t, const std::string &txt) : tag(t), text(txt), semvec(new SemanticVector) { }

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
            semvec = new SemanticVector;
            *semvec = sv;
        }
    }

    Document::Document(const ChatEngine *ce, const std::string &ti, const std::string &te) : parent(ce), title(ti), text(te), tokens(ce->tokenize(te)) { }

    ChatEngine* Document::getParent() {
        return parent;
    }

    std::string Document::getTitle() const {
        return title;
    }

    std::string Document::getText() const {
        return text;
    }

    std::vector<tag_t> Document::getTokens() const {
        return tokens;
    }

    ChatEngine::ChatEngine(unsigned pvl = PARAM_DEFAULT_VECTOR_LENGTH, unsigned pcn = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD) : sourceFile("-"), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), dict(), infotbl() { }

    ChatEngine::ChatEngine(unsigned pvl = PARAM_DEFAULT_VECTOR_LENGTH, unsigned pcn = PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, const std::string &src) : sourceFile(src), docs(), pVectorLength(pvl), pContextNeighborhood(pcn), dict(), infotbl() { }

    void ChatEngine::analyzeSemantics() {
        preprocess();
    }

    std::string ChatEngine::getSourceFile() const {
        return sourceFile;
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

    std::vector<tag_t> tokenize(const std::string &text) {
        std::vector<tag_t> tags;
        char buf[40];
        bool toksLeft = true;

        while (toksLeft) {
            auto n = std::sscanf(text.c_str(), "%s", buf);
            std::string s(buf);

            while (s.back() == "," || s.back() == "." || s.back() == "!" || s.back() == "?")
                s.pop_back();

            if (n == EOF)
                toksLeft = false;

            if (dict.find(s) != dict.end()) {
                tags.push_back(dict[s]);
            } else {
                tag_t tag = dict.size();
                dict[s] = tag;
                tags.push_back(tag);
            }
        }
    }

    void ChatEngine::preprocess() {
        std::ifstream file(sourceFile);
        std::regex regex("<TITLE>(.+)</TITLE>.+<BODY>(.+)</BODY>");
        size_t fileSize;
        char *buf;
        std::string fileData;

        file.seekg(0, file.end);
        fileSize = file.tellg();
        file.seekg(0, file.beg);

        buf = new char[fileSize];
        file.read(buf, fileSize);

        fileData = std::string(buf);
        bool matched = true;

        while (matched) {
            std::smatch m;
            size_t ix;
            matched = std::regex_search(fileData, m, regex);

            if (matched) {
                ix = m.prefix().str().length() + m.str().length();
                fileData = std::string(fileData, ix);

                Document doc(this, m[0].str(), m[1].str());
                docs.push_back(doc);
            }
        }
    }
}
