#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <random>
#include <cstdlib>

using std::vector;
using std::uniform_real_distribution;
using std::default_random_engine;

class NeuralNet {

private:
    // relu function
    inline double h(double x) {
        return std::max(0.0, x);
    }

public:
    vector<vector<double> > Mh, Mo;
    int L, M, K;
    // number of neurons in input layer, middle layer and output layer, respectively
    NeuralNet(int L, int M, int K) {
        this->L = L;
        this->M = M;
        this->K = K;
        Mh = vector<vector<double> >(M, vector<double>(L + 1));
        Mo = vector<vector<double> >(K, vector<double>(M + 1));
        randomizeNet();
    }

    void randomizeNet() {
        uniform_real_distribution ur(-1000.0, 1000.0);
        default_random_engine re;
        for(int i = 0; i < M; ++i) {
            for(int j = 0; j < L + 1; ++j) {
                Mh[i][j] = ur(re);
            }
        }

        for(int i = 0; i < K; ++i) {
            for(int j = 0; j < M + 1; ++j) {
                Mo[i][j] = ur(re);
            }
        }
    }

    vector<double> propagate(vector<double> I) {
        if(I.size() != L) throw std::runtime_error("Input size of neural network is wrong");
        vector<double> H(M);
        vector<double> O(K);

        for(int m = 0; m < M; ++m) {
            H[m] = 0;
            for(int l = 0; l < L; ++l) {
                H[m] += Mh[m][l] * I[l];
            }
            H[m] += Mh[m][L]; // sum bias
            H[m] = h(H[m]); // activation function
        }

        for(int k = 0; k < K; ++k) {
            O[k] = 0;
            for(int m = 0; m < M; ++m) {
                O[k] += Mo[k][m] * H[m];
            }
            O[k] += Mo[k][M]; // sum bias
            O[k] = h(O[k]); // activation function
        }

        return O;
    }

};

#endif