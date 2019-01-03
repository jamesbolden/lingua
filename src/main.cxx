#include <iostream>
#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine::initialize("../dataset/utf-8/reut2-000.sgm");
    lingua::ChatEngine::instance->analyzeSemantics();
//    ce.printDocuments();

    std::cout << "This file contains " << lingua::ChatEngine::instance->getDocs().size() << " documents" << std::endl;

    return 0;
}
