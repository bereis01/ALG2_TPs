# Building.
import numpy as np
import random

# Testing.
import time
import pandas as pd
from sklearn.metrics import silhouette_score
from sklearn.metrics import adjusted_rand_score

# Returns the Minkowski distance between points x and y.
# x = (x1, ..., xn), y = (y1, ..., yn) and p is the metric parameter.
# x and y should be numpy arrays.
def minkowskiDistance(p, x, y):
    return np.power(np.sum(np.power(np.abs(x - y), p)), (1/p))

# 2-approximative algorithm por the k-centers problem.
# S is a numpy array of points in the R^n space also as numpy arrays.
# p is the metric parameter for the Minkowski distance function.
# k is the amount of centers to be determined by the algorithm.
def kCenters(S, p, k):
    # If S has at most k elements, then the k-centers
    # ...are the points of S with maximum radius of 0.
    if len(S) <= k:
        return S, 0.0, np.arange(len(S))
    
    # Auxiliary data structures.
    distS = np.array([np.inf] * len(S))
    index = list(range(len(S)))
    labels = np.zeros(len(S))
    maxDistIndex = random.choice(range(len(S)))

    # The main algorithm.
    centers = np.array([[0.0] * np.shape(S)[1]] * k)
    centersIndex = 0
    radius = 0.0
    while True:
        # Includes a new center in the solution.
        # The new center is the point with the current
        # ...maximum distance from its center.
        centers[centersIndex] = S[maxDistIndex]
        labels[maxDistIndex] = centersIndex
        distS[maxDistIndex] = 0.0
        index.remove(maxDistIndex)

        # Recalculates the minimum distance between
        # ...the remnant points and the new centers.
        for i in index:
            distance = minkowskiDistance(p, centers[centersIndex], S[i])
            if distance < distS[i]:
                distS[i] = distance
                labels[i] = centersIndex
        
        # Recalculates the maximum distance between
        # ...a point and its center.
        maxDistValue = 0
        for i in index:
            if distS[i] > maxDistValue:
                maxDistIndex = i
                maxDistValue = distS[i]

        # If k centers are found, finishes.
        centersIndex += 1
        if centersIndex >= k:
            radius = maxDistValue
            break

    # Returns the array of centers of size k 
    # ...the maximum radius and the points' labels.
    return centers, radius, labels

# For each dataset, 30 executions of the k-centers algorithm are done,
# ...for each one being computed the silhouette, rand scores, radius, inertia and the execution time.
# Each dataset is a tuple with the name and the data separator.
datasets = [('abalone.data', ','), ('drybean.arff', ','), ('electricalgrid.csv', ','), ('iranianchurn.csv', ','), ('optdigits.data', ','),
            ('pendigits.data', ','), ('redwine.csv', ';'), ('segmentation.data', ','), ('whitewine.csv', ';'), ('yeast.data', '\s+')]
fullResults = pd.DataFrame(columns = ['Mean Radius', 'SD Radius', 'Mean Inertia', 'SD Inertia', 'Mean Silhouette Score', 'SD Silhouette Score',
                                                        'Mean Rand Score', 'SD Rand Score', 'Mean Exec. Time', 'SD Exec. Time'])
for dataset in datasets:
    # Creates the dataframe to store the results.
    results = pd.DataFrame(columns = ['Radius', 'Inertia', 'Silhouette Score', 'Rand Score', 'Exec. Time'])

    # Inputs the data and remove rows with missing values and columns of nonnumeric values.
    S = pd.read_csv('data/' + dataset[0], sep = dataset[1]).dropna()
    points = S.select_dtypes([np.number]).to_numpy()
    realLabels = S['Class'].to_numpy()

    # Parameters.
    p = 2
    numOfCenters = S['Class'].nunique()

    # Tests.
    for i in range(30):
        # Computes the k-centers, the silhouette score and the adjusted rand score.
        # Also, measures the execution time.
        startTime = time.time()
        centers, radius, labels = kCenters(points, p, numOfCenters)
        endTime = time.time()
        silhouetteScore = silhouette_score(points, labels)
        randScore = adjusted_rand_score(realLabels, labels)
        elapsedTime = endTime - startTime

        # Manually gets the inertia.
        inertia = 0.0
        for i in range(len(points)):
            inertia += np.sum(np.power(np.abs(points[i] - centers[int(labels[i])]), 2))

        # Stores the results.
        results.loc[len(results)] = [radius, inertia, silhouetteScore, randScore, elapsedTime]

    # Aggregates the results into the full review.
    mean = results.mean()
    sd = results.std()
    fullResults.loc[dataset[0]] = [mean['Radius'], sd['Radius'], mean['Inertia'], sd['Inertia'], mean['Silhouette Score'], sd['Silhouette Score'], 
                                mean['Rand Score'], sd['Rand Score'], mean['Exec. Time'], sd['Exec. Time']]

# Exports the full results to an external file.
fullResults.to_excel('kCentersResults.xlsx')