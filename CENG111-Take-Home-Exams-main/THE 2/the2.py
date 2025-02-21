a=eval(input())
v_0=a[0]
v_1=a[1]
v_2=a[2]
v_3=a[3]
x0=v_0[0]
x1=v_1[0]
x2=v_2[0]
x3=v_3[0]
y0=abs(v_0[1])
y1=abs(v_1[1])
y2=abs(v_2[1])
y3=abs(v_3[1])	
listex=[x0,x1,x2,x3]
listey=[y0,y1,y2,y3]
b=max(a[0][0],a[1][0],a[2][0],a[3][0])
c=min(a[0][0],a[1][0],a[2][0],a[3][0])
if b==x0 and b==x2 and c==x1 and c!=x3:
	if y0<y2:
		sayi=((y0+y1)/2)*(x0-x1)
		print(f"{sayi:.2f}")
	elif y0>y2:	
		sayi=((y2+y1)/2)*(x0-x1)
		print(f"{sayi:.2f}")
elif b==x0 and b==x2 and c!=x1 and c==x3:
	if y0<y2:
		sayi=((y0+y3)/2)*(x0-x3)
		print(f"{sayi:.2f}")
	elif y0>y2:	
		sayi=((y2+y3)/2)*(x0-x3)
		print(f"{sayi:.2f}")
elif b==x1 and b==x3 and c!=x0 and c==x2:
	if y1<y3:
		sayi=((y2+y1)/2)*(x1-x2)
		print(f"{sayi:.2f}")
	elif y1>y3:	
		sayi=((y2+y3)/2)*(x1-x2)
		print(f"{sayi:.2f}")
elif b==x1 and b==x3 and c==x0 and c!=x2:
	if y1<y3:
		sayi=((y0+y1)/2)*(x1-x0)
		print(f"{sayi:.2f}")
	elif y1>y3:	
		sayi=((y0+y3)/2)*(x1-x0)
		print(f"{sayi:.2f}")		
elif c==x0 and c==x2 and b==x1 and b!=x3:
	if y0<y2:
		sayi=((y0+y1)/2)*(x1-x0)
		print(f"{sayi:.2f}")
	elif y0>y2:	
		sayi=((y2+y1)/2)*(x1-x0)
		print(f"{sayi:.2f}")
elif c==x0 and c==x2 and b!=x1 and b==x3:
	if y0<y2:
		sayi=((y0+y3)/2)*(x3-x0)
		print(f"{sayi:.2f}")
	elif y0>y2:	
		sayi=((y2+y3)/2)*(x3-x0)
		print(f"{sayi:.2f}")
elif c==x1 and c==x3 and b!=x0 and b==x2:
	if y1<y3:
		sayi=((y1+y2)/2)*(x2-x3)
		print(f"{sayi:.2f}")
	elif y1>y3:	
		sayi=((y2+y3)/2)*(x2-x3)
		print(f"{sayi:.2f}")
elif c==x1 and c==x3 and b==x0 and b!=x2:
	if y1<y3:
		sayi=((y1+y0)/2)*(x0-x3)
		print(f"{sayi:.2f}")
	elif y1>y3:	
		sayi=((y0+y3)/2)*(x0-x3)
		print(f"{sayi:.2f}")					
