f=open('Tp1Ej1.in')
while True:
	li=map(float,f.readline().rsplit())
	if li[0]<0: break
	li.pop(0)
	li.sort()
	mi,count=li.pop(0)+1,1
	for x in li:
		if x>mi:
			count+=1
			mi=x+1
	print count
