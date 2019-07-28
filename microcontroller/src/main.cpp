#include <Arduino.h>
#include <NeuralNetwork.h>

#define LED_TRAINING_PROGRESS 5
#define LED_TRAIN_MODE_ENABLED 23
#define TRAINING_MODE_SWITCH 13
#define INPUT_A 18
#define INPUT_B 19
#define INPUT_C 21
#define OUTPUT_RED 2
#define OUTPUT_GREEN 15
#define OUTPUT_BLUE 4
#define TRAINING_RED 33
#define TRAINING_GREEN 32
#define TRAINING_BLUE 35

#define MODE_EVALUATE 0
#define MODE_TRAINING 1

int mode = MODE_EVALUATE;
int trainingChoice = -1;

#define countof(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

const unsigned int layers[] = {3, 6, 2};

const float inputs[8][3] = {
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 1},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 0},
  {1, 1, 1}
};

float expectedOutput[8][2] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1},
  {1, 1},
  {0, 1},
  {1, 0},
  {0, 1},
};

NeuralNetwork NN(layers, countof(layers));

void updateOutput(const float* classification) {
  if (classification == NULL) {
    Serial.println("Error: classification is NULL");
  } else {
    Serial.printf("Input {%d, %d, %d}\n", digitalRead(INPUT_A), digitalRead(INPUT_B), digitalRead(INPUT_C));
    Serial.printf("Classification {%f, %f}\n", classification[0], classification[1]);

    bool p = classification[0] >= 0.5 ? true : false;
    bool q = classification[1] >= 0.5 ? true : false;

    Serial.printf("Decision {%d%d}\n", p, q);

    if (!p && !q) {
      digitalWrite(OUTPUT_RED, LOW);
      digitalWrite(OUTPUT_BLUE, LOW);
      digitalWrite(OUTPUT_GREEN, LOW);
    } else if (!p && q) {
      digitalWrite(OUTPUT_RED, HIGH);
      digitalWrite(OUTPUT_BLUE, LOW);
      digitalWrite(OUTPUT_GREEN, LOW);
    } else if (p && !q) {
      digitalWrite(OUTPUT_RED, LOW);
      digitalWrite(OUTPUT_BLUE, HIGH);
      digitalWrite(OUTPUT_GREEN, LOW);
    } else if (p && q) {
      digitalWrite(OUTPUT_RED, LOW);
      digitalWrite(OUTPUT_BLUE, LOW);
      digitalWrite(OUTPUT_GREEN, HIGH);
    }
  }
}

void train() {
  digitalWrite(LED_TRAINING_PROGRESS, HIGH);

  const int epochs = 3000;
  for (int i = 0; i < epochs; ++i) {
    for (int j = 0; j < countof(inputs); ++j) {
      NN.FeedForward(inputs[j]);
      NN.BackProp(expectedOutput[j]);
    }
  }

  for (int i = 0; i < countof(inputs); ++i) {
    const float* outputs = NN.FeedForward(inputs[i]);
    updateOutput(outputs);
    delay(100);
  }

  NN.print();

  digitalWrite(LED_TRAINING_PROGRESS, LOW);
}

void reset() {
  for (int i = 0; i < countof(expectedOutput); ++i) {
    for (int j = 0; j < countof(expectedOutput[i]); ++j) {
      expectedOutput[i][j] = 0;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_TRAINING_PROGRESS, OUTPUT);
  pinMode(LED_TRAIN_MODE_ENABLED, OUTPUT);
  pinMode(TRAINING_MODE_SWITCH, INPUT);
  pinMode(INPUT_A, INPUT);
  pinMode(INPUT_B, INPUT);
  pinMode(INPUT_C, INPUT);
  pinMode(OUTPUT_RED, OUTPUT);
  pinMode(OUTPUT_GREEN, OUTPUT);
  pinMode(OUTPUT_BLUE, OUTPUT);
  pinMode(TRAINING_RED, INPUT);
  pinMode(TRAINING_GREEN, INPUT);
  pinMode(TRAINING_BLUE, INPUT);

  train();
}

void loop() {
  const int a = digitalRead(INPUT_A) == HIGH ?  1 : 0;
  const int b = digitalRead(INPUT_B) == HIGH ?  1 : 0;
  const int c = digitalRead(INPUT_C) == HIGH ?  1 : 0;
  const int input = (a << 2) + (b << 1) + c;

  if (mode == MODE_TRAINING) {
    if (digitalRead(TRAINING_MODE_SWITCH) == LOW) {
      train();
      mode = MODE_EVALUATE;
      digitalWrite(LED_TRAIN_MODE_ENABLED, LOW);
      delay(1000);
      Serial.println("Evaluation mode enabled");
      return;
    }

    const bool red = digitalRead(TRAINING_RED);
    const bool green = digitalRead(TRAINING_GREEN);
    const bool blue = digitalRead(TRAINING_BLUE);

    digitalWrite(OUTPUT_RED, red);
    digitalWrite(OUTPUT_GREEN, green);
    digitalWrite(OUTPUT_BLUE, blue);

    if (input > 0) {
      if (red == HIGH) {
        expectedOutput[input][0] = 0;
        expectedOutput[input][1] = 1;
      } else if (green == HIGH) {
        expectedOutput[input][0] = 1;
        expectedOutput[input][1] = 0;
      } else if (blue == HIGH) {
        expectedOutput[input][0] = 1;
        expectedOutput[input][1] = 1;
      }
    }
  } else {
    if (digitalRead(TRAINING_MODE_SWITCH) == HIGH) {
      reset();
      mode = MODE_TRAINING;
      digitalWrite(LED_TRAIN_MODE_ENABLED, HIGH);
      delay(1000);
      Serial.println("Training mode enabled");
      return;
    }

    const float* classification = NN.FeedForward(inputs[input]);
    updateOutput(classification);
  }

  delay(250);
}
