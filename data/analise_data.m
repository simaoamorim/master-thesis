function analise_data(filename)
    T = readtable(filename,'HeaderLines',3);
    t = table2array(T);
    x = t(:,2);
    y1 = t(:,3);
    y2 = t(:,4);

    plot(x,y1);
    hold on
    plot(x,y2);
    
    t = x(1:500);
    Y = y2(1:500);
    
    stepinfo(Y,t,'SettlingTimeThreshold',0.04)
end