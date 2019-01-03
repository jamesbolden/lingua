#include <iostream>
#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine::initialize("../dataset/utf-8/reut2-000.sgm");
    lingua::ChatEngine::instance->analyzeSemantics();
//    ce.printDocuments();

    return 0;
}
