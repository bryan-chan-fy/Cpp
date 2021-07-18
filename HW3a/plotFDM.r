# Plot Implicit Finite Difference pu,pm,pd for different volatilities
#library(plot3D)
#library(reshape2)
#library(plotly)

setwd("C:/Users/bryan79/Documents/C++ Files/FiniteDM/")
impFDM_DataSet=read.csv("impFDM_Data.csv",header=TRUE) # read CSV file data
# different maturity differet curve
Sigma=impFDM_DataSet$Sigma

pu_imp=impFDM_DataSet$pu_imp
pm_imp=impFDM_DataSet$pm_imp
pd_imp=impFDM_DataSet$pd_imp

plot(Sigma,pu_imp,col="green",lty="dotted", type="l", ylim=range(c(pu_imp,pm_imp,pd_imp)),main="Plot of pu,pm,pd vs Sigma")
lines(Sigma,pm_imp,col="red",lty="dashed", type="l")
lines(Sigma,pd_imp,col="blue",lty="dashed", type="l")
legend("topright",c("pu","pm","pd"),fill=c("green","red","blue"))
