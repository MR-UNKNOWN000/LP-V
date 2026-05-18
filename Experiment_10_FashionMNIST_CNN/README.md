# Experiment 10: Fashion MNIST Clothing Classifier using CNN

## Aim
Use MNIST Fashion Dataset and create a classifier to classify fashion clothing into categories.

## Objective
- Understand CNN architecture and its superiority over DNNs for image classification
- Learn convolution, pooling, and flattening operations
- Implement multi-layer CNN with regularization (Dropout)
- Understand activation functions: ReLU (hidden), Softmax (output)
- Evaluate multi-class classification using accuracy and confusion matrix
- Preprocess image data: normalization and reshaping

## Theory

### What is Fashion MNIST?
Fashion MNIST is a dataset of 70,000 grayscale images (28×28 pixels) of 10 clothing categories. It is a more challenging drop-in replacement for the classic MNIST digit dataset.

### 10 Categories
| Label | Category |
|-------|----------|
| 0 | T-shirt/top |
| 1 | Trouser |
| 2 | Pullover |
| 3 | Dress |
| 4 | Coat |
| 5 | Sandal |
| 6 | Shirt |
| 7 | Sneaker |
| 8 | Bag |
| 9 | Ankle boot |

### CNN Architecture
```
Input (28×28×1)
    ↓
Conv2D(32, 3×3, relu) → MaxPool(2×2)
    ↓
Conv2D(64, 3×3, relu) → MaxPool(2×2)
    ↓
Conv2D(128, 3×3, relu)
    ↓
Flatten → Dropout(0.5)
    ↓
Dense(128, relu) → Dropout(0.3)
    ↓
Dense(10, softmax)
```

## Algorithm

```
1. Load Fashion MNIST dataset from keras.datasets
2. Explore data: print shapes, visualize sample images
3. Normalize pixel values: X_train/255.0, X_test/255.0 (convert to 0-1 range)
4. Build CNN model:
   - Conv2D(32 filters, 3×3) + ReLU + MaxPool(2×2)
   - Conv2D(64 filters, 3×3) + ReLU + MaxPool(2×2)
   - Conv2D(128 filters, 3×3) + ReLU
   - Flatten to 1D
   - Dense(128) + ReLU + Dropout(0.5)
   - Dense(10) + Softmax
5. Compile with Adam optimizer, sparse_categorical_crossentropy loss
6. Train on X_train, y_train for 10 epochs with batch_size=128
7. Evaluate on X_test, y_test
8. Plot accuracy and loss curves
9. Generate confusion matrix
10. Display sample predictions
```

## Code Explanation

### Loading Data:
```python
(X_train, y_train), (X_test, y_test) = keras.datasets.fashion_mnist.load_data()
```
Loads Fashion MNIST with 60,000 training and 10,000 test images (28×28 grayscale).

### Normalization:
```python
X_train = X_train.astype('float32') / 255.0
```
Converts pixel values from [0, 255] to [0, 1] for faster training convergence.

### Conv2D Layer:
```python
model.add(Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1)))
```
Applies 32 filters (3×3 kernels) using ReLU activation to extract image features like edges.

### MaxPool2D Layer:
```python
model.add(MaxPool2D((2, 2)))
```
Reduces spatial dimensions by half (28→14→7); retains important features; reduces computation.

### Flatten Layer:
```python
model.add(Flatten())
```
Converts 2D feature maps to 1D vector for dense layer input.

### Dropout Layer:
```python
model.add(Dropout(0.5))
```
Randomly disables 50% of neurons during training to prevent overfitting; improves generalization.

### Softmax Output:
```python
model.add(Dense(10, activation='softmax'))
```
Outputs 10 probabilities (one per class); softmax ensures they sum to 1; highest probability is prediction.

## How to Run

### In Google Colab:
1. Go to https://colab.research.google.com
2. Upload the `.ipynb` file
3. Run all cells (Runtime → Run All)

### In Jupyter Notebook:
1. Install: `pip install tensorflow numpy matplotlib scikit-learn seaborn`
2. Run: `jupyter notebook fashion_mnist_classifier.ipynb`

## Expected Output
- Training Accuracy: ~93-95%
- Test Accuracy: ~90-92%

## Advantages
1. **Spatial Feature Learning**: Conv2D filters automatically learn spatial features (edges, textures, shapes) without manual feature engineering
2. **Parameter Reduction**: Convolutional layers share weights, reducing parameters vs fully connected layers
3. **Translation Invariance**: CNN can recognize patterns regardless of position in image due to filter sliding
4. **Hierarchical Representation**: Early layers learn simple features; deeper layers combine them into complex patterns
5. **Regularization**: Dropout prevents overfitting; Pooling reduces overfitting by reducing feature map size
6. **Scalability**: Works well for larger image datasets with good generalization

## Applications
1. **Fashion/Clothing Recognition**: E-commerce product categorization, virtual try-on systems
2. **Retail Analytics**: Inventory management, customer behavior analysis
3. **Medical Imaging**: Disease detection, abnormality classification
4. **Autonomous Vehicles**: Traffic sign recognition, pedestrian detection
5. **Surveillance Systems**: Object detection, activity classification
6. **Quality Control**: Manufacturing defect detection
7. **Gesture Recognition**: Sign language interpretation, hand pose estimation

## Viva Questions and Answers

**Q1: What is Fashion MNIST?**
A: A dataset of 70,000 grayscale 28×28 images of 10 clothing categories.

**Q2: Why CNN over regular DNN?**
A: CNNs detect spatial features (edges, textures) using convolutions. DNNs lose spatial info.

**Q3: What does Conv2D do?**
A: Applies learnable filters to extract image features like edges and patterns.

**Q4: What is MaxPooling?**
A: Reduces spatial dimensions by taking max value in each pooling window.

**Q5: Why normalize to 0-1?**
A: Helps the model train faster and converge better.

**Q6: What is Dropout?**
A: Randomly disables neurons during training to prevent overfitting.

**Q7: What is Softmax?**
A: Converts outputs to probabilities summing to 1.

**Q8: What is sparse_categorical_crossentropy?**
A: Loss function for integer labels (not one-hot encoded).

**Q9: What is a confusion matrix?**
A: Table showing correct vs incorrect predictions per class.

**Q10: How to improve accuracy?**
A: Data augmentation, deeper networks, batch normalization, transfer learning.

## Conclusion
We successfully classified Fashion MNIST clothing into 10 categories using CNN with ~90-92% test accuracy.
