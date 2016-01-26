import subprocess
print("Python is executed")



class Outputter:
    def __init__(self):
        self.f = open('output.tex', 'w')
        self.start_of_sentence = False
        self.space = False
    
    def output(self, s):
        if self.start_of_sentence and s.isalpha():
            s = s.upper()
            self.start_of_sentence = False
        
        if s in "\\{}[]":
            self.start_of_sentence = False
            self.space = False
            self.f.write(s)
        elif s==" ":
            self.space = True
        else:
            if s in ".,()!?;:":
                self.space = False
                if s in ".":
                    self.start_of_sentence = True
                    self.space = True
            elif self.space:
                self.space = False
                s = " " + s
            self.f.write(s)
  
    def bash_command(self, cmd):
        subprocess.call(cmd, shell=True)
    
    def close(self):
        self.f.close()
        self.bash_command('pdflatex output.tex')
        self.bash_command('xdg-open output.pdf')
        
