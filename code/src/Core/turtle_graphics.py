import turtle

angle = 35
forward_length = 5

print("Python is executed")

class Outputter:
    def __init__(self):
        self.stack = []
        
        self.window = turtle.Screen()
        #self.window.title("Turtle Graphics Module: Generated by NextGen")
        self.window.title("Turtle Graphics")
        self.window.bgcolor('black');
        self.babbel057 = turtle.Turtle()
        self.babbel057.penup()
        #self.babbel057.goto(-500,300)
        self.babbel057.shape('turtle')
        self.babbel057.speed(0)
        self.babbel057.pensize(2)
        self.babbel057.color('cyan')
        self.babbel057.pendown()

    def output(self, s):
        print(s)
        if s == '(':
            self.stack.append((self.babbel057.xcor(), self.babbel057.ycor(), self.babbel057.heading()))
        elif s == ')':
            self.babbel057.penup()
            t = self.stack.pop()
            self.babbel057.setpos(t[0], t[1])
            self.babbel057.setheading(t[2])
            self.babbel057.pendown()
        if s == '+':
            self.babbel057.right(angle)
        elif s== '-':
            self.babbel057.left(angle)
        else:
            self.babbel057.forward(forward_length)
            
    def close(self):
        self.window.mainloop()
        

#f = lambda r: r.replace("F", "F+F-F-F+F")

#o = Outputter()
#for symb in f(f(f(f(f("F"))))):
#	o.output(symb)

#o.close()

