#include <math.h>

// Sigmoid activation function
// This function is used to squash the input into a range between 0 and 1.
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Class representing a neural network layer
class Layer {
public:
    int input_size;      // Number of inputs to the layer
    int output_size;     // Number of outputs from the layer
    double** weights;    // Weights connecting inputs to outputs
    double* biases;      // Bias values for each neuron
    double* activations; // Activated outputs from the layer

    // Constructor for initializing the layer
    Layer(int input_size, int output_size) {
        this->input_size = input_size;   // Set the number of inputs
        this->output_size = output_size; // Set the number of outputs

        // Initialize weights with random values between -1 and 1
        weights = new double*[input_size];
        for (int i = 0; i < input_size; ++i) {
            weights[i] = new double[output_size];
            for (int j = 0; j < output_size; ++j) {
                weights[i][j] = random(-1000, 1000) / 1000.0; // Random initialization
            }
        }

        // Initialize biases to zero
        biases = new double[output_size];
        for (int i = 0; i < output_size; ++i) {
            biases[i] = 0.0;
        }

        // Initialize activations to zero
        activations = new double[output_size];
        for (int i = 0; i < output_size; ++i) {
            activations[i] = 0.0;
        }
    }

    // Forward pass to calculate activations
    void forward(double* input) {
        for (int i = 0; i < output_size; ++i) {
            activations[i] = 0.0; // Reset the activation for this neuron
            for (int j = 0; j < input_size; ++j) {
                activations[i] += input[j] * weights[j][i]; // Weighted sum of inputs
            }
            activations[i] += biases[i]; // Add the bias
            activations[i] = sigmoid(activations[i]); // Apply sigmoid activation
        }
    }
};

// Main function
void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging

    // Neural network parameters
    int input_size = 2;   // Number of input features
    int hidden_size = 3;  // Number of neurons in the hidden layer
    int output_size = 1;  // Number of neurons in the output layer

    // Input data
    double input[2] = {0.5, 0.7}; // Example input vector

    // Create and process the hidden layer
    Layer hidden_layer(input_size, hidden_size);
    hidden_layer.forward(input); // Perform forward pass with input data

    // Create and process the output layer
    Layer output_layer(hidden_size, output_size);
    output_layer.forward(hidden_layer.activations); // Forward pass with hidden layer output

    // Output the neural network's result
    Serial.print("Neural network output: ");
    Serial.println(output_layer.activations[0]); // Display the output activation
}

void loop() {
    // Code for the main Arduino loop, if needed
}
