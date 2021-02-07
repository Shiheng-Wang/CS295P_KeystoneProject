from lxml import etree
import os
import pandas as pd
import numpy as np
import pymysql

def norm_name(A):
    if ('-' in A):
        index = A.index('-')
        #A = A[index+1:]+A[0:index]
        A = A[index+1:]
    return A

def dataclearing(A):
    #print(A)
    if (' ' in A):
        A=A.replace(' ','')
    if ('\xa0' in A):
        A=A.replace('\xa0','')
    if ('\n' in A):
        A=A.replace('\n','')
    if (len(A)>1):
        if (A[-1]==' '):
            A=A[:-1]
    if ('/' in A):
        A=A.replace('/','')
    return A

def get_key_name(path,key_value):
    if (os.path.getsize(path)<16497):
        return key_value
    dom = etree.parse(path, etree.HTMLParser())
    keys = dom.xpath('//tr/td[@width="36%"]')
    for key in keys:
        key = norm_name(dataclearing(key.text))
        if (key not in key_value):
            key_value.append(key)
    return key_value

def init():
    filePath='E:/UCI Staff/274/2001/10/profiles/Yahoo/US/01/p/'
    file_name = os.listdir(filePath)
    html_name=[]
    data = []
    k_value=[]
    for name in file_name:
        if (name!='tse'):
            key_name=os.listdir(filePath+name)
            for key in key_name:
                html_name.append(filePath+name+'/'+key)
    for key in html_name:
        if (key[-10:]!='index.html'):
            k_value=get_key_name(key,k_value)
    k_value[k_value.index( 'Return')]='ReturnAssets'
    k_value.append('ReturnEquity')
    return k_value

def key_value_process(dom):
    keys = dom.xpath('//tr/td[@width="36%"]')
    key_value = []
    for key in keys:
        key_value.append(norm_name(dataclearing(key.text)))
    #key_value.append('Company_name')
    if ('Earnings' in key_value):
        key_value[key_value.index( 'Earnings')]='Earningsttm'
    if ('WeekChange' in key_value):
        key_value[key_value.index( 'WeekChange')]='WeekChangeWSP'
    #     if ('WeekChange52' in key_value):
    #         key_value[key_value.index( 'WeekChange52')]='WeekChangeWSP'
    #     if ('WeekChange26' in key_value):
    #         key_value[key_value.index( 'WeekChange26')]='WeekChangeWSP'
    #     if ('WeekChange13' in key_value):
    #         key_value[key_value.index( 'WeekChange13')]='WeekChangeWSP'
    #     if ('WeekChange4' in key_value):
    #         key_value[key_value.index( 'WeekChange4')]='WeekChangeWSP'
    if ('TermDebtEquity' in key_value):
        key_value[key_value.index('TermDebtEquity')]='TermDebtEquityLong'
    if ('Return' in key_value):
        key_value[key_value.index( 'Return')]='ReturnAssets'
    if ('Return' in key_value):
        key_value[key_value.index( 'Return')]='ReturnEquity'
    if ('DailyVolume' in key_value):
        key_value[key_value.index( 'DailyVolume')]='DailyVolume3'
    if ('DailyVolume' in key_value):
        key_value[key_value.index( 'DailyVolume')]='DailyVolume10'
    if ('SharesShort' in key_value):
        key_value[key_value.index( 'SharesShort')]='SharesShortPD'
    if ('Float' in key_value):
        key_value[key_value.index( 'Float')]='FL'
    if ('Sales' in key_value):
        key_value[key_value.index( 'Sales')]='Salesttm'
    if ('Mostrecentquarter' in key_value):
        key_value[key_value.index('Mostrecentquarter')]='MostrecentquarterF'
    key_value.append('Company')
    return key_value

def value_process(dom):
    value = []
    index=0
    values = dom.xpath('//td[@width="12%"][@align="right"]//text()')
    for key in values:
        key=dataclearing(key)
        if (key==''):
            continue
        if ((key[0]=='$') | (key[0]=='%')):
            continue
        if (index==1):
            key='-'+key
            index=0
        if ((len(key)==1) & (key[0]=='-')):
            index=1
            continue
        value.append("'"+dataclearing(key)+"'")
    return value

def datapipline(path):
    if (os.path.getsize(path)<16497):
        return []
    dom = etree.parse(path, etree.HTMLParser())
    key_value = key_value_process(dom)
    value = value_process(dom)
    value.append("'"+path[path.rfind('/')+1:path.rfind('.')]+"'")
    #print(key_value)
    if (len(key_value)!=len(value)):
        print('error:')
        print('The key_len: '+ str(len(key_value))+ ' The value_len: '+ str(len(value)))
    str1=",".join(key_value)
    str2=",".join(value)
    return [str1,str2]

def connDB():
    conn= pymysql.connect(host='localhost', user='root',passwd='0000',db='mysql',charset='utf8')
    cur=conn.cursor()
    return (conn,cur)

def exeUpdate(conn,cur,sql):
    sta=cur.execute(sql)
    conn.commit()
    return sta

def exeQuery(cur,sql):
    cur.execute(sql)
    result=cur.fetchone()
    return result

def connClose(conn,cur):
    cur.close()
    conn.close()
    
def main():
    ConnDB1=connDB()
    filePath='E:/UCI Staff/274/2001/10/profiles/Yahoo/US/01/p/'
    file_name = os.listdir(filePath)
    html_name=[]
    data = []
    for name in file_name:
        if (name!='tse'):
            key_name=os.listdir(filePath+name)
            for key in key_name:
                html_name.append(filePath+name+'/'+key)
    for key in html_name:
        if (key[-10:]!='index.html'):
            data = datapipline(key)
            print(data)
            if (len(data)==0):
                continue
                print('空data')
            sql="insert into yahoo("+data[0]+') values('+data[1]+');'
            print(exeUpdate(ConnDB1[0],ConnDB1[1],sql))
    connClose(ConnDB1[0],ConnDB1[1])
