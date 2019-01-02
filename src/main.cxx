#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine ce("../dataset/reut2-000.sgm");
    ce.analyzeSemantics();

    return 0;
}
