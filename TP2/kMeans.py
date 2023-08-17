# This file contains samples and testing using the SciKit Learn toolkit.

# Testing.
import time
import numpy as np
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.metrics import adjusted_rand_score

# Returns the Minkowski distance between points x and y.
# x = (x1, ..., xn), y = (y1, ..., yn) and p is the metric parameter.
# x and y should be numpy arrays.
def minkowskiDistance(p, x, y):
    return np.power(np.sum(np.power(np.abs(x - y), p)), (1/p))

# For each dataset, 30 executions of the k-means algorithm are done,
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
        kmeans = KMeans(n_clusters = numOfCenters, n_init = 1).fit(points)
        endTime = time.time()
        centers, inertia, labels = kmeans.cluster_centers_, kmeans.inertia_, kmeans.labels_
        silhouetteScore = silhouette_score(points, labels)
        randScore = adjusted_rand_score(realLabels, labels)
        elapsedTime = endTime - startTime

        # Manually gets the maximum radius.
        radius = 0.0
        for i in range(len(points)):
            distance = minkowskiDistance(p, centers[labels[i]], points[i])
            if distance > radius:
                radius = distance

        # Stores the results.
        results.loc[len(results)] = [radius, inertia, silhouetteScore, randScore, elapsedTime]

    # Aggregates the results into the full review.
    mean = results.mean()
    sd = results.std()
    fullResults.loc[dataset[0]] = [mean['Radius'], sd['Radius'], mean['Inertia'], sd['Inertia'], mean['Silhouette Score'], sd['Silhouette Score'], 
                                mean['Rand Score'], sd['Rand Score'], mean['Exec. Time'], sd['Exec. Time']]

# Exports the full results to an external file.
fullResults.to_excel('kMeansResults.xlsx')