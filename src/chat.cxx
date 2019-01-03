#include "lingua/chat.hxx"
#include <Python.h>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/tokenizer.hpp>
#include <random>
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
    }

    void ChatEngine::printDocuments() const {
        for (auto it = docs.begin(); it != docs.end(); ++it)
            it->printTokens();
    }

    void ChatEngine::preprocess() {
        PyObject *pName;
        PyObject *pModule;
        PyObject *pParseFunc;
        PyObject *pArgs;
        PyObject *pValue;
        PyObject *syspath;
        PyObject *path;

        Py_Initialize();
        syspath = PySys_GetObject("path");
        path = PyUnicode_FromString(".");
        PyList_Insert(syspath, 0, path);
        pModule = PyImport_ImportModule("sgml");

        Py_DECREF(syspath);
        Py_DECREF(path);

        if (pModule != nullptr) {
            pParseFunc = PyObject_GetAttrString(pModule, "parseSGML");

            if (pParseFunc && PyCallable_Check(pParseFunc)) {
                pArgs = PyTuple_New(1);
                pValue = PyUnicode_FromString(sourceFile.c_str());
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    std::cout << "lingua: Error while initializing python interface" << std::endl;
                    std::exit(1);
                }

                PyTuple_SetItem(pArgs, 0, pValue);
                pValue = PyObject_CallObject(pParseFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != nullptr) {
                    auto len = PyList_Size(pValue);

                    for (auto i = 0; i < len; ++i) {
                        PyObject* item = PyList_GetItem(pValue, i);

                        if (!PyUnicode_Check(item)) {
                            std::cout << "lingua: python returned broken list" << std::endl;
                            std::exit(1);
                        }

                        const char* cstr = PyUnicode_AS_DATA(item);
                        if (cstr == nullptr) {
                            std::cout << "lingua: python returned null string" << std::endl;
                            std::exit(1);
                        }
                        std::string str(cstr);
                        Document doc(tokenize(str));
                        std::cout << doc << std::endl;
                        docs.push_back(doc);
                        Py_DECREF(item);
                    }
                    Py_DECREF(pValue);
                }
                Py_DECREF(pParseFunc);
                Py_DECREF(pModule);
            }
        }
        else {
            std::cout << "lingua: Error while initializing python interface" << std::endl;
            std::exit(1);
        }

        Py_Finalize();
    }
}