else:	
	if b == x0:
		if c == x1:
			if y0>y1:
				if y2>y1+((y0-y1)/(x0-x1))*(x2-x1) and y3>y1+((y0-y1)/(x0-x1))*(x3-x1):
					sayi=((y0+y1)/2)*(x0-x1)
					print(f"{sayi:.2f}")
				elif y2<y1+((y0-y1)/(x0-x1))*(x2-x1) and y3<y1+((y0-y1)/(x0-x1))*(x3-x1):
					sayi=((y0+y1)/2)*(x0-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y1>y0:
				if y2>y1-((y1-y0)/(x0-x1))*(x2-x1) and y3>y1-((y1-y0)/(x0-x1))*(x3-x1):
					sayi=((y0+y1)/2)*(x0-x1)
					print(f"{sayi:.2f}")
				elif y2<y1-((y1-y0)/(x0-x1))*(x2-x1) and y3<y1-((y1-y0)/(x0-x1))*(x3-x1):
					sayi=((y0+y1)/2)*(x0-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y1 == y0:
				if y2 > y0 and y3 > y0:
					sayi=((y0+y1)/2)*(x0-x1)
					print(f"{sayi:.2f}")
				elif y2 < y0 and y3 < y0:
					sayi=((y0+y1)/2)*(x0-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
		elif c == x3:
			if y0>y3:
				if y2>y3+((y0-y3)/(x0-x3))*(x2-x3) and y1>y3+((y0-y3)/(x0-x3))*(x1-x3):
					sayi=((y0+y3)/2)*(x0-x3)
					print(f"{sayi:.2f}")
				elif y2<y3+((y0-y3)/(x0-x3))*(x2-x3) and y1<y3+((y0-y3)/(x0-x3))*(x1-x3):
					sayi=((y0+y3)/2)*(x0-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y3>y0:
				if y2>y3-((y3-y0)/(x0-x3))*(x2-x3) and y1>y3-((y3-y0)/(x0-x3))*(x1-x3):
					sayi=((y0+y3)/2)*(x0-x3)
					print(f"{sayi:.2f}")
				elif y2<y3-((y3-y0)/(x0-x3))*(x2-x3) and y1<y3-((y3-y0)/(x0-x3))*(x1-x3):
					sayi=((y0+y3)/2)*(x0-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
			elif y3 == y0:
				if y2 > y0 and y1 > y0:
					sayi=((y0+y3)/2)*(x0-x3)
					print(f"{sayi:.2f}")
				elif y2 < y0 and y1 < y0:
					sayi=((y0+y3)/2)*(x0-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
		elif c == x2:
			if y0>y2:
				if y1>y2+((y0-y2)/(x0-x2))*(x1-x2) and y3<y2+((y0-y2)/(x0-x2))*(x3-x2):
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y2+((y0-y2)/(x0-x2))*(x1-x2) and y3>y2+((y0-y2)/(x0-x2))*(x3-x2):
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
					print(f"{sayi:.2f}")
				elif y1>y2+((y0-y2)/(x0-x2))*(x1-x2) and y3>y2+((y0-y2)/(x0-x2))*(x3-x2):
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")	
				elif y1<y2+((y0-y2)/(x0-x2))*(x1-x2) and y3<y2+((y0-y2)/(x0-x2))*(x3-x2):
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")	
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")		
			elif y0<y2:
				if y1>y2-((y2-y0)/(x0-x2))*(x1-x2) and y3<y2-((y2-y0)/(x0-x2))*(x3-x2):
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y2-((y2-y0)/(x0-x2))*(x1-x2) and y3>y2-((y2-y0)/(x0-x2))*(x3-x2):
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
					print(f"{sayi:.2f}")
				elif y1>y2-((y2-y0)/(x0-x2))*(x1-x2) and y3>y2-((y2-y0)/(x0-x2))*(x3-x2):
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")	
				elif y1<y2-((y2-y0)/(x0-x2))*(x1-x2) and y3<y2-((y2-y0)/(x0-x2))*(x3-x2):
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")			
			elif y0==y2:
				if y1>y2 and y3<y2:
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y2 and y3>y2:
					sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
					print(f"{sayi:.2f}")
				elif y1>y2 and y3>y2:
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
				elif y1<y2 and y3<y2:
					if y1<y3:
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:	
						sayi=((y0+y2)/2)*(x0-x2)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")																		
	elif b == x1:
		if c == x0:
			if y1>y0:
				if y2>y0+((y1-y0)/(x1-x0))*(x2-x0) and y3>y0+((y1-y0)/(x1-x0))*(x3-x0):
					sayi=((y1+y0)/2)*(x1-x0)
					print(f"{sayi:.2f}")
				elif y2<y0+((y1-y0)/(x1-x0))*(x2-x0) and y3<y0+((y1-y0)/(x1-x0))*(x3-x0):
					sayi=((y1+y0)/2)*(x1-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y0>y1:
				if y2>y0-((y1-y0)/(x1-x0))*(x2-x0) and y3>y0-((y0-y1)/(x1-x0))*(x3-x0):
					sayi=((y0+y1)/2)*(x1-x0)
					print(f"{sayi:.2f}")
				elif y2<y0-((y1-y0)/(x1-x0))*(x2-x0) and y3<y0-((y0-y1)/(x1-x0))*(x3-x0):
					sayi=((y0+y1)/2)*(x1-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y1 == y0:
				if y2 > y0 and y3 > y0:
					sayi=((y0+y1)/2)*(x1-x0)
					print(f"{sayi:.2f}")
				elif y2 < y0 and y3 < y0:
					sayi=((y0+y1)/2)*(x1-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")		
		elif c == x2:
			if y1>y2:
				if y3>y2+((y1-y2)/(x1-x2))*(x3-x2) and y0>y2+((y1-y2)/(x1-x2))*(x0-x2):
					sayi=((y1+y2)/2)*(x1-x2)
					print(f"{sayi:.2f}")
				elif y3<y2+((y1-y2)/(x1-x2))*(x3-x2) and y0<y2+((y1-y2)/(x1-x2))*(x0-x2):
					sayi=((y1+y2)/2)*(x1-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y2>y1:
				if y3>y2-((y2-y1)/(x1-x2))*(x3-x2) and y0>y2-((y2-y1)/(x1-x2))*(x0-x2):
					sayi=((y1+y2)/2)*(x1-x2)
					print(f"{sayi:.2f}")
				elif y3<y2-((y2-y1)/(x1-x2))*(x3-x2) and y0<y2-((y2-y1)/(x1-x2))*(x0-x2):
					sayi=((y1+y2)/2)*(x1-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y1 == y2:
				if y0 > y1 and y3 > y1:
					sayi=((y2+y1)/2)*(x1-x2)
					print(f"{sayi:.2f}")
				elif y0 < y1 and y3 < y1:
					sayi=((y2+y1)/2)*(x1-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
		elif c == x3:
			if y1>y3:
				if y2>y3+((y1-y3)/(x1-x3))*(x2-x3) and y0<y3+((y1-y3)/(x1-x3))*(x0-x3):
					sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y2<y3+((y1-y3)/(x1-x3))*(x2-x3) and y0>y3+((y1-y3)/(x1-x3))*(x0-x3):
					sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
					print(f"{sayi:.2f}")
				elif y2>y3+((y1-y3)/(x1-x3))*(x2-x3) and y0>y3+((y1-y3)/(x1-x3))*(x0-x3):
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x1-x3)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x1-x3)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")							
				elif y2<y3+((y1-y3)/(x1-x3))*(x2-x3) and y0<y3+((y1-y3)/(x1-x3))*(x0-x3):
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")			
			elif y1<y3:
				if y2>y3-((y3-y1)/(x1-x3))*(x2-x3) and y0<y3-((y3-y1)/(x1-x3))*(x0-x3):
					sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y2<y3-((y3-y1)/(x1-x3))*(x2-x3) and y0>y3-((y3-y1)/(x1-x3))*(x0-x3):
					sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
					print(f"{sayi:.2f}")
				elif y2>y3-((y3-y1)/(x1-x3))*(x2-x3) and y0>y3-((y3-y1)/(x1-x3))*(x0-x3):
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x1-x3)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x1-x3)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")	
				elif y2<y3-((y3-y1)/(x1-x3))*(x2-x3) and y0<y3-((y3-y1)/(x1-x3))*(x0-x3):
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")			
			elif y1==y3:
				if y2>y1 and y0<y1:
					sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
					print(f"{sayi:.2f}")	
				elif y2<y1 and y0>y1:
					sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
					print(f"{sayi:.2f}")
				elif y2>y1 and y0>y1:
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
				elif y2<y1 and y0<y1:
					if y2>y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y2<y0:
						sayi=((y1+y3)/2)*(x1-x3)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")		
									
	elif b == x2:
		if c == x1:
			if y2>y1:
				if y3>y1+((y2-y1)/(x2-x1))*(x3-x1) and y0>y1+((y2-y1)/(x2-x1))*(x0-x1):
					sayi=((y1+y2)/2)*(x2-x1)
					print(f"{sayi:.2f}")
				elif y3<y1+((y2-y1)/(x2-x1))*(x3-x1) and y0<y1+((y2-y1)/(x2-x1))*(x0-x1):
					sayi=((y1+y2)/2)*(x2-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y1>y2:
				if y3>y1-((y1-y2)/(x2-x1))*(x3-x1) and y0>y1-((y1-y2)/(x2-x1))*(x0-x1):
					sayi=((y2+y1)/2)*(x2-x1)
					print(f"{sayi:.2f}")
				elif y3<y1-((y1-y2)/(x2-x1))*(x3-x1) and y0<y1-((y1-y2)/(x2-x1))*(x0-x1):
					sayi=((y2+y1)/2)*(x2-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
			elif y1 == y2:
				if y0 > y2 and y3 > y2:
					sayi=((y2+y1)/2)*(x2-x1)
					print(f"{sayi:.2f}")
				elif y0 < y2 and y3 < y2:
					sayi=((y2+y1)/2)*(x2-x1)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
		elif c == x3:
			if y2>y3:
				if y1>y3+((y2-y3)/(x2-x3))*(x1-x3) and y0>y3+((y2-y3)/(x2-x3))*(x0-x3):
					sayi=((y2+y3)/2)*(x2-x3)
					print(f"{sayi:.2f}")
				elif y1<y3+((y2-y3)/(x2-x3))*(x1-x3) and y0<y3+((y2-y3)/(x2-x3))*(x0-x3):
					sayi=((y2+y3)/2)*(x2-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y3>y2:
				if y1>y3-((y3-y2)/(x2-x3))*(x1-x3) and y0>y3-((y3-y2)/(x2-x3))*(x0-x3):
					sayi=((y3+y2)/2)*(x2-x3)
					print(f"{sayi:.2f}")
				elif y1<y3-((y3-y2)/(x2-x3))*(x1-x3) and y0<y3-((y3-y2)/(x2-x3))*(x0-x3):
					sayi=((y3+y2)/2)*(x2-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
			elif y2 == y3:
				if y0 > y2 and y1 > y2:
					sayi=((y3+y2)/2)*(x2-x3)
					print(f"{sayi:.2f}")
				elif y0 < y2 and y1 < y2:
					sayi=((y3+y2)/2)*(x2-x3)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
		elif c == x0:
			if y2>y0:
				if y1>y0+((y2-y0)/(x2-x0))*(x1-x0) and y3<y0+((y2-y0)/(x2-x0))*(x3-x0):
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y0+((y2-y0)/(x2-x0))*(x1-x0) and y3>y0+((y2-y0)/(x2-x0))*(x3-x0):
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
					print(f"{sayi:.2f}")
				elif y1>y0+((y2-y0)/(x2-x0))*(x1-x0) and y3>y0+((y2-y0)/(x2-x0))*(x3-x0):
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x2-x0)+abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x2-x0)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
							print(f"{sayi:.2f}")	
				elif y1<y0+((y2-y0)/(x2-x0))*(x1-x0) and y3<y0+((y2-y0)/(x2-x0))*(x3-x0):
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x2-x0)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x2-x0)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")					
			elif y2<y0:
				if y1>y0-((y0-y2)/(x2-x0))*(x1-x0) and y3<y0-((y0-y2)/(x2-x0))*(x3-x0):
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y0-((y0-y2)/(x2-x0))*(x1-x0) and y3>y0-((y0-y2)/(x2-x0))*(x3-x0):
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
					print(f"{sayi:.2f}")	
				elif y1>y0-((y0-y2)/(x2-x0))*(x1-x0) and y3>y0-((y0-y2)/(x2-x0))*(x3-x0):
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x2-x0)+abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x2-x0)+abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")	
				elif y1<y0-((y0-y2)/(x2-x0))*(x1-x0) and y3<y0-((y0-y2)/(x2-x0))*(x3-x0):
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1==y3:
						if x3>x1:
							sayi=((y0+y2)/2)*(x2-x0)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
							print(f"{sayi:.2f}")
						elif x1>x3:	
							sayi=((y0+y2)/2)*(x2-x0)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
							print(f"{sayi:.2f}")			
			elif y2==y0:
				if y1>y0 and y3<y0:
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y3+x3*y0-x2*y0-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
				elif y1<y0 and y3>y0:
					sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y2+x2*y1+x1*y0-x2*y0-x1*y2-x0*y1)/2)
					print(f"{sayi:.2f}")
				elif y1>y0 and y3>y0:
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)+abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")
				elif y1<y0 and y3<y0:
					if y1<y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y1+x1*y2+x2*y0-x1*y0-x2*y1-x0*y2)/2)
						print(f"{sayi:.2f}")
					elif y1>y3:
						sayi=((y2+y0)/2)*(x2-x0)-abs((x0*y3+x3*y2+x2*y0-x3*y0-x2*y3-x0*y2)/2)
						print(f"{sayi:.2f}")				
	elif b == x3:
		if c == x2:
			if y3>y2:
				if y1>y2+((y3-y2)/(x3-x2))*(x1-x2) and y0>y2+((y3-y2)/(x3-x2))*(x0-x2):
					sayi=((y2+y3)/2)*(x3-x2)
					print(f"{sayi:.2f}")
				elif y1<y2+((y3-y2)/(x3-x2))*(x1-x2) and y0<y2+((y3-y2)/(x3-x2))*(x0-x2):
					sayi=((y2+y3)/2)*(x3-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y2>y3:
				if y1>y2-((y2-y3)/(x3-x2))*(x1-x2) and y0>y2-((y2-y3)/(x3-x2))*(x0-x2):
					sayi=((y3+y2)/2)*(x3-x2)
					print(f"{sayi:.2f}")
				elif y1<y2-((y2-y3)/(x3-x2))*(x1-x2) and y0<y2-((y2-y3)/(x3-x2))*(x0-x2):
					sayi=((y3+y2)/2)*(x3-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y2 == y3:
				if y1 > y2 and y0 > y2:
					sayi=((y3+y2)/2)*(x3-x2)
					print(f"{sayi:.2f}")
				elif y1 < y2 and y0 < y2:
					sayi=((y3+y2)/2)*(x3-x2)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
		elif c == x0:
			if y3>y0:
				if y2>y0+((y3-y0)/(x3-x0))*(x2-x0) and y1>y0+((y3-y0)/(x3-x0))*(x1-x0):
					sayi=((y0+y3)/2)*(x3-x0)
					print(f"{sayi:.2f}")
				elif y2<y0+((y3-y0)/(x3-x0))*(x2-x0) and y1<y0+((y3-y0)/(x3-x0))*(x1-x0):
					sayi=((y0+y3)/2)*(x3-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y3<y0:
				if y2>y0-((y0-y3)/(x3-x0))*(x2-x0) and y1>y0-((y0-y3)/(x3-x0))*(x1-x0):
					sayi=((y0+y3)/2)*(x3-x0)
					print(f"{sayi:.2f}")
				elif y2<y0-((y0-y3)/(x3-x0))*(x2-x0) and y1<y0-((y0-y3)/(x3-x0))*(x1-x0):
					sayi=((y0+y3)/2)*(x3-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")	
			elif y3 == y0:
				if y2 > y0 and y1 > y0:
					sayi=((y0+y3)/2)*(x3-x0)
					print(f"{sayi:.2f}")
				elif y2 < y0 and y1 < y0:
					sayi=((y0+y3)/2)*(x3-x0)-abs((x0*y1+x1*y2+x2*y3+x3*y0-x1*y0-x2*y1-x3*y2-x0*y3)/2)
					print(f"{sayi:.2f}")
		elif c == x1:
			if y3>y1:
				if y0>y1+((y3-y1)/(x3-x1))*(x0-x1) and y2<y1+((y3-y1)/(x3-x1))*(x2-x1):
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y2+x2*y3+x3*y1-x2*y1-x3*y2-x1*y3)/2)
					print(f"{sayi:.2f}")
				elif y0<y1+((y3-y1)/(x3-x1))*(x0-x1) and y2>y1+((y3-y1)/(x3-x1))*(x2-x1):
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y0+x0*y3+x3*y1-x0*y1-x3*y0-x1*y3)/2)
					print(f"{sayi:.2f}")
				elif y0>y1+((y3-y1)/(x3-x1))*(x0-x1) and y2>y1+((y3-y1)/(x3-x1))*(x2-x1):
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x3-x1)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x3-x1)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")	
				elif y0<y1+((y3-y1)/(x3-x1))*(x0-x1) and y2<y1+((y3-y1)/(x3-x1))*(x2-x1):
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x3-x1)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x3-x1)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")			
			elif y3<y1:
				if y0>y1-((y1-y3)/(x3-x1))*(x0-x1) and y2<y1-((y1-y3)/(x3-x1))*(x2-x1):
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y2+x2*y3+x3*y1-x2*y1-x3*y2-x1*y3)/2)
					print(f"{sayi:.2f}")
				elif y0<y1-((y1-y3)/(x3-x1))*(x0-x1) and y2>y1-((y1-y3)/(x3-x1))*(x2-x1):
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y0+x0*y3+x3*y1-x0*y1-x3*y0-x1*y3)/2)
					print(f"{sayi:.2f}")	
				elif y0>y1-((y1-y3)/(x3-x1))*(x0-x1) and y2>y1-((y1-y3)/(x3-x1))*(x2-x1):	
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x3-x1)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x3-x1)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")	
				elif y0<y1-((y1-y3)/(x3-x1))*(x0-x1) and y2<y1-((y1-y3)/(x3-x1))*(x2-x1):	
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
					elif y2==y0:
						if x2>x0:
							sayi=((y1+y3)/2)*(x3-x1)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
							print(f"{sayi:.2f}")
						elif x0>x2:	
							sayi=((y1+y3)/2)*(x3-x1)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
							print(f"{sayi:.2f}")				
			elif y3==y1:
				if y0>y1 and y2<y1:
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y2+x2*y3+x3*y1-x2*y1-x3*y2-x1*y3)/2)
					print(f"{sayi:.2f}")
				elif y0<y1 and y2>y1:
					sayi=((y3+y1)/2)*(x3-x1)-abs((x1*y0+x0*y3+x3*y1-x0*y1-x3*y0-x1*y3)/2)
					print(f"{sayi:.2f}")
				elif y0>y1 and y2>y1:
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)+abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")
				elif y0<y1 and y2<y1:
					if y0<y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x0*y1+x1*y3+x3*y0-x1*y0-x3*y1-x0*y3)/2)
						print(f"{sayi:.2f}")
					elif y0>y2:
						sayi=((y3+y1)/2)*(x3-x1)-abs((x2*y1+x1*y3+x3*y2-x1*y2-x3*y1-x2*y3)/2)
						print(f"{sayi:.2f}")		
			 	




