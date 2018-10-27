f1 = open('result.txt','r')
f2 = open('testing_answer.txt','r')
line1 = f1.readlines()

line2 = f2.readlines()
print(len(line1),len(line2))
cnt = 0
for x in range(len(line1)):
	line1[x] = line1[x][3:]
	#print(line1[x])
	if line1[x] == line2[x]:
		#print('right')
		cnt +=1
print(cnt/2500)
# while f1.getline!='':
# 	tmp = 
