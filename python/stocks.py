import numpy as np

from sklearn import preprocessing
from sklearn.model_selection import train_test_split

import pandas as pd

import time

from neupy.layers import join,Input,Sigmoid
from neupy import environment
from neupy import algorithms
from neupy import plots

# dayoffset is for testing historical data.  dayoffset=0 means run for today.
def analyze_stock(stocksymbol,dayoffset,resultsobj):
    
    t0 = time.time()
    
    strippedsymbol = stocksymbol.strip(".txt")
    
    filename="C:\\OneDrive\\Python\\testing_code\\stocks\\" + strippedsymbol + ".txt"
    figurename = "C:\\OneDrive\\Python\\testing_code\\figures\\" + strippedsymbol + ".png"
    
    # Import data needs a header.
    df = pd.read_csv(filename, sep=',')
    
    input_data_open = np.asarray(df.iloc[dayoffset+1:,1])     #Start at row 1 (skip today):allrows,column
    input_data_high = np.asarray(df.iloc[dayoffset+1:,2])     #Start at row 1 (skip today):allrows,column
    input_data_low = np.asarray(df.iloc[dayoffset+1:,3])      #Start at row 1 (skip today):allrows,column
    input_data_close = np.asarray(df.iloc[dayoffset+1:,4])    #Start at row 1 (skip today):allrows,column
    input_data_volume = np.asarray(df.iloc[dayoffset+1:,5])   #Start at row 1 (skip today):allrows,column
    
    output_target_nextdayopen = np.asarray(df.iloc[:-(dayoffset+1),1]) #Start at row 0:allrows less 1,column
    
    input_data_collection = np.asarray([input_data_open,input_data_high,input_data_low,input_data_close,input_data_volume])
    
    input_data_collection_trans = input_data_collection.transpose()
    
    data = input_data_collection_trans
    target =output_target_nextdayopen
    
    data_scaler = preprocessing.MinMaxScaler()
    target_scaler = preprocessing.MinMaxScaler()
    
    data_fitted = data_scaler.fit_transform(data)
    target_fitted = target_scaler.fit_transform(target.reshape(-1, 1))
    
    environment.reproducible()
    
    x_train, x_test, y_train, y_test = train_test_split(data_fitted, target_fitted, test_size=0.20)
    
    n_inputs = 5
    n_outputs = 1
    
    network = join(
        Input(n_inputs),
        Sigmoid(50),
        Sigmoid(n_outputs),
    )
    
    lmnet = algorithms.GradientDescent(
        network,
        step=0.2,
        shuffle_data=True,
    )
    
    lmnet.train(x_train, y_train, x_test, y_test, epochs=2000)
    
    matlab_plot = plots.error_plot(lmnet,show=False)
    matlab_plot.set_title('GradientDescent Training Performance [' + strippedsymbol + ']')
    matlab_plot_fig = matlab_plot.get_figure()
    matlab_plot_fig.savefig(figurename,bbox_inches = "tight")
    matlab_plot_fig.clf()
    
    #cla()   # Clear axis
    #clf()   # Clear figure
    #close() # Close a figure window
    
    # Prediction Model... 
    predict_input_data_open = np.asarray(df.iloc[dayoffset:,1])    #Take everything, including today
    predict_input_data_high =  np.asarray(df.iloc[dayoffset:,2])   #Take everything, including today
    predict_input_data_low =  np.asarray(df.iloc[dayoffset:,3])    #Take everything, including today
    predict_input_data_close =  np.asarray(df.iloc[dayoffset:,4])  #Take everything, including today
    predict_input_data_volume =  np.asarray(df.iloc[dayoffset:,5]) #Take everything, including today
    
    predict_input_data_collection = np.asarray([predict_input_data_open,predict_input_data_high,predict_input_data_low,predict_input_data_close,predict_input_data_volume])
    predict_input_data_collection_trans =  predict_input_data_collection.transpose()
    
    predict_data = predict_input_data_collection_trans
    predict_data_scaler = preprocessing.MinMaxScaler()
    predict_data_fitted = predict_data_scaler.fit_transform(predict_data)
    
    output_predicted = lmnet.predict(predict_data_fitted)
    output_predicted_unscaled = target_scaler.inverse_transform(output_predicted)
    
    t1 = time.time()
    
    output_predicted_u_clean = str(output_predicted_unscaled[0]).replace("[","").replace("]","")
    
    margin = str(output_predicted_unscaled[0]/predict_input_data_close[0])
    margin_clean = margin.replace("[","").replace("]","")
    
    resultsobj.write(strippedsymbol)
    resultsobj.write("\t")
    resultsobj.write(str(dayoffset))
    resultsobj.write("\t")
    resultsobj.write(output_predicted_u_clean)
    resultsobj.write("\t")
    resultsobj.write(margin_clean)
    resultsobj.write("\n")
    
    print(output_predicted_unscaled[0])
    print(output_predicted_unscaled[0]/predict_input_data_close[0])
    
    print(t1-t0)

#analyze_stock("MSFT.txt")