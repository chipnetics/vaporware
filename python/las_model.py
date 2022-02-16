import pandas as pd
import numpy as np

from datetime import datetime

df = pd.read_csv('3WLA.txt',sep='\t')

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
    
    if (np.subtract(datetime.strptime(df.loc[i , "Start"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days <0) & (np.subtract(datetime.strptime(df.loc[i , "Finish"], "%d-%b-%Y"),datetime.strptime(df.loc[i , "Week-End"], "%d-%b-%Y")).days >0):
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


###### START MACHINE LEARNING MODEL

from sklearn import preprocessing
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPRegressor

df_ml = df[df["Actual Duration"]>0]

# LabelEncode some columns
label_encoder = LabelEncoder()

# Encoding for String items...
shaft_int_encoded = label_encoder.fit_transform(df_ml["Shaft"].astype(str))
discipline_int_encoded = label_encoder.fit_transform(df_ml["Discipline"].astype(str))
critical_path_int_encoded = label_encoder.fit_transform(df_ml["Critical Path"].astype(str))
extra_work_int_encoded = label_encoder.fit_transform(df_ml["Extra Work"].astype(str))

input_data_collection = np.asarray([shaft_int_encoded,
                                    discipline_int_encoded,
                                    critical_path_int_encoded,
                                    extra_work_int_encoded,
                                    df_ml["Initial Duration"]])

output_data_collection = np.asarray([df_ml["Actual Duration"]])

data = input_data_collection.transpose()
target = output_data_collection.transpose()

data_scaler = preprocessing.MinMaxScaler()
target_scaler = preprocessing.MinMaxScaler()

data_fitted = data_scaler.fit_transform(data)
target_fitted = target_scaler.fit_transform(target.reshape(-1,1))

x_train, x_test, y_train, y_test = train_test_split(data_fitted,target_fitted,test_size=0.80)

classifier = MLPRegressor(max_iter=10000, activation='relu', solver='adam', random_state=1)

classifier.fit(x_train,np.ravel(y_train))

#Prediction model... (using the complete data set)
# Encoding for String items...
p_shaft_int_encoded = label_encoder.fit_transform(df["Shaft"].astype(str))
p_discipline_int_encoded = label_encoder.fit_transform(df["Discipline"].astype(str))
p_critical_path_int_encoded = label_encoder.fit_transform(df["Critical Path"].astype(str))
p_extra_work_int_encoded = label_encoder.fit_transform(df["Extra Work"].astype(str))

predict_input_data_collection = np.asarray([p_shaft_int_encoded,
                                            p_discipline_int_encoded,
                                            p_critical_path_int_encoded,
                                            p_extra_work_int_encoded,
                                            df["Initial Duration"]])
predict_data = predict_input_data_collection.transpose()

predict_data_scaler = preprocessing.MinMaxScaler()
predict_data_fitted = predict_data_scaler.fit_transform(predict_data)

output_predicted = classifier.predict(predict_data_fitted)
output_predicted_unscaled = target_scaler.inverse_transform(output_predicted.reshape(-1,1))

df["Predicted Duration"] = output_predicted_unscaled

# Output the final data frame.
df.to_csv("3WLA analysis.txt", sep='\t') 




