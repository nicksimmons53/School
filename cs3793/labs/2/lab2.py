import pandas as pd

# Dataframe containing the coordinates from data.csv
coordinatesDF = pd.read_csv('data.csv', header=None, names=['loc', 'x', 'y'])

for loc in coordinatesDF:
	print(loc)
