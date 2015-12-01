class SomeClass:
    def __init__(self, name):
        self.name = name
    
    def hello(self, title):
        print("My name is", title, self.name)
        return title + " " + self.name


def somefunc(input):
    return input*2


