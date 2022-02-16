import os

from stocks import analyze_stock

resultsfile = "C:\\OneDrive\\Python\\testing_code\\results\\results.txt" 
resultsobj = open(resultsfile,"w")

resultsobj.write("SYMBOL")
resultsobj.write("\t")
resultsobj.write("DAY")
resultsobj.write("\t")
resultsobj.write("PRED")
resultsobj.write("\t")
resultsobj.write("MARGIN")
resultsobj.write("\n")
    

for days in range(1,5):
    print(days)
    for filename in os.listdir('stocks'):
         print(filename)
         analyze_stock(filename,1,resultsobj)
     
resultsobj.close()