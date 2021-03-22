import pandas as pd
import matplotlib.pyplot as plt

file_pd_not_opt=pd.read_json (r'C:\Users\Dell\Documents\GitHub\SistemiDigitaliProgetto\CNN_project\history_200_no.json')
file_pd_opt=pd.read_json (r'C:\Users\Dell\Documents\GitHub\SistemiDigitaliProgetto\CNN_project\history200.json')
print(file_pd_not_opt.head())

x_epochs=range(1,201)
first_dim=file_pd_not_opt['accuracy']
f_dim=file_pd_opt['accuracy']
second=file_pd_not_opt['loss']
s_dim=file_pd_opt['loss']
plt.figure(figsize=(32,20))
plt.subplot(211)
plt.plot(x_epochs,first_dim, '-r',label="not_optimized", linewidth=3)
plt.plot(x_epochs,f_dim, '-g',label="optimized", linewidth=3)
plt.xlabel('number_of_epochs')
plt.ylabel('Accuracy')
plt.title("Accuracy and loss trends for 200 Epochs\n Not optimized in red and the optimized in green ", fontsize = 18)
plt.subplot(212)
plt.plot(x_epochs,second, '-r',label="not_optimized", linewidth=3)
plt.plot(x_epochs,s_dim, '-g',label="optimized", linewidth=3)
plt.xlabel('number_of_epochs')
plt.ylabel('Loss')
plt.show();
