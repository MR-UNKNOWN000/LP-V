# Experiment 12: Human Face Recognition using CNN

## Aim
Design and implement a Human Face Recognition system using CNN with OpenCV and TensorFlow/Keras.

## Objective
- Understand face detection vs face recognition concepts
- Learn Haar Cascade classifier for robust face detection
- Implement CNN for feature extraction and person classification
- Understand data augmentation for improved generalization
- Build end-to-end pipeline: detection → preprocessing → feature extraction → classification
- Implement optional real-time webcam-based recognition
- Learn practical considerations: dataset collection, preprocessing, model training, evaluation

## Theory

### What is Face Recognition?
Face recognition involves:
1. **Face Detection**: Locating faces in an image (Haar Cascade)
2. **Face Preprocessing**: Cropping, resizing, normalizing
3. **Feature Extraction**: CNN learns facial features
4. **Classification**: Identifying whose face it is

### System Architecture
```
Input Image
    ↓
Face Detection (Haar Cascade)
    ↓
Face Cropping & Resizing (100×100)
    ↓
Normalization (0-1)
    ↓
CNN Feature Extraction
    ↓
Classification (Softmax)
    ↓
Person Identity + Confidence
```

### CNN Architecture
```
Input (100×100×3)
    ↓
Conv2D(32) → MaxPool
    ↓
Conv2D(64) → MaxPool
    ↓
Conv2D(128) → MaxPool
    ↓
Flatten → Dropout(0.5)
    ↓
Dense(256) → Dropout(0.3)
    ↓
Dense(NUM_CLASSES, Softmax)
```

### Haar Cascade for Face Detection
OpenCV's Haar Cascade classifier detects faces using:
- Pre-trained XML file
- Sliding window approach
- Edge and texture features

## Dataset Structure
```
dataset/
├── Person_1/          (e.g., "John")
│   ├── img001.jpg
│   ├── img002.jpg
│   ├── img003.jpg
│   └── ... (20-30 images minimum)
├── Person_2/          (e.g., "Jane")
│   ├── img001.jpg
│   └── ...
├── Person_3/
│   └── ...
└── Person_N/
    └── ...
```

### Recommended Datasets:
1. **LFW (Labeled Faces in Wild)**: http://vis-www.cs.umass.edu/lfw/
2. **AT&T/ORL Database**: https://www.kaggle.com/datasets/kasikrit/att-database-of-faces
3. **Custom dataset**: Take 20-30 photos of each person

### How to Create Your Own Dataset:
1. Create folders named after each person
2. Take photos from different angles and lighting
3. Include at least 20 images per person
4. Include variations: glasses, expressions, etc.

## Algorithm
```
1. Load images from person-named directories
2. Detect faces using Haar Cascade
3. Crop and resize faces to 100×100
4. Normalize pixel values to [0, 1]
5. Encode labels (person names → numbers)
6. Split into train (80%) and test (20%)
7. Build CNN model
8. Train with data augmentation
9. Evaluate accuracy
10. Test with webcam (optional)
```

## Code Components

### Face Detection:
```python
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
)
faces = face_cascade.detectMultiScale(gray, 1.3, 5)
```

### Data Augmentation:
```python
datagen = ImageDataGenerator(
    rotation_range=15,      # Slight rotation
    horizontal_flip=True,   # Mirror image
    zoom_range=0.1          # Slight zoom
)
```

### Webcam Integration:
```python
cap = cv2.VideoCapture(0)
while True:
    ret, frame = cap.read()
    # Detect face → Preprocess → Predict → Display
```

## Prerequisites
```bash
pip install tensorflow opencv-python numpy matplotlib scikit-learn
```

## How to Run

### Step 1: Prepare Dataset
Organize face images in the folder structure above

### Step 2: Update Path
Change `DATASET_DIR` in the code to your dataset path

### Step 3: Run
```bash
python face_recognition.py
```

### Step 4: Webcam Testing
Uncomment the webcam section at the bottom of the code

## Expected Output
```
TensorFlow Version: 2.x.x
OpenCV Version: 4.x.x

Total images loaded: 250
Number of classes (persons): 5
Classes: ['Person_1', 'Person_2', 'Person_3', 'Person_4', 'Person_5']

Training samples: 200
Testing samples: 50

Epoch 1/25 - accuracy: 0.3200
Epoch 25/25 - accuracy: 0.9450

--- Model Evaluation ---
Test Loss: 0.2341
Test Accuracy: 0.9200 (92.00%)

--- Sample Predictions ---
  Actual: Person_1        Predicted: Person_1        (95.3%)  ✓
  Actual: Person_3        Predicted: Person_3        (88.7%)  ✓
  Actual: Person_2        Predicted: Person_2        (91.2%)  ✓
```

## Advantages
1. Real-time face recognition capability
2. Simple CNN architecture (easy to train)
3. Data augmentation prevents overfitting
4. Haar Cascade provides robust face detection
5. Can be extended to more people easily

## Applications
1. Security and surveillance
2. Attendance systems
3. Access control
4. Social media (auto-tagging)
5. Law enforcement
6. Smart home systems

## Viva Questions and Answers

**Q1: What is face recognition?**
A: The ability to identify or verify a person from their face in an image or video using computer vision and deep learning.

**Q2: What is Haar Cascade?**
A: A pre-trained classifier in OpenCV that detects objects (faces) using Haar-like features (edges, lines, rectangles). Fast and efficient.

**Q3: Why use CNN for face recognition?**
A: CNNs automatically learn discriminative facial features (eyes, nose, face shape) from training data without manual feature engineering.

**Q4: What is face detection vs face recognition?**
A: Detection = finding WHERE faces are in an image. Recognition = identifying WHO the face belongs to.

**Q5: Why normalize pixel values?**
A: Scaling 0-255 to 0-1 helps the neural network converge faster and prevents numerical instability.

**Q6: What is data augmentation?**
A: Creating new training images by applying random transformations (rotation, flip, zoom). Increases dataset size and prevents overfitting.

**Q7: What is the Softmax function?**
A: Converts raw outputs to probabilities summing to 1. The class with highest probability is the prediction.

**Q8: Why use Dropout?**
A: Randomly disables neurons during training to prevent overfitting. Forces the network to learn robust features.

**Q9: How does webcam integration work?**
A: Capture frame → Detect face → Crop & resize → Normalize → Predict with model → Display label.

**Q10: How can accuracy be improved?**
A: More training data, deeper network, transfer learning (VGGFace, FaceNet), better data augmentation, higher resolution images.

## Conclusion
We successfully implemented a Human Face Recognition system using CNN with OpenCV and TensorFlow. The system detects faces using Haar Cascade, extracts features using CNN, and classifies them with high accuracy. The optional webcam integration enables real-time face recognition.
