def pattern_search(x,y):
	p90=[]
	p180=[]
	p270=[]
	for i in range(len(x[0])):
		word=""
		for j in range(1,len(x)+1):
			word=f"{word}{x[-j][i]}"
		p90.append(word)
	for i in range(len(p90[0])):
		word=""
		for j in range(1,len(p90)+1):
			word=f"{word}{p90[-j][i]}"
		p180.append(word)
	for i in range(len(p180[0])):
		word=""
		for j in range(1,len(p180)+1):
			word=f"{word}{p180[-j][i]}"
		p270.append(word)	
	for i in range(len(y)-len(x)+1):
		for j in range(len(y[0])-len(x[0])+1):
			a=0
			m=0
			for k in range(len(x)):
				if m!=a:
					break
				for n in range(len(x[0])):
					if m!=a:
						break
					m+=1	
					if x[k][n]==y[i+k][j+n]:
						a+=1
			if a==len(x)*len(x[0]):
				return (i, j, 0)
	for i in range(len(y)-len(x)+1):
		for j in range(len(y[0])-len(x[0])+1):
			a=0
			m=0
			for k in range(len(x)):
				if m!=a:
					break
				for n in range(len(x[0])):
					if m!=a:
						break
					m+=1
					if p180[k][n]==y[i+k][j+n]:
						a+=1	
			if a==len(x)*len(x[0]):
				return (i, j, 180)
	for i in range(len(y)-len(p90)+1):
		for j in range(len(y[0])-len(p90[0])+1):
			a=0
			m=0
			for k in range(len(p90)):
				if m!=a:
					break
				for n in range(len(p90[0])):
					if m!=a:
						break
					m+=1	
					if p90[k][n]==y[i+k][j+n]:
						a+=1
			if a==len(p90)*len(p90[0]):
				return (i, j, 90)
	for i in range(len(y)-len(p90)+1):
		for j in range(len(y[0])-len(p90[0])+1):
			a=0
			m=0
			for k in range(len(p90)):
				if m!=a:
					break
				for n in range(len(p90[0])):
					if m!=a:
						break
					m+=1
					if p270[k][n]==y[i+k][j+n]:
						a+=1
			if a==len(p90)*len(p90[0]):
				return (i, j, 270)
	return False			

	






	
					