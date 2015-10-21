
import random

class CFG:
	def __init__(self, V, T, P, S):
		self.V = V
		self.T = T
		self.P = P
		self.S = S

class sCFG(CFG):
	def __init__(self, V, T, P, S):
		CFG.__init__(self, V, T, P, S)
		self.table = self.create_table(1.0)
	
	def create_table(self, default):
		table = {}
		for rule in self.all_rules():
			table[rule] = {}
			for rule2 in self.all_rules():
				table[rule][rule2] = default
		return table
	
	
	def all_rules(self):
		for head in self.P:
			for body in self.P[head]:
				yield (head, body)
	
	def get_rules(self, S):
		for body in self.P[S]:
			yield (S, body)
	
	def create_pt(self, symbol):
		if symbol in self.T:
			return symbol
		# pick a rule
		body = random.choice(list(self.P[symbol]))
		return [(symbol,body)] + list([self.create_pt(s) for s in body])
	
	def fancy_pt(self, symbol, ancestors):
		if symbol in self.T:
			return [symbol]
		# create specific table for this stuff
		chances = []
		total = 0.0
		for rule in self.get_rules(symbol):
			chance = 1.0
			for anc in ancestors:
				chance *= self.table[anc][rule]
			chances.append((rule, chance))
			total += chance
		
		rand = random.random()*total
		cursor = 0.0
		the_rule = None
		for (rule, chance) in chances:
			cursor += chance
			if cursor >= rand:
				the_rule = rule
				break
		
		return [the_rule] + list([self.fancy_pt(s, ancestors+[the_rule]) for s in the_rule[1]])
	
	def score_pt(self, pt, score, run_info=None):
		if run_info is None:
			run_info = self.create_table(False)
			
		# for each rule in pt, for each rule2 in pt, table
		root = pt[0]
		for subrule in self.pt_rules_start(pt):
			if isinstance(subrule, tuple):
				self.mult_score(root, subrule, score, run_info)
		
		for direct_child in pt[1:]:
			self.score_pt(direct_child, score)
	
	def mult_score(self, root, subrule, score, run_info):
		if not run_info[root][subrule]:
			run_info[root][subrule] = True
			self.table[root][subrule] *= score
	
	def pt_rules_start(self, pt):
		for sub_pt in pt[1:]:
			yield from self.pt_rules(sub_pt)
	
	
	def pt_rules(self, pt):
		yield pt[0]
		for sub_pt in pt[1:]:
			yield from self.pt_rules(sub_pt)
	
	def print_pt(self, pt, indent=0):
		if pt[0][0] in self.V:
			print(indent*"  " + repr(pt[0]))
			for e in pt[1:]:
				self.print_pt(e, indent+1)
	
	def print_table(self):
		for rule in self.all_rules():
			print(rule)
			for rule2 in self.all_rules():
				print("  " + repr(rule2) + "\t = " + str(self.table[rule][rule2]))

P = {
	"start": {"B"},
	"B": {"PP", "PPP", "PPPP"},
	"P": {"ZZZZZ", "ZZZZZZ", "ZZZZZZZ"},
	"Z": {"WWWWWW","WWWWWWWW"}
}

bagger = sCFG({"B", "P", "Z", "start"}, {"W"}, P, "start")
bagger.print_table()
print("")
i = "niet q"
while i != "q":
	pt = bagger.fancy_pt(bagger.S, [])
	bagger.print_pt(pt)
	i = input("? ")
	try:
		score = float(i)
		bagger.score_pt(pt, score)
	except ValueError:
		if i == "t":
			bagger.print_table()
	

