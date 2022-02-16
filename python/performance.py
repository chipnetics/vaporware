# -*- coding: utf-8 -*-


import os
import pandas as pd
import numpy as np


performancefile = "C:\\OneDrive\\Python\\testing_code\\results\\performance.txt" 
resultsobj = open(performancefile,"w")

daytodayfile = "C:\\OneDrive\\Python\\testing_code\\results\\daytoday_profits.txt" 
daytodayobj = open(daytodayfile,"w")

resultsobj.write("SYMBOL")
resultsobj.write("\t")
resultsobj.write("TRUE OPEN")
resultsobj.write("\t")
resultsobj.write("PRED OPEN")
resultsobj.write("\t")
resultsobj.write("PREV CLOSE")
resultsobj.write("\t")
resultsobj.write("PRED MARGIN")
resultsobj.write("\t")
resultsobj.write("TRUE MARGIN")
resultsobj.write("\t")
resultsobj.write("OUTCOME")
resultsobj.write("\t")
resultsobj.write("PROFIT (100)")
resultsobj.write("\n")

indexptr = -1
portfolio_margin = 0

for filename in os.listdir('stocks'):
    print(filename)
  
    strippedsymbol = filename.strip(".txt")
    
    filename="C:\\OneDrive\\Python\\testing_code\\stocks\\" + strippedsymbol + ".txt"
    resultsfile="C:\\OneDrive\\Python\\testing_code\\results\\results.txt"
    
    dayoffset = 0
    
    df = pd.read_csv(filename, sep=',')
    data_open = np.asarray(df.iloc[dayoffset:,1])
    data_prev_day_close = np.asarray(df.iloc[dayoffset+1:,1])
    
    dg = pd.read_csv(resultsfile, sep='\t')
    predict_data_names = np.asarray(dg.iloc[:,0])
    predict_data_open = np.asarray(dg.iloc[:,2])
    
    resultsobj.write(strippedsymbol)        #SYMBOL
    resultsobj.write("\t")
    resultsobj.write(str(data_open[0]))     #TRUE OPEN
    resultsobj.write("\t")
    
    resultsobj.write(str(predict_data_open[indexptr]))   #PREDICT OPEN
    resultsobj.write("\t")
    resultsobj.write(str(data_prev_day_close[0]))  #PREV CLOSE
    resultsobj.write("\t")
    
    predicted_margin = predict_data_open[indexptr]/data_prev_day_close[0]
    true_margin = data_open[0]/data_prev_day_close[0]
   
    resultsobj.write(str(predicted_margin)) # Pred Margin
    resultsobj.write("\t")
    resultsobj.write(str(true_margin)) # TRUE MARGIN
    
    resultsobj.write("\t")
    
    if (predicted_margin < 1 and true_margin <1) or (predicted_margin >=1 and true_margin>=1):
        resultsobj.write("WIN")
    else:
        resultsobj.write("LOSS")
        
    resultsobj.write("\t")
    resultsobj.write(str(true_margin*100-100)) # PROFIT(100)
    
    resultsobj.write("\n")
    
    indexptr+=1
    portfolio_margin+= true_margin*100-100
    
#Record the overall gain or loss for this portfolio on the day.
daytodayobj.write(str(portfolio_margin))
daytodayobj.write("\n")
daytodayobj.close()

resultsobj.close()