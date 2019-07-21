#include <Arduino.h>
#include <NeuralNetwork.h>

#define LED_TRAINING 5
#define MODE_SWITCH 13
#define TRAIN_STORE_BUTTON 12
#define TRAIN_INPUT_A 18
#define TRAIN_INPUT_B 19
#define TRAIN_OUTPUT 21
#define NN_OUTPUT_OFF 2
#define NN_OUTPUT_ON 4
#define NN_INPUT_A 22
#define NN_INPUT_B 23

#define MODE_EVALUATE 0
#define MODE_TRAIN 1

int mode = MODE_EVALUATE;

#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

const unsigned int layers[] = {2, 4, 1};

const float inputs[4][2] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

float expectedOutput[4][1] = {
  {0},
  {1},
  {1},
  {0}
};

NeuralNetwork NN(layers, NumberOf(layers));

void train() {
  digitalWrite(LED_TRAINING, HIGH);

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
  digitalWrite(LED_TRAINING, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(NN_OUTPUT_OFF, OUTPUT);
  pinMode(NN_OUTPUT_ON, OUTPUT);
  pinMode(LED_TRAINING, OUTPUT);
  pinMode(MODE_SWITCH, OUTPUT);
  pinMode(TRAIN_STORE_BUTTON, OUTPUT);
  pinMode(TRAIN_INPUT_A, INPUT);
  pinMode(TRAIN_INPUT_B, INPUT);
  pinMode(TRAIN_OUTPUT, INPUT);
  pinMode(NN_INPUT_A, INPUT);
  pinMode(NN_INPUT_B, INPUT);

  train();
}


void loop() {
  digitalWrite(NN_OUTPUT_OFF, LOW);
  digitalWrite(NN_OUTPUT_ON, LOW);

  if (digitalRead(MODE_SWITCH) == HIGH) {
    mode = MODE_TRAIN;

    if (digitalRead(TRAIN_STORE_BUTTON) == HIGH) {
      int row;
      int a = digitalRead(TRAIN_INPUT_A);
      int b = digitalRead(TRAIN_INPUT_B);
      if (a == LOW && b == LOW) {
        row = 0;
      } else if (a == LOW && b == HIGH) {
        row = 1;
      } else if (a == HIGH && b == LOW) {
        row = 2;
      } else {
        row = 3;
      }

      *expectedOutput[row] = digitalRead(TRAIN_OUTPUT) == HIGH ? 1.0f : 0.0f;
    }
  } else {
    if (mode == MODE_TRAIN) {
      train();
      mode = MODE_EVALUATE;
    }

    const float input[2] = {
      digitalRead(NN_INPUT_A) == HIGH ? 1.0f : 0.0f,
      digitalRead(NN_INPUT_B) == HIGH ? 1.0f : 0.0f
    };
    float* classification = NN.FeedForward(input);
    if (classification == NULL) {
      Serial.println("Error: classification is NULL");
    } else if (classification[0] >= 0.5) {
      digitalWrite(NN_OUTPUT_OFF, HIGH);
    } else {
      digitalWrite(NN_OUTPUT_ON, HIGH);
    }
  }

  Serial.println("--train store--mode--a--b--out--");
  Serial.println(digitalRead(TRAIN_STORE_BUTTON));
  Serial.println(digitalRead(MODE_SWITCH));
  Serial.println(digitalRead(TRAIN_INPUT_A));
  Serial.println(digitalRead(TRAIN_INPUT_B));
  Serial.println(digitalRead(TRAIN_OUTPUT));

  delay(250);
}
