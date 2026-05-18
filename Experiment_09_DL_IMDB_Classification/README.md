# Experiment 09: Binary Classification using Deep Neural Network (IMDB)

## Aim
Classify movie reviews into "positive" and "negative" reviews based on text content using a Deep Neural Network. Use the IMDB dataset.

## Objective
- Understand text preprocessing and cleaning for NLP tasks
- Learn TF-IDF vectorization for converting text to numerical features
- Build a binary classification model with sigmoid output
- Understand binary crossentropy loss and its relationship to probability
- Evaluate classification using accuracy, confusion matrix, and classification report
- Test custom text predictions with confidence scores

## Theory

### What is Binary Classification?
Binary classification assigns input to one of two classes. Here: Positive (1) or Negative (0) movie review.

### IMDB Dataset
- **50,000** movie reviews (balanced: ~25,000 positive, ~25,000 negative)
- Reviews contain raw text with HTML tags, special characters
- Labels: `positive` and `negative` (converted to 1 and 0)

### Text Preprocessing Pipeline
1. **Text Cleaning**: Remove HTML tags, special characters; convert to lowercase
2. **TF-IDF Vectorization**: Convert text to numerical features using Term Frequency-Inverse Document Frequency
3. **Feature Selection**: Use top 5,000 most informative words

### Model Architecture
```
Input (5000 TF-IDF features)
    ↓
Dense Layer (128 neurons, ReLU)
    ↓
Dense Layer (64 neurons, ReLU)
    ↓
Output Layer (1 neuron, Sigmoid → probability)
```

### Key Concepts
| Concept | Description |
|---------|-------------|
| **TF-IDF** | Converts text to numerical vectors based on word importance |
| **ReLU** | Activation function: f(x) = max(0, x), used in hidden layers |
| **Sigmoid** | Outputs probability [0, 1] for binary classification |
| **Binary Crossentropy** | Loss function: -[y·log(p) + (1-y)·log(1-p)] |

## Algorithm
```
1. Load IMDB dataset from CSV
2. Explore and inspect data
3. Clean text (remove HTML, special chars, lowercase)
4. Convert labels to binary (positive=1, negative=0)
5. Vectorize text using TF-IDF (top 5000 features)
6. Split into training (80%) and testing (20%)
7. Build DNN model:
   - Input: 5000 TF-IDF features
   - Hidden: 128 → 64 neurons with ReLU
   - Output: 1 neuron with Sigmoid
8. Compile with binary_crossentropy loss
9. Train for 10 epochs
10. Evaluate on test data (accuracy, loss)
11. Plot accuracy and loss curves
12. Generate classification report and confusion matrix
13. Show sample predictions with confidence
14. Test with custom text input
```

## Prerequisites
```
pip install tensorflow numpy pandas matplotlib scikit-learn
```

## Code

```python
# ============================================================
# Experiment 09: IMDB Binary Classification using DNN
# ============================================================

# ---- Step 1: Import Libraries ----
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re

from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics import classification_report, confusion_matrix

import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Input

print("TensorFlow Version:", tf.__version__)

# ---- Step 2: Load Dataset ----
df = pd.read_csv("IMDB_Dataset.csv", encoding_errors='ignore',
                  on_bad_lines='skip', engine='python')
print(f"Dataset Shape: {df.shape}")
df.head()

# ---- Step 3: Explore Data ----
df = df.dropna()
print(f"Unique sentiments: {df['sentiment'].unique()}")
print(df['sentiment'].value_counts())

# ---- Step 4: Clean Text ----
def clean_text(text):
    text = str(text).lower()
    text = re.sub(r'<.*?>', '', text)         # remove HTML
    text = re.sub(r'[^a-zA-Z ]', '', text)    # remove special chars
    return text

df['review'] = df['review'].apply(clean_text)

# ---- Step 5: Convert Labels ----
df['sentiment'] = df['sentiment'].map({'positive': 1, 'negative': 0})

# ---- Step 6: TF-IDF Vectorization ----
vectorizer = TfidfVectorizer(max_features=5000)
X = vectorizer.fit_transform(df['review']).toarray()
y = df['sentiment']

# ---- Step 7: Train-Test Split ----
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# ---- Step 8: Build DNN Model ----
model = Sequential([
    Input(shape=(X_train.shape[1],)),
    Dense(128, activation='relu'),
    Dense(64, activation='relu'),
    Dense(1, activation='sigmoid')
])
model.summary()

# ---- Step 9: Compile ----
model.compile(optimizer='adam', loss='binary_crossentropy',
              metrics=['accuracy'])

# ---- Step 10: Train ----
history = model.fit(X_train, y_train, epochs=10, batch_size=32,
                    validation_data=(X_test, y_test), verbose=1)

# ---- Step 11: Evaluate ----
test_loss, test_acc = model.evaluate(X_test, y_test, verbose=0)
print(f"Test Loss: {test_loss:.4f}")
print(f"Test Accuracy: {test_acc:.4f} ({test_acc*100:.2f}%)")

# ---- Step 12: Plot Training History ----
fig, axes = plt.subplots(1, 2, figsize=(14, 5))
axes[0].plot(history.history['accuracy'], label='Training Accuracy')
axes[0].plot(history.history['val_accuracy'], label='Validation Accuracy')
axes[0].set_title('Model Accuracy'); axes[0].legend(); axes[0].grid(True)
axes[1].plot(history.history['loss'], label='Training Loss')
axes[1].plot(history.history['val_loss'], label='Validation Loss')
axes[1].set_title('Model Loss'); axes[1].legend(); axes[1].grid(True)
plt.tight_layout(); plt.show()

# ---- Step 13: Classification Report ----
y_pred = (model.predict(X_test, verbose=0) > 0.5).astype(int).flatten()
print(classification_report(y_test, y_pred, target_names=['Negative','Positive']))
print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred))

# ---- Step 14: Predict Custom Text ----
def predict_sentiment(sentence):
    cleaned = clean_text(sentence)
    vectorized = vectorizer.transform([cleaned]).toarray()
    prediction = model.predict(vectorized, verbose=0)[0][0]
    sentiment = "POSITIVE" if prediction > 0.5 else "NEGATIVE"
    confidence = prediction if prediction > 0.5 else (1 - prediction)
    return {"sentiment": sentiment, "confidence": f"{confidence*100:.2f}%"}

result = predict_sentiment("This movie is amazing!")
print(f"Sentiment: {result['sentiment']}, Confidence: {result['confidence']}")
```

