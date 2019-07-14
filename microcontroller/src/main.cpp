#include <Arduino.h>
#include <NeuralNetwork.h>

#define LED_A 2
#define LED_B 4
#define INPUT_A 22
#define INPUT_B 23

#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

const unsigned int layers[] = {2, 4, 1};

const float inputs[4][2] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

const float expectedOutput[4][1] = {
  {0},
  {1},
  {1},
  {0}
};

NeuralNetwork NN(layers, NumberOf(layers));

void setup() {
  Serial.begin(9600);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(INPUT_A, INPUT);
  pinMode(INPUT_B, INPUT);

  // Train a neural network
  const int epochs = 3000;
  for (int i = 0; i < epochs; ++i) {
    for (int j = 0; j < NumberOf(inputs); ++j) {
      NN.FeedForward(inputs[j]);
      NN.BackProp(expectedOutput[j]);
    }
  }

  // Evaluate all the inputs and output to serial
  for (int i = 0; i < NumberOf(inputs); ++i) {
    float* outputs = NN.FeedForward(inputs[i]);
    Serial.println(outputs[0], 7);
  }

  // Output weights to serial
  NN.print();
}

void loop() {
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);

  const float input[2] = {
    digitalRead(INPUT_A) == HIGH ? 1.0f : 0.0f,
    digitalRead(INPUT_B) == HIGH ? 1.0f : 0.0f
  };
  float* classification = NN.FeedForward(input);
  //Serial.println(classification[0]);

  if (classification == NULL) {
    Serial.println("Error: classification is NULL");
  } else if (classification[0] >= 0.5) {
    digitalWrite(LED_A, HIGH);
  } else {
    digitalWrite(LED_B, HIGH);
  }

  delay(250);
}
