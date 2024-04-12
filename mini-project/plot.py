import matplotlib.pyplot as plt

filename1 = 'points.txt'
x_values1 = []
y_values1 = []

# Read the data from the text file
with open(filename1, mode='r') as file:
    for line in file:
        x, y = map(float, line.split())
        x_values1.append(x)
        y_values1.append(y)

# Plot the points
plt.scatter(x_values1, y_values1, color='blue')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('2D Points')
plt.grid(True)
plt.show()

filename1 = 'points_mapped.txt'
x_values = []
y_values = []

# Read the data from the text file
with open(filename1, mode='r') as file:
    for line in file:
        x, y = map(float, line.split())
        x_values.append(x)
        y_values.append(y)

# Plot the points
plt.scatter(x_values, y_values, color='blue')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('2D Points mapped')
plt.grid(True)
plt.show()
