a = 1
b = a
print("a: " + str(a))
print("b: " + str(b))

print ("Assigning 'hello' to a")
a = "hello"
print("a: " + a) # a is a string now
print("b: " + str(b)) # b still points to a's old value
