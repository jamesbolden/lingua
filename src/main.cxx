#include <iostream>
#include "lingua/chat.hxx"

#define CORPUS_FILE_STRING  "../dataset/utf-8/reut2-000.sgm"

int main(int argc, char **argv) {
    lingua::ChatEngine::initialize(CORPUS_FILE_STRING);
    lingua::ChatEngine::instance->analyzeSemantics();
//    ce.printDocuments();

    std::cout << CORPUS_FILE_STRING " contains " << lingua::ChatEngine::instance->getDocs().size() << " documents" << std::endl;

    return 0;
}
