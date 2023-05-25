# Furnace_Temp_Data_Log.py
#	by: R.Chaffer
# ***************************** Run Notes ***************************** #
# - Data Log file will save to the same folder directory as python script.
#
#
#
# ********************************************************************* #

# --------------- #
# Import Packages #
# --------------- #
import serial 
import datetime

# Specify the port in use for arduino communication and the baud rate of the arduino's serial communication
arduino_port = "COM5"
baud = 9600

# Define date format
ct = datetime.datetime.now()
ct = ct.strftime('%Y%m%d%H%M%S')

# Generate File name for the log. Using datetime as part of the entry allow for a unique log file name every run. 
fileName = ct + "_Chamber_Temp_Log.csv"

# Access the arduino Serial communication and create the Log file.
ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "w")
print("Created file")


samples = 60000 #how many samples to collect
print_labels = False
line = 0 #start at 0 because our header is 0 (not real data)
while line <= samples:
    # When the value of line = the value of samples, the loop will end
	getData=str(ser.readline())
	data=getData.replace("b'","")
	data=data.replace(",\\r\\n'","")
	# ct stores current time
	ct = datetime.datetime.now()
	

	ct = ct.strftime("%H:%M:%S.%f")

	
	# Print the Time in the first column, then the data collected by the arduino
	printstring = ct+","+data
	print(printstring)

	file = open(fileName, "a")
	file.write(printstring + "\n") #write data with a newline
	line = line+1

# After data collection is complete, print message and close .CSV file.
print("Data collection complete!")
file.close()
