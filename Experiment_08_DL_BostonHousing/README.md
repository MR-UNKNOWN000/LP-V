# Experiment 08: Boston Housing Price Prediction using Deep Neural Network

## Aim
Implement Boston housing price prediction problem by Linear Regression using Deep Neural Network.

## Objective
- Understand regression vs classification tasks in deep learning
- Learn data preprocessing: splitting, scaling, and normalization
- Build a multi-layer neural network with hidden layers and ReLU activation
- Understand loss functions (MSE) and optimizers (Adam) for regression
- Evaluate model performance using multiple metrics (MAE, RMSE, R²)
- Visualize training history and actual vs predicted values

## Theory

### What is Linear Regression?
Linear Regression is a method to predict a continuous output variable based on input features. The model learns a linear relationship: `y = wx + b`

### What is a Deep Neural Network (DNN)?
A DNN consists of multiple layers of neurons. Each layer transforms the input using weights, biases, and activation functions. By stacking multiple hidden layers with non-linear activations (like ReLU), the network can learn complex, non-linear mappings from inputs to outputs — while the final output layer uses a linear activation to perform regression (predicting a continuous value).

### Boston Housing Dataset
- **Samples**: 506
- **Features**: 13 (CRIM, ZN, INDUS, CHAS, NOX, RM, AGE, DIS, RAD, TAX, PTRATIO, B, LSTAT)
- **Target**: MEDV — Median house price (in $1000s)

### Network Architecture
```
Input Layer (13 features)
    ↓
Dense Layer (128 neurons, ReLU)
    ↓
Dense Layer (64 neurons, ReLU)
    ↓
Dense Layer (32 neurons, ReLU)
    ↓
Output Layer (1 neuron, Linear)
```

**Total Parameters**: 12,161 (weights + biases across all layers)

## Algorithm
```
1. Load Boston Housing dataset from CSV
2. Explore and inspect the data
3. Split features (X) and target (y)
4. Split into training (80%) and testing (20%)
5. Normalize features using StandardScaler
6. Build DNN model:
   - Input: 13 features
   - Hidden Layers: 128 → 64 → 32 neurons with ReLU activation
   - Output: 1 neuron with Linear activation (price prediction)
7. Compile with Adam optimizer and MSE loss
8. Train for 100 epochs with 20% validation split
9. Evaluate on test data (MSE, MAE, RMSE, R² Score)
10. Plot training/validation loss and MAE curves
11. Plot Actual vs Predicted scatter plot
12. Display sample predictions
```

## Prerequisites
```
pip install tensorflow numpy pandas matplotlib scikit-learn
```

## Code

```python
# ============================================================
# Experiment 08: Boston Housing Price Prediction using DNN
# ============================================================

# ---- Step 1: Import Libraries ----
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Input

print("TensorFlow Version:", tf.__version__)

# ---- Step 2: Load Dataset ----
df = pd.read_csv("boston_housing.csv")
print(f"Dataset Shape: {df.shape}")
print(f"\nFeatures: {list(df.columns[:-1])}")
print(f"Target: {df.columns[-1]}")
df.head()

# ---- Step 3: Explore Dataset ----
print("Dataset Statistics:")
print(df.describe())
print(f"\nMissing values: {df.isnull().sum().sum()}")

# ---- Step 4: Split Features and Target ----
X = df.drop("MEDV", axis=1)   # 13 input features
y = df["MEDV"]                 # Target: Median house price

print(f"Features shape: {X.shape}")
print(f"Target shape:   {y.shape}")

# ---- Step 5: Train-Test Split ----
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

print(f"Training samples: {X_train.shape[0]}")
print(f"Testing samples:  {X_test.shape[0]}")

# ---- Step 6: Feature Scaling ----
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

print("Feature scaling completed!")

# ---- Step 7: Build the DNN Model ----
model = Sequential([
    Input(shape=(X_train.shape[1],)),
    Dense(128, activation='relu'),     # Hidden Layer 1
    Dense(64, activation='relu'),      # Hidden Layer 2
    Dense(32, activation='relu'),      # Hidden Layer 3
    Dense(1, activation='linear')      # Output Layer (regression)
])

model.summary()

# ---- Step 8: Compile the Model ----
model.compile(
    optimizer='adam',
    loss='mse',
    metrics=['mae']
)

# ---- Step 9: Train the Model ----
history = model.fit(
    X_train, y_train,
    epochs=100,
    batch_size=32,
    validation_split=0.2,
    verbose=1
)

# ---- Step 10: Evaluate the Model ----
print("\n--- Model Evaluation ---")
test_loss, test_mae = model.evaluate(X_test, y_test, verbose=0)
print(f"Test Loss (MSE): {test_loss:.4f}")
print(f"Test MAE:        {test_mae:.4f}")

y_pred = model.predict(X_test, verbose=0)
rmse = np.sqrt(mean_squared_error(y_test, y_pred))
r2 = r2_score(y_test, y_pred)
print(f"Test RMSE:       {rmse:.4f}")
print(f"R² Score:        {r2:.4f}")

# ---- Step 11: Plot Training History ----
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

axes[0].plot(history.history['loss'], label='Training Loss')
axes[0].plot(history.history['val_loss'], label='Validation Loss')
axes[0].set_title('Model Loss (MSE)')
axes[0].set_xlabel('Epoch')
axes[0].set_ylabel('Loss (MSE)')
axes[0].legend()
axes[0].grid(True, alpha=0.3)

axes[1].plot(history.history['mae'], label='Training MAE')
axes[1].plot(history.history['val_mae'], label='Validation MAE')
axes[1].set_title('Model MAE')
axes[1].set_xlabel('Epoch')
axes[1].set_ylabel('MAE')
axes[1].legend()
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.show()

# ---- Step 12: Actual vs Predicted Plot ----
plt.figure(figsize=(8, 6))
plt.scatter(y_test, y_pred, alpha=0.7, color='blue', edgecolors='black', linewidth=0.5)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()],
         'r--', linewidth=2, label='Perfect Prediction')
plt.xlabel('Actual Price ($1000s)')
plt.ylabel('Predicted Price ($1000s)')
plt.title('Actual vs Predicted House Prices')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

# ---- Step 13: Sample Predictions ----
print("\n--- Sample Predictions ---")
print(f"{'Actual':>10}  {'Predicted':>10}  {'Error':>10}")
print("-" * 35)
for i in range(10):
    actual = y_test.iloc[i]
    predicted = y_pred[i][0]
    error = abs(actual - predicted)
    print(f"{actual:10.2f}  {predicted:10.2f}  {error:10.2f}")
```

