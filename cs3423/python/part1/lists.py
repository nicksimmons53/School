numbers = ["one", "two", "three", "four", "five"]
colors = ["red", "orange", "YELLOW", "beige", "grey", "violet"]

# access
print("number[0] is " + numbers[0])
print("The last index in numbers is " + str(len(numbers)-1))
print("The last number is " + numbers[len(numbers)-1])

# slicing
print("Slicing odd and even numbers:")
odd_numbers = numbers[::2]
even_numbers = numbers[1::2]
print(odd_numbers)
print(even_numbers)

# assignment
print("Fixing colors with assignments:")
colors[2] = "yellow"
colors[3:5] = ["green", "blue", "indigo"]
print(colors)

# strings as lists
print("Substring with list notation:")
greeting = "hello world"
print(greeting[6:])
