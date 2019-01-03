#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine ce("../dataset/reut2-000.xml");
    ce.analyzeSemantics();
    ce.printDocuments();

    return 0;
}
