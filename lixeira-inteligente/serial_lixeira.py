from firebase import firebase
firebase = firebase.FirebaseApplication('https://catador-amigo-c9184.firebaseio.com/',  None)
import serial
import time

arduino = serial.Serial('/dev/ttyACM1')

c = 10
ant = 0
try:
	while True:
		data = arduino.readline()

		if "L1LVB" in str(data):
			
			aux = str(data).replace('b', '').replace('\\r\\n', '').replace("'", '')
			print(aux.split('B')[3], aux.split('B')[1])
			ant = aux.split('B')[3]
	        
			if c == 10:
			    if aux.split('B')[3] != 0:
				    firebase.put('/lixeira/ecoponto1/', 'peso', float(aux.split('B')[3]))
				    firebase.put('/lixeira/ecoponto1/', 'alimentacao2', float(aux.split('B')[1]))
			    else:
				    firebase.put('/lixeira/ecoponto1/', 'peso', int(ant))

		if "L1LVC" in str(data):
			
			aux = str(data).replace('b', '').replace('\\r\\n', '').replace("'", '')
			print(aux.split('C')[3], aux.split('C')[1])
			ant = aux.split('C')[3]

			if c == 10:
			    if aux.split('C')[3] != 0:
				    firebase.put('/lixeira/ecoponto1/', 'capacidade', int(aux.split('C')[3]))
				    firebase.put('/lixeira/ecoponto1/', 'alimentacao1', float(aux.split('C')[1]))
			    else:
				    firebase.put('/lixeira/ecoponto1/', 'capacidade', int(ant))
	    

except:
	print("end")