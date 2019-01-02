#ifndef LINGUA_NETWORK_HXX
#define LINGUA_NETWORK_HXX

namespace lingua {
    class Node {
    public:
        Node(size_t);
        Node(size_t, const std::vector<double>&, double);

        std::vector<Node*> getInputs();
        std::vector<double> getWeights();
        double getBias();
        double getOutput();

        void setBias(double);
        void setOutput(double);

    private:
        std::vector<Node*> inputs;
        std::vector<double> weights;
        double bias;
        double output;
    };

    class Network {
    public:
        Network(size_t, size_t, size_t, ...);

    private:
        std::vector<Node*> inputLayer;
        std::vector<std::vector<Node*>> hiddenLayers;
        std::vector<Node*> outputLayer;
    };
}

#endif // LINGUA_NETWORK_HXX
