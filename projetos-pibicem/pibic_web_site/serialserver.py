# -*- conding: utf-8 -*-
import time
import serial
#import MySQLdb

#Consumo do ar condicionado em kW = (Potencia em Watts X numero de horas ligado X numero de dias ligado ) / 1000

#con = MySQLdb.connect('localhost', 'root', '')
#con.select_db('ar_condicionado')
#cursor = con.cursor()

# Iniciando conexao serial
comport = serial.Serial('/dev/ttyACM0', 9600)
try:
    while(True):
#        t = ''
#        h = ''
        time.sleep(1)
        valor_serial = comport.readline()
        valor = valor_serial
        print valor
#        if len(valor.split()) == 2:
#            t, h = valor.split(' ')
#            cursor.execute("UPDATE real_time SET temperatura=%s" % t)
#            cursor.execute("UPDATE real_time SET humidade=%s" % h)
#            con.commit()
    # Fechando conexao serial
    comport.close()
except KeyboardInterrupt:
    exit()

#cursor.close()