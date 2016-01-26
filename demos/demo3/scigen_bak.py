import subprocess
print("Python is executed")



class Outputter:
    latex_tags = set(["documentclass", "usepackage", "article", "twocolumn", "cite", "helvet", "document", "begin", "end", "author", "date", "maketitle", "section", "latin", "subsection", "log", "pi", "bibliography", "footnotesize", "acm"])
    
    def __init__(self):
        self.f = open('output.tex', 'w')
        self.word = ""
        self.reset()
    
    def reset(self):
        self.start_of_sentence = False
        self.space = False
    
    def flush(self, s):
        if self.space:
            self.f.write(" ")
            self.space = False
        if self.word not in self.latex_tags and self.start_of_sentence:
            self.word = self.word.capitalize()
            self.start_of_sentence = False
        self.f.write(self.word + s)
        self.word = ""
    
    def output(self, s):
        if s.isalpha():
            if self.space:
                self.f.write(self.word + " ")
                self.word = ""
                self.space = False
            self.word += s
        elif self.word in self.latex_tags:
            self.f.write(self.word + s)
            self.word = ""
        else:
            if s in "\n.":
                self.start_of_sentence = True
                self.flush(s)
            elif s == " ":
                self.space = True
            else:
                self.flush(s)
            
  
    def bash_command(self, cmd):
        subprocess.call(cmd, shell=True)
    
    def close(self):
        self.f.close()
        #Dself.bash_command('pdflatex output.tex')
        #self.bash_command('xdg-open output.pdf')
        
