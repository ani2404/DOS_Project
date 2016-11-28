import sys
import json

with open('ddl.json') as json_data:
	d = json.load(json_data)
	listOfDevices=[]
	ListOfTopics=[]
	
	
	
	##print d['DDL']['Platform']
	k=d['DDL']['Platform']
	##print d['DDL']['Platform']
	for i in range(0,3):
		ListOfTopics.append([])
		
		listOfDevices.append(k['Device'][i]['Properties']['Name'])
	
		for j in range(0,len(k['Device'][i]['Interface']['Pin']['Topic'])):
					   ListOfTopics[i].append(k['Device'][i]['Interface']['Pin']['Topic'][j]['Name'])
					   print ListOfTopics[i][j]
											  
											  
	#print  ListOfTopics
					   
					   
				   
	print len(k['Device'])
##with open("./ddl.json,"r+") as f:

	##orig_stdout = sys.stdout
	#f = file('output.txt', 'w')
	#sys.stdout = f

	#print "" 
	#sys.stdout = orig_stdout
	
	#print "complete"