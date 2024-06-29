import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Define the quadratic function
def quadratic(x, a, b, c):
    return a * x**2 + b * x + c

# Read data from the file
file_name = 'grid_search_output.txt'
data = np.genfromtxt(file_name, delimiter=',', skip_header=1)

nearest_percent = data[:, 0]
min_distance = data[:, 2]

# Fit the quadratic function to the data
popt, _ = curve_fit(quadratic, nearest_percent, min_distance)

# Generate x values for plotting the fit
x_fit = np.linspace(min(nearest_percent), max(nearest_percent), 100)
y_fit = quadratic(x_fit, *popt)

# Find the minimum value of the fitted quadratic
x_min = -popt[1] / (2 * popt[0])
y_min = quadratic(x_min, *popt)

# Plot the data and the fit
plt.scatter(nearest_percent, min_distance, label='Data', color='blue')
plt.plot(x_fit, y_fit, label='Quadratic fit', color='red')
plt.scatter([x_min], [y_min], color='green', zorder=5)
plt.text(x_min, y_min, f'Min: ({x_min:.2f}, {y_min:.2f})', fontsize=12, verticalalignment='bottom')

plt.xlabel('Nearest_Percent')
plt.ylabel('Min_Distance')
plt.title('Quadratic Fit to Min_Distance vs Nearest_Percent')
plt.legend()
plt.grid(True)
plt.show()

# Print the minimum value
print(f"The minimum value is at Nearest_Percent = {x_min:.2f} with Min_Distance = {y_min:.2f}")
