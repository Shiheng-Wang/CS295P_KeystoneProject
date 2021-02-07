import pandas as pd
import numpy as np

path = 'E:/UCI Staff/274/2001/10/profiles/10months.csv'
data = pd.read_csv(path)

def dataclear(test):
    test= test.str.replace(',',"").str.replace('K','0000').str.replace('M','0000').str.replace('B','00000000').str.replace('.',"")
    test= test.str.replace('%',"").str.replace('none','0').str.replace('NA','0')
    return test.astype(float)

def col(df):
    dt = df.dtypes
    for i in range(39):
        if (dt[i] == 'object'):
            df.iloc[:, i] = dataclear(df.iloc[:, i])
        df.iloc[:, i] = df.iloc[:, i].fillna(0)
    return df

def main():
    categorical_variable = df.loc[:, ['MostrecentquarterF', 'Mostrecentquarter', 'Company', 'FiscalYearEnds']]
    df = data.loc[~np.isnan(data.WeekLow), :].drop(
        ['MostrecentquarterF', 'Mostrecentquarter', 'Company', 'FiscalYearEnds', 'id'], axis=1).fillna(0)]
    df = col(df)

main()