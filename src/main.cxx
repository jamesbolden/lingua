#include "lingua/chat.hxx"

int main(int argc, char **argv) {
    lingua::ChatEngine ce(PARAM_DEFAULT_VECTOR_LENGTH, PARAM_DEFAULT_CONTEXT_NEIGHBORHOOD, "reut2-000.sgm");

    return 0;
}
