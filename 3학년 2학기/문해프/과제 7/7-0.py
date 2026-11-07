import torch
import numpy as np
from collections import Counter

# my_n_neighbors represents the number of nearest neighbors to consider.
# You can adjust this value as needed, e.g., setting n_neighbors=32 will consider the 32 closest neighbors for prediction. 
my_n_neighbors = 32


class KNNModel:
    def __init__(self, n_neighbors):
        self.n_neighbors = n_neighbors
        self.X_train = None
        self.y_train = None
    
    # 예를 들어 28*28 크기의 사진이 60000장 있다면, images에는 60000 * 28 * 28 shape으로 담김. labels는 60000크기의 벡터. labels 각 값은 0~9 사이 정답 숫자.
    def load_data(self, file_name):
        images, labels = torch.load(file_name)
        images = [img.view(-1).numpy() for img in images]  # 2차원(가로x세로) 이미지를 1차원 벡터로 펴고, NumPy 배열로 바꿈 -> 60000 * 784 shape으로 바뀜
        return images, labels

    # KNN은 데이터저장/라벨저장 자체가 학습. 60000개 전체 데이터 중 50000개의 데이터를 담기.
    def fit(self, X_train, y_train):
        self.X_train = np.array(X_train)
        self.y_train = np.array(y_train)

    # TODO: modify only this function to implement KNN prediction. 
    # The returned predictions list will be used for scoring
    def predict(self, X_test): # 60000개 전체 데이터 중 10000개로 테스트.
        X_test = np.array(X_test) # 테스트 이미지 10000장을 10000 * 784 크기의 넘파이로 변환. 기본 파이썬 행렬보다 넘파이가 빠름.
        predictions = [] # example output. 테스트 이미지 레이블들을 담을 리스트
        
        for x in X_test:
            
            # distances. x_train 5만개 데이터와 거리 구하기

            # distances = []

            # for train_data in self.X_train :
            #     dist = np.linalg.norm(train_data - x)
            #     distances.append(dist)

            # 넘파이 자동 크기 맞추기. X_train은 50000 * 784. x는 1 * 784. -> 자동으로 x를 50000 * 784로 복사해서 연산해줌.
            distances = np.linalg.norm(self.X_train - x, axis=1)


            # n_neighbors. x에서 가까운 이웃 n_neighbors개 구하기
            k_indices = np.argsort(distances)[: self.n_neighbors]


            # predictions. 가까운 이웃들을 기반으로 카테고리 예측
            k_nearest_labels = [self.y_train[i] for i in k_indices]

            most_common = Counter(k_nearest_labels).most_common(1)
            predicted_label = most_common[0][0]

            predictions.append(predicted_label)


        return predictions

    def __call__(self,):
        pass
        
# Instructions: Complete only the class implementation. You may add print statements to test when using the "Run" button, 
# but only the class will be used for grading upon submission.