import os
import time
import shutil
import sys

while True:
    data_type = raw_input("Which type? [data or pythia]")
    print data_type

    data_type_str = str(data_type)
    input_normal = str(data_type)
    input_pythia = str(data_type)
            
    if data_type == 'data' or data_type == 'pythia':
        if data_type == 'pythia':
            print 'RUN MC PYTHIA'

            #-----------timing-----------
            start_time = time.time()
            print("--- %s seconds ---" % (time.time() - start_time))

            #-----------the program----------
            if os.path.exists('Output_Pythia'):                 #look if a directory exist
                shutil.rmtree('Output_Pythia')                  #delete directory
            os.system("mkdir Output_Pythia")                     #create a directory#

            with open('pythia.txt') as g:
                NumberLinesData = sum(1 for _ in g)             #number of lines of a file.txt
                print 'Numbers of lines - Pythia file: ',NumberLinesData

            j = 0
            while j < NumberLinesData:                          #loop data.txt
                k = j+1                                         #To make file withot zero index
                if os.path.exists('JPSIAnaOpenData2011.py'):    #look if a file exist
                    os.remove('JPSIAnaOpenData2011.py')         #delete the file
                with open('JPSIAnaOpenData2011_Model_MC.py') as f: 
                    NumberLines = sum(1 for _ in f)             #number of lines of a model file
                fileModel = open('JPSIAnaOpenData2011_Model_MC.py')#Open the model file 
                LinesModel = fileModel.readlines()
                fileModel.close()

                print '------------WRITING FILE------------------------'
                fileOutput = open('JPSIAnaOpenData2011.py', 'w') #output file
                fileData = open('pythia.txt', 'r')              #open data file
                LinesData = fileData.readlines()
                i = 0

                #loop write file
                while i < NumberLines:                          
                    LinesSrc = str(LinesModel[i])
                    a = str(LinesData[j])
                    b = a.rstrip('\n')                          #delete the end of line
                    if i == 13 or i== 24:
                        if i == 13:
                            fileOutput.write("    '%s'" % (b)  )
                            i = i+1
                        if i == 24:
                            fileOutput.write("    'pythia_histo%s.root'" % (k) )
                            i = i+1
                    else :
                        fileOutput.write(LinesSrc)
                        i = i+1
                #end of loop write file
                fileData.close()    #close file
                fileOutput.close() #close file
                
                print '------------Fill Histograms------------------------'
                j = j + 1
                os.system("cmsRun JPSIAnaOpenData2011.py > pythia_log%s.log" % (k) ) #Command cmsRun
                print '------------pythia_histo%s.root done------------' % (k)

                #print '------------BEGIN READIND------------------------'
                #file2 = open('JPSIAnaOpenData2011.py', 'r') #open the file for read
                #print file2.read()                          #print the whole file
                #file2.close()                               
                #print '------------END READIND------------------------'
                
                shutil.move('pythia_log%s.log' % (k),'Output_Pythia')    #move file to a folder
                shutil.move('pythia_histo%s.root' % (k),'Output_Pythia')
                #End write file

            hour = (time.time() - start_time)/3600
            minutes = (time.time() - start_time)/60
            print("--- %s min ---" % (minutes))
            print("--- %s hours ---" % (hour))    
            print 'End Program'
            break
    #--------------------------------------------------------        
    #--------------------------------------------------------    
    #--------------------------------------------------------        
        if data_type == 'data':
            print 'RUN DATA'

            #-----------timing-----------
            start_time = time.time()
            print("--- %s seconds ---" % (time.time() - start_time))

            #-----------the program----------
            if os.path.exists('Output_Data'):                        #look if a directory exist
                shutil.rmtree('Output_Data')                     #delete directory
            os.system("mkdir Output_Data")                           #create a directory#

            with open('data.txt') as g:
                NumberLinesData = sum(1 for _ in g)             #number of lines of a file.txt
                print 'Numbers of lines - data file: ',NumberLinesData

            j = 0
	    data_type = raw_input("From which file do you want to begin? [number (remember the index)]")
	    j = int(data_type)
            while j < NumberLinesData:                          #loop data.txt
                k = j+1 #To make file withot zero index
                if os.path.exists('JPSIAnaOpenData2011.py'):    #look if a file exist
                    os.remove('JPSIAnaOpenData2011.py')         #delete the file
                with open('JPSIAnaOpenData2011_Model_Trigger.py') as f: 
                    NumberLines = sum(1 for _ in f)             #number of lines of a model file
                fileModel = open('JPSIAnaOpenData2011_Model_Trigger.py')#Open the model file 
                LinesModel = fileModel.readlines()
                fileModel.close()

                print '------------WRITING FILE------------------------'
                fileOutput = open('JPSIAnaOpenData2011.py', 'w') #output file
                fileData = open('data.txt', 'r')                    #open data file
                LinesData = fileData.readlines()
                i = 0 
		
                #loop write file
                while i < NumberLines:                          
                    LinesSrc = str(LinesModel[i])
                    a = str(LinesData[j])
                    b = a.rstrip('\n')                          #delete the end of line
                    if i == 13 or i== 24:
                        if i == 13:
                            fileOutput.write("    '%s'" % (b)  )
                            i = i+1
                        if i == 24:
                            fileOutput.write("    'data_histo%s.root'" % (k) )
                            i = i+1
                    else :
                        fileOutput.write(LinesSrc)
                        i = i+1
                #end of loop write file
                fileData.close()
                fileOutput.close()
                
                print '------------Fill Histograms------------------------'
                j = j + 1
                os.system("cmsRun JPSIAnaOpenData2011.py > data_log%s.log" % (k) ) #Command cmsRun
                print '------------data_histo%s.root done------------' % (k)

                #print '------------BEGIN READIND------------------------'
                #file2 = open('JPSIAnaOpenData2011.py', 'r') 
                #print file2.read() 
                #file2.close()
                #print '------------END READIND------------------------'
                
                shutil.move('data_log%s.log' % (k),'Output_Data')    #move file to a folder
                shutil.move('data_histo%s.root' % (k),'Output_Data')
                #End write file

            hour = (time.time() - start_time)/3600
            minutes = (time.time() - start_time)/60
            print("--- %s min ---" % (minutes))
            print("--- %s hours ---" % (hour))    
            print 'End Program'         
            break
    else:
        print '-----------------------------------------------------------'
        print 'WRONG WORDS. TRY AGAIN'
        print '-----------------------------------------------------------'
        continue
        #print 'EXITING PROGRAM'
#sys.exit("Error message")
