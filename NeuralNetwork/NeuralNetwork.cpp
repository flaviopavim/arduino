#include <math.h>

// Função de ativação sigmoide
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Classe para a camada de neurônios
class Layer {
public:
    int input_size; // Tamanho da entrada
    int output_size; // Tamanho da saída
    double** weights; // Pesos
    double* biases; // Viéses
    double* activations; // Ativações

    // Construtor
    Layer(int input_size, int output_size) {
        this->input_size = input_size;
        this->output_size = output_size;

        // Inicialização aleatória dos pesos
        weights = new double*[input_size];
        for (int i = 0; i < input_size; ++i) {
            weights[i] = new double[output_size];
            for (int j = 0; j < output_size; ++j) {
                weights[i][j] = random(-1000, 1000) / 1000.0; // Inicialização aleatória entre -1 e 1
            }
        }

        // Inicialização dos viéses como zero
        biases = new double[output_size];
        for (int i = 0; i < output_size; ++i) {
            biases[i] = 0.0;
        }

        // Inicialização das ativações como zero
        activations = new double[output_size];
        for (int i = 0; i < output_size; ++i) {
            activations[i] = 0.0;
        }
    }

    // Cálculo das ativações
    void forward(double* input) {
        for (int i = 0; i < output_size; ++i) {
            activations[i] = 0.0;
            for (int j = 0; j < input_size; ++j) {
                activations[i] += input[j] * weights[j][i];
            }
            activations[i] += biases[i];
            activations[i] = sigmoid(activations[i]);
        }
    }
};

// Função principal
void setup() {
    Serial.begin(9600); // Inicializa a comunicação serial

    // Parâmetros da rede neural
    int input_size = 2;
    int hidden_size = 3;
    int output_size = 1;

    // Dados de entrada
    double input[2] = {0.5, 0.7};

    // Camada oculta
    Layer hidden_layer(input_size, hidden_size);
    hidden_layer.forward(input);

    // Camada de saída
    Layer output_layer(hidden_size, output_size);
    output_layer.forward(hidden_layer.activations);

    // Saída da rede neural
    Serial.print("Saída da rede neural: ");
    Serial.println(output_layer.activations[0]);
}

void loop() {
    // Seu código para o loop principal do Arduino, se necessário
}