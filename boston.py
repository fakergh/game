import numpy as np
import matplotlib.pyplot as plt 
import pandas as pd  
import seaborn as sns 
import warnings
warnings.filterwarnings("ignore")
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
from sklearn.datasets import load_boston
boston = load_boston()
df = pd.DataFrame(boston.data, columns=boston.feature_names)
# print(df.DESCR)
print(df.head())

df['MEDV'] = boston.target
print(df.head())

from sklearn.preprocessing import StandardScaler

X = df.drop('MEDV', axis=1)

y = df['MEDV']

from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

print('Training set shape:', X_train.shape, y_train.shape)

print('Testing set shape:', X_test.shape, y_test.shape)
from keras.models import Sequential

from keras.layers import Dense, Dropout

model = Sequential()

model.add(Dense(64, input_dim=13, activation='relu'))

model.add(Dropout(0.2))

model.add(Dense(32, activation='relu'))

model.add(Dense(1))

print(model.summary())

model.compile(loss='mean_squared_error', optimizer='adam', metrics=['mean_absolute_error'])

from keras.callbacks import EarlyStopping

# Train the model

early_stopping = EarlyStopping(monitor='val_loss', patience=5)

history = model.fit(X_train, y_train, validation_split=0.2, epochs=100, batch_size=32, callbacks=[early_stopping])

# Plot the training and validation loss over epochs

import matplotlib.pyplot as plt

plt.plot(history.history['loss'])

plt.plot(history.history['val_loss'])

plt.title('Model Loss')

plt.xlabel('Epochs')

plt.ylabel('Loss')

plt.legend(['Training', 'Validation'])

plt.show()

loss, mae = model.evaluate(X_test, y_test)


print('Mean Absolute Error:', mae)

lin_model = LinearRegression()
lin_model.fit(X_train, y_train)

y_train_predict = lin_model.predict(X_train)
rmse = (np.sqrt(mean_squared_error(y_train, y_train_predict)))
print('Root Mean Squared Error is {',rmse,'}')
r2 = r2_score(y_train, y_train_predict)
print('R2 score is {}'.format(r2))