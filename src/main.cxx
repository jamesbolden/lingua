#include <iostream>
#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine ce("../dataset/utf-8/reut2-000.sgm");
    ce.analyzeSemantics();
//    ce.printDocuments();

    std::cout << "This file contains " << ce.getDocs().size() << " documents" << std::endl;

    return 0;
}
