import pandas as pd
import numpy as np

from datetime import datetime

df = pd.read_csv('3WLA.txt',sep='\t', encoding='latin1')

epoch = datetime(1980, 1, 1)
df["Cardinality TW"] = df["Week-End"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y")-epoch))
df["Activity CL"] =  df["Activity"].apply(lambda x: x.lower().strip().replace(" ", ""))
df["Activity CLSH"] =  df["Activity CL"].apply(lambda x: str(x)) + df["Shaft"]

df["Initial Index"] = df["Activity CLSH"].apply(lambda x: (x==df["Activity CLSH"]).idxmax())
df["Initial Start"] = df["Initial Index"].apply(lambda x: df.loc[x , "Start"])
df["Initial Finish"] = df["Initial Index"].apply(lambda x: df.loc[x , "Finish"])

df["Appears"] = df["Activity CLSH"].apply(lambda x: df.index[(df["Activity CLSH"] == x)].tolist()) # List of index apperances
df["Plan Duration"] = np.subtract(df["Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))


# LW Index Loop
df["LW Index"] = 0
for i in df.index:
    focus_desc = df.loc[i , "Activity CLSH"]
    
    for j in df.index:
        if (j<i) & (df.loc[j , "Activity CLSH"]==focus_desc):
            df.loc[i , "LW Index"]=j
            
# Completed flag
df["Completed"] = "Not Started"
df["To Finish TW"] = "No"
df["To Finish LW"] = "No"
for i in df.index:
    
    original_days = np.subtract(datetime.strptime(df.loc[i , "Initial Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Initial Start"], "%d-%b-%Y")).days+1
    df.loc[i , "Initial Duration"]=original_days
    
    if np.subtract(datetime.strptime(df.loc[i , "Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days <=0:
        actual_days = np.subtract(datetime.strptime(df.loc[i , "Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Start"], "%d-%b-%Y")).days+1
        
        df.loc[i , "Completed"]="Completed"
        df.loc[i , "Actual Duration"]=actual_days
        
        df.loc[i , "Var Days Duration"]=original_days-actual_days
        df.loc[i , "Var % Duration"]=(actual_days-original_days)/original_days
    elif np.subtract(datetime.strptime(df.loc[i , "Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days <7:
        df.loc[i , "To Finish TW"]="Yes"
    
    if (np.subtract(datetime.strptime(df.loc[i , "Start"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days <=0) & (np.subtract(datetime.strptime(df.loc[i , "Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days >0):
        df.loc[i , "Completed"]="In Progress"
        

df["To Finish LW"] = df["LW Index"].apply(lambda x: df.loc[x , "To Finish TW"])
df.loc[df["LW Index"] == 0, "To Finish LW"] = "No"  #Post-lambda correction to new look-ahead entries
            
df["LW Start"] = df["LW Index"].apply(lambda x: df.loc[x , "Start"])
df["LW Finish"] = df["LW Index"].apply(lambda x: df.loc[x , "Finish"])

df["OV Start Slip"] = np.subtract(df["Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["Initial Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))
df["OV Finish Slip"] = np.subtract(df["Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["Initial Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))
df["LW Start Slip"] = np.subtract(df["Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["LW Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))
df["LW Finish Slip"] = np.subtract(df["Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["LW Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))
df["LW Duration Slip"] = np.add(np.subtract(df["Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y")))),np.subtract(df["LW Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["LW Finish"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y")))))


## Filters
df["Movement"]= (df["LW Duration Slip"]!="0 days") | (df["LW Start Slip"]!="0 days") | (df["LW Finish Slip"]!="0 days")
df["Immenence"] = np.subtract(df["Start"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))),df["Week-End"].apply(lambda x: (datetime.strptime(x, "%d-%b-%Y"))))
df["First Appear"] =  df.index==df["Initial Index"]
df["Surprise"] =  (df["First Appear"]) & (df["Immenence"].dt.days <14) & (df["Completed"] != "Completed")
#df["PD Date"] = pd.to_datetime(df["Week-End"])

#Change movements to just days
df["Plan Duration"]  = df["Plan Duration"].apply(lambda x: x.days+1)
df["OV Start Slip"]  = df["OV Start Slip"].apply(lambda x: x.days)
df["OV Finish Slip"]  = df["OV Finish Slip"].apply(lambda x: x.days)
df["LW Start Slip"]  = df["LW Start Slip"].apply(lambda x: x.days)
df["LW Finish Slip"]  = df["LW Finish Slip"].apply(lambda x: x.days)
df["LW Duration Slip"]  = df["LW Duration Slip"].apply(lambda x: x.days)
df["Immenence"] = df["Immenence"].apply(lambda x: x.days)

#Drop Unrequired columns from dataframe (not needed in output)
df = df.drop(columns=["Cardinality TW","Activity CL", "Activity CLSH","Initial Index","Appears","LW Index"])

# Output the final data frame.
df.to_csv("3WLA analysis.txt", sep='\t') 