## How to Run

### In Jupyter Notebook:
1. Open Jupyter Notebook
2. Open `IMDB.ipynb`
3. Ensure `IMDB_Dataset.csv` is in the same directory
4. Run cells sequentially (Shift + Enter)

### In Google Colab:
1. Go to https://colab.research.google.com
2. Upload `IMDB.ipynb` and `IMDB_Dataset.csv`
3. Run all cells sequentially

## Expected Output
```
Dataset Shape: (50000, 2)
Training samples: ~39700
Testing samples:  ~9900

Model Summary:
_________________________________________________________________
Layer (type)                Output Shape              Param #
=================================================================
dense (Dense)               (None, 128)               640,128
dense_1 (Dense)             (None, 64)                8,256
dense_2 (Dense)             (None, 1)                 65
=================================================================
Total params: 648,449

--- Model Evaluation ---
Test Loss:     ~0.27
Test Accuracy: ~0.88 (88%)
```

## Advantages
1. TF-IDF captures word importance across the corpus
2. Simple and fast architecture (~88% accuracy)
3. Handles variable-length text input
4. Can classify new, unseen reviews

## Applications
1. Sentiment analysis (product reviews)
2. Spam detection
3. Fake news detection
4. Customer feedback classification
5. Social media monitoring

## Viva Questions and Answers

**Q1: What is binary classification?**
A: Classifying input into one of two categories (positive/negative, spam/ham, etc.).

**Q2: What is TF-IDF?**
A: Term Frequency-Inverse Document Frequency. It measures how important a word is to a document relative to the entire corpus. High TF-IDF = word is frequent in the document but rare across all documents.

**Q3: Why clean the text before vectorization?**
A: To remove noise (HTML tags, special characters) that doesn't carry sentiment information, improving model accuracy.

**Q4: Why use Sigmoid in the output layer?**
A: Sigmoid outputs a probability between 0 and 1, perfect for binary classification. Output > 0.5 → Positive, < 0.5 → Negative.

**Q5: What is binary crossentropy?**
A: Loss function for binary classification: -[y·log(p) + (1-y)·log(1-p)]. Penalizes confident wrong predictions heavily.

**Q6: What is the difference between categorical and binary crossentropy?**
A: Binary crossentropy is for 2 classes with a single sigmoid output. Categorical crossentropy is for 3+ classes with softmax output.

**Q7: What is overfitting in text classification?**
A: Model memorizes training reviews but fails on new ones. Signs: training accuracy ~100% but validation accuracy plateaus or drops.

**Q8: How does TF-IDF differ from Bag of Words?**
A: Bag of Words counts word frequency. TF-IDF also considers how rare a word is across all documents, giving more weight to distinctive words.

**Q9: What accuracy can we expect?**
A: ~87-88% with this TF-IDF + DNN architecture. More complex models (LSTM, Transformer) can achieve 90%+.

**Q10: Why use max_features=5000 in TF-IDF?**
A: To limit vocabulary size and reduce dimensionality. Using too many features increases computation and may cause overfitting.

## Conclusion
We successfully implemented binary classification of IMDB movie reviews using a Deep Neural Network with TF-IDF vectorization and TensorFlow/Keras. The model with Dense layers achieves ~88% accuracy, demonstrating effective sentiment analysis using simple text preprocessing and a DNN.
