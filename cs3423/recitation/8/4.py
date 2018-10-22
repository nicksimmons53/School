import sys
import os
def greet(name, spanish=False, caps=False):
    if spanish:
        greeting = "Hola," + name
    else:
        greeting = "Hello," + name

    if caps:
        print(greeting.upper( ))
    else:
        print(greeting)
greet("Kevin", spanish=True)
greet("Kevin", caps=True)
greet("Kevin", spanish=True, caps=True)
