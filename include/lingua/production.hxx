#ifndef LINGUA_PRODUCTION_HXX
#define LINGUA_PRODUCTION_HXX

namespace lingua {
    class Production {
    public:
        Production(Prototype*, Prototype*, Prototype*);
    private:
        Prototype* trigger;
        Prototype* response;
        Prototype* info;
    };
}

#endif // LINGUA_PRODUCTION_HXX
