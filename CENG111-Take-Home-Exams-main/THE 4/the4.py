def construct_forest(x):
	alp=["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","y","z"]
	op=["*","^","-","+","/"]
	leaf=[]
	result=[]
	twos=[]
	p=len(x)
	for t in range(p):
		for i in x[:]:
			n=[]
			b=""
			for l in i:
					n.append(l)
			for k in range(len(n)):
				if " " in n:
					n.pop(n.index(" "))
				else:
					break
			for k in n:
				b=b+k
			c=0	
			for j in alp:
				if j in b[b.index("="):]:
					c+=1
					if c==1:
						index1=b.index(j)
					if c==2:
						index2=b.index(j)	
			d=0		
			for k in x[:]:
				if b[0] in k:
					d+=1
			for k in op:
					if k in b:
						o=k		
			if d==1 and c==0:
				result.append([b[0],b[b.index(o)],[b[b.index("=")+1:b.index(o)]],[b[b.index(o)+1:]]])
				x.remove(i)
				continue
			if d==2 and c==0:
				leaf.append([b[0],b[b.index(o)],[b[b.index("=")+1:b.index(o)]],[b[b.index(o)+1:]]])
				x.remove(i)
				continue
			if d==2 and c==1:
				for u in leaf[:]:
					if u[0] in b:
						if index1<b.index(o):
							leaf.append([b[0],b[b.index(o)],u,[b[b.index(o)+1:]]])
							leaf.remove(u)
							x.remove(i)
							continue
						elif index1>b.index(o):	
							leaf.append([b[0],b[b.index(o)],[b[b.index("=")+1:b.index(o)]],u])
							leaf.remove(u)
							x.remove(i)
							continue
			if d==2 and c==2:
				for u in leaf[:]:
					if u[0] in b:
						if b.index(u[0])<b.index(o):
							a=0
							for v in twos[:]:
								if b[0]==v[0]:
									a+=1
									v[2].extend(u)
									leaf.append(v)
									leaf.remove(u)
									twos.remove(v)
									x.remove(i)
									break
							if a==0:				
								twos.append([b[0],b[b.index(o)],u,[]])
								leaf.remove(u)
						elif b.index(u[0])>b.index(o):
							a=0
							for v in twos[:]:
								if b[0]==v[0]:
									a+=1
									v[3].extend(u)
									leaf.append(v)
									twos.remove(v)
									leaf.remove(u)
									x.remove(i)
									break
							if a==0:		
								twos.append([b[0],b[b.index(o)],[],u])
								leaf.remove(u)
			if d==1 and c==2:			
				for u in leaf[:]:
					if u[0] in b:
						if b.index(u[0])<b.index(o):
							a=0
							for v in twos[:]:
								if b[0]==v[0]:
									a+=1
									v[2].extend(u)
									result.append(v)
									twos.remove(v)
									leaf.remove(u)
									x.remove(i)
									break
							if a==0:				
								twos.append([b[0],b[b.index(o)],u,[]])
								leaf.remove(u)
						elif b.index(u[0])>b.index(o):
							a=0
							for v in twos[:]:
								if b[0]==v[0]:
									a+=1
									v[3].extend(u)
									result.append(v)
									twos.remove(v)
									leaf.remove(u)
									x.remove(i)
									break
							if a==0:		
								twos.append([b[0],b[b.index(o)],[],u])
								leaf.remove(u)						
			if d==1 and c==1:
				for u in leaf[:]:
					if u[0] in b:
						if index1<b.index(o):
							result.append([b[0],b[b.index(o)],u,[b[b.index(o)+1:]]])
							leaf.remove(u)
							x.remove(i)
						elif index1>b.index(o):	
							result.append([b[0],b[b.index(o)],[b[b.index("=")+1:b.index(o)]],u])
							leaf.remove(u)
							x.remove(i)	

	return result					