## How to Run

### In Jupyter Notebook:
1. Open Jupyter Notebook
2. Open `Boston_House_Price_Prediction.ipynb`
3. Run cells sequentially (Shift + Enter)

### In Google Colab:
1. Go to https://colab.research.google.com
2. Upload the notebook and `boston_housing.csv`
3. Run all cells sequentially

### As Python Script:
```bash
python boston_housing.py
```

## Expected Output
```
TensorFlow Version: 2.x.x

Dataset Shape: (506, 14)
Training samples: 404
Testing samples:  102

Model Summary:
_________________________________________________________________
Layer (type)                Output Shape              Param #
=================================================================
dense (Dense)               (None, 128)               1792
dense_1 (Dense)             (None, 64)                8256
dense_2 (Dense)             (None, 32)                2080
dense_3 (Dense)             (None, 1)                 33
=================================================================
Total params: 12,161

--- Model Evaluation ---
Test Loss (MSE): ~10-15
Test MAE:        ~2.0-3.0
Test RMSE:       ~3.0-4.0
R² Score:        ~0.80-0.90
```

## Advantages
1. Can capture non-linear relationships between features and price
2. Multiple hidden layers learn complex, hierarchical patterns
3. Adam optimizer adapts learning rate automatically for faster convergence
4. StandardScaler normalization improves training stability

## Applications
1. Real estate price prediction
2. Stock market forecasting
3. Energy consumption prediction
4. Sales forecasting
5. Weather prediction

## Viva Questions and Answers

**Q1: What is Linear Regression?**
A: A method to predict continuous values by learning a linear relationship y = wx + b.

**Q2: Why use a DNN for regression?**
A: DNNs can capture non-linear relationships that simple linear regression cannot. The hidden layers with ReLU activation learn complex feature interactions.

**Q3: What is ReLU activation?**
A: Rectified Linear Unit: f(x) = max(0, x). It's computationally efficient and helps avoid vanishing gradients.

**Q4: Why normalize the data?**
A: Features have different scales (e.g., CRIM ranges 0-89, TAX ranges 187-711). Normalization ensures all features contribute equally and helps faster convergence.

**Q5: What is MSE loss?**
A: Mean Squared Error = average of squared differences between actual and predicted values. It penalizes large errors more heavily.

**Q6: What is Adam optimizer?**
A: Adaptive Moment Estimation. It combines momentum and RMSProp for adaptive learning rates per parameter, leading to faster convergence.

**Q7: What is R² Score?**
A: Coefficient of determination. R²=1 means perfect prediction. R²=0 means same as predicting the mean. Higher values indicate better model fit.

**Q8: Why use validation split?**
A: To monitor overfitting during training. If validation loss increases while training loss decreases, the model is overfitting.

**Q9: What is overfitting?**
A: When the model memorizes training data but fails on new data. It has high training accuracy but low test accuracy.

**Q10: How many parameters does the model have?**
A: 12,161 parameters: Dense(128)=13×128+128=1,792; Dense(64)=128×64+64=8,256; Dense(32)=64×32+32=2,080; Dense(1)=32×1+1=33.

## Conclusion
We successfully implemented Boston Housing Price Prediction using a Deep Neural Network with TensorFlow/Keras. The model uses 3 hidden layers (128→64→32 neurons with ReLU activation) and achieves good accuracy with an R² score typically above 0.80, demonstrating that DNNs can effectively solve regression problems by capturing non-linear patterns in the data.
