#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <vector>
#include <random>
#include <cstdlib>
#include <stdexcept>
#include <fstream>

using std::vector;

class NeuralNet {

private:
    // relu function
    inline double h(double x) {
        return std::max(0.0, x);
    }

public:
    vector<vector<double> > Mh, Mo;
    int L, M, K;

    NeuralNet(){} // default constructor
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
        RandomNumber * gen = RandomNumber::getGenerator();
        for(int i = 0; i < M; ++i) {
            for(int j = 0; j < L + 1; ++j) {
                Mh[i][j] = gen->randDouble(-1000.0, 1000.0);
            }
        }

        for(int i = 0; i < K; ++i) {
            for(int j = 0; j < M + 1; ++j) {
                Mo[i][j] = gen->randDouble(-100.0, 100.0);
            }
        }
    }

    void print() {
        printf("Neural Net:\n");
        for(int i = 0; i < M; ++i) {
            for(int j = 0; j < L + 1; ++j) {
                std::cout << Mh[i][j] << ' ';
            }
        }
        std::cout << std::endl;

        for(int i = 0; i < K; ++i) {
            for(int j = 0; j < M + 1; ++j) {
                std::cout << Mo[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
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

    void writeToFile(std::ofstream & file) {
        for(int m = 0; m < M; ++m) {
            for(int l = 0; l < L + 1; ++l) {
                file.write((char *) &Mh[m][l], sizeof(Mh[m][l]));
            }
        }
        for(int k = 0; k < K; ++k) {
            for(int m = 0; m < M + 1; ++m) {
                file.write((char *) &Mo[k][m], sizeof(Mo[k][m]));
            }
        }
    }

    void readFromFile(std::ifstream & file) {
        for(int m = 0; m < M; ++m) {
            for(int l = 0; l < L + 1; ++l) {
                file.read((char *) &Mh[m][l], sizeof(Mh[m][l]));
            }
        }
        for(int k = 0; k < K; ++k) {
            for(int m = 0; m < M + 1; ++m) {
                file.read((char *) &Mo[k][m], sizeof(Mo[k][m]));
            }
        }
    }

    static NeuralNet mix(NeuralNet a, NeuralNet b) {
        NeuralNet ret(a);
        for(int m = 0; m < ret.M; ++m) {
            for(int l = 0; l < ret.L + 1; ++l) {
                ret.Mh[m][l] += b.Mh[m][l];
                ret.Mh[m][l] /= 2.0;
            }
        }

        for(int k = 0; k < ret.K; ++k) {
            for(int m = 0; m < ret.M + 1; ++m) {
                ret.Mo[k][m] += b.Mo[k][m];
                ret.Mo[k][m] /= 2.0;
            }
        }
        return ret;
    }

    static void crossover(NeuralNet & a, NeuralNet & b) {
        RandomNumber * gen = RandomNumber::getGenerator();
        for(int m = 0; m < a.M; ++m) {
            for(int l = 0; l < a.L + 1; ++l) {
                if(gen->randInt(0, 1))
                    std::swap(a.Mh[m][l], b.Mh[m][l]);
            }
        }

        for(int k = 0; k < a.K; ++k) {
            for(int m = 0; m < a.M + 1; ++m) {
                if(gen->randInt(0, 1))
                    std::swap(a.Mo[k][m], b.Mo[k][m]);
            }
        }
    }

};

#endif