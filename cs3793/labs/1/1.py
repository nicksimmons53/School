 import random

userGreetings = ['Hello', 'hello', 'Hi', 'hi', 'Hey', 'hey', 'Howdy', 'howdy']
userQuestion = ['What\'s up', 'What\'s up?']
userRandom = ['Cool!', 'Sounds fun', 'Exciting']
userGoodbye = ['bye', 'Bye', 'goodbye', 'See ya']

botGreeting = ['Hi there', 'Hello', 'Hey!', 'Howdy']
botAnswer = ['Just talking with you!', 'Not much, how about you?', 'Same old, same old']
botRandom = ['Not really', 'It\'s ok', 'Could be better!']
botGoodbye = ['See ya next time', 'Hasta luego']

def welcome( ):
    print("*******************************")
    print("  Welcome to Rowdy Messenger   ")
    print("*******************************")
    print("<<<type “bye” to exit>>>")
    print( )

def conversation( ):
    while(True):
        userInput = input("You: ")

        if userInput in userGreetings:
            print("Bot: " + random.choice(botGreeting))

        if userInput in userQuestion:
            print("Bot: " + random.choice(botAnswer))

        if userInput in userRandom:
            print("Bot: " + random.choice(botRandom))

        if (userInput in userGoodbye):
            print("Bot: " + random.choice(botGoodbye))
            break

    print( )

welcome( )
conversation( )
